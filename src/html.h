// HTML.h  HyperText Markup Language Export Process

// Class HTML_ostream (derived from base class Field_ostream)
// converts Standard Format fields to HTML-tagged elements
// which can then be displayed using web browsers.

#ifndef HTML_H
#define HTML_H

// 1998-12-02 MRP: This feature is omitted from the 16-bit version,
// mainly because there would be problems with long file names and
// the four-letter .html file name extension.

#define EXPORT_WEB_PAGES
#ifdef EXPORT_WEB_PAGES

#include "sfstream.h"  // class Field_ostream

class CMarker;  // mkr.h
class CMarkerSet;  // mkr.h

// --------------------------------------------------------------------------

// Convert Standard Format fields to a stream of HyperText Markup Language.

class HTML_ostream : public Field_ostream
{
private:
    BOOL m_bEmbeddedMarkup;  // Interpret embedded markup: |fv{...}
    BOOL m_bHTML_XML;  // 1998-12-05 MRP
    BOOL m_bUTF8;  // 1999-02-16 MRP: UCS Transformation Form, 8-bit form
    Str8 m_sCharset;  // 1999-02-15 MRP
    const Str8 m_sTitle;
    const Str8 m_sBaseTarget;
    const Str8 m_sStylesheetPath;
    const Str8 m_sCopyright;  // 1998-12-11 MRP
    const Str8 m_sGenerator;  // 1999-02-04 MRP
    BOOL m_bNoRobots;  // 1999-02-15 MRP
        // Tell visiting [search engine] robots that these web pages
        // shouldn't be indexed, nor used to harvest more links.

    CMarkerSet* m_pmkrset;    
    char* m_pchRenBuf;  // Buffer for output of a rendering DLL

    // State of the conversion mechanism
    BOOL m_bHaveWrittenField;
    BOOL m_bPar;  // Have already output a paragraph
    BOOL m_bCharInThisPar;  // Have already output a char style in this par
    BOOL m_bWhiteSpace;  // The last character output was white space

    enum { s_maxlevHTML = 9 };
    const CMarker* m_apmkrHTML[s_maxlevHTML + 1];  // 1998-08-20 MRP
    
public:
    HTML_ostream(std::ostream& ios, CMarkerSet* pmkrset,
            BOOL bHTML_XML, BOOL bUTF8,
            const Str8& sTitle,
            const Str8& sBaseTarget,
            const Str8& sStylesheetPath,
            const Str8& sCopyright,
            const Str8& sGenerator,
            BOOL bNoRobots);
    virtual ~HTML_ostream();

    virtual void WriteField(const CField* pfld, BOOL bFirstField);
        // Note: If a record's first field has a character style,
        // a new Normal paragraph is automatically started.

    static Str8 s_sConvertURL(const Str8& sURL, BOOL bAnchor = FALSE);
    static Str8 s_sConvertURL(const char* pch, const char* pchEnd,
            BOOL bAnchor = FALSE);  // 1999-02-19 MRP
        // The only valid characters in a URL (i.e. a link HREF="..."
        // or the file name of a web page) are letters and space.
        // In an anchor NAME="...", we do allow all 7-bit characters,
        // but convert 8-bit characters to avoid conflict with UTF-8.
        //
        // Convert ALL characters except letters and space to decimal codes.
        // Separate codes from adjacent characters (or codes) by an underline.
        // Return the converted valid URL corresponding either to sURL or
        // to the characters [pch, pchEnd).
        //
        // Note: Some web browsers convert special characters in URLs
        // to hexadecimal %xx, but to be consistent with character references
        // &#ddd; in the body of a web page, we'll use decimal codes here too.

private:
    void WriteHead();
        // Write the <HTML> tag, HTML head, and opening <BODY> tag.
    void WriteTail();
        // Close any unclosed tags.
        // Write the closing </BODY> and </HTML> tags.
    
    void WriteFieldAsSPAN(const CField* pfld);
        // Write pfld whose marker has Style to Export: Character
        // as a SPAN with the marker as its class.
    void WriteFieldAsDIV(const CField* pfld);
        // Write pfld whose marker has Style to Export: Paragraph
        // as a SPAN with the marker as its class.
    void WriteFieldAsHTML(const CField* pfld);
        // Markers in all upper-case are considered to be
        // built-in HTML tags (e.g. TABLE, TR, TD).

