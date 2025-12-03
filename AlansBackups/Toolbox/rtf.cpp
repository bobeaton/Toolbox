// rtf.cpp  Rich Text Format

#include "stdafx.h"
#include "toolbox.h"
#include "rtf.h"
#include "pgs.h"  // class RtfPageSetup
#include "set.h"  // class CSetEl, CSet
#include "cfield.h"  // class CField
#include "font.h"  // class CFontDef
#include "ddx.h"  // Shw_bNeedsToBeTrimmed
#include <ctype.h>  // isupper
#include "typ.h"


// --------------------------------------------------------------------------


#ifdef _MAC
static const char* s_pszCharSet = "\\mac";
static const char s_chNonBreakingSpace = '\xCA';  // decimal 202
static const char* s_pszDefaultFontName = "Times";
static const char* s_pszMarkerFontName = "Courier";
#else
static const char* s_pszCharSet = "\\ansi";
static const char s_chNonBreakingSpace = '\xA0';  // decimal 160
static const char* s_pszDefaultFontName = "Times New Roman";
static const char* s_pszMarkerFontName = "Courier New";
#endif  // _MAC

static const unsigned int s_nDefaultFont = 0;
static const char* s_pszDefaultFontAndSize =
    "\\f0\\fs20";  // Font size: 10 points
static const char* s_pszAutoFieldFormatting =
    "\\fi-720\\li720";  // Hanging indent: 0.5 in.
static const char* s_pszAutoRecordSeparatorFormatting =
//  "\\sb120"  // Space before: 6 points
    "\\brdrb\\brdrdot\\brdrw15\\brdrsp20";  
    // Border: bottom, dotted, pen width 3/4 point, space to text 1 point
static const char* s_pszAutoRecordFormatting =
    "\\sb120"  // Space before: 6 points
    "\\fi-720\\li720";  // Hanging indent: 0.5 in.

static char* s_pszHeaderFooterFormatting = "\\tqc\\tx4320\\tqr\\tx8640";
static char* s_pszFooterInside = "{DATE}";
static char* s_pszFooterOutside = "{PAGE}";


// ==========================================================================

// A FontName corresponds to an \fN control word in the font table group.
// It assigns to a font name the number used elsewhere in the document.

class FontName : public CSetEl  // Hungarian: fnm
{
private:
    unsigned int m_n;
    
    friend class FontNameSet;
    FontNameSet* m_pfnmsetMyOwner;

    FontName(const char* pszName, FontNameSet* pfnmsetMyOwner,
            unsigned int n);
    
    void WriteDefinition(ostream& ios) const;
    
public:
    void AppendFormattingTo(Str8& sFormatting) const;
    
    virtual CSet* psetMyOwner();    
};  // class FontName


// --------------------------------------------------------------------------

// The FontNameSet corresponds to the {\fonttbl ...} group.
// Wheras Word includes all available fonts, this conversion mechanism
// presently includes only those fonts actually referred to by a style.

class FontNameSet : public CSet  // Hungarian: fnmset
{
private:
    unsigned int m_nNext;
    
public:
    FontNameSet(const char* pszDefaultFontName);
    
    void WriteDefinition(ostream& ios) const;

    FontName* pfnmFirst() const
            { return (FontName*)pselFirst(); }
    FontName* pfnmLast() const
            { return (FontName*)pselLast(); }
    FontName* pfnmNext(const FontName* pfnmCur) const
            { return (FontName*)pselNext(pfnmCur); }
    FontName* pfnmPrev(const FontName* pfnmCur) const
            { return (FontName*)pselPrev(pfnmCur); }
    
    FontName* pfnmSearch(const char* pszName) const
            { return (FontName*)pselSearch(pszName); }
    FontName* pfnmSearch_AddIfNew(const char* pszName);
};  // class FontNameSet


// --------------------------------------------------------------------------

FontName::FontName(const char* pszName, FontNameSet* pfnmsetMyOwner,
        unsigned int n) :
    CSetEl(pszName),
    m_n(n),
    m_pfnmsetMyOwner(pfnmsetMyOwner)
{
    ASSERT( m_pfnmsetMyOwner );
}

void FontName::WriteDefinition(ostream& ios) const
{
    ios << "{\\f" << m_n << ' ' << sName() << ";}\n";
}

void FontName::AppendFormattingTo(Str8& sFormatting) const
{
    char pszFontNameFormatting[8];  // \fUUUUU\0: 2 + 5 + 1
    sprintf(pszFontNameFormatting, "\\f%u", m_n);
    sFormatting += pszFontNameFormatting;
}

CSet* FontName::psetMyOwner()
{
    return m_pfnmsetMyOwner;
}


// --------------------------------------------------------------------------

FontNameSet::FontNameSet(const char* pszDefaultFontName) :
    m_nNext(s_nDefaultFont)
{
    (void) pfnmSearch_AddIfNew(pszDefaultFontName);
}

void FontNameSet::WriteDefinition(ostream& ios) const
{
    ios << "{\\fonttbl\n";
    FontName* pfnm = pfnmFirst();
    for ( ; pfnm; pfnm = pfnmNext(pfnm) )
        pfnm->WriteDefinition(ios);
    ios << "}\n";
}

FontName* FontNameSet::pfnmSearch_AddIfNew(const char* pszName)
{
    FontName* pfnm = pfnmSearch(pszName);
    if ( !pfnm )
        {
        pfnm = new FontName(pszName, this, m_nNext++);
        Add(pfnm);
        }
    
    return pfnm;
}


// ==========================================================================

// Each Style is a descriptive tag for a distinct kind of functional element
// in the document. It specifies a set of character, paragraph, or section
// formatting properties to be applied to text so marked.
// A Style is referred to by number elsewhere in the document.

class Style : public CSetEl  // Hungarian: sty
{
protected:
    unsigned int m_n;
    Str8 m_sFormatting;
    
    friend class StyleSet;
    StyleSet* m_pstysetMyOwner;
    
    Style(const char* pszName, StyleSet* pstysetMyOwner,
            const char* pszFormatting = "", unsigned int n = 0);
    void SetNumber(unsigned int n) { m_n = n; }

    void WriteBasedOn(ostream& ios) const;
    void AppendFormattingTo(const CFont* pfnt, Str8& sFormatting);
    
    virtual void WriteFormatting(ostream& ios) const = 0;

public:
    virtual void WriteDefinition(ostream& ios) const = 0;
    virtual void Write(ostream& ios) const = 0;
    virtual void WriteField(RTF_ostream& sfs, const CField* pfld) const = 0;
    
    virtual CSet* psetMyOwner();    
};  // class Style


// --------------------------------------------------------------------------

class CharStyle : public Style  // Hungarian: sty
{
private:
    const CharStyle* m_pstyBasedOn;

    friend class StyleSet;
    CharStyle(const char* pszName, StyleSet* pstysetMyOwner, const CFont* pfnt,
            const CharStyle* pstyBasedOn = NULL, unsigned int n = 0);

    virtual void WriteFormatting(ostream& ios) const;

public:
    virtual void WriteDefinition(ostream& ios) const;
    virtual void Write(ostream& ios) const;
    virtual void WriteField(RTF_ostream& sfs, const CField* pfld) const;
};  // class CharStyle


// --------------------------------------------------------------------------

class StandardCharStyle : public Style  // Hungarian: sty
{
private:
    const StandardCharStyle* m_pstyBasedOn;

    friend class StyleSet;
    StandardCharStyle(const char* pszName, StyleSet* pstysetMyOwner,
        const StandardCharStyle* pstyBasedOn,
        const char* pszFormatting = "", unsigned int n = 0);
            
    void Construct(const CFont* pfnt);

    virtual void WriteFormatting(ostream& ios) const;

public:
    virtual void WriteDefinition(ostream& ios) const;
    virtual void Write(ostream& ios) const;
    virtual void WriteField(RTF_ostream& sfs, const CField* pfld) const;
};  // class StandardCharStyle


// --------------------------------------------------------------------------

