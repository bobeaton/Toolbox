// rtf.h  Rich Text Format

// RTF provides a standard format for interchanging text regardless of
// the output device, operating system, or operating environment.
// RTF text is a form of encoding for various text formatting properties,
// document structures, and document properties.
// [Microsoft Systems Journal, v.2, n.1, March, 1987, pp. 63-70.]

// Class RTF_ostream (derived from base class Field_ostream)
// converts Standard Format fields to Rich Text Format output
// which can then be read and printed by Microsoft Word.

#ifndef RTF_H
#define RTF_H

#include "sfstream.h"  // class Field_ostream

class CMarker;  // mkr.h
class CMarkerSet;  // mkr.h
class RtfPageSetup;  // pgs.h
class StyleSet;  // rtf.cpp
class CharStyle;  // rtf.cpp
class ParStyle;  // rtf.cpp

typedef int RtfTwips;  // pgs.h - Hungarian: tw, twx, twy

// --------------------------------------------------------------------------

// Convert Standard Format fields to a stream of Rich Text Format output.

class RTF_ostream : public Field_ostream
{
private:
    BOOL m_bEmbeddedMarkup;  // Interpret embedded markup: |fv{...}
    BOOL m_bAutoUpdateStyles;  // 1997-11-24 MRP
    BOOL m_bMarkerFontFormatting;

    // 1998-07-11 MRP: Tab-aligned interlinear text
    // is an experimental feature in version 4.0.
    BOOL m_bInterlinearTabAligned;
    RtfTwips m_twxInterlinearSpacing;  // Nominal character width in twips
    BOOL m_bInterlinearRtl;
        // The dominant text direction is determined by the first field.
        // Unlike ordinary data fields, right-to-left text reversal
        // for interlinear fields is done by RTF Export in Shoebox,
        // not by the Rtl Reverser after the document is opened in Word.
    
    Str8 m_sHeaderLeft;
    Str8 m_sHeaderRight;
    Str8 m_sFooterCenter;
    BOOL m_bFooterInsideDate;
    Str8 m_sDocumentTemplatePath;
    const RtfPageSetup* m_ppgs;
    
    StyleSet* m_pstyset;
    char* m_pchRenBuf;  // Buffer for output of a rendering DLL

    // State of the conversion mechanism
    BOOL m_bHaveWrittenField;
    int m_iSectAfter;  // If non-zero, insert a section break after this par
    BOOL m_bPar;  // Have already output a paragraph
    BOOL m_bCharInThisPar;  // Have already output a char style in this par
    BOOL m_bWhiteSpace;  // The last character output was white space
    
public:
    RTF_ostream(std::ostream& ios, CMarkerSet* pmkrset,
            BOOL bAutoUpdateStyles, BOOL bMarkerFontFormatting,
            BOOL bInterlinearTabAligned, RtfTwips twxInterlinearSpacing,
            const Str8& sHeaderLeft, const Str8& sHeaderRight,
            const Str8& sFooterCenter, BOOL bFooterInsideDate,
            const Str8& sDocumentTemplatePath,
            const RtfPageSetup* ppgs);
    virtual ~RTF_ostream();

    virtual void WriteField(const CField* pfld, BOOL bFirstField);
        // Note: If a record's first field has a character style,
        // a new Normal paragraph is automatically started.
    
    void WriteFieldAsCharStyle(const CField* pfld, const CharStyle* psty);
        // Write pfld whose marker has Style to Export: Character
        // with psty (whose name corresponds to the marker's field name).
    void WriteFieldAsParStyle(const CField* pfld, const ParStyle* psty);
        // Write pfld whose marker has Style to Export: Paragraph
        // with psty (whose name corresponds to the marker's field name).

    void WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst ); // 1.1cm Add alphabetic dividers

	void WriteMultipleColumnEnd(); // 1.2du On rtf export double column, even up column ends on last page

    const RtfPageSetup* ppgsRtfPageSetup() { return m_ppgs; }; // 1.2dr Allow access to page setup information

