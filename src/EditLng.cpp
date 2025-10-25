// EditLng.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "EditLng.h"
#include "lng.h"  // CLangEnc
#include "mkr.h"  // CMarker
#include "project.h"
#include "kmn.h"  // ActivateKeyboard(), DeActivateKeyboards()
#include "SHConv.h" // 1.4qzhm // 1.4zam 
#include <afxpriv.h> // For WM_COMMANDHELP

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditLng

CEditLng::CEditLng()
{
	m_plng = NULL; // Shw_plngset()->plngDefault(); // 1.4qpp Use default language in language edit control if none assigned // 1.4ym Fix crash on initial open of project // 1.4ym 
	m_iCharPos = 0;
	m_iSelStart = 0; // 1.4kyb
	m_iSelEnd = 0; // 1.4kyb
	m_bHasFocus = FALSE;
#ifndef _UNICODE // 1.4zam 
    m_bConvertNext = FALSE; // 1.4qzhm // 1.4zam 
    m_cpIM = CP_ACP; // 1.4qzhm // 1.4zam 
#endif  // 1.4zam 
	CEdit::CEdit(); // 1.4vyn Fix bug of not scrolling multi-line edit control
}

CEditLng::~CEditLng()
{
	CEdit::~CEdit(); // 1.4vyn Fix bug of not scrolling multi-line edit control
}

void CEditLng::SetLangEnc(CLangEnc* plng) 
{
	m_plng = plng;
	CEdit::SetFont( (CFont*)m_plng->pfnt() ); // 1.4vxq Set font of underlying edit control for Unicode use of standard CEdit
}

void CEditLng::SetEditLngText( Str8 s ) // 1.4qna Set text in CEditLng
	{ 
	m_sText = s; 
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		s.Replace( "\n", "\r\n" ); // 1.4vxq 
		CEdit::SetWindowText( swUTF16( s ) ); // 1.4vxq 
		return; // 1.4vxq 
		}
#endif
	}

Str8 CEditLng::sGetEditLngText() // 1.4qna Get text from CEditLng
	{ 
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CString sw;
		CEdit::GetWindowText( sw ); // 1.4vxq 
		m_sText = sUTF8( sw ); // 1.4vxq 
		m_sText.Replace( "\r\n", "\n" ); // 1.4vxq 
		}
#endif
	return m_sText; 
	}

void CEditLng::SetSel( int iStart, int iEnd, BOOL bNoScroll ) // 1.4kya For Wnd will need to implement copy and paste for self
	{ // 1.4kyc For now, don't implement bNoScroll, it is only used one place, see if it really matters
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::SetSel( iStart, iEnd, bNoScroll ); // 1.4vxq 
		}
#endif
	m_iSelStart = iStart; // 1.4kyb
	m_iSelEnd = iEnd; // 1.4kyb
	}

BEGIN_MESSAGE_MAP(CEditLng, CEdit) // 1.4qms Try derive from CEdit again to fix combo drop // 1.4kya Change lang edit ctrl to derive from Wnd, not Edit
	//{{AFX_MSG_MAP(CEditLng)
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UNICHAR, OnUnicodeChar)
//    ON_MESSAGE(WM_INPUTLANGCHANGE,OnInputLangChange) // 1.4qzhm
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLng message handlers

int iBeforeCurPos( const char* psz, int iCurPos, int iLines ) // Return position desired number of lines before current pos
{
    Str8 s( psz, iCurPos ); // Get just first part
	int iN = s.Find( '\n' ); // 1.4qmx Fix bug (1.4qmw) of assert after paste in edit control
	if ( iN < 0 ) // 1.4qmx Reverse find seems to return incorrect result in Korean Unicode
		return 0; // 1.4qmx
	for ( ; iLines; iLines-- )
	{
		iCurPos = s.ReverseFind( '\n' );
		if ( iCurPos < 0 )
			return 0;
		else
			s = s.Left( iCurPos );
	}
    return iCurPos + 1;
}

void CEditLng::OnPaint() 
{
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnPaint(); // 1.4vxq 
		return;
		}
