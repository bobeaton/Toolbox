// RNE 1996-02-08 edlngctl.cpp
// Implementation of edit control for language encodings, markers, fonts, (and keyboards)

#include "stdafx.h"
#include "toolbox.h"
#include "edlngctl.h"
#include "lng.h"  // CLangEnc
#include "mkr.h"  // CMarker
#include "project.h"
#include "shw.h"
#include "kmn.h"  // ActivateKeyboard(), DeActivateKeyboards()


BEGIN_MESSAGE_MAP(CEditLngCombo, CEdit)
	ON_WM_PAINT()
	ON_WM_KILLFOCUS( )
	ON_WM_SETFOCUS( )
	ON_WM_LBUTTONDOWN() // 1.4qmr
END_MESSAGE_MAP()


CEditLngCombo::CEditLngCombo()
{
	m_plng = NULL;
	m_pmkr = NULL;
	m_pfnt = NULL;
	m_bKeyboardOverridden = FALSE;
	m_pkbd = NULL;
}

CEditLngCombo::~CEditLngCombo()
{
}


void CEditLngCombo::SetMarker(CMarker* pmkr)	
{ 
	m_pmkr = pmkr;
	ASSERT( m_pmkr );
	m_plng = NULL;
	m_pfnt = NULL;
	m_bKeyboardOverridden = FALSE;
	m_pkbd = NULL;
	
	CEdit::SetFont((CFont*)m_pmkr->pfntDlg());
}

void CEditLngCombo::SetLangEnc(CLangEnc* plng) 
{
	m_plng = plng;
	ASSERT( m_plng );
	m_pmkr = NULL;
	m_pfnt = NULL;
	m_bKeyboardOverridden = FALSE;
	m_pkbd = NULL;

	CEdit::SetFont((CFont*)m_plng->pfntDlg());
}

void CEditLngCombo::SetFont(CFont* pfnt)
{
	m_pfnt = pfnt;
	if (m_pfnt)
		CEdit::SetFont(m_pfnt);
	else // Set to the marker or lang. enc. font.
		{
		ASSERT(m_pmkr || m_plng);
		CEdit::SetFont((m_pmkr) ? (CFont*)m_pmkr->pfntDlg()
							: (CFont*)m_plng->pfntDlg());
		}
}

void CEditLngCombo::SetKeyboard(CKeyboard* pkbd)
{
	m_bKeyboardOverridden = TRUE;
	m_pkbd = pkbd;
}

void CEditLngCombo::OnSetFocus(CWnd* pwnd)
{
	CEdit::OnSetFocus(pwnd);
	Invalidate(); // 1.4qme Try to make edit control paint Unicode

}

void CEditLngCombo::OnKillFocus(CWnd* pwnd)
{
	// Simply disable keyman.
	Shw_pProject()->pkmnManager()->DeActivateKeyboards();
		
	CEdit::OnKillFocus(pwnd);
}

void CEditLngCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	CWnd::OnLButtonDown(nFlags, point);
	CString s;
	GetWindowText( s );
//	if ( s.Find( '\n' ) <= 0 ) // 1.4hea Don't jump to end of multi-line lng edit ctrl on mouse click
//		m_iCharPos = s.GetLength(); // Go to end, later calculate better
	SetWindowText( s );
	SetSel( -1, 0 ); // Clear selection, if any // 1.4kya For Wnd will need to implement copy for self // 1.4kyb
	Invalidate();
	SetFocus(); // 1.4qma Fix focus problem of new language edit control
}
