#ifndef __PROGBAR_H__
#define __PROGBAR_H__
                        
///////////////////////////////////////////////////////////////////////////////
//
// PROGBAR.H - Declaration of Progress Bar
// 
// Contains:    CProgressBar
//
// Notes:
//
///////////////////////////////////////////////////////////////////////////////

// forward declarations

// includes

///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar - Class which implements a progress bar
//
// Inheritance:    CStatic
//
// Public Members: None
//
// Notes:          The control that gets attached must be of type Static
//
///////////////////////////////////////////////////////////////////////////////
class CProgressBar : public CStatic
{
public:  
   CProgressBar();
                   
   BOOL Attach( UINT nID, CWnd* pwndParent, LONG nRange, COLORREF rgbText = CProgressBar::rgbBlue, COLORREF rgbBkg = CProgressBar::rgbWhite );

   WNDPROC* GetSuperWndProcAddr();
   
   void SetPosition( LONG nPos );
   inline void SetTextColor( COLORREF rgbText );
   inline void SetBkgColor( COLORREF rgbBkg );              
              
   static COLORREF rgbBlack;
   static COLORREF rgbWhite;
   static COLORREF rgbRed;
   static COLORREF rgbGreen;
   static COLORREF rgbBlue;
   static COLORREF rgbGray;
   static COLORREF rgbPurple;
   
protected:
   afx_msg void OnPaint();
   
   DECLARE_MESSAGE_MAP();
   
private:
   LONG     m_nRange;   // the relative range of the progress bar
   LONG     m_nPos;     // the current position
   COLORREF m_rgbText;  // the text and bar color
   COLORREF m_rgbBkg;   // the background color
};

///////////////////////////////////////////////////////////////////////////////
//
// PROGBAR.INL - Inline functions for the Progress Bar
// 
// Contains:      CProgressBar
//
// Notes:
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::SetTextColor - sets the text and bar color
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
inline void
CProgressBar::SetTextColor( COLORREF rgbText )
{
   m_rgbText = rgbText;
}


///////////////////////////////////////////////////////////////////////////////
//
// CProgressBar::SetBkgColor - sets the progress bar's background color
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
inline void
CProgressBar::SetBkgColor( COLORREF rgbBkg )
{                                            
   m_rgbBkg = rgbBkg;
}

// inlines
// #include "progbar.inl" // 1.0-ad Move contents of progbar.inl into progbar.h, delete progbar.inl file
                       
#endif // __PROGBAR_H__

