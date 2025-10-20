// cck_d.cpp  Dialog boxes for Consistency Checks

#include "stdafx.h"
#include "toolbox.h"
// Because of CNumberedRecElPtr, you must include "ind.h" above "cck.h".
#include "ind.h"  // CIndex, CNumberedRecElPtr
#include "cck.h"
#include "ddx.h"
#include "mrfset_d.h"  // CSelectFieldsDlg
#include "shwdoc.h"  // CShwDoc
#include "shwview.h"  // CShwView
#include "progress.h"
#include "crngset.h"  // CRangeSet
#include "crngst_d.h"  // CRangeSetListBox
#include "cck_d.h"
#include "mkr_d.h"
#include "help_ids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// --------------------------------------------------------------------------

CCheckConsistencyDlg::CCheckConsistencyDlg(CFieldConsistencyChecker* pcck,
        CIndex* pind) :
    CDialog(IDD),
    m_pcck(pcck),
    m_pmkrset(m_pcck->m_pviewMyOwner->pdoc()->ptyp()->pmkrset()),
    m_cboSingleField(m_pmkrset, FALSE, NULL),
    m_subsetFieldsToCheck(pcck->m_subsetFieldsToCheck)
{
    ASSERT( m_pcck );
    ASSERT( m_pmkrset );
    ASSERT( pind );

    m_sDatabase = sGetFileName( sUTF8( m_pcck->m_pviewMyOwner->pdoc()->GetTitle() ) ); // 1.4qvj Upgrade sGetFileName for Unicode build
    if ( pind->bUsingFilter() )
        m_sFilter = pind->pfil()->sName();
    else
        m_sFilter = _("[none]");
    m_sDatabaseType = m_pcck->m_pviewMyOwner->pdoc()->ptyp()->sName();

    m_pmkrSingleField = m_pmkrset->pmkrSearch(m_pcck->m_sSingleFieldMarker);
    if ( !m_pmkrSingleField )
        m_pmkrSingleField = m_pmkrset->pmkrFirst();
    if ( m_pcck->m_bSingleField )
        m_iFieldsToCheck = 1;
    else if ( m_pcck->m_bSelectedFields )
        m_iFieldsToCheck = 2;
    else
        m_iFieldsToCheck = 0;

#ifdef RECORD_ORDER_CHECKING_ENABLED
    m_bRecordByRecord = m_pcck->m_bRecordByRecord;
#endif

    m_bCheckDataDefn = m_pcck->m_bCheckDataDefn;
    m_bCheckRangeSets = m_pcck->m_bCheckRangeSets;
    m_bCheckRefConsistency = m_pcck->m_bCheckRefConsistency;
}

CCheckConsistencyDlg::~CCheckConsistencyDlg()
{
}

void CCheckConsistencyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CCheckConsistencyDlg)
//    DDX_Txt(pDX, IDC_cck_lblDatabase, m_sDatabase);
//    DDX_Txt(pDX, IDC_cck_lblFilter, m_sFilter);
//    DDX_Txt(pDX, IDC_cck_lblDatabaseType, m_sDatabaseType);
    DDX_Radio(pDX, IDC_cck_optAllFields, m_iFieldsToCheck);
    DDX_Control(pDX, IDC_cck_cboSingleField, m_cboSingleField);
    DDX_Control(pDX, IDC_cck_btnSelectFields, m_btnSelectFields);
    DDX_Check(pDX, IDC_cck_chkCheckDataDefn, m_bCheckDataDefn);
    DDX_Check(pDX, IDC_cck_chkCheckRangeSets, m_bCheckRangeSets);
    DDX_Check(pDX, IDC_cck_chkCheckJumpPaths, m_bCheckRefConsistency);
    //}}AFX_DATA_MAP
}

