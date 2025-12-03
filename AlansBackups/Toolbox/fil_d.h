// fil_d.h  Interface for the filter dialog box

#include "resource.h"
#include "fil.h"
#include "EditLng.h"

// --------------------------------------------------------------------------

class CMarkerTextDlg : public CDialog
{
private:
    CFilterConMarkerString* m_pcon;
    CFilterEl** m_ppfelNew;
    CFilterElList* m_pfellst;
    CFilterSet* m_pfilset;
    
    Str8 m_sMarker;
    CMarkerSet* m_pmkrset;
    CMarker* m_pmkr;
    
    Str8 m_sText;

    CMarkerSetComboBox m_cboMarker;
    CEditLng m_elcText; // 1.4qpn
    CVarSet* m_pvarset;
    CVarSetComboBox m_cboVariable;
    
// Dialog Data
    //{{AFX_DATA(CMarkerTextDlg)
    enum { IDD = IDD_fil_MarkerText };
    CButton m_btnInsertVariable;
    CEdit   m_edtNote;
    //}}AFX_DATA

public:
    CMarkerTextDlg( CFilterElList* pfellst, CFilterEl** ppfelNew );
        // First, Last, and Insert buttons
    CMarkerTextDlg( CFilterConMarkerString* pcon );
        // Modify... button
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CMarkerTextDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeMarker();
    afx_msg void OnInsertVariable();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMarkerTextDlg


// --------------------------------------------------------------------------

class CMarkerDateDlg : public CDialog
{
private:
    CFilterConDate* m_pcon;
    CFilterEl** m_ppfelNew;
    CFilterElList* m_pfellst;
    CFilterSet* m_pfilset;
    
    Str8 m_sMarker;
    CMarkerSet* m_pmkrset;
    CMarker* m_pmkr;

    Str8 m_sDate;

    CMarkerSetComboBox m_cboMarker;
    CEditLng m_elcDate; // 1.4qpn
    
// Dialog Data
    //{{AFX_DATA(CMarkerDateDlg)
    enum { IDD = IDD_fil_MarkerDate };
    int     m_iDateRelation;
    CEdit   m_edtNote;
    //}}AFX_DATA

public:
    CMarkerDateDlg( CFilterElList* pfellst, CFilterEl** ppfelNew );
        // First, Last, and Insert buttons
    CMarkerDateDlg( CFilterConDate* pcon );
        // Modify... button
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CMarkerDateDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeMarker();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMarkerDateDlg


// --------------------------------------------------------------------------

class CLangEncTextDlg : public CDialog
{
private:
    CFilterConLangEncString* m_pcon;
    CFilterEl** m_ppfelNew;
    CFilterElList* m_pfellst;
    CFilterSet* m_pfilset;
    
    CLangEncSet* m_plngset;
    CLangEnc* m_plng;
    
    Str8 m_sText;

    CLangEncSetComboBox m_cboLangEnc;
    CEditLng m_elcText; // 1.4qpn
    CVarSet* m_pvarset;
    CVarSetComboBox m_cboVariable;
    
// Dialog Data
    //{{AFX_DATA(CLangEncTextDlg)
    enum { IDD = IDD_fil_LangEncText };
    CButton m_btnInsertVariable;
    CEdit   m_edtNote;
    //}}AFX_DATA

public:
    CLangEncTextDlg( CFilterElList* pfellst, CFilterEl** ppfelNew );
        // First, Last, and Insert buttons
    CLangEncTextDlg( CFilterConLangEncString* pcon );
        // Modify... button
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CLangEncTextDlg)
    virtual void OnOK();
    virtual BOOL OnInitDialog();
    afx_msg void OnSelchangeLangEnc();
    afx_msg void OnInsertVariable();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CLangEncTextDlg


// --------------------------------------------------------------------------

class CFilterEl : public CDblListEl  // Hungarian: fel
{
private:
    CFilterCon* m_pcon;
    Str8 m_sElement;
    Str8 m_sInternalElement;  // 1997-12-01 MRP

    friend class CFilterElList;

    static BOOL s_bReadProperties(Object_istream& obs,
            CFilterSet* pfilsetMyOwner, CFilterEl** ppfel);
    static BOOL s_bSimpleElement(const Str8& sRead,
            Str8& sExternalElement, Str8& sInternalElement);
        // If sRead is equal (disregarding case) to the internal string
        // for a simple filter element (And, Or, Not, etc.) then
        // return its standard external and internal strings.
    
public:
    CFilterEl(CFilterCon* pcon);
    // CFilterEl(const char* psz);
    CFilterEl(const char* pszExternalElement, const char* pszInternalElement);
    virtual ~CFilterEl();
    
