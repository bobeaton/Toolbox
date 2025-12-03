// XML.cpp  Extensible Markup Language Export Process

// 1998-12-02 MRP: This feature is omitted from the 16-bit version,
// mainly because there would be problems with long file names and
// the four-letter .html file name extension.
#include "stdafx.h"
#include "toolbox.h"
#include "xml.h"
#include "cfield.h"  // class CField
#include "ddx.h"  // Shw_bNeedsToBeTrimmed
#include <ctype.h>  // isupper

// --------------------------------------------------------------------------

#ifdef _MAC
static const char s_chNonBreakingSpace = '\xCA';  // decimal 202
#else
static const char s_chNonBreakingSpace = '\xA0';  // decimal 160
#endif  // _MAC


// ==========================================================================

static const int s_maxlenRenBuf = 30000;  // 1998-03-03 MRP

XML_ostream::XML_ostream(ostream& ios, CMarkerSet* pmkrset,
        const Str8& sRootNode,
        BOOL bUTF8,
        const Str8& sDTDName,
        const Str8& sStylesheetPath) :
    Field_ostream(ios, FALSE),
    m_pmkrset(pmkrset),
    m_sRootNode(sRootNode),
    m_bUTF8(bUTF8),
    m_sDTDName(sDTDName),
    m_sStylesheetPath(sStylesheetPath)
{
    ASSERT( m_pmkrset );    
    if ( m_bUTF8 )
        m_sCharset = "UTF-8";  // 1999-02-16 MRP
    else
        m_sCharset = "ISO-8859-1";  // 1999-02-15 MRP: Default is Latin-1

    m_bEmbeddedMarkup = TRUE;
    m_bHaveWrittenField = FALSE;

    m_pchRenBuf = new char[s_maxlenRenBuf + 1];  // 1998-03-03 MRP

    InitOpenTags();  // 1998-08-20 MRP    
    WriteHead();
}

XML_ostream::~XML_ostream()
{
    WriteTail();
    delete m_pchRenBuf;
}


void XML_ostream::WriteHead()
{
// Lines below were proposed by Larry H for Unicode, but didn't work
//    m_ios << "<?xml version=\"1.0\" encoding=\"" << m_sCharset << "\"?>\n";
//	m_ios << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" ";
//	m_ios << "\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n";
//	m_ios << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";

// Line below was given by Peter C for Unicode, and does work
//	m_ios << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"; // 1.2pt Fix bug of xml output having too many main headers

/* Code below does not work
#ifdef XML_DTD
    m_ios << "<!DOCTYPE ";
    m_ios << m_sRootNode;
    m_ios << " SYSTEM \"" << m_sDTDName << ".dtd\">\n";
#endif  // XML_DTD
	*/

// Line below was given by Joan Spanne and is in "Teach Yourself XML" // 1.3am Fix missing xml header
    m_ios << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";

    if ( !m_sStylesheetPath.IsEmpty() )
		{
		Str8 sStylesheetFileName = sGetFileName( m_sStylesheetPath ); // 1.5.9d Eliminate path from style sheet in XML export
		if ( sStylesheetFileName.Find( ".css" ) > 0 ) // 1.5.9c Allow .css cascading style sheet in XML export
		    m_ios << "<?xml-stylesheet href=\"" << sStylesheetFileName << "\" type=\"text/css\"?>\n"; // 1.5.9c 
		else // 1.5.9c 
		    m_ios << "<?xml-stylesheet href=\"" << sStylesheetFileName << "\" type=\"text/xsl\"?>\n";
		}
    // m_ios << '<' << m_sRootNode << ">\n";
    WriteStartTag(m_sRootNode);  // 1999-10-11 MRP
    m_ios << '\n';  // 1999-10-11 MRP
}

void XML_ostream::WriteTail()
{
    CloseTag(0);
    // m_ios << "</" << m_sRootNode << ">\n";
	int iStart = 0; // 1.5.9b 
	Str8 sRootAlone; // 1.5.9b 
	BOOL bSucc = m_sRootNode.bNextWord( sRootAlone, iStart); // 1.5.9b Get root node without extras used in header
    WriteEndTag(sRootAlone);  // 1999-10-11 MRP
    m_ios << '\n';  // 1999-10-11 MRP
}


static BOOL s_bGroup(const CMarker* pmkr)
{
    // 1999-08-03 MRP: This experimental shortcut causes assertion
    // "Line 153 in xml.cpp".
    // Eventually, "group" will probably become a marker property.
    // return pmkr->bParStyle();

    ASSERT( pmkr );
    CMarkerSet* pmkrset = pmkr->pmkrsetMyOwner();
    CMarker* pmkrUnder = pmkrset->pmkrFirst();
    for ( ; pmkrUnder; pmkrUnder = pmkrset->pmkrNext(pmkrUnder) )
        if ( pmkrUnder->pmkrOverThis() == pmkr )
            return TRUE;

    return FALSE;
}