BOOL CCheckConsistencyDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();
    SetDlgItemText( IDC_cck_lblDatabase, swUTF16( m_sDatabase ) ); // 1.4qpv
    SetDlgItemText( IDC_cck_lblFilter, swUTF16( m_sFilter ) ); // 1.4qpv
    SetDlgItemText( IDC_cck_lblDatabaseType, swUTF16( m_sDatabaseType ) ); // 1.4qpv

    m_cboSingleField.UpdateElements(m_pmkrSingleField);
    m_cboSingleField.EnableWindow((m_iFieldsToCheck == 1));
    m_btnSelectFields.EnableWindow((m_iFieldsToCheck == 2));

    return bRet;
}

BEGIN_MESSAGE_MAP(CCheckConsistencyDlg, CDialog)
    //{{AFX_MSG_MAP(CCheckConsistencyDlg)
    ON_BN_CLICKED(IDC_cck_btnSelectFields, OnSelectFields)
    ON_BN_CLICKED(IDC_cck_optAllFields, OnChangeFieldsToCheckOption)
    ON_BN_CLICKED(IDC_cck_optSingleField, OnChangeFieldsToCheckOption)
    ON_BN_CLICKED(IDC_cck_optSelectedFields, OnChangeFieldsToCheckOption)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCheckConsistencyDlg::OnOK()
{
    CDialog::OnOK();

//    GetDlgItemText( IDC_cck_lblDatabase, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_cck_lblDatabase, sDlgItem ); // 1.4qpw
	m_sDatabase = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_cck_lblFilter, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_cck_lblFilter, sDlgItem ); // 1.4qpw
	m_sFilter = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_cck_lblDatabaseType, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_cck_lblDatabaseType, sDlgItem ); // 1.4qpw
	m_sDatabaseType = sUTF8( sDlgItem ); // 1.4qpw

    m_pcck->m_bSingleField = (m_iFieldsToCheck == 1);
    m_pcck->m_bSelectedFields = (m_iFieldsToCheck == 2);
    m_pmkrSingleField = m_cboSingleField.pmkrSelected();
    if ( m_pmkrSingleField )
        m_pcck->m_sSingleFieldMarker = m_pmkrSingleField->sMarker();
    else
        m_pcck->m_sSingleFieldMarker.Empty();
    m_pcck->m_subsetFieldsToCheck = m_subsetFieldsToCheck;

#ifdef RECORD_ORDER_CHECKING_ENABLED
    m_pcck->m_bRecordByRecord = m_bRecordByRecord;
#endif

    m_pcck->m_bCheckDataDefn = m_bCheckDataDefn;
    m_pcck->m_bCheckRangeSets = m_bCheckRangeSets;
    m_pcck->m_bCheckRefConsistency = m_bCheckRefConsistency;
}

void CCheckConsistencyDlg::OnSelectFields()
{
    CSelectFieldsDlg dlg(m_subsetFieldsToCheck, TRUE ); // , IDSx_SelectCheckFields); // 1.4gb
    dlg.SetHelpID(IDD_SELECT_FIELDS_TO_CHECK);
    dlg.DoModal();
}

void CCheckConsistencyDlg::OnChangeFieldsToCheckOption()
{
    UpdateData();
    m_cboSingleField.EnableWindow((m_iFieldsToCheck == 1));
    m_btnSelectFields.EnableWindow((m_iFieldsToCheck == 2));
}

// --------------------------------------------------------------------------

CInconsistencyDlg::CInconsistencyDlg(UINT nIDTemplate, CShwView* pview, CRecPos& rpsStart,
                                     CRecPos& rpsEnd, CCheckingState *pcks, BOOL& bNeedToRecheck,
                                     Str8 sMessage, // 1.4heq Add message arg to inconsistency dlg func
									 BOOL bExplicitNewlines, // 1.4heq Add bool arg to inconsistency dlg func
									 Str8 s) :
    CDialog(nIDTemplate),
    m_pview(pview),
    m_rpsStart(rpsStart),
    m_rpsEnd(rpsEnd),
    m_pcks(pcks),
    m_bCheckingDatabase(( pcks ) ? pcks->bCheckingDatabase() : FALSE ),
    m_bNeedToRecheck(bNeedToRecheck),
