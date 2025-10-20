// fil_d.cpp  Implementation of the filter dialog box

#include "stdafx.h"
#include "toolbox.h"
#include "fil_d.h"
#include "date.h"  // 1998-04-08 MRP: CDateCon
#include "not.h"
#include "typ.h"  // CDatabaseType::pmkrRecord()
#include "project.h"
#include "obstream.h"  // Object_istream, Object_ostream

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog

// --------------------------------------------------------------------------

// First, Last, and Insert buttons
CMarkerTextDlg::CMarkerTextDlg( CFilterElList* pfellst, CFilterEl** ppfelNew ) :
    CDialog(CMarkerTextDlg::IDD),
    m_pcon(NULL),
    m_ppfelNew(ppfelNew),
    m_pfellst(pfellst),
    m_pfilset(m_pfellst->pfilsetMyOwner()),
    m_pmkrset(m_pfilset->pmkrset()),
    m_pmkr(m_pmkrset->pmkrFirst()),
    m_cboMarker(m_pmkrset, FALSE, NULL),
    m_pvarset(m_pfilset->pvarset()),
    m_cboVariable(m_pvarset)
{
    ASSERT( m_ppfelNew );
    ASSERT( !*m_ppfelNew );
    ASSERT( m_pfilset );
    ASSERT( m_pmkrset );
    ASSERT( m_pvarset );
}

// Modify... button
CMarkerTextDlg::CMarkerTextDlg( CFilterConMarkerString* pcon ) :
    CDialog(CMarkerTextDlg::IDD),
    m_pcon(pcon),
    m_ppfelNew(NULL),
    m_pfellst(NULL),
    m_pfilset(pcon->pfilsetMyOwner()),
    m_sMarker(m_pcon->sMarker()),
    m_pmkrset(m_pfilset->pmkrset()),
    m_pmkr(m_pmkrset->pmkrSearch(m_sMarker)),
    m_cboMarker(m_pmkrset, FALSE, NULL),
    m_sText(m_pcon->sText()),
    m_pvarset(m_pfilset->pvarset()),
    m_cboVariable(m_pvarset)
{
    ASSERT( m_pcon );
    ASSERT( m_pfilset );
    ASSERT( m_pmkrset );
    ASSERT( m_pvarset );
}


void CMarkerTextDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_fil_cboMarker, m_cboMarker);
    DDX_Control(pDX, IDC_fil_edtText, m_elcText);
    DDX_Control(pDX, IDC_fil_cboVariable, m_cboVariable);
    DDX_Control(pDX, IDC_fil_edtNote, m_edtNote);
    //{{AFX_DATA_MAP(CMarkerTextDlg)
//    DDX_Txt(pDX, IDC_fil_edtText, m_sText);
    DDX_Control(pDX, IDC_fil_btnInsertVariable, m_btnInsertVariable);
    //}}AFX_DATA_MAP
}

BOOL CMarkerTextDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_cboMarker.UpdateElements(m_pmkr);
    if ( m_pmkr )
        m_elcText.SetLangEnc( m_pmkr->plng() ); // 1.4vxk 
    else
        m_elcText.SetLangEnc( m_pmkrset->plngDefault() );
	m_elcText.SetEditLngText( m_sText ); // 1.4qpn
	m_elcText.SetSel( 0, m_sText.GetLength() ); // 1.4vxw Fix U bug of not selecting text in filter dialog

    CVar* pvar = m_pvarset->pvarFirst();
    m_cboVariable.UpdateElements(pvar);
    m_btnInsertVariable.EnableWindow(pvar != NULL);
        
    // If adding a new filter let Name, the first control, have input focus,
    // but if editing an existing filter, set focus to the condition instead.   
    BOOL bLetWindowsSetFocusToName = ( m_pcon == NULL );  // adding new filter
    if ( !bLetWindowsSetFocusToName )
        m_elcText.SetFocus();
    
    return bLetWindowsSetFocusToName;
}

BEGIN_MESSAGE_MAP(CMarkerTextDlg, CDialog)
    //{{AFX_MSG_MAP(CMarkerTextDlg)
    ON_CBN_SELCHANGE(IDC_fil_cboMarker, OnSelchangeMarker)
    ON_BN_CLICKED(IDC_fil_btnInsertVariable, OnInsertVariable)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMarkerTextDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
	m_sText = m_elcText.sGetEditLngText(); // 1.4qpn

    CMarker* pmkr = m_cboMarker.pmkrSelected();
    if ( pmkr )
        m_sMarker = pmkr->sMarker();

    CNote* pnot = NULL;
    BOOL bOK = ( m_pcon ?
        m_pcon->bModifyProperties(m_sMarker, m_sText, &pnot) :
        m_pfellst->bNewMarkerText(m_sMarker, m_sText, m_ppfelNew, &pnot)
        );
    
    if ( bOK )
        {
        ASSERT( !pnot );
        CDialog::OnOK();
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the marker. The marker object itself has not been changed.
        ASSERT( pnot );
//        CEdit* pedt = NULL;
//        if ( pnot->bRefersToString(m_sText) )
//            pedt = &m_elcText;
            
        // Set the insertion point, or select text referred to in note
//      ASSERT( pedt );
//      pedt->SetSel(pnot->dwSelReferent());
//      pedt->SetFocus();
            
        // Show the note on the dialog itself
        m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
        m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
        MessageBeep(0);

        delete pnot;
        }
}

