// Find_d.h  Interface for Find dialog box  (1995-05-24)

#ifndef Find_D_H
#define Find_D_H

#include "resource.h"  // IDD_FIND
#include "lng.h"
#include "srt.h"  // MatchSetting
#include "mkr.h"
#include "EditLng.h"
// #include "edlngctl.h" // 1.4ym 


/////////////////////////////////////////////////////////////////////////////
// CFindText

class CFindText : public CDblListEl  // Hungarian: ftx
{
private:
    Str8 m_sText; // Text to find
    CLangEnc* m_plng; // language of text

    friend class CFindTextList;
        
#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CFindText** ppftx);

    CFindText( const CFindText& ftx ) // Copy constructor
        { m_sText = ftx.m_sText;
        m_plng = ftx.m_plng; }

public:
    CFindText(const char* pszText, CLangEnc* plng)
        { m_sText = pszText; m_plng = plng; ASSERT(plng); }
        
    CLangEnc* plng() { return m_plng; } // return language used for this find
    const char* pszText() { return m_sText; } // return text used for this find
};  // class CFindText


/////////////////////////////////////////////////////////////////////////////
// CFindTextList

class CFindTextList : public CDblList  // Hungarian: ftxlst
{
public:
    CFindTextList() {}; // Constructor

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);

    CFindText* pftxFirst() const { return (CFindText*)pelFirst(); }
    CFindText* pftxNext( const CFindText* pftx) const { return (CFindText*)pelNext(pftx); }
    CFindText* pftxPrev( const CFindText* pftx) const { return (CFindText*)pelPrev(pftx); }
    CFindText* pftxLast() const  { return (CFindText*)pelLast(); }    

    void Add(const char* pszText, CLangEnc* plng); // Add latest find text to list
    void Add(CFindText* pftx); // Add CFindText object to list
    
        // Delete element without changing pel, needed for doubly derived classes
    void Delete( CFindText* pftx )
        { CDblList::Delete( (CDblListEl*)pftx ); }
        // Delete element, pel is changed to Next if it exists, else Prev if it exists, else NULL
    void Delete( CFindText** ppftx )
        { CDblList::Delete( (CDblListEl**)ppftx ); }
    
    CFindText* pftxSearch( const char* pszText ); // search list for exact match of text
    
    void DeletingLanguage( CLangEnc* plng ); // notification of language being destroyed - called from language encoding destructor
};  // class CFindTextList

/////////////////////////////////////////////////////////////////////////////
// CFindSettings

class CFindSettings  // Hungarian: fndset
{
private:
    // 1999-08-13 MRP: Initializing Match Characters for Find and Replace.
    MatchSetting m_matsetFind;  // the user's previous choice for a Find
    MatchSetting m_matsetCur;  // the current operation: Find or Replace

    BOOL m_bMatchWhole; // if TRUE match whole word only
    BOOL m_bSingleField; // if TRUE search only fields of marker m_sMkr, else search all fields of language m_plng
    BOOL m_bSingleRecord; // if TRUE search only current record, else search entire record set
    Str8 m_sMkr; // this isn't a CMarker* because CFindSettings may be global (across database types)
	CLangEnc* m_plng; // 1.4zaj Remember language encoding
    Str8 m_sReplaceText; // text for replace
    CFindTextList m_ftxlst; // list of previous find texts

    // non-persistant members
    BOOL m_bReplace; // signals replace operation
    BOOL m_bReplaceAll; // signals replace all operation

public:
    CFindSettings();
    
    void Set( MatchSetting matset, BOOL bMatchWhole, BOOL bSingleField, BOOL bSingleRecord,
                const char* pszMkr, const char* pszReplaceText, BOOL bReplace, BOOL bReplaceAll );
    
    CFindTextList* pftxlst() { return &m_ftxlst; } // return pointer to list of previous find texts

    MatchSetting matsetFind() const { return m_matsetFind; }  // 1999-08-13 MRP
    MatchSetting matsetCur() const { return m_matsetCur; }
    BOOL bMatchWhole() { return m_bMatchWhole; } // if TRUE match whole word only
    BOOL bSingleField() { return m_bSingleField; } // if TRUE search only fields of marker m_sMkr, else search all fields of language m_plng
    BOOL bSingleRecord() { return m_bSingleRecord; } // if TRUE search only current record, else search entire record set
    void SetMatchWhole( BOOL b ) { m_bMatchWhole = b; } // 1.1bt Set match whole
    void SetSingleField( BOOL b ) { m_bSingleField = b; } // 1.1bt Set match single field
    void SetSingleRecord( BOOL b ) { m_bSingleRecord = b; } // 1.1bt Set match single record
    const char* pszMkr() { return m_sMkr; } // this isn't a CMarker* because CFindSettings may be global (across database types)
	void SetMkr( const char* psz ) { m_sMkr = psz; } // 1.4yd Set match marker
	void SetLng( CLangEnc* plng ) { m_plng = plng; } // 1.4zaj Remember language encoding
    CLangEnc* plng( CShwView* pview=NULL ); // return language used for last find
    const char* pszReplaceText() { return m_sReplaceText; } // return replacement text