#endif
	CPaintDC dc(this); // device context for painting
	CBrush  brs;
    brs.CreateSolidBrush( GetSysColor(COLOR_WINDOW) );
	CRect rect;
	CWnd::GetClientRect( rect );
    dc.FillRect( rect , &brs); // Paint background
	CDC cdc;
	cdc.Attach( dc.m_hDC );
	Str8 sDisplay = m_sText; // 1.4qmv Fix bug (1.4qmu) of not displaying text in edit control
	BOOL bHighlight = ( m_iSelEnd > 0 ); // 1.4kyb
    if ( m_bHasFocus && !bHighlight )
		sDisplay.Insert( m_iCharPos, "|" ); // Add caret simulator to focused control // 1.4qmu // 1.4qvc Upgrade Insert for Unicode build
	CRect rectFont = rect; // Special rect so font can be larger
	rectFont.bottom += 10; // Allow larger font
	if ( Shw_papp()->bProjectOpen() && m_plng )
		{
		m_plng->SetTextAndBkColorInDC( &cdc, bHighlight ); // Set colors
		cdc.SelectObject( (CFont*)m_plng->pfnt() ); // pfntDlg() ); // Set font // TryLarge
		int iLineStart = iBeforeCurPos( sDisplay, m_iCharPos, 2 ); // 1.4qmu
		int iLineEnd = 0;
		int iY = 0;
		while ( iLineEnd < sDisplay.GetLength() ) // 1.4qmu
			{
			iLineEnd = sDisplay.Find( "\n", iLineStart ); // 1.4qmu
			if ( iLineEnd < 0 )
				iLineEnd = sDisplay.GetLength(); // 1.4qmu
			m_plng->ExtTextOutLng( &cdc, 0, iY, ETO_CLIPPED, &rectFont, (const char*)sDisplay + iLineStart, iLineEnd - iLineStart, 0 ); // 1.4qmu
			iLineStart = iLineEnd + 1;
			iY += Shw_pProject()->iLangCtrlHeight();
			}
		}
	else
		dc.TextOut( 0, 0,  swUTF16( sDisplay ) ); // 1.4qmu // 1.4qyv
}

// Unicode char input routine
LRESULT CEditLng::OnUnicodeChar( WPARAM wParam, LPARAM lParam )
{
	if ( wParam == UNICODE_NOCHAR )
		return 1;
	Str8 sUTF8 = (const char*)sUTF32ToUTF8( wParam ); // 1.4hey Fix mismatch of CString and Str8 in CEditLng
    // since we've made OnCharEx support a string of chars (rather than one at a time), just
    //  call that directly with the whole thing.
    OnCharEx(sUTF8, 0, 0);
	return 1;
}

void CEditLng::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	USES_CONVERSION; // 1.4xe 
	CString sw((wchar_t)nChar); // 1.4qyu // 1.5.0fm 
    Str8 strChar; // 1.4ua Fix U bug of legacy keyboard input not working, didn't work because of code page conversion
    if( m_plng && m_plng->bUnicodeLang() ) // 1.4ua  // 1.5.0ff  // 1.5.0fm 
		strChar =  sUTF8( sw ); // 1.4qyu // 1.5.0fm 
	else
	    strChar = W2CA( sw );  // 1.4xe Fix U bug of incorrect chars from legacy Keyman // 1.5.0fm 
	if ( m_plng )
		m_plng->ApplyKeyDefs( strChar ); // 1.5.0fm Implement key definitions in edit control
	if ( strChar != "" ) // 1.4qmx
		{
		if ( m_plng && m_plng->bUnicodeLang() ) // 1.5.0fm 
			{
			sw = swUTF16( strChar ); // 1.5.0fm Change back to UTF16 for standard CEdit
		    if ( nChar < 32 ) // If backspace or other control code, send it to CEdit // 1.5.0fm 
				CEdit::OnChar( nChar, nRepCnt, nFlags ); // 1.5.0fm 
			else
				ReplaceSel( sw ); // 1.5.0fm Inserts at current cursor location
			}
		else
		    OnCharEx(strChar, nRepCnt, nFlags);
		}
}

