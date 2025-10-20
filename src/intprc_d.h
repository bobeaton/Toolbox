// intprc_d.h Interface to interlinear processes

#ifndef INTPRC_D_H
#define INTPRC_D_H

#include "resource.h"
#include "EditLng.h" // 1.4qmb
#include "mkr.h"
#include "spath_d.h" // For CDocListBox

// --------------------------------------------------------------------------
class CIntProcListBox : public CDblListListBox
{
private:
	CInterlinearProcList* m_pintprclst;
	int m_xTo; // Horizontal position of column
	int m_xFrom; // Horizontal position of column
	int m_xProc; // Horizontal position of column

public:
	CIntProcListBox(CInterlinearProcList* pintprclst);

	void InitListBox();
	void Close();
	CInterlinearProcList* pintprclst() { return m_pintprclst; }
	CInterlinearProc* pintprcSelected() { return (CInterlinearProc*)pelSelected(); }

protected:
	virtual void InitLabels();
	virtual void DrawElement(CDC& cDC, const RECT& rcItem, const CDblListEl *pel);
};

// --------------------------------------------------------------------------
class CIntProcPage : public CPropertyPage  // Hungarian: dlg
{
private:
	CInterlinearProcList* m_pintprclst;
    BOOL m_bWasActive; // reflects whether this page was ever active
// Dialog Data
	//{{AFX_DATA(CIntProcPage)
	enum { IDD = IDD_INTPRC_PAGE };
	CButton	m_btnExpressSetup;
	CButton	m_btnModify;
	CButton	m_btnDelete;
	CButton	m_btnAdd;
	CIntProcListBox m_lboIntProcs;
	int		m_iInterlinearSpacing;
	//}}AFX_DATA

public:
	CIntProcPage( CInterlinearProcList* pintprclst );
		
    virtual void OnOK();

	BOOL bListEmpty()	{ return ( m_lboIntProcs.GetCount() <= 0 ); }

private:
	void DeleteAllUnselected(); // Delete all not selected

protected:			
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CIntProcPage)
	afx_msg void OnDblclkIntProcList();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnModify();
	afx_msg void OnExpressSetup();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void UpdateButtons();
		// Whenever the number of items in the list box changes	
};

	
/////////////////////////////////////////////////////////////////////////////
// CSelectProcDlg dialog

class CSelectProcDlg : public CDialog
{
private:
	int* m_piProcID; // Place to return selected process id
// Construction
public:
	CSelectProcDlg( int* piProcID );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectProcDlg)
	enum { IDD = IDD_SELECT_PROCESS };
	CListBox	m_lboSelectProc;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSelectProcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkSelectProcList();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog

class CParseDlg : public CDialog
{
private:
	CLookupProc* m_pintprc;
    CWordFormulaSet* m_pwdfset;  // 1999-09-06 MRP
// Construction
public:
	CParseDlg( CLookupProc* pintprc ); // standard constructor
    ~CParseDlg();

// Dialog Data
	//{{AFX_DATA(CParseDlg)
	enum { IDD = IDD_PARSE };
	CButton	m_chkCompRoot;
	CButton	m_chkAdapt;
	CButton	m_chkInfixBefore;
	CButton	m_chkInfixMultiple;
	CButton	m_chkLongerOverride;
	CButton	m_chkOverrideShowAll;
	CButton	m_chkInfixFront;
	CButton	m_chkInsertFail;
	CButton	m_chkShowWord;
	CButton	m_chkShowRootGuess;
	CButton	m_chkShowFailMark;
	CButton	m_chkKeepPunc;
	CButton	m_chkKeepCap;
	CMarkerSetComboBox	m_cboToMarker;
	CMarkerSetComboBox	m_cboFromMarker;
	CButton	m_chkSH2Parse;
	CButton	m_btnConjAff;
	CButton	m_btnParseDB;
	CEditLng m_elcMorphBreaks; // 1.4qpn
	CEditLng m_elcMorphBound; // 1.4qpn
	CEditLng m_elcForcedGlossStart; // 1.4qpn
	CEditLng m_elcForcedGlossEnd; // 1.4qpn
	Str8	m_sForcedGlossEnd;
	Str8	m_sForcedGlossStart;
	Str8	m_sMorphBound;
	Str8	m_sMorphBreaks;
	int m_iAffixSetting;
    BOOL m_bEnableWordFormulas;
    CButton m_chkEnableWordFormulas;
	CButton m_btnFormulas;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CParseDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelchangeToMarker();
	afx_msg void OnParse();
	afx_msg void OnPrefix();
	afx_msg void OnRoot();
	afx_msg void OnSh2Parse();
	afx_msg void OnEnableWordFormulas();
	afx_msg void OnFormulas();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL bGetRuleFile( BOOL bOnlyIfNecessary = NULL );
	BOOL bRuleFileExists();
};

/////////////////////////////////////////////////////////////////////////////
// CLookupDlg dialog

class CLookupDlg : public CDialog
{
private:
	CLookupProc* m_pintprc;
	Str8 m_sCCTable; // Path of CC Table
// Construction
public:
	CLookupDlg( CLookupProc* pintprc );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLookupDlg)
	enum { IDD = IDD_LOOKUP };
	CButton	m_chkAdapt;
	CButton	m_chkInsertFail;
	CEditLng m_elcGlossSeparator; // 1.4qpn
	CButton	m_chkStopAtSemicolon;
	CButton	m_chkKeepPunc;
	CButton	m_chkKeepCap;
	CButton	m_chkApplyCC;
	CButton	m_chkShowWord;
	CButton	m_chkShowFailMark;
	CStatic	m_lblCCTable;
	CMarkerSetComboBox	m_cboToMarker;
	CMarkerSetComboBox	m_cboFromMarker;
	Str8	m_sGlossSeparator;
	//}}AFX_DATA
