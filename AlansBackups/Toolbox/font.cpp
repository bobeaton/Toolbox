#include "stdafx.h"
#include "toolbox.h"
#include "font.h"
#include "obstream.h"  // Object_istream, Object_ostream

#ifdef TLB_07_18_2000_FONT_HANDLING_BROKEN
// CFontDef::CFontDef(const char* pszName, int iHeight, BOOL bBold, // used to initialize to default values
//            COLORREF rgb, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut)
#elif defined(_MAC)
CFontDef::CFontDef(const char* pszName, int iHeight )
#else
CFontDef::CFontDef(const char* pszName, int iHeight, BOOL bPoints )
#endif
{
    memset( &m_lf, 0, sizeof( m_lf ) ); // clear out all members
    _tcscpy( m_lf.lfFaceName, swUTF16( Str8(pszName).Left( LF_FACESIZE-1 ) ) ); // 1.4qys
    if (iHeight > 0)
        iHeight = -iHeight; // read and write properties assume a negative value for lfHeight
                            // When creating a font, negative means character height (as opposed to line height)
#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)
    if ( bPoints )
        iHeight = s_iConvertPointsToPixels(iHeight);
#endif
    m_lf.lfHeight = iHeight;
#if defined(TLB_07_18_2000_FONT_HANDLING_BROKEN)
//    m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
//    m_lf.lfItalic = bItalic;
//    m_lf.lfUnderline = bUnderline;
//    m_lf.lfStrikeOut = bStrikeOut;
//    m_lf.lfCharSet = DEFAULT_CHARSET;
//    if (rgb == -1) // not a valid color value
//        {
//        m_rgb = ::GetSysColor( COLOR_WINDOWTEXT );
//        m_bSaveColor = FALSE;
//        }
//    else
//        {
//        m_rgb = rgb;
//        m_bSaveColor = TRUE;
//        }
#else
    m_lf.lfWeight = FW_NORMAL;
    m_lf.lfCharSet = DEFAULT_CHARSET;
    m_rgb = ::GetSysColor( COLOR_WINDOWTEXT );
    m_bSaveColor = FALSE;
#endif
}

void CFontDef::Set(const CFont* pfnt, COLORREF rgb) // initialize from an existing font
{
    pfnt->GetObject( sizeof(m_lf), &m_lf ); // get parameters from font

    if (rgb == -1) // not a valid color value
        {
        m_rgb = ::GetSysColor( COLOR_WINDOWTEXT );
        m_bSaveColor = FALSE;
        }
    else
        {
        m_rgb = rgb;
        m_bSaveColor = TRUE;
        }
}

// create a font and return it to caller - the return value is font height in pixels
int CFontDef::iCreateFont( CFont* pfnt, COLORREF* prgb ) const
{
    int iAscent;
    int iOverhang;
    return iCreateFont(pfnt, prgb, &iAscent, &iOverhang);
}

int CFontDef::iCreateFont( CFont* pfnt, COLORREF* prgb,
        int* piAscent, int* piOverhang ) const
{
    pfnt->DeleteObject(); // trash old font (if any)
    BOOL bCreated = pfnt->CreateFontIndirect( &m_lf ); // create new font
    ASSERT( bCreated );

    CDC dc;
    dc.CreateCompatibleDC( NULL ); // Create a temporary screen device context for measuring font
    dc.SelectObject( pfnt ); // select font into device context

    TEXTMETRIC tm;
    dc.GetTextMetrics( &tm ); // get text measurements

    if ( prgb )
        *prgb = m_rgb; // return font color to caller

    ASSERT( piAscent );        
    *piAscent = tm.tmAscent;
    ASSERT( piOverhang );
    *piOverhang = tm.tmOverhang;

    return tm.tmHeight; // return font height
}

#ifdef _MAC
// Some mac fonts (Geneva) return a height greater than what we ask for.
// This function is used to make sure we return a font no larger than specified
int CFontDef::iCreateFontMakeFit( CFont* pfnt, COLORREF* prgb )
{
    int iTargetHt = abs(m_lf.lfHeight);

    int iHt = 0;
    while (TRUE)
        {
        ASSERT(m_lf.lfHeight);

        iHt = iCreateFont( pfnt, prgb );

        if ( iHt <= iTargetHt ) // size okay
            return iHt;

        if ( m_lf.lfHeight > 0 ) // shrink till it fits
            m_lf.lfHeight--;
        else
            m_lf.lfHeight++;
        }
}
#endif