    const CFilterCon* pcon() const { return m_pcon; }
    const Str8& sElement() const { return m_sElement; }
    const Str8& sInternalElement() const { return m_sInternalElement; }

    BOOL bModifiable() const { return ( m_pcon != NULL ); }
    BOOL bModify();

private:
    static BOOL s_bReadText(Object_istream& obs, Str8& sText);
};  // class CFilterEl


// --------------------------------------------------------------------------

class CFilterElList : public CDblList  // Hungarian: fellst
{
private:
    CFilterSet* m_pfilsetMyOwner;
    
public:
    CFilterElList(CFilterSet* pfilsetMyOwner);
    CFilterElList(const CFilter* pfil);
    
    CFilterSet* pfilsetMyOwner() const { return m_pfilsetMyOwner; }

    CFilterEl* pfelFirst() const
        { return (CFilterEl*)pelFirst(); }
    CFilterEl* pfelLast() const
        { return (CFilterEl*)pelLast(); }
    CFilterEl* pfelNext( const CFilterEl* pfelCur ) const
        { return (CFilterEl*)pelNext( pfelCur ); }
    CFilterEl* pfelPrev( const CFilterEl* pfelCur ) const
        { return (CFilterEl*)pelPrev( pfelCur ); }
    
    void Insert(CFilterEl* pfelCur, CFilterEl* pfel);
    void Delete(CFilterEl** ppfel)
        { CDblList::Delete((CDblListEl**)ppfel); }

    BOOL bReadProperties(Object_istream& obs);
    BOOL bMatchAt(const CFilterEl** ppfel, const char* pszElement) const;

    BOOL bNewMarkerText(CFilterEl** ppfelNew);
    BOOL bNewMarkerText(const char* pszMarker, const char* pszText,
            CFilterEl** ppfelNew, CNote** ppnot);
    BOOL bNewMarkerDate(CFilterEl** ppfelNew);
    BOOL bNewMarkerDate(const char* pszMarker,
            int iRelation, const Str8& sDate,
            CFilterEl** ppfelNew, CNote** ppnot);
    BOOL bNewLangEncText(CFilterEl** ppfelNew);
    BOOL bNewLangEncText(CLangEnc* plng, const char* pszText,
            CFilterEl** ppfelNew, CNote** ppnot);
};  // class CFilterElList


class CFilterElListListBox : public CDblListListBox  // Hungarian: lbo
{
public:
    CFilterElListListBox(CFilterElList* pfellst) :
        CDblListListBox(pfellst) {}

    void InitListBox();     
    CFilterEl* pfelSelected() { return (CFilterEl*)pelSelected(); }

protected:
    int m_xElement;
    int m_xText;
    
    virtual void InitLabels();
    virtual int iItemHeight();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
};  // class CFilterElListListBox


// --------------------------------------------------------------------------

class CNewFilterDlg : public CDialog
{
    CFilter* m_pfil;  // the filter being viewed or added new
    CFilter** m_ppfilNew;  // means by which to return new filter
    CFilterSet* m_pfilset;  // filter set which owns the filter
    MatchSetting m_matset;
    
    CFilterElList m_fellst;
    CFilterElListListBox m_lboElements; 
    
// Dialog Data
    //{{AFX_DATA(CNewFilterDlg)
    enum { IDD = IDD_fil_New };
    Str8 m_sName;
    CEdit   m_edtName;
    CListBox    m_lboChoices;
    CButton m_btnModify;
    CButton m_btnDelete;
    CButton m_btnDeleteAll;
    BOOL    m_bMatchWhole;
    CComboBox m_cboMatchSetting;
    int     m_iMatchSetting;
    CEdit   m_edtNote;
    //}}AFX_DATA

public:
    CNewFilterDlg(CFilterSet* pfilset, CFilter** ppfilNew);
        // A dialog box for adding a new filter to the set
    CNewFilterDlg(const CFilter* pfilToCopyFrom, CFilter** ppfilNew);
        // A dialog box for copying an existing filter to the set
        // by copying from an existing one
    CNewFilterDlg(CFilter* pfil);
        // A dialog box for viewing and possibly modifying an existing filter

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    
    void Update(CFilterEl* pfel);
    void Insert(CFilterEl* pfelPrev);

    // Generated message map functions
    //{{AFX_MSG(CNewFilterDlg)
    virtual void OnOK();
    virtual void OnCancel();
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
};  // class CNewFilterDlg

