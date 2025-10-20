// newdoc_d.h : header file
//

#ifndef Newdoc_d_h
#define Newdoc_d_h

#include "typ_d.h"

/////////////////////////////////////////////////////////////////////////////
// CNewDocDlg dialog

class CNewDocDlg : public CDialog
{
private:
	CDatabaseTypeProxy** m_pptrx;

// Construction
public:
	CNewDocDlg( const char* pszFileName, CDatabaseTypeProxy** pptrx );

// Dialog Data
	//{{AFX_DATA(CNewDocDlg)
	enum { IDD = IDD_NEWDOC };
	CDatabaseTypeSetComboBox m_cboDatabaseType;
	Str8	m_sFileName;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CNewDocDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDatabaseTypes();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // Newdoc_d_h