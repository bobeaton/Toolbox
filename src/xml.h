// XML.h  Extensible Markup Language Export Process

// Class XML_ostream (derived from base class Field_ostream)
// converts Standard Format fields to XML-tagged elements
// which can then be displayed using web browsers.

#ifndef XML_H
#define XML_H

// 1998-12-02 MRP: This feature is omitted from the 16-bit version,
// mainly because there would be problems with long file names and
// the four-letter .html file name extension.

#include "sfstream.h"  // class Field_ostream

class CMarker;  // mkr.h
class CMarkerSet;  // mkr.h

// 1999-10-11 MRP: Don't write the Document Type Definition
// If anything, an XML Schema (.xsd file) would be better,
// but it's only a working draft.
// #define XML_DTD

// --------------------------------------------------------------------------

// Convert Standard Format fields to a stream of Extensible Markup Language.

class XML_ostream : public Field_ostream
{
private:
    Str8 m_sRootNode;
    BOOL m_bEmbeddedMarkup;  // Interpret embedded markup: |fv{...}
    CMarkerSet* m_pmkrset;    
    BOOL m_bUTF8;  // 1999-02-16 MRP: UCS Transformation Form, 8-bit form
    Str8 m_sCharset;  // 1999-02-15 MRP
    const Str8 m_sDTDName;
    const Str8 m_sStylesheetPath;

    // State of the conversion mechanism
    BOOL m_bHaveWrittenField;
    char* m_pchRenBuf;  // Buffer for output of a rendering DLL

    enum { s_maxlevHTML = 9 };
    const CMarker* m_apmkrHTML[s_maxlevHTML + 1];  // 1998-08-20 MRP
    
public:
    XML_ostream(ostream& ios, CMarkerSet* pmkrset,
            const Str8& sRootNode,
            BOOL bUTF8,
            const Str8& sDTDName,
            const Str8& sStylesheetPath);
    virtual ~XML_ostream();

    virtual void WriteField(const CField* pfld, BOOL bFirstField);
	BOOL bXML() { return TRUE; } // 1.4qnk Add func to test whether this is XML export


#ifdef XML_DTD
    static void s_WriteDTD(ostream& ios, const CMarkerSet* pmkrset,
            const Str8& sRootNode);
#endif

    void WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst); // 1.1cm Add alphabetic dividers

//	void WriteString(const char* pszContents); // 1.4pcf Write string in XML, added for head extra info string

private:
    void WriteHead();
        // Write the DTD, stylesheet, and opening root node tag.
    void WriteTail();
        // Close any unclosed tags and write the closing root note tag.
    
    void WriteField(const CField* pfld);

    // 1998-07-02 MRP: Make SDF rendering work at the subfield level.
    // The following functions consider pchEnd to be the first character
    // after the string: the position where there would ordinarily be
    // zero-termination character.

    BOOL bTrimmedFieldContents(const CField* pfld,
            const char** ppchBegin, const char** ppchEnd);
        // Return whether pfld is non-empty after white space
        // has been trimmed from the beginning and end of its contents.
        // If non-empty, its trimmed contents is [*ppchBegin, *ppchEnd).
        // *** MOVE TO Field_ostream base class in sfstream.h/.cpp!!!
    
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

    // 1998-08-20 MRP
    void InitOpenTags();
    void OpenTag(const CMarker* pmkr);
    void CloseTag(int levCur);

    // 1999-10-11 MRP
    void WriteStartTag(const CMarker* pmkr, BOOL bGroup = FALSE);
    void WriteEndTag(const CMarker* pmkr, BOOL bGroup = FALSE);
    void WriteStartTag(const Str8& sMarker, BOOL bGroup = FALSE);
    void WriteEndTag(const Str8& sMarker, BOOL bGroup = FALSE);
    void WriteMarker(const Str8& sMarker, BOOL bGroup = FALSE);
    static void s_WriteMarker(ostream& ios,
            const CMarker* pmkr, BOOL bGroup = FALSE);
    static void s_WriteMarker(ostream& ios,
            const Str8& sMarker, BOOL bGroup = FALSE);

#ifdef XML_DTD
    static void s_WriteElement(ostream& ios, const CMarker* pmkr);
    static void s_WriteMarkersInGroup(ostream& ios, const CMarker* pmkrGroup);
#endif
};  // class XML_ostream

#endif  // XML_H