class ParStyle : public Style  // Hungarian: sty
{
private:
    friend class StyleSet;
    ParStyle(const char* pszName, StyleSet* pstysetMyOwner,
            const char* pszFormatting = "", unsigned int n = 0);
    ParStyle(const char* pszName, StyleSet* pstysetMyOwner,
            const CFont* pfnt, unsigned int n = 0);

    virtual void WriteFormatting(ostream& ios) const;

public:
    virtual void WriteDefinition(ostream& ios) const;
    virtual void Write(ostream& ios) const;
    virtual void WriteField(RTF_ostream& sfs, const CField* pfld) const;
};  // class ParStyle


// --------------------------------------------------------------------------

// The StyleSet corresponds to the {\stylesheet ...} group.
// In this conversion mechanism, each Standard Format marker
// corresponds to a unique character style.
// The stylesheet also includes standard styles found in Microsoft Word's
// Document Templates, such as Normal, Footer, Footnote Text, etc.

class MarkerStyleRefSet;
class MarkerStyleRef;

class StyleSet : public CSet  // Hungarian: styset
{
private:
    BOOL m_bMarkerFontFormatting;
    unsigned int m_nNext;
    FontNameSet m_fnmset;
    MarkerStyleRefSet* m_psrfsetMarker;
    
    ParStyle* m_pstyNormal;
    StandardCharStyle* m_pstyDefaultParagraphFont;
    ParStyle* m_pstyHeader;
    ParStyle* m_pstyFooter;
    ParStyle* m_pstyFootnoteText;
    StandardCharStyle* m_pstyFootnoteReference;

    void AddStandardStyles(BOOL bHeaderAndFooter); // 1.2dj On RTF export, if template attached, don't define header and footer styles
    void AddUserStyles(const CMarkerSet* pmkrset);
    BOOL bMakeValidUniqueName(Str8& sName, CNote** ppnot);
    
public:
    StyleSet(CMarkerSet* pmkrset, BOOL bMarkerFontFormatting, BOOL bHeaderAndFooter); // 1.2dj On RTF export, if template attached, don't define header and footer styles
    ~StyleSet();
    
    void WriteDefinition(ostream& ios);

    BOOL bMarkerFontFormatting() const { return m_bMarkerFontFormatting; }
    const FontNameSet* pfnmset() const { return &m_fnmset; }
    FontNameSet* pfnmset() { return &m_fnmset; }
    
    const ParStyle* pstyNormal() const { return m_pstyNormal; }
    const StandardCharStyle* pstyDefaultParagraphFont() const
        { return m_pstyDefaultParagraphFont; }
    const ParStyle* pstyHeader() const { return m_pstyHeader; }
    const ParStyle* pstyFooter() const { return m_pstyFooter; }
    const ParStyle* pstyFootnoteText() const { return m_pstyFootnoteText; }
    const StandardCharStyle* pstyFootnoteReference() const
        { return m_pstyFootnoteReference; }
    
    Style* pstyFirst() const
            { return (Style*)pelFirst(); }
    Style* pstyLast() const
            { return (Style*)pelLast(); }
    Style* pstyNext(const Style* pstyCur) const
            { return (Style*)pelNext(pstyCur); }
    Style* pstyPrev(const Style* pstyCur) const
            { return (Style*)pelPrev(pstyCur); }
    
    MarkerStyleRef* psrfSearch_AddIfNew(CMarker* pmkr);
    MarkerStyleRef* psrfSearch(CMarker* pmkr);
    MarkerStyleRef* psrfSearchForWholeSubString(const char* pszName,
            int lenName) const;
};  // StyleSet


// --------------------------------------------------------------------------


Style::Style(const char* pszName, StyleSet* pstysetMyOwner,
        const char* pszFormatting, unsigned int n) :
    CSetEl(pszName),
    m_n(n),
    m_sFormatting(pszFormatting),
    m_pstysetMyOwner(pstysetMyOwner)
{
}

void Style::WriteBasedOn(ostream& ios) const
{
    if ( !m_pstysetMyOwner->bMarkerFontFormatting() )
        return;
        
    ios << "\\sbasedon" << m_n;
}

CSet* Style::psetMyOwner()
{
    return m_pstysetMyOwner;
}

void Style::AppendFormattingTo(const CFont* pfnt, Str8& sFormatting)
{
    ASSERT( pfnt );
    CFontDef fntdef((CFont*)pfnt);  // Cast away const-ness
    LOGFONT lf = *fntdef.plf();

    if ( lf.lfWeight == FW_BOLD )
        sFormatting += "\\b";
    if ( lf.lfItalic )
        sFormatting += "\\i";
    if ( lf.lfStrikeOut )
        sFormatting += "\\strike";
    if ( lf.lfUnderline )
        sFormatting += "\\ul";

	CString sw = lf.lfFaceName; // 1.4qum
	Str8 s = sUTF8( sw ); // 1.4qum
    const FontName* pfnm = m_pstysetMyOwner->pfnmset()->pfnmSearch_AddIfNew( s ); // 1.4qum Upgrade pfnmSearch_AddIfNew for Unicode build
    ASSERT( pfnm );
    pfnm->AppendFormattingTo(sFormatting);

#if defined(TLB_07_18_2000_FONT_HANDLING_BROKEN)
//    int sizeInPoints = ((-lf.lfHeight)*3+1)/4;  // Pixels to points
#elif defined(_MAC)
    int sizeInPoints = -lf.lfHeight;  // Pixels to points
#else
    int sizeInPoints = CFontDef::s_iConvertPixelsToPoints(-lf.lfHeight);
#endif
    char pszSize[9];  // 3 + 5 + 1
    char* pszSizeFormat = "\\fs%u";
    sprintf(pszSize, pszSizeFormat, 2*sizeInPoints);  // in HALF-points
    sFormatting += pszSize;
}

// --------------------------------------------------------------------------

CharStyle::CharStyle(const char* pszName, StyleSet* pstysetMyOwner,
        const CFont* pfnt, const CharStyle* pstyBasedOn, unsigned int n) :
    Style(pszName, pstysetMyOwner, "", n),
    m_pstyBasedOn(pstyBasedOn)
{
    if ( !m_pstyBasedOn )
        AppendFormattingTo(pfnt, m_sFormatting);
}

void CharStyle::WriteDefinition(ostream& ios) const
{
    ios << "{\\*\\cs" << m_n << " \\additive";
    if ( m_pstyBasedOn )
        m_pstyBasedOn->WriteBasedOn(ios);
    else
        WriteFormatting(ios);
    ASSERT( !sName().IsEmpty() );
    ios << ' ' << sName() << ";}\n";
}

void CharStyle::Write(ostream& ios) const
{
    ios << "\\cs" << m_n;
    if ( m_pstyBasedOn )
        m_pstyBasedOn->WriteFormatting(ios);
    else
        WriteFormatting(ios);
    ios << ' ';
}

void CharStyle::WriteFormatting(ostream& ios) const
{
    ASSERT( !m_pstyBasedOn );
    if ( !m_pstysetMyOwner->bMarkerFontFormatting() )
        return;

    ios << m_sFormatting;
}

void CharStyle::WriteField(RTF_ostream& sfs, const CField* pfld) const
{
    sfs.WriteFieldAsCharStyle(pfld, this);
}


// --------------------------------------------------------------------------

StandardCharStyle::StandardCharStyle(const char* pszName,
        StyleSet* pstysetMyOwner,
        const StandardCharStyle* pstyBasedOn, const char* pszFormatting,
        unsigned int n) :
    Style(pszName, pstysetMyOwner, pszFormatting, n),
    m_pstyBasedOn(pstyBasedOn){
}

void StandardCharStyle::WriteDefinition(ostream& ios) const
{
    ios << "{\\*\\cs" << m_n << " \\additive";
    WriteFormatting(ios);
    if ( m_pstyBasedOn )
        m_pstyBasedOn->WriteBasedOn(ios);
    ASSERT( !sName().IsEmpty() );
    ios << ' ' << sName() << ";}\n";
}

