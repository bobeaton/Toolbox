// lbo.cpp  Implementation of a list box control representing a list (1995-06-19)


#include "stdafx.h"
#include "toolbox.h"
#include "lbo.h"
#include "resource.h"  // IDC_SET_NAME
#include "lng.h"  // CLangEnc::iEncode
#include "mkr.h"  // CMarker::pfntDlg
#include "tools.h"  // pszNextLineBrk
#include "font.h" // CDCFontRightSizer

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CDblListListBox::CDblListListBox(CDblList* plst)
{
    m_plst = plst;
    ASSERT( m_plst );
    m_iItemHeight = 0;
}
    
void CDblListListBox::UpdateElements(const CDblListEl* pelToSelect)
{
    ResetContent();  // remove any items currently in the list box

    // Insert the names of the elements in the list in the list box.
    CDblListEl* pel = m_plst->pelFirst();
    for ( ; pel != NULL; pel = m_plst->pelNext(pel) )
        {
        // Insert the item at the end of the list box.
        ItemIndex itmInserted = InsertString(-1, swUTF16( "" ) ); // 1.4qre
            // Unlike AddString(), InsertString() does not cause
            // a list with the LBS_SORT style to be sorted.
        ASSERT( itmInserted != LB_ERR );
        (void) SetItemDataPtr(itmInserted, (void*)pel);
        if ( pel == pelToSelect )
            SetCurSel(itmInserted);
        }
}

CDblListEl* CDblListListBox::pelSelected()
{
    ItemIndex itmSelected = GetCurSel();
    CDblListEl* pel = ( itmSelected != LB_ERR ?
        (CDblListEl*)GetItemDataPtr(itmSelected) :
        NULL  // list box is empty
        );
        
    return pel;
}


void CDblListListBox::SetItemHeight(int iItemHeight)
{
    ASSERT( m_iItemHeight == 0 );
    m_iItemHeight = iItemHeight;
    if ( m_iItemHeight != 0 )
        {
        ASSERT( 0 < m_iItemHeight );
        CListBox::SetItemHeight(0, m_iItemHeight);
        }
}

void CDblListListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
        CDblListEl* pel = (CDblListEl*)lpDrawItemStruct->itemData;  // the element
        ASSERT( pel != NULL );
        DrawElement(cDC, rcItem, pel);  // implemented in the derived class
        }
#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif
    cDC.Detach();
}


// --------------------------------------------------------------------------

void ExtTextOut_AsOneLine(CDC* pDC, int* pxLeft, int y,
        UINT nOptions, LPCRECT prect, LPCSTR psz, UINT len, 
		LPINT pDxWidths, CLangEnc* plng)
    // Write to device context pDC at logical coordinates (xLeft,y)
    // according to nOptions within rectangular region pRect
    // the first len characters of psz.
    // This is similar to the standard Windows ExtTextOut API function,
    // except that it writes pszString on exactly one line;
    // in the place of any newline, it writes a space instead.
    // NOTE: Unlike ExtTextOut, this function forbids a NULL prect,
    // and the logic assumes the string is to be clipped to the rectangle.
    // It is used in owner-draw list boxes
{
//    static const char s_pszSpace[] = " "; // 1.4qtd 
    ASSERT( pDC );
    // NOTE: Logic below assumes the text is to be clipped to a rectangle.
    ASSERT( nOptions & ETO_CLIPPED );
    ASSERT( prect );
//    RECT rectRemaining = *prect;
    CRect rectRemaining = *prect;
    const char* pszLineBegin = psz;
    ASSERT( pszLineBegin );
    UINT lenRemaining = len;
    ASSERT( pxLeft );
    int xLeft = *pxLeft;
    while ( TRUE )
        {
        // NOTE: This logic assumes that the string is null-terminated,
        // even when less than its entirety is to be written.
        const char* pszLineEnd = pszNextLineBrk(pszLineBegin);
        UINT lenLine = pszLineEnd - pszLineBegin;
        if ( lenRemaining < lenLine )
            lenLine = lenRemaining;
		UINT lenContext = 0; // c not available from above
		if ( plng )
			{
			plng->ExtTextOutLng(pDC, xLeft, y, nOptions, &rectRemaining, pszLineBegin, lenLine, lenContext);
			xLeft += plng->GetTextExtent(pDC, pszLineBegin, lenLine).cx; // 1.4qtd Upgrade GetTextExtent for Unicode build
			}
		else
			{
			Str8 sText = pszLineBegin; // 1.4qta
			sText = sText.Left( lenLine ); // 1.4qzfe Fix problem (1.4qta) of displaying too much
			CString swText = swUTF16( sText ); // 1.4qta
			pDC->ExtTextOut( xLeft, y, nOptions, &rectRemaining, swText, swText.GetLength(), pDxWidths); // 1.4qta
			xLeft += pDC->GetTextExtent( swText, swText.GetLength() ).cx; // 1.4qtd
			}
        // 1996-06-05 MRP: I first tried using SetTextAlign with TA_UPDATECP,
        // but calling GetTextExtent explicitly seems better.
        lenRemaining -= lenLine;

        if ( !*pszLineEnd || lenRemaining == 0)  // If it's all been written
            break;

        // Output a space in place of the newline, and then move past it.
        // Prevent ETO_OPAQUE from erasing previously written lines.
        // 1996-06-05 MRP: Review this logic when we support RTL scripts
        if ( xLeft < rectRemaining.left )
            ;  
        else if ( xLeft <= rectRemaining.right )
            rectRemaining.left = xLeft;
        else  // If outside the clipping rectangle
            break;
		Str8 sText = " "; // 1.4qta // 1.4qtd Use explicit space instead of static const
		CString swText = swUTF16( sText ); // 1.4qta
        pDC->ExtTextOut( xLeft, y, nOptions, &rectRemaining, swText, swText.GetLength(), pDxWidths); // 1.4qta // 1.4qzfe
        xLeft += pDC->GetTextExtent( swText, swText.GetLength() ).cx; // 1.4qtd
        if ( xLeft < rectRemaining.left )
            ;  
        else if ( xLeft <= rectRemaining.right )
            rectRemaining.left = xLeft;
        else  // If outside the clipping rectangle
            break;
        pszLineBegin = pszLineEnd + 1;  // Beginning of the next line
        }       
        
    *pxLeft = xLeft;
}

