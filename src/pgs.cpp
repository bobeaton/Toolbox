// pgs.cpp  Page Setup for export as Rich Text Format

#include "stdafx.h"
#include "toolbox.h"
#include "pgs.h"
#include "obstream.h"  // Object_ostream, Object_istream
#include <iostream>
using namespace std;  // ostream

#include "pgs_d.h"  // CPageSetupSheet


RtfPageSetup::RtfPageSetup(BOOL bOnlyOneOrTwoColumns,
        RtfUnits unyTopMargin, RtfUnits unyBottomMargin,
        RtfUnits unxLeftMargin, RtfUnits unxRightMargin,
        RtfUnits unxGutter, BOOL bMirrorMargins,
        RtfUnits unyHeaderToEdge, RtfUnits unyFooterToEdge,
        int numColumns, RtfUnits unxColumnSpacing) :
    m_iPaperSize(0),
    m_unxPaperWidth(s_unxPaperWidth(m_iPaperSize)),
    m_unyPaperHeight(s_unyPaperHeight(m_iPaperSize)),
    m_unyTopMargin(unyTopMargin),
    m_unyBottomMargin(unyBottomMargin),
    m_unxLeftMargin(unxLeftMargin),
    m_unxRightMargin(unxRightMargin),
    m_unxGutter(unxGutter),
    m_bMirrorMargins(bMirrorMargins),
    m_unyHeaderToEdge(unyHeaderToEdge),
    m_unyFooterToEdge(unyFooterToEdge),
    m_bFirstPage(FALSE),  // 1998-10-14 MRP
    m_bOddAndEven(TRUE),
    m_bOnlyOneOrTwoColumns(bOnlyOneOrTwoColumns),
    m_numColumns(numColumns),
    m_unxColumnSpacing(unxColumnSpacing),
    m_bLineBetweenColumns(FALSE)
{
}

RtfPageSetup::RtfPageSetup(const RtfPageSetup& pgs)
{
    m_iPaperSize =           pgs.m_iPaperSize;
    m_unxPaperWidth =        pgs.m_unxPaperWidth;
    m_unyPaperHeight =       pgs.m_unyPaperHeight;
    m_unyTopMargin =         pgs.m_unyTopMargin;
    m_unyBottomMargin =      pgs.m_unyBottomMargin;
    m_unxLeftMargin =        pgs.m_unxLeftMargin;
    m_unxRightMargin =       pgs.m_unxRightMargin;
    m_unxGutter =            pgs.m_unxGutter;
    m_bMirrorMargins =       pgs.m_bMirrorMargins;
    
    m_unyHeaderToEdge =      pgs.m_unyHeaderToEdge;
    m_unyFooterToEdge =      pgs.m_unyFooterToEdge;
    m_bFirstPage =			 pgs.m_bFirstPage;  // 1998-10-14 MRP
    m_bOddAndEven =          pgs.m_bOddAndEven;
    m_bOnlyOneOrTwoColumns = pgs.m_bOnlyOneOrTwoColumns;
    m_numColumns =           pgs.m_numColumns;
    m_unxColumnSpacing =     pgs.m_unxColumnSpacing;
    m_bLineBetweenColumns =  pgs.m_bLineBetweenColumns;
}

void RtfPageSetup::operator=(const RtfPageSetup& pgs)
{
    m_iPaperSize =           pgs.m_iPaperSize;
    m_unxPaperWidth =        pgs.m_unxPaperWidth;
    m_unyPaperHeight =       pgs.m_unyPaperHeight;
    m_unyTopMargin =         pgs.m_unyTopMargin;
    m_unyBottomMargin =      pgs.m_unyBottomMargin;
    m_unxLeftMargin =        pgs.m_unxLeftMargin;
    m_unxRightMargin =       pgs.m_unxRightMargin;
    m_unxGutter =            pgs.m_unxGutter;
    m_bMirrorMargins =       pgs.m_bMirrorMargins;
    
    m_unyHeaderToEdge =      pgs.m_unyHeaderToEdge;
    m_unyFooterToEdge =      pgs.m_unyFooterToEdge;
    m_bFirstPage =			 pgs.m_bFirstPage;  // 1998-10-14 MRP
    m_bOddAndEven =          pgs.m_bOddAndEven;
    m_bOnlyOneOrTwoColumns = pgs.m_bOnlyOneOrTwoColumns;
    m_numColumns =           pgs.m_numColumns;
    m_unxColumnSpacing =     pgs.m_unxColumnSpacing;
    m_bLineBetweenColumns =  pgs.m_bLineBetweenColumns;
}


