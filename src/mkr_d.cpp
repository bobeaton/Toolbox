// mkr_d.cpp  Implementation of the marker dialog box

#include "stdafx.h"
#include "toolbox.h"
#include "mkr_d.h"
#include "not.h"  // 1995-05-17 MRP
#include "ddx.h"
#include "typ.h"
#include "project.h"
#include "CRngSt_d.h"  //09-17-1997

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CMarkerGeneralPage::CMarkerGeneralPage(CMarker* pmkr, CMarkerSet* pmkrset, CFontDef& fntdef, CMarker*& pmkrOverThis,
					   CMarker*& pmkrFollowingThis, CLangEncPtr& plng, Str8& sMarker, Str8& sFieldName,
                       BOOL& bUseDefaultFont, BOOL& bCharStyle, Str8& sDescription, BOOL bSetFocusToMarkerEditBox,
					   BOOL bMarkerNameReadOnly) :
    CPropertyPage(IDD),
    m_pmkrset(pmkrset),
    m_fntdef(fntdef),
    m_pmkrOverThis(pmkrOverThis),
    m_pmkrFollowingThis(pmkrFollowingThis),
    m_plng(plng),
    m_bCharStyle(bCharStyle),
    m_cbolng(pmkrset->plngset()),
    m_cbomkrOverThis(pmkrset, pmkr), // Okay for pmkr to be NULL
    m_cbomkrFollowingThis(pmkrset, TRUE, pmkrset->ptyp()->pmkrRecord()),
    m_sMarker(sMarker),
    m_sFieldName(sFieldName),
    m_bUseDefaultFont(bUseDefaultFont),
	m_iStyle(m_bCharStyle ? eCharStyle : eParStyle ),
    m_sDescription(sDescription),
    m_bSetFocusToFieldNameEditBox(!bSetFocusToMarkerEditBox),
    m_bMarkerNameReadOnly(bMarkerNameReadOnly)
{
    ASSERT( m_pmkrset );
}

void CMarkerGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CMarkerGeneralPage)
    DDX_Control(pDX, IDC_MKRFONT, m_btnFont);
    DDX_Control(pDX, IDC_USEDEFAULTFONT, m_chkUseDefaultFont);
    DDX_Control(pDX, IDC_DLG_NOT, m_edtNote);
    DDX_Control(pDX, IDC_MKR_FIELD_NAME, m_edtFieldName);
    DDX_Control(pDX, IDC_MKR_MARKER, m_edtMarker);
    DDX_Control(pDX, IDC_MKR_LNGSET_COMBO, m_cbolng);
    DDX_Control(pDX, IDC_MKR_cboUnder, m_cbomkrOverThis);
    DDX_Control(pDX, IDC_MKR_cboFollowing, m_cbomkrFollowingThis);
    DDX_Check(pDX, IDC_USEDEFAULTFONT, m_bUseDefaultFont);
    DDX_Radio(pDX, IDC_mkr_optCharStyle, m_iStyle);
//    DDX_Txt(pDX, IDC_MKR_DESCRIPTION, m_sDescription);
    //}}AFX_DATA_MAP

//    m_pmkrset->DDX_Name(pDX, IDC_MKR_MARKER, m_sMarker); // 1.4hdk  Replace DDX_Name in marker general page dialog
//    Shw_DDX_TrimmedTxt(pDX, IDC_MKR_FIELD_NAME, m_sFieldName); // 1.4gw Replace DDX_Trimmed in marker Field Name

    if ( pDX->m_bSaveAndValidate )
        {
        m_pmkrOverThis = m_cbomkrOverThis.pmkrSelected();
        m_pmkrFollowingThis = m_cbomkrFollowingThis.pmkrSelected();
	    m_bCharStyle = ( m_iStyle == eCharStyle );
        }
}

BEGIN_MESSAGE_MAP(CMarkerGeneralPage, CPropertyPage)
    //{{AFX_MSG_MAP(CMarkerGeneralPage)
//2000-07-06 TLB - Use of OnSetFocus (which never got called by the framework) replaced by OnSetActive
//    ON_WM_SETFOCUS()
    ON_BN_CLICKED(IDC_MKRFONT, OnChooseFont)
    ON_BN_CLICKED(IDC_USEDEFAULTFONT, OnUseDefaultFont)
    ON_BN_CLICKED(IDC_MKR_LNG, OnLanguageProperties)
    ON_CBN_SELCHANGE(IDC_MKR_LNGSET_COMBO, OnSelchangeLangEncCombo)
    ON_EN_SETFOCUS(IDC_MKR_MARKER, OnSetFocusMarker)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMarkerGeneralPage message handlers

