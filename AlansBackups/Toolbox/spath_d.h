// spath_d.h  Interface for the jump path dialog boxes
//
// "But the path of the righteous is like the light of dawn,
// That shines brighter and brighter until the full day."
//                      Proverbs 4:18

#ifndef Spath_d_h
#define Spath_d_h

#include "resource.h"
#include "lbo.h"
#include "jmp.h"
#include "mrfset_d.h"
#include "mkr.h"
#include "set_d.h"
#include "cck_d.h"
#include "recset.h"

class CShwDoc;

class CDatabaseRefListBox : public CDblListListBox
{
public:
    CDatabaseRefListBox(const CDatabaseRefList* pdrflst);

    void InitListBox();
    const CDatabaseRefList* pdrflst() { return m_pdrflst; }
    CDatabaseRef* pdrfSelected() { return (CDatabaseRef*)pelSelected(); }

    int InsertPath(int iDest, Str8 sPath, Str8 sKey);

protected:
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem, const CDblListEl *pel);

private:
    const CDatabaseRefList* m_pdrflst;
    CDatabaseRefList m_drflstCleanup; // used to delete dynamically created drf's
    int m_xDatabase;
    int m_xMarker;
};  // class CDatabaseRefListBox

// --------------------------------------------------------------------------

class CDocListBox : public CListBox
{
public:
    CShwDoc* pdocSelected()
        {
        int iSelected = GetCurSel();
        return (CShwDoc*)( iSelected != LB_ERR ? GetItemDataPtr(iSelected) : NULL );
        }

    int LoadDbsAvailable( char* pszDbSel=NULL ); // load up list with names of all open dbs
    void LoadDbsInsertable( const CDatabaseRefList* pdrflst); // load up list based on a jump path
    void AddDocument(CShwDoc* pdoc, BOOL bCheckForDuplicates=FALSE); // add a document to the list

private:
    virtual void DrawItem(LPDRAWITEMSTRUCT);
};  // class CDocListBox

// --------------------------------------------------------------------------

class CJumpPathSetPage : public CSetPage  // Hungarian: dlg
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active

// Dialog Data
    //{{AFX_DATA(CJumpPathSetPage)
    enum { IDD = IDD_JUMPPATH_PAGE };
    BOOL m_bWarnDocClose;
    //}}AFX_DATA

public:
    CJumpPathSetPage(CSetListBox& lboSet);
        // A dialog box for viewing the set;
        // adding new elements and viewing or modifying existing ones;
        // and returning the selection when the dialog box was closed.

    void OnOK();            // ok or apply now pressed -- KillActive is called first
    
protected:          
    void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CJumpPathSetPage)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
};  // class CJumpPathSetPage

// --------------------------------------------------------------------------

class CJumpPathDlg : public CDialog
{
private:
    CJumpPath* m_pjmp;  // the jump path being viewed or added new
    CJumpPath** m_ppjmpNew;  // means by which to return new jump path
    CJumpPathSet* m_pjmpset;  // jump path set which owns the jump path
    CShwDoc* m_pdocCurSel; // used to inhibit marker combo reload if user double-clicks on already selected item
    CMarkerSubSet m_subsetUsed; // Subset of all markers used by other jump paths
    CMarkerSubSet m_subsetTemp;
    CDatabaseRefList m_drflstTemp;
    BOOL m_bDefault;
    BOOL m_bKeySetExplicitlyByUser;
    CKey m_key;
// Construction
public:
    CJumpPathDlg(CJumpPath* pjmp);
        // A dialog box for viewing and possibly modifying an existing jump path
    CJumpPathDlg(CJumpPathSet* pjmpset, CJumpPath** ppjmpNew);
        // A dialog box for adding a new jump path to the set
    CJumpPathDlg(const CJumpPath* pjmpToCopyFrom, CJumpPath** ppjmpNew);
        // A dialog box for copying an existing jump path to the set
        // by copying from an existing one

// Dialog Data
    //{{AFX_DATA(CJumpPathDlg)
    enum { IDD = IDD_Jump_Path };
    Str8 m_sName;
    CButton m_chkDefault;
    CMarkerSubSetBox m_lboExcluded;
    CButton m_btnAddField;
    CButton m_btnRemoveField;
    CButton m_btnClearFields;
    CMarkerSubSetBox m_lboIncluded;
    CDatabaseRefListBox m_lboPaths;
    CDocListBox m_lboDbsAvailable;
    CMarkerSetComboBox m_cboMarkers;
    CButton m_chkShowPath;
    CButton m_chkRefIntegrity;
    //}}AFX_DATA

// Implementation
protected:
    virtual void UpdateSourceButtons();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CJumpPathDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnClkDefault();
    afx_msg void OnClkDataLink();
    afx_msg void OnAdd();
    afx_msg void OnRemove();
    afx_msg void OnRemoveAll();
    afx_msg void ReloadMkrCombo();
    afx_msg void OnSearchpathFirst();
    afx_msg void OnSearchpathLast();
    afx_msg void OnSearchpathInsert();
    afx_msg void OnSearchpathRemove();
    afx_msg void OnSearchpathClear();
    afx_msg void OnSearchpathShowpath();
    afx_msg void OnJumpPathMatchingCriteria();
    afx_msg void OnFirst();
    afx_msg void OnLast();
    afx_msg void OnInsert(); 
    afx_msg void OnModify();
    afx_msg void OnDelete();
    afx_msg void OnDeleteAll();
    afx_msg void OnDblclkChoice();
    afx_msg void OnDblclkElement();
    afx_msg void OnSelchangeElement();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void AddToSearchPathLB(int idxDest); // copy selected database and mkr to jump path listbox
};  // class CJumpPathDlg

