// dlgimpt.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "project.h"
#include "shwdoc.h"		// enum constants for import mode
#include "dlgimpt.h"
#include "shw.h"
#include "alan.h" // 1.5.8h 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog


CImportDlg::CImportDlg( const char* pszPathName, Str8 &sFileText,
					const int nMode, const Str8& sMissingDBType,
					CDatabaseTypeProxy** pptrx, BOOL* pbMakeOri ) :
	CDialog(CImportDlg::IDD),
	m_sFileText( sFileText ),
	m_nMode (nMode),
	m_sMissingDBType (sMissingDBType),
	m_cboDatabaseType( Shw_ptypset() )
{
	m_sSourceFile = pszPathName;
	m_pptrx = pptrx;
	m_bRemoveSpaces = Shw_pProject()->bRemoveSpaces();
	m_pbMakeOri = pbMakeOri;
	m_bMakeOri = *m_pbMakeOri;

	//{{AFX_DATA_INIT(CImportDlg)
	//}}AFX_DATA_INIT
}

void CImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportDlg)
//	DDX_Txt(pDX, IDC_SOURCEFILE, m_sSourceFile);
//	DDX_Txt(pDX, IDC_IMPORT_FILEVIEW, m_sFileText);
	DDX_Control(pDX, IDC_lblWhyImport, m_lblWhy);
	DDX_Control(pDX, IDC_IMPORT_DBTYP, m_cboDatabaseType);
	DDX_Control(pDX, IDC_Import_CCGroup, m_grpCCStuff);
	DDX_Control(pDX, IDC_USE_CCTABLE, m_chkUseCCTable);
	DDX_Control(pDX, IDC_LBL_CCTABLE, m_lblCCTable);
	DDX_Control(pDX, IDC_BROWSE_CCTABLE, m_bnBrowseCCTable);
	DDX_Check  (pDX, IDC_Import_chkRemoveSpaces, m_bRemoveSpaces);
	DDX_Control(pDX, IDC_Import_chkRemoveSpaces, m_chkRemoveSpaces);
	DDX_Check  (pDX, IDC_BACKUP_ORI, m_bMakeOri);
	DDX_Control(pDX, IDC_BACKUP_ORI, m_chkMakeOri);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImportDlg, CDialog)
	//{{AFX_MSG_MAP(CImportDlg)
	ON_BN_CLICKED(IDC_DATABASE_TYPES, OnAddNewType)
	ON_BN_CLICKED(IDC_BROWSE_CCTABLE, OnBrowseCctable)
	ON_BN_CLICKED(IDC_USE_CCTABLE, OnUseCctable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CImportDlg message handlers

BOOL CImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_SOURCEFILE, swUTF16( m_sSourceFile ) ); // 1.4qpv
	SetDlgItemText( IDC_IMPORT_FILEVIEW, swUTF16( m_sFileText ) ); // 1.4qpv
	
	m_cboDatabaseType.UpdateElements( Shw_ptypset()->ptrxFirst() );
	
	// set up import CC table controls
	BOOL bUseImportCCT = Shw_pProject()->bUseImportCCT();
	m_sCCTable = Shw_pProject()->pszImportCCT(); // Get name of CC table
	if ( !bCCTableExists() ) // If table doesn't exist, clear its name, 
							 // and don't try to use it
		{
		bUseImportCCT = FALSE;
		m_sCCTable = ""; // if not valid file, trash it
		}
	m_chkUseCCTable.SetCheck( bUseImportCCT );
	if ( !m_sCCTable.IsEmpty() )
		m_lblCCTable.SetWindowText(  swUTF16( sGetFileName(m_sCCTable) ) ); //name only // 1.4qpy
	else
		{
		Str8 sNone;
		sNone = _("None");
	    m_lblCCTable.SetWindowText(  swUTF16( sNone ) ); // 1.4qpy
	    }

	// set up for DB Type Not Found mode
	if (m_nMode == CShwDoc::iDBTypeNotFound)
		{
		Str8 sWhy = _("Database type not found"); // 1.5.0fg 
		m_lblWhy.SetWindowText( swUTF16( sWhy) ); // 1.4qpy
		// disable inappropriate controls for this mode
		m_grpCCStuff.EnableWindow(FALSE);
		m_chkUseCCTable.SetCheck(FALSE);
		m_chkUseCCTable.EnableWindow(FALSE);
		m_lblCCTable.EnableWindow(FALSE);
		m_bnBrowseCCTable.EnableWindow(FALSE);
		m_chkRemoveSpaces.SetCheck(FALSE);
		m_chkRemoveSpaces.EnableWindow(FALSE);
		m_chkMakeOri.SetCheck(FALSE);
		m_chkMakeOri.EnableWindow(FALSE);
		}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImportDlg::OnOK()
{
	UpdateData(TRUE);	// retrieve data from dialog
//	GetDlgItemText( IDC_SOURCEFILE, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_SOURCEFILE, sDlgItem ); // 1.4qpw
	m_sSourceFile = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_IMPORT_FILEVIEW, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_IMPORT_FILEVIEW, sDlgItem ); // 1.4qpw
	m_sFileText = sUTF8( sDlgItem ); // 1.4qpw

	CDatabaseTypeProxy* ptrx = m_cboDatabaseType.ptrxSelected();
	if ( !ptrx )
		{			// no Type selected
		AfxMessageBox( _("You must choose a Database Type.") );
		return;
		}
	
	CDialog::OnOK();  // retrieve again; plus calls EndDialog()

	*m_pptrx = ptrx; // return selected type to caller
	
	if (m_nMode != CShwDoc::iDBTypeNotFound)
		{
		Shw_pProject()->SetImportCCT( m_sCCTable, m_chkUseCCTable.GetCheck() ); // save CC table settings
		Shw_pProject()->SetRemoveSpaces( m_bRemoveSpaces );
		*m_pbMakeOri = m_bMakeOri;
		}
}

