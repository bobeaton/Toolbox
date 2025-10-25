// HTML.cpp  HyperText Markup Language Export Process

// 1998-12-02 MRP: This feature is omitted from the 16-bit version,
// mainly because there would be problems with long file names and
// the four-letter .html file name extension.
#include "stdafx.h"
#include "toolbox.h"
#include "html.h"
#include "cfield.h"  // class CField
#include "ddx.h"  // Shw_bNeedsToBeTrimmed
#include <ctype.h>  // isupper

#ifdef EXPORT_WEB_PAGES

// --------------------------------------------------------------------------

#ifdef _MAC
static const char s_chNonBreakingSpace = '\xCA';  // decimal 202
#else
static const char s_chNonBreakingSpace = '\xA0';  // decimal 160
#endif  // _MAC


// ==========================================================================

static const int s_maxlenRenBuf = 30000;  // 1998-03-03 MRP

HTML_ostream::HTML_ostream(std::ostream& ios, CMarkerSet* pmkrset,
        BOOL bHTML_XML, BOOL bUTF8,
        const Str8& sTitle,
        const Str8& sBaseTarget,
        const Str8& sStylesheetPath,
        const Str8& sCopyright,
        const Str8& sGenerator,
        BOOL bNoRobots) :
    Field_ostream(ios, FALSE),
    m_bHTML_XML(bHTML_XML),
    m_bUTF8(bUTF8),
    m_sTitle(sTitle),
    m_sBaseTarget(sBaseTarget),
    m_sStylesheetPath(sStylesheetPath),
    m_sCopyright(sCopyright),
    m_sGenerator(sGenerator),
    m_bNoRobots(bNoRobots)
{
    if ( m_bUTF8 )
        m_sCharset = "UTF-8";  // 1999-02-16 MRP
    else
        m_sCharset = "ISO-8859-1";  // 1999-02-15 MRP: Default is Latin-1

    m_bEmbeddedMarkup = TRUE;
    m_bHaveWrittenField = FALSE;
    m_bPar = FALSE;
    m_bCharInThisPar = FALSE;
    m_bWhiteSpace = FALSE;

    m_pmkrset = pmkrset;
    ASSERT( m_pmkrset );    
    m_pchRenBuf = new char[s_maxlenRenBuf + 1];  // 1998-03-03 MRP

    InitOpenTags();  // 1998-08-20 MRP    
    WriteHead();
}

HTML_ostream::~HTML_ostream()
{
    WriteTail();
    delete m_pchRenBuf;
}


// A file or stream of RTF consists of "plain text" interspersed with
// control symbols, control words, and braces. The control words,
// control symbols, and braces constitute control information.
// All other characters in the file are plain text.
// [Rich Text Format (RTF Version 1.3) Specification.]

void HTML_ostream::WriteHead()
{
    m_ios << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\">\n";
    m_ios << "<HTML>\n";
    m_ios << "<HEAD>\n";
    WriteCharset();  // 1999-02-15 MRP
    WriteMETA("copyright", m_sCopyright);  // 1998-12-11 MRP // 1.5.0ft Don't translate, html header
    WriteMETA("generator", m_sGenerator);  // 1999-02-04 MRP // 1.5.0ft Don't translate, html header
    if ( m_bNoRobots )
        // Tell visiting [search engine] robots that this web page
        // shouldn't be indexed, nor used to harvest more links
        WriteMETA("robots", "noindex, nofollow");  // 1999-02-15 MRP
    if ( !m_sStylesheetPath.IsEmpty() )
    	{
        // 1998-12-05 MRP
        if ( m_bHTML_XML )
            m_ios << "<XML:namespace prefix=\"MDF\">\n";
    	// 1998-11-21 MRP
    	// Str8 sStylesheetFilename = sGetFileName(m_sStylesheetPath);
	    m_ios << "<LINK REL=\"stylesheet\" HREF=\"" <<
	    	m_sStylesheetPath <<
	    	"\" TYPE=\"text/css\">\n";
	    }
    if ( !m_sBaseTarget.IsEmpty() )
    	m_ios << "<BASE TARGET=\"" << m_sBaseTarget << "\">\n";
    if ( !m_sTitle.IsEmpty() )
        {
    	m_ios << "<TITLE>";
        WriteChars(m_sTitle, m_sTitle.GetLength());  // 1999-02-04 MRP
        m_ios << "</TITLE>\n";
        }
    m_ios << "</HEAD>\n";
    m_ios << "<BODY>\n";
}

