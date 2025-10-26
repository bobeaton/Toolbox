// CPrintProperties implementation
// 1996-05-31
// Rod Early

#include "stdafx.h"
#include "toolbox.h"
#include "project.h"
#include "shwdoc.h"
#include "shwview.h"
#include "printinf.h"
#include "tools.h"
#include "obstream.h"  // Object_istream, Object_ostream

#include <stdlib.h>



// Add a new print position to the end of the list, also returning
// a pointer to the newly created position element.
void CPrintPosList::Add(const CNumberedRecElPtr& prel, const CRecPos& rps ) // Insert new at end
{
    CDblList::Add((CDblListEl*)new CPrintPosition(prel, rps ) );
}


CSize CHeaderInfo::sizGetDimensions(CDC* pDC)
{
    CSize sizDim;

    pDC->SelectObject((CFont*)m_pfnt); // Cast away constness
	Str8 sText = m_sTxt; // 1.4qtk Upgrade some GetTextExtent for Unicode build
	CString swText = swUTF16( sText ); // 1.4qtk
    sizDim = pDC->GetTextExtent( swText, swText.GetLength() ); // 1.4qtk

    return sizDim;
}


void CHeaderInfo::Draw(CDC* pDC, CRect& rect)
{
    pDC->SelectObject((CFont*)m_pfnt);
	Str8 sText = m_sTxt; // 1.4qta
	CString swText = swUTF16( sText ); // 1.4qta
    pDC->ExtTextOut( 0, 0, ETO_CLIPPED | ETO_OPAQUE, &rect, swText, swText.GetLength(), NULL ); // 1.4qta
    // Assumes pDC->SetTextAlign( TA_UPDATECP ); was called
    // and the drawing position was set.
}

CSize CHeaderInfoList::sizMaxDimensions(CDC* pDC)
{
    CSize sizTotal(0,0);
    CSize sizThisOne;

    for (CHeaderInfo* phdr = phdrFirst(); phdr; phdr = phdrNext(phdr))
        {
        sizThisOne = phdr->sizGetDimensions(pDC);

        sizTotal.cx += sizThisOne.cx;

        if (sizThisOne.cy > sizTotal.cy)
            sizTotal.cy = sizThisOne.cy;
        }

    return sizTotal;
}


void CHeaderInfoList::LoadData(CPrintInfo* pInfo, const CShwView* pView)
{
    char buf[200];
    CPrintProperties* pprtp = (CPrintProperties*)pInfo->m_lpUserData;

    CHeaderInfo* phdr = phdrFirst();

    while (phdr)
        {
        switch (phdr->m_eType)
            {
            case ePageNumber:
                _ltoa_s(pInfo->m_nCurPage, buf, (int)sizeof(buf), 10);
				phdr->SetContents(buf);
                break;

            case eRecordNumber:
                _ltoa_s(pprtp->prel().lNum() + 1, buf, (int)sizeof(buf), 10);
				phdr->SetContents(buf);
                break;

            case eRecordsTotal:
                _ltoa_s(pView->lGetRecordCount(), buf, (int)sizeof(buf), 10);
				phdr->SetContents(buf);
                break;

            case eRecordMarker:
                phdr->SetContents( ((CIndex*) pView->pind())->pindsetMyOwner()->pmkrRecord()->sName());
                // Casting away constness of pind() for sake of getting its owner.
                break;

            case eRecordField:  // Assumes record field is always first.
                phdr->SetContents(pprtp->prel()->prec()->pfldFirst()->sContents());
                break;

            case eKeyMarker:
                phdr->SetContents(pprtp->prel()->pfldPriKey()->pmkr()->sName());
                break;

            case eKeyField:
                phdr->SetContents(pprtp->prel()->pfldPriKey()->sContents());
                break;
            }

        phdr = phdrNext(phdr);
        }
}


void CHeaderInfoList::Add( HeaderDatatype eType, const CFont* pfnt, const char* pszTxt )
{
    CDblList::Add((CDblListEl*)new CHeaderInfo(eType, pfnt, pszTxt) );
}


