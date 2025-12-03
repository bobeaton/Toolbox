// srtset_d.h  Dialog tabbed page representing a set of sort orders

#ifndef SRTSET_D_H
#define SRTSET_D_H

#include "set_d.h"
#include "srt.h"

// **************************************************************************

class CSortOrderSetPage : public CSetPage  // Hungarian: dlg
{
private:
	CSortOrderSet* m_psrtset;
	CSortOrderSetComboBox m_cbosrtDefault;

public:
	CSortOrderSetPage(CSortOrderSetListBox& lbosrtset,
			CSortOrderSet* psrtset);
		// A dialog box for viewing the sort orders and choosing the default.

protected:			
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CSortOrderSetPage)
	afx_msg void OnSelchangeDefaultSortOrderCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
	virtual void UpdateOtherControls();
		// Overrided so that the Default Sort Order combo box
		// if the name of its selected element is modified.
};  // class CSortOrderSetPage

// **************************************************************************

#endif  // not SRTSET_D_H
