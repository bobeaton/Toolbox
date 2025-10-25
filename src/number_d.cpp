// number_d.cpp  (Re)Numbering dialog boxes

#include "stdafx.h"
#include "toolbox.h"
#include "number.h"
#include "number_d.h"
#include "typ.h"
#include "help_ids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// --------------------------------------------------------------------------
BOOL CMarkerUseContentsComboBox::bIncludeElement(const CSetEl* psel) const
{
    const CMarker* pmkr = (const CMarker*)psel;
    ASSERT( pmkr );
    return ( pmkr != pmkr->pmkrsetMyOwner()->ptyp()->pmkrTxt() &&
		     pmkr != pmkr->pmkrsetMyOwner()->ptyp()->pmkrDateStamp() );
}

// --------------------------------------------------------------------------
CMarkerNumberingComboBox::CMarkerNumberingComboBox(CMarkerSet* pmkrset, CMarker* pmkrDontUse)
	: CSetComboBox(pmkrset, TRUE)
{
	m_pmkrDontUse = pmkrDontUse;
}

void CMarkerNumberingComboBox::UpdateElements(CMarker* pmkrToSelect, CMarker* pmkrDontUse)
{
	m_pmkrDontUse = pmkrDontUse;
	CSetComboBox::UpdateElements( pmkrToSelect );
}

BOOL CMarkerNumberingComboBox::bIncludeElement(const CSetEl* psel) const
{
    const CMarker* pmkr = (const CMarker*)psel;
    ASSERT( pmkr );
    return ( pmkr != pmkr->pmkrsetMyOwner()->pmkrRecord() &&
		     pmkr != pmkr->pmkrsetMyOwner()->ptyp()->pmkrDateStamp() &&
			 pmkr != m_pmkrDontUse );
}

// --------------------------------------------------------------------------

CReNumberDlg::CReNumberDlg(CNumberProcess* pnbrproc, const CField* pfldCurRecordMkr, int iIDD) :
    CDialog(iIDD),
    m_pnbrproc(pnbrproc),
	m_pfldCurRecordMkr(pfldCurRecordMkr)
{
    ASSERT( m_pnbrproc );
    m_sMkrRef = m_pnbrproc->m_pmkrRef->sMarker();
    m_sMkrTxt = m_pnbrproc->m_pmkrTxt->sMarker();
}

void CReNumberDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_NBRPROC_edtCurRecord, m_elcRecord);
    //{{AFX_DATA_MAP(CReNumberDlg)
	DDX_Control(pDX, IDC_NBRPROC_optCurRecord, m_optCurRecord);
    DDX_Control(pDX, IDC_NBRPROC_optWholeDB, m_optWholeDB);
    DDX_Control(pDX, IDC_NBRPROC_edtStartRef, m_edtStartRef);
//    DDX_Txt(pDX, IDC_NBRPROC_lblMkrRef, m_sMkrRef);
//    DDX_Txt(pDX, IDC_NBRPROC_lblMkrTxt, m_sMkrTxt);
//}}AFX_DATA_MAP
}

BOOL CReNumberDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();
    SetDlgItemText( IDC_NBRPROC_lblMkrRef, swUTF16( m_sMkrRef ) ); // 1.4qpv
    SetDlgItemText( IDC_NBRPROC_lblMkrTxt, swUTF16( m_sMkrTxt ) ); // 1.4qpv

	InitControls();
    return bRet;
}