void CMarkerTextDlg::OnSelchangeMarker()
{
    m_pmkr = m_cboMarker.pmkrSelected();
    // 1998-06-13 MRP: On 32-bit Windows guard against a null pointer
    // when the marker has been deleted and the user clicks in the
    // empty box with the list of markers dropped down.
    // Note: Neither the Mac nor the 16-bit Windows versions
    // seem to call OnSelchangeMarker in this situation.
    if ( m_pmkr )
        m_elcText.SetLangEnc( m_pmkr->plng() ); // 1.4vxk 
    else
        m_elcText.SetLangEnc( m_pmkrset->plngDefault() );
}

void CMarkerTextDlg::OnInsertVariable()
{
    Str8 sText;
    CVar* pvar = m_cboVariable.pvarSelected();
    ASSERT( pvar );
    pvar->Text(sText);
	m_elcText.SetEditLngText( m_elcText.sGetEditLngText() + sText ); // 1.4vxr Fix problem of not inserting variable into marker text
//    m_elcText.ReplaceSel(sText); // 1.4vxr 
    m_elcText.SetFocus();
}


// --------------------------------------------------------------------------

// First, Last, and Insert buttons
CMarkerDateDlg::CMarkerDateDlg( CFilterElList* pfellst, CFilterEl** ppfelNew ) :
    CDialog(CMarkerDateDlg::IDD),
    m_pcon(NULL),
    m_ppfelNew(ppfelNew),
    m_pfellst(pfellst),
    m_pfilset(m_pfellst->pfilsetMyOwner()),
    m_pmkrset(m_pfilset->pmkrset()),
    m_pmkr(m_pmkrset->pmkrFirst()),
    m_cboMarker(m_pmkrset, FALSE, NULL),
    m_iDateRelation(CDateCon::eEqual)
{
    ASSERT( m_ppfelNew );
    ASSERT( !*m_ppfelNew );
    ASSERT( m_pfilset );
    ASSERT( m_pmkrset );
}

// Modify... button
CMarkerDateDlg::CMarkerDateDlg( CFilterConDate* pcon ) :
    CDialog(CMarkerDateDlg::IDD),
    m_pcon(pcon),
    m_ppfelNew(NULL),
    m_pfellst(NULL),
    m_pfilset(pcon->pfilsetMyOwner()),
    m_sMarker(m_pcon->sMarker()),
    m_pmkrset(m_pfilset->pmkrset()),
    m_pmkr(m_pmkrset->pmkrSearch(m_sMarker)),
    m_cboMarker(m_pmkrset, FALSE, NULL),
    m_sDate(m_pcon->sDate()),
    m_iDateRelation((int)m_pcon->iRelation())
{
    ASSERT( m_pcon );
    ASSERT( m_pfilset );
    ASSERT( m_pmkrset );
}


void CMarkerDateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_fil_cboMarker, m_cboMarker);
    DDX_Control(pDX, IDC_fil_edtDate, m_elcDate);
    DDX_Control(pDX, IDC_fil_edtNote, m_edtNote);
    //{{AFX_DATA_MAP(CMarkerDateDlg)
    DDX_Radio(pDX, IDC_fil_optBefore, m_iDateRelation);
//    DDX_Txt(pDX, IDC_fil_edtDate, m_sDate);
    //}}AFX_DATA_MAP
}

BOOL CMarkerDateDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
//    SetDlgItemTxt( IDC_fil_edtDate, m_sDate); // 1.4qpn
	m_elcDate.SetEditLngText( m_sDate ); // 1.4qpn

    m_cboMarker.UpdateElements(m_pmkr);
    if ( m_pmkr )
        m_elcDate.SetLangEnc( m_pmkr->plng() ); // 1.4vxk 
    else
        m_elcDate.SetLangEnc( m_pmkrset->plngDefault() );
        
    // If adding a new filter let Name, the first control, have input focus,
    // but if editing an existing filter, set focus to the condition instead.   
    BOOL bLetWindowsSetFocusToName = ( m_pcon == NULL );  // adding new filter
    if ( !bLetWindowsSetFocusToName )
        m_elcDate.SetFocus();
    
    return bLetWindowsSetFocusToName;
}

