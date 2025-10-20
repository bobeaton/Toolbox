// seldb_d.h : header file
//

#ifndef Seldb_d_h
#define Seldb_d_h

#include "spath_d.h"

class CDatabaseRef; // forward reference

/////////////////////////////////////////////////////////////////////////////
// CSelectDbDlg dialog

class CSelectDbDlg : public CDialog
{
private:
	const CDatabaseRefList* m_pdrflst; // used to load up list box with documents from jump path
	CShwDoc** m_ppdoc;
	BOOL m_bExitOnOne;  // TRUE if dialog should automatically exit if given only one file
    BOOL m_bOverrideLabel;

// Construction
public:
    enum EAction { actCopy, actMove, actInsert, actMerge };

	CSelectDbDlg( const CDatabaseRefList* pdrflst, CShwDoc** ppdoc, EAction act, BOOL bExitOnOne = FALSE );
		// ppdoc is used to return selected database

// Dialog Data
	//{{AFX_DATA(CSelectDbDlg)
	enum { IDD = IDD_SELECT_DB, IDD_MERGE = IDD_SELECT_MERGE_DB };
    Str8 m_sSelectDB;
	CButton	m_chkShowPath;
	CDocListBox	m_lboDbsAvailable;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSelectDbDlg)
	afx_msg void OnSelectdbShowpath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkSelectdbListbox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // Seldb_d_h