BOOL CFontDef::bModalView_Properties( BOOL bUseColor ) // allow user to change font
{
    int iFlags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT; // | CF_NOSCRIPTSEL 1.4va Try Removing script control from font dialog boxes, no need for now
    if (bUseColor)
        iFlags |= CF_EFFECTS;

    CFontDialog dlg( &m_lf, iFlags );
    dlg.m_cf.rgbColors = m_rgb;
    if ( dlg.DoModal() == IDOK )
        {
        m_rgb = dlg.GetColor();
        return TRUE;
        }
    return FALSE;
}

static const char* psz_fnt = "fnt";
static const char* psz_Name = "Name";
static const char* psz_Size = "Size";
static const char* psz_Bold = "Bold";
static const char* psz_Italic = "Italic";
static const char* psz_Underline = "Underline";
static const char* psz_StrikeOut = "StrikeOut";
static const char* psz_charset = "charset";  // 1999-09-25 MRP
static const char* psz_rgbColor = "rgbColor";

#ifdef prjWritefstream // 1.6.4aa 
void CFontDef::WriteProperties(Object_ofstream& obs, const char* pszQualifier) const 
#else
void CFontDef::WriteProperties(Object_ostream& obs, const char* pszQualifier) const 
#endif
{
        obs.WriteBeginMarkerWithQualifier(psz_fnt, pszQualifier);
		CString sw = m_lf.lfFaceName; // 1.4qyr
        obs.WriteString(psz_Name, sUTF8( sw ) ); // 1.4qyr
#ifdef _MAC
        obs.WriteInteger(psz_Size, -m_lf.lfHeight ); // no conversion necessary
#elif defined(TLB_07_18_2000_FONT_HANDLING_BROKEN)
//        obs.WriteInteger(psz_Size, ((-m_lf.lfHeight)*3+1)/4 ); // convert pixels to points
#else
        obs.WriteInteger(psz_Size, s_iConvertPixelsToPoints(-m_lf.lfHeight));
#endif
        obs.WriteBool(psz_Bold, m_lf.lfWeight > FW_MEDIUM ); // 1.4qzpgh Fix bug of Code2000 font regular changing to bold
        obs.WriteBool(psz_Italic, m_lf.lfItalic );
        obs.WriteBool(psz_Underline, m_lf.lfUnderline );
        obs.WriteBool(psz_StrikeOut, m_lf.lfStrikeOut );
        // 1999-09-25 MRP: Write the "Font Charset" property
        // (i.e. a hex code that corresponds to the Script drop-down list
        // in the Font dialog box). Users have been able to select the Script,
        // but it was "forgetten" when Shoebox exited.
        if ( m_lf.lfCharSet != DEFAULT_CHARSET )
            {
            Str8 sCharsetByteHex;
            sCharsetByteHex.Format("%02X", m_lf.lfCharSet);
            obs.WriteString(psz_charset, sCharsetByteHex);
            }
        if (m_bSaveColor)
            obs.WriteCOLORREF(psz_rgbColor, m_rgb);
        obs.WriteEndMarkerWithQualifier(psz_fnt, pszQualifier);
}

BOOL CFontDef::bReadProperties(Object_istream& obs, const char* pszQualifier)
{
    if ( !obs.bReadBeginMarkerWithQualifier(psz_fnt, pszQualifier) )
        return FALSE;
        
    while ( !obs.bAtEnd() ) // While more in file
        {
        Str8 sFontName; // these temporaries are used because the types of the LOGFONT
        int iFontSize;     // members don't match the types required by the bRead functions
        BOOL bBold;
        BOOL bItalic;
        BOOL bUnderline;
        BOOL bStrikeOut;
        Str8 sCharsetByteHex;
        
        if ( obs.bReadString(psz_Name, sFontName ) )
            _tcscpy( m_lf.lfFaceName, swUTF16( sFontName.Left( LF_FACESIZE-1 ) ) ); // 1.4qys
        else if ( obs.bReadInteger(psz_Size, iFontSize) )
            {
#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN)
            if ( iFontSize > 0 )
#endif
                {
#ifdef _MAC
                m_lf.lfHeight = -iFontSize;   // no conversion necessary on Mac
#elif defined(TLB_07_18_2000_FONT_HANDLING_BROKEN)
//                m_lf.lfHeight = -((iFontSize*4+1)/3);   // convert points to pixels
#else
                m_lf.lfHeight = -s_iConvertPointsToPixels(iFontSize);
#endif
                }
            }
        else if ( obs.bReadBool(psz_Bold, bBold) )
            m_lf.lfWeight = FW_BOLD;
        else if ( obs.bReadBool(psz_Italic, bItalic) )
            m_lf.lfItalic = TRUE;
        else if ( obs.bReadBool(psz_Underline, bUnderline) )
            m_lf.lfUnderline = TRUE;
        else if ( obs.bReadBool(psz_StrikeOut, bStrikeOut) )
            m_lf.lfStrikeOut = TRUE;
        // 1999-09-25 MRP: Read the "Font Charset" property.
        else if ( obs.bReadString(psz_charset, sCharsetByteHex) )
            {
            int iCharset = DEFAULT_CHARSET;
            if ( sscanf(sCharsetByteHex, "%X", &iCharset) == 1)
                m_lf.lfCharSet = (unsigned char)iCharset;
            }
        else if ( obs.bReadCOLORREF(psz_rgbColor, m_rgb) )
            ;
        else if ( obs.bEndWithQualifier( psz_fnt, pszQualifier ) ) // If end marker or erroneous begin break
            break;
        }
        
    return TRUE;
}

