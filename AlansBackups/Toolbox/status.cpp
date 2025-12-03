// status.cpp  Implementation of a status bar derived from CStatusBar (1995-10-02)

// Change history:
// 1995-06-19 0.15  RNE: Initial

#include "stdafx.h"
#include "toolbox.h"
#include "status.h"
#include "shwdoc.h"
#include "shwview.h"
#include "project.h"

#include "shw.h"

#include "mainfrm.h"
#include "font.h"

// These constants are copied from mfc\src\auxdata.h
#define CX_BORDER   1
#define CY_BORDER   1

// 2000-09-19 TLB : Moved here from resource.h
#define STATUS_WIDTH_COUNT              40
#define STATUS_WIDTH_PROJ               3000 // 1.4wj Widen status bar pane that shows project file name
#define STATUS_WIDTH_KEY                175 // 1.2bk Widen status bar pane that shows key
#define STATUS_WIDTH_MESSAGE            150

CShwStatusBar::CShwStatusBar() :
    m_bShowingProgress(FALSE)
{
    m_rgbText = RGB( 255, 255, 255 );
    m_rgbBkg =  RGB( 0, 0, 255 );

    m_lCurrentRec = -1L;  // 1998-01-26 MRP: Initialize these members
    m_lTotalRecs = -1L;
    m_hfntRecord = NULL;
    m_hfntPriKey = NULL;
    
    m_plngRecFld = NULL;   //09-15-1997
    m_plngPriKeyFld = NULL;   //09-15-1997
}


CShwStatusBar::~CShwStatusBar()
{
}

BOOL CShwStatusBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{                        
    if (!CStatusBar::Create(pParentWnd, dwStyle, nID))
        return FALSE;

    SetFontAndSize(FALSE); // set size of status bar    

    return TRUE;
}

void CShwStatusBar::SetFontAndSize( BOOL bRelayOut )
{
    int iYPad = 0;
    // Calculate/set the height of the status bar.
    CFontDef fntdef("Times New Roman", 0);
    if ( Shw_papp()->bProjectOpen() )
        fntdef.plf()->lfHeight = Shw_pProject()->iLangCtrlHeight()+iYPad; // force height to size we want
    else
        fntdef.plf()->lfHeight = CProject::eNormalLangCtrl+iYPad; // force height to size we want

#ifdef _MAC
    fntdef.iCreateFontMakeFit( &m_fntSize ); // create a font
#else
    fntdef.iCreateFont( &m_fntSize ); // create a font
#endif
    
#ifdef _MAC
    CFontDef fntdefS("Helvetica", -12); // on Mac point size == height in pixels
#else
    CFontDef fntdefS("MS Sans Serif", -15 ); // use reasonable font
#endif
    fntdefS.iCreateFont( &m_fntNormal );

    SetFont( &m_fntSize ); // status bar sizes dynamically to fit the font

    if ( bRelayOut )
        Shw_pmainframe()->RecalcLayout(); // make dynamic sizing show   
}

static UINT BASED_CODE ind_normal[] =
{
    ID_SEPARATOR,           // message line     ID_STATUS_MSGLINE
    ID_SEPARATOR,           // record key       ID_STATUS_RECORD_KEY
    ID_SEPARATOR,           // primary key      ID_STATUS_PRIMARY_KEY
    ID_SEPARATOR,           // record count     ID_STATUS_RECORD_POSITION
    ID_SEPARATOR,           // Current project  ID_STATUS_PROJECT
};

static UINT BASED_CODE ind_progress[] =
{
    ID_SEPARATOR,           // progress bar     ID_STATUS_PROGRESSBAR
    ID_SEPARATOR,           // message line     ID_STATUS_PROGRESSMSG
};