    BOOL bReplace() { return m_bReplace; }
    BOOL bReplaceAll() { return m_bReplaceAll; }
    void ClearReplace() { m_bReplace = m_bReplaceAll = FALSE; }

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);
    
    BOOL bModalView_Properties( CDatabaseType* ptyp, const char* pszText, BOOL bIsReplace ); // bring up Find dialog
};

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog

class CFindDlg : public CDialog
{
private:
    CFindSettings* m_pfndset;
    CDatabaseType* m_ptyp;
    BOOL m_bIsReplace; // indicates whether we're using Find or Replace dialog
	BOOL m_bSelectedText; // 1.4zaj Indicates whether find came from selected text

    // Edit control for the text entry area.
    CEditLng   m_elcText; // 1.4qpn
    CEditLng   m_elcReplace; // 1.4qpn

// Dialog Data
    //{{AFX_DATA(CFindDlg)
    CLangEncSetComboBox m_cboLangEnc;
    CMarkerSetComboBox  m_cboField;
    Str8 m_sText;
    Str8 m_sReplace;
    BOOL    m_bMatchWhole;
    int     m_iMatchSetting;
    int     m_bNotSingleRecord;
    int     m_bNotSingleField;
    //}}AFX_DATA
    MatchSetting m_matset;

public:
    CFindDlg(CFindSettings* pfndset, CDatabaseType* ptyp, const char* pszText, BOOL bIsReplace);
    void DoIt( BOOL bReplaceAll );

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CFindDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnReplaceAll();
    afx_msg void OnSelchangeFindLngcombo();
    afx_msg void OnSelchangeFindMkrcombo();
    afx_msg void OnFindInFld();
    afx_msg void OnFindInLng();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CFindLangEdit - edit control class for subclassing edit control of CFindComboBox
class CFindLangEdit : public CEditLng // 1.4qmc Change find CLngEditCtrl to CEditLng // 5.96x Change from CEdtLng // 1.4ym 
{
private:

public:
    virtual WNDPROC* GetSuperWndProcAddr(); // needed for subclass operation
    
protected:
	afx_msg void OnSetFocus(CWnd* pOldWnd); // 1.4ym 
private:
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags); // needed to catch Esc and Enter
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CFindComboBox dialog

class CFindComboBox : public CComboBox  // Hungarian: cbo
{
private:
    CFindTextList m_ftxlstEmpty; // for when no project is loaded
    // 2000-08-29 TLB & MRP : to avoid accessing freed memory, access this data via project.
    // CFindTextList* m_pftxlst;  // the list (or set) of elements to be viewed
    CFindLangEdit m_elcFind; // subclassed
    CLangEnc* m_plng; // used to remember language in use by child edit control
    CFont m_fnt; // used to override combo's edit control font because find combo requires a small font (smaller than dialog language controls)
    int m_iHeight; // remember initial height of combo. On Mac, edit control can resize when we set the language (font)

    CFindTextList* pftxlst();

public:

    // 2000-08-29 TLB & MRP : This object is never constructed with anything but an empty list.
    // CFindComboBox(CFindTextList* pftxlst=NULL);
    CFindComboBox();
    
    // 2000-08-29 TLB & MRP : CFindCombo now accesses CProject to get current CFindTextList.
    // void ChangeSet(CFindTextList* pftxlst); // change to different list
    
    void UpdateElements();  // Add the list's elements as items in the list box,
                            // first removing any items that had already been in it.

    BOOL bHasChanged(CFindTextList* pftxlst); // see if list matches what's in the combo listbox
    void LangEncUpdated(CLangEncUpdate& lup); // update edit control's font
    void Init(); // subclass combo's edit control and set edit control's initial language
    CFindText* pftxSelected(); // return currently selected find text from combo's listbox (may be NULL)

    void SetLangEnc(CLangEnc* plng); // set language of child edit control
    CLangEnc* plng() { return m_plng; } // return language in use by child edit control
    BOOL bHasFocus() // return TRUE if combo (or combo's edit control) has focus
        { return GetFocus() == &m_elcFind || GetFocus() == this; }
	CFindLangEdit* elc() { return &m_elcFind; } // 1.4ym 
private:
    virtual void MeasureItem(LPMEASUREITEMSTRUCT);
    virtual void DrawItem(LPDRAWITEMSTRUCT);

//  DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAskReplaceDlg dialog

class CAskReplaceDlg : public CDialog
{
private:
    CShwView* m_pviewCaller;
    CPoint m_pntCaret;

// Construction
public:
    CAskReplaceDlg(CShwView* pview, CPoint pntCaret);   // standard constructor

    void CheckPosition( CPoint pntCaret ); // make sure dialog is not covering find text
    void FindAgain( BOOL bDoReplace ); // do replace based on bDoReplace and do another Find

// Dialog Data
    //{{AFX_DATA(CAskReplaceDlg)
    enum { IDD = IDD_ASK_REPLACE };
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAskReplaceDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CAskReplaceDlg)
    afx_msg void OnSkip();
    virtual void OnOK();
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};
#endif  // not Find_D_H