void StandardCharStyle::Write(ostream& ios) const
{
    ios << "\\cs" << m_n;
    if ( m_pstyBasedOn )
        m_pstyBasedOn->WriteFormatting(ios);
    WriteFormatting(ios);
    ios << ' ';
}

void StandardCharStyle::WriteFormatting(ostream& ios) const
{
    if ( !m_pstysetMyOwner->bMarkerFontFormatting() )
        return;

    ios << m_sFormatting;       
}

void StandardCharStyle::WriteField(RTF_ostream& sfs, const CField* pfld) const
{
    ASSERT( FALSE );
}


// --------------------------------------------------------------------------

ParStyle::ParStyle(const char* pszName, StyleSet* pstysetMyOwner,
        const char* pszFormatting, unsigned int n) :
    Style(pszName, pstysetMyOwner, pszFormatting, n)
{
    m_sFormatting += s_pszDefaultFontAndSize;
}

ParStyle::ParStyle(const char* pszName, StyleSet* pstysetMyOwner,
        const CFont* pfnt, unsigned int n) :
    Style(pszName, pstysetMyOwner, "", n)
{
    AppendFormattingTo(pfnt, m_sFormatting);
}

void ParStyle::WriteDefinition(ostream& ios) const
{
    ios << "{";
    if ( m_n != 0 )
        ios << "\\s" << m_n;
    WriteFormatting(ios);
    if ( m_n != 0 )
        ios << "\\sbasedon0";
    ios << "\\snext" << m_n;
    ASSERT( !sName().IsEmpty() );
    ios << ' ' << sName() << ";}\n";
}

void ParStyle::Write(ostream& ios) const
{
    ios << "\\pard\\plain";
    if ( m_n != 0 )
        ios << "\\s" << m_n;
    WriteFormatting(ios);
    ios << ' ';
}

void ParStyle::WriteFormatting(ostream& ios) const
{
    if ( m_pstysetMyOwner->bMarkerFontFormatting() )
        ios << m_sFormatting;
    else
        ios << s_pszDefaultFontAndSize;
}

void ParStyle::WriteField(RTF_ostream& sfs, const CField* pfld) const
{
    sfs.WriteFieldAsParStyle(pfld, this);
}


// ==========================================================================

// 1998-07-06 MRP: Adding character styles for language encodings
// didn't turn out to be helpful, so that feature has been removed.
// The logic for based-on style relationships can now be removed.

class MarkerStyleRef : public CSetEl  // Hungarian: srf
{
private:
    CMarker* m_pmkr;
    Style* m_psty;
    
    friend class MarkerStyleRefSet;
    MarkerStyleRefSet* m_psrfsetMyOwner;
    
    MarkerStyleRef(CMarker* pmkr, Style* psty,
            MarkerStyleRefSet* psrfsetMyOwner);
    
public:
    CMarker* pmkr() const { return m_pmkr; }
    Style* psty() const { return m_psty; }

    virtual CSet* psetMyOwner();
};  // MarkerStyleRef


// --------------------------------------------------------------------------

class MarkerStyleRefSet : public CSet // CFastSet  // Hungarian: srfset
{
public:
    MarkerStyleRef* psrfSearch(const char* pszName) const
        { return (MarkerStyleRef*)pselSearch(pszName); }
    MarkerStyleRef* psrfSearchForWholeSubString(const char* pszName,
            int lenName) const
        { return (MarkerStyleRef*)pselSearchForWholeSubString(pszName, lenName); }
            
    MarkerStyleRef* psrfAddMarkerStyleRef(CMarker* pmkr, Style* psty);
};  // MarkerStyleRefSet


// --------------------------------------------------------------------------

MarkerStyleRef::MarkerStyleRef(CMarker* pmkr, Style* psty,
        MarkerStyleRefSet* psrfsetMyOwner) :
    CSetEl(pmkr->sName()),
    m_pmkr(pmkr),
    m_psty(psty),
    m_psrfsetMyOwner(psrfsetMyOwner)
{
    ASSERT( m_pmkr );
    ASSERT( m_psty );
    ASSERT( m_psrfsetMyOwner );
}

CSet* MarkerStyleRef::psetMyOwner()
{
    return m_psrfsetMyOwner;
}


MarkerStyleRef* MarkerStyleRefSet::psrfAddMarkerStyleRef(CMarker* pmkr, Style* psty)
{
    MarkerStyleRef* psrf = new MarkerStyleRef(pmkr, psty, this);
    Add(psrf);
    return psrf;
}


// ==========================================================================


StyleSet::StyleSet(CMarkerSet* pmkrset, BOOL bMarkerFontFormatting, BOOL bHeaderAndFooter) :
    m_bMarkerFontFormatting(bMarkerFontFormatting),
    m_nNext(10),  // Number of Default Paragraph Font
    m_fnmset(s_pszDefaultFontName),
    m_psrfsetMarker(new MarkerStyleRefSet)
{
    // Add some styles that are standard in Word Document Templates
    AddStandardStyles(bHeaderAndFooter); // 1.2dj On RTF export, if template attached, don't define header and footer styles

    // Add styles representing this Database Type's languages and markers
    AddUserStyles(pmkrset);   
}

StyleSet::~StyleSet()
{
    delete m_psrfsetMarker;
}


// Rules for style names [Microsoft Word User's Guide, v6, p.191.]
// Maximum number of characters, including spaces, commas, and aliases: 253.
// Characters not allowed: backslash, braces, semicolon; nor colon, if Mac.
// Note that style names are case-sensitive.
// Note that Microsoft Word for Macintosh interprets commas
// in the style name as separating style synonyms.

void StyleSet::AddStandardStyles(BOOL bHeaderAndFooter) // 1.2dj On RTF export, if template attached, don't define header and footer styles
{
    m_pstyNormal = new ParStyle("Normal", this, "", 0);
    Add(m_pstyNormal);
    
    m_pstyDefaultParagraphFont =
        new StandardCharStyle("Default Paragraph Font", this,
        NULL, "", m_nNext++);
    Add(m_pstyDefaultParagraphFont);
    
	m_pstyHeader = new ParStyle("header", this, s_pszHeaderFooterFormatting);
	m_pstyFooter = new ParStyle("footer", this, s_pszHeaderFooterFormatting);
    
	if ( bHeaderAndFooter ) // 1.2dj On RTF export, if template attached, don't define header and footer styles
		{
		Add(m_pstyHeader);
		Add(m_pstyFooter);
		}
    
    m_pstyFootnoteText = new ParStyle("footnote text", this);
    Add(m_pstyFootnoteText);
    
    m_pstyFootnoteReference =
        new StandardCharStyle("footnote reference", this,
        m_pstyDefaultParagraphFont, "\\super");
    Add(m_pstyFootnoteReference);
}

void StyleSet::AddUserStyles(const CMarkerSet* pmkrset)
{
    ASSERT( pmkrset );

    // Add a character style for every marker
    CMarker* pmkr = pmkrset->pmkrFirst();
    for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) )
        (void) psrfSearch_AddIfNew(pmkr);
        
    // Number the styles after they have all been added,
    // so that they will appear in both alphabetical and numeric order.
    Style* psty = pstyFirst();
    for ( ; psty; psty = pstyNext(psty) )
        if ( psty != m_pstyNormal && psty != m_pstyDefaultParagraphFont)
            psty->SetNumber(m_nNext++);
}

static const char* s_pszInvalidChars = "\\{};:";

BOOL StyleSet::bMakeValidUniqueName(Str8& sName, CNote** ppnot)
{
    BOOL bMadeValidUnique = FALSE;
    const char* psz = sName;
    if ( strcspn(psz, s_pszInvalidChars) != strlen(psz) )
        {
        Str8 sValidName;
        for ( ; *psz; psz++ )
            if ( !strchr(s_pszInvalidChars, *psz) )
                sValidName += *psz;
        sName = sValidName;
        bMadeValidUnique = TRUE;
        }

    if ( sName.GetLength() == 0 )
        {
        sName = "Unnamed";
        bMadeValidUnique = TRUE;
        }
    
    const maxlen = 250;
    if ( maxlen < sName.GetLength() )
        {
        sName = sName.Left(maxlen);
        bMadeValidUnique = TRUE;
        }
    
    Str8 sUnique = sUniqueName(sName);
    if ( !bEqual(sUnique, sName) )
        {
        sName = sUnique;
        bMadeValidUnique = TRUE;
        }
        
    return bMadeValidUnique;
}