void CEditLng::OnCharEx(Str8& strChar, UINT nRepCnt, UINT nFlags)
{
    UINT nChar = strChar[0];    // for the operator==() testing for control chsr below
	if ( nChar == 3 || nChar == 24 ) // 5.99n If copy or cut, do copy and leave everything alone
		{
//		Copy(); // 1.4kya For Wnd will need to implement copy in ctrl
		return;
		}
	if ( m_iSelEnd > 0 ) // If selected, clear all // 1.4kyb
		{
		m_sText = ""; // 1.4qmu Clear text
		m_iCharPos = 0;
		}
	if ( nChar == '\b' ) // If backspace, delete back 1
		{
		if ( m_iCharPos > 0 )
			{
			int ichlen = m_plng ? 
				m_plng->iCharLeftNumBytes( (const char*)m_sText + m_iCharPos, m_iCharPos ) // 1.4gz // 1.4qmu
				: 1;
			m_sText.Delete( m_iCharPos - ichlen, ichlen ); // Delete char // 1.4qmu
			m_iCharPos -= ichlen; // Move back amount deleted
			}
		}
	else if ( nChar == '\r' ) // If Enter insert line feed
		{
		m_sText.Insert( m_iCharPos, "\r\n" ); // Insert crlf // 1.4qmu
		m_iCharPos += 2;
		}
	else if ( nChar == 22 ) // If Ctrl+V paste, insert paste buffer
		{
		Str8 sPaste = sGetClipBoardData( this, m_plng->bUnicodeLang() );  // 1.4rbd Fix U bug of failing to paste Unicode into edit controls
		m_sText.Insert( m_iCharPos, sPaste ); // Insert pasted text
		m_iCharPos += sPaste.GetLength(); // Update position
		}
	else if ( nChar == 27 ) // 1.4ss Fix U bug of escape showing as box in edit control
		{
		; // 1.4ss Don't insert escape char
		}
	else
		{
		m_sText.Insert( m_iCharPos, strChar ); // Insert char(s) // 1.4qmu
        m_iCharPos += strChar.GetLength();
		}
//	SetWindowText( m_sText ); // 1.4qnf Temp use WindowText in edit ctrl until change for IME // 1.4qpk
	SetSel( -1, 0 ); // Clear selection, if any // 1.4kya Wnd doesn't have select member vars // 1.4kyb
	Invalidate();
}

void CEditLng::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnKeyDown( nChar, nRepCnt, nFlags ); // 1.4vxq 
		return;
		}