//    m_bAlwaysConsiderReplacementAsModified(bExplicitNewlines), // 1.4heq Add bool arg to inconsistency dlg func
    m_bAlwaysConsiderReplacementAsModified(TRUE), // 1.5.4g 
    m_bReplacementModified(m_bAlwaysConsiderReplacementAsModified)
{
    ASSERT(m_pview);
    ASSERT(m_rpsStart.pfld == m_rpsEnd.pfld);
    ASSERT(m_rpsStart.prec == m_rpsEnd.prec);
    ASSERT(m_rpsStart.iChar <= m_rpsEnd.iChar);
    ASSERT(m_rpsStart.pfld);
    ASSERT(m_rpsStart.prec);
    AfxFormatString1(m_sInconsistency, sMessage, s);
    m_bReadOnly = pview->pdoc()->pindset()->bReadOnly();
    m_sMarkerLabel = "\\" + m_rpsStart.pfld->pmkr()->sKey();
     // If data is multiple items, we assume rpsStart and rpsEnd delimit a single word.
    m_sFailedData = m_rpsStart.pfld->sItem(m_rpsStart, rpsEnd, !m_rpsStart.pmkr()->bMultipleItemData());
    m_sReplacement = m_sFailedData;
    m_bNeedToRecheck = FALSE;
}

CInconsistencyDlg::~CInconsistencyDlg()
{
}

BEGIN_MESSAGE_MAP(CInconsistencyDlg, CDialog)
    //{{AFX_MSG_MAP(CInconsistencyDlg)
    ON_EN_CHANGE(IDC_cck_edtReplacement, OnChangeReplacement)
    ON_BN_CLICKED(IDC_cck_btnSkip, OnSkip)
    ON_BN_CLICKED(IDC_cck_btnSkipAll, OnSkipAll)
    ON_BN_CLICKED(IDC_cck_btnReplace, OnReplace)
    ON_BN_CLICKED(IDC_cck_btnReplaceAll, OnReplaceAll)
    ON_BN_CLICKED(IDC_cck_btnModify, OnModify)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInconsistencyDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    // Edit boxes that use the derived class for font and keyboard.
    DDX_Control(pDX, IDC_cck_edtFailedData, m_elcFailedData);
    DDX_Control(pDX, IDC_cck_edtReplacement, m_elcReplacement);
//    Shw_DDX_TrimmedTxt(pDX, IDC_cck_edtReplacement, m_sReplacement); // 1.4gt Replace DDX_Trimmed in consistency check replacement

    //{{AFX_DATA_MAP(CInconsistencyDlg)
//    DDX_Txt(pDX, IDC_cck_lblMarker, m_sMarkerLabel);
//    DDX_Txt(pDX, IDC_cck_lblInconsistency, m_sInconsistency);
    DDX_Control(pDX, IDC_cck_btnSkipAll, m_btnSkipAll);
    DDX_Control(pDX, IDC_cck_btnReplace, m_btnReplace);
    DDX_Control(pDX, IDC_cck_btnReplaceAll, m_btnReplaceAll);
    //}}AFX_DATA_MAP
}

BOOL CInconsistencyDlg::s_bWindowPlacement = FALSE;
RECT CInconsistencyDlg::s_rcWindowPlacement = { 0, 0, 0, 0 };

BOOL CInconsistencyDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_cck_lblMarker, swUTF16( m_sMarkerLabel ) ); // 1.4qpv
    SetDlgItemText( IDC_cck_lblInconsistency, swUTF16( m_sInconsistency ) ); // 1.4qpv