void StyleSet::WriteDefinition(ostream& ios)
{
    pfnmset()->WriteDefinition(ios);
    ios << "{\\stylesheet\n";
    m_pstyNormal->WriteDefinition(ios);
    m_pstyDefaultParagraphFont->WriteDefinition(ios);
    Style* psty = pstyFirst();
    for ( ; psty; psty = pstyNext(psty) )
        if ( psty != m_pstyNormal && psty != m_pstyDefaultParagraphFont )
            psty->WriteDefinition(ios);
    ios << "}\n";
}

MarkerStyleRef* StyleSet::psrfSearch_AddIfNew(CMarker* pmkr)
{
    ASSERT( pmkr );
    MarkerStyleRef* psrf = m_psrfsetMarker->psrfSearch(pmkr->sName());
    if ( !psrf )
        {
        Str8 sName = pmkr->sFieldName();
        if ( sName.IsEmpty() || bEqual(sName, "*") )
            sName = pmkr->sMarker();
        CNote* pnot = NULL;
        (void) bMakeValidUniqueName(sName, &pnot);
        Style* psty = NULL;
        if ( pmkr->bCharStyle() )
            psty = new CharStyle(sName, this, pmkr->pfnt());
        else
            psty = new ParStyle(sName, this, pmkr->pfnt());
        Add(psty);
        psrf = m_psrfsetMarker->psrfAddMarkerStyleRef(pmkr, psty);
        }

    return psrf;
}

MarkerStyleRef* StyleSet::psrfSearch(CMarker* pmkr)
{
    ASSERT( pmkr );
    return m_psrfsetMarker->psrfSearch(pmkr->sName());
}

MarkerStyleRef* StyleSet::psrfSearchForWholeSubString(const char* pszName,
        int lenName) const
{
    return m_psrfsetMarker->psrfSearchForWholeSubString(pszName, lenName);
}


// ==========================================================================

static const int s_maxlenRenBuf = 30000;  // 1998-03-03 MRP

RTF_ostream::RTF_ostream(ostream& ios, CMarkerSet* pmkrset,
        BOOL bAutoUpdateStyles, BOOL bMarkerFontFormatting,
        BOOL bInterlinearTabAligned, RtfTwips twxInterlinearSpacing,
        const Str8& sHeaderLeft, const Str8& sHeaderRight,
        const Str8& sFooterCenter, BOOL bFooterInsideDate,
        const Str8& sDocumentTemplatePath,
        const RtfPageSetup* ppgs) :
    Field_ostream(ios, FALSE),
    m_bAutoUpdateStyles(bAutoUpdateStyles),
    m_bInterlinearTabAligned(bInterlinearTabAligned),
    m_twxInterlinearSpacing(twxInterlinearSpacing),
    m_sHeaderLeft(sHeaderLeft),
    m_sHeaderRight(sHeaderRight),
    m_sFooterCenter(sFooterCenter),
    m_bFooterInsideDate(bFooterInsideDate),
    m_sDocumentTemplatePath(sDocumentTemplatePath),
    m_ppgs(ppgs)
{
    m_bEmbeddedMarkup = TRUE;
    m_bMarkerFontFormatting = bMarkerFontFormatting;
    m_bHaveWrittenField = FALSE;
    m_iSectAfter = 0;
    m_bPar = FALSE;
    m_bCharInThisPar = FALSE;
    m_bWhiteSpace = FALSE;
    ASSERT( m_ppgs );
    
    m_pstyset = new StyleSet(pmkrset, m_bMarkerFontFormatting, TRUE); // , m_sDocumentTemplatePath.IsEmpty()); // 1.2dk Disable not writing header and footer if template attached, it didn't work // 1.2dj On RTF export, if template attached, don't define header and footer styles
    m_pchRenBuf = new char[s_maxlenRenBuf + 1];  // 1998-03-03 MRP

    // 1998-07-11 MRP
    m_bInterlinearRtl = FALSE;
    if ( m_bInterlinearTabAligned )
        {
        BOOL bInterlinear = FALSE;
        BOOL bInterlinearNonParStyle = FALSE;
        ASSERT( pmkrset );
        const CMarker* pmkr = pmkrset->pmkrFirst();
        for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) )
            if ( pmkr->bInterlinear() )
                {
                bInterlinear = TRUE;
                if ( pmkr->bFirstInterlinear() )
                    m_bInterlinearRtl = pmkr->plng()->bRightToLeft();
                if ( !pmkr->bParStyle() )
                    bInterlinearNonParStyle = TRUE;
                }

        // Prerequisites: The database type must have interlinear markers,
        // all of which must be exported as paragraph styles.
        if ( !bInterlinear || bInterlinearNonParStyle )
            m_bInterlinearTabAligned = FALSE;

        // Valid range of values: 24 per inch to 6 per inch.
        if ( m_twxInterlinearSpacing < 20 || 2000 < m_twxInterlinearSpacing ) // 6.0y Increase range of possible values
            m_twxInterlinearSpacing = 120;  // Default value: 12 per inch
        }
    WriteHead( pmkrset->bUnicodeLang() ); // Use Unicode in header and footer if any lang uses it
}

RTF_ostream::~RTF_ostream()
{
    WriteTail();
    delete m_pstyset;
    delete m_pchRenBuf;
}


// A file or stream of RTF consists of "plain text" interspersed with
// control symbols, control words, and braces. The control words,
// control symbols, and braces constitute control information.
// All other characters in the file are plain text.
// [Rich Text Format (RTF Version 1.3) Specification.]

void RTF_ostream::WriteHead(BOOL bUnicode)
{
    // Write the RTF header.
    
    // An entire RTF file is considered a group, so is enclosed in braces.
    // The control word \rtfN must (immediately) follow the opening brace.
    m_ios << "{\\rtf1";
    m_ios << s_pszCharSet;
    // The default font specification must precede the font table group.
    m_ios << "\\deff0";
    m_ios << "\\deflang1033\n";  // English (U.S.) 0x0409
    m_ios << "\\uc0\n"; // 1.4ywk Change RTF export to only write \uc0 once
    
    // (Note: the \filtbl and \colortbl groups would go between
    // the \fonttbl and \stylesheet groups.) 
    m_pstyset->WriteDefinition(m_ios);  // The \fonttbl and \stylesheet groups
    
    // The document area
    // The \info group would go here
    // Document formatting properties, such as margins and footnote placement.
    m_ppgs->WriteDocumentFormatting(m_ios);

    // 1996-05-20 MRP: Without this WinWord 6.0 read footnotes as endnotes
    m_ios << "\\ftnbj";  // Footnotes at the bottom of the page
    m_ios << "\\ftnnalc";  // Footnote numbering alphabetic lowercase
    m_ios << "\n";
    
    if ( !m_sDocumentTemplatePath.IsEmpty() )
        {
        m_ios << "{\\*\\template ";
        WriteString(m_sDocumentTemplatePath, FALSE);  // Escapes backslashes in path
        m_ios << '}';
        // 1997-11-24 MRP: Word 6 for Windows and Macintosh
        // require that the \linkstyles control word FOLLOW
        // the template group to "Automatically Update Document Styles".
        // Word 7 and 8 do it correctly no matter where it occurs.
        if ( m_bAutoUpdateStyles )
            m_ios << "\\linkstyles";
        m_ios << '\n';
        }

    // The document's first section
    // The section break control words (those beginning with \sbk)
    // describe the preceding break. In contrast, the other control words
    // specify section formatting properties which apply to the text
    // that follows. If the \sectd control word is not present,
    // the current section inherits all section properties defined in
    // the previous section.
    m_ppgs->WriteSectionFormatting(m_ios);
//    if ( m_sDocumentTemplatePath.IsEmpty() ) // 1.2dk Disable not writing header and footer if template attached, it didn't work // 1.2cd In RTF export don't output header and footer if template attached
	    WriteHeadersAndFooters(bUnicode);
}  // RTF_ostream::WriteHead()

