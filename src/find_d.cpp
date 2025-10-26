// Find_d.cpp  Implementation of find dialog box  (1995-05-24)

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
// #include "mainfrm.h"
#include "shwdoc.h"
#include "shwview.h"
#include "find_d.h"
#include "mkr.h"  // CMarker, CMString
#include "font.h"
#include "project.h"
#include "obstream.h"  // Object_istream, Object_ostream

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFindText

static const char* psz_text = "text";
static const char* psz_lng = "lng";
static const char* psz_ftx = "ftx";

#ifdef prjWritefstream // 1.6.4aa 
void CFindText::WriteProperties(Object_ofstream& obs) const 
#else
void CFindText::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteBeginMarker(psz_ftx);
    obs.WriteString(psz_text, m_sText);
    obs.WriteString(psz_lng, m_plng->sName());
    obs.WriteEndMarker(psz_ftx);
}

BOOL CFindText::s_bReadProperties(Object_istream& obs, CFindText** ppftx)
{
    if ( !obs.bReadBeginMarker(psz_ftx) )
        return FALSE;

    Str8 sLangEnc;
    CLangEnc* plng = NULL;
    Str8 sText;
    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadString(psz_text, sText) )
            ;
        else if ( obs.bReadString(psz_lng, sLangEnc) )
            plng = Shw_plngset()->plngSearch(sLangEnc); // try to find it in language encoding set
        else if ( obs.bEnd(psz_ftx) )
            break;
        }

    if ( plng ) // don't add item to list if it's language doesn't exist
        *ppftx = new CFindText(sText, plng);
    return TRUE;            
}


/////////////////////////////////////////////////////////////////////////////
// CFindTextList

void CFindTextList::DeletingLanguage( CLangEnc* plng ) // notification of language being destroyed - called from language encoding destructor
{
    CFindText* pftx = pftxFirst();
    for ( ; pftx; ) // loop through previous find text list
        {
        if ( pftx->plng() == plng ) // was this item using the language being deleted?
            {
            Delete( &pftx ); // delete the item from the list
            if ( !pftx ) // just deleted last item in list
                break;
            }
        else
            pftx = pftxNext( pftx ); // advance thru list
        }

#ifdef ToolbarFindCombo // 1.4yq
    Shw_pcboFind()->UpdateElements(); // update find combo immediately (don't wait for idle loop to update it)
#endif // ToolbarFindCombo // 1.4yq
}

static const char* psz_ftxlst = "ftxlst";

#ifdef prjWritefstream // 1.6.4aa 
void CFindTextList::WriteProperties(Object_ofstream& obs) const
#else
void CFindTextList::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_ftxlst);
    
    for ( CFindText* pftx = pftxLast(); pftx; pftx = pftxPrev(pftx) )
        pftx->WriteProperties(obs);
        
    obs.WriteEndMarker(psz_ftxlst);
}

BOOL CFindTextList::bReadProperties(Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_ftxlst) )
        return FALSE;

    while ( !obs.bAtEnd() )
        {
        CFindText* pftx = NULL;
        if ( CFindText::s_bReadProperties(obs, &pftx) )
            {
            if ( pftx )
                Add(pftx);
            }
        else if ( obs.bEnd(psz_ftxlst) )
            break;
        }
        
    return TRUE;
}

void CFindTextList::Add(const char* pszText, CLangEnc* plng) // Add latest find text to list
{
    if ( !pszText || !*pszText ) // don't add empty string
        return;

    if ( !bIsEmpty() )
        if ( !strcmp( pszText, pftxFirst()->pszText() ) // same as first item in list?
                && plng == pftxFirst()->plng() )
            return; // don't add it if not necessary

    Add( new CFindText( pszText, plng ) );
}

void CFindTextList::Add( CFindText* pftx )
{
    CFindText* pftxExisting = pftxSearch( pftx->pszText() ); // search for previous occurence of this text

    if ( pftxExisting ) // if this text exists in list, delete previous occurence
        Delete( pftxExisting );

    if ( bIsEmpty() )
        CDblList::Add( pftx );
    else
        InsertBefore( pftxFirst(), pftx ); // add to front of list

    if ( lGetCount() > 20 )
        Delete( pftxLast() ); // delete last in list if at limit
}

