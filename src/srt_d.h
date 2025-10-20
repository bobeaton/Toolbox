// srt_d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortEditDlg dialog

#include "resource.h"
#include "srt.h"
#include "edlngctl.h" 
#include "editlng.h"


class CSortEditDlg : public CDialog
{
private:

	CSortOrder* m_psrt;	// Sort order being viewed or added new.
	CSortOrder** m_ppsrtNew;	// means for returning new sort order.
	CSortOrderSet* m_psrtset;	// Owner of this sort order.

	CEdit			m_edtName;
	CEditLng	m_elcPrimaries; // 1.4qpn
	CEditLng	m_elcSecondariesBefore; // 1.4qpn
	CEditLng	m_elcSecondariesAfter; // 1.4qpn
	CEditLng	m_elcIgnores; // 1.4qpn
	CEdit			m_edtNote;
	
// Dialog Data
	//{{AFX_DATA(CSortEditDlg)
	enum { IDD = IDD_SORTEDIT };
	CStatic	m_lblSortCCTable;
	Str8	m_sIgnores;
	Str8	m_sPrimaries;
	Str8	m_sSecondsAfter;
	Str8	m_sSecondsBefore;
	Str8	m_sName;
	Str8	m_sDescribe;
	BOOL	m_bSecondariesAfter;
	//}}AFX_DATA

// Construction
public:
	// CSortEditDlg(CWnd* pParent = NULL);	// standard constructor
	CSortEditDlg(CSortOrderSet* psrtset, CSortOrder** ppsrtNew);
		// A dialog box for adding a new sort order to the set
	CSortEditDlg(const CSortOrder* psrtToCopyFrom, CSortOrder** ppsrtNew);
		// A dialog box for copying an existing sort order to the set
		// by copying from an existing one
	CSortEditDlg(CSortOrder* psrt);
		// A dialog box for viewing and possibly modifying an existing sort order.
	


// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSortEditDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
