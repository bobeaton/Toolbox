// typ_d.cpp  Implementation of a dialog box representing a database type (1996-01-13)

#include "stdafx.h"
#include "toolbox.h"
#include "typ_d.h"
#include "ddx.h"
#include "shw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CDatabaseTypeSheet::CDatabaseTypeSheet(CDatabaseType* ptyp, int iActivePage,
        CMarker* pmkr, CFilter* pfil) :
    CPropertySheet(_T(" "), NULL, iActivePage), // 1.4qtu Upgrade CPropertySheet for Unicode build // 1.4sb
    m_ptyp(ptyp),
    m_sDescription(m_ptyp->sDescription()),
    m_pmkr(pmkr),
    m_lboMarkers(m_ptyp->pmkrset(), &m_pmkr),
    m_dlgMarkers(m_lboMarkers, m_ptyp->pmkrset()),
    m_pfil(pfil),
    m_lboFilters(m_ptyp->pfilset(), &m_pfil),
    m_dlgFilters(m_lboFilters, IDS_FILSET_TITLE), // 1.4et OK, CSetPage doesn't use CStrg
    m_pjmp(NULL),
    m_lboJumpPaths(m_ptyp->pjmpset(), (CSetEl **)&m_pjmp),
    m_dlgJumpPaths(m_lboJumpPaths),
    m_dlgOptions(m_ptyp, &m_sDescription),
    m_dlgIntProc(m_ptyp->pintprclst()),
	m_dlgNumbering(m_ptyp)
{
    m_dlgMarkers.SetHelpID(IDD_DBTYP_MARKERS_PAGE);
    AddPage(&m_dlgMarkers);
    ASSERT(eMarkerSetPage == 0);

    m_dlgFilters.SetHelpID(IDD_DBTYP_FILTERS_PAGE);
    AddPage(&m_dlgFilters);
    ASSERT(eFilterSetPage == eMarkerSetPage+1);

    m_dlgJumpPaths.SetHelpID(IDD_DBTYP_JUMPPATH_PAGE);
    AddPage(&m_dlgJumpPaths);
    ASSERT(eJumpPathPage == eFilterSetPage+1);
    
    m_dlgOptions.SetHelpID(IDD_DBTYP_OPTIONS_PAGE); 
    AddPage(&m_dlgOptions);
    ASSERT(eOptionsPage == eJumpPathPage+1);
    
    m_dlgIntProc.SetHelpID(IDD_DBTYP_INTERLINEAR_PAGE); 
    AddPage(&m_dlgIntProc);
    ASSERT(eIntProcPage == eOptionsPage+1);

	// NOTE: The numbering page must be last (see explanation in typ_d.h)
	if ( m_ptyp->bAllowNumberingOpt() )
		{
		m_dlgNumbering.SetHelpID(IDD_DBTYP_NUMBERING_PAGE); 
		AddPage(&m_dlgNumbering);
		ASSERT(eNumberingPage == eIntProcPage+1);
		}
	
    Str8 sTitle = _("Database Type Properties:"); // 1.5.0fg 
	sTitle = sTitle + " " + m_ptyp->sName(); // 1.5.0fg 
    SetTitle( swUTF16( sTitle ) ); // 1.4qtv Upgrade SetTitle for Unicode build
}

BEGIN_MESSAGE_MAP(CDatabaseTypeSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CDatabaseTypeSheet)
    ON_COMMAND(IDOK, OnOK)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

afx_msg void CDatabaseTypeSheet::OnOK()
{
//  m_dlgMarkers.OnOK();
//  m_dlgFilters.OnOK();
    m_dlgJumpPaths.OnOK();
    CMarker* pmkrOriginalDateStamp = m_ptyp->pmkrDateStamp();
    m_dlgOptions.OnOK();
    CMarker* pmkrDateStamp = m_ptyp->pmkrDateStamp();
    m_dlgIntProc.OnOK();
	// NOTE: The numbering page must be last (see explanation in typ_d.h)
	if ( m_ptyp->bAllowNumberingOpt() )
	    m_dlgNumbering.OnOK();
    EndDialog(IDOK);
}