BOOL CHeaderInfoList::bParse(CPrintProperties* pptp, const CShwView* pView, const char* pszTxt, BOOL bParseOnly)
{
    ASSERT(pszTxt != NULL);
    const char* psz = pszTxt;
    CTime timNow;
    Str8 sElement;
    Str8 sTmp;
    const CFont *pfnt;
    const CFont *pfntDefault = Shw_pProject()->pfntMkrs();

    if (!bParseOnly)
        DeleteAll();    // Delete the results of the prior parse.

    while (*psz)
        {
        if (*psz == '&')
            {
            if (!sElement.IsEmpty())
                Add(eText, pfntDefault, sElement);
            sElement.Empty();
            
            psz++;
            if (*psz == '\0')
                return FALSE;

            switch (*psz)
                {
                case '&':   // Amersand itself.
                    sElement += *psz;
                    break;

                case 'F':
                case 'f':   // File name.
                    if (!bParseOnly)
                        {
                        sTmp =  sUTF8( pView->GetDocument()->GetPathName() ); // 1.4qra
                        sTmp = sGetFileName(sTmp);
                        Add(eFileName, pfntDefault, sTmp);
                        }
                    break;

                case 'p':
                case 'P':   // Page number.
                    Add(ePageNumber, pfntDefault, "0");
                    break;

                case 'n':   // Record number
                    Add(eRecordNumber, pfntDefault, "0");
                    break;

                case 'N':   // Total records in index.
                    Add(eRecordsTotal, pfntDefault, "0");
                    break;

                case 'R':   // Record marker.
                    Add(eRecordMarker, pfntDefault, "");
                    break;

                case 'r':   // Record field contents.
                    pfnt = (bParseOnly) ? pfntDefault : (CFont*)pptp->rps().pfld->pmkr()->pfnt();
                    Add(eRecordField, pfnt, "");
                    break;

                case 'K':   // Key marker.
                    Add(eKeyMarker, pfntDefault, "");
                    break;

                case 'k':   // Key field contents.
                    pfnt = (bParseOnly) ? pfntDefault : pptp->prel()->pfldPriKey()->pmkr()->pfnt();
                    Add(eKeyField, pfnt, "");
                    break;

                case 'd':   // Date
                    timNow = CTime::GetCurrentTime();
					LocaleFormatDate(timNow, sTmp, FALSE);
					Add(eDate, pfntDefault, sTmp);
					break;

                case 'D':
                    timNow = CTime::GetCurrentTime();
                    LocaleFormatDate(timNow, sTmp, TRUE);
                    Add(eDate, pfntDefault, sTmp);
                    break;

                case 't':   // Time
                case 'T':
                    timNow = CTime::GetCurrentTime();
                    LocaleFormatTime(timNow, sTmp);
                    Add(eTime, pfntDefault, sTmp);
                    break;

                case 'L':   // Filter name.
                case 'l':
                    if (!bParseOnly)
                        {
                        if ( (pView->pind()->pfil() ) )
                            sTmp = pView->pind()->pfil()->sName();
                        else
                            sTmp.Empty();
                        Add(eFilterName, pfntDefault, sTmp);
                        }
                    break;

                default:
                    return FALSE;   // Illegal format.

                }

            }
        else
            sElement += *psz;

        psz++;
        };

    if (!sElement.IsEmpty() && (!bParseOnly))
        Add(eText, pfntDefault, sElement);

    return TRUE;
}


