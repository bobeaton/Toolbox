// font.h
// Brian Yoder  1-24-95

#ifndef font_h
#define font_h

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h
class Object_ofstream;  // obstream.h // 1.6.4aa 

class CFontDef  // Hungarian: fntdef
{
private:
    LOGFONT m_lf; // font definition structure
    COLORREF m_rgb; // color of font
    BOOL m_bSaveColor; // if FALSE, WriteProperties won't write color to settings file
        
public:
#ifdef TLB_07_18_2000_FONT_HANDLING_BROKEN
//    CFontDef( const char* pszName, int iHeight, BOOL bBold=FALSE, // used to initialize to default values
//                COLORREF rgb = -1, BOOL bItalic=FALSE, BOOL bUnderline=FALSE,
//                BOOL bStrikeOut=FALSE );
#elif defined(_MAC)
    CFontDef(const char* pszName, int iHeight ); // iHeight always interpreted as point size on Mac
#else
    CFontDef(const char* pszName, int iHeight, BOOL bPoints=FALSE ); // iHeight can mean points or pixels depending on bPoints
#endif
    CFontDef(const CFont* pfnt, COLORREF rgb = -1) { Set(pfnt, rgb); } // initialize from an existing font

    // copy constructor
    CFontDef(const CFontDef& fntdef) { m_lf = fntdef.m_lf;
                                       m_rgb = fntdef.m_rgb;
                                       m_bSaveColor = fntdef.m_bSaveColor; }

    void Set(const CFont* pfnt, COLORREF rgb = -1); // change font definition

    void SetColor( COLORREF rgb ) { m_rgb = rgb; }
    LOGFONT* plf() { return &m_lf; } // Access to logical font structure
    COLORREF rgb() const { return m_rgb; } // access to color

#ifdef prjWritefstream // 1.6.4aa 
   void WriteProperties(Object_ofstream& obs, const char* pszQualifier=NULL) const;
#else
    void WriteProperties(Object_ostream& obs, const char* pszQualifier=NULL) const;
#endif
    BOOL bReadProperties(Object_istream& obs, const char* pszQualifier=NULL);

    // create a font and return it to caller - the return value is font height in pixels
    int iCreateFont( CFont* pfnt, COLORREF* prgb=NULL ) const;
    int iCreateFont( CFont* pfnt, COLORREF* prgb,
            int* piAscent, int* piOverhang ) const;
#ifdef _MAC
    int iCreateFontMakeFit( CFont* pfnt, COLORREF* prgb=NULL );
#endif

    BOOL bModalView_Properties( BOOL bUseColor ); // allow user to change font

#ifndef TLB_07_18_2000_FONT_HANDLING_BROKEN
    static int s_iConvertPointsToPixels(int iPoints);
    static int s_iConvertPixelsToPoints(int iPixels);
#endif
    
};  // class CFontDef

// NOTE! The default value of -1 for the rgb parameter in the constructors and Set() can
// mean either that the owner doesn't care about color or that they want the default system
// color.  If the owner of a CFontDef object wants the color setting saved, it must
// explicitly pass in the color instead of using the default parameter.

// See MKR.H, MKR.CPP and MKR_D.CPP for an example of how this class is used.

#if !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)
// On the Mac, we don't know if this is a problem and if it is we don't know enough to be able to fix it.

class CDCFontRightSizer
{
private:
    CDC * const m_pcDC;
    CFont* m_pfntOrig; // Original font selected for the DC, or value passed to constructor (will be restored in destructor)
    CFont* m_pfntNew;  // Right-sized font created and selected for the DC (will be destroyed in destructor)

public:
    CDCFontRightSizer(CDC* pcDC, CFont* pfntOrig = NULL); // constructor
    ~CDCFontRightSizer(); // destructor (restores original font and destroys new font, if needed)

    // This is the workhorse function. Based on the DC's existing font, it creates,
    // if necessary, a new version of the font that is no larger than the maximum requested
    // height and selects it into the DC. It returns in the tm parameter the text metrics for
    // the font that is ultimately selected.
    void SelectRightFont(int iMaxHeight, TEXTMETRIC &tm);

};  // class CDCFontRightSizer

#endif !defined(TLB_07_18_2000_FONT_HANDLING_BROKEN) && !defined(_MAC)

#endif // font_h
