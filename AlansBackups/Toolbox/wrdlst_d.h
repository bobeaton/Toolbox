// wrdlst_d.h - Word list and corcordance dialog boxes

#ifndef Wrdlst_d_h
#define Wrdlst_d_h

#include "resource.h"
#include "corpus.h"
#include "EditLng.h"

// --------------------------------------------------------------------------

class CWordListDlg : public CDialog
{

private:

// Dialog Data
    //{{AFX_DATA(CWordListDlg)
    enum { IDD = IDD_WORDLST };
    CCorpus*        m_pcor;     // could be NULL if no corpus in project yet
    Str8         m_sOutput;  // SF output file name (or path)
    BOOL            m_bCount;
    BOOL            m_bRefs;
    int             m_nLimitRefs;
    CEdit           m_edRefCount;
    CEdit           m_edOutput;
    int             m_nRefCount;
    CButton         m_btnUnlimited;
    CButton         m_btnLimited;
    CCorpusComboBox m_cboCorpus;
    CStatic         m_lblCorpusDir;
    //}}AFX_DATA

// Construction
public:
    CWordListDlg(CCorpusSet* pcorset, CWnd* pParent = NULL);

// Interface
public:
	CCorpus* pcor() { return m_pcor; }
    void DoWordList();

// Implementation
private:
    void ShowCorpusFolder();
    Str8 sOutputFullPath();      //get full path of output file
    void SimplifyOutputFileName();  //simplify m_sOutput
    CDatabaseType* CWordListDlg::ptypGetWordListType(CCorpus* pcor);

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CWordListDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnRefs();
    afx_msg void OnLimited();
    afx_msg void OnUnlimited();
    afx_msg void OnBrowse();
    afx_msg void OnCorpusSelChange();
    afx_msg void OnCorpusEdit();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


// --------------------------------------------------------------------------

class CConcordanceDlg : public CDialog
{
private:

// Dialog Data
    //{{AFX_DATA(CConcordanceDlg)
    enum { IDD = IDD_CONCORDANCE };
    CCorpus*        m_pcor;     // could be NULL if no corpus in project yet
    Str8         m_sOutput;  // SF output file name (or path)
    int             m_nLimitMatches;
    int             m_nMatchCount;
    Str8         m_sSearch;
    int             m_nSearchType;
    BOOL            m_bMatchCase;
    BOOL            m_bAlignMatchedStrings; // 1.4vyx 
    CEdit           m_edMatchCount;
    CEditLng		m_elcSearch;  // 1.4qpn
    CEdit           m_edOutput;
    CCorpusComboBox m_cboCorpus;
    CStatic         m_lblCorpusDir;
    //}}AFX_DATA

// Construction
public:
    CConcordanceDlg(CCorpusSet* pcorset, CWnd* pParent = NULL);

// Interface
public:
    void DoConcordance(const Str8& sCurrentWord);    // 4-2-98 added paramter

// Implementation
private:
    void ShowCorpusFolder();
    Str8 sOutputFullPath();      //get full path of output file
    void SimplifyOutputFileName();  //simplify m_sOutput
    CDatabaseType* ptypGetConcordanceType(CCorpus* pcor);
    void SetSearchLangEnc();  // 1998-07-15 MRP
        // Set appropriate font and keyboard.

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CConcordanceDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnLimited();
    afx_msg void OnUnlimited();
    afx_msg void OnBrowse();
    afx_msg void OnCorpusSelChange();
    afx_msg void OnCorpusEdit();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

class WrdInfo // wrd Info about word for word list // 1.4ted 
	{
//	friend class WrdInfoList; // 1.4ted Let word info list access pointers
public:
	Str8 sWord; // Word
	int iCount; // Count
	Str8 sRefs; // References
	WrdInfo* pwrdNext; // Pointer to next in linked list
public:
	WrdInfo();
	WrdInfo( const char* pszWord );
	~WrdInfo() {};
	};

class WrdInfoList // wrdlst List of word info structures, word list // 1.4ted 
	{
public:
	WrdInfo* pwrdFirst; // First word in list
public:
	WrdInfo* pwrdAdd( const char* pszWord, WrdInfo* pwrdPrev ); // Add a new word to the list
	WrdInfo* pwrdSearch( const char* pszWord, WrdInfo*& pwrdPrev ); // Search for a word
	void MoveToTop( WrdInfo* pwrd, WrdInfo* pwrdPrev ); // Move a word to top of list for faster search
	WrdInfoList();
	~WrdInfoList();
	};

#endif