void CReNumberDlg::InitControls()
{
	Str8 sStartRef;
	sStartRef.Format( "%.*u", m_pnbrproc->m_iNumberOfDigits, 1 );
	m_edtStartRef.SetWindowText(  swUTF16( sStartRef ) ); // 1.4qpy
	if ( m_pfldCurRecordMkr )
		{
        m_elcRecord.SetLangEnc( m_pfldCurRecordMkr->pmkr()->plng() ); // 1.4vxk 
        m_elcRecord.SetEditLngText( m_pfldCurRecordMkr->sContents() ); // 1.4qpn
		m_optCurRecord.SetCheck( !m_pnbrproc->m_bWholeDocument );
		m_optWholeDB.SetCheck( m_pnbrproc->m_bWholeDocument );
		}
	else
		{
        m_elcRecord.ShowWindow(FALSE);
		m_optCurRecord.SetCheck( FALSE );
		m_optCurRecord.EnableWindow( FALSE );
		m_optWholeDB.SetCheck( TRUE );
		}
}

BEGIN_MESSAGE_MAP(CReNumberDlg, CDialog)
    //{{AFX_MSG_MAP(CReNumberDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CReNumberDlg::OnOK()
{
	if (!UpdateData(TRUE))
		{
		// the UpdateData routine will set focus to correct item
		return;
		}
//    GetDlgItemText( IDC_NBRPROC_lblMkrRef, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_NBRPROC_lblMkrRef, sDlgItem ); // 1.4qpw
	m_sMkrRef = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_NBRPROC_lblMkrTxt, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_NBRPROC_lblMkrTxt, sDlgItem ); // 1.4qpw
	m_sMkrTxt = sUTF8( sDlgItem ); // 1.4qpw

	if ( bValidate() )
		{
		UpdateNumberProc();
		EndDialog( IDOK );
		}
}

BOOL CReNumberDlg::bValidate()
{
//	char buffer[DLGBUFMAX+1];
//	m_edtStartRef.GetWindowText( (char*)buffer, DLGBUFMAX ); // 1.4ec Change to buffer
	CString sDlgItem; // 1.4qpx
	m_edtStartRef.GetWindowText( sDlgItem ); // 1.4ec Change to buffer // 1.4qpx
	Str8 s = sUTF8( sDlgItem ); // 1.4qpx
	m_lStartRef = atol( s );
	if ( m_lStartRef <= 0 || m_lStartRef > MAX_START_REF_NUM )
		{
		AfxMessageBox( _("Invalid starting reference number") );
		m_edtStartRef.SetFocus();
		return FALSE;
		}

	return TRUE;
}

void CReNumberDlg::UpdateNumberProc()
{
    m_pnbrproc->m_iStartRef = (unsigned int) m_lStartRef;
//	char buffer[DLGBUFMAX+1];
	CString sDlgItem; // 1.4qpx
	m_edtStartRef.GetWindowText( sDlgItem ); // 1.4ec Change to buffer // 1.4qpx
	Str8 s = sUTF8( sDlgItem ); // 1.4qpx
    m_pnbrproc->m_iNumberOfDigits = s.GetLength(); // 5.97s Make it handle 4 digit numbers if desired
	m_pnbrproc->m_bWholeDocument = m_optWholeDB.GetCheck();
}

// --------------------------------------------------------------------------

CNumberDlg::CNumberDlg(CNumberProcess* pnbrproc, const CField* pfldCurRecordMkr)
			: CReNumberDlg(pnbrproc, pfldCurRecordMkr, IDD_NUMBER),
			  m_subsetInvalidMarkersForTextBreak(pnbrproc->m_ptypMyOwner->pmkrset(), FALSE),
			  m_lboTextBreakMkrs(pnbrproc->m_subsetTextBreakMarkers,
								 TRUE,
// TLB 06/25/1999								 pnbrproc->m_pmkrTxt->plng(),
								 &m_subsetInvalidMarkersForTextBreak ),
			  m_sPunctuation( pnbrproc->m_sBreakChars ), // 1.1tj Remember punctuation for break/number text
			  m_cboUseContents(pnbrproc->m_ptypMyOwner->pmkrset())
{
	m_subsetInvalidMarkersForTextBreak.Include( m_pnbrproc->m_pmkrRef, TRUE );
	m_subsetInvalidMarkersForTextBreak.Include( m_pnbrproc->m_ptypMyOwner->pmkrRecord(), TRUE );
	if ( m_pnbrproc->m_ptypMyOwner->pmkrDateStamp() )
		m_subsetInvalidMarkersForTextBreak.Include( m_pnbrproc->m_ptypMyOwner->pmkrDateStamp(), TRUE );
	// May also want to include all interlinear markers except m_pnbrproc->m_pmkrTxt
}