void RTF_ostream::WriteTail()
{
    WriteParEnd();
    // An entire RTF file is considered a group, so is enclosed in braces.
    m_ios << "}";
}


void RTF_ostream::WriteHeadersAndFooters(BOOL bUnicode)
{
    // Each section in the document can have its own set of
    // headers and footers. If no headers or footers are defined
    // for the current section, it uses the headers and footers (if any)
    // from the previous section.
    // 1998-10-14 MRP: First page and odd/even header and footer options.
    if ( m_ppgs->bFirstPage() )
    	{
    	WriteHeader("headerf", bUnicode);
    	WriteFooter("footerf", bUnicode, TRUE);
    	}
    if ( m_ppgs->bOddAndEven() )
    	{
    	WriteHeader("headerl", bUnicode);
    	WriteFooter("footerl", bUnicode, FALSE);
    	WriteHeader("headerr", bUnicode);
    	WriteFooter("footerr", bUnicode, TRUE);
    	}
    else
    	{
    	WriteHeader("header", bUnicode);
    	WriteFooter("footer", bUnicode, TRUE);
    	}

    m_bWhiteSpace = FALSE;  // Note: WriteTab() sets it TRUE.
}

void RTF_ostream::WriteHeader(const char* pszControlWord, BOOL bUnicode)
{
	ASSERT( pszControlWord );
	ASSERT( *pszControlWord );
    m_ios << "{\\" << pszControlWord << ' ';
    m_pstyset->pstyHeader()->Write(m_ios);
    // Headers on left (even) and right (odd) pages are identical
    WriteString(m_sHeaderLeft, bUnicode, TRUE);
    WriteTab();
    WriteTab();
    WriteString(m_sHeaderRight, bUnicode, TRUE);
    m_ios << "\\par}\n";
}

void RTF_ostream::WriteFooter(const char* pszControlWord, BOOL bUnicode, BOOL bFirstOrOdd)
{
	ASSERT( pszControlWord );
	ASSERT( *pszControlWord );
    m_ios << "{\\" << pszControlWord << ' ';
    m_pstyset->pstyFooter()->Write(m_ios);
    if ( bFirstOrOdd )
    	{
	    if ( m_bFooterInsideDate )
	        WriteString(s_pszFooterInside, bUnicode, TRUE);
	    WriteTab();
	    WriteString(m_sFooterCenter, bUnicode, TRUE);
	    WriteTab();
	    WriteString(s_pszFooterOutside, bUnicode, TRUE);
	    m_ios << "\\par}\n";
    	}
    else
    	{
	    WriteString(s_pszFooterOutside, bUnicode, TRUE);
	    WriteTab();
	    WriteString(m_sFooterCenter, bUnicode, TRUE);
	    WriteTab();
	    if ( m_bFooterInsideDate )
	        WriteString(s_pszFooterInside, bUnicode, TRUE);
	    m_ios << "\\par}\n";
    	}
}

void RTF_ostream::WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst) // 1.1cm Write alphabetic divider
	{
	CMarker* pmkr = ptyp->pmkrset()->pmkrSearch("zzzAlphDiv"); // 1.1ed Change to capital letters
	if ( !pmkr) 
		{
		pmkr = ptyp->pmkrset()->pmkrAdd_MarkAsNew("zzzAlphDiv");
	    CNote* pnot = NULL;
		pmkr->bSetFieldName("Alphabetic Divider", &pnot);
		}
    MarkerStyleRef* psrf = m_pstyset->psrfSearch_AddIfNew((CMarker*)pmkr);
    ASSERT( psrf );
	int iCols = ppgsRtfPageSetup()->numColumns(); // 1.2dr On export rtf alphabetic dividers, write column change information
	double unxColWid = ppgsRtfPageSetup()->unxColWidth();
	m_ios << "\n\\par"; // 1.2ea Fix bug (1.2e) of no par before alph div in single column
	if ( iCols > 1 ) // 1.2dr 
		{
		if ( !bFirst )
			{
			m_ios << "\n\\sect";
			}
		m_ios << "\n\\sectd\\sbknone\\cols1\n";
		m_ios << "\\headery" << ppgsRtfPageSetup()->s_twFromRtfUnits( ppgsRtfPageSetup()->unyHeaderToEdge(), ppgsRtfPageSetup()->bCentimeters() ) << '\n'; // 1.3be Fix bug of not getting header and footer spacing right on first page of rtf export
		m_ios << "\\footery" << ppgsRtfPageSetup()->s_twFromRtfUnits( ppgsRtfPageSetup()->unyFooterToEdge(), ppgsRtfPageSetup()->bCentimeters() ) << '\n'; // 1.3be
		}
	m_bPar = FALSE; // 1.2ds Prevent par in wrong place
	CField fld(pmkr, sAlphLet);
	WriteField(&fld, TRUE); // 1.2dr Don't write paragraph mark before field // 1.1cn Write RTF alphabetic divider
	m_ios << "\n\\par"; // 1.2dr Write paragraph mark after field
	m_bPar = FALSE; // 1.2dr Prevent par in wrong place
	if ( iCols > 1 ) // 1.2dr 
		{
		m_ios << "\n\\sect\n";
		ppgsRtfPageSetup()->WriteSectionFormatting( m_ios, iCols ); // 1.2ha Change to write section formatting so that linebreaks will go out
/*		BOOL bCentimeters = ppgsRtfPageSetup()->bCentimeters();
		m_ios << "\\sectd\\sbknone\\cols";
		m_ios << iCols;
		m_ios << "\\colsx";
		m_ios << ppgsRtfPageSetup()->s_twFromRtfUnits(unxColWid, bCentimeters ); 
		m_ios << "\n"; */
		}
	}

void RTF_ostream::WriteMultipleColumnEnd() // 1.2du On rtf export double column, even up column ends on last page
	{
	if ( ppgsRtfPageSetup()->numColumns() > 1 ) // 1.2du For rtf export double column, even up columns on last page
		{
		m_ios << "\n\\par"; // 1.2dv For rtf export double column, put par before final output
		m_ios << "\n\\sect";
		m_ios << "\n\\sectd\\sbknone";
		m_ios << "\n\\pard\\plain\\s0\\f0\\fs20\n";
//		m_ios << "\n\\par\n";
		}
	}

void RTF_ostream::WriteField(const CField* pfld, BOOL bFirstField)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();
    
    if ( bFirstField && !pmkr->bParStyle() )
        WriteParBegin(m_pstyset->pstyNormal());
    
    MarkerStyleRef* psrf = m_pstyset->psrfSearch_AddIfNew((CMarker*)pmkr);
    ASSERT( psrf );
    const Style* psty = psrf->psty();
    psty->WriteField(*this, pfld);
    // Calls either WriteFieldAsCharStyle or WriteFieldAsParStyle below.

    m_bHaveWrittenField = TRUE;
    m_iSectAfter = pmkr->iSect();
}

void RTF_ostream::WriteFieldAsCharStyle(const CField* pfld,
        const CharStyle* psty)
{
    ASSERT( pfld );
    ASSERT( psty );
    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;

    if ( m_bCharInThisPar && !m_bWhiteSpace )
		WriteSpace( pfld->plng()->bUnicodeLang() ); // 1.4ywj Fix U bug of no space between fields on RTF export 
//        m_ios << ' ';  // Separate from preceding character style // 1.4ywj 

    const CMarker* pmkr = pfld->pmkr();     
    WriteFieldContents(pch, pchEnd, pmkr, psty);  // 1998-07-02 MRP
    m_bCharInThisPar = TRUE;
}