// --------------------------------------------------------------------------
class CJumpPathMatchingCriteriaDlg : public CDialog
{
private:
	CKey* m_pkey;

// Dialog Data
	//{{AFX_DATA(CJumpPathMatchingCriteriaDlg)
	enum { IDD = IDD_JumpPathMatchingCriteria };
	BOOL	m_bMatchWhole;
	int		m_iMatchSetting;
	//}}AFX_DATA
	MatchSetting m_matset;

public:
	CJumpPathMatchingCriteriaDlg(CKey* pkey);

// Implementation
protected:
	void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	BOOL OnInitDialog();
	void OnOK();
}; // class CJumpPathMatchingCriteriaDlg

// --------------------------------------------------------------------------
// CDocCloseWarnDlg dialog

class CDocCloseWarnDlg : public CDialog
{
// Construction
public:
    CDocCloseWarnDlg(CWnd* pParent = NULL); // standard constructor

// Dialog Data
    //{{AFX_DATA(CDocCloseWarnDlg)
    enum { IDD = IDD_WARN_DOC_IN_PATH };
    CButton m_chkWarnDocClose;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDocCloseWarnDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnNo();
    afx_msg void OnYes();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------
// CDBNotOpenDlg dialog

class CDBNotOpenDlg : public CDialog
{
// Construction
public:
    CDBNotOpenDlg(const Str8& sClosedDB, const Str8& sJumpPath, CWnd* pParent = NULL);

// Dialog Data
    //{{AFX_DATA(CDBNotOpenDlg)
    enum { IDD = IDD_DB_NOT_OPEN };
    Str8 m_sJumpPath;
    Str8 m_sClosedDB;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDBNotOpenDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSkip();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
}; // CDBNotOpenDlg

// --------------------------------------------------------------------------
// CDBDiffPathDlg dialog

class CDBDiffPathDlg : public CDialog
{
// Construction
public:
    CDBDiffPathDlg(const Str8& sClosedDB, const Str8& sOpenDB, const Str8& sJumpPath, CWnd* pParent = NULL);

// Dialog Data
    //{{AFX_DATA(CDBDiffPathDlg)
    enum { IDD = IDD_DB_DIFF_PATH };
    Str8 m_sJumpPath;
    Str8 m_sClosedDB;
    Str8 m_sOpenDB;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDBDiffPathDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnFixIt();
    afx_msg void OnSkip();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
}; // CDBDiffPathDlg

// --------------------------------------------------------------------------
// CFieldNotDefinedDlg dialog

class CFieldNotDefinedDlg : public CDialog
{
// Construction
public:
    CFieldNotDefinedDlg(const Str8& sDB, const Str8& sMkr, const Str8& sJumpPath, CWnd* pParent = NULL);

// Dialog Data
    //{{AFX_DATA(CFieldNotDefinedDlg)
    enum { IDD = IDD_FIELD_NOT_DEFINED };
    Str8 m_sJumpPath;
    Str8 m_sMkr;
    Str8 m_sDB;
    //}}AFX_DATA

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CFieldNotDefinedDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSkip();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
}; // CFieldNotDefinedDlg

// --------------------------------------------------------------------------

class CJumpPathSetListBox : public CSetListBox
{
public:
    CJumpPathSetListBox(CJumpPathSet* pjmpset, CJumpPath** ppjmp) :
        CSetListBox(pjmpset, (CSetEl**)ppjmp) {}

protected:
    virtual BOOL bDelete(CSetEl* psel);
};  // class CJumpPathSetListBox

// --------------------------------------------------------------------------

class CReferentialInconsistencyDlg : public CInconsistencyDlg  // Hungarian: dlg
{
private:
    BOOL m_bEnableInsert;
    CJumpPath* m_pjmp;
    CShwDoc** m_ppdoc;

    // Dialog Data
    //{{AFX_DATA(CReferentialInconsistencyDlg)
    enum { IDD = IDD_cck_Referential };
    CButton m_btnInsert;
    //}}AFX_DATA

public:
    CReferentialInconsistencyDlg(CShwView* pview, CRecPos& rpsStart, CRecPos& rpsEnd,
            CCheckingState *pcks, BOOL& bNeedToRecheck, BOOL bEnableInsert,
            CJumpPath* pjmp, CShwDoc** ppdoc);
    ~CReferentialInconsistencyDlg();

protected:
    void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CReferentialInconsistencyDlg)
    afx_msg void OnChangeReplacement();
    afx_msg void OnInsert();
    afx_msg void OnModify();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CReferentialInconsistencyDlg

#endif // Spath_d_h
