// jumpdlg.h : header file
//
#ifndef Jumpdlg_h
#define	Jumpdlg_h

#include "resource.h"
#include "EditLng.h"
#include "lng.h"  // CLangEncSetComboBox
#include "cbo.h" // CSetComboBox

class CKey;  // recset.h
class CDatabaseType;  // typ.h
class CKeyboard;  // kbd.h
class CJumpPath;  // jmp.h
class CJumpPathSet;  // jmp.h

class CJumpPathSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CJumpPathSetComboBox(CJumpPathSet* pjmpset) : CSetComboBox((CSet *)pjmpset, TRUE, NULL) // 1.4gg
		{}
    
    void UpdateElements(CJumpPath* pjmp)
        { CSetComboBox::UpdateElements((CSetEl *)pjmp); }
    CJumpPath* pjmpSelected()
        { return (CJumpPath*)CSetComboBox::pselSelected(); }
};  // class CJumpPathSetComboBox


// **************************************************************************

/////////////////////////////////////////////////////////////////////////////
// CJumpDlg dialog

class CDatabaseType;

class CJumpDlg : public CDialog
{
private:
	CKey* m_pkey;
	CDatabaseType* m_ptyp;
	CLangEnc** m_pplng;
    CLangEnc* m_plng; // TLB: 06/08/99
    BOOL m_bIsJumpInsert;
    BOOL m_bLngEncChangedByUser; // TLB: 06/08/99
    CJumpPath** m_ppjmp; // TLB: 06/07/99
    CJumpPath* m_pjmp; // TLB: 06/08/99
    
	CEditLng	m_elcJumpKey; // 1.4qpn

// Dialog Data
	//{{AFX_DATA(CJumpDlg)
	enum { IDD = IDD_JUMP };
	CLangEncSetComboBox	m_cboLangEncSet;
    CJumpPathSetComboBox m_cboPriJumpPath;
	Str8	m_sKey;
	BOOL	m_bMatchWhole;
	int		m_iMatchSetting;
	//}}AFX_DATA
	MatchSetting m_matset;

public:
	CJumpDlg( CKey* pkey, CDatabaseType* ptyp, CLangEnc** pplng, CJumpPath** ppjmp, BOOL bIsJumpInsert );

// Implementation
protected:
    void SetMatchSetting(MatchSetting matset);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CJumpDlg)
	afx_msg void OnJumppath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLangsetCombo();
	afx_msg void OnSelchangePriJumpPathCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // Jumpdlg_h