BOOL CMarkerGeneralPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    SetDlgItemText( IDC_MKR_MARKER, swUTF16( m_sMarker ) ); // 1.4hdk // 1.4qpv
    SetDlgItemText( IDC_MKR_DESCRIPTION, swUTF16( m_sDescription ) ); // 1.4qpv
    SetDlgItemText( IDC_MKR_FIELD_NAME, swUTF16( m_sFieldName ) ); // 1.4gw // 1.4qpv
    
    m_edtMarker.SetReadOnly(m_bMarkerNameReadOnly);  // 1996-10-16 MRP    
    m_cbolng.UpdateElements(m_plng);
    m_cbomkrOverThis.UpdateElements(m_pmkrOverThis);
    m_cbomkrFollowingThis.UpdateElements(m_pmkrFollowingThis);

    m_btnFont.EnableWindow( !m_bUseDefaultFont ); // disable Choose Font... button if using language font
   
    return TRUE;  // return FALSE if we set the focus
}

void CMarkerGeneralPage::GetDlgText() // 1.4gx Fix bug (1.4dw) of losing marker description
{
//    GetDlgItemText( IDC_MKR_MARKER, (char*)buffer, DLGBUFMAX ); // 1.4hdk
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_MKR_MARKER, sDlgItem ); // 1.4qpw
	m_sMarker = sUTF8( sDlgItem ); // 1.4qpw
	RemoveBackslashAtFront( m_sMarker ); // 1.4hdk
//    GetDlgItemText( IDC_MKR_DESCRIPTION, (char*)buffer, DLGBUFMAX ); // 1.4gx
	GetDlgItemText( IDC_MKR_DESCRIPTION, sDlgItem ); // 1.4qpw
	m_sDescription = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_MKR_FIELD_NAME, (char*)buffer, DLGBUFMAX ); // 1.4gw
	GetDlgItemText( IDC_MKR_FIELD_NAME, sDlgItem ); // 1.4qpw
	m_sFieldName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sFieldName ); // 1.4hdc
}

// When this functionality was moved here, I decided that it would be best if the tab order were to start
// with the marker, then the name, etc. Starting with the name seemed reasonable since it is most commonly the first
// control to get focus, but that makes the marker go to the end of the order. Thus, when attempting to tab from the
// marker to the name, the user gets an unwelcome surprise.
void CMarkerGeneralPage::OnSetFocusMarker()
{
    // Since OnSetFocus never gets called by the framework for a page and since setting the focus
    // in OnSetActive doesn't seem to have any effect (presumably because the framework sets it
    // to the Name box after that, we'll catch it here and redirect the focus to the Marker box
    // when we're adding a new (unnamed) marker.
    if ( m_bSetFocusToFieldNameEditBox )
        {
        // This is ugly, but all the nicely wrappered methods use SendMessage, which does it right away. We need to
        // post the message so it won't get handled until after the framework does its default action.
        PostMessage(WM_NEXTDLGCTL, (WPARAM)m_edtFieldName.m_hWnd, 1L);
        m_bSetFocusToFieldNameEditBox = FALSE;
        }

}

void CMarkerGeneralPage::ReportError(CNote* pnot)
{
    ASSERT( pnot );
    CEdit* pedt = NULL;
    if ( pnot->bRefersToString(m_sMarker) )
        pedt = &m_edtMarker;
    else if ( pnot->bRefersToString(m_sFieldName) )
        pedt = &m_edtFieldName;
        
    // Set the insertion point, or select text referred to in note
    ASSERT( pedt );
    pedt->SetSel(pnot->dwSelReferent());
    pedt->SetFocus();
        
    // Show the note on the dialog itself
    m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
    m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
    MessageBeep(0);
}

void CMarkerGeneralPage::OnSelchangeLangEncCombo()
{
    m_plng = m_cbolng.plngSelected();
}

void CMarkerGeneralPage::OnLanguageProperties()
{
    BOOL b = m_plng->bViewProperties();
    // Used to do UpdateRangeSetKeyboardAndFont
}