    // 1998-07-02 MRP: Make SDF rendering work at the subfield level.
    // The following functions consider pchEnd to be the first character
    // after the string: the position where there would ordinarily be
    // zero-termination character.

    BOOL bTrimmedFieldContents(const CField* pfld,
            const char** ppchBegin, const char** ppchEnd);
        // Return whether pfld is non-empty after white space
        // has been trimmed from the beginning and end of its contents.
        // If non-empty, its trimmed contents is [*ppchBegin, *ppchEnd).
    
    void WriteFieldContents(const char* pch, const char* pchEnd,
            const CMarker* pmkr);
        // Write the trimmed contents [pch, pchEnd) of a field
        // whose marker is pmkr.
        // Note: If pmkr has has Style to Export: Paragraph,
        // then pstyChar should be NULL.

    BOOL bAtControlSymbol(const char* pch, const char* pchEnd);
        // Return whether a |{...} control symbol begins at pch.
    BOOL bControlSymbol(const char** ppch, const char* pchEnd,
            const char** ppchControlSymbol, int* plenControlSymbol);
        // Return whether a |{...} control symbol begins at *ppch.
        // If so, return via *ppch the first position after it,
        // *ppchControlSymbol and *plenControlSymbol: its contents.
    void WriteControlSymbol(const char** ppch, const char* pchEnd);
        // Write a |{...} control symbol beginning at *ppch
        // that has been found using bAtControlSymbol.
        // Return via *ppch the first position after it.

    // 1999-01-23 MRP: Convert non-alphabetic characters in URLs.
    BOOL bWroteAnchorOrLink(const char* pch, const char* pchEnd);
    BOOL bWroteAnchor(const char** ppch, const char* pchEnd);
    BOOL bWroteLink(const char** ppch, const char* pchEnd);
    // BOOL bFirstPrimary(const char* pch, const char* pchEnd,
    //         Str8& sChar);  // 2000-04-17 MRP: commented out

    BOOL bAtSubfield(const char* pch, const char* pchEnd);
        // Return whether a |mkr{...} subfield begins at pch.
    BOOL bSubfieldBegin(const char** ppch, const char* pchEnd,
            const CMarker** ppmkr);
        // Return whether a |mkr{...} subfield begins at *ppch.
        // If so, return via *ppch the beginning of its contents,
        // *ppmkr its marker, and *ppstyChar its style.
    void WriteSubfield(const char** ppch, const char* pchEnd,
            const CMarker* pmkrSurrounding, BOOL bInFootnote);
        // Write a |mkr{...} subfield beginning at *ppch
        // that has been found using bAtSubfield.
        // Note: If this is a first-level subfield in a footnote
        // or a field whose marker has Style to Export: Paragraph,
        // then pstySurroundingChar should be NULL.
        // Return via *ppch the first position after the subfield.
        // Recursive embedding of subfields is allowed,
        // but the RTF output is disjoint character style groups.

    void WriteRenderableSubstring(const char* pch,
            const char* pchEnd, const CMarker* pmkr);
        // Write a substring [pch, pchEnd) whose marker is pmkr.
        // If pmkr's language encoding uses SDF rendering for display,
        // apply those context-sensitive rules to the RTF output.
        // However, right-to-left text reversal can't occur here,
        // because it depends on line layout in the document.
        // Note: Control symbols |{...} and subfield begin |mkr{ and
        // end tags } shouldn't be processed by the rendering DLL,
        // therefore the higher-level logic divides the field
        // into renderable substrings.

    void WriteParEnd();  // End of the previous paragraph
    void WriteTab();
    void WriteLine();  // Required line break (only in non-wrapped fields)
    void WriteSpace();
    void WriteNonBreakingSpace();
    void WriteChar(const char** ppsz);
    void WriteChars(const char* pch, int len);  // 1998-07-13

    void WriteCharset();  // 1999-02-15 MRP
    void WriteMETA(const char* pszName, const Str8& sContent);
        // 1999-02-04 MRP: Information in the Head of web pages, e.g.
        // <META NAME="generator" CONTENT="Multi-Dictionary Formatter">
        // Don't write anything is sContent is empty.

    // 1998-08-20 MRP
    void InitOpenTags();
    void OpenTag(const CMarker* pmkr);
    void CloseTag(int levCur);
};  // class HTML_ostream

#endif  // EXPORT_WEB_PAGES
#endif  // HTML_H