private:
    void WriteHead(BOOL bUnicode);
    void WriteTail();
    // 1998-10-14 MRP: First page and odd/even header and footer options.
    void WriteHeadersAndFooters(BOOL bUnicode);
    void WriteHeader(const char* pszControlWord, BOOL bUnicode);
    void WriteFooter(const char* pszControlWord, BOOL bUnicode, BOOL bFirstOrOdd);

    // 1998-07-02 MRP: Make SDF rendering work at the subfield level.
    // The following functions consider pchEnd to be the first character
    // after the string: the position where there would ordinarily be
    // zero-termination character.

    BOOL bTrimmedFieldContents(const CField* pfld,
            const char** ppchBegin, const char** ppchEnd);
        // Return whether pfld is non-empty after white space
        // has been trimmed from the beginning and end of its contents.
        // If non-empty, its trimmed contents is [*ppchBegin, *ppchEnd).

    // 1998-07-11 MRP: Experimental feature in version 4.0.
    void WriteTabAlignedFieldContents(const char* pch, const char* pchEnd,
            const CMarker* pmkr);
        // Write the trimmed contents [pch, pchEnd) of an interlinear field
        // whose marker is pmkr, using tab alignment.
        // Note: Does SDF rendering and right-to-left text reversal.
    void WriteTabAlignedFieldContentsLtr(const char* pch, const char* pchEnd,
            const CMarker* pmkr);
        // When the dominant text direction is left-to-right.
    void WriteTabAlignedFieldContentsRtl(const char* pch, const char* pchEnd,
            const CMarker* pmkr);
        // When the dominant text direction is right-to-left.
    BOOL bWroteMultiLineFieldContents(const char* pch, const char* pchEnd,
            const CMarker* pmkr);
        // Returns TRUE if an aligned field contains explicit line breaks,
        // and, if so, writes it out line-by-line without any tab stops.
        // Because all the tab stops would be common to the entire paragraph,
        // not set individually for each line, alignment wouldn't work right.
        // Besides, such a field probably hasn't been interlinearized yet.
    
    void WriteFieldContents(const char* pch, const char* pchEnd,
            const CMarker* pmkr, const CharStyle* pstyChar);
        // Write the trimmed contents [pch, pchEnd) of a field
        // whose marker is pmkr.
        // Note: If pmkr has has Style to Export: Paragraph,
        // then pstyChar should be NULL.

	BOOL bAtRawOutput(const char* pch, const char* pchEnd); // 1.4rad Add bar square bracket for raw output to export
	void WriteRawOutput(const char** ppch, const char* pchEnd); // 1.4rad

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

    BOOL bAtSubfield(const char* pch, const char* pchEnd);
        // Return whether a |mkr{...} subfield begins at pch.
    BOOL bSubfieldBegin(const char** ppch, const char* pchEnd,
            const CMarker** ppmkr, const CharStyle** ppstyChar);
        // Return whether a |mkr{...} subfield begins at *ppch.
        // If so, return via *ppch the beginning of its contents,
        // *ppmkr its marker, and *ppstyChar its style.
    void WriteSubfield(const char** ppch, const char* pchEnd,
            const CharStyle* pstySurroundingChar, BOOL bInFootnote);
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

    void WriteString(const char* psz, BOOL bUnicode, BOOL bConvertRTFfields = FALSE);
        // Escape literal backslashes and braces.
        // If bConvertFields, convert {DATE}, {PAGE}, etc. to RTF fields.
    BOOL bWroteRTFfield(const char** ppsz, BOOL bUnicode);
    void WriteSect();  // Section break
    void WriteParBegin(const ParStyle* psty);
    void WriteParEnd();  // End of the previous paragraph
    void WriteTab();
    void WriteLine();  // Required line break (only in non-wrapped fields)
    void WriteSpace( BOOL bUnicode ); // 1.4ywj 
    void WriteNonBreakingSpace();
    void WriteChar(const char** ppsz, BOOL bUnicode);
    void WriteChars(const char* pch, int len, BOOL bUnicode);  // 1998-07-13
    
#ifdef _DEBUG
public:
    static void s_Test();
#endif  // _DEBUG
};  // class RTF_ostream

#endif  // RTF_H