void CMarkerGeneralPage::OnUseDefaultFont()
{
    m_bUseDefaultFont = ( m_chkUseDefaultFont.GetCheck() == 1 );
    
    if ( !m_bUseDefaultFont )
        {
        // When Use Default Font is unchecked, initialize the properties of
        // this marker's font to those of its language's default font.
        m_fntdef.Set(m_plng->pfnt(), m_plng->rgbColor());
        }
    m_btnFont.EnableWindow( !m_bUseDefaultFont ); // disable Choose Font... button if using language font
}

void CMarkerGeneralPage::OnChooseFont()
{
    ASSERT( !m_bUseDefaultFont ); // BJY 4/8/96 Now disabling Choose Font... button if default is checked
#ifdef BJY_4_8_96
    // If Use Default Font is still checked, initialize the properties of
    // this marker's font to those of its language's default font.
    if ( m_bUseDefaultFont )
        m_fntdef.Set(m_plng->pfnt(), m_plng->rgbColor());
#endif
    if ( m_fntdef.bModalView_Properties( TRUE ) ) // let user change font
        {
#ifdef BJY_4_8_96
        m_bUseDefaultFont = FALSE;
        m_chkUseDefaultFont.SetCheck(0);
#endif
        }
}

const CFont* CMarkerGeneralPage::pfnt()
{
    if (m_bUseDefaultFont)
        {
        return NULL;
        }

    HGDIOBJ hFont = m_fnt.Detach();

    // Note: In almost all cases, if the specific marker font is too big for the controls, we just use the
    // CtrlFont of the language (see CMarker::pfntDlg()), but in the case of the Marker Properties sheet,
    // we create a special control font if necessary so that the range set will display using a downsized
    // version of the marker font if necessary.
#ifdef TLB_07_18_2000_FONT_HANDLING_BROKEN
//    int iHt = m_fntdef.plf()->lfHeight; // create a font limited to an appropriate size for dialog controls
//    if ( -(iHt+(iHt/3)) > Shw_pProject()->iLangCtrlHeight() )
//        m_fntdef.plf()->lfHeight = min( -(iHt+(iHt/3)), Shw_pProject()->iLangCtrlHeight() );
//    m_fntdef.iCreateFont( &m_fnt ); // create the font
//    m_fntdef.plf()->lfHeight = iHt; // restore correct height
//
#else

    CFontDef fntdefTemp = m_fntdef;
    m_fntdef.iCreateFont( &m_fnt ); // create a font based on current marker logical font

    // Test to see if resulting font is too big
    CDC dc;
    dc.CreateCompatibleDC( NULL ); // Create a temporary screen device context for measuring font
    CFont* pfntOrig = dc.SelectObject( &m_fnt ); // select font into device context
    TEXTMETRIC tm;
    BOOL bGotTextMetrics = dc.GetTextMetrics( &tm );
    ASSERT( bGotTextMetrics );
    if ( tm.tmHeight > Shw_pProject()->iLangCtrlHeight() )
        {
        fntdefTemp.plf()->lfHeight = Shw_pProject()->iLangCtrlHeight();
#ifdef _MAC
        fntdefTemp.iCreateFontMakeFit( &m_fnt ); // create a smaller font
#else
        fntdefTemp.iCreateFont( &m_fnt ); // create a smaller font
#endif
        }
#endif

    if ( hFont )
        ::DeleteObject(hFont);

	return &m_fnt;
}

/////////////////////////////////////////////////////////////////////////////

CDataPropertiesPage::CDataPropertiesPage(BOOL& bMultipleItemData, BOOL& bMultipleWordItems,
                                         BOOL& bMustHaveData, BOOL& bNoWordWrap) :
    CPropertyPage(IDD),
    m_bMultipleItemData(bMultipleItemData),
    m_bMultipleWordItems(bMultipleWordItems),
    m_bMustHaveData(bMustHaveData),
    m_bNoWordWrap(bNoWordWrap)
{
    m_iDataType = (bMultipleWordItems) ? eMultiwordItem : ((bMultipleItemData) ? eMultipleWords : eSingleWord );
    m_bAllowEmptyContents = !bMustHaveData;
}

void CDataPropertiesPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CDataPropertiesPage)
    DDX_Radio(pDX, IDC_optSingleWord, m_iDataType);
    DDX_Check(pDX, IDC_chkAllowEmptyContents, m_bAllowEmptyContents);
    DDX_Check(pDX, IDC_chkNoWordWrap, m_bNoWordWrap);
    if ( pDX->m_bSaveAndValidate )
        {
        m_bMultipleItemData = ( m_iDataType == eMultipleWords );
        m_bMultipleWordItems = ( m_iDataType == eMultiwordItem );
        m_bMustHaveData = !m_bAllowEmptyContents;
        }
    //}}AFX_DATA_MAP
}

// **************************************************************************
// Add... button
CMarkerPropertiesSheet::CMarkerPropertiesSheet( CMarkerSet* pmkrset, CMarker** ppmkrNew, const char* pszMarker ) :
    CPropertySheet(_T(" "), NULL, pagGeneral), // 1.4quw Upgrade CPropertySheet for Unicode build // 1.4sb
    m_pmkr(NULL),
    m_ppmkrNew(ppmkrNew),
    m_pmkrset(pmkrset),
    m_fntdef(m_pmkrset->plngDefault()->pfnt(), m_pmkrset->plngDefault()->rgbColor()),
    m_pmkrOverThis(m_pmkrset->pmkrRecord()),
    m_pmkrFollowingThis(NULL),
    m_plng(m_pmkrset->plngDefault()),
    m_sMarker(pszMarker),
    m_bUseDefaultFont(TRUE),
    m_bCharStyle(TRUE), // 1.1kb Change default of new marker to char style instead of paragraph style
    m_bUseRangeSet(FALSE),
    m_bMultipleItemData(TRUE),
    m_bMultipleWordItems(FALSE),
    m_bMustHaveData(FALSE),
    m_bNoWordWrap(FALSE),
    m_dlgMarkerProperties(NULL, m_pmkrset, m_fntdef, m_pmkrOverThis, m_pmkrFollowingThis, m_plng, m_sMarker,
                          m_sFieldName, m_bUseDefaultFont, m_bCharStyle, m_sDescription, m_sMarker.IsEmpty(), FALSE),
    m_dlgRangeSet(m_bUseRangeSet, m_datset, m_bMultipleItemData, m_bMultipleWordItems),
    m_dlgDataProperties(m_bMultipleItemData, m_bMultipleWordItems, m_bMustHaveData, m_bNoWordWrap)
{
    ASSERT(m_pmkrset);
    ASSERT( m_ppmkrNew );

	Str8 sTitle = "Marker Properties:"; // 1.5.0fg 
	sTitle = sTitle + " " + "New Marker"; // 1.5.0fg 
    SetTitle( swUTF16( sTitle  ) ); // 1.4quv Upgrade SetTitle for Unicode build
    AddPages();
}

// Copy... button
CMarkerPropertiesSheet::CMarkerPropertiesSheet( const CMarker* pmkrToCopyFrom, CMarker** ppmkrNew ) :
    CPropertySheet(_T(" "), NULL, pagGeneral), // 1.4quw Upgrade CPropertySheet for Unicode build // 1.4sb
    m_pmkr(NULL),
    m_ppmkrNew(ppmkrNew),
    m_pmkrset(pmkrToCopyFrom->pmkrsetMyOwner()),
    m_fntdef(pmkrToCopyFrom->pfnt(), pmkrToCopyFrom->rgbColor()),
    m_pmkrOverThis(pmkrToCopyFrom->pmkrOverThis()),
    m_pmkrFollowingThis(pmkrToCopyFrom->pmkrFollowingThis()),
    m_plng(pmkrToCopyFrom->plng()),
    m_sMarker(pmkrToCopyFrom->sMarker()),
    m_sFieldName(pmkrToCopyFrom->sFieldName()),
    m_bUseDefaultFont(pmkrToCopyFrom->bUsingDefaultFont()),
    m_bCharStyle(pmkrToCopyFrom->bCharStyle()),
    m_bUseRangeSet(pmkrToCopyFrom->bUseRangeSet()),
    m_datset(pmkrToCopyFrom->prngset()),
    m_sDescription(pmkrToCopyFrom->sDescription()),
    m_bMultipleItemData(pmkrToCopyFrom->bMultipleItemData()),
    m_bMultipleWordItems(pmkrToCopyFrom->bMultipleWordItems()),
    m_bMustHaveData(!pmkrToCopyFrom->bAllowEmptyContents()),
    m_bNoWordWrap(pmkrToCopyFrom->bNoWordWrap()),
    m_dlgMarkerProperties(NULL, m_pmkrset, m_fntdef, m_pmkrOverThis, m_pmkrFollowingThis, m_plng, m_sMarker,
                          m_sFieldName, m_bUseDefaultFont, m_bCharStyle, m_sDescription, TRUE, FALSE),
    m_dlgRangeSet(m_bUseRangeSet, m_datset, m_bMultipleItemData, m_bMultipleWordItems),
    m_dlgDataProperties(m_bMultipleItemData, m_bMultipleWordItems, m_bMustHaveData, m_bNoWordWrap)
{
    ASSERT(m_pmkrset);
    ASSERT( m_ppmkrNew );
    AddPages();

	Str8 sTitle = "Marker Properties:"; // 1.5.0fg 
	sTitle = sTitle + " " + "New Marker"; // 1.5.0fg 
    SetTitle( swUTF16( sTitle  ) ); // 1.4quv Upgrade SetTitle for Unicode build
}

