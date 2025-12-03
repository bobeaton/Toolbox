// lng_d.h  Interface for a dialog box representing a language encoding (1996-01-18)

// This class was adapted from MSJ 10/94 pp. 42ff

// NOTE: Additional pages (e.g. Date) should use dialog resources
// exactly the same size (250 x 150) as ID_SET_PAGE with properties
// Child, Thin border, no Title, Disabled. [see MSJ 10/94, p. 42]
// But do NOT disable a resource you intend to use as an ordinary dialog!

// ITEMS FOR FURTHER INVESTIGATION:
// 1. How and where best to cause the property sheet to show
//    only Close and Help buttons, left aligned at the bottom?
// 2. Is there a better way to handle the Close button event?
//    Actually, it is only if we want to allow the user to change
//    the "Filter in use" that we need to notify the individual pages,
//    since additions, modifications, and deletions occur immediately.

#ifndef LNG_D_H
#define LNG_D_H

#include "resource.h"
#include "lng.h"
#include "kbd.h"
#include "set_d.h"
#include "srtset_d.h"
#include "font.h"
//#include "edlngctl.h"
#include "editlng.h"

// --------------------------------------------------------------------------

class CLangEncSetDlg : public CSetDlg  // Hungarian: dlg
{
private:
    CLangEncSet* m_plngset;
    BOOL m_bAutoSwitchingOriginal;

protected:  
// Dialog Data
    //{{AFX_DATA(CLangEncSetDlg)
    enum { IDD = IDD_SET };
    CButton m_chkAutoSwitching;
    BOOL m_bAutoSwitching;
    //}}AFX_DATA

public:
    CLangEncSetDlg(CLangEncSetListBox& lboSet, CLangEncSet* plngset);

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnCancel();

    // Generated message map functions
    //{{AFX_MSG(CLangEncSetDlg)
    afx_msg void OnAutoSwitching();
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CLangEncSetDlg


// --------------------------------------------------------------------------

class CLangEncSheet;

class CCasePage : public CPropertyPage
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active
    CLangEnc* m_plng;
    Str8& m_sCase;
    Str8& m_sPunct; // 1.4qb

    CEditLng m_elcCase; // 1.4qpn
    CEditLng m_elcPunct; // 1.4qpn // 1.4qb Add punc to lng

// Dialog Data
    //{{AFX_DATA(CCasePage)
    enum { IDD = IDD_lng_CasePage };
    CEdit m_edtNote;
    //}}AFX_DATA

// Construction
public:
    CCasePage( CLangEnc* plng, Str8* psCase, Str8* psPunct ); // 1.4qb

    virtual void OnOK();

    BOOL bContainsReferentOfNote(CNote* pnot, CLangEncSheet* pdlg);
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CCasePage)
    virtual BOOL OnInitDialog();
    afx_msg void OnSetfocusCase();
//    afx_msg void OnSetFocus(CWnd* pOldWnd);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

    BOOL OnSetActive(); // called when Case page is switched to

};


// --------------------------------------------------------------------------

class CCharSetPage : public CSetPage  // Hungarian: dlg
{
public:
    CCharSetPage(CCharSetListBox& lboChars, ID idsTitle, ID idd) :
        CSetPage(lboChars, idsTitle, idd) {}

protected:          
    virtual void UpdateButtons();
        // Whenever the number of items in the list box changes
    virtual void UpdateDeleteButton();
};  // class CCharSetPage


// --------------------------------------------------------------------------

class CLangEncOptionsPage : public CPropertyPage
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active
    CLangEnc* m_plng;
    CLangEncSet* m_plngset;
    BOOL m_bAddOrCopy;
    CFontDef m_fntdef; // font definition for language's default font
    Str8& m_sName;
    Str8& m_sDescription;
	Str8 m_sKeymanFile; // Path of Keyman file

// Dialog Data
    //{{AFX_DATA(CLangEncOptionsPage)
	enum { IDD = IDD_LANGENC_OPTIONS_PAGE };
	CStatic	m_lblKeymanFile;
	CButton	m_chkUseInternalKeyboard;
    CEdit m_edtName;
    CEdit m_edtDescription;
    CStatic m_lblKeyboard;
    CKeyboardSetComboBox m_cboKeyboard;
    CEdit m_edtNote;
	//}}AFX_DATA

// Construction
public:
    CLangEncOptionsPage( CLangEnc* plng, BOOL bAddOrCopy,
            Str8* psName, Str8* psDescription );

    virtual void OnOK();

    BOOL bContainsReferentOfNote(CNote* pnot, CLangEncSheet* pdlg);
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CLangEncOptionsPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeKeyboard();
    afx_msg void OnFont();
    afx_msg void OnAdvancedOptions();
	afx_msg void OnBrowseKeymanFile();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


// --------------------------------------------------------------------------

// 1998-02-12 MRP

class CLangEncAdvancedOptionsDlg : public CDialog
{
private:
    CLangEnc* m_plng;

// Dialog Data
    //{{AFX_DATA(CLangEncAdvancedOptionsDlg)
	enum { IDD = IDD_LNG_OPTIONS_ADV };
    BOOL    m_bRightToLeft;
	BOOL	m_bUnicodeLang;
    Str8 m_sRenDLL;
    Str8 m_sRenTable;
	//}}AFX_DATA

public:
    CLangEncAdvancedOptionsDlg(CLangEnc* plng);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CLangEncAdvancedOptionsDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


// **************************************************************************

class CLangEncSheet : public CPropertySheet
{
private:
    CLangEncPtr m_plng;
    Str8 m_sName;
    Str8 m_sDescription;
    Str8 m_sCase;
    Str8 m_sPunct;
    
    BOOL m_bAddOrCopy;

    CSortOrder* m_psrt;
    CSortOrder* m_psrtDefaultOriginal;  
    CSortOrderSetListBox m_lboSortOrders;
    CSortOrderSetPage m_dlgSortOrders;
    
    CCasePage m_dlgCase;

    CVarInstance* m_pvin;   
    CVarInstanceSetListBox m_lboVars;
    CSetPage m_dlgVars;
    
    CMChar* m_pmch;
//    CCharSetListBox m_lboChars; // 5.97j
//    CCharSetPage m_dlgChars; // 5.97j

    CLangEncOptionsPage m_dlgOptions;

public:
    CLangEncSheet(CLangEnc* plng, BOOL bAddOrCopy = FALSE, int iActivePage = 0,
            CSortOrder* psrt = NULL, CVarInstance* pvin = NULL);

    // constants for page selection
    enum { eSortOrderSetPage=0, eCasePage, eVarSetPage, eCharSetPage,
            eOptionsPage };
//  enum { eSortOrderSetPage=0, eVarSetPage, eOptionsPage };

    void SetActivePage(int ePage);

    virtual BOOL OnInitDialog();

protected:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CLangEncSheet)
    afx_msg void OnOK();
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CLangEncSheet)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	void SetButtonLabels();  // 1998-09-30 MRP: For French version
};  // class CLangEncSheet


#endif // LNG_D_H
