// page_d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg dialog

class CPageSetupDlg : public CDialog
{
// Construction
public:
	CPageSetupDlg(CPrintProperties* pptp, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSetupDlg)
	enum { IDD = IDD_PAGE_SETUP };
	Str8	m_sFooter;
	BOOL	m_bPageBreakRecord;
	Str8	m_sHeader;
	Str8	m_sMargBottom;
	Str8	m_sMargLeft;
	Str8	m_sMargRight;
	Str8	m_sMargTop;
	//}}AFX_DATA
	
	BOOL	m_bPrintWholeView;
	CPrintProperties* m_pptp;

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK( );


	// Generated message map functions
	//{{AFX_MSG(CPageSetupDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