// Modify... button
CMarkerPropertiesSheet::CMarkerPropertiesSheet( CMarker* pmkr, EPage pagActive,
                            BOOL bDisableMarkerChange, BOOL bDisableRangeSetRemove) :
    CPropertySheet(_T(" "), NULL, pagActive), // 1.4quw Upgrade CPropertySheet for Unicode build // 1.4sb
    m_pmkr(pmkr),
    m_ppmkrNew(NULL),
    m_pmkrset(m_pmkr->pmkrsetMyOwner()),
    m_sOriginalMarker(m_pmkr->sMarker()),
    m_fntdef(m_pmkr->pfnt(), m_pmkr->rgbColor()),
    m_pmkrOverThis(m_pmkr->pmkrOverThis()),
    m_pmkrFollowingThis(m_pmkr->pmkrFollowingThis()),
    m_plng(m_pmkr->plng()),
    m_sMarker(m_pmkr->sMarker()),
    m_sFieldName(m_pmkr->sFieldName()),
    m_bUseDefaultFont(m_pmkr->bUsingDefaultFont()),
    m_bCharStyle(m_pmkr->bCharStyle()),
    m_bUseRangeSet(m_pmkr->bUseRangeSet()),
    m_datset(m_pmkr->prngset()),
    m_sDescription(m_pmkr->sDescription()),
    m_bMultipleItemData(m_pmkr->bMultipleItemData()),
    m_bMultipleWordItems(m_pmkr->bMultipleWordItems()),
    m_bMustHaveData(!m_pmkr->bAllowEmptyContents()),
    m_bNoWordWrap(m_pmkr->bNoWordWrap()),
    // The marker's name must be read-only if it has field references AND IF
    // any read-only documents depend on this marker's set's database type.
    m_dlgMarkerProperties(m_pmkr, m_pmkrset, m_fntdef, m_pmkrOverThis, m_pmkrFollowingThis, m_plng, m_sMarker,
                          m_sFieldName, m_bUseDefaultFont, m_bCharStyle, m_sDescription, FALSE,
					      ( (m_pmkr->bHasFieldRefs() && m_pmkrset->ptyp()->bHasRefsFromReadOnlyIndexSets()) ||
                            bDisableMarkerChange) ),
    m_dlgRangeSet(m_bUseRangeSet, m_datset, m_bMultipleItemData, m_bMultipleWordItems, m_pmkr, bDisableRangeSetRemove),
    m_dlgDataProperties(m_bMultipleItemData, m_bMultipleWordItems, m_bMustHaveData, m_bNoWordWrap)
{
    ASSERT(m_pmkr);
    ASSERT(m_pmkrset);
    AddPages();

	Str8 sTitle = "Marker Properties:"; // 1.5.0fg 
	sTitle = sTitle + " " + "\\" + m_pmkr->sName() + " " + m_pmkr->sFieldName(); // 1.5.0fg 
    SetTitle( swUTF16( sTitle  ) ); // 1.4quv Upgrade SetTitle for Unicode build
}