BOOL CDatabaseTypeSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    CButton *pBtn = (CButton*)GetDlgItem(ID_APPLY_NOW);
    ASSERT(pBtn);
    pBtn->ShowWindow(SW_HIDE); // make Apply button do the disappearing act

	SetButtonLabels();  // 1998-09-30 MRP: For French version

    return TRUE;
}

int CDatabaseTypeSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CDatabaseTypeSheet::SetButtonLabels()
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
    sLabel = _("Help");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
}


/////////////////////////////////////////////////////////////////////////////
// CDbTypOptionsPage dialog


CDbTypOptionsPage::CDbTypOptionsPage( CDatabaseType* ptyp, Str8* psDescription )
    : CPropertyPage(CDbTypOptionsPage::IDD),
    m_cboDateStamp( ptyp->pmkrset(), TRUE, ptyp->pmkrRecord() ),
    m_cboTextRef( ptyp->pmkrset(), TRUE, ptyp->pmkrRecord() ),
    m_sDescription(*psDescription)
{
    m_ptyp = ptyp;
    ASSERT( m_ptyp );
    m_bMDF = m_ptyp->bMultiDictionaryFormatter();
    m_bWasActive = FALSE;
}

void CDbTypOptionsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDbTypOptionsPage)
    DDX_Control(pDX, IDC_TYP_NAME, m_lblName);
    DDX_Control(pDX, IDC_TYP_RECORDMARKER, m_lblRecordMkr);
    DDX_Control(pDX, IDC_TYP_EDITDESCRIPTION, m_edtDescription);
    DDX_Control(pDX, IDC_DATESTAMP_COMBO, m_cboDateStamp);
    DDX_Control(pDX, IDC_TEXTREF_COMBO, m_cboTextRef);
    DDX_Check(pDX, IDC_TYP_chkMDF, m_bMDF);
    //}}AFX_DATA_MAP
//    Shw_DDX_MultiLineText(pDX, IDC_TYP_EDITDESCRIPTION, m_sDescription); // 1.4hdv Replace DDX_MultiLineText in db type dlg
}

BEGIN_MESSAGE_MAP(CDbTypOptionsPage, CPropertyPage)
    //{{AFX_MSG_MAP(CDbTypOptionsPage)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDbTypOptionsPage message handlers

BOOL CDbTypOptionsPage::OnInitDialog()
{
    m_bWasActive = TRUE;    
    CPropertyPage::OnInitDialog();
	m_sDescription.Replace( "\n", "\r\n" ); // 1.4hdv Use Replace to change lf to crlf
    SetDlgItemText( IDC_TYP_EDITDESCRIPTION, swUTF16( m_sDescription ) ); // 1.4hdv // 1.4qpv
    m_lblName.SetWindowText(  swUTF16( m_ptyp->sName() ) ); // 1.4qpy
    m_lblRecordMkr.SetWindowText(  swUTF16( m_ptyp->pmkrRecord()->sName() ) ); // 1.4qpy
    m_cboDateStamp.UpdateElements( m_ptyp->pmkrDateStamp() );
    m_cboTextRef.UpdateElements( m_ptyp->pmkrTextRef() );
    return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CDbTypOptionsPage::OnSetActive()
{
    m_cboDateStamp.UpdateElements( m_cboDateStamp.pmkrSelected() ); // reload marker combo in case marker set changed on markers page
    m_cboTextRef.UpdateElements( m_cboTextRef.pmkrSelected() ); // reload marker combo in case marker set changed on markers page

    return CPropertyPage::OnSetActive();
}

void CDbTypOptionsPage::OnOK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;
    UpdateData(TRUE);
//    GetDlgItemText( IDC_TYP_EDITDESCRIPTION, pszBigBuffer, BIGBUFMAX ); // 1.4hdv // 1.4kh
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_TYP_EDITDESCRIPTION, sDlgItem ); // 1.4qpw
	m_sDescription = sUTF8( sDlgItem ); // 1.4qpw
	m_sDescription.Replace( "\r\n", "\n" ); // 1.4hdv Use Replace to change crlf to lf
    m_ptyp->SetDateStampMkr( m_cboDateStamp.pmkrSelected() );
    m_ptyp->SetTextRefMkr( m_cboTextRef.pmkrSelected() );
    m_ptyp->SetDescription( m_sDescription );
    m_ptyp->SetMultiDictionaryFormatter(m_bMDF);    
    CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CNewDbTypDlg dialog