void ExtTextOut_AsOneLine(CDC* pDC, int xLeft, int y,
        UINT nOptions, LPCRECT prect, LPCSTR psz, UINT len, 
		LPINT pDxWidths, CLangEnc* plng)
    // Write substring [psz,psz+len), then increment *pxLeft by its text extent.
{
    ExtTextOut_AsOneLine(pDC, &xLeft, y, nOptions, prect, 
		psz, len, pDxWidths, plng);
}


void CDblListListBox::DrawSubItem(CDC& cDC, const RECT& rcItem,
        int xCur, int xNext, const char* pszSubItem)
{
    RECT rcSubItem = rcItem;  // rectangle containing entire list box item
    rcSubItem.left = xCur;  // offset to left edge of current subitem's text
    if ( xNext != 0 )
        rcSubItem.right = xNext - 5;  // clip just to left of next subitem
        
    ASSERT( m_iItemHeight == 0 ); // This means that height is system-defined
#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)
    CDCFontRightSizer frs(&cDC);
    TEXTMETRIC tm;
    frs.SelectRightFont(GetItemHeight(0), tm);
#endif
    ExtTextOut_AsOneLine(&cDC, rcSubItem.left, rcSubItem.top,
        ETO_CLIPPED, &rcSubItem, pszSubItem, strlen(pszSubItem), NULL, NULL);
}


void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int xLeft, int xRight, const char* pszSubItem, CLangEnc* plng)
{
    CFont* pfnt = NULL;
    if ( plng )
        pfnt = (CFont*)plng->pfntDlg();  // 1999-06-23 MRP
    DrawSubItemJustify(cDC, rcItem, &xLeft, xRight, pszSubItem, pfnt,
        plng, TRUE);
}

void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int* pxLeft, int xRight, const char* pszSubItem, CLangEnc* plng)
{
    // 1998-03-03 MRP: Used for drawing filters element-by-element:
    // the dominant text direction is left-to-right, so don't right align.
    CFont* pfnt = NULL;
    if ( plng )
        pfnt = (CFont*)plng->pfntDlg();  // 1999-06-23 MRP
    DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, pszSubItem, pfnt,
        plng, FALSE);
}

void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int xLeft, int xRight, const char* pszSubItem, CMarker* pmkr)
{
    ASSERT( pmkr );
    CFont* pfnt = NULL;
    if ( pmkr )
        pfnt = (CFont*)pmkr->pfntDlg();
    CLangEnc* plng = pmkr->plng();
    DrawSubItemJustify(cDC, rcItem, &xLeft, xRight, pszSubItem, pfnt,
        plng, TRUE);
}