void CImportDlg::OnAddNewType()
{
	CDatabaseTypeProxy* ptrx = m_cboDatabaseType.ptrxSelected();

	Shw_ptypset()->ViewElements(&ptrx); // let 'em go add a new one
	
	m_cboDatabaseType.UpdateElements( ptrx ); // different type may be selected
}

void CImportDlg::OnBrowseCctable()
{
	if ( bGetCCTable() )
		m_chkUseCCTable.SetCheck( TRUE );
}

BOOL CImportDlg::bCCTableExists()
{
	if ( m_sCCTable.GetLength() )
		{
		if ( bFileExists( m_sCCTable ) ) // does cc table exist
			return TRUE;
		}
	return FALSE;
}

BOOL CImportDlg::bGetCCTable( BOOL bOnlyIfNecessary )
{
	if ( bOnlyIfNecessary && bCCTableExists() )
		return TRUE;

	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		long lFlags = (OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);
		Str8 sCCTFilter = _("Change Table"); // 1.5.0fz 
		sCCTFilter += " (*.cct)|*.cct||"; // 1.5.0fz 
		CFileDialog dlgCCTable(TRUE,  swUTF16( "cct" ), swUTF16( sGetFileName(m_sCCTable) ), lFlags, swUTF16( sCCTFilter ) ); // 1.4qrd // 1.5.8h 
		Str8 sInitialDir = sGetDirPath(m_sCCTable);
		dlgCCTable.m_ofn.lpstrInitialDir =  swUTF16( sInitialDir ); // 1.4qra
		dlgCCTable.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		dlgCCTable.SetHelpID(IDD_SELECT_CCTABLE);
		if ( iDlgDoModalSetReg( &dlgCCTable ) != IDOK )
			return FALSE;
		sPath =  sUTF8( dlgCCTable.GetPathName() ); // 1.4qxf
		m_sCCTable =  sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sCCTable) ) // 1.5.8h 
			return FALSE; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return FALSE; // 1.4vze 

	m_lblCCTable.SetWindowText(  swUTF16( sGetFileName(m_sCCTable) ) ); // 1.4qpy
	return TRUE;
}

void CImportDlg::OnUseCctable()
{
	if ( m_chkUseCCTable.GetCheck() ) // just got checked on
		if ( !bGetCCTable( TRUE ) )
			m_chkUseCCTable.SetCheck( FALSE );
}



/////////////////////////////////////////////////////////////////////////////
// CDBTypeNFDlg dialog

CDBTypeNFDlg::CDBTypeNFDlg(const char* pszPathName, 
						   const Str8 &sMissingDBType) : 
    // Added AfxGetMainWnd() to prevent assert in APPUI due to bogus parent window (C++ 6.00)
    // See also: 1999-04-01 TLB and TLB 06-29-1999
	CDialog(CDBTypeNFDlg::IDD, AfxGetMainWnd()),  // 2000-03-16 MRP
	m_sMissingDBType (sMissingDBType)
{
	m_pszPathName = pszPathName;

	ASSERT(*m_pszPathName);
	ASSERT(!m_sMissingDBType.IsEmpty());

	//{{AFX_DATA_INIT(CDBTypeNFDlg)
	//}}AFX_DATA_INIT
}

CDBTypeNFDlg::~CDBTypeNFDlg()
{
}

void CDBTypeNFDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDBTypeNFDlg)
    DDX_Control(pDX, IDC_DBTypeNF_FileAndType, m_lblToPrint);
    //}}AFX_DATA_MAP
}


/////////////////////////////////////////////////////////////////////////////
// CDBTypeNFDlg message handlers

BOOL CDBTypeNFDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();

    Str8 sMessage; // 1.5.0fd
	sMessage = sMessage + _("Cannot find database type:") + " " + m_sMissingDBType; // 1.5.0fd 
    m_lblToPrint.SetWindowText( swUTF16( sMessage) ); // 1.4qpy

    return TRUE;	// return T unless you set focus to a control
}