void CMarkerPropertiesSheet::AddPages()
{
    m_dlgMarkerProperties.SetHelpID(IDD_MKR_General_Page);
    AddPage(&m_dlgMarkerProperties);

    m_dlgRangeSet.SetHelpID(IDD_MKR_RangeSet_Page);
    m_dlgRangeSet.SetOwner(this);
    AddPage(&m_dlgRangeSet);

    m_dlgDataProperties.SetHelpID(IDD_MKR_DataProperties_Page);
    AddPage(&m_dlgDataProperties);
}

BEGIN_MESSAGE_MAP(CMarkerPropertiesSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CDatabaseTypeSheet)
    ON_COMMAND(IDOK, OnOK)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMarkerPropertiesSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    CButton *pBtn = (CButton*)GetDlgItem(ID_APPLY_NOW);
    ASSERT(pBtn);
    pBtn->ShowWindow(SW_HIDE); // make Apply button do the disappearing act

	SetButtonLabels();  // 1998-09-30 MRP: For French version

    return TRUE;
}

int CMarkerPropertiesSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
        return -1;
//	SetButtonLabels(); // 1.6.2b Try to make Help button disappear, didn't work
    return 0;
}

void CMarkerPropertiesSheet::SetActivePage(EPage pag)
{
    CPropertySheet::SetActivePage(pag); // switch to page
}

