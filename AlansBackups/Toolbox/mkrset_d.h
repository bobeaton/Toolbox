// mkrset_d.h  Dialog tabbed page representing a set of markers

#ifndef MKRSET_D_H
#define MKRSET_D_H

#include "set_d.h"  // CSetPage
#include "mkr.h"


// **************************************************************************

class CMarkerSetPage : public CSetPage  // Hungarian: dlg
{
private:
    CMarkerSet* m_pmkrset;
    CLangEncSetComboBox m_cbolngDefault;

public:
    CMarkerSetPage(CMarkerSetListBox& lbomkrset,
            CMarkerSet* pmkrset);
        // A dialog box for viewing the markers and choosing the default.
    ~CMarkerSetPage();  // 1997-12-15 MRP

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CMarkerSetPage)
    afx_msg void OnSelchangeDefaultLangEncCombo();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMarkerSetPage

// **************************************************************************

#endif  // not MKRSET_D_H