BOOL CShwStatusBar::UseProgressIndicators() // setup status bar for progress indicator
{
    if ( !SetIndicators(ind_progress, sizeof(ind_progress)/sizeof(UINT)) )
        return FALSE;

    SetPaneInfo(ID_STATUS_PROGRESSBAR, ID_SEPARATOR, SBT_OWNERDRAW, STATUS_WIDTH_MESSAGE);
    SetPaneInfo(ID_STATUS_PROGRESSMSG, ID_SEPARATOR, SBPS_STRETCH | SBT_OWNERDRAW, 1);

    // I'm not sure why this is necessary! BJY 5-16-96
    SendMessage(SB_SETTEXT, ID_STATUS_PROGRESSBAR | SBT_OWNERDRAW );
    SendMessage(SB_SETTEXT, ID_STATUS_PROGRESSMSG | SBPS_STRETCH | SBT_OWNERDRAW );

    return TRUE;
}

BOOL CShwStatusBar::UseNormalIndicators() // setup normal status bar panes
{
    if ( !SetIndicators(ind_normal, sizeof(ind_normal)/sizeof(UINT)) )
        return FALSE;

    SetPaneInfo(ID_STATUS_MSGLINE, ID_SEPARATOR, SBPS_STRETCH | SBT_OWNERDRAW, STATUS_WIDTH_MESSAGE);
    SetPaneInfo(ID_STATUS_RECORD_KEY, ID_SEPARATOR, SBT_OWNERDRAW, STATUS_WIDTH_KEY);
    SetPaneInfo(ID_STATUS_PRIMARY_KEY, ID_SEPARATOR, SBT_OWNERDRAW, STATUS_WIDTH_KEY);
    SetPaneInfo(ID_STATUS_PROJECT, ID_SEPARATOR, SBT_OWNERDRAW, STATUS_WIDTH_PROJ);
    
    // Set the width of the position/count field.
    CDC *pDC = GetDC();
    pDC->SelectObject( &m_fntNormal );
	Str8 s = "99999/99999"; // 1.4qwn
	CString sw = swUTF16( s ); // 1.4qwn
    CSize sizTxt = pDC->GetTextExtent(sw, 11); // 1.4qwn

    SetPaneInfo(ID_STATUS_RECORD_POSITION, ID_SEPARATOR, SBT_OWNERDRAW, sizTxt.cx);
    ReleaseDC( pDC );

    // Set the text of the first window.
    SetStatusMessage("");

    return TRUE;
}

int CShwStatusBar::iPanes() const // return number of panes
{
    return m_bShowingProgress ? sizeof(ind_progress)/sizeof(UINT)
                              : sizeof(ind_normal)/sizeof(UINT);
}

void CShwStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDIS) // owner draw paint function
{
    CDC cDC;  // context with which to draw the item
    cDC.Attach(lpDIS->hDC);
    CRect rect = lpDIS->rcItem;
    DrawPane( &cDC, lpDIS->itemID, rect ); // call actual drawing routine
    cDC.Detach();
}

void CShwStatusBar::DrawPane( CDC* pDC, int iPane, CRect& rect ) // owner draw paint function
{
    pDC->SelectObject( &m_fntNormal ); // use standard status bar font
    pDC->SetTextColor( ::GetSysColor(COLOR_BTNTEXT) );
    pDC->SetBkColor( ::GetSysColor(COLOR_BTNFACE) );

    if ( m_bShowingProgress )
        {
        switch (iPane)
            {
            case ID_STATUS_PROGRESSBAR:
                {
                if (m_bShowPercent)
                    PaintProgress( pDC, rect ); // show progress bar
                break;
                }
            case ID_STATUS_PROGRESSMSG: // when progress bar is being used
                {
                DrawMessage( pDC, rect ); // show message prompt
                break;
                }
            default:
                ASSERT(FALSE);
            }
        }
    else // not status bar, normal panes are in use
        {
        switch (iPane)
            {
            case ID_STATUS_MSGLINE:
                {
                DrawMessage( pDC, rect ); // show message prompt
                break;
                }
            case ID_STATUS_RECORD_KEY:
                {
                DrawRecordKey( pDC, rect ); // show record key info
                break;
                }
            case ID_STATUS_PRIMARY_KEY:
                {
                DrawPrimaryKey( pDC, rect ); // show primary key info
                break;
                }
            case ID_STATUS_RECORD_POSITION:
                {
                DrawRecordPos( pDC, rect );
                break;
                }
            case ID_STATUS_PROJECT:
                {
                DrawProjectName( pDC, rect );
                break;
                }

            default:
                ASSERT(FALSE);
            }
        }
}