CFindText* CFindTextList::pftxSearch( const char* pszText ) // search list for exact match of text
{
    CFindText* pftx = pftxFirst();
    for ( ; pftx; pftx = pftxNext( pftx ) ) // ramble through entire list
        if ( !strcmp( pszText, pftx->pszText() ) )
            break;
    return pftx; // return matching element or NULL if no matches
}

/////////////////////////////////////////////////////////////////////////////
// CFindSettings

CFindSettings::CFindSettings()
{
    m_bMatchWhole = FALSE;
    m_bSingleRecord = FALSE;
    m_bSingleField = FALSE;
    m_matsetFind = CMCharOrder::matsetSamePrimaryGroup;
    m_matsetCur = CMCharOrder::matsetSamePrimaryGroup;
    m_bReplace = FALSE;
    m_bReplaceAll = FALSE;
}

CLangEnc* CFindSettings::plng( CShwView* pview ) // return language used for last find
{
	if ( !pview && m_plng ) // 1.4zaj If no view given and language is stored, return it
		return m_plng; // 1.4zaj 
    if ( pview && bSingleField() ) // searching specified field
        {
        CMarker* pmkr = pview->ptyp()->pmkrset()->pmkrSearch( pszMkr() ); // try to find in view's marker set
        if ( pmkr )
            return pmkr->plng();
        }
    else if ( !pftxlst()->bIsEmpty() )
        return pftxlst()->pftxFirst()->plng(); // get language from last find

    // when closing the project, we oughtn't return a language set which is ultimately going 
    //  away (this causes fatal exception when the CFindLangEdit is getting OnPaint message
    //  after having set its m_plng to this value which is subsequently trashed (but without
    //  clobbering the reference... boom!) // rde 52t
    else if( Shw_papp()->bGetClosingProjectOrMainFrame() )
        return  0;

    return Shw_plngset()->plngDefault(); // return default language from set
}

static const char* psz_find = "Find";
static const char* psz_MatchWhole = "MatchWhole";
static const char* psz_SingleRecord = "SingleRecord";
static const char* psz_SingleField = "SingleField";
static const char* psz_Field = "Field";
static const char* psz_Replace = "Replace";

#ifdef prjWritefstream // 1.6.4aa 
void CFindSettings::WriteProperties(Object_ofstream& obs) const
#else
void CFindSettings::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_find);
    obs.WriteBool(psz_MatchWhole, m_bMatchWhole);
    obs.WriteBool(psz_SingleRecord, m_bSingleRecord);
    obs.WriteBool(psz_SingleField, m_bSingleField);
    if ( m_sMkr.GetLength() )
        obs.WriteString(psz_Field, m_sMkr);

    // 1995-09-06 MRP: Need to encapsulate this during revision of matching
    const char* pszProperty;
    if ( m_matsetFind == CMCharOrder::matsetSamePrimaryGroup )
        pszProperty = "p";
    else if ( m_matsetFind == CMCharOrder::matsetExactDisregardCase )
        pszProperty = "c";
    else if ( m_matsetFind == CMCharOrder::matsetExact )
        pszProperty = "e";
    else if ( m_matsetFind == CMCharOrder::matsetEvenIgnore )
        pszProperty = "i";
    else
        ASSERT( FALSE );
    obs.WriteString("match_char", pszProperty);
    m_ftxlst.WriteProperties(obs);
    obs.WriteString(psz_Replace, m_sReplaceText);
    
    obs.WriteEndMarker(psz_find);
}