void HTML_ostream::WriteTail()
{
    CloseTag(0);
    m_ios << "</BODY>\n";
    m_ios << "</HTML>\n";
}


// 1998-08-20 MRP
static BOOL s_bHTML(const CMarker* pmkr)
{
    ASSERT( pmkr );
    const char* pch = pmkr->sMarker();
    ASSERT( *pch );
    
    if ( !__isascii(*pch) || !isupper(*pch) )
        return FALSE;

    for ( ; *pch; pch++ )
        if ( !__isascii(*pch) || !(isupper(*pch) || isdigit(*pch)) )
            return FALSE;

    // 1998-08-20 MRP: Markers in all upper-case are considered to be
    // built-in HTML tags (e.g. TABLE, TR, TD).
    return TRUE;
}

void HTML_ostream::InitOpenTags()
{
    Level lev = 0;
    for ( ; lev <= s_maxlevHTML; lev++ )
        m_apmkrHTML[lev] = NULL;
}

void HTML_ostream::OpenTag(const CMarker* pmkr)
{
    ASSERT( pmkr );
    if ( s_bHTML(pmkr) )
        {
        m_ios << '<' << pmkr->sMarker() << '>';
        if ( pmkr->bParStyle() )
            m_ios << '\n';
        }
    else
        {
        m_ios << "<DIV CLASS=\"" << pmkr->sMarker() << "\">\n";
        m_bPar = TRUE;
        }

    const char* pszMarker = pmkr->sMarker();
    int levMarker = pmkr->lev();
    if ( levMarker <= s_maxlevHTML )
        m_apmkrHTML[levMarker] = pmkr;
}

void HTML_ostream::CloseTag(int levCur)
{
    ASSERT( 0 <= levCur );
    BOOL bNewline = FALSE;
    int lev = s_maxlevHTML;
    while ( levCur <= lev )
        {
        ASSERT( 0 <= lev );
        const CMarker* pmkr = m_apmkrHTML[lev];
        if ( pmkr )
            {
            if ( !bNewline && pmkr->bParStyle() ) // 1998-12-02 MRP
                {
                m_ios << '\n';
                bNewline = TRUE;
                }
            if ( s_bHTML(pmkr) )
                {
                m_ios << "</" << pmkr->sMarker() << '>';
                if ( pmkr->bParStyle() )
                    m_ios << '\n';
                }
            else
                {
                if ( m_bPar )
                    m_ios << "</DIV>\n";

                m_bCharInThisPar = FALSE;
                m_bWhiteSpace = FALSE;
                }
            m_apmkrHTML[lev] = NULL;
            }
        if ( lev == 0 )
            break;
        else
            lev--;
        }
}


void HTML_ostream::WriteField(const CField* pfld, BOOL bFirstField)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();

    // 1998-08-20 MRP: Use a generic DIV???    
    // if ( bFirstField && !pmkr->bParStyle() )
    //    WriteParBegin(m_pstyset->pstyNormal());

    CloseTag(pmkr->lev());
    if ( s_bHTML(pmkr) )
        WriteFieldAsHTML(pfld);  // *** ASSUMES NO FIELD CONTENT
    else if ( pmkr->bCharStyle() )
        WriteFieldAsSPAN(pfld);   // or DIV or built-in HTML
    else
        WriteFieldAsDIV(pfld);

    m_bHaveWrittenField = TRUE;
}