CNewDbTypDlg::CNewDbTypDlg( CDatabaseTypeSet* ptypset, Str8* psName, Str8* psRecordMkr,
                            CMarkerSet* pmkrset )
    : CDialog(CNewDbTypDlg::IDD, NULL),
    m_sName( *psName ),
    m_sRecordMkr( *psRecordMkr )
{
    m_ptypset = ptypset;
    m_pmkrset = pmkrset;
    //{{AFX_DATA_INIT(CNewDbTypDlg)
    //}}AFX_DATA_INIT
}

void CNewDbTypDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CNewDbTypDlg)
    DDX_Control(pDX, IDC_TYP_NAME_EDIT, m_edtName);
    DDX_Control(pDX, IDC_TYP_RECORDMKR_COMBO, m_cboRecordMkr);
    //}}AFX_DATA_MAP
//    m_pmkrset->DDX_Name(pDX, IDC_TYP_RECORDMKR_COMBO, m_sRecordMkr); // 1.4hdn Replace DDX_Name in new database type dlg
//    Shw_DDX_TrimmedTxt(pDX, IDC_TYP_NAME_EDIT, m_sName); // 1.4hda Replace DDX_Trimmed in new database type dlg
}

BEGIN_MESSAGE_MAP(CNewDbTypDlg, CDialog)
    //{{AFX_MSG_MAP(CNewDbTypDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewDbTypDlg message handlers