BOOL CFindSettings::bReadProperties(Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_find) )
        return FALSE;

    Str8 sMatchChar;
    m_bMatchWhole = FALSE;
    m_bSingleRecord = FALSE;
    m_bSingleField = FALSE;
    m_sReplaceText = "";

    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadBool(psz_MatchWhole, m_bMatchWhole) )
            ;
        else if ( obs.bReadBool(psz_SingleRecord, m_bSingleRecord) )
            ;
        else if ( obs.bReadBool(psz_SingleField, m_bSingleField) )
            ;
        else if ( obs.bReadString(psz_Field, m_sMkr) )
            ;
        else if ( obs.bReadString("match_char", sMatchChar) )
            {
            if ( bEqual(sMatchChar, "p") )
                m_matsetFind = CMCharOrder::matsetSamePrimaryGroup;
            else if ( bEqual(sMatchChar, "c") )
                m_matsetFind = CMCharOrder::matsetExactDisregardCase;
            else if ( bEqual(sMatchChar, "e") )
                m_matsetFind = CMCharOrder::matsetExact;
            else if ( bEqual(sMatchChar, "i") )
                m_matsetFind = CMCharOrder::matsetEvenIgnore;
            m_matsetCur = m_matsetFind;
            }
        else if ( m_ftxlst.bReadProperties(obs) )
            ;
        else if ( obs.bReadString(psz_Replace, m_sReplaceText) )
            ;
        else if ( obs.bEnd(psz_find) )
            break;
        }
        
    return TRUE;
}

void CFindSettings::Set( MatchSetting matset, BOOL bMatchWhole, BOOL bSingleField,
                        BOOL bSingleRecord, const char* pszMkr, const char* pszReplaceText,
                        BOOL bReplace, BOOL bReplaceAll )
{
    m_matsetCur = matset;
    if ( !bReplace )
        m_matsetFind = matset;
    m_bMatchWhole = bMatchWhole;
    m_bSingleField = bSingleField;
    m_bSingleRecord = bSingleRecord;
    m_sMkr = pszMkr;
    m_sReplaceText = pszReplaceText;
    m_bReplace = bReplace;
    m_bReplaceAll = bReplaceAll;
}

BOOL CFindSettings::bModalView_Properties( CDatabaseType* ptyp, const char* pszText, BOOL bIsReplace ) // bring up Find dialog
{
    CFindDlg dlg( this, ptyp, pszText, bIsReplace );
    return dlg.DoModal() == IDOK;
}

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog


CFindDlg::CFindDlg(CFindSettings* pfndset, CDatabaseType* ptyp, const char* pszText, BOOL bIsReplace) :
        CDialog(bIsReplace ? IDD_REPLACE : IDD_FIND),
        m_cboField( ptyp->pmkrset(), FALSE, NULL ),
        m_cboLangEnc( Shw_plngset() )
{
    m_bIsReplace = bIsReplace;
    m_pfndset = pfndset;
    ASSERT( pfndset );
    m_ptyp = ptyp;
    ASSERT( ptyp );
	m_bSelectedText = ( pszText != NULL ); // 1.4zaj Remember whether this find is from selected text
    
    if ( pszText )
        m_sText = pszText; // use selected text
    else if ( !m_pfndset->pftxlst()->bIsEmpty() ) // use text from last search
        m_sText = m_pfndset->pftxlst()->pftxFirst()->pszText();

    m_sReplace = m_pfndset->pszReplaceText();

    // 1999-08-13 MRP: Initializing Match Characters for Find and Replace.
    // Find: the user's previous choice (by default, the most general match).
    // Replace: the most specific match.
    if ( m_bIsReplace )
        m_matset = CMCharOrder::matsetEvenIgnore;
    else
        m_matset = m_pfndset->matsetFind();

    // 1995-05-24 MRP: need to encapsulate this better!!!   
    if ( m_matset == CMCharOrder::matsetSamePrimaryGroup )
        m_iMatchSetting = 0;  // index of radio button in group
    else if ( m_matset == CMCharOrder::matsetExactDisregardCase )
        m_iMatchSetting = 1;
    else if ( m_matset == CMCharOrder::matsetExact )
        m_iMatchSetting = 2;
    else if ( m_matset == CMCharOrder::matsetEvenIgnore )
        m_iMatchSetting = 3;
    else
        ASSERT( FALSE );
        
    //{{AFX_DATA_INIT(CFindDlg)
    m_bMatchWhole = m_pfndset->bMatchWhole();
    m_bNotSingleRecord = !m_pfndset->bSingleRecord(); // logic is reversed for radio buttons
    m_bNotSingleField = !m_pfndset->bSingleField();
    //}}AFX_DATA_INIT
}


