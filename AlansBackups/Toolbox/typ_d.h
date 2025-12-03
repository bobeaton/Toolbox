// typ_d.h  Interface for a dialog box representing a database type (1996-01-13)

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

#ifndef Typ_d_h
#define Typ_d_h

#include "resource.h"
#include "typ.h"
#include "set_d.h"
#include "mkrset_d.h"  // CMarkerSetPage
#include "spath_d.h"
#include "ptr.h"
#include "intprc_d.h"
#include "number_d.h"


/////////////////////////////////////////////////////////////////////////////
// CDbTypOptionsPage dialog

class CDbTypOptionsPage : public CPropertyPage
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active

    Str8& m_sDescription;
    BOOL m_bMDF;

public:
    CDatabaseType* m_ptyp;

// Construction
public:
    CDbTypOptionsPage( CDatabaseType* ptyp, Str8* psDescription );

// Dialog Data
    //{{AFX_DATA(CDbTypOptionsPage)
    enum { IDD = IDD_DBTYP_OPTIONS_PAGE };
    CStatic m_lblName;
    CStatic m_lblRecordMkr;
    CEdit m_edtDescription;
    CMarkerSetComboBox m_cboDateStamp;
    CMarkerSetComboBox m_cboTextRef;
    CButton m_chkMDF;
    //}}AFX_DATA

    virtual void OnOK();
    virtual BOOL OnSetActive(); // called when our page is switched to

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDbTypOptionsPage)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


// **************************************************************************

class CDatabaseTypeSheet : public CPropertySheet
{
private:
    CDatabaseTypePtr m_ptyp;
    Str8 m_sDescription;

    CMarker* m_pmkr;    
    CMarkerSetListBox m_lboMarkers;
    CMarkerSetPage m_dlgMarkers;

    CFilter* m_pfil;    
    CFilterSetListBox m_lboFilters;
    CSetPage m_dlgFilters;
    
    CJumpPath* m_pjmp;    
    CSetListBox m_lboJumpPaths;
    CJumpPathSetPage m_dlgJumpPaths;

    CDbTypOptionsPage m_dlgOptions;

    CIntProcPage m_dlgIntProc;

    CDbTypNumberingPage m_dlgNumbering;

public:
    CDatabaseTypeSheet(CDatabaseType* ptyp, int iActivePage = 0,
            CMarker* pmkr = NULL, CFilter* pfil = NULL);
        // Construct with explicit arguments to initialize property page
        // items (current field's marker, filter in use) when viewing
        // the active window's type:
        // Database, Properties of this Type
        //
        // Omit them when viewing the type as an independent entity:
        // File, Database Types, Modify

    // constants for page selection
	// NOTE: The numbering page must be last because it will not be shown in all cases (i.e., when the Numbering function is not allowed for a DB type).
    enum { eMarkerSetPage=0, eFilterSetPage, eJumpPathPage, eOptionsPage,
			eIntProcPage, eNumberingPage };

    virtual BOOL OnInitDialog();

protected:
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CDatabaseTypeSheet)
    afx_msg void OnOK();
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CDatabaseTypeSheet)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	void SetButtonLabels();  // 1998-09-30 MRP: For French version
};  // class CDatabaseTypeSheet


/////////////////////////////////////////////////////////////////////////////
// CNewDbTypDlg dialog

class CNewDbTypDlg : public CDialog
{
private:
    CDatabaseTypeSet* m_ptypset;
    CMarkerSet* m_pmkrset;

// Construction
public:
    CNewDbTypDlg( CDatabaseTypeSet* ptypset, Str8* psName, Str8* psRecordMkr,
                    CMarkerSet* pmkrset );

// Dialog Data
    Str8& m_sName;
    Str8& m_sRecordMkr;
    //{{AFX_DATA(CNewDbTypDlg)
    enum { IDD = IDD_NEWDBTYP };
    CEdit   m_edtName;
    CComboBox   m_cboRecordMkr;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CNewDbTypDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CDbTypRecMkrDlg dialog

class CDbTypRecMkrDlg : public CDialog
{
    CDatabaseType* m_ptyp;

// Construction
public:
    CDbTypRecMkrDlg(CDatabaseType* ptyp);

// Dialog Data
    //{{AFX_DATA(CDbTypRecMkrDlg)
    enum { IDD = IDD_TYP_RECMKR };
    CComboBox   m_cboRecordMkr;
    Str8 m_sRecordMkr;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDbTypRecMkrDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    virtual void OnCancel();
    DECLARE_MESSAGE_MAP()
};

class CDatabaseTypeSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CDatabaseTypeSetComboBox(CDatabaseTypeSet* ptypset) : CSetComboBox( ptypset ) {}
    
    void UpdateElements(CDatabaseTypeProxy* ptrxToSelect)
        { CSetComboBox::UpdateElements( ptrxToSelect ); }

    CDatabaseTypeProxy* ptrxSelected()
        { return (CDatabaseTypeProxy*)CSetComboBox::pselSelected(); }
};  // class CDatabaseTypeSetComboBox
                         
#endif // Typ_d_h
