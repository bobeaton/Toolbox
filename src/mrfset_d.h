// mrfset_d.h  Marker reference set list box and Select Fields dialog box

#ifndef MRFSET_D_H
#define MRFSET_D_H

#include "lbo.h"  // CListBox
#include "mkr.h"  // CMarkerSubSet
#include "resource.h"  // IDD_SelectFields

// --------------------------------------------------------------------------

class CMarkerSubSetBox : public CListBox  // Hungarian: lbo
{
private:
    CMarkerSubSet& m_subset;
    BOOL m_bIncludedSide;
    Str8 m_sMatched;  // User's keystrokes for selecting an item
	const CLangEnc* m_plng;
	const CMarkerSubSet* m_psubsetInvalid;

public:
    CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide);
    CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide,
                     const CMarkerSubSet* psubsetInvalid);
    CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide,
					 const CLangEnc* plng, const CMarkerSubSet* psubsetInvalid);

    void Init();
    void AddSelectedItemsTo(CMarkerSubSetBox& lboTo);
    void UpdateMarkerSubSet() const;
    
    BOOL bEmpty() const { return GetCount() == 0; }
    BOOL bNonEmpty() const { return GetCount() > 0; }

protected:
    afx_msg void OnChar(UINT, UINT, UINT);
    afx_msg void OnKillFocus(CWnd*);
    DECLARE_MESSAGE_MAP()
    
    virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
        LRESULT* pLResult);
	virtual BOOL bInclude(const CMarker* pmkr) const;

private:
    CMarker* pmkrItem(ItemIndex itm) const;
};  // class CMarkerSubSetBox


// --------------------------------------------------------------------------

class CSelectFieldsDlg : public CDialog  // Hungarian: dlg
{
private:
    CMarkerSubSet& m_subset;
//	UINT m_idsInstructions; // 1.4gb
    BOOL m_bShowAutoInclude;

// Dialog Data
    //{{AFX_DATA(CSelectFieldsDlg)
    enum { IDD = IDD_SelectFields };
    CStatic m_lblInstructions;
    CMarkerSubSetBox m_lboExcluded;
    CButton m_btnAdd;
    CButton m_btnRemove;
    CButton m_btnRemoveAll;  // 1999-02-04 MRP
    CMarkerSubSetBox m_lboIncluded;
    CButton m_chkAutoInclude;
    BOOL m_bAutoInclude;
    //}}AFX_DATA

public:
    CSelectFieldsDlg(CMarkerSubSet& subset, BOOL bEnableAutoInclude ); // , UINT idsInstructions = 0); // 1.4gb
    CSelectFieldsDlg(CMarkerSubSet& subset, BOOL bEnableAutoInclude,
					 CLangEncPtr plng, CMarkerSubSet* psubsetInvalid ); // , UINT idsInstructions = 0); // 1.4gb

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void UpdateButtons();
    
    // Generated message map functions
    //{{AFX_MSG(CSelectFieldsDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnAdd();
    afx_msg void OnRemove();
    afx_msg void OnRemoveAll();  // 1999-02-04 MRP
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CSelectFieldsDlg

#endif  // not MRFSET_D_H