BEGIN_MESSAGE_MAP(CMarkerDateDlg, CDialog)
    //{{AFX_MSG_MAP(CMarkerDateDlg)
    ON_CBN_SELCHANGE(IDC_fil_cboMarker, OnSelchangeMarker)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMarkerDateDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
	m_sDate = m_elcDate.sGetEditLngText(); // 1.4qpn

    CMarker* pmkr = m_cboMarker.pmkrSelected();
    if ( pmkr )
        m_sMarker = pmkr->sMarker();

    CNote* pnot = NULL;
    BOOL bOK = ( m_pcon ?
        m_pcon->bModifyProperties(m_sMarker, m_iDateRelation, m_sDate, &pnot) :
        m_pfellst->bNewMarkerDate(m_sMarker, m_iDateRelation, m_sDate, m_ppfelNew, &pnot)
        );
    
    if ( bOK )
        {
        ASSERT( !pnot );
        CDialog::OnOK();
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the marker. The marker object itself has not been changed.
        ASSERT( pnot );
//        CEdit* pedt = NULL;
//        if ( pnot->bRefersToString(m_sDate) )
//            pedt = &m_elcDate;
            
        // Set the insertion point, or select text referred to in note
//      ASSERT( pedt );
//      pedt->SetSel(pnot->dwSelReferent());
//      pedt->SetFocus();
            
        // Show the note on the dialog itself
        m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
        m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
        MessageBeep(0);

        delete pnot;
        }
}

void CMarkerDateDlg::OnSelchangeMarker()
{
    m_pmkr = m_cboMarker.pmkrSelected();
    // 1998-06-13 MRP: On 32-bit Windows guard against a null pointer
    // when the marker has been deleted and the user clicks in the
    // empty box with the list of markers dropped down.
    // Note: Neither the Mac nor the 16-bit Windows versions
    // seem to call OnSelchangeMarker in this situation.
    if ( m_pmkr )
        m_elcDate.SetLangEnc( m_pmkr->plng() ); // 1.4vxk 
    else
        m_elcDate.SetLangEnc( m_pmkrset->plngDefault() );
}


// --------------------------------------------------------------------------

// First, Last, and Insert buttons
CLangEncTextDlg::CLangEncTextDlg( CFilterElList* pfellst, CFilterEl** ppfelNew ) :
    CDialog(CLangEncTextDlg::IDD),
    m_pcon(NULL),
    m_ppfelNew(ppfelNew),
    m_pfellst(pfellst),
    m_pfilset(m_pfellst->pfilsetMyOwner()),
    m_plngset(m_pfilset->plngset()),
    m_plng(m_pfilset->pmkrset()->plngDefault()),
    m_cboLangEnc(m_plngset, TRUE),
    m_pvarset(m_pfilset->pvarset()),
    m_cboVariable(m_pvarset)
{
    ASSERT( m_ppfelNew );
    ASSERT( !*m_ppfelNew );
    ASSERT( m_pfilset );
    ASSERT( m_plngset );
    ASSERT( m_pvarset );
}

// Modify... button
CLangEncTextDlg::CLangEncTextDlg( CFilterConLangEncString* pcon ) :
    CDialog(CLangEncTextDlg::IDD),
    m_pcon(pcon),
    m_ppfelNew(NULL),
    m_pfellst(NULL),
    m_pfilset(pcon->pfilsetMyOwner()),
    m_plngset(m_pfilset->plngset()),
    m_plng(m_pcon->plng()),
    m_cboLangEnc(m_plngset, TRUE),
    m_sText(m_pcon->sText()),
    m_pvarset(m_pfilset->pvarset()),
    m_cboVariable(m_pvarset)
{
    ASSERT( m_pcon );
    ASSERT( m_pfilset );
    ASSERT( m_plngset );
    ASSERT( m_pvarset );
}


void CLangEncTextDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_fil_cboLangEnc, m_cboLangEnc);
    DDX_Control(pDX, IDC_fil_edtText, m_elcText);
    DDX_Control(pDX, IDC_fil_cboVariable, m_cboVariable);
    DDX_Control(pDX, IDC_fil_edtNote, m_edtNote);
    //{{AFX_DATA_MAP(CLangEncTextDlg)
//    DDX_Txt(pDX, IDC_fil_edtText, m_sText);
    DDX_Control(pDX, IDC_fil_btnInsertVariable, m_btnInsertVariable);
    //}}AFX_DATA_MAP
}

BOOL CLangEncTextDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_cboLangEnc.UpdateElements(m_plng);
    CLangEnc* plng = ( m_plng ? m_plng : m_pfilset->pmkrset()->plngDefault() );
    m_elcText.SetLangEnc( plng );
	m_elcText.SetEditLngText( m_sText ); // 1.4qpn
	m_elcText.SetSel( 0, m_sText.GetLength() ); // 1.4vxw Fix U bug of not selecting text in filter dialog

    CVar* pvar = m_pvarset->pvarFirst();
    m_cboVariable.UpdateElements(pvar);
    m_btnInsertVariable.EnableWindow(pvar != NULL);
        
    // If adding a new filter let Name, the first control, have input focus,
    // but if editing an existing filter, set focus to the condition instead.   
    BOOL bLetWindowsSetFocusToName = ( m_pcon == NULL );  // adding new filter
    if ( !bLetWindowsSetFocusToName )
        m_elcText.SetFocus();
    
    return bLetWindowsSetFocusToName;
}

