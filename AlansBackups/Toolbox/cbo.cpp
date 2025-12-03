// cbo.cpp  Implementation of a combo box representing a set (1995-06-19)

// Change history:
// 1995-06-19 0.15  MRP: Initial design sketch


#include "stdafx.h"
#include "toolbox.h"
#include "cbo.h"
#include "tools.h"
#ifdef OWNER_DRAW
// 1996-12-04 MRP: The reason for adding owner-draw support to combo boxes
// was to allow the same bold/regular distinction for referenced/unreferenced
// object instances. (However, there is no obvious way to align columns,
// since there won't be labels on the dialog box, like for the list boxes.)
// Brian Yoder discovered that owner-draw combo boxes don't get drawn
// correctly by the Macintosh compatibility layer.
// So we'll define the capability out until it works correctly and is needed.
#include "tools.h"  // ExtTextOut_AsOneLine
#endif  // OWNER_DRAW

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

// If bUseNoneItem is TRUE a [none] will appear as the first item
// in the combo box. A set element can be prevented from being in
// the list by passing it as pselDontUse.
CSetComboBox::CSetComboBox(CSet* pset, BOOL bUseNoneItem, CSetEl* pselDontUse, const char* pszNoneItem) // , unsigned int iNoneID) // 1.4gg Eliminate unused NoneID // 1.4gh Add string NoneItem
{
// 1-3-96 BJY Needed to pass in NULL pset
//  ASSERT( pset != NULL );
    m_pset = pset;
    m_bUseNoneItem = bUseNoneItem;
    m_pselDontUse = pselDontUse;
    if ( m_bUseNoneItem )
		{
		if ( pszNoneItem ) // 1.4kx Fix bug of showing [none] instead of [no filter] on toolbar
			m_sNoneItem = pszNoneItem; // 1.4kx
		else
	        m_sNoneItem = _("[none]");
		}
    m_bMyOwnNotification = FALSE;
}


BEGIN_MESSAGE_MAP(CSetComboBox, CComboBox)
    ON_WM_CHAR()
    ON_WM_KILLFOCUS( )
END_MESSAGE_MAP()

void CSetComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( (nChar < 32 || 255 < nChar) && nChar != VK_BACK )
        {
        m_sMatched.Empty();
        CComboBox::OnChar(nChar, nRepCnt, nFlags);
        return;
        }

    Str8 s = m_sMatched;     
    if ( nChar == VK_BACK )
        {
        if ( s.IsEmpty() )
            {
            MessageBeep(0);
            return;
            }
        else if ( s.GetLength() == 1 )
            {
            m_sMatched.Empty();
            SetCurSelAndNotifyParent(0);
            return;
            }
        s = s.Left(s.GetLength()-1);
        }
    else
        s += (char)nChar;
        
    ItemIndex itm = 0;
    if ( !m_sMatched.IsEmpty() && nChar != VK_BACK )
        itm = GetCurSel();
    else if ( m_bUseNoneItem )
        itm = 1;
    ASSERT( 0 <= itm );

    itm = FindString(itm-1, swUTF16( s ) ); // 1.4qtf
    if ( 0 <= itm )
        {
        m_sMatched = s;
        SetCurSelAndNotifyParent(itm);
        }
    else
        MessageBeep(0);
}

void CSetComboBox::OnKillFocus(CWnd* pwnd)
{
    m_sMatched.Empty();
    CComboBox::OnKillFocus(pwnd);   
}
        
BOOL CSetComboBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
    LRESULT* pLResult)
{
    if ( message == WM_COMMAND )
        {
        WORD wNotificationCode = HIWORD(wParam);
        if ( wNotificationCode == CBN_SELCHANGE && !m_bMyOwnNotification )
            m_sMatched.Empty();
        }
#ifdef OWNER_DRAW
    else if ( message == WM_DRAWITEM )
        {
        DrawItem((LPDRAWITEMSTRUCT)lParam);
        return TRUE;
        }
#endif  // OWNER_DRAW
        
    return FALSE;  // Let the parent handle the message
}

