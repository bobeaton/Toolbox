// File : CRngSt_d.h
// This file contains all dialog classes which are related to the RangeSet

#ifndef RNGSET_D_H
#define RNGSET_D_H

//include files
#include "resource.h"  // IDD_MKR_RangeSet_Page, IDD_RNGSET_INSERT
#include "crngset.h"
#include "mkr.h"  // CMarkerPtr
#include "cck_d.h"
#include "lbo.h"

class CMarkerPropertiesSheet;

// --------------------------------------------------------------------------
class CDataItemSetListBox : public CSetListBox  // Hungarian: lbo
{
protected:
    CLangEnc* m_plng;
    CFont* m_pfnt;

public:
    CDataItemSetListBox(CDataItemSet* pdatset);

    CDataItemEl* pdatSelected()
        { return (CDataItemEl*)pselSelected(); }

    void SetLangEnc(CLangEnc* plng);
    void SetFont(CFont* pfnt);

protected:
    virtual int iItemHeight();
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
};  // class CDataItemSetListBox

// --------------------------------------------------------------------------

class CRangeSetListBox : public CSetListBox  // Hungarian: lbo
{
protected:
    CMarkerPtr m_pmkr;

public:
    CRangeSetListBox(CRangeSet* prngset, CRangeEl** pprng);
    CRangeSetListBox(CRangeSet* prngset);

    CRangeEl* prngSelected()
        { return (CRangeEl*)pselSelected(); }

protected:
    virtual int iItemHeight();
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
};  // class CRangeSetListBox

//************************************************************************/

class CMarkerRangeSetPage : public CPropertyPage
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active

	CMarkerPropertiesSheet* m_pwndMyOwner;
    CDataItemSet&     m_datset;
    const CMarker*    m_pmkr;                    //09-17-1997
        // 1998-02-10 MRP: Note that m_pmkr will be NULL
        // if it's a newly added marker! The Range Set label and
        // Build range set button now take this into account.
        // Be careful when adding new functions to this dialog box!
    BOOL                m_bEnableUseRangeSet;
    const BOOL&         m_bMultipleItemData;
    const BOOL&         m_bMultipleWordItems;
    CEditLng       m_elcRangeSetEl; // 1.4qpn
	CDataItemSetListBox m_lboRangeSet;

// Dialog Data
    //{{AFX_DATA(CMarkerRangeSetPage)
    enum { IDD = IDD_MKR_RangeSet_Page };
    CButton m_chkUseRangeSet;
    BOOL&   m_bUseRangeSet;
    CButton m_btnAdd;
    CButton m_btnReplace;
    CButton m_btnDelete;
    CButton m_btnClear;
    CButton m_btnBuildRangeSet;       // 1998-02-10 MRP
    CStatic m_lblCharRangeSet;        // 1999-10-05 TLB
    CButton m_btnMoreInfo;            // 1999-10-05 TLB
    CButton m_btnConvertCharRangeSet; // 1999-10-05 TLB
    //}}AFX_DATA

	BOOL m_bValidToBuild;
	BOOL m_bValidToConvert;

public:
    CMarkerRangeSetPage(BOOL& bUseRangeSet, CDataItemSet& datset,
        const BOOL& bMultipleItemData, const BOOL& bMultipleWordItems,
        const CMarker* pmkr= NULL, BOOL bDisableRangeSetRemove = FALSE);
    //If the RangeSet should contain only characters, this
    //function checks it. The first wrong occurence will be marked and
    //a message box appears.
    void SetOwner(CMarkerPropertiesSheet* pwndMyOwner);

private:
    void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    void SetLanguageControlFonts();
	void EnableControls();
    void OnApplyEdit(BOOL bReplaceExisting);
    BOOL OnInitDialog();
    BOOL OnSetActive();

	// Generated message map functions
    //{{AFX_MSG(CMarkerRangeSetPage)
	afx_msg void OnUseRangeSet();
    afx_msg void UpdateButtons();
	afx_msg void OnSelchangeRangeSet();
	afx_msg void OnAdd();
	afx_msg void OnReplace();
	afx_msg void OnDelete();
	afx_msg void OnClear();
    afx_msg void OnBuildRangeSet();   //09-17-1997
    afx_msg void OnMoreInfo();
    afx_msg void OnConvertCharRangeSet();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//************************************************************************
//************************************************************************
// Dialog 'Insert from Range Set' (CRangeSetDlg)
//************************************************************************
// header file  implemented by Brian Yoder  14-Jul-95

class CRangeSetDlg : public CDialog
{
private:
    CRangeSet* m_prngset;
    Str8& m_s;
    CRangeSetListBox m_lboRangeSet;

// Dialog Data
    //{{AFX_DATA(CRangeSetDlg)
    enum { IDD = IDD_RNGSET_INSERT };
    Str8 m_sTitle;   //09-16-1997
    //}}AFX_DATA

public:
// Construction
    CRangeSetDlg(CRangeSet* prngset, Str8& s); // standard constructor

// Implementation
private:
    void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CRangeSetDlg)
    virtual void OnOK();
    afx_msg void OnDblclkRngsetList();
    virtual void OnModify();    //09-17-1997
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------

class CRangeSetInconsistencyDlg : public CInconsistencyDlg  // Hungarian: dlg
{
protected:
    CRangeSet* m_prngset;
    CRangeEl* m_prng;
    CRangeSetListBox m_lboRangeSet;

    // Dialog Data
    //{{AFX_DATA(CRangeSetInconsistencyDlg)
    enum { IDD = IDD_cck_RangeSet };
    CButton m_btnAdd;
    //}}AFX_DATA

public:
    CRangeSetInconsistencyDlg(CShwView* pview, CRecPos& rpsStart, CRecPos& rpsEnd,
            CCheckingState *pcks, BOOL& bNeedToRecheck, 
			Str8 sMessage,  // 1.4heq Add message arg to inconsistency dlg func
			BOOL bExplicitNewlines,  // 1.4heq Add bool arg to inconsistency dlg func
            Str8 s = "");
    ~CRangeSetInconsistencyDlg();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CRangeSetInconsistencyDlg)
    afx_msg void OnChangeReplacement();
    virtual afx_msg void OnSelchangeRangeSet();
    virtual afx_msg void OnDblclkRangeSet();
    virtual afx_msg void OnAddToRangeSet();
    afx_msg void OnModify();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CRangeSetInconsistencyDlg

// --------------------------------------------------------------------------

class CCharRangeSetInconsistencyDlg : public CRangeSetInconsistencyDlg  // Hungarian: dlg
{
private:
    int m_iInconsistentChar;

public:
    CCharRangeSetInconsistencyDlg(CShwView* pview, CRecPos& rpsStartItem, CRecPos& rpsEndItem,
            CCheckingState *pcks, BOOL& bNeedToRecheck, 
			int iInconsistentChar);
    ~CCharRangeSetInconsistencyDlg();

private:
    BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CCharRangeSetInconsistencyDlg)
    afx_msg void OnSelchangeRangeSet();
    afx_msg void OnDblclkRangeSet();
    afx_msg void OnAddToRangeSet();
    //}}AFX_MSG

    Str8 sInconsistentChar() { return Str8(sFailedData().GetChar(m_iInconsistentChar)); }

};  // class CCharRangeSetInconsistencyDlg

#endif  // RNGSET_D_H