BEGIN_MESSAGE_MAP(CLangEncTextDlg, CDialog)
    //{{AFX_MSG_MAP(CLangEncTextDlg)
    ON_CBN_SELCHANGE(IDC_fil_cboLangEnc, OnSelchangeLangEnc)
    ON_BN_CLICKED(IDC_fil_btnInsertVariable, OnInsertVariable)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLangEncTextDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
	m_sText = m_elcText.sGetEditLngText(); // 1.4qpn

    CLangEnc* plng = m_cboLangEnc.plngSelected();

    CNote* pnot = NULL;
    BOOL bOK = ( m_pcon ?
        m_pcon->bModifyProperties(plng, m_sText, &pnot) :
        m_pfellst->bNewLangEncText(plng, m_sText, m_ppfelNew, &pnot)
        );
    
    if ( bOK )
        {
        ASSERT( !pnot );
        CDialog::OnOK();
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the marker. The marker object itself has not been changed.
        ASSERT( pnot );
//        CEdit* pedt = NULL;
//        if ( pnot->bRefersToString(m_sText) )
//            pedt = &m_elcText;
            
        // Set the insertion point, or select text referred to in note
//      ASSERT( pedt );
//      pedt->SetSel(pnot->dwSelReferent());
//      pedt->SetFocus();
            
        // Show the note on the dialog itself
        m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
        m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
        MessageBeep(0);

        delete pnot;
        }
}

void CLangEncTextDlg::OnSelchangeLangEnc()
{
    m_plng = m_cboLangEnc.plngSelected();
    CLangEnc* plng = ( m_plng ? m_plng : m_pfilset->pmkrset()->plngDefault() );
    m_elcText.SetLangEnc( plng );
}

void CLangEncTextDlg::OnInsertVariable()
{
    Str8 sText;
    CVar* pvar = m_cboVariable.pvarSelected();
    ASSERT( pvar );
    pvar->Text(sText);
	m_elcText.SetEditLngText( m_elcText.sGetEditLngText() + sText ); // 1.4vxr Fix problem of not inserting variable into marker text
//    m_elcText.ReplaceSel(sText); // 1.4vxr 
    m_elcText.SetFocus();
}


// --------------------------------------------------------------------------

CFilterElList::CFilterElList(CFilterSet* pfilsetMyOwner) :
    m_pfilsetMyOwner(pfilsetMyOwner)
{
    ASSERT( m_pfilsetMyOwner );
}

CFilterElList::CFilterElList(const CFilter* pfil) :
    m_pfilsetMyOwner(pfil->pfilsetMyOwner())
{
    pfil->CopyToList(this);
}

void CFilterElList::Insert(CFilterEl* pfelCur, CFilterEl* pfelNew)
{
    if ( pfelCur )
        InsertAfter(pfelCur, pfelNew);
    else
        {
        CFilterEl* pfel = pfelFirst();
        if ( pfel )
            InsertBefore(pfel, pfelNew);
        else
            Add(pfelNew);
        }
}

static const char* psz_mkr = "mkr";
static const char* psz_mkrRecord = "mkrRecord";
static const char* psz_dat = "dat";
static const char* psz_rel = "rel";
static const char* psz_lng = "lng";
static const char* psz_lngAny = "lngAny";
static const char* psz_txt = "txt";
static const char* psz_fel = "fel";

static const char* s_pszAnd = "And";
static const char* s_pszOr = "Or";
static const char* s_pszNot = "Not";
static const char* s_pszLParen = "(";
static const char* s_pszRParen = ")";
static const char* s_pszWith = "With";
static const char* s_pszNonUnique = "NonUnique";
static const char* s_pszNon_Unique = "Non-Unique";

BOOL CFilterElList::bReadProperties(Object_istream& obs)
{
    BOOL bRead = FALSE;
    while ( !obs.bAtEnd() )
        {
        CFilterEl* pfel = NULL;
        if ( CFilterEl::s_bReadProperties(obs, m_pfilsetMyOwner, &pfel) )
            {
            if ( pfel )
                Add(pfel);
            bRead = TRUE;
            }
        else
            break;
        }
        
    return bRead;
}

BOOL CFilterElList::bMatchAt(const CFilterEl** ppfel, const char* pszElement) const
{
    ASSERT( ppfel );
    const CFilterEl* pfel = *ppfel;
    if ( !pfel )
        return FALSE;
        
    ASSERT( pszElement );
    ASSERT( *pszElement );
    if ( stricmp(pfel->sInternalElement(), pszElement) )
        return FALSE;
        
    *ppfel = pfelNext(pfel);
    return TRUE;
}

