// dlgrcins.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecInsertDlg dialog

class CRecInsertDlg : public CDialog
{
private:
	CMarker* m_pmkr;

// Construction
public:
	CRecInsertDlg( CMarker* pmkr, CWnd* pParent = NULL );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRecInsertDlg)
	enum { IDD = IDD_RECORD_INSERT };
	CEdit	m_edtKey;
	Str8	m_sKey;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CRecInsertDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