ItemIndex CSetComboBox::SetCurSelAndNotifyParent(ItemIndex itmToSelect)
{
    ItemIndex itmSelected = SetCurSel(itmToSelect);
    WORD idc = GetDlgCtrlID();
    ASSERT( idc );
    ASSERT( !m_bMyOwnNotification );
    m_bMyOwnNotification = TRUE;
    GetParent()->SendMessage( WM_COMMAND, (WPARAM)MAKELONG(idc, CBN_SELCHANGE), (LPARAM)m_hWnd );
    m_bMyOwnNotification = FALSE;

    return itmSelected;
}

    
void CSetComboBox::UpdateElements(CSetEl* pselToSelect)
{
    ResetContent();  // remove any items currently in the combo box

    if ( m_bUseNoneItem )
        {
        int i = InsertString(-1, swUTF16( m_sNoneItem ) );  // 1996-09-09 MRP // 1.4qre
        SetItemDataPtr(i, NULL);
        }

    // Add the names of the elements in the set to the combo box.
    CSetEl* psel = m_pset->pselFirst();
    for ( ; psel != NULL; psel = m_pset->pselNext(psel) )
        {
        if ( psel != m_pselDontUse && bIncludeElement(psel) )
            {
            // const Str8& sItem = psel->sName();
            const Str8& sItem = psel->sItem();  // 1996-12-04 MRP
            ItemIndex itmAdded = InsertString(-1, swUTF16( sItem ) ); // 1.4qre
            (void) SetItemDataPtr(itmAdded, (void*)psel);
            }
        }
    
    SelectElement(pselToSelect);
}

void CSetComboBox::ChangeSet(CSet* pset, CSetEl* pselToSelect, CSetEl* pselDontUse)
{
    ASSERT( pset != NULL );
    m_pset = pset;
    m_pselDontUse = pselDontUse;
    UpdateElements(pselToSelect);
}

void CSetComboBox::SelectElement(CSetEl* pselToSelect)
{
    if ( pselToSelect != NULL )
        {   
        // Select the item corresponding to the element
        ASSERT( GetCount() != 0 );
        for ( int i = 0; i < GetCount(); i++ )
            if ( pselToSelect == (CSetEl*)GetItemDataPtr( i ) )
                {
                SetCurSel( i );
                return;
                }
        SetCurSel( 0 );
        }
    else if ( m_bUseNoneItem )
        SetCurSel(0); // should be first item on list
    else
        // Clear the current selection
        SetCurSel(-1);
}

// note that this will return NULL if either [None] or nothing is selected
CSetEl* CSetComboBox::pselSelected()
{
    ItemIndex itmSelected = GetCurSel();
    CSetEl* psel = ( itmSelected != CB_ERR ?
        (CSetEl*)GetItemDataPtr(itmSelected) :
        NULL  // either the combo box is empty, or no item is selected
        );
        
    return psel;
}

BOOL CSetComboBox::bIncludeElement(const CSetEl* psel) const
{
    ASSERT( psel );
    return TRUE;
}


#ifdef OWNER_DRAW
void CSetComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    COLORREF crBack;
    COLORREF crText;
    if ((lpDrawItemStruct->itemState) & ODS_SELECTED)
        {
        // Highlight if selected.
        crBack = GetSysColor(COLOR_HIGHLIGHT);
        crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
        }
    else
        {
        crBack = GetSysColor(COLOR_WINDOW);
        crText = GetSysColor(COLOR_WINDOWTEXT);
        }

    CDC cDC;  // context with which to draw the item
    cDC.Attach(lpDrawItemStruct->hDC);