void CShwStatusBar::DrawMessage( CDC* pDC, CRect& rect ) // draw message prompt
{
    pDC->SetTextAlign( TA_BOTTOM | TA_LEFT );
	CString swText = swUTF16( m_sMessage ); // 1.4qta
    pDC->ExtTextOut( rect.left+1, rect.bottom-1, ETO_OPAQUE | ETO_CLIPPED, &rect, swText, swText.GetLength(), NULL); // 1.4qta
}

void CShwStatusBar::DrawRecordKey( CDC* pDC, CRect& rect )
{
    DrawKey( pDC, rect, m_sRecordMkr, m_sRecordFld, m_plngRecFld, m_hfntRecord);
}

void CShwStatusBar::DrawPrimaryKey( CDC* pDC, CRect& rect )
{
    DrawKey( pDC, rect, m_sPriKeyMkr, m_sPriKeyFld, m_plngPriKeyFld, m_hfntPriKey);
}

// 1998-03-05 MRP: Remove duplication in drawing code.
void CShwStatusBar::DrawKey( CDC* pDC, CRect& rect,
        const Str8& sKeyMarker, const Str8& sKeyField,
        const CLangEnc* plngKey, HFONT& hfntKey )
{
    pDC->SetTextAlign( TA_BOTTOM | TA_LEFT | TA_UPDATECP );
	Str8 sText = ""; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
    pDC->ExtTextOut( 0, 0, ETO_OPAQUE | ETO_CLIPPED, &rect, swText, 0, NULL); // clear rect
    if ( !sKeyMarker.GetLength() ) // nothing to draw!
        return;

    if ( !plngKey )             // 09-22-1997 no language - don't draw something
        return;

    int iYOffset=0;
    CFont* pfnt=NULL;
    if ( hfntKey )
        {
        pfnt = CFont::FromHandle(hfntKey); // make sure it's safe to use
        if ( pfnt )
            {
            TEXTMETRIC tm;
            pDC->GetTextMetrics(&tm); // measure fonts being used so we can align them nicely
            int iMkrDescent = tm.tmDescent;
            CFont* pfntOld = pDC->SelectObject(pfnt); // select contents font into device context
            pDC->GetTextMetrics(&tm);
            pDC->SelectObject(pfntOld); // select normal font again for marker display
            if ( iMkrDescent < tm.tmDescent )
                iYOffset = tm.tmDescent - iMkrDescent; // make baselines align
            }
        else
            hfntKey = NULL;
        }

    pDC->MoveTo( rect.left+1, rect.bottom-iYOffset );
	Str8 s = "\\"; // 1.4qwr
    pDC->TextOut( 0, 0, swUTF16( s ), 1 ); // 1.4qwr Upgrade TextOut for Unicode build
    pDC->TextOut( 0, 0, swUTF16( sKeyMarker ), sKeyMarker.GetLength() ); // 1.4qwr
	s = "  "; // 1.4qwr
    pDC->TextOut( 0, 0, swUTF16(s), 2 ); // 1.4qwr
    if ( !pfnt )
        return;

    if ( iYOffset )
        {
        CPoint pnt = pDC->GetCurrentPosition(); // move back down if necessary
        pnt.y += iYOffset;
        pDC->MoveTo( pnt );
        }
    pDC->SelectObject( pfnt );
        
    //09-15-1997 Replaced the following line in order to provide the rendering DLL
    //pDC->ExtTextOut( 0, 0, ETO_CLIPPED, &rect, m_sRecordFld, m_sRecordFld.GetLength(), NULL );
    // 1998-03-05 MRP: Use CLangEnc functions
    // int iTextToDraw;
    // if (m_plngRecFld->bRightToLeft() )
    //     iTextToDraw = iSetRightToLeftText(m_sRecordFld, rect, pDC, m_plngRecFld);
    // else
    //     iTextToDraw = m_sRecordFld.GetLength();
    // TextOutClip(pDC, m_plngRecFld, m_sRecordFld, iTextToDraw,
    //             &rect, NULL, 0, ETO_CLIPPED);
    const char* pszUnderlying = sKeyField;
    int lenUnderlying = sKeyField.GetLength();
    const int ixGap = 5;  // Leaving a gap at the right edge looks better
    int ixWidth = rect.right - pDC->GetCurrentPosition().x - ixGap;
    if ( ixWidth < 0 )  // If the marker is especially long
        ixWidth = 0;  // NOTE: bClipAlignedSubstring requires 0 <= ixWidth
    if ( plngKey->bRightToLeft() )
        {
        plngKey->bClipAlignedSubstring(pDC, TRUE,
            &pszUnderlying, &lenUnderlying, &ixWidth);
        int iy = pDC->GetCurrentPosition().y;
        pDC->MoveTo(rect.right - ixGap - ixWidth , iy);  // Right-align
        }
    plngKey->ExtTextOutLng(pDC, 0, 0, ETO_CLIPPED, &rect,
        pszUnderlying, lenUnderlying, 0);
}  // CShwStatusBar::DrawKey