void CNumberDlg::DoDataExchange(CDataExchange* pDX)
{
    CReNumberDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNumberDlg)
//    DDX_Txt(pDX, IDC_NBRPROC_edtPunctuation, m_sPunctuation);
	DDX_Control(pDX, IDC_NBRPROC_lboTextBreakMkrs, m_lboTextBreakMkrs);
    DDX_Control(pDX, IDC_NBRPROC_optUseContents, m_optUseContents);
    DDX_Control(pDX, IDC_NBRPROC_cboUseContents, m_cboUseContents);
    DDX_Control(pDX, IDC_NBRPROC_optUseText, m_optUseText);
    DDX_Control(pDX, IDC_NBRPROC_edtUseText, m_edtUseText);

//}}AFX_DATA_MAP
}

void CNumberDlg::InitControls()
{
	CReNumberDlg::InitControls();
    SetDlgItemText( IDC_NBRPROC_edtPunctuation, swUTF16( m_sPunctuation ) ); // 1.4qpv

	m_lboTextBreakMkrs.Init();
	if ( m_lboTextBreakMkrs.bEmpty() )
		{
		m_pnbrproc->m_subsetTextBreakMarkers.Include(m_pnbrproc->m_pmkrTxt, TRUE);
		m_lboTextBreakMkrs.Init();
		}
	m_lboTextBreakMkrs.SetSel(-1, FALSE);
	m_optUseContents.SetCheck( m_pnbrproc->m_bGetNameFromMkr );
    m_optUseText.SetCheck( !m_pnbrproc->m_bGetNameFromMkr );
	m_cboUseContents.UpdateElements( m_pnbrproc->m_pmkrUseContentsAsName );
	if ( !m_pnbrproc->m_bGetNameFromMkr )
		m_edtUseText.SetWindowText(  swUTF16( m_pnbrproc->m_sName ) ); // 1.4qpy
	SetState();
}

void CNumberDlg::SetState()
{
	if ( m_optUseContents.GetCheck() )
		{
		m_cboUseContents.EnableWindow( TRUE );
		m_edtUseText.EnableWindow( FALSE );
		}
    else
		{
		m_cboUseContents.EnableWindow( FALSE );
		m_edtUseText.EnableWindow( TRUE );
		}
}

BOOL CNumberDlg::bValidate()
{
	if ( !CReNumberDlg::bValidate() )
		return FALSE;

	// May want to do checks for non-empty punctuation and name (if Use Name is selected)
	return TRUE;
}

void CNumberDlg::UpdateNumberProc()
{
	CReNumberDlg::UpdateNumberProc();
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_NBRPROC_edtPunctuation, sDlgItem ); // 1.4qpw
	m_sPunctuation = sUTF8( sDlgItem ); // 1.4qpw

	m_pnbrproc->m_sBreakChars = m_sPunctuation;
	m_pnbrproc->m_bGetNameFromMkr = m_optUseContents.GetCheck();
	if ( m_pnbrproc->m_bGetNameFromMkr )
		m_pnbrproc->m_pmkrUseContentsAsName = m_cboUseContents.pmkrSelected();
	else
		{
		GetDlgItemText( IDC_NBRPROC_edtUseText, sDlgItem ); // 1.4sn Fix U bug of interlin number using punc instead of name
		m_pnbrproc->m_sName = sUTF8( sDlgItem ); // 1.4qpx
		}
}

