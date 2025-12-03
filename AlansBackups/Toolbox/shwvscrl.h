// shwvscrl.h : interface of the CShwScrollView class
//
/////////////////////////////////////////////////////////////////////////////

class CLPoint
{
public:
    int x;
    long int y;
    
    // constructors
    CLPoint() { y = 0; x = 0; }
    CLPoint( int xpar, long int ypar ) { x = xpar; y = ypar; }
    CLPoint( CPoint pnt ) { x = pnt.x; y = pnt.y; }
    
    CLPoint( const CLPoint& pnt ) { x = pnt.x; y = pnt.y; } // copy constructor
    
    CLPoint operator+(const CPoint& pnt) const
    	{ return CLPoint( this->x + pnt.x, this->y + pnt.y ); }
    CLPoint operator+(const CLPoint& pnt) const
    	{ return CLPoint( this->x + pnt.x, this->y + pnt.y ); }
    CLPoint operator-(const CPoint& pnt) const
    	{ return CLPoint( this->x + pnt.x, this->y + pnt.y ); }
    CLPoint operator-(const CLPoint& pnt) const
    	{ return CLPoint( this->x + pnt.x, this->y + pnt.y ); }

    operator struct tagPOINT () const
    	{
//    	ASSERT( y > -32000 && y < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // don't convert to CPoint with out of range values
		return CPoint( x, (int)y );
		}
};

class CShwScrollView : public CView
{
    DECLARE_DYNAMIC(CShwScrollView)

// Constructors
protected:
    CShwScrollView();

public:
    static const SIZE sizeDefault;
        // used to specify default calculated page and line sizes

    // in logical units - call one of the following Set routines
    void SetScrollSizes(int nMapMode, SIZE sizeTotal,
                const SIZE& sizePage = sizeDefault,
                const SIZE& sizeLine = sizeDefault);

// Attributes
public:
    CPoint GetScrollPosition() const;       // upper corner of scrolling
    CSize GetTotalSize() const;             // logical size

    // for device units
    CPoint GetDeviceScrollPosition() const;
    void GetDeviceScrollSizes(int& nMapMode, SIZE& sizeTotal,
            SIZE& sizePage, SIZE& sizeLine) const;

// Operations
public:
    void ScrollToPosition(POINT pt);    // set upper left position
    void FillOutsideRect(CDC* pDC, CBrush* pBrush);

// Implementation
protected:
    int m_nMapMode;
    CSize m_totalLog;         // total size in logical units (no rounding)
    CSize m_totalDev;         // total size in device units
    CSize m_pageDev;          // per page scroll size in device units
    CSize m_lineDev;          // per line scroll size in device units

    BOOL m_bInsideUpdate;    // internal state for OnSize callback
    void ScrollToDevicePosition(POINT ptDev); // explicit scrolling no checking

protected:
    virtual void OnDraw(CDC* pDC) = 0;      // pass on pure virtual

    void UpdateBars(BOOL bSendRecalc = TRUE); // adjust scrollbars etc
    BOOL GetTrueClientSize(CSize& size, CSize& sizeSb);
        // size with no bars
    void GetScrollBarSizes(CSize& sizeSb);
    void GetScrollBarState(CSize sizeClient, CSize& needSb,
        CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient);

public:
    virtual ~CShwScrollView();
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif //_DEBUG
    virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);

    // scrolling implementation support for OLE 2.0
    virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
    virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);

    //{{AFX_MSG(CShwScrollView)
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	 afx_msg BOOL OnMouseWheel(UINT fFlags, short zDelta, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