void CHeaderInfoList::Draw(CDC* pDC, CPrintInfo* pInfo, const CShwView* pView, CRect &rectBounding, BOOL bBottomAlign)
{

    // Load up with data for this page.
    LoadData(pInfo, pView);
    pDC->SetTextAlign( TA_UPDATECP );

    // Determine the initial position (assume we want to horizontally center it
    // in the bounding rect.
    CPoint ptStart;
    CSize  sizHeader = sizMaxDimensions(pDC);
	if (bBottomAlign)
		ptStart.y = rectBounding.bottom + sizHeader.cy;
	else
		ptStart.y = rectBounding.top;
    ptStart.x = max(0, rectBounding.Width()/2 - sizHeader.cx/2 );
    // If the text is bigger than the rect, it will be chopped off at
    // the far end.

    pDC->SetTextAlign( TA_UPDATECP );
    pDC->MoveTo( ptStart );

    for (CHeaderInfo* phdr = phdrFirst(); phdr; phdr = phdrNext(phdr))
        {
        CSize sizThisOne = phdr->sizGetDimensions(pDC);
        CRect rectItem(ptStart.x, rectBounding.top, 
            ptStart.x + sizThisOne.cx + m_iSpaceBetweenHeaderItems, 
            rectBounding.bottom);

        pDC->MoveTo( ptStart );
        phdr->Draw(pDC, rectItem);
        ptStart.x += sizThisOne.cx + m_iSpaceBetweenHeaderItems;
        }
}

int CHeaderInfoList::iLineHt(CDC* pDC)
{
	return sizMaxDimensions(pDC).cy;
}

const int CPrintProperties::s_MargLeft_Default = 25;
const int CPrintProperties::s_MargRight_Default = 25;
const int CPrintProperties::s_MargTop_Default = 100;
const int CPrintProperties::s_MargBottom_Default = 100;

const int CPrintProperties::s_SpaceBetweenRecords_Default = 10;
const int CHeaderInfoList::s_SpaceBetweenHeaderItemsDefault = 2;


CPrintProperties::CPrintProperties() :
    m_bBreakBetweenRecords(TRUE),
    m_bPrintAllRecords(TRUE),
    m_iSpaceBetweenRecs(CPrintProperties::s_SpaceBetweenRecords_Default)
{
    SetMargins(CPrintProperties::s_MargLeft_Default,
                CPrintProperties::s_MargTop_Default,
                CPrintProperties::s_MargRight_Default,
                CPrintProperties::s_MargBottom_Default);
	m_sFooter = _("Page");
	m_sFooter = m_sFooter + " " + "&p";	//10-5-98 for French version
	m_sHeader = _("File:"); // 1.5.0ft 
	m_sHeader = m_sHeader + " " + "&f" + _("Date:") + " " + "&d"; // 1.5.0ft 
}


CRect CPrintProperties::GetMargins()
{
    CRect rectMargin;

    // For now, assume measurement in inches.
    // Look for terminating unit of measure information.  Must be in or cm


    rectMargin.SetRect(iLoEnglish(m_sMargLeft),
                    iLoEnglish(m_sMargTop),
                    iLoEnglish(m_sMargRight),
                    iLoEnglish(m_sMargBottom));
                    
    return rectMargin;
}

    
void CPrintProperties::SetMargins(CRect& rectMargins)
{
    char buf[32];
	double dConvert;

	// rectMargins is assumed to be in LoEnglish units.  Store its values
	// in inches or cm, depending on locale.  Use dConvert to convert from
	// LoEnglish to the appropriate unit.
	if (bIsLocaleEnglish())
		dConvert = 1.0/100.0;
	else
		dConvert = 2.54/100.0;

    sprintf_s(buf, sizeof(buf), "%.2f", ((double)rectMargins.top*dConvert) );
    m_sMargTop  = buf;
    
    sprintf_s(buf, sizeof(buf), "%.2f", ((double)rectMargins.left*dConvert) );
    m_sMargLeft = buf;
    
    sprintf_s(buf, sizeof(buf), "%.2f", ((double)rectMargins.bottom*dConvert) );
    m_sMargBottom = buf;
    
    sprintf_s(buf, sizeof(buf), "%.2f", ((double)rectMargins.right*dConvert) );
    m_sMargRight = buf;

    bValidateMeasure(m_sMargTop);
    bValidateMeasure(m_sMargLeft);
    bValidateMeasure(m_sMargBottom);
    bValidateMeasure(m_sMargRight);
    
}