void HTML_ostream::WriteFieldAsHTML(const CField* pfld)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();
    ASSERT( pmkr );
    OpenTag(pmkr);

    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;
        
    WriteFieldContents(pch, pchEnd, pmkr);  // 1998-08-20 MRP
    m_bCharInThisPar = TRUE;
}

void HTML_ostream::WriteFieldAsDIV(const CField* pfld)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();
    ASSERT( pmkr );
    // 1998-12-12 MRP: Open and close DIV tags according to hierarchy
    OpenTag(pmkr);
    // WriteDivBegin(pmkr);

    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;
        
    WriteFieldContents(pch, pchEnd, pmkr);  // 1998-08-20 MRP
    m_bCharInThisPar = TRUE;
}

void HTML_ostream::WriteFieldAsSPAN(const CField* pfld)
{
    ASSERT( pfld );
    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;

    if ( m_bCharInThisPar && !m_bWhiteSpace )
        m_ios << '\n';  // Separate from preceding character style

    const CMarker* pmkr = pfld->pmkr();     
    WriteFieldContents(pch, pchEnd, pmkr);  // 1998-08-20 MRP
    m_bCharInThisPar = TRUE;
}

BOOL HTML_ostream::bTrimmedFieldContents(const CField* pfld,
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


void HTML_ostream::WriteFieldContents(const char* pch, const char* pchEnd,
        const CMarker* pmkr)
{
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( pmkr );

    if ( pmkr->bCharStyle() )
        {
        if ( m_bHTML_XML )  // 1998-12-05 MRP
            m_ios << "<MDF:" << pmkr->sMarker() << ">";
        else
            m_ios << "<SPAN CLASS=\"" << pmkr->sMarker() << "\">";
        }

    // Write the field's contents [pch, pchEnd).
    // Because control symbols |{...} and subfield begin |mkr{ and
    // end tags } shouldn't be processed by the rendering DLL,
    // the field must be divided into renderable substrings.
    const char* pchBegin = pch;
    while ( pch != pchEnd )
        // 1999-03-31 MRP: Let a newline immediately preceding
        // a control symbol be preserved in the HTML output.
        // For example, MDF formats \tb Table fields using
        // |{BR} (i.e. the HTML <BR> tag) at the beginning of a line.
        if ( *pch == '\n' && pch + 1 != pchEnd && bAtControlSymbol(pch + 1, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            m_ios << '\n';
            m_bWhiteSpace = TRUE;
            pch += 1;
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        else if ( bAtControlSymbol(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        // 1999-02-26 MRP: Let a newline immediately preceding
        // a subfield be preserved in the HTML output.
        // That is, a subfield at the beginning of a line becomes
        // a SPAN that begins on a new line.
        else if ( *pch == '\n' && pch + 1 != pchEnd && bAtSubfield(pch + 1, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            m_ios << '\n';
            m_bWhiteSpace = TRUE;
            pch += 1;
            WriteSubfield(&pch, pchEnd, pmkr, FALSE);
            pchBegin = pch;
            }
        else if ( bAtSubfield(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteSubfield(&pch, pchEnd, pmkr, FALSE);
            pchBegin = pch;
            }
        else
            pch++;

    WriteRenderableSubstring(pchBegin, pch, pmkr);

    if ( pmkr->bCharStyle() )
        {
        if ( m_bHTML_XML )  // 1998-12-05 MRP
            m_ios << "</MDF:" << pmkr->sMarker() << ">";
        else
            m_ios << "</SPAN>";
        }
}


BOOL HTML_ostream::bAtControlSymbol(const char* pch, const char* pchEnd)
{
    const char* pchControlSymbol = NULL;
    int len = 0;
    return bControlSymbol(&pch, pchEnd, &pchControlSymbol, &len);
}

BOOL HTML_ostream::bControlSymbol(const char** ppch, const char* pchEnd,
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

    // 1998-08-21 MRP: Use for special HTML tags like A and BR
    if ( __isascii(*pch) && isupper(*pch) )
        {
        pch++;
        while ( pch != pchEnd && __isascii(*pch) && isupper(*pch) )
            pch++;
        
        if ( pch != pchEnd && *pch == ' ' )
            while ( pch != pchEnd && *pch != '}' )
                pch++;
        }
    else if ( *pch == '/' )
        {
        pch++;
        while ( pch != pchEnd && __isascii(*pch) && isupper(*pch) )
            pch++;
        }
    else
        return FALSE;
        
    if ( pch == pchEnd || *pch != '}' )
        return FALSE;
        
    int len = pch - pchBegin;  // Note: Must do here before incrementing
    ASSERT( *pch == '}' );
    pch++;  // Skip over closing brace

	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    ASSERT( len != 0 );
    *ppch = pch;
    *ppchControlSymbol = pchBegin;
    *plenControlSymbol = len;
    return TRUE;
}

Str8 HTML_ostream::s_sConvertURL(const Str8& sURL, BOOL bAnchor)
{
    const char* pch = sURL;
    const char* pchEnd = pch + sURL.GetLength();
    return s_sConvertURL(pch, pchEnd, bAnchor);
}

Str8 HTML_ostream::s_sConvertURL(const char* pch, const char* pchEnd,
        BOOL bAnchor)
{
	ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    Str8 s;
    // Numeric codes for non-alphabetic characters are separated
    // from adjacent characters (or codes) by an underline.
    // But note that a code at the beginning of the string
    // shouldn't be preceded by an underline, nor should a code
    // at the end be followed by one.
    BOOL bPrev = FALSE;  // There is a previous chararacter
    BOOL bPrevNonAlpha = FALSE;  // The previous char wasn't alphabetic
	for ( ; pch != pchEnd; pch++ )
        {
        ASSERT( *pch );
        // 1999-02-19 MRP: In an anchor, we do allow all 7-bit characters,
        // but convert 8-bit characters to avoid conflict with UTF-8.
        BOOL bCopy = FALSE;
        if ( bAnchor )
            bCopy = __isascii(*pch);
        else
            bCopy = (__isascii(*pch) && (isalpha(*pch) || *pch == ' '));
		if ( bCopy )
            {
            if ( bPrevNonAlpha )
                s += '_';
			s += *pch;
            bPrevNonAlpha = FALSE;
            }
        else
            {
            if ( bPrev )
                s += '_';
            unsigned char ch = *pch;
            unsigned int uiChar = ch;
            Str8 sCharCode;
            sCharCode.Format("%d", uiChar);
            s += sCharCode;
            bPrevNonAlpha = TRUE;
            }  
        bPrev = TRUE;
        }

    return s;
}

static BOOL s_bWhiteSpaceHTML(char ch)
{
    return ( ch == ' ' || ch == '\n' );
}

BOOL HTML_ostream::bWroteAnchorOrLink(const char* pch, const char* pchEnd)
{
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch < pchEnd );

    if ( *pch != 'A' )
        return FALSE;

    if ( pch + 1 != pchEnd && !s_bWhiteSpaceHTML(pch[1]) )
        return FALSE;

    m_ios << '<';
    while ( pch != pchEnd )
        if ( bWroteAnchor(&pch, pchEnd) )
            ;
        else if ( bWroteLink(&pch, pchEnd) )
            ;
        else
            m_ios << *pch++;
    m_ios << '>';
    m_bWhiteSpace = FALSE;
    return TRUE;
}

BOOL HTML_ostream::bWroteAnchor(const char** ppch, const char* pchEnd)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );

    if ( pch == pchEnd || !s_bWhiteSpaceHTML(*pch) )
        return FALSE;

    if ( pchEnd < pch + 7 || _strnicmp(pch + 1, "NAME=\"", 6) != 0 )
        return FALSE;

    m_ios.write(pch, 7);  // 7 = white space, NAME=, double quote

    pch += 7;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    const char* pchAnchor = pch;
    for ( ; pch != pchEnd; pch++ )
        if ( *pch == '"' )  // Closing double quote
            break;

    Str8 sConvertedAnchor = s_sConvertURL(pchAnchor, pch, TRUE);
    m_ios << sConvertedAnchor << '"';

    if ( pch != pchEnd )
        pch++;  // Skip over the closing double quote
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
    return TRUE;
}

BOOL HTML_ostream::bWroteLink(const char** ppch, const char* pchEnd)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );

    if ( pch == pchEnd || !s_bWhiteSpaceHTML(*pch) )
        return FALSE;

    if ( pchEnd < pch + 7 || _strnicmp(pch + 1, "HREF=\"", 6) != 0 )
        return FALSE;

    m_ios.write(pch, 7);  // 7 = white space, HREF=, double quote

    pch += 7;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    const char* pchLink = pch;
    const char* pchHash = NULL;
    for ( ; pch != pchEnd; pch++ )
        if ( *pch == '"' )  // Closing double quote
            break;
        else if ( *pch == '#' && !pchHash )
            pchHash = pch;

    const char* pchClose = pch;

    int lenLinkExceptHash = ( pchHash ? pchHash - pchLink : pch - pchLink );
    Str8 sLinkExceptHash(pchLink, lenLinkExceptHash);
    if ( pchHash )
        {
        const char* pchChar = NULL;
        for ( pch = pchLink; pch < pchClose; pch++ )
            if ( pch + 6 <= pchHash && _strnicmp(pch, "@.html", 6) == 0 )
                {
                pchChar = pch;
                break;
                }

        Str8 sMChar;
        // 2000-04-17 MRP: Rewrite to account for changes made to the sort key
        // using a CCT or DLL (optional step for non-Roman scripts).
        // if ( pchChar && bFirstPrimaryLowerCase(pchHash + 1, pchClose, sMChar) )
    	extern const CSortOrder* g_psrtHTML;  // 1998-10-26 MRP
        CSortOrder* psrt = (CSortOrder*)g_psrtHTML;  // 2000-04-17 MRP: cast away const-ness
        int lenAnchorName = pchClose - (pchHash + 1);
        if ( pchChar && psrt && psrt->bFirstPrimaryLowerCase(pchHash + 1, lenAnchorName, sMChar) )
            {
        	// Convert any non-alphabetic characters in the file name
            Str8 sConvertedMChar = s_sConvertURL(sMChar);
            int lenPrecedingChar = pchChar - pchLink;
            int lenFollowingChar = pchHash - (pchChar + 1);
            ASSERT( lenPrecedingChar + 1 + lenFollowingChar == lenLinkExceptHash );
            sLinkExceptHash = sLinkExceptHash.Left(lenPrecedingChar) +
                sConvertedMChar +
                sLinkExceptHash.Right(lenFollowingChar);
            }
        }
    
    Str8 sHash;
    if ( pchHash )
        {
        ASSERT( *pchHash == '#' );
        ASSERT( pchHash < pchClose );
        // 1999-06-30 MRP: Recode to avoid compiler error on Power Macintosh:
        // 4 overloads have similar conversions
        sHash = '#';
    	// Convert any non-alphabetic characters in the anchor name
        sHash += s_sConvertURL(pchHash + 1, pchClose, TRUE);
        }

    m_ios << sLinkExceptHash << sHash << '"';  // Closing double quote

    pch = pchClose;
    if ( pch != pchEnd )
        pch++;  // Skip over the closing double quote
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
    return TRUE;
}

void HTML_ostream::WriteControlSymbol(const char** ppch, const char* pchEnd)
{
    const char* pch = NULL;
    int len = 0;
    BOOL b = bControlSymbol(ppch, pchEnd, &pch, &len);
    ASSERT( b );
    ASSERT( pch );
    ASSERT( len != 0 );
    m_bWhiteSpace = FALSE;  // 1999-02-26 MRP

    if ( bWroteAnchorOrLink(pch, pch+len) )  // 1999-01-23 MRP
        return;

    // 1998-08-21 MRP: Use for special HTML tags like BR
    m_ios << '<';
    m_ios.write(pch, len);
    m_ios << '>';
}


BOOL HTML_ostream::bAtSubfield(const char* pch, const char* pchEnd)
{
    const CMarker* pmkr = NULL;
    return bSubfieldBegin(&pch, pchEnd, &pmkr);
}

BOOL HTML_ostream::bSubfieldBegin(const char** ppch, const char* pchEnd,
        const CMarker** ppmkr)
{
    ASSERT( ppch );
    const char* pch = *ppch;
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( ppmkr );

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

    const CMarker* pmkr = m_pmkrset->
        pmkrSearchForWholeSubString(pchMarker, lenMarker);
    if ( !pmkr )  // If the marker is not in the style sheet,
        return FALSE;  // It gets put out as text

    if ( !pmkr->bCharStyle() )
        return FALSE;

    ASSERT( *pch == '{' );
    pch++;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
    *ppmkr = pmkr;
    return TRUE;
}

void HTML_ostream::WriteSubfield(const char** ppch, const char* pchEnd,
        const CMarker* pmkrSurrounding, BOOL bInFootnote)
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
    BOOL b = bSubfieldBegin(&pch, pchEnd, &pmkr);
    ASSERT( b );
    ASSERT( pmkr );
    BOOL bHTML = s_bHTML(pmkr);  // 1998-08-22 MRP: |NOBR{...}

    // Don't allow a footnote within a footnote.
    BOOL bFootnote = ( !bInFootnote && bEqual(pmkr->sMarker(), "*f") );
    // 1998-07-04 MRP: This way of doing footnotes is undocumented.
    // The long-term plan is to use a footnote reference control symbol
    // in the field with the footnote contents in following field(s).
    // The change is needed to allow multi-paragraph footnote contents.
    // We'll need to review the way CTW and SF Converter do it.
		{
        if ( bHTML )
            m_ios << '<' << pmkr->sMarker() << '>';
        else
            {
            if ( m_bHTML_XML )  // 1998-12-05 MRP
                m_ios << "<MDF:" << pmkr->sMarker() << ">";
            else
                m_ios << "<SPAN CLASS=\"" << pmkr->sMarker() << "\">";
            }
        }

    // Write the subfield's contents.
    // Because control symbols |{...} and subfield begin |mkr{ and
    // end tags } shouldn't be processed by the rendering DLL,
    // the subfield must be divided into renderable substrings.
    const char* pchBegin = pch;
    while ( pch != pchEnd )
        // 1999-03-31 MRP: Let a newline immediately preceding
        // a control symbol be preserved in the HTML output.
        // For example, MDF formats \tb Table fields using
        // |{BR} (i.e. the HTML <BR> tag) at the beginning of a line.
        if ( *pch == '\n' && pch + 1 != pchEnd && bAtControlSymbol(pch + 1, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            m_ios << '\n';
            m_bWhiteSpace = TRUE;
            pch += 1;
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        else if ( bAtControlSymbol(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            WriteControlSymbol(&pch, pchEnd);
            pchBegin = pch;
            }
        // 1999-02-26 MRP: Let a newline immediately preceding
        // a subfield be preserved in the HTML output.
        // That is, a subfield at the beginning of a line becomes
        // a SPAN that begins on a new line.
        else if ( *pch == '\n' && pch + 1 != pchEnd && bAtSubfield(pch + 1, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            m_ios << '\n';
            m_bWhiteSpace = TRUE;
            pch += 1;
            WriteSubfield(&pch, pchEnd, pmkr, FALSE);
            pchBegin = pch;
            }
        else if ( bAtSubfield(pch, pchEnd) )
            {
            WriteRenderableSubstring(pchBegin, pch, pmkr);
            // Note: A footnote's contents has a paragraph style.
            // Recursively embedded subfields are allowed, but the
            // RTF output consists of disjoint (i.e. "flattened,
            // not embedded) character style groups.
            WriteSubfield(&pch, pchEnd, pmkr, bInFootnote);
            pchBegin = pch;
            }
        else if ( *pch == '}' )  // End of the subfield
            break;
        else
            pch++;

    WriteRenderableSubstring(pchBegin, pch, pmkr);

    // Note: Even if this subfield's closing brace is missing,
    // it will automatically be closed correctly in the RTF output.
    if ( bHTML )
        m_ios << "</" << pmkr->sMarker() << '>';
    else
        {
        if ( m_bHTML_XML )  // 1998-12-05 MRP
            m_ios << "</MDF:" << pmkr->sMarker() << ">";
        else
            m_ios << "</SPAN>"; // Close the subfield's RTF group
        }
    // 1998-08-21 MRP: Embed subfields?
    // if ( !bFootnote && pmkrSurrounding->bCharStyle() )
    //     m_ios << "<SPAN CLASS=\"" << pmkrSurrounding->sMarker() << "\">";

    if ( pch != pchEnd && *pch == '}' )
        pch++;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
}


void HTML_ostream::WriteRenderableSubstring(const char* pch,
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
        if ( *pch == ' ' )
            {
            // 1998-07-13 MRP: Separated out tab-aligned interlinear logic.
            pch++;
            WriteSpace();
            }
        else if ( *pch == '\n' )
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
                WriteSpace();
                }
            }
        else
            WriteChar(&pch);
		if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
			pch = pchEnd;
        }
}


void HTML_ostream::WriteTab()
{
    m_ios << ' ';
    m_bWhiteSpace = TRUE;
}

void HTML_ostream::WriteLine()
{
    m_ios << "<BR>";
    m_bWhiteSpace = TRUE;
}

void HTML_ostream::WriteSpace()
{
    m_ios << ' ';
    m_bWhiteSpace = TRUE;
}

void HTML_ostream::WriteNonBreakingSpace()
{
    m_ios << ' ';  // ***
    m_bWhiteSpace = TRUE;
}

void HTML_ostream::WriteChar(const char** ppsz)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );

    if ( *psz == s_chNonBreakingSpace )
        m_bWhiteSpace = TRUE;

    if ( !__isascii(*psz) && !m_bUTF8 ) // 1.2hd Fix bug of not outputting Unicode correctly in html export
        {
        unsigned char uch = *psz;
        int i = uch;
        m_ios << "&#" << i << ';';
        }
    else if( *psz == '<' )
        m_ios << "&lt;";
    else if ( *psz == '>' )
        m_ios << "&gt;";
    else if ( *psz == '"' )
        m_ios << "&quot;";
    else if ( *psz == '&' )
        m_ios << "&amp;";
    else
        m_ios << *psz;
    
    *ppsz = psz + 1;  // Point to the next character
}

void HTML_ostream::WriteChars(const char* pch, int len)
{
    ASSERT( pch );
    ASSERT( 0 <= len );
    const char* pchEnd = pch + len;
    while ( pch != pchEnd )
        WriteChar(&pch);
}

void HTML_ostream::WriteCharset()
{
    m_ios << "<META HTTP-EQUIV=\"Content-Type\" CONTENT=\"text/html; charset=";
    m_ios << m_sCharset;
    m_ios << "\">\n";
}

void HTML_ostream::WriteMETA(const char* pszName, const Str8& sContent)
{
    ASSERT( pszName );
    ASSERT( *pszName );
    if ( sContent.IsEmpty() )
        return;

    m_ios << "<META NAME=\"" << pszName << "\" CONTENT=\"";
    WriteChars(sContent, sContent.GetLength());
    m_ios << "\">\n";
}

#endif  // EXPORT_WEB_PAGES
