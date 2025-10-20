// page_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "printinf.h"
#include "page_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg dialog


CPageSetupDlg::CPageSetupDlg(CPrintProperties* pptp, CWnd* pParent /*=NULL*/)
	: CDialog(CPageSetupDlg::IDD, pParent)
{
	ASSERT(pptp);
	m_pptp = pptp;

	//{{AFX_DATA_INIT(CPageSetupDlg)
	m_sFooter = pptp->sFooter();
	m_sHeader = pptp->sHeader();
	m_bPageBreakRecord = pptp->bBreakBetweenRecords();
	m_sMargBottom = pptp->sBottomMargin();
	m_sMargLeft = pptp->sLeftMargin();
	m_sMargRight = pptp->sRightMargin();
	m_sMargTop = pptp->sTopMargin();
	//}}AFX_DATA_INIT
	
	m_bPrintWholeView = pptp->bPrintingAllRecords(); 
}

void CPageSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetupDlg)
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_FOOTER, m_sFooter);
	DDX_Check(pDX, IDC_PAGESETUP_CHK_PAGEBREAKS, m_bPageBreakRecord);
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_HEADER, m_sHeader);
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_MARGBOTTOM, m_sMargBottom);
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_MARGLEFT, m_sMargLeft);
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_MARGRIGHT, m_sMargRight);
//	DDX_Txt(pDX, IDC_PAGESETUP_TXT_MARGTOP, m_sMargTop);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPageSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CPageSetupDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg message handlers


BOOL CPageSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_PAGESETUP_TXT_FOOTER, swUTF16( m_sFooter ) ); // 1.4qpv
	SetDlgItemText( IDC_PAGESETUP_TXT_HEADER, swUTF16( m_sHeader ) ); // 1.4qpv
	SetDlgItemText( IDC_PAGESETUP_TXT_MARGBOTTOM, swUTF16( m_sMargBottom ) ); // 1.4qpv
	SetDlgItemText( IDC_PAGESETUP_TXT_MARGLEFT, swUTF16( m_sMargLeft ) ); // 1.4qpv
	SetDlgItemText( IDC_PAGESETUP_TXT_MARGRIGHT, swUTF16( m_sMargRight ) ); // 1.4qpv
	SetDlgItemText( IDC_PAGESETUP_TXT_MARGTOP, swUTF16( m_sMargTop ) ); // 1.4qpv

	CButton *pbtnWholeView = (CButton*)GetDlgItem(IDC_PAGESETUP_RB_PRINTVIEW);
	CButton *pbtnRecordOnly = (CButton*)GetDlgItem(IDC_PAGESETUP_RB_PRINTREC);

	pbtnWholeView->SetCheck((m_bPrintWholeView) ? 1 : 0);
	pbtnRecordOnly->SetCheck((!m_bPrintWholeView) ? 1 : 0);

	return TRUE;
}


void CPageSetupDlg::OnOK()
{
	UpdateData();
//	GetDlgItemText( IDC_PAGESETUP_TXT_FOOTER, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_PAGESETUP_TXT_FOOTER, sDlgItem ); // 1.4qpw
	m_sFooter = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PAGESETUP_TXT_HEADER, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PAGESETUP_TXT_HEADER, sDlgItem ); // 1.4qpw
	m_sHeader = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PAGESETUP_TXT_MARGBOTTOM, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PAGESETUP_TXT_MARGBOTTOM, sDlgItem ); // 1.4qpw
	m_sMargBottom = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PAGESETUP_TXT_MARGLEFT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PAGESETUP_TXT_MARGLEFT, sDlgItem ); // 1.4qpw
	m_sMargLeft = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PAGESETUP_TXT_MARGRIGHT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PAGESETUP_TXT_MARGRIGHT, sDlgItem ); // 1.4qpw
	m_sMargRight = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PAGESETUP_TXT_MARGTOP, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PAGESETUP_TXT_MARGTOP, sDlgItem ); // 1.4qpw
	m_sMargTop = sUTF8( sDlgItem ); // 1.4qpw

	CButton *pbtnWholeView = (CButton*)GetDlgItem(IDC_PAGESETUP_RB_PRINTVIEW);
	CButton *pbtnRecordOnly = (CButton*)GetDlgItem(IDC_PAGESETUP_RB_PRINTREC);

	BOOL bPrintWholeView = (pbtnWholeView->GetCheck() == 1);

	ASSERT( !( (pbtnRecordOnly->GetCheck() == 1) && bPrintWholeView) );

	// Check to see that the data is valid, that the user entered things sensible.
	// if not valid then beep and switch to that control.
	if (!m_pptp->phdrlstHeader()->bParse(m_pptp, NULL, m_sHeader, TRUE))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_HEADER)->SetFocus();
		return;
	}

	if (!m_pptp->phdrlstFooter()->bParse(m_pptp, NULL, m_sHeader, TRUE))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_FOOTER)->SetFocus();
		return;
	}

	// Validate the margin information.
	if (!m_pptp->bValidateMeasure(m_sMargTop))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_MARGTOP)->SetFocus();
		return;
	}

	if (!m_pptp->bValidateMeasure(m_sMargLeft))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_MARGLEFT)->SetFocus();
		return;
	}


	if (!m_pptp->bValidateMeasure(m_sMargBottom))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_MARGBOTTOM)->SetFocus();
		return;
	}


	if (!m_pptp->bValidateMeasure(m_sMargRight))
	{
		MessageBeep(0);
		GetDlgItem(IDC_PAGESETUP_TXT_MARGRIGHT)->SetFocus();
		return;
	}




	// Set data as appropriate.
	m_pptp->SetTopMargin(m_sMargTop);
	m_pptp->SetLeftMargin(m_sMargLeft);
	m_pptp->SetBottomMargin(m_sMargBottom);
	m_pptp->SetRightMargin(m_sMargRight);

	if (bPrintWholeView)
		m_pptp->PrintAllRecords();
	else
		m_pptp->PrintCurrentRecord();

	m_pptp->SetBreakBetweenRecords(m_bPageBreakRecord);

	m_pptp->SetHeader(m_sHeader);
	m_pptp->SetFooter(m_sFooter);

	CDialog::OnOK();

}