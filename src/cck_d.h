// cck_d.h  Dialog boxes for Consistency Checks

#ifndef CCK_D_H
#define CCK_D_H

#include "resource.h"  // IDD_cck
#include "cck.h"
#include "EditLng.h" 
#include "crecpos.h"

// --------------------------------------------------------------------------

class CCheckConsistencyDlg : public CDialog  // Hungarian: dlg
{
private:
    CFieldConsistencyChecker* m_pcck;
    CMarkerSet* m_pmkrset;

    CMarker* m_pmkrSingleField;
    CMarkerSubSet m_subsetFieldsToCheck;
    CMarkerSetComboBox m_cboSingleField;  // Derived class

    // Dialog Data
    //{{AFX_DATA(CCheckConsistencyDlg)
    enum { IDD = IDD_cck };
    Str8 m_sDatabase;
    Str8 m_sFilter;
    Str8 m_sDatabaseType;
    int m_iFieldsToCheck;
    CButton m_btnSelectFields;
#ifdef RECORD_ORDER_CHECKING_ENABLED
    BOOL m_bRecordByRecord;
#endif
    BOOL m_bCheckDataDefn;
    BOOL m_bCheckRangeSets;
    BOOL m_bCheckRefConsistency;
    //}}AFX_DATA

public:
    CCheckConsistencyDlg(CFieldConsistencyChecker* pcck, CIndex* pind);
    ~CCheckConsistencyDlg();

protected:
    void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    BOOL OnInitDialog();
    void OnOK();

    // Generated message map functions
    //{{AFX_MSG(CCheckConsistencyDlg)
    afx_msg void OnSelectFields();
    afx_msg void OnChangeFieldsToCheckOption();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CCheckConsistencyDlg

// --------------------------------------------------------------------------
// This is an abstract base class. For an example of how to derive from it, see crngst_d
class CInconsistencyDlg : public CDialog  // Hungarian: dlg
{
private:
    CCheckingState* m_pcks;
    CShwView* m_pview;
    BOOL m_bReadOnly;
    BOOL m_bCheckingDatabase;
    CRecPos& m_rpsStart;
    CRecPos& m_rpsEnd;
    Str8 m_sFailedData;
    BOOL m_bAlwaysConsiderReplacementAsModified;
    BOOL m_bReplacementModified;
protected:
    BOOL& m_bNeedToRecheck;
private:
    // Controls that use the derived classes for font and/or keyboard.
    CEditLng m_elcFailedData; // 1.4qpn
    CEditLng m_elcReplacement; // 1.4qpn

    // Dialog Data
    //{{AFX_DATA(CInconsistencyDlg)
    Str8 m_sReplacement;
    Str8 m_sMarkerLabel;
    Str8 m_sInconsistency;
    CButton m_btnSkipAll;
    CButton m_btnReplace;
    CButton m_btnReplaceAll;
    //}}AFX_DATA

    static BOOL s_bWindowPlacement;
    static RECT s_rcWindowPlacement;
    void RememberWindowPlacement();

public:
    CInconsistencyDlg(UINT nIDTemplate, CShwView* pview, CRecPos& rpsStart, CRecPos& rpsEnd,
            CCheckingState *pcks, BOOL& bNeedToRecheck, 
			Str8 sMessage,  // 1.4heq Add message arg to inconsistency dlg func
			BOOL bExplicitNewlines,  // 1.4heq Add bool arg to inconsistency dlg func
			Str8 s = "");
    ~CInconsistencyDlg() = 0;

protected:
    Str8 sReplacement() const { return m_sReplacement; }
    BOOL bReplacementModified() const { return m_bReplacementModified; }
    BOOL bReadOnly() const { return m_bReadOnly; }
    BOOL bCheckingDatabase() const { return m_bCheckingDatabase; }
    Str8 sFailedData() const { return m_sFailedData; }

    void SetReplacementText(const Str8 &sSelection)
        { m_elcReplacement.SetWindowText( swUTF16( sSelection) ); // 1.5.4g Trigger OnChangeReplacement message
		m_elcReplacement.SetEditLngText( sSelection );  // 1.5.4g Change text in control if legacy font
		m_sReplacement = sSelection; // 1.5.4g Change text to be used for replace
		}
    void EditReplacementSetFocus() { m_elcReplacement.SetFocus(); }
    void EditReplacementSetSel(int iStart, int iEnd) { m_elcReplacement.SetSel(iStart, iEnd); }

    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();
    void OnOK();
    void OnCancel();

    // Generated message map functions
    //{{AFX_MSG(CInconsistencyDlg)
    virtual afx_msg void OnChangeReplacement();
    afx_msg void OnSkip();
    afx_msg void OnSkipAll();
    afx_msg void OnReplace();
    afx_msg void OnReplaceAll();
    virtual afx_msg void OnModify();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CInconsistencyDlg

// --------------------------------------------------------------------------

class CDataDefnInconsistencyDlg : public CInconsistencyDlg  // Hungarian: dlg
{
private:
    // Dialog Data
    //{{AFX_DATA(CDataDefnInconsistencyDlg)
    enum { IDD = IDD_cck_DataDefn };
    //}}AFX_DATA

public:
    CDataDefnInconsistencyDlg(CShwView* pview, CRecPos& rpsStart, CRecPos& rpsEnd,
            CCheckingState *pcks, BOOL& bNeedToRecheck, Str8 sMessage, BOOL bExplicitNewlines);
    ~CDataDefnInconsistencyDlg();
};  // class CDataDefnInconsistencyDlg

// --------------------------------------------------------------------------
class CMsgCircularReplacmentDlg : public CDialog  // Hungarian: dlg
{
private:
    Str8 m_sFrom;
    Str8 m_sTo;
    CMarker* m_pmkr;

    // Controls that use the derived classes for font and/or keyboard.
    CEditLng m_elcFrom1; // 1.4qpn
    CEditLng m_elcTo1; // 1.4qpn
    CEditLng m_elcFrom2; // 1.4qpn
    CEditLng m_elcTo2; // 1.4qpn

public:
    CMsgCircularReplacmentDlg(Str8 sFrom, Str8 sTo, CMarker* pmkr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();

};  // class CMsgCircularReplacmentDlg

#endif  // ifndef CCK_D_H
