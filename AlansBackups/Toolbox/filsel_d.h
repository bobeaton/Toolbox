// fil_d.h  Interface for the filter dialog box

#ifndef Filsel_d_h
#define Filsel_d_h

#include "resource.h"
//#include "fil.h"
#include "cbo.h"

class CFilterSet; // forward reference
class CFilter;


class CFilterSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
	CFilterSetComboBox(CFilterSet* pfilset);
	
	void UpdateElements(CFilter* pfilToSelect);
	CFilter* pfilSelected()
		{ return (CFilter*)CSetComboBox::pselSelected(); }
	void ChangeSet(CFilterSet* pfilset, CFilter* pfilToSelect, BOOL bForce=FALSE);
	BOOL bHasChanged(CFilterSet* pfilset); // see if filter set has changed

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
#ifdef _MAC
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
#endif
	DECLARE_MESSAGE_MAP()

};  // class CFilterSetComboBox
                         

/////////////////////////////////////////////////////////////////////////////
// CFilterSelectionDlg dialog

class CFilterSelectionDlg : public CDialog
{
private:
	CFilterSet* m_pfilset;
	CFilter** m_ppfil;

// Construction
public:
	CFilterSelectionDlg(CFilterSet* pfilset, CFilter** ppfil);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFilterSelectionDlg)
	enum { IDD = IDD_FILTERSELECTION };
	CFilterSetComboBox m_cboFilterSet;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CFilterSelectionDlg)
	afx_msg void OnFilselFilters();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // Filsel_d_h
