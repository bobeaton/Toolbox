// dlgsort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortDlg dialog

#include "resource.h"
#include "ind.h"

class CSortDlg : public CDialog
{
private:
	CIndex** m_ppind;
	CIndex** m_ppindUnfiltered;
	CIndexSet* m_pindset;
	
	CMarkerSet* m_pmkrset;	// The set of markers for the document in question.
//	CMarker*	m_pmkrRecord;
	CMarker* m_pmkrPriKey;
	CSortOrder* m_psrtPri;
	const CMarkerRefList*  m_pmrflstSecKeys;
	CMarkerRefList	m_mrflstUsersChoice;
	CFilter* m_pfil;
	UINT	m_iSortKeyLen;

// Dialog Data
	//{{AFX_DATA(CSortDlg)
	enum { IDD = IDD_SORTINDEX };
	BOOL	m_bSortFromEnd;
	//}}AFX_DATA
	
// Internal operations.
	void LoadSortOrdersCbx();
	void LoadLBMkrsAvailable(const char* pszMrkSel=NULL); // load markers available listbox

// Construction
public:
	CSortDlg(CIndex** ppind, CIndex** ppindUnfiltered);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CSortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSortindexMovefrom();
	afx_msg void OnSortindexMoveto();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDblclkSortindexLstmrkstosort();
	afx_msg void OnDblclkSortindexLstmrksavailable();
	afx_msg void OnSortindexFirstto();
	afx_msg void OnSortindexLastto();
	afx_msg void OnSortindexClearsorts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