#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)

#define POINTS_PER_LOGICAL_INCH 72

CFontDef::s_iConvertPointsToPixels(int iPoints)
{
    CClientDC dc(NULL); // Device context for full screen
    const int iPixelsPerLogicalInch = dc.GetDeviceCaps(LOGPIXELSY);
    const double dRoundUp = (iPoints < 0)? -0.5 : 0.5;
    return (int)((((double)(iPixelsPerLogicalInch * iPoints)) / POINTS_PER_LOGICAL_INCH) + dRoundUp);
}

CFontDef::s_iConvertPixelsToPoints(int iPixels)
{
    CClientDC dc(NULL); // Device context for full screen
    const int iPixelsPerLogicalInch = dc.GetDeviceCaps(LOGPIXELSY);
    ASSERT(iPixelsPerLogicalInch > 0); // If =0, Shoebox will crash.
    const double dRoundUp = (iPixels < 0)? -0.5 : 0.5;
    return (int)((((double)(POINTS_PER_LOGICAL_INCH * iPixels)) / iPixelsPerLogicalInch) + dRoundUp);
}
#endif

#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)

///////////////////////////////////////////////////////////
// CDCFontRightSizer
///////////////////////////////////////////////////////////

// CDCFontRightSizer constructor: if caller passes a non-NULL pfntOrig, we will restore this
// value in the destructor, even if we create a new font in SelectRightFont.
CDCFontRightSizer::CDCFontRightSizer(CDC* pcDC, CFont* pfntOrig) :
    m_pcDC(pcDC),
    m_pfntOrig(pfntOrig),
    m_pfntNew(NULL)
{
}

// CDCFontRightSizer destructor restores original font and destroys new font, if needed
CDCFontRightSizer::~CDCFontRightSizer()
{
    // It's important to restore the original before deleting the new font.
    if ( m_pfntOrig )
        m_pcDC->SelectObject( m_pfntOrig );
    if ( m_pfntNew )
        delete m_pfntNew;
}

// This is the workhorse function. Based on the DC's existing font, it creates,
// if necessary, a new version of the font that is no larger than the maximum requested
// height and selects it into the DC. It returns in the tm parameter the text metrics for
// the font that is ultimately selected.
void CDCFontRightSizer::SelectRightFont(int iMaxHeight, TEXTMETRIC &tm)
{
    BOOL bGotTextMetrics = m_pcDC->GetTextMetrics( &tm );
    if ( tm.tmHeight > iMaxHeight )
        {
        LOGFONT lf;
        lf.lfHeight = iMaxHeight;
        lf.lfWidth = 0; 
        lf.lfEscapement = 0;
        lf.lfOrientation = 0;
        lf.lfWeight = tm.tmWeight;
        lf.lfItalic = tm.tmItalic;
        lf.lfUnderline = tm.tmUnderlined;
        lf.lfStrikeOut = tm.tmStruckOut;
        lf.lfCharSet = tm.tmCharSet;
        lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
        lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		lf.lfClipPrecision |= 0x40; // AB 5.2g fix from Tim Erickson for Korean Windows
        lf.lfQuality = DEFAULT_QUALITY;
        lf.lfPitchAndFamily = tm.tmPitchAndFamily;
        m_pcDC->GetTextFace( sizeof( lf.lfFaceName ), lf.lfFaceName ); // get name of selected font

        if ( !m_pfntNew )
            {
            m_pfntNew = new CFont;
            }

        m_pfntNew->CreateFontIndirect( &lf ); // try to make a copy of the font with only the height changed

        if ( m_pfntOrig ) // Could already be set if it was supplied this as a parameter to constructor or if
                          // if this is not the first time this function is being called
            m_pcDC->SelectObject( m_pfntNew );
        else
            m_pfntOrig = m_pcDC->SelectObject( m_pfntNew );
        bGotTextMetrics = m_pcDC->GetTextMetrics( &tm );
#ifdef _DEBUG
        ASSERT(bGotTextMetrics);
#endif
        }    
}
#endif // !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)
