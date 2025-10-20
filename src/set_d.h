// set_d.h  Interface for a dialog box representing a set (1995-06-24)

#ifndef SET_D_H
#define SET_D_H

#include "resource.h"
#include "lbo.h"


// **************************************************************************

class CSetDlg : public CDialog  // Hungarian: dlg
{
private:
    Str8 m_sTitle; // 1.4bb Bring in set dialog title strings from resources

protected:
    CSetListBox& m_lboSet;
    
// Dialog Data
    //{{AFX_DATA(CSetDlg)
    enum { IDD = IDD_SET };
    CButton m_btnAdd;
    CButton m_btnCopy;
    CButton m_btnModify;
    CButton m_btnDelete;
    Str8 m_sLabel;
    //}}AFX_DATA

public:
    CSetDlg(CSetListBox& lboSet, const char* pszTitle, const Str8& sLabel, ID idHelp, ID iddDialog = IDD);
        // A dialog box for viewing the set;
        // adding new elements and viewing or modifying existing ones;
        // and returning the selection when the dialog box was closed.
    
    BOOL bListEmpty()   { return ( m_lboSet.GetCount() <= 0 ); }

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
    virtual void OnCancel();  // 1997-11-15 MRP

    // Generated message map functions
    //{{AFX_MSG(CSetDlg)
    afx_msg void OnClose();
    afx_msg void OnAdd();
    afx_msg void OnCopy();
    afx_msg void OnModify();
    afx_msg void OnDblclkItem();
    afx_msg void OnDelete();
    afx_msg void OnSelchangeList();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    void UpdateButtons();
        // Whenever the number of items in the list box changes
    void UpdateDeleteButton();
};  // class CSetDlg
        
void DlgNotYetImplemented();  // For operations under development


// **************************************************************************

class CSetPage : public CPropertyPage  // Hungarian: dlg
{
private:
    ID m_idsTitle;
    
protected:
    CSetListBox& m_lboSet;
    
// Dialog Data
    //{{AFX_DATA(CSetPage)
    enum { IDD = IDD_SET_PAGE };
    CButton m_btnAdd;
    CButton m_btnCopy;
    CButton m_btnModify;
    CButton m_btnDelete;
    //}}AFX_DATA

public:
    CSetPage(CSetListBox& lboSet, ID idsTitle, ID iddDialog = IDD);
        // A dialog box for viewing the set;
        // adding new elements and viewing or modifying existing ones;
        // and returning the selection when the dialog box was closed.

    virtual void OnOK();            // ok or apply now pressed -- KillActive is called first
    
    BOOL bListEmpty()   { return ( m_lboSet.GetCount() <= 0 ); }

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CSetPage)
    afx_msg void OnAdd();
    afx_msg void OnCopy();
    afx_msg void OnModify();
    afx_msg void OnDblclkItem();
    afx_msg void OnDelete();
    afx_msg void OnSelchangeList();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    
    virtual void UpdateOtherControls();
        // Derived classes can override if any of their controls
        // is affected by changes to the set elements.
        // E.g. Default Sort Order in the Sort Orders page.

    virtual void UpdateButtons();
        // Whenever the number of items in the list box changes
    virtual void UpdateDeleteButton();
};  // class CSetPage

#endif  // SET_D_H