static const char* psz_pgs =                "rtfPageSetup";

static const char* psz_PaperSize =          "paperSize";
static const char* psz_PaperWidth =         "paperWidth";
static const char* psz_PaperHeight =        "paperHeight";
static const char* psz_TopMargin =          "topMargin";
static const char* psz_BottomMargin =       "bottomMargin";
static const char* psz_LeftMargin =         "leftMargin";
static const char* psz_RightMargin =        "rightMargin";
static const char* psz_Gutter =             "gutter";
static const char* psz_MirrorMargins =      "MirrorMargins";

static const char* psz_HeaderToEdge =       "headerToEdge";
static const char* psz_FooterToEdge =       "footerToEdge";
static const char* psz_FirstPage =          "FirstPage";  // 1998-10-14 MRP
static const char* psz_NoOddAndEven =       "NoOddAndEven";
static const char* psz_Columns =            "columns";
static const char* psz_ColumnSpacing =      "columnSpacing";
static const char* psz_LineBetweenColumns = "LineBetweenColumns";

#ifdef typWritefstream // 1.6.4ac
void RtfPageSetup::WriteProperties(Object_ofstream& obs) const
#else
void RtfPageSetup::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_pgs);

    Str8 sUnits;
    BOOL bCentimeters = s_bCentimeters(m_iPaperSize);

    obs.WriteString(psz_PaperSize, s_pszPaperSize(m_iPaperSize));
    if ( !s_bStandardSize(m_iPaperSize) )
        {
        s_RtfUnitsToString(m_unxPaperWidth, bCentimeters, sUnits);
        obs.WriteString(psz_PaperWidth, sUnits);
        s_RtfUnitsToString(m_unyPaperHeight, bCentimeters, sUnits);
        obs.WriteString(psz_PaperHeight, sUnits);
        }
    s_RtfUnitsToString(m_unyTopMargin, bCentimeters, sUnits);
    obs.WriteString(psz_TopMargin, sUnits);
    s_RtfUnitsToString(m_unyBottomMargin, bCentimeters, sUnits);
    obs.WriteString(psz_BottomMargin, sUnits);
    s_RtfUnitsToString(m_unxLeftMargin, bCentimeters, sUnits);
    obs.WriteString(psz_LeftMargin, sUnits);
    s_RtfUnitsToString(m_unxRightMargin, bCentimeters, sUnits);
    obs.WriteString(psz_RightMargin, sUnits);
    s_RtfUnitsToString(m_unxGutter, bCentimeters, sUnits);
    obs.WriteString(psz_Gutter, sUnits);
    obs.WriteBool(psz_MirrorMargins, m_bMirrorMargins);
    
    s_RtfUnitsToString(m_unyHeaderToEdge, bCentimeters, sUnits);
    obs.WriteString(psz_HeaderToEdge, sUnits);
    s_RtfUnitsToString(m_unyFooterToEdge, bCentimeters, sUnits);
    obs.WriteString(psz_FooterToEdge, sUnits);
    obs.WriteBool(psz_FirstPage, m_bFirstPage);  // 1998-10-14 MRP
    obs.WriteBool(psz_NoOddAndEven, !m_bOddAndEven);
    obs.WriteInteger(psz_Columns, m_numColumns);
    s_RtfUnitsToString(m_unxColumnSpacing, bCentimeters, sUnits);
    obs.WriteString(psz_ColumnSpacing, sUnits);
    obs.WriteBool(psz_LineBetweenColumns, m_bLineBetweenColumns);
    
    obs.WriteEndMarker(psz_pgs);
}