void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int* pxLeft, int xRight, const char* pszSubItem, CMarker* pmkr)
{
    // 1998-03-03 MRP: Used for drawing filters element-by-element:
    // the dominant text direction is left-to-right, so don't right align.
    ASSERT( pmkr );
    CFont* pfnt = NULL;
    if ( pmkr )
        pfnt = (CFont*)pmkr->pfntDlg();
    CLangEnc* plng = pmkr->plng();
    DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, pszSubItem, pfnt,
        plng, FALSE);
}

void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int xLeft, int xRight, const char* pszSubItem, CFont* pfnt,
        CLangEnc* plng)
{
    DrawSubItemJustify(cDC, rcItem, &xLeft, xRight, pszSubItem, pfnt,
        plng, TRUE);
}

void CDblListListBox::DrawSubItemJustify(CDC& cDC, const RECT& rcItem,
        int* pxLeft, int xRight, const char* pszSubItem, CFont* pfnt,
        CLangEnc* plng, BOOL bAlignIfRightToLeft)
{
    RECT rcSubItem = rcItem;  // rectangle containing entire list box item
    ASSERT( pxLeft );
    rcSubItem.left = *pxLeft;  // offset to left edge of current subitem's text
    if ( xRight != 0 )
        rcSubItem.right = xRight - 5;  // clip just to left of next subitem
    if ( rcSubItem.right < rcSubItem.left )
        return;  // 1998-03-03 MRP: ClipAlignedSubstring requires left <= right

#ifdef _DEBUG // TLB 07-18-2000 : Don't ASSERT this in production - could crash Shoebox
    ASSERT( m_iItemHeight != 0 );
#endif

    CFont* pfntOriginal = NULL;
    if ( pfnt )  // 1998-02-13 MRP
        pfntOriginal = cDC.SelectObject( pfnt );  // 1999-06-23 MRP

    TEXTMETRIC tm;
    // 2000-07-12 MRP: Comment out the following two assertions,
    // because they cause Shoebox to fail and lose data.
    // We still don't know why the font height assertion has failed
    // for Grimes in Multiple Matches and for Kopris on the Filters tab
    // and for Prof. Rennison in the Multiple Matches dialog.
    // 2000-07-18 TLB: We think it might be the system font when the
    // font size is greater than 96 dpi.
#if defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) || defined(_MAC)
//    BOOL bGotTextMetrics = cDC.GetTextMetrics( &tm );
#ifdef _DEBUG
    ASSERT(bGotTextMetrics);
#endif
#else
    CDCFontRightSizer frs(&cDC, pfntOriginal);
    frs.SelectRightFont(m_iItemHeight, tm);
#endif // not TLB_07_18_2000_FONT_HANDLING_BROKEN or _MAC

    int iFntHt = tm.tmHeight;
    // position items so they align vertically at baseline of normal 16 pt font (whatever that is)
    int iBoxHt = rcItem.bottom - rcItem.top;
    int iTop;
    if ( iFntHt < m_iItemHeight )
        {
        iTop = rcItem.top + iBoxHt - iFntHt - ( m_iItemHeight/4 - tm.tmDescent ); // figure baseline at 1/4 of total height
        if ( iTop < rcItem.top )
            iTop = rcItem.top; // this seems unlikely
        }
    else
        iTop = rcItem.top;
        
    const char* psz = pszSubItem;
    int len = strlen(psz);  // 1998-02-20 MRP
    if ( plng )  // 1998-02-13 MRP
        {
        if ( plng->bRightToLeft() )  // 1998-02-20 MRP
            {
            int ixTextWidth = rcSubItem.right - rcSubItem.left;
            plng->bClipAlignedSubstring(&cDC, TRUE, &psz, &len, &ixTextWidth);
            if ( bAlignIfRightToLeft )
                *pxLeft = rcSubItem.right - ixTextWidth;  // 1998-03-03 MRP
            }
        }
    ExtTextOut_AsOneLine(&cDC, pxLeft, iTop,
        ETO_CLIPPED, &rcSubItem, psz, len, NULL, plng);

#if defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) || defined(_MAC)
//    if ( pfntOriginal )
//        cDC.SelectObject( pfntOriginal ); // restore original font
#endif // For windows, the restoration of the original font is now handled in CDCFontRightSizer destructor

}  // CDblListListBox::DrawSubItemJustify