void RTF_ostream::WriteFieldAsParStyle(const CField* pfld,
        const ParStyle* psty)
{
    ASSERT( pfld );
    ASSERT( psty );
    WriteParBegin(psty);

    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;
        
    const CMarker* pmkr = pfld->pmkr();
    if ( m_bInterlinearTabAligned && pmkr->bInterlinear() )
        WriteTabAlignedFieldContents(pch, pchEnd, pmkr);  // 1998-07-11 MRP
    else
        WriteFieldContents(pch, pchEnd, pmkr, NULL);  // 1998-07-02 MRP
    m_bCharInThisPar = TRUE;
}

BOOL RTF_ostream::bTrimmedFieldContents(const CField* pfld,
        const char** ppchBegin, const char** ppchEnd)
{
    ASSERT( pfld );
    ASSERT( ppchBegin );
    ASSERT( ppchEnd );
    if ( pfld->sContents().IsEmpty() )
        return FALSE;

    const char* psz = pfld->sContents();  // 1998-03-03 MRP
    int len = strlen(psz);
    const CMarker* pmkr = pfld->pmkr();
    if ( !pmkr->bNoWordWrap() )  // If this is a wrapped field
        {
        // Skip preceding and following white space
        int lenPrecedingWS = 0;
        int lenFollowingWS = 0;
        if ( Shw_bNeedsToBeTrimmed(psz, &lenPrecedingWS, &lenFollowingWS) )
            {
            psz += lenPrecedingWS;
            len -= (lenPrecedingWS + lenFollowingWS);
            }
        }
        
    if ( len == 0 )
        return FALSE;
        
    *ppchBegin = psz;
    *ppchEnd = psz + len;
    return TRUE;
}


void RTF_ostream::WriteTabAlignedFieldContents(const char* pchBegin,
        const char* pchEnd, const CMarker* pmkr)
{
    // If an aligned field contains explicit line breaks,
    // write it out line-by-line without any tab stops.
    // Because all the tab stops would be common to the entire paragraph,
    // not set individually for each line, alignment wouldn't work right.
    // Besides, such a field probably hasn't been interlinearized yet.
    if ( bWroteMultiLineFieldContents(pchBegin, pchEnd, pmkr) )
        return;

    if ( m_bInterlinearRtl )
        WriteTabAlignedFieldContentsRtl(pchBegin, pchEnd, pmkr);
    else
        WriteTabAlignedFieldContentsLtr(pchBegin, pchEnd, pmkr);
}
    
void RTF_ostream::WriteTabAlignedFieldContentsLtr(const char* pchBegin,
        const char* pchEnd, const CMarker* pmkr)
{
    ASSERT( pchBegin );
    ASSERT( pchEnd );
    ASSERT( pchBegin <= pchEnd );
    ASSERT( pmkr );
    ASSERT( pmkr->bInterlinear() );
    ASSERT( pmkr->bParStyle() );

	BOOL bUnicode = pmkr->plng()->bUnicodeLang();
    int twxTab = 0;
    BOOL bFirstItem = TRUE;

    const char* pch = pchBegin;
    while ( pch < pchEnd ) // 1.3aa Fix bug of crash on export
        {
        // Skip spaces
        while ( pch != pchEnd && *pch == ' ' )
            {
            pch++;
            twxTab += m_twxInterlinearSpacing;
            }
    
        if ( pch == pchEnd )
            break;
    
        // Write an aligned item
        if ( !bFirstItem )
            {
            WriteTab();
            m_ios << "\\tx" << twxTab << ' ';  // Tab stop
            }
        bFirstItem = FALSE;
                
        const char* pchItem = pch;
        while ( pch != pchEnd && *pch != ' ' )
            {
            ASSERT( *pch != '\n' );  // See bWroteMultiLineFieldContents
            pch++;
            twxTab += m_twxInterlinearSpacing;
            }
    
        // If pmkr's language encoding uses SDF rendering for display,
        // apply those context-sensitive rules in the RTF output.
        // Right-to-left reversal of items like glosses occurs here too.
        const CLangEnc* plng = pmkr->plng();
        int lenItem = pch - pchItem;
        plng->lenRenSubstring(pchItem, lenItem, lenItem, TRUE,
            m_pchRenBuf, s_maxlenRenBuf, &pchItem, &lenItem);
        // Note: If rendering occurred, pchItem now points
        // into this object's m_pchRenBuf character buffer;
        // otherwise it is unchanged.
    
        WriteChars(pchItem, lenItem, bUnicode);
		if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
			pch = pchEnd;
        }
}

void RTF_ostream::WriteTabAlignedFieldContentsRtl(const char* pchBegin,
        const char* pchEnd, const CMarker* pmkr)
{
    ASSERT( pchBegin );
    ASSERT( pchEnd );
    ASSERT( pchBegin <= pchEnd );
    ASSERT( pmkr );
    ASSERT( pmkr->bInterlinear() );
    ASSERT( pmkr->bParStyle() );

	BOOL bUnicode = pmkr->plng()->bUnicodeLang();
    int len = pchEnd - pchBegin;
    int twxTab = m_ppgs->twxLineWidth() - len * m_twxInterlinearSpacing;

    // Process the line in reverse, from end to beginning
    pchBegin--;
    const char* pch = pchEnd - 1;
    while ( pch != pchBegin )
        {
        ASSERT( pchBegin < pch );
                
        // Skip spaces
        while ( pch != pchBegin && *pch == ' ' )
            {
            pch--;
            twxTab += m_twxInterlinearSpacing;
            }
        
        if ( pch == pchBegin )
            break;
        
        // Write an aligned item
        int lenItem = 0;
        while ( pch != pchBegin && *pch != ' ' )
            {
            ASSERT( *pch != '\n' );  // See bWroteMultiLineFieldContents
            pch--;
            lenItem++;
            twxTab += m_twxInterlinearSpacing;
            }
        
        // If pmkr's language encoding uses SDF rendering for display,
        // apply those context-sensitive rules in the RTF output.
        // Right-to-left reversal of items occurs here too.
        const CLangEnc* plng = pmkr->plng();
        const char* pchItem = pch + 1;
        plng->lenRenSubstring(pchItem, lenItem, lenItem, TRUE,
            m_pchRenBuf, s_maxlenRenBuf, &pchItem, &lenItem);
        // Note: If rendering occurred, pchItem now points
        // into this object's m_pchRenBuf character buffer;
        // otherwise it is unchanged.

        WriteTab();        
        m_ios << "\\tqr\\tx" << twxTab << ' ';  // Right-aligned tab stop
        WriteChars(pchItem, lenItem, bUnicode);
        }
}

BOOL RTF_ostream::bWroteMultiLineFieldContents(const char* pchBegin,
        const char* pchEnd, const CMarker* pmkr)
{
    ASSERT( pchBegin );
    ASSERT( pchEnd );
    ASSERT( pchBegin <= pchEnd );
    ASSERT( pmkr );
    ASSERT( pmkr->bInterlinear() );
    ASSERT( pmkr->bParStyle() );

	BOOL bUnicode = pmkr->plng()->bUnicodeLang();
    const char* pch = pchBegin;
    while ( pch != pchEnd && *pch != '\n' )
        pch++;

    if ( pch == pchEnd )  // If no embedded newlines found
        return FALSE;

    pch = pchBegin;
    while ( pch != pchEnd )
        {
        const char* pchLine = pch;
        while ( pch != pchEnd && *pch != '\n' )
            pch++;

        // If pmkr's language encoding uses SDF rendering for display,
        // apply those context-sensitive rules in the RTF output.
        // Right-to-left reversal of lines occurs here too.
        const CLangEnc* plng = pmkr->plng();
        int lenLine = pch - pchLine;
        plng->lenRenSubstring(pchLine, lenLine, lenLine, TRUE,
            m_pchRenBuf, s_maxlenRenBuf, &pchLine, &lenLine);
        // Note: If rendering occurred, pchLine now points
        // into this object's m_pchRenBuf character buffer;
        // otherwise it is unchanged.

        WriteChars(pchLine, lenLine, bUnicode);

        if ( pch != pchEnd && *pch == '\n' )
            {
            WriteLine();
            pch++;
            }
        }

    return TRUE;
}