BOOL RtfPageSetup::s_bReadProperties(Object_istream& obs,
        BOOL bOnlyOneOrTwoColumns, RtfPageSetup** pppgs)
{
    if ( !obs.bReadBeginMarker(psz_pgs) )
        return FALSE;
        
    RtfPageSetup* ppgs = new RtfPageSetup(bOnlyOneOrTwoColumns);
    ppgs->ReadProperties(obs, bOnlyOneOrTwoColumns);
    
    ASSERT( pppgs );
    if ( *pppgs )
        delete ppgs;
    else
        *pppgs = ppgs;
    return TRUE;
}

void RtfPageSetup::ReadProperties(Object_istream& obs, BOOL bOnlyOneOrTwoColumns)
{
    Str8 s;
    BOOL b = FALSE;

    RtfUnits unxPaperWidth = 0.0;
    RtfUnits unyPaperHeight = 0.0;
    RtfUnits unyTopMargin = 0.0;
    RtfUnits unyBottomMargin = 0.0;
    RtfUnits unxLeftMargin = 0.0;
    RtfUnits unxRightMargin = 0.0;
    RtfUnits unxGutter = 0.0;
    
    RtfUnits unyHeaderToEdge = 0.0;
    RtfUnits unyFooterToEdge = 0.0;
    int numColumns = 0;
    RtfUnits unxColumnSpacing = 0.0;

    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadString(psz_PaperSize, s) )
            m_iPaperSize = s_iPaperSize(s);
        else if ( obs.bReadDecimal(psz_PaperWidth, unxPaperWidth) )
            ;
        else if ( obs.bReadDecimal(psz_PaperHeight, unyPaperHeight) )
            ;
        else if ( obs.bReadDecimal(psz_TopMargin, unyTopMargin) )
            ;
        else if ( obs.bReadDecimal(psz_BottomMargin, unyBottomMargin) )
            ;
        else if ( obs.bReadDecimal(psz_LeftMargin, unxLeftMargin) )
            ;
        else if ( obs.bReadDecimal(psz_RightMargin, unxRightMargin) )
            ;
        else if ( obs.bReadDecimal(psz_Gutter, unxGutter) )
            ;
        else if ( obs.bReadBool(psz_MirrorMargins, m_bMirrorMargins) )
            ;
        else if ( obs.bReadDecimal(psz_HeaderToEdge, unyHeaderToEdge) )
            ;
        else if ( obs.bReadDecimal(psz_FooterToEdge, unyFooterToEdge) )
            ;
        else if ( obs.bReadBool(psz_FirstPage, m_bFirstPage) )  // 1998-10-14 MRP
        	;
        else if ( obs.bReadBool(psz_NoOddAndEven, b) )
            m_bOddAndEven = FALSE;
        else if ( obs.bReadInteger(psz_Columns, numColumns) )
            ;
        else if ( obs.bReadDecimal(psz_ColumnSpacing, unxColumnSpacing) )
            ;
        else if ( obs.bReadBool(psz_LineBetweenColumns, m_bLineBetweenColumns) )
            ;
        else if ( obs.bEnd(psz_pgs) )
            break;
        }

    const RtfUnits minun = s_minun();
    const RtfUnits maxun = s_maxun(s_bCentimeters(m_iPaperSize));
    m_unxPaperWidth = s_unxPaperWidth(m_iPaperSize);
    m_unyPaperHeight = s_unyPaperHeight(m_iPaperSize);
    if ( !s_bStandardSize(m_iPaperSize) )
        {
        if ( minun <= unxPaperWidth && unxPaperWidth <= maxun )
            m_unxPaperWidth = unxPaperWidth;
        if ( minun <= unyPaperHeight && unyPaperHeight <= maxun )
            m_unyPaperHeight = unyPaperHeight;
        }

    if ( minun <= unyTopMargin && unyTopMargin <= m_unyPaperHeight )
        m_unyTopMargin = unyTopMargin;
    if ( minun <= unyBottomMargin && unyBottomMargin <= m_unyPaperHeight )
        m_unyBottomMargin = unyBottomMargin;
    if ( minun <= unxLeftMargin && unxLeftMargin <= m_unxPaperWidth )
        m_unxLeftMargin = unxLeftMargin;
    if ( minun <= unxRightMargin && unxRightMargin <= m_unxPaperWidth )
        m_unxRightMargin = unxRightMargin;
    if ( 0.0 <= unxGutter && unxGutter <= m_unxPaperWidth )
        m_unxGutter = unxGutter;

    if ( minun <= unyHeaderToEdge && unyHeaderToEdge <= m_unyPaperHeight )
        m_unyHeaderToEdge = unyHeaderToEdge;
    if ( minun <= unyFooterToEdge && unyFooterToEdge <= m_unyPaperHeight )
        m_unyFooterToEdge = unyFooterToEdge;

    int maxnumColumns = ( bOnlyOneOrTwoColumns ? 2 : 4 );
    if ( 0 < numColumns && numColumns <= maxnumColumns )
        m_numColumns = numColumns;
    if ( minun <= unxColumnSpacing && unxColumnSpacing <= m_unxPaperWidth )
        m_unxColumnSpacing = unxColumnSpacing;
}

