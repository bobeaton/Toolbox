// var_d.h  Interface for the variable dialog box

#include "resource.h"
#include "var.h"
// #include "edlngctl.h"
#include "editlng.h"
                         

class CVarInstanceDlg : public CDialog
{
private:
	CVarInstance* m_pvin;  // the variable being viewed or added new
	CVarInstance** m_ppvinNew;  // means by which to return new variable
	CVarInstanceSet* m_pvinset;  // variable set which owns the variable
	Str8 m_sOriginalName;
	
// Dialog Data
	//{{AFX_DATA(CMarkerDlg)
	enum { IDD = IDD_VAR };
	Str8	m_sName;
	CEdit	m_edtName;
	Str8	m_sChars;
	CEdit	m_edtNote;
	//}}AFX_DATA
	CEditLng m_elcChars; // 1.4qpn

public:
	CVarInstanceDlg( CVarInstanceSet* pvinset, CVarInstance** ppvinNew );
		// A dialog box for adding a new variable to the set
	CVarInstanceDlg( const CVarInstance* pvinToCopyFrom, CVarInstance** ppvinNew );
		// A dialog box for copying an existing variable to the set
		// by copying from an existing one
	CVarInstanceDlg( CVarInstance* pvin );
		// A dialog box for viewing and possibly modifying an existing variable
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CMarkerDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