void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_FIND_TEXT, m_elcText); // 1.4kza Put back in to fix but of not showing font
//    DDX_Txt(pDX, IDC_FIND_TEXT, m_sText);
    if ( m_bIsReplace )
        {
        DDX_Control(pDX, IDC_REPLACE_TEXT, m_elcReplace); // 1.4kza Put back in to fix but of not showing font
//        DDX_Txt(pDX, IDC_REPLACE_TEXT, m_sReplace);
        }
    //{{AFX_DATA_MAP(CFindDlg)
    DDX_Control(pDX, IDC_FIND_LNGCOMBO, m_cboLangEnc);
    DDX_Control(pDX, IDC_FIND_MKRCOMBO, m_cboField);
    DDX_Check(pDX, IDC_FIND_WHOLE_WORD, m_bMatchWhole);
    DDX_Radio(pDX, IDC_FIND_MATCH_SAME_PRIMARY, m_iMatchSetting);
    DDX_Radio(pDX, IDC_FIND_CURRENTREC, m_bNotSingleRecord);
    DDX_Radio(pDX, IDC_FIND_IN_FLD, m_bNotSingleField);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
    //{{AFX_MSG_MAP(CFindDlg)
    ON_CBN_SELCHANGE(IDC_FIND_LNGCOMBO, OnSelchangeFindLngcombo)
    ON_CBN_SELCHANGE(IDC_FIND_MKRCOMBO, OnSelchangeFindMkrcombo)
    ON_BN_CLICKED(IDC_FIND_IN_FLD, OnFindInFld)
    ON_BN_CLICKED(IDC_FIND_IN_LNG, OnFindInLng)
    ON_BN_CLICKED(IDC_REPLACE_ALL, OnReplaceAll)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFindDlg message handlers

BOOL CFindDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	m_elcText.SetEditLngText( m_sText ); // 1.4qpn
	m_elcText.SetSel( 0, m_sText.GetLength() ); // 1.4vya 
	if ( m_bIsReplace ) // 1.4vxt Fix problem (1.4vxr) of crash on find dialog box
		{
		m_elcReplace.SetEditLngText( m_sReplace ); // 1.4qpn
		m_elcReplace.SetSel( 0, m_sReplace.GetLength() ); // 1.4vya 
		}

    CMarker* pmkr = m_ptyp->pmkrset()->pmkrSearch( m_pfndset->pszMkr() );
    if ( !pmkr )
        pmkr = m_ptyp->pmkrset()->pmkrFirst(); // set it to something!
    m_cboField.UpdateElements( pmkr );
                                         
    CLangEnc* plng = NULL;
	if ( m_bSelectedText ) // 1.4zaj If find from selected text, set language encoding to language of selection
		{
		plng = m_pfndset->plng(); // 1.4zaj Get language of the field
//		m_pfndset->SetSingleField( FALSE ); // 1.4zaj Clear single field so as to search all fields of language // 1.5.8c Fix bug of losing specific field on cancel of find dialog box
		}
	else
		{
		CFindText* pftx = m_pfndset->pftxlst()->pftxFirst(); // get language from last find
		if ( pftx )
			plng = pftx->plng();
		}
    if ( !plng )
        plng = Shw_plngset()->plngDefault(); // use default language if first find
    m_cboLangEnc.UpdateElements( plng );

    if ( m_bNotSingleField )
        {
        m_elcText.SetLangEnc( plng );
        if ( m_bIsReplace )
            m_elcReplace.SetLangEnc( plng );
        m_cboField.EnableWindow( FALSE ); // disable marker combo if single field radio button not selected
        }
    else
        {
        m_elcText.SetLangEnc( pmkr->plng() ); // 1.4vxk 
        if ( m_bIsReplace )
            m_elcReplace.SetLangEnc( pmkr->plng() ); // 1.4vxk 
        m_cboLangEnc.EnableWindow( FALSE ); // disable language combo if "all fields of language" radio button not selected
        }

    return TRUE;  // return TRUE unless you set the focus to a control
}

