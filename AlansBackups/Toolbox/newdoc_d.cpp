// newdoc_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "project.h"
#include "newdoc_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg dialog


CNewDocDlg::CNewDocDlg( const char* pszFileName, CDatabaseTypeProxy** pptrx )
	: CDialog(CNewDocDlg::IDD, NULL),
	m_cboDatabaseType( Shw_ptypset() )
{
	m_pptrx = pptrx;
	m_sFileName = pszFileName;

	//{{AFX_DATA_INIT(CNewDocDlg)
	//}}AFX_DATA_INIT
}

void CNewDocDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDocDlg)
	DDX_Control(pDX, IDC_NEWDOC_DBTYP, m_cboDatabaseType);
//	DDX_Txt(pDX, IDC_LBL_FILENAME, m_sFileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNewDocDlg, CDialog)
	//{{AFX_MSG_MAP(CNewDocDlg)
	ON_BN_CLICKED(IDC_DATABASE_TYPES, OnDatabaseTypes)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg message handlers

BOOL CNewDocDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_LBL_FILENAME, swUTF16( m_sFileName ) ); // 1.4qpv
	
	m_cboDatabaseType.UpdateElements( Shw_ptypset()->ptrxFirst() );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNewDocDlg::OnDatabaseTypes()
{
	CDatabaseTypeProxy* ptrx = m_cboDatabaseType.ptrxSelected();

	Shw_ptypset()->ViewElements(&ptrx); // let 'em go add a new one
	
	m_cboDatabaseType.UpdateElements( ptrx ); // different type may be selected
}

void CNewDocDlg::OnOK()
{
	CDatabaseTypeProxy* ptrx = m_cboDatabaseType.ptrxSelected();
	if ( !ptrx )
		{
		AfxMessageBox( _("You must choose a Database Type.") );
		return;
		}
	*m_pptrx = ptrx; // return selected type to caller
//	GetDlgItemText( IDC_LBL_FILENAME, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_LBL_FILENAME, sDlgItem ); // 1.4qpw
	m_sFileName = sUTF8( sDlgItem ); // 1.4qpw
	
	CDialog::OnOK();
}