void CShwStatusBar::DrawRecordPos( CDC* pDC, CRect& rect ) // draw current record number and total record count
{
    char pszRecPos[22];
    if ( m_lTotalRecs == -1 ) // signals no view open
        pszRecPos[0] = '\0';
    else
        wsprintfA(pszRecPos, "%lu/%lu", m_lCurrentRec, m_lTotalRecs ); // 1.4qwe

    pDC->SetTextAlign( TA_BOTTOM | TA_CENTER );
	Str8 sText = pszRecPos; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
    pDC->ExtTextOut( rect.left+rect.Width()/2, rect.bottom-1, ETO_OPAQUE | ETO_CLIPPED, &rect, swText, swText.GetLength(), NULL); // 1.4qta
}


void CShwStatusBar::DrawProjectName( CDC* pDC, CRect& rect )  // draw the current project.
{
    pDC->SetTextAlign( TA_BOTTOM | TA_LEFT );
	Str8 sText = m_sProject; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
    pDC->ExtTextOut( rect.left+1, rect.bottom-1, ETO_OPAQUE | ETO_CLIPPED, &rect, swText, swText.GetLength(), NULL); // 1.4qta
}

void CShwStatusBar::PaintProgress(CDC* pDC, CRect& rectBar)
{
    CRect rectFinished = rectBar;
    CRect rectRemaining = rectBar;
   
    int iGaugeWidth = rectBar.Width();

    // Compute the width without using floating point arithmetic.
    // 1999-10-12 MRP: Avoid overflow in numerator of the width formula.
    int wProgressX = rectBar.Width();
    if ( 0 < m_maxlValue )
            wProgressX = (WORD)((m_lPercent * rectBar.Width()) / 100);

    // adjust the rects for the appropriate size
    rectFinished.right = rectFinished.left + wProgressX;
    rectRemaining.left = rectRemaining.left + wProgressX;

    char caPercent[10];
    
    int iLen = wsprintfA(caPercent, "%3ld%%", m_lPercent); // 1.4qwe
	Str8 s = caPercent; // 1.4qwn
	CString sw = swUTF16( s ); // 1.4qwn
    CSize sizeExtent = pDC->GetTextExtent( sw, iLen); // 1.4qwn Upgrade GetTextExtent for Unicode build

    pDC->SetBkColor( m_rgbBkg );
    pDC->SetTextColor( m_rgbText );

	Str8 sText = caPercent; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
    pDC->ExtTextOut( (rectBar.Width() - sizeExtent.cx) / 2 + rectBar.left,
                      (rectBar.Height() - sizeExtent.cy) / 2 + rectBar.top,
                      ETO_OPAQUE | ETO_CLIPPED, &rectFinished, swText, swText.GetLength(), NULL); // 1.4qta

    //  Reverse fore and back colors for drawing the undone (ie the non-
    //  finished) side of the box.
    pDC->SetBkColor( m_rgbText );     // reverse 'em
    pDC->SetTextColor( m_rgbBkg );

    //  Draw the un-finished (ie the percent not-done) side of box.
    pDC->ExtTextOut( (rectBar.Width() - sizeExtent.cx) / 2 + rectBar.left,
                      (rectBar.Height() - sizeExtent.cy) / 2 + rectBar.top,
                    ETO_OPAQUE | ETO_CLIPPED, &rectRemaining, swText, swText.GetLength(), NULL);
}