void CFindDlg::OnOK()
{
	DoIt(FALSE);
}

void CFindDlg::OnReplaceAll()
{
    DoIt(TRUE);
}

void CFindDlg::DoIt( BOOL bReplaceAll )
{
    UpdateData(TRUE);  // from dialog controls to member variables

	m_sText = m_elcText.sGetEditLngText(); // 1.4qpn
    if ( m_bIsReplace ) // 1.4vxt 
		m_sReplace = m_elcReplace.sGetEditLngText(); // 1.4qpn

    // 1995-05-24 MRP: need to encapsulate this better!!!   
    if ( m_iMatchSetting == 0 )  // index of radio button in group
        m_matset = CMCharOrder::matsetSamePrimaryGroup;
    else if ( m_iMatchSetting == 1 )
        m_matset = CMCharOrder::matsetExactDisregardCase;
    else if ( m_iMatchSetting == 2 )
        m_matset = CMCharOrder::matsetExact;
    else if ( m_iMatchSetting == 3 )
        m_matset = CMCharOrder::matsetEvenIgnore;
    else
        ASSERT( FALSE );

    CMarker* pmkr = m_cboField.pmkrSelected(); // get field selection from marker combo
    m_pfndset->Set( m_matset, m_bMatchWhole, !m_bNotSingleField, !m_bNotSingleRecord,
                    pmkr->sName(), m_sReplace, m_bIsReplace, bReplaceAll );
    
    ASSERT( m_cboField.pmkrSelected() ); // these combos should always have a valid selection
    ASSERT( m_cboLangEnc.plngSelected() );
    CLangEncPtr plngPtr;
    if ( !m_bNotSingleField ) // searching specified field
        plngPtr = m_cboField.pmkrSelected()->plng();
    else
        plngPtr = m_cboLangEnc.plngSelected(); // searching all fields of this language

    if ( m_sText.IsEmpty() ) // change nothing to [...] variable
        {
        m_sText = Shw_pszLMeta;
        m_sText += "...";
        m_sText += Shw_pszRMeta;
        }
    m_pfndset->pftxlst()->Add( m_sText, plngPtr ); // add to list of previous finds
    CLangEnc* plng = plngPtr; // get a copy of pointer
    plngPtr = NULL; // this can cause an unused language to destruct, removing item just added from the list
    if ( m_pfndset->pftxlst()->bIsEmpty() ||
        m_pfndset->pftxlst()->pftxFirst()->plng() != plng ) // if language did destruct...
        {
        MessageBeep(0); // make find fail
        CDialog::OnCancel();
        }
    else
        CDialog::OnOK();                  
}

void CFindDlg::OnSelchangeFindLngcombo()
{
    m_elcText.SetLangEnc( m_cboLangEnc.plngSelected() ); // this shouldn't happen unless 'all fields of language' radio button selected
    if ( m_bIsReplace )
        m_elcReplace.SetLangEnc( m_cboLangEnc.plngSelected() ); // this shouldn't happen unless 'all fields of language' radio button selected
}

void CFindDlg::OnSelchangeFindMkrcombo()
{
    m_elcText.SetLangEnc( m_cboField.pmkrSelected()->plng() ); // this shouldn't happen unless single field radio button selected // 1.4vxk 
    if ( m_bIsReplace )
        m_elcReplace.SetLangEnc( m_cboField.pmkrSelected()->plng() ); // this shouldn't happen unless single field radio button selected // 1.4vxk 
}

void CFindDlg::OnFindInFld()
{
    m_cboLangEnc.EnableWindow(FALSE);
    m_cboField.EnableWindow(TRUE);
    OnSelchangeFindMkrcombo();
}

void CFindDlg::OnFindInLng()
{
    m_cboLangEnc.EnableWindow(TRUE);
    m_cboField.EnableWindow(FALSE);
    OnSelchangeFindLngcombo();
}

/////////////////////////////////////////////////////////////////////////////
// CFindComboBox