BOOL CPrintProperties::bValidateMeasure(Str8 &sVal)
{
    const char* psz;
    Str8 sInches("in");
    Str8 sCm("cm");
    //CPatElString patIn(sInches);
    //CPatElString patCm(sCm);
    Str8 sRebuild;

    // Check the first part of the string.
    psz = sVal;

    // check for white space (which will be eliminated)
    Shw_MatchOptionalWhiteSpaceAt(&psz);

    // check for numbers and period.
    for (; *psz; psz++)
        {
        if ( Shw_bAtWhiteSpace(psz)
                || !( ((*psz >= '0') && (*psz <= '9')) || (*psz == '.')) )
            break;
        sRebuild += *psz;
        }

    if (sRebuild.IsEmpty())
        sRebuild = "0";

    sRebuild += ' ';

    // What remains should be either inches "in" or centimeters "cm"
    Shw_MatchOptionalWhiteSpaceAt(&psz);
    if (_strnicmp(psz, sCm, sCm.GetLength()) == 0)
        {
        sRebuild += sCm;
        }
    else
        if (_strnicmp(psz, sInches, sInches.GetLength())  == 0)
            {
            sRebuild += sInches;
            }
        else
            // Are there any characters remaining in the string.  If so it's invalid.
            if ((*psz != '\0') && !Shw_bMatchOptionalWhiteSpaceAt(&psz))
                return FALSE;
            else if (bIsLocaleEnglish())   // OK, but append default (based on locale)
                sRebuild += sInches;
            else
                sRebuild += sCm;

    sVal = sRebuild;

    return TRUE;
    
}


BOOL CPrintProperties::bMeasureInches(const char* psz)
{
    Str8 sInches("in");
    // must have an "in" in it.

    BOOL bResult =  (strstr(psz, sInches) != NULL);

    // If not inches, is it centimeters?
    if (!bResult)
        {
        bResult = !bMeasureCm(psz);
        // If not centimeters, default back to inches.
        }
    return bResult;
}


BOOL CPrintProperties::bMeasureCm(const char* psz)
{
    Str8 sCm("cm");
    // must have a "cm" in it.
    return (strstr(psz, sCm) != NULL);
}


int CPrintProperties::iLoEnglish(const char* psz)
{
    int iVal;
    double dblVal = atof(psz);

    ASSERT( !(bMeasureCm(psz) && bMeasureInches(psz)) );
    ASSERT( bMeasureCm(psz) || bMeasureInches(psz) );

    if (bMeasureCm(psz))
        dblVal /= 2.54; // 2.54cm/in

    iVal = (int)(100.0*dblVal); // Dealing in 100ths of inches.

    return iVal;
}




void CPrintProperties::ClearRecPos()
{
    m_rps.pfld = NULL;
    m_rps.prec = NULL;
    m_ptplst.DeleteAll();
}


void CPrintProperties::SaveCurrentPositionAtEnd()
{
    m_ptplst.Add(m_prel, m_rps);
}