//    SetDlgItemTxt( IDC_cck_edtReplacement, m_sReplacement); // 1.4gt // 1.4qpn
	m_elcReplacement.SetEditLngText( m_sReplacement ); // 1.4qpn

    // Note: Enabling and disabling buttons must be done before
    // the text is set in the Replacement edit box,
    // because that causes OnChangeReplacement() to be called, which
    // may change some of these default settings.
    m_btnSkipAll.EnableWindow(m_bCheckingDatabase);
    m_btnReplace.EnableWindow(FALSE);
    m_btnReplaceAll.EnableWindow(FALSE);

    m_elcFailedData.SetLangEnc(m_rpsStart.pfld->plng());  // determines font and keyboard // 1.4vxk 
    m_elcFailedData.SetEditLngText(m_sFailedData);

    m_elcReplacement.SetLangEnc(m_rpsStart.pfld->plng());  // determines font and keyboard // 1.4vxk 
//    m_elcReplacement.SetWindowText(m_sFailedData);

    if ( m_bReadOnly )
        {
        m_elcReplacement.EnableWindow(FALSE);
        }
    else
        {
        m_elcReplacement.SetFocus();
        m_elcReplacement.SetSel(0, -1);
        }


    if ( s_bWindowPlacement )
        {
        WINDOWPLACEMENT wpl;
        GetWindowPlacement(&wpl);
        int iWidth = wpl.rcNormalPosition.right - wpl.rcNormalPosition.left;
        int iHeight = wpl.rcNormalPosition.bottom - wpl.rcNormalPosition.top;
        wpl.rcNormalPosition.left = s_rcWindowPlacement.left;
        wpl.rcNormalPosition.top = s_rcWindowPlacement.top;
        wpl.rcNormalPosition.right = wpl.rcNormalPosition.left + iWidth;
        wpl.rcNormalPosition.bottom = wpl.rcNormalPosition.top + iHeight;
        SetWindowPlacement(&wpl);
        }

    MessageBeep(0);
    return FALSE;  // OnInitDialog has already set the focus
}

void CInconsistencyDlg::RememberWindowPlacement()
{
    // Remember where to open the dialog box next time.
    WINDOWPLACEMENT wpl;
    GetWindowPlacement(&wpl);
    s_rcWindowPlacement = wpl.rcNormalPosition;
    s_bWindowPlacement = TRUE;
}

void CInconsistencyDlg::OnOK()
{
//    GetDlgItemText( IDC_cck_lblMarker, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_cck_lblMarker, sDlgItem ); // 1.4qpw
	m_sMarkerLabel = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_cck_lblInconsistency, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_cck_lblInconsistency, sDlgItem ); // 1.4qpw
	m_sInconsistency = sUTF8( sDlgItem ); // 1.4qpw
	m_sReplacement = m_elcReplacement.sGetEditLngText(); // 1.4qpn
	TrimBothEnds( m_sReplacement ); // 1.4hdc
    // Remember where to open the dialog box next time.
    RememberWindowPlacement();
    CDialog::OnOK();
}

void CInconsistencyDlg::OnCancel()
{
    // Remember where to open the dialog box next time.
    RememberWindowPlacement();
    CDialog::OnCancel();
}

void CInconsistencyDlg::OnChangeReplacement()
{
    UpdateData();
	m_sReplacement = m_elcReplacement.sGetEditLngText(); // 1.4qpn
	TrimBothEnds( m_sReplacement ); // 1.4hdc
    if ( m_sFailedData == m_sReplacement && !m_bAlwaysConsiderReplacementAsModified )
        {
        m_bReplacementModified = FALSE;
        m_btnReplace.EnableWindow(FALSE);  
        m_btnReplaceAll.EnableWindow(FALSE);
        }
    else
        {
        m_bReplacementModified = TRUE;
        SetDefID(IDC_cck_btnReplace);
        if ( !m_bReadOnly )
            {
            m_btnReplace.EnableWindow(TRUE);
            // Prevent Replace All for missing data
            if ( m_bCheckingDatabase && !m_sFailedData.IsEmpty() )
                m_btnReplaceAll.EnableWindow(TRUE);
            }
        }
}

