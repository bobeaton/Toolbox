#if !defined(AFX_EDITLNG_H__A3214591_DAA0_43F5_A483_B9D70B0EEA42__INCLUDED_)
#define AFX_EDITLNG_H__A3214591_DAA0_43F5_A483_B9D70B0EEA42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLng.h : header file
//

class CLangEnc;  // lng.h
class CMarker;  // mkr.h
class CKeyboard;  // kbd.h
/////////////////////////////////////////////////////////////////////////////
// CEditLng window

class CEditLng : public CEdit // 1.4qms Derive CEditLng from CEdit to fix combo drop // 1.4kya Change lang edit ctrl to derive from Wnd, not Edit
{
private:
	Str8 m_sText; // 1.4qmu CEditLng store text in member var, not wnd text
	CLangEnc* m_plng;
	int m_iCharPos;
	int m_iSelStart; // 1.4kyb Add member vars for select
	int m_iSelEnd; // 1.4kyb Add member vars for select
	BOOL m_bHasFocus;

public:
	CEditLng();
	void SetEditLngText( Str8 s ); // 1.4qna Add funcs to get and set text from CEditLng
	Str8 sGetEditLngText(); // 1.4qna Add funcs to get and set text from CEditLng
	void SetLangEnc(CLangEnc* plng);
	void SetSel( int iStart, int iEnd, BOOL bNoScroll = FALSE  ); // 1.4kya
//	void ReplaceSel( Str8 s ); // 1.4kya // 1.4vxs 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLng)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditLng();

#ifndef _MAC
	// Generated message map functions
protected:
	//{{AFX_MSG(CEditLng)
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
    afx_msg LRESULT OnUnicodeChar(WPARAM wParam, LPARAM lParam); // AB Unicode support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
    void OnCharEx(Str8& strChar, UINT nRepCnt, UINT nFlags); // 1.4qmt Change all CStrg in CEditLng to Str8
//    afx_msg LRESULT OnInputLangChange(WPARAM wParam, LPARAM lParam); // 1.4qzhm
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
#ifndef _UNICODE // 1.4zam 
    BOOL m_bConvertNext;
    Str8 m_chLeadByte;
    UINT m_cpIM;
#endif  // 1.4zam 
#endif // _MAC
};


/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLNG_H__A3214591_DAA0_43F5_A483_B9D70B0EEA42__INCLUDED_)