void XML_ostream::WriteStartTag(const CMarker* pmkr, BOOL bGroup)
{
    ASSERT( pmkr );
    WriteStartTag(pmkr->sMarker(), bGroup);
}

void XML_ostream::WriteEndTag(const CMarker* pmkr, BOOL bGroup)
{
    ASSERT( pmkr );
    WriteEndTag(pmkr->sMarker(), bGroup);
}

// The SGML Handbook, p. 23.
// Descriptive markup tags identify the start and end of elements.
// There are three special character strings that are important:
static const char* s_pszSTAGO = "<";   // Start-TAG Open
static const char* s_pszETAGO = "</";  // End-TAG Open
static const char* s_pszTAGC = ">";    // TAG Close

void XML_ostream::WriteStartTag(const Str8& sMarker, BOOL bGroup)
{
    m_ios << s_pszSTAGO;
    WriteMarker(sMarker, bGroup);
    m_ios << s_pszTAGC;
    if ( bGroup )
        m_ios << '\n';
}

void XML_ostream::WriteEndTag(const Str8& sMarker, BOOL bGroup)
{
    m_ios << s_pszETAGO;
    WriteMarker(sMarker, bGroup);
    m_ios << s_pszTAGC;
    if ( bGroup )
        m_ios << '\n';
}

void XML_ostream::WriteMarker(const Str8& sMarker, BOOL bGroup)
{
    s_WriteMarker(m_ios, sMarker, bGroup);
}

void XML_ostream::s_WriteMarker(ostream& ios, const CMarker* pmkr, BOOL bGroup)
{
    ASSERT( pmkr );
    if ( bGroup )
        ASSERT( s_bGroup(pmkr) );
    s_WriteMarker(ios, pmkr->sMarker(), bGroup);
}

void XML_ostream::s_WriteMarker(ostream& ios, const Str8& sMarker, BOOL bGroup)
{
    ASSERT( !sMarker.IsEmpty() );
	char cFirstChar = sMarker.GetChar( 0 ); // 1.5.1nd If marker starts with non-letter, output with "a" in front to make valid XML
	if ( ! ( __isascii( cFirstChar ) && (isalpha( cFirstChar ) ) ) ) // 1.5.1nd 
		ios << "a"; // 1.5.1nd 
    ios << sMarker;

    // 1999-10-11 MRP: Use "Group" as the suffix to make the extra tag
    // that encloses a data field and all those that are "under" it
    // according to the marker hierarchy relationships.
    if ( bGroup )
        ios << "Group";
}

// --------------------------------------------------------------------------
#ifdef XML_DTD
static BOOL s_bOptionalHierarchicalLevel(const CMarker* pmkr)
{
    ASSERT( pmkr );
    const Str8& sMarker = pmkr->sMarker();
    return (strcmp(sMarker, "se") == 0 || strcmp(sMarker, "sn") == 0);
}

void XML_ostream::s_WriteDTD(ostream& ios, const CMarkerSet* pmkrset,
        const Str8& sRootNode)
{
    ASSERT( pmkrset );
    const CMarker* pmkrRecord = ((CMarkerSet*)pmkrset)->pmkrRecord();
    ASSERT( s_bGroup(pmkrRecord) );

    ios << "<!ELEMENT ";
    // ios << sRootNode;
    s_WriteMarker(ios, sRootNode);  // 1999-10-11 MRP
    ios << " (";
    s_WriteMarker(ios, pmkrRecord, TRUE);
    ios << "*)>\n";

    const CMarker* pmkr = pmkrset->pmkrFirst();
    for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) )
        s_WriteElement(ios, pmkr);
}

void XML_ostream::s_WriteElement(ostream& ios, const CMarker* pmkrElement)
{
    ASSERT( pmkrElement );
    
    ios << "<!ELEMENT ";
    s_WriteMarker(ios, pmkrElement, FALSE);
    ios << " (#PCDATA)>\n";

    if ( s_bGroup(pmkrElement) )
        {
        ios << "<!ELEMENT ";
        s_WriteMarker(ios, pmkrElement, TRUE);
        ios << " (";
        s_WriteMarker(ios, pmkrElement, FALSE);
        ios << ',';
        s_WriteMarkersInGroup(ios, pmkrElement);
        ios << "*)>\n";
        }
}

