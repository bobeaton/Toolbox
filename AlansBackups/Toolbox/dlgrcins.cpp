// dlgrcins.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "mkr.h"
#include "dlgrcins.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRecInsertDlg dialog


CRecInsertDlg::CRecInsertDlg( CMarker* pmkr, CWnd* pParent /*=NULL*/)
	: CDialog(CRecInsertDlg::IDD, pParent)
{
	m_pmkr = pmkr;
	//{{AFX_DATA_INIT(CRecInsertDlg)
	m_sKey = "";
	//}}AFX_DATA_INIT
}

void CRecInsertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecInsertDlg)
//	DDX_Txt(pDX, IDC_RECORD_INSERT_KEY, m_sKey);
	DDX_Control(pDX, IDC_RECORD_INSERT_KEY, m_edtKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecInsertDlg, CDialog)
	//{{AFX_MSG_MAP(CRecInsertDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRecInsertDlg message handlers

BOOL CRecInsertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_RECORD_INSERT_KEY, swUTF16( m_sKey ) ); // 1.4qpv
	
    m_edtKey.SetFont((CFont*)m_pmkr->pfntDlg());  // must follow OnInitDialog

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecInsertDlg::OnOK()
{
//	GetDlgItemText( IDC_RECORD_INSERT_KEY, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_RECORD_INSERT_KEY, sDlgItem ); // 1.4qpw
	m_sKey = sUTF8( sDlgItem ); // 1.4qpw
	
	CDialog::OnOK();
}

void CRecInsertDlg::OnCancel()
{
	CDialog::OnCancel();
}

