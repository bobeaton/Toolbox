// status.h  Interface for a statusbar with custom fonts for its items (1995-10-02)

// Also, the justification of items within their areas will be adjustable.

// A class derived from (MFC class) CStatusBar provides the functionality of
// a standard status bar plus user-definable fonts for the elements of the
// status bar.

// If a font is not defined for a particular item, then the default font for the
// whole status bar is in effect.

// RNE  More detail will be added later...
// RNE  More detail will be added later...


// 09-15-1997 Changed some parameterlists and commands in the class in
// order to provide the rendering DLL. The changes are marked with this date


#include <afx.h>
#include <afxext.h>


#ifndef _status_h
#define _status_h

class CLangEnc;   //09-15-1997

// Helper functions.
BOOL    bTranslateThisMessage(MSG* pMsg);

class CShwStatusBar : public CStatusBar
{
    // DECLARE_DYNAMIC(CShwStatusBar)
    
private:
    CFont m_fntSize; // for sizing status bar - not used to display anything
    CFont m_fntNormal; // standard status bar font

// Job progress information.
    BOOL        m_bShowingProgress;
    // DWORD       m_nRange;   // the relative range of the progress bar
    // DWORD       m_nPos;     // the current position
    // WORD        m_wPercent; // The current percentage done.
    long m_maxlValue;
    long m_lValue;
    long m_lPercent;
    BOOL m_bShowPercent;  // Show the graphical percent/progress bar?
    BOOL m_bOverflow;
    COLORREF    m_rgbText;  // the text and bar color
    COLORREF    m_rgbBkg;   // the background color
    
    Str8 m_sMessage;
    Str8 m_sRecordFld; // info from current view
    Str8 m_sRecordMkr; // cache it to avoid unnecessary redraws
    Str8 m_sPriKeyFld;
    Str8 m_sPriKeyMkr;
    Str8 m_sProject;
    
    long m_lCurrentRec;
    long m_lTotalRecs;
    HFONT m_hfntRecord;
    HFONT m_hfntPriKey;

    CLangEnc* m_plngRecFld;    //09-15-1997
    CLangEnc* m_plngPriKeyFld; //09-15-1997
    
protected:
    void InvalidateBar();
    // Invalidates the progress rectangle only.

public:
    CShwStatusBar();
    virtual ~CShwStatusBar();
    
    // ************* Overrides of CStatusBar

    // Calculates minimum, maximum size of status bar according to default font.
    // Calls base class CStatusBar::Create()
    virtual BOOL Create(CWnd* pParentWnd,
            DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_BOTTOM,
            UINT nID = AFX_IDW_STATUS_BAR);
    
    void SetFontAndSize( BOOL bRelayOut=TRUE ); // set size of status bar

    BOOL UseProgressIndicators(); // setup normal status bar panes
    BOOL UseNormalIndicators(); // setup status bar for progress indicator

    int iPanes() const; // return number of panes
    
    // Override of CControlBar's.  This processes the application's WM_IDLEUPDATECMDUI
    // message.
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS); // owner draw paint function
    
      //09-15-1997 Added parameter 4. This parameter is necessary to support
      //the TextOutClip function. This function gets only called if there is
      //something to draw. plng can be NULL if only a empty field should
      //be drawn. But make shure, that plng != NULL if TextOutClip is called
    void SetRecordInfo( const char* pszContents, // Primary key info from current view
                        const char* pszMkr, HFONT hfnt, CLangEnc* plng );
    void SetPriKeyInfo( const char* pszContents, // Primary key info from current view
                        const char* pszMkr, HFONT hfnt, CLangEnc* plng );
    
    
    void SetRecordPosInfo( long int lCurrent, long int lTotal ); // record position info from view
    void SetStatusMessage(const char* pszMessage); // set message prompt
    void SetProjectName(const char* pszProject);
    
    void InvalidatePane( int iPane ); // invalidate the area of status bar used by iPane
    
    // *********************** Progress information ****************/
    void InitializeProgress(long maxlValue, const char* pszMessage, 
        BOOL bShowPercent);
    void EndProgress();
    
    void SetProgressColors(COLORREF rgbText, COLORREF rgbBkg)
    {
        m_rgbText = rgbText;
        m_rgbBkg = rgbBkg;
    }
    
    void UpdateProgress(long lValue);
    void UpdateTextProgress( const char* pszMessage );

private:
    void DrawMessage( CDC* pDC, CRect& rect ); // draw message prompt
    void DrawRecordKey( CDC* pDC, CRect& rect ); // draw Record key
    void DrawPrimaryKey( CDC* pDC, CRect& rect ); // draw primary key
    void DrawKey( CDC* pDC, CRect& rect,
            const Str8& sKeyMarker, const Str8& sKeyField,
            const CLangEnc* plngKey, HFONT& hfntKey );  // 1998-03-05 MRP
    void DrawRecordPos( CDC* pDC, CRect& rect ); // draw current record number and total record count
    void DrawProjectName( CDC* pDC, CRect& rect ); // draw current project name
    void PaintProgress( CDC* pDC, CRect& rectStatusBar );

    void DrawPane( CDC* pDC, int iPane, CRect& rect ); // paint specified pane

protected:

public:
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif
};

CShwStatusBar* Shw_pbarStatus();

#endif // _status_h

