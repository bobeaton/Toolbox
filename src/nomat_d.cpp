// nomat_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "resource.h"
#include "nomat_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoMatchesDlg dialog


CNoMatchesDlg::CNoMatchesDlg( const char* pszKey, const CLangEnc* plng,
        BOOL bEnableInsert, BOOL bEnableTryAgain /* = true */ )
    : CDialog(CNoMatchesDlg::IDD)
{
    m_sKey = pszKey;
    m_plng = (CLangEnc*)plng;
    m_bEnableInsert = bEnableInsert;
    m_bEnableTryAgain = bEnableTryAgain;

    //{{AFX_DATA_INIT(CNoMatchesDlg)
	//}}AFX_DATA_INIT
}

const char* CNoMatchesDlg::pszKey() // Return modified key
{
	return m_sKey;
}

void CNoMatchesDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOMATCHES_KEY, m_elcKey);
    //{{AFX_DATA_MAP(CNoMatchesDlg)
    DDX_Control(pDX, IDC_INSERT, m_btnInsert);
    DDX_Control(pDX, IDC_TRYAGAIN, m_btnTryAgain);
//	DDX_Txt(pDX, IDC_NOMATCHES_KEY, m_sKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNoMatchesDlg, CDialog)
    //{{AFX_MSG_MAP(CNoMatchesDlg)
    ON_BN_CLICKED(IDC_INSERT, OnNomatchesInsert)
    ON_BN_CLICKED(IDC_TRYAGAIN, OnNomatchesTryagain)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNoMatchesDlg message handlers

BOOL CNoMatchesDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
	m_elcKey.SetLangEnc(m_plng);
	m_elcKey.SetEditLngText( m_sKey ); // 1.4qpn

//    m_stcKey.SetFont( m_pfnt );
    m_btnInsert.EnableWindow( m_bEnableInsert );
    m_btnTryAgain.EnableWindow( m_bEnableTryAgain );
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNoMatchesDlg::OnNomatchesInsert()
{
	UpdateData(TRUE);  // from dialog controls to member variables
	m_sKey = m_elcKey.sGetEditLngText(); // 1.4qpn

    EndDialog( IDC_INSERT );
}

void CNoMatchesDlg::OnNomatchesTryagain()
{
	UpdateData(TRUE);  // from dialog controls to member variables
	m_sKey = m_elcKey.sGetEditLngText(); // 1.4qpn

    EndDialog( IDC_TRYAGAIN );
}