void RTF_ostream::WriteFieldContents(const char* pch, const char* pchEnd,
        const CMarker* pmkr, const CharStyle* pstyChar)
{
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( pmkr );
    // Note: If pmkr has has Style to Export: Paragraph,
    // then pstyChar should be NULL.
    ASSERT( pstyChar || pmkr->bParStyle() );

    if ( pstyChar )
        {
        m_ios << '{';  // Open an RTF group
        pstyChar->Write(m_ios);  // Specify its character style
        }

    // Write the field's contents [pch, pchEnd).
    // Because control symbols |{...} and subfield begin |mkr{ and
    // end tags } shouldn't be processed by the rendering DLL,
    // the field must be divided into renderable substrings.
    const char* pchBegin = pch;
    while ( pch != pchEnd )
		{
        if ( bAtRawOutput(pch, pchEnd) ) // 1.4rad
			{
            WriteRawOutput(&pch, pchEnd); // 1.4rad
            pchBegin = pch; // 1.4rad
			}
        else if ( bAtControlSymbol(pch, pchEnd) ) // 1.4rad
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        else if ( bAtSubfield(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteSubfield(&pch, pchEnd, pstyChar, FALSE);
            pchBegin = pch;
            }
        else
            pch++;
		}

    WriteRenderableSubstring(pchBegin, pch, pmkr);

    if ( pstyChar )
        m_ios << '}';  // Close the RTF group
}

BOOL RTF_ostream::bAtRawOutput(const char* pch, const char* pchEnd) // 1.4rad Add bar square bracket for raw output to export
	{
	return ( pch < pchEnd - 1 && *pch == '|' && *(pch + 1) == '[' );
	}

void RTF_ostream::WriteRawOutput(const char** ppch, const char* pchEnd) // 1.4rad
	{
	for ( const char* pch = *ppch + 2; pch < pchEnd; pch++ ) // While in raw output, write it out
		{
		if ( pch < pchEnd - 1 && *pch == '|' && *(pch + 1) == ']' ) // If at end of raw output, return
			{
			pch += 2; // Move past end of raw output
			break;
			}
		m_ios << *pch; // Write next char of raw output
		}
	*ppch = pch; // Return end
	}

BOOL RTF_ostream::bAtControlSymbol(const char* pch, const char* pchEnd)
{
    const char* pchControlSymbol = NULL;
    int len = 0;
    return bControlSymbol(&pch, pchEnd, &pchControlSymbol, &len);
}

BOOL RTF_ostream::bControlSymbol(const char** ppch, const char* pchEnd,
        const char** ppchControlSymbol, int* plenControlSymbol)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( ppchControlSymbol );
    ASSERT( plenControlSymbol );

    // 1997-12-04 MRP: New notation with |{ reduces chance of confusing
    // a control symbol with ordinary text content.
    if ( pch == pchEnd || *pch++ != '|' )
        return FALSE;

    if ( pch == pchEnd || *pch++ != '{' )
        return FALSE;

    const char* pchBegin = pch;

    // For example: |{~} non-breaking space.
    if ( pch != pchEnd && __isascii(*pch) && !isalnum(*pch) &&
            pch + 1 != pchEnd && pch[1] == '}' )
        {
        pch += 2;
		if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
			pch = pchEnd;
        *ppch = pch;
        *ppchControlSymbol = pchBegin;
        *plenControlSymbol = 1;
        return TRUE; 
        }
        
    while ( pch != pchEnd && __isascii(*pch) && islower(*pch) )
        pch++;

    if ( pch == pchBegin )  // If no lower case letters
        return FALSE;

    if ( pch == pchEnd )
        return FALSE;

    // 1998-03-17 MRP: Control word can have a numeric parameter.
    if ( *pch == '-' && pch + 1 != pchEnd &&
            __isascii(pch[1]) && isdigit(pch[1]) )
        pch++;
    while ( pch != pchEnd && __isascii(*pch) && isdigit(*pch) )
        pch++;

    int len = pch - pchBegin;  // Note: Must do here before incrementing
    if ( pch == pchEnd || *pch++ != '}' )
        return FALSE;

	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    ASSERT( len != 0 );
    *ppch = pch;
    *ppchControlSymbol = pchBegin;
    *plenControlSymbol = len;
    return TRUE;
}

void RTF_ostream::WriteControlSymbol(const char** ppch, const char* pchEnd)
{
    const char* pchControlSymbol = NULL;
    int len = 0;
    BOOL b = bControlSymbol(ppch, pchEnd, &pchControlSymbol, &len);
    ASSERT( b );
    ASSERT( pchControlSymbol );
    ASSERT( len != 0 );

    m_ios << '\\';
    m_ios.write(pchControlSymbol, len);
    if ( isalnum(*pchControlSymbol) )  // 1998-07-18 MRP: If not ~ _ -
        m_ios << ' ';  // 1998-07-10 MRP: Separate it from following text

    if ( len == 1 && *pchControlSymbol == '~' )
        m_bWhiteSpace = TRUE;  // 1998-04-06 MRP: Non-breaking space
    else if ( len == 3 && strncmp(pchControlSymbol, "tab", len) == 0 )
        m_bWhiteSpace = TRUE;
    else if ( len == 7 && strncmp(pchControlSymbol, "emspace", len) == 0 )
        m_bWhiteSpace = TRUE;
    else if ( len == 7 && strncmp(pchControlSymbol, "enspace", len) == 0 )
        m_bWhiteSpace = TRUE;
    else
        m_bWhiteSpace = FALSE;  // 1998-07-25 MRP
}


BOOL RTF_ostream::bAtSubfield(const char* pch, const char* pchEnd)
{
    const CMarker* pmkr = NULL;
    const CharStyle* pstyChar = NULL;
    return bSubfieldBegin(&pch, pchEnd, &pmkr, &pstyChar);
}

BOOL RTF_ostream::bSubfieldBegin(const char** ppch, const char* pchEnd,
        const CMarker** ppmkr, const CharStyle** ppstyChar)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( ppmkr );
    ASSERT( ppstyChar );

    // 1998-07-04 MRP: Should subfields be enabled per language encoding?
    if ( !m_bEmbeddedMarkup )
        return FALSE;

    if ( pch == pchEnd || *pch++ != '|' )
        return FALSE;

    const char* pchMarker = pch;
    while ( pch != pchEnd && *pch != '{' )
        pch++;

    int lenMarker = pch - pchMarker;
    if ( pch == pchEnd || lenMarker == 0 )
        return FALSE;

    MarkerStyleRef* psrf =
        m_pstyset->psrfSearchForWholeSubString(pchMarker, lenMarker);
    if ( !psrf )  // If the marker is not in the style sheet,
        return FALSE;  // It gets put out as text

    const CMarker* pmkr = psrf->pmkr();
    ASSERT( pmkr );
    if ( !pmkr->bCharStyle() )
        return FALSE;

    CharStyle* pstyChar = (CharStyle*)psrf->psty();
    ASSERT( pstyChar );

    ASSERT( *pch == '{' );
    pch++;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
    *ppmkr = pmkr;
    *ppstyChar = pstyChar;
    return TRUE;
}

