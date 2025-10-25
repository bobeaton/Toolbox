// pgs.h  Page Setup for export as Rich Text Format

#ifndef PGS_H
#define PGS_H


class Object_ostream;  // obstream.h
class Object_istream;  // obstream.h
class ostream;  // iostream.h

typedef double RtfUnits;  // Hungarian: un, unx, uny
typedef int RtfTwips;  // Hungarian: tw, twx, twy


class RtfPageSetup  // Hungarian: pgs
{
private:
    // Document formatting
    friend class CDocumentPage;
    int m_iPaperSize;
    RtfUnits m_unxPaperWidth;
    RtfUnits m_unyPaperHeight;
    RtfUnits m_unyTopMargin;
    RtfUnits m_unyBottomMargin;
    RtfUnits m_unxLeftMargin;  // Inside, if Mirror Margins
    RtfUnits m_unxRightMargin;  // Outside, if Mirror Margins
    RtfUnits m_unxGutter;
    BOOL m_bMirrorMargins;
    
    // Section formatting
    friend class CSectionPage;
    RtfUnits m_unyHeaderToEdge;
    RtfUnits m_unyFooterToEdge;
    BOOL m_bFirstPage;  // 1998-10-14 MRP
    BOOL m_bOddAndEven;
    BOOL m_bOnlyOneOrTwoColumns;
    int m_numColumns;
    RtfUnits m_unxColumnSpacing;
    BOOL m_bLineBetweenColumns;

public:
    RtfPageSetup(BOOL bOnlyOneOrTwoColumns = FALSE,
            RtfUnits txyTopMargin = 1.0, RtfUnits unyBottomMargin = 1.0,
            RtfUnits unxLeftMargin = 1.25, RtfUnits unxRightMargin = 1.25,
            RtfUnits unxGutter = 0.0, BOOL bMirrorMargins = FALSE,
            RtfUnits unyHeaderToEdge = 0.5, RtfUnits unyFooterToEdge = 0.5,
            int numColumns = 1, RtfUnits unxColumnSpacing = 0.5);

    RtfPageSetup(const RtfPageSetup& pgs);  // copy constructor
    void operator=(const RtfPageSetup& pgs);  // assignment operator

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
        BOOL bOnlyOneOrTwoColumns, RtfPageSetup** pppgs);

    void WriteDocumentFormatting(std::ostream& ios) const;
    void WriteSectionFormatting(std::ostream& ios,
            int numColumnsThisSection = 0) const;

    BOOL bCentimeters() const;
    RtfUnits unyHeaderToEdge() const { return m_unyHeaderToEdge; } // 1.3be Fix bug of not getting header and footer spacing right on first page of rtf export
    RtfUnits unyFooterToEdge() const { return m_unyFooterToEdge; } // 1.3be
    int numColumns() const { return m_numColumns; }
    double unxColWidth() const { return m_unxColumnSpacing; }

    RtfTwips twxLineWidth() const;  // 1998-07-13 MRP
    
    // 1998-10-14 MRP: First page and odd/even header and footer options.
    BOOL bFirstPage() const { return m_bFirstPage; }
    BOOL bOddAndEven() const { return m_bOddAndEven; }

    static const char* s_pszPaperSize(int iPaperSize);
    static int s_iPaperSize(const char* pszPaperSize);
    static BOOL s_bStandardSize(int iPaperSize);    
    static BOOL s_bCentimeters(int iPaperSize);
    static RtfUnits s_unxPaperWidth(int iPaperSize);
    static RtfUnits s_unyPaperHeight(int iPaperSize);
    static RtfUnits s_minun();
    static RtfUnits s_maxun(BOOL bCentimeters);
    static void s_RtfUnitsToString(RtfUnits un, BOOL bCentimeters,
            Str8& sUnits);
    static RtfTwips s_twFromRtfUnits(RtfUnits un, BOOL bCentimeters);

    BOOL bViewProperties();
    
private:
    void ReadProperties(Object_istream& obs, BOOL bOnlyOneOrTwoColumns);
};  // class RtfPageSetup

#endif  // not PGS_H