BOOL CFilterElList::bNewMarkerText(CFilterEl** ppfelNew)
{
    CMarkerTextDlg dlg(this, ppfelNew);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilterElList::bNewMarkerText(const char* pszMarker,
        const char* pszText, CFilterEl** ppfelNew, CNote** ppnot)
{
    CFilterCon* pcon = NULL;
    if ( !CFilterConMarkerString::s_bConstruct(pszMarker, pszText,
            m_pfilsetMyOwner, &pcon, ppnot) )
        return FALSE;

    CFilterEl* pfelNew = new CFilterEl(pcon);
    ASSERT( ppfelNew );
    *ppfelNew = pfelNew;
        
    return TRUE;
}

BOOL CFilterElList::bNewMarkerDate(CFilterEl** ppfelNew)
{
    CMarkerDateDlg dlg(this, ppfelNew);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilterElList::bNewMarkerDate(const char* pszMarker,
        int iRelation, const Str8& sDate,
        CFilterEl** ppfelNew, CNote** ppnot)
{
    CFilterCon* pcon = NULL;
    if ( !CFilterConDate::s_bConstruct(pszMarker, iRelation, sDate,
            m_pfilsetMyOwner, &pcon, ppnot) )
        return FALSE;

    CFilterEl* pfelNew = new CFilterEl(pcon);
    ASSERT( ppfelNew );
    *ppfelNew = pfelNew;
        
    return TRUE;
}

BOOL CFilterElList::bNewLangEncText(CFilterEl** ppfelNew)
{
    CLangEncTextDlg dlg(this, ppfelNew);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilterElList::bNewLangEncText(CLangEnc* plng,
        const char* pszText, CFilterEl** ppfelNew, CNote** ppnot)
{
    CFilterCon* pcon = NULL;
    if ( !CFilterConLangEncString::s_bConstruct(plng, pszText,
            m_pfilsetMyOwner, &pcon, ppnot) )
        return FALSE;

    CFilterEl* pfelNew = new CFilterEl(pcon);
    ASSERT( ppfelNew );
    *ppfelNew = pfelNew;
        
    return TRUE;
}


CFilterEl::CFilterEl(CFilterCon* pcon) :
    m_pcon(pcon)
{
    ASSERT( m_pcon );
    m_sElement = "\\";
}

// 1997-12-01 MRP
// CFilterEl::CFilterEl(const char* psz) :
CFilterEl::CFilterEl(const char* pszExternalElement,
        const char* pszInternalElement) :
    m_pcon(NULL)
{
    ASSERT( pszExternalElement );
    m_sElement = pszExternalElement;
    m_sInternalElement = pszInternalElement;
}

CFilterEl::~CFilterEl()
{
    delete m_pcon;
}

BOOL CFilterEl::s_bReadProperties(Object_istream& obs,
        CFilterSet* pfilsetMyOwner, CFilterEl** ppfel)
{
    ASSERT( ppfel );
    Str8 s;
    BOOL bAny = FALSE;
    BOOL bKey = FALSE;
    if ( obs.bReadString(psz_fel, s) )
        {
        Str8 sExternalElement;
        Str8 sInternalElement;
        if ( CFilterEl::s_bSimpleElement(s, sExternalElement, sInternalElement) )
            *ppfel = new CFilterEl(sExternalElement, sInternalElement);
        else
            ;  // Note: unexpected filter element
        }
    else if ( obs.bReadString(psz_mkr, s) ||
            obs.bReadBool(psz_mkrRecord, bKey) )
        {
        BOOL bConstructed = FALSE;
        CFilterCon* pcon = NULL;
        CNote* pnot = NULL;
        if ( CFilterConDate::s_bReadProperties(obs, s, pfilsetMyOwner, &pcon) )
            {
            if (pcon) // 2000-06-15 TLB
                bConstructed = TRUE;  // 1998-04-08 MRP
            }
        else
            {
            Str8 sText;
            (void) s_bReadText(obs, sText);
            bConstructed = CFilterConMarkerString::s_bConstruct(s, sText,
                pfilsetMyOwner, &pcon, &pnot);
            }
        if ( bConstructed )
            *ppfel = new CFilterEl(pcon);
        else
            obs.notlst().Add(pnot);
        }
    else if ( obs.bReadString(psz_lng, s) ||
            obs.bReadBool(psz_lngAny, bAny) )
        {
        CLangEncSet* plngset = pfilsetMyOwner->plngset();
        CNoteList notlst;
        CLangEnc* plng = ( !s.IsEmpty() ?
            plngset->plngSearch_AddIfNew(s, notlst) :
            NULL
            );

        Str8 sText;  
        (void) s_bReadText(obs, sText);
        CFilterCon* pcon = NULL;
        CNote* pnot = NULL;
        if ( CFilterConLangEncString::s_bConstruct(plng, sText, pfilsetMyOwner,
                &pcon, &pnot) )
            {
            if ( pcon )
                *ppfel = new CFilterEl(pcon);
            }
        else
            obs.notlst().Add(pnot);
        }
    else
        return FALSE;
        
    return TRUE;
}

BOOL CFilterEl::s_bReadText(Object_istream& obs, Str8& sText)
{
    if ( obs.bAtEnd() )
        return FALSE;

    return obs.bReadString(psz_txt, sText);
}


BOOL CFilterEl::bModify()
{
    ASSERT( m_pcon );
    return m_pcon->bModify();
}


void CFilterElListListBox::InitListBox()
{
    InitLabels();
    SetItemHeight(iItemHeight());
}

void CFilterElListListBox::InitLabels()
{
    m_xElement = xSubItem_Show(IDC_fil_lblElement);
    m_xText = xSubItem_Show(IDC_fil_lblText);
}

int CFilterElListListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CFilterElListListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CFilterEl* pfel = (CFilterEl*)pel;
    ASSERT( pfel );
    const CFilterCon* pcon = pfel->pcon();
    if ( pcon )
        pcon->DrawElementText(this, cDC, rcItem, m_xElement, m_xText);
    else
        {
        const Str8& sElement = pfel->sElement();
        ASSERT( !sElement.IsEmpty() );
        int xLeft = m_xElement;
        DrawSubItemJustify(cDC, rcItem, &xLeft, m_xText, sElement);
        }
}


#ifdef prjWritefstream // 1.6.4aa 
void CFilterConMarkerString::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConMarkerString::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteString(psz_mkr, m_sMarker);
    obs.WriteString(psz_txt, m_sText);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConDate::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConDate::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteString(psz_mkr, m_sMarker);
    m_pdat->WriteProperties(obs);  // 1998-04-08 MRP
}

BOOL CFilterConDate::s_bReadProperties(Object_istream& obs,
        const Str8& sMarker, CFilterSet* pfilsetMyOwner,
        CFilterCon** ppcon)  // 1998-04-08 MRP
{
    CDateCon* pdat = NULL;
    if ( !CDateCon::s_bReadProperties(obs, &pdat) )
        return FALSE;
    
    if ( pdat )
        {
        ASSERT( ppcon );
        ASSERT( !*ppcon );
        *ppcon = new CFilterConDate(sMarker, *pdat, pfilsetMyOwner);
        delete pdat;
        }
    return TRUE;
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConLangEncString::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConLangEncString::WriteProperties(Object_ostream& obs) const 
#endif
{
    if ( m_plng )
        m_plng->WriteRef(obs, "");
    else
        obs.WriteBool(psz_lngAny, TRUE);
    obs.WriteString(psz_txt, m_sText);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConAnd::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConAnd::WriteProperties(Object_ostream& obs) const 
#endif
{
    m_pconA->WriteProperties(obs);
    obs.WriteString(psz_fel, s_pszAnd);
    m_pconB->WriteProperties(obs);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConOr::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConOr::WriteProperties(Object_ostream& obs) const 
#endif
{
    m_pconA->WriteProperties(obs);
    obs.WriteString(psz_fel, s_pszOr);
    m_pconB->WriteProperties(obs);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConNot::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConNot::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteString(psz_fel, s_pszNot);
    m_pcon->WriteProperties(obs);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConParen::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConParen::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteString(psz_fel, s_pszLParen);
    m_pcon->WriteProperties(obs);
    obs.WriteString(psz_fel, s_pszRParen);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConWith::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConWith::WriteProperties(Object_ostream& obs) const 
#endif
{
    m_pconA->WriteProperties(obs);
    obs.WriteString(psz_fel, s_pszWith);
    m_pconB->WriteProperties(obs);
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterConNonUnique::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterConNonUnique::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteString(psz_fel, s_pszNonUnique);
}


// --------------------------------------------------------------------------

// Add... button
CNewFilterDlg::CNewFilterDlg(CFilterSet* pfilset, CFilter** ppfilNew) :
    CDialog(CNewFilterDlg::IDD),
    m_pfil(NULL),
    m_ppfilNew(ppfilNew),
    m_pfilset(pfilset),
    m_fellst(m_pfilset),
    m_lboElements(&m_fellst)
{
    ASSERT( m_ppfilNew );
    ASSERT( !*m_ppfilNew );
    ASSERT( m_pfilset );
    m_bMatchWhole = FALSE;
    m_matset = CMCharOrder::matsetExactDisregardCase;
}

// Copy... button
CNewFilterDlg::CNewFilterDlg(const CFilter* pfilToCopyFrom, CFilter** ppfilNew) :
    CDialog(CNewFilterDlg::IDD),
    m_pfil(NULL),
    m_ppfilNew(ppfilNew),
    m_pfilset(pfilToCopyFrom->pfilsetMyOwner()),
    m_fellst(pfilToCopyFrom),
    m_lboElements(&m_fellst)
{
    ASSERT( m_ppfilNew );
    ASSERT( !*m_ppfilNew );
    ASSERT( m_pfilset );
    ASSERT( pfilToCopyFrom );
    m_sName = pfilToCopyFrom->sName();
    m_bMatchWhole = pfilToCopyFrom->bMatchWhole();
    m_matset = pfilToCopyFrom->GetMatchSetting();
}

// Modify... button
CNewFilterDlg::CNewFilterDlg(CFilter* pfil) :
    CDialog(CNewFilterDlg::IDD),
    m_pfil(pfil),
    m_ppfilNew(NULL),
    m_pfilset(m_pfil->pfilsetMyOwner()),
    m_fellst(m_pfil),
    m_lboElements(&m_fellst)
{
    ASSERT( m_pfil );
    ASSERT( m_pfilset );
    m_sName = m_pfil->sName();
    m_bMatchWhole = m_pfil->bMatchWhole();
    m_matset = m_pfil->GetMatchSetting();
}


void CNewFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewFilterDlg)
//    DDX_Txt(pDX, IDC_fil_edtName, m_sName);
    DDX_Control(pDX, IDC_fil_edtName, m_edtName);
    DDX_Control(pDX, IDC_fil_lboChoices, m_lboChoices);
    DDX_Control(pDX, IDC_fil_btnModify, m_btnModify);
    DDX_Control(pDX, IDC_fil_btnDelete, m_btnDelete);
    DDX_Control(pDX, IDC_fil_btnDeleteAll, m_btnDeleteAll);
    DDX_Check(pDX, IDC_fil_chkMatchWhole, m_bMatchWhole);
    DDX_Control(pDX, IDC_fil_cboMatchChars, m_cboMatchSetting);
    DDX_CBIndex(pDX, IDC_fil_cboMatchChars, m_iMatchSetting);
    DDX_Control(pDX, IDC_fil_edtNote, m_edtNote);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_fil_lboElements, m_lboElements);
}


// 1997-12-01 MRP: The following two lists of items MUST correspond
// one-to-one. They are the internal strings for the simple elements
// and the resource IDs of the (external) strings used in the
// visual user interface. They're listed in the order they occur
// in the Choices list box.
// This revision breaks the link between internal and external,
// so that the latter set of strings can be translated into French.
// Note that the first three are empty because their constructors
// are selected by conditional logic in OnOK.
static const int s_numChoices = 10;

static const char* s_apszInternalChoice[s_numChoices] =
    {
    "",  // Marker Text
    "",  // Marker Date
    "",  // Language Text
    s_pszAnd,
    s_pszOr,
    s_pszNot,
    s_pszLParen,
    s_pszRParen,
    s_pszWith,
    s_pszNonUnique
    };
    
static const char* s_apszExternalChoice[s_numChoices] = // 1.4em
    {
    _("Marker Text..."),
    _("Marker Date..."),
    _("Language Text..."),
    _("And"),
    _("Or"),
    _("Not"),
    0,  // Left Paren
    0,  // Right Paren
    _("With"),
    _("Non-Unique")
    };

static Str8 s_sExternalChoice(int itm)
{
    ASSERT( 0 <= itm );
    ASSERT( itm < s_numChoices );
    Str8 sExternalChoice;
    const char* psz = s_apszExternalChoice[itm];
    if ( !psz || *psz == 0 ) // 1.4ga Fix bug (1.4g) of crash on add or modify filter
        sExternalChoice = s_apszInternalChoice[itm];  // Parens
    else
		sExternalChoice = s_apszExternalChoice[itm]; // 1.4em
    return sExternalChoice;
}

BOOL CFilterEl::s_bSimpleElement(const Str8& sRead,
    Str8& sExternalElement, Str8& sInternalElement)
{
    int itm = 3;
    for ( ; itm != s_numChoices; itm++ )
        if ( !stricmp(sRead, s_apszInternalChoice[itm]) )
            {
            sExternalElement = s_sExternalChoice(itm);
            sInternalElement = s_apszInternalChoice[itm];
            return TRUE;
            }    

    return FALSE;
}

static const int s_numMatchSettings = 4;  // 1998-10-01 MRP: For French version

static const char* s_apszMatchSetting[s_numMatchSettings] = // 1.4en
	{
	_("by Primary groupings"),
	_("disregarding Case"),
	_("by Secondary ordering"),
	_("normally Ignored")
	};

BOOL CNewFilterDlg::OnInitDialog()
{
    // Do this before calling OnInitDialog, which updates the
    // controls and their data members.
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
    CDialog::OnInitDialog();    
    SetDlgItemText( IDC_fil_edtName, swUTF16( m_sName ) ); // 1.4qpv
    int itm = 0;
    for ( ; itm != s_numChoices; itm++ )
		{
		Str8 s = s_sExternalChoice(itm); // 1.4qvf  
        m_lboChoices.AddString( swUTF16( s ) ); // 1.4qvf Upgrade AddString for Unicode build
		}
    m_lboChoices.SetCurSel(0);

    m_lboElements.InitListBox();
    CFilterEl* pfel = m_fellst.pfelFirst();
    Update(pfel);
	// 1998-10-01 MRP: For French version
    itm = 0;
    for ( ; itm != s_numMatchSettings; itm++ )
    	{
	 	Str8 sItem;
		sItem = s_apszMatchSetting[itm]; // 1.4en
        m_cboMatchSetting.InsertString(itm, swUTF16( sItem ) ); // 1.4qre
        }
	m_cboMatchSetting.SetCurSel(m_iMatchSetting);
    // If adding a new filter let Name, the first control, have input focus,
    // but if editing an existing filter, set focus to the condition instead.   
    BOOL bLetWindowsSetFocusToName = (m_pfil == NULL);  // adding new filter
    if ( !bLetWindowsSetFocusToName )
        // editing an existing filter
        m_lboElements.SetFocus();
    
    return bLetWindowsSetFocusToName;
}

void CNewFilterDlg::Update(CFilterEl* pfel)
{
    m_lboElements.UpdateElements(pfel);
    
    BOOL bModifiable = ( pfel && pfel->bModifiable() );
    m_btnModify.EnableWindow(bModifiable);
    
    BOOL bDeletable = ( pfel != NULL );
    m_btnDelete.EnableWindow(bDeletable);
    m_btnDeleteAll.EnableWindow(bDeletable);
}


BEGIN_MESSAGE_MAP(CNewFilterDlg, CDialog)
    //{{AFX_MSG_MAP(CNewFilterDlg)
    ON_BN_CLICKED(IDC_fil_btnFirst, OnFirst)
    ON_BN_CLICKED(IDC_fil_btnLast, OnLast)
    ON_BN_CLICKED(IDC_fil_btnInsert, OnInsert)
    ON_BN_CLICKED(IDC_fil_btnModify, OnModify)
    ON_BN_CLICKED(IDC_fil_btnDelete, OnDelete)
    ON_BN_CLICKED(IDC_fil_btnDeleteAll, OnDeleteAll)
    ON_LBN_DBLCLK(IDC_fil_lboChoices, OnDblclkChoice)
    ON_LBN_DBLCLK(IDC_fil_lboElements, OnDblclkElement)
    ON_LBN_SELCHANGE(IDC_fil_lboElements, OnSelchangeElement)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CNewFilterDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_fil_edtName, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_fil_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw

    // Update the filter's data with the match setting information. 
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
    
    CNote* pnot = NULL;
    BOOL bOK = ( m_pfil != NULL ?
        m_pfil->bModifyProperties(m_sName, m_fellst, m_bMatchWhole, m_matset, &pnot) :
            // User viewed and possibly modified an existing filter
        m_pfilset->bAddFilter(m_sName, m_fellst, m_bMatchWhole, m_matset, &m_pfil, &pnot)
            // User added a new filter
        );
        
    if ( bOK )
        {
        ASSERT( m_pfil != NULL );
        ASSERT( pnot == NULL );
        
        if ( m_ppfilNew != NULL )
            *m_ppfilNew = m_pfil;  // return the new filter
        CDialog::OnOK();
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the filter. The filter object itself has not been changed.
        ASSERT( pnot );
        if ( pnot->bRefersToString(m_sName) )
            {
            // Set the insertion point, or select text referred to in note
            m_edtName.SetSel(pnot->dwSelReferent());
            m_edtName.SetFocus();
            }
        else
            {
            BOOL bRef = FALSE;
            CFilterEl* pfel = m_fellst.pfelFirst();
            for ( ; pfel ; pfel = m_fellst.pfelNext(pfel) )
                {
                const char* pszElement = pfel->sElement();
                if ( pnot->bRefersToString(pszElement) )
                    {
                    bRef = TRUE;
                    Update(pfel);
                    }
                }
            if ( !bRef )
                Update(m_fellst.pfelLast());
            }
            
        // Show the note on the dialog itself
        m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
        m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
        MessageBeep(0);

        delete pnot;
        }
}  // CNewFilterDlg::OnOK()

void CNewFilterDlg::OnCancel()
{
    CDialog::OnCancel();
}

void CNewFilterDlg::OnFirst()
{
    Insert(NULL);
}

void CNewFilterDlg::OnLast()
{
    Insert(m_fellst.pfelLast());
}

void CNewFilterDlg::OnInsert()
{
    Insert(m_lboElements.pfelSelected());
}

void CNewFilterDlg::Insert(CFilterEl* pfelPrev)
{
    // Get current selection in left-hand side list box
    ItemIndex itm = m_lboChoices.GetCurSel();

    CFilterEl* pfel = NULL;
    if ( itm == 0 )
        {
        if ( !m_fellst.bNewMarkerText(&pfel) )
            return;
        }
    else if ( itm == 1 )
        {
        if ( !m_fellst.bNewMarkerDate(&pfel) )
            return;
        }
    else if ( itm == 2 )
        {
        if ( !m_fellst.bNewLangEncText(&pfel) )
            return;
        }
    else
        {
        // pfel = new CFilterEl(sChoice);
        // 1997-12-01 MRP: Construct simple elements with both strings.
        ASSERT( 0 <= itm );
        ASSERT( itm < s_numChoices );
        pfel = new CFilterEl(s_sExternalChoice(itm), s_apszInternalChoice[itm]);
        }

    m_fellst.Insert(pfelPrev, pfel);
    Update(pfel);
}

void CNewFilterDlg::OnModify()
{
    CFilterEl* pfel = m_lboElements.pfelSelected();
    ASSERT( pfel );
    if ( pfel->bModifiable() && pfel->bModify() )
        Update(pfel);
}

void CNewFilterDlg::OnDelete()
{
    CFilterEl* pfel = m_lboElements.pfelSelected();
    m_fellst.Delete(&pfel);
    Update(pfel);
}

void CNewFilterDlg::OnDeleteAll()
{
    m_fellst.DeleteAll();
    Update(NULL);
}

void CNewFilterDlg::OnDblclkChoice()
{
    OnInsert();  // shortcut for the Insert button
}

void CNewFilterDlg::OnDblclkElement()
{
    OnModify();  // shortcut for the Modify button
}

void CNewFilterDlg::OnSelchangeElement()
{
    CFilterEl* pfel = m_lboElements.pfelSelected();
    Update(pfel);
}
