// multmat.h  Multiple search match list definition

#ifndef MULTMAT_H
#define MULTMAT_H

#include "ind.h"
#include "dbref.h"
#include "resource.h"

class CMultipleMatch : public CDblListEl  // Hungarian: mmt
{
private:
    CNumberedRecElPtr m_prel; // pointer to matching rel
    CDatabaseRef* m_pdbref; // index/database where it was found
     
    friend class CMultipleMatchList; 
     
public:
    CMultipleMatch( CRecLookEl* prel, long lRecNum, CDatabaseRef* pdbref )
                : m_prel( prel, lRecNum )
        { ASSERT( prel ); ASSERT( pdbref );  m_pdbref = pdbref; }

    CDatabaseRef* pdbref() { return m_pdbref; }
    CNumberedRecElPtr prel() { return m_prel; }
};
    

// --------------------------------------------------------------------------

class CMultipleMatchList : public CDblList  // Hungarian: mmtlst
{
public:
    ~CMultipleMatchList() { DeleteAll(); }

    CMultipleMatch* pmmtFirst() const { return (CMultipleMatch*)pelFirst(); }
    CMultipleMatch* pmmtNext( const CMultipleMatch* pmmt) const { return (CMultipleMatch*)pelNext(pmmt); }
    CMultipleMatch* pmmtPrev( const CMultipleMatch* pmmt) const { return (CMultipleMatch*)pelPrev(pmmt); }
    CMultipleMatch* pmmtLast() const  { return (CMultipleMatch*)pelLast(); }    
    
    void Add(CRecLookEl* prel, long lRecNum, CDatabaseRef* pdbref) // Add another match to list
        { Add( new CMultipleMatch(prel, lRecNum, pdbref) ); }
    void Add(const CMultipleMatch* pmmt) // Add another match to list
        { CDblList::Add((CDblListEl*)pmmt); }

    int iModalView_Elements(CMultipleMatch** ppmmt, BOOL bSingleIndex,
            BOOL bEnableInsert);
};


// --------------------------------------------------------------------------


class CMultipleMatchListBox : public CDblListListBox
{
public:
    CMultipleMatchListBox(CMultipleMatchList* pmmtlst, CMultipleMatch** ppmmt,
                BOOL bSingleIndex);

    void InitListBox();
    virtual int iItemHeight();
    void Close();
    CMultipleMatchList* pmmtlst() { return m_pmmtlst; }
    CMultipleMatch* pmmtSelected() { return (CMultipleMatch*)pelSelected(); }
    CIndex* pindSelected() { return pmmtSelected()->pdbref()->pind(); }
    BOOL bSingleIndex() { return m_bSingleIndex; }

protected:
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem, const CDblListEl *pel);

private:
    CMultipleMatchList* m_pmmtlst;
    CMultipleMatch** m_ppmmt;
    BOOL m_bSingleIndex; // TRUE for Search, Record Insert - don't display database name
    int m_xWord;
    int m_xAdditionalField;
    int m_xDatabase;
};


// --------------------------------------------------------------------------


class CMultipleMatchDlg : public CDialog  // Hungarian: dlg
{
private:
    CMultipleMatchListBox& m_lboMatches;
    BOOL m_bEnableInsert;
    
// Dialog Data
    //{{AFX_DATA(CMultipleMatchDlg)
    enum { IDD = IDD_MULTMATCH };
    CButton m_btnInsert;
    CStatic m_lblAdditionalField;
    CButton m_chkShowPath;
    CMarkerSetComboBox m_cboAdditionalField;
    //}}AFX_DATA

public:
    CMultipleMatchDlg(CMultipleMatchListBox& lboMatches, BOOL bEnableInsert);
        
    BOOL bListEmpty()   { return ( m_lboMatches.GetCount() <= 0 ); }

protected:          
    void ReloadMkrCombo( BOOL bForce ); // reload additional field combo with proper marker set

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CMultipleMatchDlg)
    virtual void OnOK();
    afx_msg void OnDblclkMultmatchList();
    afx_msg void OnSelchangeMultmatchMkrs();
    afx_msg void OnSelchangeMultmatchList();
    afx_msg void OnMultmatchShowpath();
    afx_msg void OnTryagain();
    afx_msg void OnInsert();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


    
#endif // MULTMAT_H