// wdf_d.h  Word formulas for selecting valid parses automatically

#ifndef WDF_D_H
#define WDF_D_H

#include "resource.h"
#include "wdf.h"
#include "lbo.h"  // CSetListBox
#include "EditLng.h" // 1.4qmb
#include "lng.h"  // CLangEncSetComboBox
#include "set_d.h"  // CSetDlg

// --------------------------------------------------------------------------

class CWordFormulaSetListBox : public CSetListBox
{
public:
    CWordFormulaSetListBox(CWordFormulaSet* pwdfset, CWordFormula** ppwdf);

    CWordFormula* pwdfSelected();
    UINT uItemHeight(CWordFormula* pwdf);
    void LangEncPatterns(CLangEnc* plng);
    void ShowExpandedList(BOOL b) { m_bShowExpandedList = b; }

protected:
    int m_xSymbol;
    int m_xPatterns;
    int m_xEtc;

    CWordFormulaSet* m_pwdfset;
    CLangEnc* m_plngPatterns;
    UINT m_uLineHeight;
    Str8 m_sPrimaryMark;
    Str8 m_sEtc;
    BOOL m_bShowExpandedList;
    
    virtual void InitLabels();
    virtual int iItemHeight();
    virtual void MeasureItem(LPMEASUREITEMSTRUCT);
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
    virtual BOOL bDelete(CSetEl* psel);
    virtual BOOL bIsBold(CSetEl* psel);

private:
    int maxnItemsToShow() const;
};  // class CWordFormulaSetListBox

// --------------------------------------------------------------------------

class CWordFormulaDlg : public CDialog
{
private:    
    CWordFormula* m_pwdf;
    CWordFormula** m_ppwdfNew;
    CWordFormulaSet* m_pwdfset;

// Dialog Data
    //{{AFX_DATA(CWordFormulaDlg)
    enum { IDD = IDD_WordFormula };
    Str8 m_sSymbol;
    Str8 m_sPatterns;
    //}}AFX_DATA

//    CEditLng m_elcPatterns; // 1.4qpn // 1.4qmb Change most CLngEditCtrl to CEditLng // 1.4vxf Change word formula edit control to standard ctrl
    CEdit m_elcPatterns; // 1.4qpn // 1.4qmb Change most CLngEditCtrl to CEditLng // 1.4vxf Change word formula edit control to standard ctrl

public:
    CWordFormulaDlg(CWordFormulaSet* pwdfset, CWordFormula** ppwdfNew);
        // A dialog box for adding a new formula to the set
    CWordFormulaDlg(const CWordFormula* pwdfToCopyFrom, CWordFormula** ppwdfNew);
        // A dialog box for adding a formula to the set
        // by copying from an existing one
    CWordFormulaDlg(CWordFormula* pwdf);
        // A dialog box for viewing and possibly modifying an existing formula

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

    // Generated message map functions
    //{{AFX_MSG(CWordFormulaDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CWordFormulaDlg

// --------------------------------------------------------------------------

class CWordFormulaSetDlg : public CSetDlg
{
private:
    CWordFormulaSet* m_pwdfset;
	
// Dialog Data
	//{{AFX_DATA(CWordFormulaSetDlg)
	enum { IDD = IDD_WordFormulas };
    CWordFormulaSetListBox& m_lboFormulas;
    BOOL m_bShowExpandedList;
	//}}AFX_DATA

    CLangEncSetComboBox m_cboLangEncPatterns;

    CWordFormulaSetDlg(CWordFormulaSet* pwdfset,
            CWordFormulaSetListBox& lboSet, const Str8& sLabel);
	
public:
    static BOOL s_bDoModal(CWordFormulaSet* pwdfset);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnInitDialog();
    virtual void OnOK();

	// Generated message map functions
	//{{AFX_MSG(CWordFormulaSetDlg)
    afx_msg void OnPrimary();
    afx_msg void OnClear();
    afx_msg void OnSelchangeLangEncPatterns();
    afx_msg void OnShowExpandedList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};  // CWordFormulaSetDlg

#endif  // WDF_D_H