int CDblListListBox::xSubItem_Show(ID idSubItemLabel)
{
	if ( !idSubItemLabel ) // Default value of 4 if no label
		return 4;

    RECT rcListBox;
    GetWindowRect(&rcListBox);
    
    CWnd* pwndDialog = GetParent();  // containing both list box and label
    ASSERT( pwndDialog != NULL );
    CStatic* plbl = (CStatic*)pwndDialog->GetDlgItem(idSubItemLabel);
    ASSERT( plbl != NULL );
    
    plbl->ShowWindow(SW_SHOWNOACTIVATE);    

    RECT rcLabel;
    plbl->GetWindowRect(&rcLabel);

    ASSERT( rcListBox.left < rcLabel.left );
    ASSERT( rcLabel.left < rcListBox.right );   
    int x = rcLabel.left - rcListBox.left;
    
    return x;
}


// --------------------------------------------------------------------------
// Use this version for a listbox that sets the selection to the specified element
// and passes the selected element back to the caller upon close
CSetListBox::CSetListBox(CSet* pset, CSetEl** ppsel) : CDblListListBox(pset)
{
    ASSERT( pset != NULL );
    ASSERT( ppsel != NULL );
    m_pset = pset;
    m_ppsel = ppsel;
    m_psel = ( *m_ppsel != NULL ?
        *m_ppsel :
        m_pset->pselFirst()  // defaults to the first; or NULL, if empty
        );
    m_bMyOwnNotification = FALSE;
}

// Use this version for a listbox that selects the first element initially and
// doesn't tell the caller what was selected upon close
CSetListBox::CSetListBox(CSet* pset) : CDblListListBox(pset),
    m_ppsel(NULL)
{
    ASSERT( pset != NULL );
    m_pset = pset;
    m_psel = m_pset->pselFirst();  // defaults to the first; or NULL, if empty
    m_bMyOwnNotification = FALSE;
}

BEGIN_MESSAGE_MAP(CSetListBox, CDblListListBox)
    ON_WM_CHAR()
    ON_WM_KILLFOCUS( )
END_MESSAGE_MAP()

void CSetListBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( (nChar < 32 || 255 < nChar) && nChar != VK_BACK )
        {
        m_sMatched.Empty();
        CListBox::OnChar(nChar, nRepCnt, nFlags);
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
    ASSERT( 0 <= itm );

    itm = FindString(itm-1, swUTF16( s ) ); // 1.4qte Upgrade a FindString for Unicode build
    if ( 0 <= itm )
        {
        m_sMatched = s;
        SetCurSelAndNotifyParent(itm);
        }
    else
        MessageBeep(0);
}

void CSetListBox::OnKillFocus(CWnd* pwnd)
{
    m_sMatched.Empty();
    CListBox::OnKillFocus(pwnd);    
}

        
BOOL CSetListBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
    LRESULT* pLResult)
{
    if ( message == WM_COMMAND )
        {
        WORD wNotificationCode = HIWORD(wParam);
        if ( wNotificationCode == LBN_SELCHANGE && !m_bMyOwnNotification )
            m_sMatched.Empty();
        }
    else if ( message == WM_DRAWITEM )
        {
        DrawItem((LPDRAWITEMSTRUCT)lParam);
        return TRUE;
        }
    // 1999-09-16 MRP: Used for owner-draw variable-height list boxes
    else if ( message == WM_MEASUREITEM )
        {
        MeasureItem((LPMEASUREITEMSTRUCT)lParam);
        return TRUE;
        }
        
    return FALSE;  // Let the parent handle the message
}

ItemIndex CSetListBox::SetCurSelAndNotifyParent(ItemIndex itmToSelect)
{
    ItemIndex itmSelected = SetCurSel(itmToSelect);
    WORD idc = GetDlgCtrlID();
    ASSERT( idc );
    m_bMyOwnNotification = TRUE;
    GetParent()->SendMessage( WM_COMMAND, (WPARAM)MAKELONG(idc, CBN_SELCHANGE), (LPARAM)m_hWnd );
    m_bMyOwnNotification = FALSE;

    return itmSelected;
}


void CSetListBox::InitListBox()
{
    InitLabels();
    SetItemHeight(iItemHeight());
    UpdateElements(m_psel);
}

void CSetListBox::Close()
{
    // Return the final selection in the list box
    if ( m_ppsel )
        *m_ppsel = pselSelected();
}
    
void CSetListBox::UpdateElements(const CSetEl* pselToSelect)
{
    ResetContent();  // remove any items currently in the list box

    // Insert the names of the elements in the list in the list box.
    CSetEl* psel = m_pset->pselFirst();
    for ( ; psel != NULL; psel = m_pset->pselNext(psel) )
        {
        // Insert the item at the end of the list box.
        ItemIndex itmInserted = InsertString(-1, swUTF16( psel->sName() ) ); // 1.4qre
            // Unlike AddString(), InsertString() does not cause
            // a list with the LBS_SORT style to be sorted.
        ASSERT( itmInserted != LB_ERR );
        (void) SetItemDataPtr(itmInserted, (void*)psel);
        if ( psel == pselToSelect )
            SetCurSel(itmInserted);
        }
}

