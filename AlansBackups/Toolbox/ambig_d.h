// ambig_d.h Choose among ambiguities

#ifndef AMBIG_D_H
#define AMBIG_D_H

#include "resource.h"
#include "EditLng.h" 

// --------------------------------------------------------------------------
class CAmbigListBox : public CDblListListBox
{
private:
	CFieldList* m_pfldlst;
	int m_xWord;

public:
	CAmbigListBox(CFieldList* pfldlst);

	void InitListBox();
	virtual int iItemHeight();
	void Close();
	CFieldList* pfldlst() { return m_pfldlst; }
	CField* pfldSelected() { return (CField*)pelSelected(); }

protected:
	virtual void InitLabels();
	virtual void DrawElement(CDC& cDC, const RECT& rcItem, const CDblListEl *pel);
};

// --------------------------------------------------------------------------

class CWordFormulaSet;  // wdf.h

class CAmbigDlg : public CDialog  // Hungarian: dlg
{
private:
	CFieldList* m_pfldlst;
	CAmbigListBox m_lboAmbigs;

    CFieldList* m_pfldlstExcluded;
    CWordFormulaSet* m_pwdfset;
	
// Dialog Data
	//{{AFX_DATA(CAmbigDlg)
	enum { IDD = IDD_AMBIG };
    CButton m_btnFormulas;
	//}}AFX_DATA

public:
	CAmbigDlg( CFieldList* pfldlst );  // Lookup
	CAmbigDlg( CFieldList* pfldlst, CFieldList* pfldlstExcluded,
            CWordFormulaSet* pwdfset );  // Parse
		
	BOOL bListEmpty()	{ return ( m_lboAmbigs.GetCount() <= 0 ); }

private:
	void DeleteAllUnselected(); // Delete all not selected

protected:			
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	virtual BOOL OnInitDialog();

	// Generated message map functions
	//{{AFX_MSG(CAmbigDlg)
	virtual void OnOK();
	afx_msg void OnDblclkAmbigList();
	afx_msg void OnFormulas();
	afx_msg void OnAmbigFont();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif // AMBIG_D_H

	
/////////////////////////////////////////////////////////////////////////////
// CLookFailDlg dialog

class CLookFailDlg : public CDialog
{
private:
    CLangEnc* m_plng; // language of key

	// Edit control for the key entry area.
	CEditLng	m_elcKey; // 1.4qpn

	// Construction
public:
	CLookFailDlg( const char* pszKey, const CLangEnc* m_plng, CWnd* pParent = NULL);	// standard constructor
	const char* pszKey(); // Return modified key

// Dialog Data
	//{{AFX_DATA(CLookFailDlg)
	enum { IDD = IDD_LOOKFAIL };
	Str8	m_sKey;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CLookFailDlg)
	afx_msg void OnYes();
	afx_msg void OnNo();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnWholeWord();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