#endif
	if ( nChar == VK_DELETE )
	{
		if ( m_iSelEnd > 0 ) // If selected, clear all // 1.4kyb
		{
			m_sText = ""; // 1.4qmu Clear text
			m_iCharPos = 0;
		}
		else
		{
			int ichlen = m_plng ? 
				m_plng->iCharNumBytes( (const char*)m_sText + m_iCharPos ) // 1.4qmu
				: 1;
			m_sText.Delete( m_iCharPos, ichlen ); // 1.4qmu
		}
	}
	else if ( ( nChar == VK_RIGHT && !m_plng->bRightToLeft() ) 
		|| ( nChar == VK_LEFT && m_plng->bRightToLeft() ) )
	{
		if ( m_iSelEnd > 0 ) // If selected, go to end // 1.4kyb
			m_iCharPos = m_sText.GetLength(); // 1.4qmu
		else if ( m_iCharPos < m_sText.GetLength() ) // 1.4qmu
		{
			int ichlen = m_plng ? 
				m_plng->iCharNumBytes( (const char*)m_sText + m_iCharPos ) // 1.4qmu
				: 1;
			m_iCharPos += ichlen;
		}
	}
	else if ( ( nChar == VK_LEFT && !m_plng->bRightToLeft() ) 
		|| ( nChar == VK_RIGHT && m_plng->bRightToLeft() ) )
	{
		if ( m_iSelEnd > 0 ) // If selected, go to beginning // 1.4kyb
			m_iCharPos = 0;
		else if ( m_iCharPos > 0 )
		{
			int ichlen = m_plng ? 
				m_plng->iCharLeftNumBytes( (const char*)m_sText + m_iCharPos, m_iCharPos ) // 1.4gz // 1.4qmu
				: 1;
			m_iCharPos -= ichlen;
		}
	}
	else if ( nChar == VK_DOWN )
	{
		if ( m_iSelEnd > 0 ) // If selected, go to end // 1.4kyb
			m_iCharPos = m_sText.GetLength(); // 1.4qmu
		else if ( m_iCharPos < m_sText.GetLength() ) // 1.4qmu
		{
			int iNextLine = m_sText.Find( "\n", m_iCharPos ); // 1.4qmu
			if ( iNextLine > 0 )
				m_iCharPos = iNextLine + 1;
		}
	}
	else if ( nChar == VK_UP )
	{
		if ( m_iSelEnd > 0 ) // If selected, go to beginning // 1.4kyb
			m_iCharPos = 0;
		else if ( m_iCharPos > 0 )
		{
			while ( m_iCharPos > 0 )
			{
				m_iCharPos--; // Step back 1
				if ( m_sText[m_iCharPos] == '\n' && m_iCharPos > 0 ) // If at end of prev line, step past \r // 1.4qmu
				{
//					m_iCharPos--; // 1.4qpn Not needed
					break;
				}
			}
		}
	}
	else if ( nChar == VK_HOME )
	{
		m_iCharPos = 0; // Go to beginning
	}
	else if ( nChar == VK_END )
	{
		m_iCharPos = m_sText.GetLength(); // Go to end // 1.4qmu
	}
	else
		return; // If none of the above commands, let OnChar handle it
	SetSel( -1, 0 ); // Clear selection, if any  // 1.4kya For Wnd will need to implement copy in ctrl // 1.4kyb
	Invalidate();
}

void CEditLng::OnSetFocus(CWnd* pOldWnd) 
{
	if ( m_plng ) // 1.4vzg Fix U bug of not setting keyboard in edit cntrl
		Shw_pProject()->pkmnManager()->ActivateKeyboard(m_plng);
	m_bHasFocus = TRUE;
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnSetFocus( pOldWnd ); // 1.4vxq 
		return;
		}
#endif
	Invalidate();
}

void CEditLng::OnKillFocus(CWnd* pNewWnd) 
{
	Shw_pProject()->pkmnManager()->DeActivateKeyboards(); // 1.4vzg 
	m_bHasFocus = FALSE;
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnKillFocus( pNewWnd ); // 1.4vxq 
		return;
		}
#endif
	Invalidate();
}

void CEditLng::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnVScroll(nSBCode, nPos, pScrollBar); // 1.4vxs 
		return;
		}
#endif
}

void CEditLng::OnLButtonDown(UINT nFlags, CPoint point) 
{
#ifdef _UNICODE // 1.4vxq 
	if ( !m_plng || m_plng->bUnicodeLang() ) // 1.4vxq If Unicode lng, use standard CEdit
		{
		CEdit::OnLButtonDown( nFlags, point ); // 1.4vxs 
		return;
		}
#endif
	SetSel( -1, 0 ); // Clear selection, if any // 1.4kya For Wnd will need to implement copy for self // 1.4kyb
    CDC* pDC = GetDC(); // Get device context // 1.4vxy 
	BOOL bMultiLine = ( m_sText.Find( '\n' ) >= 0 ); // 1.4vxy Don't try to do mouse click on multiline custom edit control
	int i = 0;
	for ( ; i < m_sText.GetLength(); i++ ) // 1.4vxy While not at end, see if mouse point is reached
		{
		if ( m_plng->GetTextWidth( pDC, m_sText, i ) + 4 > point.x ) // 1.4vxy 
			break;
		}
	if ( !bMultiLine ) // 1.4vxy 
		m_iCharPos = i; // 1.4vxy Make mouse click work in single line legacy edit controls
	Invalidate();
	SetFocus(); // 1.4qma Fix focus problem of new language edit control
}