// 2000-08-29 TLB & MRP : This object is never constructed with anything but an empty list.
// CFindComboBox::CFindComboBox(CFindTextList* pftxlst)
CFindComboBox::CFindComboBox() :
    m_plng(NULL)
{
    // 2000-08-29 TLB & MRP : to avoid accessing freed memory, access pftxlst via project.
    // m_pftxlst = pftxlst ? pftxlst : &m_ftxlstEmpty;
}

// 2000-08-29 TLB & MRP : to avoid accessing freed memory, access pftxlst via project.
/*
void CFindComboBox::ChangeSet(CFindTextList* pftxlst)
{
    m_pftxlst = pftxlst ? pftxlst : &m_ftxlstEmpty;
    UpdateElements();
}
*/

CFindTextList* CFindComboBox::pftxlst()
{
    if ( Shw_papp()->bProjectOpen() && !Shw_pProject()->bClosing() )
        {
        return Shw_pProject()->pftxlst();
        }
    else
        return &m_ftxlstEmpty;
}

void CFindComboBox::UpdateElements()
{
    ResetContent();  // remove any items currently in the list box

    if ( !Shw_papp()->bProjectOpen() || Shw_pProject()->bClosing() )
        return;

    // Insert the names of the elements in the list in the list box.
    CFindText* pftx = pftxlst()->pftxFirst();
    for ( ; pftx != NULL; pftx = pftxlst()->pftxNext(pftx) )
        {
        // Insert the item at the end of the list box.
        int iInserted = InsertString(-1, swUTF16( pftx->pszText() ) ); // 1.4qre
            // Unlike AddString(), InsertString() does not cause
            // a list with the LBS_SORT style to be sorted.
        ASSERT( iInserted != LB_ERR );
        (void) SetItemDataPtr(iInserted, (void*)pftx);
        }

    CLangEnc* plng = Shw_pProject()->pfndset()->plng(); // get language for edit control
//    ASSERT( plng ); // rde 52t // plng can be 0 if project is being closed. It is OK to send 0 to SetLangEnc below
    const char* pszText = "";
    if ( ( !plng || plng->bUnicodeLang() ) && !pftxlst()->bIsEmpty() ) // 1.4ym 
        pszText = pftxlst()->pftxFirst()->pszText();
    m_elcFind.SetLangEnc( plng ); // set edit control to language of selected item
    m_elcFind.SetEditLngText( pszText ); // set toolbar combo contents to text just entered // 1.4qpy // 1.4ym 
    SetLangEnc( plng );
}

// note that this can return NULL if nothing is selected
CFindText* CFindComboBox::pftxSelected()
{
    int idx = GetCurSel();
    return idx != CB_ERR ? (CFindText*)GetItemDataPtr(idx) : NULL; // either the combo box is empty, or no item is selected
}

void CFindComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    COLORREF crBack;
    COLORREF crText;
    if ((lpDrawItemStruct->itemState) & ODS_SELECTED)
        {
        // Highlight if selected.
        crBack = GetSysColor(COLOR_HIGHLIGHT);
        crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
        }
    else
        {
        crBack = GetSysColor(COLOR_WINDOW);
        crText = GetSysColor(COLOR_WINDOWTEXT);
        }

    CDC cDC;  // context with which to draw the item
    cDC.Attach(lpDrawItemStruct->hDC);
    const RECT& rcItem = lpDrawItemStruct->rcItem;  // rectangle containing it
	CRect rectItem( rcItem );
    CBrush  brsSelected;
    brsSelected.CreateSolidBrush(crBack);
    cDC.FillRect(&rcItem, &brsSelected);        
    cDC.SetBkColor(crBack);
    cDC.SetTextColor(crText);

    if ( lpDrawItemStruct->itemID != -1 ) // happens when a listbox is empty - used to draw empty focus rect
        {
        CFindText* pftx = (CFindText*)lpDrawItemStruct->itemData;  // the element
        ASSERT( pftx != NULL );
        int iItemHt = rcItem.bottom - rcItem.top;
        CFont fnt;
		if ( pftx->plng() ) // 5.96x
			{
			const char* pszTemp = pftx->pszText();
			if ( pszTemp && pszTemp != (const char*)0xfeeefeee && *pszTemp != 0 ) // 5.96x Protect against crash on arrow down and Enter
				{
				if ( pftx->plng()->iLineHeight() > iItemHt ) // do we need a trimmed version of language font?
					{
					CFontDef fntdef( pftx->plng()->pfnt() );
					fntdef.plf()->lfHeight = iItemHt;
		#ifdef _MAC
					fntdef.iCreateFontMakeFit( &fnt ); // create a smaller font
		#else
					fntdef.iCreateFont( &fnt ); // create a smaller font
		#endif
					cDC.SelectObject( &fnt ); // select new font into device context
					}
				else
					cDC.SelectObject( (CFont*)pftx->plng()->pfnt() );
				pftx->plng()->ExtTextOutLng( &cDC, rcItem.left, rcItem.top, ETO_CLIPPED, 
						&rectItem, pftx->pszText(), strlen( pftx->pszText() ), NULL ); // 5.96x
				}
			}

//        cDC.ExtTextOut(rcItem.left, rcItem.top, ETO_CLIPPED,
//                        &rcItem, pftx->pszText(), strlen(pftx->pszText()), NULL);
        }