void XML_ostream::s_WriteMarkersInGroup(ostream& ios, const CMarker* pmkrGroup)
{
    ASSERT( pmkrGroup );
    ASSERT( s_bGroup(pmkrGroup) );
    
    ios << '(';
    BOOL bHaveWrittenAny = FALSE;

    const CMarkerSet* pmkrset = pmkrGroup->pmkrsetMyOwner();
    const CMarker* pmkr = pmkrset->pmkrFirst();
    for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) )
        if ( pmkr->pmkrOverThis() == pmkrGroup )
            {
            if ( bHaveWrittenAny )
                ios << '|';
            if ( s_bOptionalHierarchicalLevel(pmkr) )
                {
                ASSERT( s_bGroup(pmkr) );
                ios << '(';
                s_WriteMarker(ios, pmkr, TRUE);
                ios << '|';
                s_WriteMarkersInGroup(ios, pmkr);
                ios << ')';
                }
            else
                s_WriteMarker(ios, pmkr, s_bGroup(pmkr));
            bHaveWrittenAny = TRUE;
            }

    ASSERT( bHaveWrittenAny );
    ios << ')';
}
#endif  // ifdef XML_DTD
// --------------------------------------------------------------------------


void XML_ostream::InitOpenTags()
{
    Level lev = 0;
    for ( ; lev <= s_maxlevHTML; lev++ )
        m_apmkrHTML[lev] = NULL;
}

void XML_ostream::OpenTag(const CMarker* pmkr)
{
    ASSERT( pmkr );

    if ( s_bGroup(pmkr) )
        // m_ios << '<' << pmkr->sMarker() << "_>\n";
        WriteStartTag(pmkr, TRUE);  // 1999-10-11 MRP

    const char* pszMarker = pmkr->sMarker();
    int levMarker = pmkr->lev();
    if ( levMarker <= s_maxlevHTML )
        m_apmkrHTML[levMarker] = pmkr;
}

void XML_ostream::CloseTag(int levCur)
{
    ASSERT( 0 <= levCur );
    int lev = s_maxlevHTML;
    while ( levCur <= lev )
        {
        ASSERT( 0 <= lev );
        const CMarker* pmkr = m_apmkrHTML[lev];
        if ( pmkr )
            {
            if ( s_bGroup(pmkr) )
                // m_ios << "</" << pmkr->sMarker() << "_>\n";
                WriteEndTag(pmkr, TRUE);  // 1999-10-11 MRP
            m_apmkrHTML[lev] = NULL;
            }
        if ( lev == 0 )
            break;
        else
            lev--;
        }
}


void XML_ostream::WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst) // 1.1cm Write alphabetic divider
	{
	CloseTag( 0 ); // 1.4bw Fix bug of alph divider in wrong place in XML export
    WriteStartTag("AlphDiv ");
	m_ios << sAlphLet; // 1.1cm Write alphabetic divider
    WriteEndTag("AlphDiv ");
    m_ios << '\n';
	}

void XML_ostream::WriteField(const CField* pfld, BOOL bFirstField)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();

    CloseTag(pmkr->lev());

    if ( bFirstField && m_bHaveWrittenField )
        m_ios << '\n';

    WriteField(pfld);

    m_bHaveWrittenField = TRUE;
}

void XML_ostream::WriteField(const CField* pfld)
{
    ASSERT( pfld );
    const CMarker* pmkr = pfld->pmkr();
    ASSERT( pmkr );
    // 1998-12-12 MRP: Open and close DIV tags according to hierarchy
    // OpenTag(pmkr);  // 1999-05-18 MRP
    // WriteDivBegin(pmkr);

    const char* pch = NULL;
    const char* pchEnd = NULL;
    if ( !bTrimmedFieldContents(pfld, &pch, &pchEnd) )  // If no field content
        return;  // 1999-05-18 MRP: No group tag for empty fields // 1.5.8wb Comment out this line to fix XML export bug of no group for empty key fld // 1.5.8yc Restore because it caused an assert
        
    OpenTag(pmkr);
    WriteFieldContents(pch, pchEnd, pmkr);  // 1998-08-20 MRP
}