void CShwStatusBar::SetRecordInfo( const char* pszContents, const char* pszMkr,
                                   HFONT hfnt, CLangEnc* plng )
{
    if ( hfnt != m_hfntRecord || // font changed
        strcmp( pszContents, m_sRecordFld ) || // record field contents changed
        strcmp( pszMkr, m_sRecordMkr ) || // marker changed
        plng != m_plngRecFld )   // language changed   09-16-1997
        {
        m_hfntRecord = hfnt;
        m_sRecordFld = pszContents;
        m_sRecordMkr = pszMkr;
        m_plngRecFld = plng;                  //09-16-1997
        InvalidatePane(ID_STATUS_RECORD_KEY); // cause redraw
        }
}

void CShwStatusBar::SetPriKeyInfo( const char* pszContents,
                                   const char* pszMkr, HFONT hfnt,
                                   CLangEnc* plng )
{
    if ( hfnt != m_hfntPriKey || // font changed
        strcmp( pszContents, m_sPriKeyFld ) || // record field contents changed
        strcmp( pszMkr, m_sPriKeyMkr ) || // marker changed
        plng != m_plngPriKeyFld )     // language changed  09-16-1997
        {
        m_hfntPriKey = hfnt;
        m_sPriKeyFld = pszContents;
        m_sPriKeyMkr = pszMkr;
        m_plngPriKeyFld = plng;                //09-16-1997
        InvalidatePane(ID_STATUS_PRIMARY_KEY); // cause redraw
        }
}

void CShwStatusBar::SetRecordPosInfo( long int lCurrent, long int lTotal )
{
    if ( lCurrent != m_lCurrentRec || lTotal != m_lTotalRecs )
        {
        m_lCurrentRec = lCurrent;
        m_lTotalRecs = lTotal;
        InvalidatePane(ID_STATUS_RECORD_POSITION); // cause redraw
        }
}

void CShwStatusBar::InvalidatePane( int iPane ) // invalidate the area of status bar used by iPane
{
    CRect rect;
    GetItemRect( iPane, &rect );
    InflateRect( &rect, -CX_BORDER, -CY_BORDER ); // don't need to draw borders each time
    // We have to paint directly on the Mac because of some strange bug that causes a pane to be
    // cleared but DrawItem() not called when we call InvalidateRect().
    // A nice side effect though, of painting directly on Win platforms is that we eliminate the
    // flicker caused by the owner draw pane's insistence on erasing the background before
    // calling our DrawItem().
    if ( !m_bShowingProgress ||
         (m_bShowingProgress && (iPane == ID_STATUS_PROGRESSBAR || iPane == ID_STATUS_PROGRESSMSG)) )
        {
        CDC* pDC = GetDC();
        DrawPane( pDC, iPane, rect);    
        ReleaseDC( pDC );
        }
}   

/************
CShwStatusBar::OnUpdateCmdUI


It's an override of the CControlBar and CStatusBar virtual member function that 
is called when CControlBar processes WM_IDLEUPDATECMDUI.  That message is generated
in CWinApp::OnIdle and sent to the various frame windows and their children.

CStatusBar::OnUpdateCmdUI asks each of the items in the status bar to update
themselves by calling DoUpdate for each.  It then calls UpdateDialogControls.

In CShwStatusBar, I don't need to iterate through each of the items.  Rather, I
have the view object update the status bar (the view object contains all of the
information relevant to the status bar.)  If there is no view, the items are
all made blank.

*************/

void CShwStatusBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    CMainFrame* pfrm = (CMainFrame*)pTarget->GetActiveFrame();
    
    if (pfrm == NULL)
        return;

    CShwView* pview = (CShwView*)pfrm->GetActiveView();
    
    if (pview == NULL)
        {
        SetRecordInfo("","",NULL,NULL);  //09-15-1997 Added parameter 4
        SetPriKeyInfo("","",NULL,NULL);  //09-15-1997 Added parameter 4
        SetRecordPosInfo(-1,-1);
        SetProjectName( (Shw_papp()->bProjectOpen()) ? 
                    sGetFileName(Shw_pProject()->pszPath()) : "");
        // Project name may yet be available.

        return;
        }
        
    // If it's a Shoebox database view, then
    if (!pview->IsKindOf(RUNTIME_CLASS( CShwView ) ) )
        return;

    pview->UpdateStatusBar(this);
}



CShwStatusBar* Shw_pbarStatus()
{
    return (CShwStatusBar*)AfxGetApp()->m_pMainWnd->
        GetDescendantWindow(IDW_SHW_STATUS_BAR);
}

void CShwStatusBar::InitializeProgress(long maxlValue, const char* pszMessage,
        BOOL bShowPercent)
{
    // 1999-10-12 MRP: Avoid overflow in numerator of the percent formula.
    m_maxlValue = maxlValue;
    m_lValue = 0;
    m_lPercent = 0;
    m_bShowPercent = bShowPercent;
    // The ordinary formula will overflow if the maximum value
    // is greater than 1 percent of the maximum long value (about 20M).
    m_bOverflow = ((LONG_MAX / 100) < m_maxlValue);

    UseProgressIndicators();
    SetStatusMessage(pszMessage);
    m_bShowingProgress = TRUE;
    Invalidate(FALSE);
    UpdateWindow();
}


void CShwStatusBar::EndProgress()
{
    m_bShowingProgress = FALSE;
    UseNormalIndicators();
    Invalidate(FALSE);
}


void CShwStatusBar::UpdateProgress(long lValue)
{
    ASSERT( 0 <= lValue );

    m_lValue = lValue;
    if ( m_maxlValue < m_lValue )
        m_lValue = m_maxlValue;
   
    long lPercent = 100;
    if ( m_maxlValue != 0 )  // Avoid divide-by-zero
        {
        // Compute the percent without using floating point arithmetic.
        // 1999-10-12 MRP: Avoid overflow in numerator of the percent formula.
        // The ordinary formula will overflow if the maximum value
        // is greater than 1 percent of the maximum long value (about 20M).
        if ( m_bOverflow )
            lPercent = m_lValue / (m_maxlValue / 100);
        else
            lPercent = (m_lValue * 100) / m_maxlValue;
        }
   
   if ( m_lPercent != lPercent)
        {
        m_lPercent = lPercent;
        InvalidateBar();  // Invalidate only the progress bar area.
        }
}

void CShwStatusBar::SetStatusMessage(const char* pszMessage)
{
	return; // 1.4qzhp Eliminate status bar messages
    if ( m_bShowingProgress )
        return; // lock out attempts to set message while showing progress bar
	if ( pszMessage ) // 1.4hfc Fix bug of crash on status bar null message
	    m_sMessage = pszMessage;
	else
		m_sMessage = ""; // 1.4hfc
    InvalidatePane(ID_STATUS_MSGLINE);
}

void CShwStatusBar::SetProjectName(const char* pszProject)
{
    if (strcmp(m_sProject, pszProject) != 0)
        {   // Update only if changed.
        m_sProject = pszProject;
        InvalidatePane(ID_STATUS_PROJECT);
        }
}

void CShwStatusBar::InvalidateBar()
{
    CRect rectProgress;
    GetItemRect( ID_STATUS_PROGRESSBAR, &rectProgress );
    InflateRect( &rectProgress, -CX_BORDER, -CY_BORDER ); // don't need to draw borders
    // have to paint directly to get rid of annoying flicker
    CDC* pDC = GetDC();
    pDC->SelectObject( &m_fntNormal );
    PaintProgress( pDC, rectProgress ); 
    ReleaseDC( pDC );
}

#ifdef _DEBUG
void CShwStatusBar::AssertValid() const
{
    CStatusBar::AssertValid();
}
#endif  // _DEBUG
