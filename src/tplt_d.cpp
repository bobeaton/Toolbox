// tplt_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "tplt_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

CTemplateDlg::CTemplateDlg( BOOL* pbIncludeContents )
	: CDialog(CTemplateDlg::IDD, NULL)
{
	m_pbIncludeContents = pbIncludeContents;
	//{{AFX_DATA_INIT(CTemplateDlg)
	//}}AFX_DATA_INIT
}

void CTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemplateDlg)
	DDX_Check(pDX, IDC_INCLUDE_CONTENTS, *m_pbIncludeContents);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CTemplateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg message handlers

BOOL CTemplateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}