BEGIN_MESSAGE_MAP(CNumberDlg, CDialog)
    //{{AFX_MSG_MAP(CNumberDlg)
	ON_BN_CLICKED(IDC_nbrproc_btnSelectFields, OnSelectFields)
	ON_BN_CLICKED(IDC_NBRPROC_optUseContents, SetState)
	ON_BN_CLICKED(IDC_NBRPROC_optUseText, SetState)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CNumberDlg::OnSelectFields()
{
    CSelectFieldsDlg dlg( m_pnbrproc->m_subsetTextBreakMarkers,
						  FALSE,
						  m_pnbrproc->m_pmkrTxt->plng(),
						  &m_subsetInvalidMarkersForTextBreak ); // , IDSx_SelectBreakFields ); 1.4gb
    dlg.SetHelpID(IDD_SELECT_TEXT_BREAK_FIELDS);
    dlg.DoModal();
	m_lboTextBreakMkrs.ResetContent();
	m_lboTextBreakMkrs.Init();
	m_lboTextBreakMkrs.SetSel(-1, FALSE);
}

// --------------------------------------------------------------------------

CSetupNbrDlg::CSetupNbrDlg(const Str8 sDBName) :
    CDialog(CSetupNbrDlg::IDD),
    m_sDBName(sDBName)
{
}

void CSetupNbrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetupNbrDlg)
//    DDX_Txt(pDX, IDC_NBRPROC_lblDBType, m_sDBName);
//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetupNbrDlg, CDialog)
    //{{AFX_MSG_MAP(CSetupNbrDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetupNbrDlg::OnInitDialog()
	{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_NBRPROC_lblDBType, swUTF16( m_sDBName ) ); // 1.4qpv
	return TRUE;
	}

void CSetupNbrDlg::OnOK()
	{
//    GetDlgItemText( IDC_NBRPROC_lblDBType, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_NBRPROC_lblDBType, sDlgItem ); // 1.4qpw
	m_sDBName = sUTF8( sDlgItem ); // 1.4qpw

	CDialog::OnOK();
	}

// --------------------------------------------------------------------------

CDbTypNumberingPage::CDbTypNumberingPage( CDatabaseType* ptyp )
    : CPropertyPage(CDbTypNumberingPage::IDD),
	m_cboMkrRef(ptyp->pmkrset()),
	m_cboMkrTxt(ptyp->pmkrset())
{
    m_ptyp = ptyp;
    ASSERT( m_ptyp );
	if ( m_ptyp->bAllowNumberingOpt() )
		{
		m_bWasActive = FALSE;
		m_sDBTypeName = m_ptyp->sName();
		m_sMkrRec = m_ptyp->pmkrRecord()->sMarker();
		m_bNumberingDisabled = !ptyp->bNumberingEnabled();
		}
}

void CDbTypNumberingPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDbTypNumberingPage)
//    DDX_Txt(pDX, IDC_TYP_NAME, m_sDBTypeName);
//    DDX_Txt(pDX, IDC_TYP_RECORDMARKER, m_sMkrRec);
    DDX_Control(pDX, IDC_TYP_cboMkrRef, m_cboMkrRef);
    DDX_Control(pDX, IDC_TYP_cboMkrTxt, m_cboMkrTxt);
    DDX_Check(pDX, IDC_TYP_chkDisableNumbering, m_bNumberingDisabled);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDbTypNumberingPage, CPropertyPage)
    //{{AFX_MSG_MAP(CDbTypNumberingPage)
	ON_BN_CLICKED(IDC_TYP_chkDisableNumbering, OnDisableNumberingChanged)
	ON_CBN_SELCHANGE(IDC_TYP_cboMkrRef, OnSelchangeMkrRef)
	ON_CBN_SELCHANGE(IDC_TYP_cboMkrTxt, OnSelchangeMkrTxt)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDbTypNumberingPage::OnInitDialog()
{
	ASSERT( m_ptyp->bAllowNumberingOpt() );
    m_bWasActive = TRUE;
    CPropertyPage::OnInitDialog();
    SetDlgItemText( IDC_TYP_NAME, swUTF16( m_sDBTypeName ) ); // 1.4qpv
    SetDlgItemText( IDC_TYP_RECORDMARKER, swUTF16( m_sMkrRec ) ); // 1.4qpv

	SetState();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDbTypNumberingPage::OnDisableNumberingChanged()
{
	UpdateData(TRUE);
	SetState();
}

void CDbTypNumberingPage::OnSelchangeMkrRef()
{
	UpdateData(TRUE);
    m_cboMkrTxt.UpdateElements( m_cboMkrTxt.pmkrSelected(), m_cboMkrRef.pmkrSelected() ); // reload marker combo in case marker set changed on markers page
}

void CDbTypNumberingPage::OnSelchangeMkrTxt()
{
	UpdateData(TRUE);
	m_cboMkrRef.UpdateElements( m_cboMkrRef.pmkrSelected(), m_cboMkrTxt.pmkrSelected() ); // reload marker combo in case marker set changed on markers page
}

void CDbTypNumberingPage::SetState()
{
	if ( m_bNumberingDisabled )
		{
		m_cboMkrRef.UpdateElements( NULL );
		m_cboMkrRef.EnableWindow( FALSE );
		m_cboMkrTxt.UpdateElements( NULL );
		m_cboMkrTxt.EnableWindow( FALSE );
		}
    else if ( m_ptyp->pnbrproc() == NULL )
		{
		m_cboMkrRef.UpdateElements( NULL );
		m_cboMkrRef.EnableWindow( TRUE );
		m_cboMkrTxt.UpdateElements( NULL );
		m_cboMkrTxt.EnableWindow( TRUE );
		}
	else
		{ // Note: once numbering is set up, it can't be changed!
		m_cboMkrRef.UpdateElements( m_ptyp->pnbrproc()->pmkrRef(), m_ptyp->pnbrproc()->pmkrTxt() );
		m_cboMkrRef.EnableWindow( FALSE );
		m_cboMkrTxt.UpdateElements( m_ptyp->pnbrproc()->pmkrTxt(), m_ptyp->pnbrproc()->pmkrRef() );
		m_cboMkrTxt.EnableWindow( FALSE );
		}
}

BOOL CDbTypNumberingPage::OnSetActive()
{
	ASSERT( m_ptyp->bAllowNumberingOpt() ); 

    m_cboMkrRef.UpdateElements( m_cboMkrRef.pmkrSelected(), m_cboMkrTxt.pmkrSelected() ); // reload marker combo in case marker set changed on markers page
    m_cboMkrTxt.UpdateElements( m_cboMkrTxt.pmkrSelected(), m_cboMkrRef.pmkrSelected() ); // reload marker combo in case marker set changed on markers page

    return CPropertyPage::OnSetActive();
}

void CDbTypNumberingPage::OnOK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;
    UpdateData(TRUE);
//    GetDlgItemText( IDC_TYP_NAME, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_TYP_NAME, sDlgItem ); // 1.4qpw
	m_sDBTypeName = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_TYP_RECORDMARKER, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_TYP_RECORDMARKER, sDlgItem ); // 1.4qpw
	m_sMkrRec = sUTF8( sDlgItem ); // 1.4qpw

	if ( m_bNumberingDisabled )
		m_ptyp->DisableNumbering();
	else
		{
		m_ptyp->EnableNumbering();
		if ( m_ptyp->pnbrproc() == NULL )
			{
			CMarker* pmkrRef;
			CMarker* pmkrTxt;
			pmkrRef = m_cboMkrRef.pmkrSelected();
			pmkrTxt = m_cboMkrTxt.pmkrSelected();
			if ( pmkrRef != NULL && pmkrTxt != NULL )
				{
				ASSERT ( pmkrRef != pmkrTxt );
				m_ptyp->SetupNumbering(pmkrRef, pmkrTxt);
				}
			}
		}
    CPropertyPage::OnOK();
}