void CSetListBox::ChangeSet(CSet* pset, CSetEl* pselToSelect)
{
    ASSERT( pset != NULL );
    m_pset = pset;
    UpdateElements(pselToSelect);
}

BOOL CSetListBox::bAdd()
{
    CSetEl* pselNew = NULL;
    BOOL bOK = m_pset->bAdd(&pselNew);
    if ( bOK )
        UpdateElements(pselNew);
        
    return bOK;
}

BOOL CSetListBox::bCopy()
{
    CSetEl* pselNew = NULL;
    CSetEl* psel = pselSelected();
    ASSERT( psel );
    if ( psel->bCopy(&pselNew) )
        {
        ASSERT( pselNew );
        UpdateElements(pselNew);
        return TRUE;
        }
        
    return FALSE;
}


// 1997-12-15 MRP: Emergency correction for problem with merging markers
// via Modify from the Markers tab in Database Type Properties.
// See lbo.cpp, mkr_d.cpp, mkrset_d.h, mkrset_d.cpp

// Merging a marker is an exception to the ordinary logic for Modify:
// 1. The selected marker gets deleted
// 2. When the Marker Properties dialog box disappears,
//    the list box of markers gets redrawn immediately
//    in its old state. If the memory that the deleted marker's
//    object occuppied has be reused, this will fail.

// The immediate remedy uses the approach used in export_d.cpp
// to handle deletions from the Export Processes dialog box.
// Clear the contents of the list box, so there's nothing to redraw
// and select the next marker if the selected one was merged.

// After version 4.0, we need to make this option an explicit part
// of the interface for list boxes and eliminate the global variables.

CSetListBox* g_plboMarkers = NULL;
CSetEl* g_pselMarkerToBeSelected = NULL;

BOOL CSetListBox::bModify()
{
    CSetEl* psel = pselSelected();
    ASSERT( psel != NULL );

    if ( this == g_plboMarkers )
        {
        g_pselMarkerToBeSelected = psel;
        ResetContent();
        }

    BOOL bOK = psel->bModify();

    if ( this == g_plboMarkers )
        {
        UpdateElements(g_pselMarkerToBeSelected);
        g_pselMarkerToBeSelected = NULL;
        return bOK;
        }

    if ( bOK )
        UpdateElements(psel);
    else
        Invalidate(); // redraw in case an item changes from bold to regular font or whatever

    return bOK;
}

BOOL CSetListBox::bDelete()
{
    CSetEl* psel = pselSelected();
    ASSERT( psel != NULL );
    ASSERT( psel->bDeletable() );

    return bDelete(psel);
}

BOOL CSetListBox::bDelete(CSetEl* psel)
{
    ASSERT( psel != NULL );
    m_pset->Delete(&psel);  // delete the element from the set
    UpdateElements(psel);  // select the next, previous, or none
    
    return TRUE;
}



void CSetListBox::InitLabels()
{
    m_xName = xSubItem_Show(IDC_SET_NAME);
}

void CSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CSetEl* psel = (CSetEl*)pel;
    DrawSubItem(cDC, rcItem, m_xName, 0, psel->sName());
}

void CSetListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
        ASSERT( psel != NULL );

        TEXTMETRIC tm;
        cDC.GetTextMetrics( &tm );
        int iWeight = bIsBold( psel ) ? FW_BOLD : FW_NORMAL; // determine desired font weight
        if ( iWeight == tm.tmWeight ) // is default font what we want?
            {
            DrawElement(cDC, rcItem, (CDblListEl*)psel);  // implemented in the derived class
            }
        else // need to change weight of font
            {
            LOGFONT lf;
            memset( &lf, 0, sizeof( lf ) ); // clear unused fields
#ifdef _MAC
            // Note (TLB 07-18-2000): To clarify this code, it appears that the Mac creates a font based on
            // the height of the ascender (which seems to correspond to point size, when internal leading
            // is included).
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
            DrawElement(cDC, rcItem, (CDblListEl*)psel);  // implemented in the derived class
            cDC.SelectObject( pfnt ); // restore default font
            }
        }
#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif
    cDC.Detach();
}

BOOL CSetListBox::bIsBold(CSetEl* psel)
{
    ASSERT( psel );
    return psel->bHasRefs();
}

// **************************************************************************