void RtfPageSetup::WriteDocumentFormatting(std::ostream& ios) const
{
    // Document formatting properties, such as margins and footnote placement.
    // Note: A4 paper width is 11909 (210 mm) and height is 16834 (297 mm)
    BOOL bCentimeters = s_bCentimeters(m_iPaperSize);
    ios << "\\paperw" << s_twFromRtfUnits(m_unxPaperWidth, bCentimeters);
    ios << "\\paperh" << s_twFromRtfUnits(m_unyPaperHeight, bCentimeters);
    ios << "\\margl" << s_twFromRtfUnits(m_unxLeftMargin, bCentimeters);
    ios << "\\margr" << s_twFromRtfUnits(m_unxRightMargin, bCentimeters);
    ios << "\\margt" << s_twFromRtfUnits(m_unyTopMargin, bCentimeters);
    ios << "\\margb" << s_twFromRtfUnits(m_unyBottomMargin, bCentimeters);
    if ( m_bOddAndEven )
        ios << "\\facingp";
    if ( m_unxGutter != 0 )
        ios << "\\gutter" << s_twFromRtfUnits(m_unxGutter, bCentimeters);
    if ( m_bMirrorMargins )
        ios << "\\margmirror";
    ios << '\n';
}

void RtfPageSetup::WriteSectionFormatting(std::ostream& ios,
        int numColumnsThisSection) const
{
    BOOL bCentimeters = s_bCentimeters(m_iPaperSize);
    ios << "\\sectd\\sbknone\n";  // Continuous section break
    int num = (numColumnsThisSection != 0 ? numColumnsThisSection : m_numColumns);
    ios << "\\cols" << num << '\n';
    ios << "\\colsx" << s_twFromRtfUnits(m_unxColumnSpacing, bCentimeters) << '\n';
    if ( m_bLineBetweenColumns )
        ios << "\\linebetcol\n";
    if ( m_bFirstPage )
        ios << "\\titlepg";  // 1998-10-14 MRP
    ios << "\\headery" << s_twFromRtfUnits(m_unyHeaderToEdge, bCentimeters) << '\n';
    ios << "\\footery" << s_twFromRtfUnits(m_unyFooterToEdge, bCentimeters) << '\n';
}

BOOL RtfPageSetup::bCentimeters() const
{
    return s_bCentimeters(m_iPaperSize);
}

RtfTwips RtfPageSetup::twxLineWidth() const
{
    BOOL bCentimeters = s_bCentimeters(m_iPaperSize);
    RtfTwips twx = s_twFromRtfUnits(m_unxPaperWidth, bCentimeters);
    twx -= s_twFromRtfUnits(m_unxLeftMargin, bCentimeters);    
    twx -= s_twFromRtfUnits(m_unxRightMargin, bCentimeters);    
    twx -= s_twFromRtfUnits(m_unxGutter, bCentimeters);    
    
    if ( 1 < m_numColumns )
        twx -= (m_numColumns - 1) *
            s_twFromRtfUnits(m_unxColumnSpacing, bCentimeters);

    if ( twx < 0 )
        twx = 0;

    return twx;    
}

RtfTwips RtfPageSetup::s_twFromRtfUnits(RtfUnits un, BOOL bCentimeters)
{
    return bCentimeters ?
        (RtfTwips)(un * 1440.0 / 2.54 + 0.5) :
        (RtfTwips)(un * 1440.0 + 0.5)
        ;
}