void RTF_ostream::WriteSubfield(const char** ppch, const char* pchEnd,
        const CharStyle* pstySurroundingChar, BOOL bInFootnote)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    // Note: If this is a first-level subfield in a footnote
    // or a field whose marker has Style to Export: Paragraph,
    // then pstySurroundingChar should be NULL.

    const CMarker* pmkr = NULL;
    const CharStyle* pstyChar = NULL;
    BOOL b = bSubfieldBegin(&pch, pchEnd, &pmkr, &pstyChar);
    ASSERT( b );
    ASSERT( pmkr );
    ASSERT( pstyChar );

    // Don't allow a footnote within a footnote.
    BOOL bFootnote = ( !bInFootnote && bEqual(pmkr->sMarker(), "*f") );
    // 1998-07-04 MRP: This way of doing footnotes is undocumented.
    // The long-term plan is to use a footnote reference control symbol
    // in the field with the footnote contents in following field(s).
    // The change is needed to allow multi-paragraph footnote contents.
    // We'll need to review the way CTW and SF Converter do it.

    if ( bFootnote )
        {
        m_ios << '{';
        m_pstyset->pstyFootnoteReference()->Write(m_ios);
        m_ios << "\\chftn}";
        m_ios << "{\\*\\footnote ";  // Open the RTF group
        m_pstyset->pstyFootnoteText()->Write(m_ios);
        m_ios << '{';
        m_pstyset->pstyFootnoteReference()->Write(m_ios);
        m_ios << "\\chftn}";
        // Note: Word would insert a space here before the text
        bInFootnote = TRUE;
        }
    else
        {
        if ( pstySurroundingChar )
            m_ios << '}';  // Close the surrounding RTF group
        m_ios << '{';  // Open the subfield's RTF group
        pstyChar->Write(m_ios);  // Specify its (character) style
        }

    // Write the subfield's contents.
    // Because control symbols |{...} and subfield begin |mkr{ and
    // end tags } shouldn't be processed by the rendering DLL,
    // the subfield must be divided into renderable substrings.
    const char* pchBegin = pch;
    while ( pch != pchEnd )
        if ( bAtRawOutput(pch, pchEnd) ) // 1.4rad
			{
            WriteRawOutput(&pch, pchEnd); // 1.4rad
            pchBegin = pch; // 1.4rad
			}
        else if ( bAtControlSymbol(pch, pchEnd) ) // 1.4rad
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        else if ( bAtSubfield(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            // A footnote's contents has a paragraph style.
            const CharStyle* psty = ( bFootnote ? NULL : pstyChar );    
            // Recursively embedded subfields are allowed, but the
            // RTF output consists of disjoint (i.e. "flattened,
            // not embedded) character style groups.
            WriteSubfield(&pch, pchEnd, psty, bInFootnote);
            pchBegin = pch;
            }
        else if ( *pch == '}' )  // End of the subfield
            break;
        else
            pch++;

    WriteRenderableSubstring(pchBegin, pch, pmkr);

    // Note: Even if this subfield's closing brace is missing,
    // it will automatically be closed correctly in the RTF output.
    m_ios << '}'; // Close the subfield's RTF group
    if ( !bFootnote && pstySurroundingChar )
        {
        m_ios << '{';  // Reopen the surrounding RTF group
        pstySurroundingChar->Write(m_ios);  // Specify its character style
        }

    if ( pch != pchEnd && *pch == '}' )
        pch++;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
}


void RTF_ostream::WriteRenderableSubstring(const char* pch,
        const char* pchEnd, const CMarker* pmkr)
{
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( pmkr );
    
    if ( pch == pchEnd )
        return;

    // If pmkr's language encoding uses SDF rendering for display,
    // apply those context-sensitive rules in the RTF output.
    // However, right-to-left text reversal can't occur here,
    // because line layout in the document is a prerequisite.
    const CLangEnc* plng = pmkr->plng();
	BOOL bUnicode = plng->bUnicodeLang();
    int len = pchEnd - pch;
    plng->lenRenSubstring(pch, len, len, FALSE,
        m_pchRenBuf, s_maxlenRenBuf, &pch, &len);
    pchEnd = pch + len;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    // Note: If rendering occurred, pch and pchEnd now point
    // into this object's m_pchRenBuf character buffer;
    // otherwise they are unchanged.

    // Write the substring [pch, pchEnd).
    while ( pch < pchEnd ) // 1.3aa Fix bug of crash on export
        {
        m_bWhiteSpace = FALSE;
		if ( *pch == '\n' )
            {
            if ( pmkr->bNoWordWrap() )
                {
                pch++;
                WriteLine();
                }
            else
                {
                pch++;
                // 1998-07-04 MRP: I don't remember the reason for this loop.
                while ( pch != pchEnd &&
                        (*pch == ' ' || *pch == '\n' || *pch == '\t' ) )
                    pch++;
                WriteSpace( bUnicode ); // 1.4ywj 
//				Str8 sSpace = " "; // 5.2y Fix so Unicode spaces write correctly // 1.4ywj 
//				const char* pchSpace = sSpace; // Provide a dummy pointer for WriteChar to move // 1.4ywj 
//	            WriteChar(&pchSpace, bUnicode); // 1.4ywj 
                }
            }
        else
            WriteChar(&pch, bUnicode);
		if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
			pch = pchEnd;
        }
}


void RTF_ostream::WriteString(const char* psz, BOOL bUnicode, BOOL bConvertRTFfields)
{
    ASSERT( psz );
    while ( *psz )
        if ( *psz == '{' && bConvertRTFfields && bWroteRTFfield(&psz, bUnicode) )
            ;
        else
            WriteChar(&psz, bUnicode); 
}

BOOL RTF_ostream::bWroteRTFfield(const char** ppsz, BOOL bUnicode)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );
    ASSERT( *psz == '{' );
    psz++;
    if ( !isupper(*psz) )
        return FALSE;

    m_ios << "{\\field{\\*\\fldinst ";
    while ( *psz )
        if ( *psz == '}' )
            {
            psz++;
            break;
            }
        else
            WriteChar(&psz, bUnicode);
    m_ios << "}}";
            
    *ppsz = psz;  // Points to the char after the closing brace
    return TRUE;
}


void RTF_ostream::WriteSect()
{
    m_ios << "\\sect ";
    m_ppgs->WriteSectionFormatting(m_ios, m_iSectAfter);
    m_iSectAfter = 0;
}

void RTF_ostream::WriteParBegin(const ParStyle* psty)
{
    WriteParEnd();
    ASSERT( psty );
    psty->Write(m_ios);
    m_bPar = TRUE;
    if ( m_iSectAfter )
        WriteSect();
}

void RTF_ostream::WriteParEnd()
{
    if ( m_bPar )
        m_ios << "\n\\par ";
    m_bCharInThisPar = FALSE;
    m_bWhiteSpace = FALSE;
}

void RTF_ostream::WriteTab()
{
    m_ios << "\\tab ";
    m_bWhiteSpace = TRUE;
}

void RTF_ostream::WriteLine()
{
    m_ios << "\\line\n";
    m_bWhiteSpace = TRUE;
}

void RTF_ostream::WriteSpace( BOOL bUnicode ) // 1.4ywj Pass in bool for Unicode
{
//    m_ios << ' '; // 1.4ywj 
	Str8 sSpace = " "; // 1.4ywj Fix U bug of no space between fields on RTF export
	const char* pchSpace = sSpace; // Provide a dummy pointer for WriteChar to move
	WriteChar(&pchSpace, bUnicode); // 1.4ywj 
    m_bWhiteSpace = TRUE;
}

void RTF_ostream::WriteNonBreakingSpace()
{
    m_ios << "\\~";
    m_bWhiteSpace = TRUE;
}

void RTF_ostream::WriteChar(const char** ppsz, BOOL bUnicode)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );

	int iNumBytes = 1;
    if ( *psz == s_chNonBreakingSpace )
        m_bWhiteSpace = TRUE;
    
    if ( *psz == '\\' || *psz == '{' || *psz == '}' )
        m_ios << '\\' << *psz;
    else if ( *psz == '\t' )
        WriteTab();
	else if ( bUnicode ) // && *psz < 0 ) // If UTF-8 char, output as Unicode
		{
		LONG lUTF32 = lUTF8ToUTF32( psz, iNumBytes );
		char pszUTF32[20];
		_ltoa( lUTF32, pszUTF32, 10 );
//        m_ios << "\\uc0\\u" << pszUTF32; // 1.4ywk 
        m_ios << "\\u" << pszUTF32; // 1.4ywk Change RTF export to only write \uc0 once
		}
    else
        m_ios << *psz;

    *ppsz = psz + iNumBytes;  // Point to the next character
}

void RTF_ostream::WriteChars(const char* pch, int len, BOOL bUnicode)
{
    ASSERT( pch );
    ASSERT( 0 <= len );
    const char* pchEnd = pch + len;
    while ( pch != pchEnd )
        WriteChar(&pch, bUnicode);
}
