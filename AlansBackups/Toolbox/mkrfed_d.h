// mkrfed_d.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEdit window

class CFieldMarkerEdit : public CEdit
{
// Construction
public:
	CFieldMarkerEdit() {}

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CFieldMarkerEdit() {}
	virtual WNDPROC* GetSuperWndProcAddr();

protected:
	// Generated message map functions
	//{{AFX_MSG(CFieldMarkerEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEditDlg dialog

#include "resource.h"
#include "cfield.h"

class CFieldMarkerEditDlg : public CDialog
{
private:
// Dialog Data
	//{{AFX_DATA(CFieldMarkerEditDlg)
	enum { IDD = IDD_FIELD_MKR_EDIT };
	CFieldMarkerEdit m_edtMarker;
	CComboBox* m_combo;
	Str8	m_sMarker;
	//}}AFX_DATA
	
	CMarkerSet* m_pmkrset;
	CField* m_pfld;
	CField** m_ppfldNew;
	RECT m_rcWindowPos;
	CPoint m_pntCaretLoc;
	const CMarker* m_pmkrRecMark; // Record marker to prevent inserting it in a record
	
public:
// Construction
	CFieldMarkerEditDlg( CField* pfld, RECT rcWindowPos, CPoint pntCaretLoc, const CMarker* pmkrRecMark ); // Constructor for editing existing field
	CFieldMarkerEditDlg( CMarkerSet* pmkrset, CField** ppfldNew, RECT rcWindowPos, CPoint pntCaretLoc, const CMarker* pmkrRecMark ); // Constructor for new field

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFieldMarkerEditDlg)
	afx_msg void OnFieldMarkerBackspace();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	virtual BOOL OnInitDialog(); // Override to turn off selection and place caret at end
	void OnOK(); // Override of OnOK
};