#ifdef _MAC
    // the following fixes a bug in the Windows Portability Library that causes a listbox
    //  item that is scrolled just out of the visible list to be painted outside the listbox
    //  on the dialog's background.  Sometimes the clipping rectangle comes thru set to what
    //  looks like the owning dialog's coords instead of the listbox window.
    // Don't ask me why we even get called if we're not visible, but we do!
    if ((lpDrawItemStruct->itemState) & ODS_SELECTED)
        {
        CRect rectClient;
        GetClientRect(&rectClient);
        CRgn rgn;
        rgn.CreateRectRgnIndirect( &rectClient );
        cDC.SelectClipRgn( &rgn ); // force clipping region to be size of listbox window
        }
#endif
    const RECT& rcItem = lpDrawItemStruct->rcItem;  // rectangle containing it
    CBrush  brsSelected;
    brsSelected.CreateSolidBrush(crBack);
    cDC.FillRect(&rcItem, &brsSelected);        
    cDC.SetBkColor(crBack);
    cDC.SetTextColor(crText);

    if ( lpDrawItemStruct->itemID != -1 ) // happens when a listbox is empty - used to draw empty focus rect
        {
        CSetEl* psel = (CSetEl*)lpDrawItemStruct->itemData;  // the element
        //ASSERT( psel != NULL );  // 1996-09-09 MRP: None item is NULL

        TEXTMETRIC tm;
        cDC.GetTextMetrics( &tm );
        int iWeight = psel && bIsBold( psel ) ? FW_BOLD : FW_NORMAL; // determine desired font weight
        if ( iWeight == tm.tmWeight ) // is default font what we want?
            {
            DrawElement(cDC, rcItem, psel);  // implemented in the derived class
            }
        else // need to change weight of font
            {
            LOGFONT lf;
            memset( &lf, 0, sizeof( lf ) ); // clear unused fields
#ifdef _MAC
            lf.lfHeight = tm.tmAscent; // I can't make much sense out of this, but that's a Mac for ya
#else
            lf.lfHeight = tm.tmHeight;
#endif
            lf.lfWeight = iWeight;
            lf.lfCharSet = tm.tmCharSet;
            lf.lfPitchAndFamily = tm.tmPitchAndFamily;
            cDC.GetTextFace( sizeof( lf.lfFaceName ), lf.lfFaceName ); // get name of default font

            CFont fnt;
            fnt.CreateFontIndirect( &lf ); // try to make a copy of the default font with only the weight changed
            CFont* pfnt = cDC.SelectObject( &fnt );
#ifdef _DEBUG
            TEXTMETRIC tmd;
            cDC.GetTextMetrics(&tmd);
#endif
            DrawElement(cDC, rcItem, psel);  // implemented in the derived class
            cDC.SelectObject( pfnt ); // restore default font
            }
        }
#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif
    cDC.Detach();
}

BOOL CSetComboBox::bIsBold(CSetEl* psel)
{
    if ( !m_bUseNoneItem )
        ASSERT( psel );
    return psel && psel->bHasRefs();
}

void CSetComboBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CSetEl *psel)
{
    const char* pszName = (psel ? psel->sName() : m_sNoneItem);
    DrawSubItem(cDC, rcItem, 0, 0, pszName);
}

void CSetComboBox::DrawSubItem(CDC& cDC, const RECT& rcItem,
        int xCur, int xNext, const char* pszSubItem)
{
    RECT rcSubItem = rcItem;  // rectangle containing entire list box item
    rcSubItem.left = xCur;  // offset to left edge of current subitem's text
    if ( xNext != 0 )
        rcSubItem.right = xNext - 5;  // clip just to left of next subitem
        
//  cDC.ExtTextOut(rcSubItem.left, rcSubItem.top, ETO_CLIPPED,
//      &rcSubItem, pszSubItem, strlen(pszSubItem), NULL);
        // RNE note: ExtTextOut is used to write text within a clip rectangle.
    ExtTextOut_AsOneLine(&cDC, rcSubItem.left, rcSubItem.top,
        ETO_CLIPPED, &rcSubItem, pszSubItem, strlen(pszSubItem), NULL);
}
#endif  // OWNER_DRAW

// **************************************************************************