BOOL CNewDbTypDlg::OnInitDialog()
{
    CDialog::OnInitDialog();   
    SetDlgItemText( IDC_TYP_RECORDMKR_COMBO, swUTF16( m_sRecordMkr  ) ); // 1.4hdn // 1.4qpv
    SetDlgItemText( IDC_TYP_NAME_EDIT, swUTF16( m_sName  ) ); // 1.4hda // 1.4qpv
    if ( m_pmkrset ) // copying from existing database type, fill combo with marker set
        {
        CMarker* pmkr = m_pmkrset->pmkrFirst();
        for ( ; pmkr; pmkr = m_pmkrset->pmkrNext( pmkr ) )
            m_cboRecordMkr.InsertString( -1, swUTF16( pmkr->sName() ) ); // 1.4qre
        }   
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewDbTypDlg::OnOK()
{
    UpdateData( TRUE ); // get data from controls to member variables    
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_TYP_RECORDMKR_COMBO, sDlgItem ); // 1.4qpw
	m_sRecordMkr = sUTF8( sDlgItem ); // 1.4qpw
	RemoveBackslashAtFront( m_sRecordMkr ); // 1.4hdn
	GetDlgItemText( IDC_TYP_NAME_EDIT, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdc
    if ( !m_sName.GetLength() || m_ptypset->ptrxSearch( m_sName ) ) // valid name?
        {
        AfxMessageBox( _("There is an existing database type using this name.  Please enter a different name.") );
        m_edtName.SetFocus();
        return;
        }
    CNote* pnot = NULL;
    if ( !CMarker::s_bValidName( m_sRecordMkr, &pnot ) ) // If not valid syntactically, give message and make them fix it
        {
        pnot->SetStringContainingReferent( m_sRecordMkr ); // Inform note of beginning of string
        DWORD dwSel = pnot->dwSelReferent();
        m_cboRecordMkr.SetFocus();
        m_cboRecordMkr.SetEditSel( LOWORD(dwSel), HIWORD(dwSel) ); // Select problem character
        int iAnswer = AfxMessageBox( pnot->sNote(), MB_OKCANCEL ); // Give message
        delete pnot; // If failed, delete dynamically created note
        return;
        }
    CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CDbTypRecMkrDlg dialog


CDbTypRecMkrDlg::CDbTypRecMkrDlg(CDatabaseType* ptyp)
    : CDialog(CDbTypRecMkrDlg::IDD, AfxGetMainWnd())
{
    m_ptyp = ptyp;
    //{{AFX_DATA_INIT(CDbTypRecMkrDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDbTypRecMkrDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDbTypRecMkrDlg)
    DDX_Control(pDX, IDC_TYP_RECMKR, m_cboRecordMkr);
    //}}AFX_DATA_MAP
//    m_ptyp->pmkrset()->DDX_Name(pDX, IDC_TYP_RECMKR, m_sRecordMkr); // 1.4hdp Replace DDX_Name in record marker dlg
}

BEGIN_MESSAGE_MAP(CDbTypRecMkrDlg, CDialog)
    //{{AFX_MSG_MAP(CDbTypRecMkrDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDbTypRecMkrDlg message handlers

BOOL CDbTypRecMkrDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_TYP_RECMKR, swUTF16( m_sRecordMkr ) ); // 1.4hdp // 1.4qpv
//	char buffer[DLGBUFMAX+1];
//	GetWindowText( (char*)buffer, DLGBUFMAX ); // 1.4ec Change to buffer // 1.4qpx Eliminate GetWindowText
//	Str8 sTitle = buffer; // get existing text // 1.4qpx
	Str8 sTitle = _("Database Type"); // 1.4qpx Use internal string instead of GetWindowText // 1.5.0ft 
    sTitle = sTitle + " - " + m_ptyp->sName(); // add type name // 1.5.0ft 
    SetWindowText( swUTF16( sTitle) ); // 1.4qpy
    CMarker* pmkr = m_ptyp->pmkrset()->pmkrFirst(); // fill combo from marker set
    for ( ; pmkr; pmkr = m_ptyp->pmkrset()->pmkrNext( pmkr ) )
        m_cboRecordMkr.InsertString( -1, swUTF16( pmkr->sName()  ) ); // 1.4qre
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDbTypRecMkrDlg::OnOK()
{
    UpdateData(TRUE);
//    GetDlgItemText( IDC_TYP_RECMKR, (char*)buffer, DLGBUFMAX ); // 1.4hdp
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_TYP_RECMKR, sDlgItem ); // 1.4qpw
	m_sRecordMkr = sUTF8( sDlgItem ); // 1.4qpw
	RemoveBackslashAtFront( m_sRecordMkr ); // 1.4hdp
    CNote* pnot = NULL;
    if ( !CMarker::s_bValidName( m_sRecordMkr, &pnot ) ) // If not valid syntactically, give message and make them fix it
        {
        pnot->SetStringContainingReferent( m_sRecordMkr ); // Inform note of beginning of string
        DWORD dwSel = pnot->dwSelReferent();
        m_cboRecordMkr.SetFocus();
        m_cboRecordMkr.SetEditSel( LOWORD(dwSel), HIWORD(dwSel) ); // Select problem character
        int iAnswer = AfxMessageBox( pnot->sNote(), MB_OKCANCEL ); // Give message
        delete pnot; // If failed, delete dynamically created note
        return;
        }
    m_ptyp->SetRecordMarker( m_sRecordMkr );
    
    CDialog::OnOK();
}

void CDbTypRecMkrDlg::OnCancel()
{
    AfxMessageBox( _("You must enter the record marker for this database type.") );
}
