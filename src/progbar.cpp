///////////////////////////////////////////////////////////////////////////////
//
// SRVAPPPB.CPP - Implementation of Progress Bar
// 
// Contains:      CProgressBar
//
// Notes:         The control that gets attached must be of type Static
//
///////////////////////////////////////////////////////////////////////////////

// includes
//#include <afxwin.h>
#include "stdafx.h"
#include "toolbox.h"
#include "progbar.h"

// initialize static members
COLORREF CProgressBar::rgbBlack  = RGB(   0,   0,   0 );
COLORREF CProgressBar::rgbWhite  = RGB( 255, 255, 255 );
COLORREF CProgressBar::rgbRed    = RGB( 255,   0,   0 );
COLORREF CProgressBar::rgbGreen  = RGB(   0, 255,   0 );
COLORREF CProgressBar::rgbBlue   = RGB(   0,   0, 255 );
COLORREF CProgressBar::rgbGray   = RGB( 192, 192, 192 );
COLORREF CProgressBar::rgbPurple = RGB(  64,   0, 128 );


///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::CProgressBar - ctor
//
// Returns:       None
//
// Inheritance:   None
//
// Allocation:    None
//
// Exceptions:    None
//
// Notes:
//
///////////////////////////////////////////////////////////////////////////////
CProgressBar::CProgressBar()
   :  m_nRange( 100L ),
      m_nPos( 0L ),
      m_rgbText( rgbBlue ),
      m_rgbBkg( rgbWhite )
{
}


///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::Attach - attaches a static control to the class by
//                          subclassing the control
//
// Returns:       BOOL - TRUE if successful, FALSE otherwise
//
// Inheritance:   CStatic::SubclassDlgItem
//
// Allocation:    None
//
// Exceptions:    None
//
// Notes:         The control which gets attached must be of type Static.
//                This is easily done by creating a black frame in
//                AppStudio or any other resource editor.
//
///////////////////////////////////////////////////////////////////////////////
BOOL
CProgressBar::Attach( UINT nID, CWnd* pwndParent, LONG nRange, COLORREF rgbText, COLORREF rgbBkg )
{
   // subclass the control
   if( !CStatic::SubclassDlgItem( nID, pwndParent ) )
      return( FALSE );
                          
   // make sure the range is valid
   // ASSERT( nRange > 0L );    // RNE Range could be zero.
   if( nRange < 0L )
      nRange = 0L;
      
   // set the range
   m_nRange = nRange;                          
      
   // set the colors
   m_rgbText = rgbText;
   m_rgbBkg  = rgbBkg;
                    
   // invalidate it so it gets painted                          
   InvalidateRect( NULL );

   // bye!
   return( TRUE );
}  


WNDPROC* 
CProgressBar::GetSuperWndProcAddr()
{
   static WNDPROC NEAR pfnSuperWndProc;
   return( &pfnSuperWndProc );
}

// 1997-05-26 MRP: This function had formerly been in tools.cpp
static void RouteMessages()
{
    MSG msg;

#if 1
    while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg); 
        }
#else
    CWnd* pwnd = AfxGetMainWnd();
    while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        if (msg.hwnd != pwnd->GetSafeHwnd())
            {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            }
        }
#endif      
}

///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::SetPosition - repositions the progress bar
//
// Returns:       void
//
// Inheritance:   None
//
// Allocation:    None
//
// Exceptions:    None
//
// Notes:         Moves the progress bar along
//
///////////////////////////////////////////////////////////////////////////////
void
CProgressBar::SetPosition( LONG nPos )
{      
   // make sure we get a valid position
   ASSERT( (nPos >= 0L) && (nPos <= m_nRange) );
   
   // make sure we get a valid position
   if( nPos < 0L )
      nPos = 0L;
      
   if( nPos > m_nRange )
      nPos = m_nRange;

   // set the position
   m_nPos = nPos;

   // invalidate the rectange to repaint the bar
   // It may be better to explicitly send a paint message.
   InvalidateRect( NULL );
   UpdateWindow();
   // Yield();
   RouteMessages();
   // SendMessage(WM_PAINT);  // Won't work.  Will call manually.
   
   //OnPaint();
}


