// hide_d.h : header file

#ifndef Hide_d_h
#define	Hide_d_h

#include "cdbllist.h" // For derivation
#include "cdblel.h" // For derivation
#include "mkr.h"
#include "crecord.h"

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h

/////////////////////////////////////////////////////////////////////////////
// CHideFieldsDlg dialog

class CHideFieldsDlg : public CDialog
{
private:
	CMarkerSet* m_pmkrset;	// The set of markers for the document in question.
	CFieldList* m_pbrflst; // List of fields to display in hide view.
	
// Dialog Data
	//{{AFX_DATA(CHideFieldsDlg)
	enum { IDD = IDD_HIDEFIELDS };
      
      //09-23-1997
    Str8 m_sNoField;
    Str8 m_sEmptyField;
	//}}AFX_DATA
	
	void LoadLBMkrsAvailable(const char* pszMrkSel=NULL);
// Construction
public:
	CHideFieldsDlg( CMarkerSet* pmkrset );
	
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CHideFieldsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHideFieldMovefrom();
	afx_msg void OnHideFieldMoveto();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkHideFieldLstfldstoHide();
	afx_msg void OnDblclkHideFieldLstfldsavailable();
	afx_msg void OnHideFieldFirstto();
	afx_msg void OnHideFieldLastto();
	afx_msg void OnHideFieldClearHide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif Hide_d_h