afx_msg void CMarkerPropertiesSheet::OnOK()
{
    GetActivePage()->OnKillActive();

	m_dlgMarkerProperties.GetDlgText(); // 1.4gx

    if ( !m_bMultipleWordItems )
        {
        // Check for range set items that have multiple words
        if ( m_datset.bHasMultiwordElements() )
            {
            SetActivePage(pagDataProp);
            AfxMessageBox(_("The data properties define data items to consist of single words, but the range set contains one or more multiple-word items."), MB_ICONINFORMATION);
            return;
            }
        }

    CNote* pnot = NULL;

    if ( !m_ppmkrNew && !bEqual(m_sMarker, m_sOriginalMarker) ) // Changed name of existing marker
        {
        CMarker* pmkrTo = m_pmkrset->pmkrSearch( m_sMarker );
        if ( pmkrTo ) // does another marker with this name exist?
            {
            SetActivePage(pagGeneral);
            Str8 sMessage;
            if ( pmkrTo == m_pmkrset->ptyp()->pmkrRecord() ||
                 m_pmkr == m_pmkrset->ptyp()->pmkrRecord() ) // can't merge record marker
                {
                m_sMarker = m_sOriginalMarker;  // restore original name
                m_dlgMarkerProperties.UpdateData(FALSE);  // from member data to dialog controls // 1.4kub May be needed
			    SetDlgItemText( IDC_MKR_FIELD_NAME, swUTF16( m_sFieldName ) ); // 1.4gw // 1.4qpv
                AfxMessageBox(_("Cannot merge marker"), MB_ICONINFORMATION); // 1.5.0fg 
                return;
                }
            
            if ( m_pmkr->bHasSettingsRefs() ) // can't do a merge if settings are referring to this marker // 1.4vj Fix bug of crash on change marker // 1.4vm Undo 1.4vj as too strong
                {
                m_sMarker = m_sOriginalMarker;  // restore original name
                m_dlgMarkerProperties.UpdateData(FALSE);  // from member data to dialog controls // 1.4kub May be needed
			    SetDlgItemText( IDC_MKR_FIELD_NAME, swUTF16( m_sFieldName ) ); // 1.4gw // 1.4qpv
                AfxMessageBox(_("Cannot merge marker"), MB_ICONINFORMATION);
                return;
                }

			sMessage = _("Merge markers?"); // 1.5.0fg 
            if ( AfxMessageBox(sMessage, MB_ICONQUESTION | MB_YESNO) == IDNO )
                {
                m_sMarker = m_sOriginalMarker;  // restore original name
                m_dlgMarkerProperties.UpdateData(FALSE);  // from member data to dialog controls // 1.4kub May be needed
			    SetDlgItemText( IDC_MKR_FIELD_NAME, swUTF16( m_sFieldName ) ); // 1.4gw // 1.4qpv
                }
            else // user wants to merge this marker with another one!
                {
                m_pmkrset->RemoveFromHierarchy( m_pmkr ); // don't want anyone under marker being merged
                CMarkerUpdate mup( pmkrTo, m_pmkr );
                extern void Shw_Update(CUpdate& up);
                Shw_Update(mup); // merge 'em
                ASSERT( !m_pmkr->bHasRefs() );
                m_pmkrset->Delete( &m_pmkr ); // delete the marker now that it's merged
                // 1997-12-15 MRP: Emergency correction for problem with merging markers
                // via Modify from the Markers tab in Database Type Properties.
                // See lbo.cpp, mkr_d.cpp, mkrset_d.h, mkrset_d.cpp
                extern CSetEl* g_pselMarkerToBeSelected;
                g_pselMarkerToBeSelected = m_pmkr;
                EndDialog(IDOK);
                }
            return;
            }
        // Confirm modification to the name of an existing marker
        // since it will change everywhere it occurs.
        Str8 sMessage;
        int i = AfxMessageBox( _("Replace marker everywhere?"), MB_ICONQUESTION | MB_YESNOCANCEL); // 1.5.0fg 
        if ( i == IDNO )
            {
            m_sMarker = m_sOriginalMarker;  // restore original name
            m_dlgMarkerProperties.UpdateData(FALSE);  // from member data to dialog controls // 1.4kub May be needed
			SetDlgItemText( IDC_MKR_FIELD_NAME, swUTF16( m_sFieldName ) ); // 1.4gw // 1.4qpv
            SetActivePage(pagGeneral);
            return;
            }
        else if ( i == IDCANCEL )
            {
            EndDialog(IDCANCEL);
            return;
            }
        }
    
    // !!! RNE 1996-10-23  Reflect addition of description and sound.
    // Do this when the dialog can handle it.
    // 08-18-1997 Changed parameterlist-> added m_iRangeSetCondition + m_bRangeSetSpaceAllowed
    BOOL bOK = ( m_pmkr ?
        m_pmkr->bModifyProperties(m_sMarker, m_sDescription, m_sFieldName,
            m_plng, m_pmkrOverThis, m_pmkrFollowingThis,
            //m_sRangeSet, m_iRangeSetCharacter, m_bRangeSetSpaceAllowed,
            m_bUseDefaultFont ? NULL : &m_fntdef,
            m_bMultipleItemData, m_bMultipleWordItems, m_bMustHaveData,
            m_bNoWordWrap, m_bCharStyle, &pnot) :
            // User edited an existing marker
        m_pmkrset->bAddMarker(m_sMarker, m_sDescription, m_sFieldName,
            m_plng, m_pmkrOverThis, m_pmkrFollowingThis,
            //m_sRangeSet, m_iRangeSetCharacter, m_bRangeSetSpaceAllowed,
            m_bUseDefaultFont ? NULL : &m_fntdef,
            m_bMultipleItemData, m_bMultipleWordItems, m_bMustHaveData,
            m_bNoWordWrap, m_bCharStyle, &m_pmkr, &pnot)
            // User added a new marker
        );        
    if ( bOK )
        {
        ASSERT( m_pmkr != NULL );
        ASSERT( pnot == NULL );
        
        // Attach the range set, if any.
        if ( m_bUseRangeSet )
            {
            m_pmkr->TakeRangeSet(m_datset); // This will leave pdatset empty
            }
        else
            m_pmkr->DiscardRangeSet();

        if ( m_ppmkrNew )
            *m_ppmkrNew = m_pmkr;  // Set the marker
        EndDialog(IDOK);
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the marker. The marker object itself has not been changed.
        SetActivePage(pagGeneral);
        m_dlgMarkerProperties.ReportError( pnot );
        delete pnot;
        }
}

void CMarkerPropertiesSheet::SetButtonLabels()
{
	// 1998-09-30 MRP: For French version
	CButton *pbtn = NULL;
	Str8 sLabel;
    
    pbtn = (CButton*)GetDlgItem(IDOK);
    ASSERT( pbtn );
    sLabel = _("OK");
	pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
    
    pbtn = (CButton*)GetDlgItem(IDCANCEL);
    ASSERT( pbtn );
    sLabel = _("Cancel");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy

    pbtn = (CButton*)GetDlgItem(IDHELP);
    ASSERT( pbtn );
//	pbtn->SetButtonStyle(BS_OWNERDRAW); // 1.6.2b Didn't make button disappear
//	pbtn->EnableWindow(FALSE); // 1.6.2b 
//  pbtn->ShowWindow(SW_HIDE); // 1.6.2b 
    sLabel = _("Help");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
}

