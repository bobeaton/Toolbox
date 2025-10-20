// tplt_d.h : header file
//

#ifndef Tplt_d_h
#define	Tplt_d_h

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg dialog

class CTemplateDlg : public CDialog
{
	BOOL* m_pbIncludeContents;

// Construction
public:
	CTemplateDlg( BOOL* pbIncludeContents );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTemplateDlg)
	enum { IDD = IDD_TEMPLATE };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CTemplateDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // Tplt_d_h