BOOL XML_ostream::bTrimmedFieldContents(const CField* pfld,
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


void XML_ostream::WriteFieldContents(const char* pch, const char* pchEnd,
        const CMarker* pmkr)
{
    ASSERT( pch );
    ASSERT( pchEnd );
    ASSERT( pch <= pchEnd );
    ASSERT( pmkr );

    // m_ios << '<' << pmkr->sMarker() << '>';
    WriteStartTag(pmkr);  // 1999-10-11 MRP

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

    // m_ios << "</" << pmkr->sMarker() << ">\n";
    WriteEndTag(pmkr);  // 1999-10-11 MRP
    m_ios << '\n';  // 1999-10-11 MRP
}


BOOL XML_ostream::bAtControlSymbol(const char* pch, const char* pchEnd)
{
    const char* pchControlSymbol = NULL;
    int len = 0;
    return bControlSymbol(&pch, pchEnd, &pchControlSymbol, &len);
}

BOOL XML_ostream::bControlSymbol(const char** ppch, const char* pchEnd,
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

void XML_ostream::WriteControlSymbol(const char** ppch, const char* pchEnd)
{
    const char* pch = NULL;
    int len = 0;
    BOOL b = bControlSymbol(ppch, pchEnd, &pch, &len);
    ASSERT( b );
    ASSERT( pch );
    ASSERT( len != 0 );

    // 1998-08-21 MRP: Use for special HTML tags like BR
    // m_ios << '<';
    m_ios << s_pszSTAGO;  // 1999-10-11 MRP
    m_ios.write(pch, len);
    // m_ios << '>';
    m_ios << s_pszTAGC;  // 1999-10-11 MRP
}


BOOL XML_ostream::bAtSubfield(const char* pch, const char* pchEnd)
{
    const CMarker* pmkr = NULL;
    return bSubfieldBegin(&pch, pchEnd, &pmkr);
}

BOOL XML_ostream::bSubfieldBegin(const char** ppch, const char* pchEnd,
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

void XML_ostream::WriteSubfield(const char** ppch, const char* pchEnd,
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
    // m_ios << '<' << pmkr->sMarker() << '>';
    WriteStartTag(pmkr);  // 1999-10-11 MRP

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
    // m_ios << "</" << pmkr->sMarker() << '>';
    WriteEndTag(pmkr);  // 1999-10-11 MRP

    if ( pch != pchEnd && *pch == '}' )
        pch++;
	if ( pch > pchEnd ) // 1.3ca Fix bug of possible crash on Unicode export
		pch = pchEnd;
    *ppch = pch;
}


void XML_ostream::WriteRenderableSubstring(const char* pch,
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
				// if ( pch < pchEnd ) // 1.5.0gs Note possible fix for no trailing nl in XML output
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


void XML_ostream::WriteTab()
{
    m_ios << ' ';
}

void XML_ostream::WriteLine()
{
//    m_ios << '\n' << "<BR>";  // 1999-10-11 MRP: Start it on a new line
    // Use correct XML notation for an empty element
    m_ios << '\n' << "<br />";  // 2000-06-26 MRP
}

void XML_ostream::WriteSpace()
{
    m_ios << ' ';
}

void XML_ostream::WriteNonBreakingSpace()
{
    m_ios << ' ';  // ***
}

void XML_ostream::WriteChar(const char** ppsz)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );

    // if ( *psz == s_chNonBreakingSpace )
    //     ;  // ***

//	CDatabaseType* ptyp = m_pmkrset->ptyp(); // 1.5.7d Set up for XML export option to not escape wedges
    if ( !__isascii(*psz) && !m_bUTF8 )
        {
        unsigned char uch = *psz;
        int i = uch;
        m_ios << "&#" << i << ';';
        }
    else if( *psz == '<' ) // 1.5.1ne  // 1.5.7c Fix bug of XML export not escaping wedges
		{
//		if ( FALSE ) // 1.5.7d Set up for XML export option to not escape wedges
//			m_ios << "<"; // 1.5.7d Set up for XML export option to not escape wedges
//		else
	        m_ios << "&lt;"; // 1.5.1ne  // 1.5.7c 
		}
    else if ( *psz == '>' ) // 1.5.1ne  // 1.5.7c 
		{
//		if ( FALSE ) // 1.5.7d Set up for XML export option to not escape wedges
//			m_ios << ">"; // 1.5.7d Set up for XML export option to not escape wedges
//		else
	        m_ios << "&gt;"; // 1.5.1ne  // 1.5.7c 
		}
    else if ( *psz == '"' )
        m_ios << "&quot;";
    else if ( *psz == '&' )
        m_ios << "&amp;";
    else
        m_ios << *psz;
    
    *ppsz = psz + 1;  // Point to the next character
}

void XML_ostream::WriteChars(const char* pch, int len)
{
    ASSERT( pch );
    ASSERT( 0 <= len );
    const char* pchEnd = pch + len;
    while ( pch != pchEnd )
        WriteChar(&pch);
}
#ifdef UPTOBASECLASS
void XML_ostream::WriteString(const char* pszContents) // 1.4pcf Write string in XML, added for head extra info string
	{
    m_ios << pszContents; // 1.4kmb Write string with no extra markers or nl
	}
#endif