#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif        
    cDC.Detach();
}

BOOL CFindComboBox::bHasChanged(CFindTextList* pftxlst) // see if list matches what's in the combo listbox
{
    if ( GetCount() != pftxlst->lGetCount() ) // same number of elements?
        return TRUE;
        
    if ( pftxlst->bIsEmpty() )
        {
        if ( !m_plng ) // language hasn't been set yet
            return TRUE;
        else
            return FALSE;
        }
    if ( pftxlst->pftxFirst() != (CFindText*)GetItemDataPtr(0) ) // first elements match?
        return TRUE;
    
    return FALSE; // currently no way to change elements further down list without changing first element
}

void CFindComboBox::LangEncUpdated(CLangEncUpdate& lup)
{
    if ( !lup.bModified() ) // Only care about font change via modify
        return;
    if ( m_plng == lup.plng() ) // is edit control using language just modified?
        SetLangEnc( m_plng ); // make sure new font is used in combo's edit control
}

void CFindComboBox::Init() // subclass combo's edit control and set edit control's initial language
{
    CWnd* pEditWnd = GetWindow( GW_CHILD );
    m_elcFind.SubclassWindow( pEditWnd->GetSafeHwnd() );
    CRect rect;
    m_elcFind.GetClientRect( &rect ); // get dimensions of edit control
    m_iHeight = rect.bottom; // remember normal edit control height
#ifdef _MAC
    m_iHeight -= 3; // need to trim a little - full height doesn't seem to be totally visible
#else
    // this is no longer relevant
    //DWORD dwVersion = GetVersion(); // see if we're running 3.1
    //if ( LOBYTE(LOWORD(dwVersion)) == 3 && HIBYTE(LOWORD(dwVersion)) < 95 ) // 3.1, NT 3.51 requires more padding at top and bottom
    //    m_iHeight -= 8;
#endif // MAC
    SetItemHeight( 0, m_iHeight ); // set height of listbox items to same as edit control height
}

void CFindComboBox::SetLangEnc(CLangEnc* plng) // set language of child edit control
{
	if ( plng ) // 1.4ma Fix bug (1.4m) of assert in SetLangEnc on close, note at find_d line 562 says zero here is OK
		{
//		m_elcFind.SetLangEnc( plng ); // 5.96x
		m_plng = plng;
		}
    if ( plng && (m_iHeight < plng->iLineHeight()) ) // need to create a smaller font to fit find combo?
        { // rde 52t do this only if plng is non-zero
        CFontDef fntdef( plng->pfnt() );
        fntdef.plf()->lfHeight = m_iHeight;
#ifdef _MAC
        fntdef.iCreateFontMakeFit( &m_fnt ); // create a smaller font
#else
        fntdef.iCreateFont( &m_fnt ); // create a smaller font
#endif
        m_elcFind.SetFont( &m_fnt ); // select new font into device context
        }
}

void CFindComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
}