// 1.4qmb Change most CLngEditCtrl to CEdtLng

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CLookupDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnLookup();
	afx_msg void OnBrowseCctable();
	afx_msg void OnShowFail();
	afx_msg void OnApplycc();
	afx_msg void OnShowWord();
	afx_msg void OnShowFailMark();
	afx_msg void OnSelchangeToMarker();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
private:
	void SetCCTWindowTextNone(); // Set the CC table text to None
	BOOL bGetCCTable( BOOL bOnlyIfNecessary = NULL );
	BOOL bCCTableExists();
};

/////////////////////////////////////////////////////////////////////////////
// CTrieDlg dialog

class CTrieDlg : public CDialog
{
private:
	CDbTrie* m_ptri;
	CMarkerSet m_mkrsetEmpty;
	CMarker* m_pmkrEmpty;
// Construction
public:
	CTrieDlg( CDbTrie* ptri ); // standard constructor
// Dialog Data
	//{{AFX_DATA(CTrieDlg)
	enum { IDD = IDD_TRIE };
	CButton	m_chkShowPath;
	CMarkerSetComboBox	m_cboMkrOutput;
	CListBox	m_lboMkrsToFind;
	CMarkerSetListBox	m_lboMkrsAvailable;
	CDocListBox	m_lboDbsAvailable;
	CDatabaseRefListBox	m_lboDbsToSearch;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CTrieDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTrieLstdbsavailable();
	afx_msg void OnDblclkTrieLstmkrsavailable();
	afx_msg void OnTrieMoveto();
	afx_msg void OnTrieRemove();
	afx_msg void OnTrieMkrmoveto();
	afx_msg void OnTrieMkrremove();
	afx_msg void OnTrieShowpath();
	virtual void OnOK();
	afx_msg void OnDblclkTrieLstmkrstofind();
	afx_msg void OnDblclkTrieLstdbstosearch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CExpressTextDlg dialog

class CExpressTextDlg : public CDialog
{
// Construction
public:
	CExpressTextDlg();

// Dialog Data
	//{{AFX_DATA(CExpressTextDlg)
	enum { IDD = IDD_EXPRESS_TEXT };
	Str8	m_sGloss;
	Str8	m_sMB;
	Str8	m_sPS;
	Str8	m_sText;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CExpressTextDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CExpressLexiconDlg dialog

class CExpressLexiconDlg : public CDialog
{
	CDatabaseRefList* m_pdrflst;
// Construction
public:
	CExpressLexiconDlg( CDatabaseRefList* pdrflst );	// standard constructor // 1.4yg 

// Dialog Data
	//{{AFX_DATA(CExpressLexiconDlg)
	enum { IDD = IDD_EXPRESS_LEXICON };
	CButton	m_chkShowPath;
	CDatabaseRefListBox	m_lboDbsToSearch;
	CDocListBox	m_lboDbsAvailable;
	Str8	m_sGloss;
	Str8	m_sPS;
	Str8	m_sAlt;
	Str8	m_sUnd;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CExpressLexiconDlg)
	virtual void OnOK();
	afx_msg void OnMoveto();
	afx_msg void OnRemove();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLstdbsavailable();
	afx_msg void OnDblclkLstdbstosearch();
	afx_msg void OnShowpath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CRearrangeDlg dialog

class CRearrangeDlg : public CDialog
{
private:
	CRearrangeProc* m_pintprc;
	Str8 m_sRuleFile; // Path of Rule File
// Construction
public:
	CRearrangeDlg( CRearrangeProc* pintprc );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRearrangeDlg)
	enum { IDD = IDD_REARRANGE };
	CMarkerSetComboBox	m_cboToMarker;
	CMarkerSetComboBox	m_cboFromMarker;
	CMarkerSetComboBox	m_cboPSMarker;
	CMarkerSetComboBox	m_cboPuncMarker;
	CStatic	m_lblRuleFile;
	CButton	m_chkAdapt;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CRearrangeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseRulefile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetRuleFileWindowTextNone(); // Set the rule file text to None
	BOOL bGetRuleFile( BOOL bOnlyIfNecessary = NULL );
	BOOL bRuleFileExists();
};
/////////////////////////////////////////////////////////////////////////////
// CGivenDlg dialog

class CGivenDlg : public CDialog
{
private:
	CGivenProc* m_pintprc;
// Construction
public:
	CGivenDlg( CGivenProc* m_pintprc );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGivenDlg)
	enum { IDD = IDD_GIVENPROC };
	CMarkerSetComboBox	m_cboFromMarker;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CGivenDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CGenerateDlg dialog

class CGenerateDlg : public CDialog
{
private:
	CRearrangeProc* m_pintprc;
	Str8 m_sRuleFile; // Path of Rule File
// Construction
public:
	CGenerateDlg( CRearrangeProc* pintprc );	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGenerateDlg)
	enum { IDD = IDD_GENERATE };
	CMarkerSetComboBox	m_cboToMarker;
	CMarkerSetComboBox	m_cboFromMarker;
	CMarkerSetComboBox	m_cboPuncMarker;
	CStatic	m_lblRuleFile;
	CButton	m_chkKeepMBrks;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CGenerateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseRulefile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void SetRuleFileWindowTextNone(); // Set the rule file text to None
	BOOL bGetRuleFile( BOOL bOnlyIfNecessary = NULL );
	BOOL bRuleFileExists();
};
#endif // INTPRC_D_H