void RtfPageSetup::s_RtfUnitsToString(RtfUnits un, BOOL bCentimeters,
        Str8& sUnits)
{
    ASSERT( 0.0 <= un );
    ASSERT( un <= RtfPageSetup::s_maxun(bCentimeters) );
    char pszUnits[7] = "00.000";
    // 1997-09-16 MRP: At first values in centimeters got only one
    // digit after the decimal point, but this caused values of
    // margins and such to get mangled when changing paper sizes.
	sprintf_s(pszUnits, sizeof(pszUnits), "%6.3f", un);
    if ( pszUnits[5] == '0' )
        {
        pszUnits[5] = '\0';
        if ( pszUnits[4] == '0' )
            {
            pszUnits[4] = '\0';
            if ( pszUnits[3] == '0' )
                {
                pszUnits[2] = '\0';
                pszUnits[3] = '\0';
                }
            }
        }
    sUnits = ( pszUnits[0] == ' ' ? pszUnits + 1 : pszUnits );
}


static const int s_numPaperSizes = 5;
static const char* s_apszPaperSize[s_numPaperSizes] = { "letter", "legal", "A4", "custom_size_in", "custom_size_cm" };
static const BOOL s_abStandardSize[s_numPaperSizes] = { TRUE, TRUE, TRUE, FALSE, FALSE };
static const BOOL s_abCentimeters[s_numPaperSizes] = { FALSE, FALSE, TRUE, FALSE, TRUE };
static const RtfUnits s_aunxPaperWidth[s_numPaperSizes] = { 8.5, 8.5, 21.0, 8.5, 21.0 };
static const RtfUnits s_aunyPaperHeight[s_numPaperSizes] = { 11.0, 14.0, 29.7, 11.0, 21.7 };

int RtfPageSetup::s_iPaperSize(const char* pszPaperSize)
{
    ASSERT( pszPaperSize );
    int i;
    for ( i = 0; i != s_numPaperSizes; i++ )
        if ( strcmp(pszPaperSize, s_apszPaperSize[i]) == 0 )
            return i;

    return 0;
}

const char* RtfPageSetup::s_pszPaperSize(int iPaperSize)
{
    if ( iPaperSize < 0 || s_numPaperSizes <= iPaperSize )
        iPaperSize = 0;
    return s_apszPaperSize[iPaperSize];
}

BOOL RtfPageSetup::s_bStandardSize(int iPaperSize)
{
    if ( iPaperSize < 0 || s_numPaperSizes <= iPaperSize )
        iPaperSize = 0;
    return s_abStandardSize[iPaperSize];
}

BOOL RtfPageSetup::s_bCentimeters(int iPaperSize)
{
    if ( iPaperSize < 0 || s_numPaperSizes <= iPaperSize )
        iPaperSize = 0;
    return s_abCentimeters[iPaperSize];
}

RtfUnits RtfPageSetup::s_unxPaperWidth(int iPaperSize)
{
    if ( iPaperSize < 0 || s_numPaperSizes <= iPaperSize )
        iPaperSize = 0;
    return s_aunxPaperWidth[iPaperSize];
}

RtfUnits RtfPageSetup::s_unyPaperHeight(int iPaperSize)
{
    if ( iPaperSize < 0 || s_numPaperSizes <= iPaperSize )
        iPaperSize = 0;
    return s_aunyPaperHeight[iPaperSize];
}

RtfUnits RtfPageSetup::s_minun()
{
    return 0.1;
}

RtfUnits RtfPageSetup::s_maxun(BOOL bCentimeters)
{
    // Although the Rich Text Format specification allows numeric values
    // to range between -32767 and 32767 inclusive, Microsoft Word
    // limits values to the range -31680 to 31680 twips inclusive.
    // This is 55.88 centimeters and 22.0 inches in measurement units.
    return bCentimeters ? 55.88 : 22.0;
}


BOOL RtfPageSetup::bViewProperties()
{
    CRtfPageSetupSheet dlg(this);
    return ( dlg.DoModal() == IDOK );
}