CRect CPrintProperties::GetTruePageMargin(CDC* pDC)
{
    CRect rectMargins;
    POINT pt;

    // Start by getting the dimensions of the unprintable part of the
    // page (in device units). GETPRINTINGOFFSET will tell us the left
    // and upper unprintable area.

    pDC->Escape(GETPRINTINGOFFSET, 0, NULL, &pt);
    rectMargins.left = pt.x;
    rectMargins.top  = pt.y;
 
    // To get the right and lower unprintable area, we need to take
    // the entire width and height of the paper (GETPHYSPAGESIZE) and
    // subtract everything else.
 
   pDC->Escape(GETPHYSPAGESIZE, 0, NULL, &pt);
    
   rectMargins.right  = pt.x                     // total paper width
                          - pDC->GetDeviceCaps(HORZRES) // printable width
                          - rectMargins.left;   // left unprtable margin
 
   rectMargins.bottom = pt.y                     // total paper height
                          - pDC->GetDeviceCaps(VERTRES) // printable ht
                          - rectMargins.top;    // rt unprtable margin
 
   // At this point, rectMargins contains the widths of the
   // unprintable regions on all four sides of the page in device units.
   // Convert the device units to Lo-English units, since that's what
   // everybody else uses. (100 Lo-English units per inch)
 
   pt.x = pDC->GetDeviceCaps(LOGPIXELSX);    // dpi in X direction
   pt.y = pDC->GetDeviceCaps(LOGPIXELSY);    // dpi in Y direction

   rectMargins.left   = MulDiv(rectMargins.left, 100, pt.x);
   rectMargins.top    = MulDiv(rectMargins.top, 100, pt.y);
   rectMargins.right  = MulDiv(rectMargins.right, 100, pt.x);
   rectMargins.bottom = MulDiv(rectMargins.bottom, 100, pt.y);
 
   // Get the Lo-English margin values from the Page Setup dialog and
   // subtract the unprintable part we just calculated.  Save the
   // results back in rectTrueMargins.

   CRect rectTrueMargins;
   CRect rectUserMargins = GetMargins();

   rectTrueMargins.left = max(0, rectUserMargins.left - rectMargins.left);
   rectTrueMargins.top = max(0, rectUserMargins.top - rectMargins.top);
   rectTrueMargins.right = max(0, rectUserMargins.right - rectMargins.right);
   rectTrueMargins.bottom = max(0, rectUserMargins.bottom - rectMargins.bottom);

   return rectTrueMargins;
}



static const char* psz_PrintProperites = "PrintProperties";
static const char* psz_header = "header";
static const char* psz_footer = "footer";
static const char* psz_margtop = "topmargin";
static const char* psz_margleft = "leftmargin";
static const char* psz_margbottom = "bottommargin";
static const char* psz_margright = "rightmargin";
static const char* psz_breakrecordswithspace = "breakrecordswithspace";
static const char* psz_printcurrentrecord = "printcurrentrecord";
static const char* psz_recordsspace = "recordsspace";

#ifdef typWritefstream // 1.6.4ac
void CPrintProperties::WriteProperties(Object_ofstream& obs) const
#else
void CPrintProperties::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_PrintProperites, "");
    obs.WriteString(psz_header, m_sHeader);
    obs.WriteString(psz_footer, m_sFooter);
    obs.WriteString(psz_margtop, m_sMargTop);
    obs.WriteString(psz_margleft, m_sMargLeft);
    obs.WriteString(psz_margbottom, m_sMargBottom);
    obs.WriteString(psz_margright, m_sMargRight);
    obs.WriteBool(psz_breakrecordswithspace, !m_bBreakBetweenRecords);
    obs.WriteBool(psz_printcurrentrecord, !m_bPrintAllRecords);
    obs.WriteInteger(psz_recordsspace, m_iSpaceBetweenRecs);
    obs.WriteEndMarker(psz_PrintProperites);
}


BOOL CPrintProperties::bReadProperties(Object_istream& obs)
{
    BOOL bTmp;

    if ( !obs.bReadBeginMarker(psz_PrintProperites) )
        return FALSE;

    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadBool(psz_breakrecordswithspace, bTmp) )
            m_bBreakBetweenRecords = !bTmp;
        else if ( obs.bReadBool(psz_printcurrentrecord, bTmp) )
            m_bPrintAllRecords = !bTmp;
        else if ( obs.bReadInteger(psz_recordsspace, m_iSpaceBetweenRecs) )
            ;
        else if ( obs.bReadString(psz_header, m_sHeader) )
            ;
        else if ( obs.bReadString(psz_footer, m_sFooter) )
            ;
        else if ( obs.bReadString(psz_margtop, m_sMargTop) )
            ;
        else if ( obs.bReadString(psz_margleft, m_sMargLeft) )
            ;
        else if ( obs.bReadString(psz_margbottom, m_sMargBottom) )
            ;
        else if ( obs.bReadString(psz_margright, m_sMargRight) )
            ;
        else if ( obs.bEnd(psz_PrintProperites) )
            break;
        }
        
    return TRUE;
}
