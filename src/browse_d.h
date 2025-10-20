// browse_d.h : header file
// Brian Yoder  10-2-95

#ifndef Browse_d_h
#define	Browse_d_h

#include "browse.h"

/////////////////////////////////////////////////////////////////////////////
// CBrowseFieldsDlg dialog

class CBrowseFieldsDlg : public CDialog
{
private:
	CMarkerSet* m_pmkrset;	// The set of markers for the document in question.
	CBrowseFieldList* m_pbrflst; // List of fields to display in browse view.
	
// Dialog Data
	//{{AFX_DATA(CBrowseFieldsDlg)
	enum { IDD = IDD_BROWSEFIELDS };
      
    Str8 m_sNoField;
    Str8 m_sEmptyField;
	//}}AFX_DATA
	
	void LoadLBMkrsAvailable(const char* pszMrkSel=NULL);
// Construction
public:
	CBrowseFieldsDlg( CMarkerSet* pmkrset, CBrowseFieldList* pbrflst );
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CBrowseFieldsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseFieldMovefrom();
	afx_msg void OnBrowseFieldMoveto();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkBrowseFieldLstfldstoBrowse();
	afx_msg void OnDblclkBrowseFieldLstfldsavailable();
	afx_msg void OnBrowseFieldFirstto();
	afx_msg void OnBrowseFieldLastto();
	afx_msg void OnBrowseFieldClearBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif Browse_d_h