///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar - MESSAGE_MAP
//
///////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CProgressBar, CStatic)
   ON_WM_PAINT()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::OnPaint - paints the progress bar
//
// Returns:       void
//
// Inheritance:   None
//
// Allocation:    None
//
// Exceptions:    None
//
// Notes:
//
///////////////////////////////////////////////////////////////////////////////
void
CProgressBar::OnPaint()
{
   WORD        wOffset;
   CSize       sizeExtent;
   RECT        rc1, rc2;
   char        szPercent[ 6 ];
   WORD        dx, dy, wProgressX;
               
   // get the paint device contrext                 
   CPaintDC dc( this );
     
   // set the colors into for the gauge into the control
   dc.SetTextColor( m_rgbText );
   dc.SetBkColor( m_rgbBkg );
                 
   // draw black rectangle for gauge
   GetClientRect( &rc1 );

   // draw a black border on the outside
   CBrush brushBlack( RGB( 0, 0, 0 ) );
   dc.FrameRect( &rc1, &brushBlack );

   // we want to draw just inside the black border
   InflateRect( &rc1, -1, -1 );

   // one line thick so far...
   wOffset = 1;

   // dup--one rect for 'how much filled', one rc for 'how much empty'
   rc2 = rc1;

   // get the range--make sure it's a valid range 
   // ASSERT( m_nRange > 0L );
   // if( m_nRange <= 0L )  // We make sure to prevent divides-by-zero
   // m_nRange = 1L;

   // get the position--greater than 100% would be bad 
   ASSERT( (m_nPos >= 0L) && (m_nPos <= m_nRange) );
   if( m_nPos < 0L )
     m_nPos = 0L;
     
   if( m_nPos > m_nRange )
     m_nPos = m_nRange;
     
   // compute the actual size of the gauge
   dx = (WORD)(rc1.right - rc1.left); // AB 1-8-97 Cast to prevent warning
   dy = (WORD)(rc1.bottom - rc1.top);
   if (m_nRange > 0)
        wProgressX = (WORD)((DWORD)m_nPos * dx / m_nRange);
    else
        wProgressX = dx;
    

   // adjust the rects for the appropriate size
   rc1.right = rc2.left += wProgressX;

   // build up a string to blit out
   int iPercent = 100; // 1.4qut Upgrade wsprintf for Unicode build
   if (m_nRange > 0) // 1.4qut
	   iPercent = m_nPos * 100 / m_nRange; // 1.4qut
   _itoa_s(iPercent, szPercent, (int)sizeof(szPercent), 10); // 1.4qut Upgrade wsprintf for Unicode build

//   if (m_nRange > 0) // 1.4qut
//        wsprintf(szPercent, "%3d%%", (WORD)((DWORD)m_nPos * 100L / m_nRange)); // 1.4qut
//    else // 1.4qut
//        wsprintf(szPercent, "%3d%%", (WORD)100); // 1.4qut
   
	Str8 sText = szPercent; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
   // get the size of the string
   wProgressX = swText.GetLength(); // 1.4quu Upgrade GetTextExtent for Unicode build
   sizeExtent = dc.GetTextExtent( swText ); // 1.4quu

   //  Draw the finished (ie the percent done) side of box.
   dc.ExtTextOut( (dx - sizeExtent.cx) / 2 + wOffset,
                  (dy - sizeExtent.cy) / 2 + wOffset,
                  ETO_OPAQUE | ETO_CLIPPED, &rc2, swText, wProgressX, NULL); // 1.4qta

   //  Reverse fore and back colors for drawing the undone (ie the non-
   //  finished) side of the box.
   dc.SetBkColor( m_rgbText );     // reverse 'em
   dc.SetTextColor( m_rgbBkg );

   //  Draw the un-finished (ie the percent not-done) side of box.
   dc.ExtTextOut( (dx - sizeExtent.cx) / 2 + wOffset,
                  (dy - sizeExtent.cy) / 2 + wOffset,
                  ETO_OPAQUE | ETO_CLIPPED, &rc1, swText, wProgressX, NULL); // 1.4qta

   // bye!
   return;
}