void CInconsistencyDlg::OnSkip()
{
    if ( m_pcks )
        m_pcks->IncrementInconsistenciesSkipped();
    OnOK();
}

void CInconsistencyDlg::OnSkipAll()
{
    ASSERT(m_pcks);
    m_pcks->AddSkipAllElement(m_rpsStart.pfld->pmkr(), m_sFailedData);
    OnSkip();
}

void CInconsistencyDlg::OnReplace()
{
    m_pview->ReplaceSelectedInconsistency(m_sReplacement);
    m_bNeedToRecheck = TRUE;
    OnOK();
}

void CInconsistencyDlg::OnReplaceAll()
{
    ASSERT(m_pcks);
    switch ( m_pcks->araAddReplaceAllElement(m_rpsStart.pfld->pmkr(), m_sFailedData, m_sReplacement) )
        {
        case CAssociationSet::araAdded:
            OnReplace();
            break;
        case CAssociationSet::araCircularReplacement:
            {
            CMsgCircularReplacmentDlg dlg( m_sFailedData, m_sReplacement, m_rpsStart.pfld->pmkr() );
            dlg.DoModal();
            break;
            }
        case CAssociationSet::araCallerError:
        case CAssociationSet::araSystemError:
        default:
            ASSERT(FALSE);
        }
}

void CInconsistencyDlg::OnModify()
{
}

// --------------------------------------------------------------------------

CDataDefnInconsistencyDlg::CDataDefnInconsistencyDlg(CShwView* pview, CRecPos& rpsStart, CRecPos& rpsEnd,
            CCheckingState *pcks, BOOL& bNeedToRecheck, Str8 sMessage, BOOL bExplicitNewlines) : // 1.4heq
    CInconsistencyDlg(CDataDefnInconsistencyDlg::IDD, pview, rpsStart, rpsEnd, pcks, bNeedToRecheck, sMessage, bExplicitNewlines)
{
}

CDataDefnInconsistencyDlg::~CDataDefnInconsistencyDlg()
{
}

// --------------------------------------------------------------------------

CMsgCircularReplacmentDlg::CMsgCircularReplacmentDlg(Str8 sFrom, Str8 sTo, CMarker* pmkr) :

    CDialog(IDD_CIRCULAR_REPLACEMENT),
    m_sFrom(sFrom),
    m_sTo(sTo),
    m_pmkr(pmkr)
{
    ASSERT(m_pmkr);
}

void CMsgCircularReplacmentDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);

    // Edit boxes that use the derived class for font and keyboard.
    DDX_Control(pDX, IDC_edtFrom1, m_elcFrom1);
    DDX_Control(pDX, IDC_edtTo1,   m_elcTo1);
    DDX_Control(pDX, IDC_edtFrom2, m_elcFrom2);
    DDX_Control(pDX, IDC_edtTo2,   m_elcTo2);
}

BOOL CMsgCircularReplacmentDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_elcFrom1.SetLangEnc( m_pmkr->plng() );      // determines font and keyboard // 1.4vxk 
    m_elcFrom1.SetEditLngText(m_sTo);

    m_elcTo1.SetLangEnc( m_pmkr->plng() );      // determines font and keyboard // 1.4vxk 
    m_elcTo1.SetEditLngText(m_sFrom);

    m_elcFrom2.SetLangEnc( m_pmkr->plng() );      // determines font and keyboard // 1.4vxk 
    m_elcFrom2.SetEditLngText(m_sFrom);

    m_elcTo2.SetLangEnc( m_pmkr->plng() );      // determines font and keyboard // 1.4vxk 
    m_elcTo2.SetEditLngText(m_sTo);

    return FALSE;  // OnInitDialog has already set the focus
}