/////////////////////////////////////////////////////////////////////////////
// CFindLangEdit

static WNDPROC NEAR pfnSuper = NULL;

WNDPROC* CFindLangEdit::GetSuperWndProcAddr()
{
    return &pfnSuper;
}

void CFindLangEdit::OnSetFocus(CWnd* pOldWnd) // 1.4ym 
	{
		CEdit::OnSetFocus( pOldWnd ); // 1.4ym 
	}

BEGIN_MESSAGE_MAP(CFindLangEdit, CEdit ) // 1.4qmc Change find CLngEditCtrl to CEditLng // 5.96x Change from CEdtLng // 1.4ym 
    //{{AFX_MSG_MAP(CFindLangEdit)
    ON_WM_CHAR()
    ON_WM_SETFOCUS()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Enter and Esc hits get routed either here or to the mainframe depending on the status
// of the combo at the time the keys are pressed.  If they are routed here we just notify
// the mainframe and handle it there.  They are handled in the mainframe's OnOK and OnCancel
// handler's.  See mainfrm.cpp
void CFindLangEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    CMDIFrameWnd* pwndMainFrame = (CMDIFrameWnd*)Shw_papp()->m_pMainWnd;
    if ( nChar == 27 ) // catch Esc hit
        pwndMainFrame->SendMessage( WM_COMMAND, IDCANCEL );
    else if ( nChar == '\r' ) // Enter initiates find
        pwndMainFrame->SendMessage( WM_COMMAND, IDOK );
    else
        CEdit::OnChar(nChar, nRepCnt, nFlags); // 1.4qmc Change find CLngEditCtrl to CEditLng // 1.4ym 
}

/////////////////////////////////////////////////////////////////////////////
// CAskReplaceDlg dialog

CAskReplaceDlg::CAskReplaceDlg(CShwView* pview, CPoint pntCaret)
    : CDialog(CAskReplaceDlg::IDD, NULL)
{
    m_pviewCaller = pview;
    m_pntCaret = pntCaret;
    //{{AFX_DATA_INIT(CAskReplaceDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CAskReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAskReplaceDlg)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAskReplaceDlg, CDialog)
    //{{AFX_MSG_MAP(CAskReplaceDlg)
    ON_BN_CLICKED(IDC_SKIP, OnSkip)
    ON_WM_SHOWWINDOW()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskReplaceDlg message handlers

void CAskReplaceDlg::OnSkip() 
{
    FindAgain( FALSE );
}

void CAskReplaceDlg::OnOK() 
{
    FindAgain( TRUE );
}

void CAskReplaceDlg::FindAgain( BOOL bDoReplace )
{
    CPoint pntCaret;
    if ( m_pviewCaller->bReplace( bDoReplace, &pntCaret ) )
        CheckPosition( pntCaret );
    else
        CDialog::OnOK();
}

void CAskReplaceDlg::CheckPosition( CPoint pntCaret )
{
    CRect rect;
    GetWindowRect( &rect ); // get position of dialog
    if ( rect.top - 40 < pntCaret.y && rect.bottom + 20 > pntCaret.y &&
         rect.left - 50 < pntCaret.x && rect.right + 50 > pntCaret.x )
        {
        if ( pntCaret.y - rect.Height() - 20 > 20 ) // have enough room above selection?
            SetWindowPos( NULL, rect.left, pntCaret.y - rect.Height() - 20,
                                0, 0, SWP_NOSIZE | SWP_NOZORDER );
        else // try to put dialog below selection
            SetWindowPos( NULL, rect.left, pntCaret.y + 50,
                                0, 0, SWP_NOSIZE | SWP_NOZORDER );
        }
}

void CAskReplaceDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
    CRect rect;
    GetWindowRect( &rect ); // get position of dialog
    CRect rectMainWnd;
    AfxGetApp()->m_pMainWnd->GetWindowRect( &rectMainWnd );
    // position on top of title bar and menu
    SetWindowPos( NULL, rect.left, rectMainWnd.top+25, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
    CheckPosition( m_pntCaret ); // make sure this position is okay
    CDialog::OnShowWindow(bShow, nStatus);
}
