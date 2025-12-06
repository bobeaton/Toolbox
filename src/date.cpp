// date.cpp - Shoebox marker-date filter condition and datestamp

#include "stdafx.h"
#include "toolbox.h"
#include "date.h"
#include "obstream.h"  // Object_ostream, Object_istream
#include "ddx.h"  // g_TrimSurroundingWhiteSpace
#include "resource.h"
#include <time.h>  // time, localtime
#include <ctype.h>  // isdigit
#include <string.h>  // strncmp, strnicmp, strncpy, strcpy, strlen

#include "shwnotes.h"  // IDN_dat1
#include "not.h"  // CNote

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

static const char* s_pszBefore = "<";
static const char* s_pszEqual  = "=";
static const char* s_pszAfter  = ">";

void TrimSurroundingWhiteSpace(const char** ppsz, size_t* plen) // 1.4hdr Change to local in date file
{
    ASSERT( ppsz );
    ASSERT( plen );
    const char* psz = *ppsz;
    ASSERT( psz );
    int len = strlen(psz);
    int lenPrecedingWS = 0;
    int lenFollowingWS = 0;
    if ( Shw_bNeedsToBeTrimmed(psz, &lenPrecedingWS, &lenFollowingWS) )
        {
        psz += lenPrecedingWS;
        len -= (lenPrecedingWS + lenFollowingWS);
        }

    *ppsz = psz;
    *plen = len;
}


CDateCon::CDateCon(int iRelation, const Str8& sSurface,
        const char* pszUnderlying)
{
    m_iRelation = iRelation;
    if ( m_iRelation == eBefore )
        m_pszRelation = s_pszBefore;
    else if ( m_iRelation == eEqual )  // 1998-06-30 MRP: ==
        m_pszRelation = s_pszEqual;
    else if ( m_iRelation == eAfter )  // 1998-06-30 MRP: ==
        m_pszRelation = s_pszAfter;
    else
        ASSERT( FALSE );

    m_sSurface = sSurface;

    ASSERT( pszUnderlying );
    m_sUnderlying = pszUnderlying;
    m_bValidUnderlying = ( m_sUnderlying.GetLength() != 0 );
}

CDateCon::CDateCon(const CDateCon& dat)
{
    m_iRelation = dat.m_iRelation;
    m_pszRelation = dat.m_pszRelation;
    m_sSurface = dat.m_sSurface;
    m_sUnderlying = dat.m_sUnderlying;
    m_bValidUnderlying = dat.m_bValidUnderlying;
}


static const char* psz_rel = "rel";
static const char* psz_dat = "dat";
    
#ifdef prjWritefstream // 1.6.4aa 
void CDateCon::WriteProperties(Object_ofstream& obs) const
#else
void CDateCon::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteString(psz_rel, m_pszRelation);
    obs.WriteString(psz_dat, m_sSurface);
}

BOOL CDateCon::s_bReadProperties(Object_istream& obs, CDateCon** ppdat)
{
    if ( obs.bAtEnd() )
        return FALSE;

    Str8 sRelation;
    if ( !obs.bReadString(psz_rel, sRelation) )
        return FALSE;

    Str8 sSurface;
    if ( !obs.bAtEnd() )
        obs.bReadString(psz_dat, sSurface);

    int iRelation = eBefore;  // Default in case property isn't valid
    if ( strcmp(sRelation, s_pszEqual) == 0 )
        iRelation = eEqual;
    else if ( strcmp(sRelation, s_pszAfter) == 0 )
        iRelation = eAfter;

    size_t len = sSurface.GetLength();
    char pszUnderlying[maxlenUnderlying + 1];
    BOOL bMinorVariation = FALSE;
    if ( !s_bSurfaceToUnderlying(sSurface, len, TRUE, FALSE,
            pszUnderlying, &bMinorVariation) )
        *pszUnderlying = '\0';
    else if ( bMinorVariation )
        // Automatically upgrade marker-date filter element's format.
        s_UnderlyingToSurface(pszUnderlying, sSurface);

    ASSERT( ppdat );
    ASSERT( !*ppdat );  // If it isn't NULL now, there will be a leak.
    *ppdat = new CDateCon(iRelation, sSurface, pszUnderlying);
    return TRUE;
}


BOOL CDateCon::bMatches(const Str8& sDateField) const
{
    if ( !m_bValidUnderlying )
        return FALSE;

    const char* psz = sDateField;
    size_t len = 0;
    TrimSurroundingWhiteSpace(&psz, &len);

    char pszUnderlying[maxlenUnderlying + 1];
    BOOL bMinorVariation = FALSE;
    if ( !s_bSurfaceToUnderlying(psz, len, FALSE, FALSE,
            pszUnderlying, &bMinorVariation) )
        return FALSE;

    // The date field is assumed to be full but the date filter condition
    // can be either full (i.e. yyyymmdd) or partial (i.e. yyyymm or yyyy).
    // The relationship could be indeterminate if the date field had
    // less precision than the filter element, e.g. is Apr/1998
    // Before or Equal to or After 09/Apr/1998? On the other hand,
    // it's reasonable to say 09/Apr/1998 is Equal to Apr/1998.
    int i = strncmp(pszUnderlying, m_sUnderlying, m_sUnderlying.GetLength());

    BOOL bMatched = FALSE;
    if ( m_iRelation == eBefore )
        bMatched = ( i < 0 );
    else if ( m_iRelation == eEqual )
        bMatched = ( i == 0 );
    else if ( m_iRelation == eAfter )
        bMatched = ( i > 0 );
    else
        ASSERT( FALSE );
    
    return bMatched;
}


BOOL CDateCon::s_bConstruct(int iRelation, const Str8& sSurface,
        CDateCon** ppdat, CNote** ppnot)
{
    const char* psz = sSurface;
    size_t len = 0;
    TrimSurroundingWhiteSpace(&psz, &len);

    char pszUnderlying[maxlenUnderlying + 1];
    BOOL bMinorVariation = FALSE;
    if ( !s_bSurfaceToUnderlying(psz, len, TRUE, FALSE,
            pszUnderlying, &bMinorVariation) )
        {
        *ppnot = new CNote(_("Unrecognized date:"), sSurface, // 1.5.0ft 
            sSurface, sSurface.GetLength(), sSurface);
        return FALSE;
        }

    Str8 s(psz,len);
    if ( bMinorVariation )
        // Automatically upgrade marker-date filter element's format.
        s_UnderlyingToSurface(pszUnderlying, s);

    ASSERT( ppdat );
    ASSERT( !*ppdat );  // If it isn't NULL now, there will be a leak.
    *ppdat = new CDateCon(iRelation, s, pszUnderlying);
    return TRUE;
}

BOOL CDateCon::bModifyProperties(int iRelation, const Str8& sSurface,
        CNote** ppnot)
{
    const char* psz = sSurface;
    size_t len = 0;
    TrimSurroundingWhiteSpace(&psz, &len);

    char pszUnderlying[maxlenUnderlying + 1];
    BOOL bMinorVariation = FALSE;
    if ( !s_bSurfaceToUnderlying(psz, len, TRUE, FALSE,
            pszUnderlying, &bMinorVariation) )
        {
        *ppnot = new CNote(_("Unrecognized date:"), sSurface, // 1.5.0ft 
            sSurface, sSurface.GetLength(), sSurface);
        return FALSE;
        }

    m_iRelation = iRelation;
    if ( m_iRelation == eBefore )
        m_pszRelation = s_pszBefore;
    else if ( m_iRelation == eEqual )  // 1998-06-30 MRP: ==
        m_pszRelation = s_pszEqual;
    else if ( m_iRelation == eAfter )  // 1998-06-30 MRP: ==
        m_pszRelation = s_pszAfter;
    else
        ASSERT( FALSE );

    if ( bMinorVariation )
        // Automatically upgrade marker-date filter element's format.
        s_UnderlyingToSurface(pszUnderlying, m_sSurface);
    else
        m_sSurface = Str8(psz,len);

    m_sUnderlying = pszUnderlying;
    m_bValidUnderlying = TRUE;
    
    return TRUE;
}


// --------------------------------------------------------------------------

static const char* s_pszMonth[] =
    {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
    };

static void s_TimeToSurface(time_t lTime, Str8& sSurface)
{
    struct tm tmLocal;
    errno_t err = localtime_s(&tmLocal, &lTime);  // thread-safe local time conversion
    // The error return occurs in Windows if the system date
    // is 19/Jan/2038 and later.
    static BOOL s_bMessageShown = FALSE;
    if (err != 0)
        {
        // 1999-03-15 MRP: Diagnostic message for unexpected NULL value.
        // 1999-03-30 MRP: Show the message only once per session.
        if ( !s_bMessageShown )
            {
// 2000-01-10 TLB: No longer want to assert for diagnostic reasons. Also changed to use a user-oriented message.
//            ASSERT( tm );
//            Str8 sMessage;
            AfxMessageBox(_("Please check your system date. Toolbox cannot handle a year higher than 2038."), MB_ICONINFORMATION | MB_OK);
            s_bMessageShown = TRUE;
            }

        return;  // Leave surface form (i.e., Date Stamp) empty
        }

    int iDay = tmLocal.tm_mday;
    ASSERT( 1 <= iDay && iDay <= 31 );  // 1998-12-02 MRP
    int iMonth = tmLocal.tm_mon;
    ASSERT( 0 <= iMonth && iMonth <= 11 );  // 1998-12-02 MRP
    const char* pszMonth = s_pszMonth[iMonth];
    int iYear = 1900 + tmLocal.tm_year;
    // Note: Maximum signed long time value represents 18/Jan/2038
    ASSERT( 1900 <= iYear && iYear <= 2038 );
    // Format: dd/Mmm/yyyy, e.g. 09/Apr/1998
    sSurface.Format("%02d/%s/%d", iDay, pszMonth, iYear);
}

void CDateCon::s_DateStamp(Str8& sDateStamp)
{
    time_t lTime;
    time(&lTime);  // Get current time as long integer
    s_TimeToSurface(lTime, sDateStamp);
}

BOOL CDateCon::s_bConvertDateField(const Str8& sDateField,
        Str8& sConvertedDateField, BOOL* pbNotDate)
{
    ASSERT( pbNotDate );

    const char* psz = sDateField;
    size_t len = 0;
    TrimSurroundingWhiteSpace(&psz, &len);

    char pszUnderlying[maxlenUnderlying + 1];
    BOOL bMinorVariation = FALSE;
    if ( !s_bSurfaceToUnderlying(psz, len, FALSE, TRUE,
            pszUnderlying, &bMinorVariation) )
        {
        *pbNotDate = TRUE;
        return FALSE;
        }

    if ( !bMinorVariation )    
        {
        *pbNotDate = FALSE;  // It's a date
        return FALSE;  // That doesn't need to be upgraded
        }

    s_UnderlyingToSurface(pszUnderlying, sConvertedDateField);

    // Preserve any white space following the datestamp.
    int lenFollowingWS = strlen(psz + len);  // Note: Right expects an int
    if ( lenFollowingWS != 0 )
        sConvertedDateField += sDateField.Right(lenFollowingWS);

    return TRUE;
}


static BOOL s_bSurfaceDayToUnderlying(const char** ppsz, size_t* plen,
        char* pszUnderlying, BOOL* pbMinorVariation)
{
    ASSERT( ppsz );
    ASSERT( plen );
    ASSERT( pszUnderlying );
    ASSERT( pbMinorVariation );
    const char* psz = *ppsz;
    ASSERT( psz );
    
    if ( 3 <= *plen && isdigit(psz[0]) && isdigit(psz[1]) && psz[2] == '/' )
        {
        *ppsz += 3;
        *plen -= 3;
        pszUnderlying[6] = psz[0];
        pszUnderlying[7] = psz[1];
        return TRUE;
        }
    else if ( 2 <= *plen && isdigit(psz[0]) && psz[1] == '/' )
        {
        *ppsz += 2;
        *plen -= 2;
        pszUnderlying[6] = '0';
        pszUnderlying[7] = psz[0];
        *pbMinorVariation = TRUE;
        return TRUE;
        }
    
    return FALSE;
}  // s_bSurfaceDayToUnderlying

static BOOL s_bSurfaceMonthToUnderlying(const char** ppsz, size_t* plen,
        char* pszUnderlying, BOOL* pbMinorVariation)
{
    ASSERT( ppsz );
    ASSERT( plen );
    ASSERT( pszUnderlying );
    ASSERT( pbMinorVariation );
    const char* psz = *ppsz;
    ASSERT( psz );
    
    if ( 4 <= *plen && psz[3] == '/' )
        {
        int iMonth = 0;
        for ( ; iMonth != 12; iMonth++ )
            if ( _strnicmp(psz, s_pszMonth[iMonth], 3) == 0 )
                break;
    
        if ( iMonth == 12 )
            return FALSE;

        if ( strncmp(psz, s_pszMonth[iMonth], 3) != 0 )
            *pbMinorVariation = TRUE;
    
        iMonth++;
        ASSERT( 1 <= iMonth && iMonth <= 12 );
        if ( iMonth < 10 )
            {
            pszUnderlying[4] = '0';
            pszUnderlying[5] = '0' + iMonth;
            }
        else
            {
            pszUnderlying[4] = '1';
            pszUnderlying[5] = '0' + (iMonth - 10);
            }

        *ppsz += 4;
        *plen -= 4;    
        return TRUE;
        }
    
    return FALSE;
}  // s_bSurfaceMonthToUnderlying

static BOOL s_bSurfaceYearToUnderlying(const char** ppsz, size_t* plen,
        BOOL bCanBeMangled, BOOL bCanBeTwoDigits,
        char* pszUnderlying, BOOL* pbMinorVariation)
{
    ASSERT( ppsz );
    ASSERT( plen );
    ASSERT( pszUnderlying );
    ASSERT( pbMinorVariation );
    const char* psz = *ppsz;
    ASSERT( psz );
 
    if ( *plen == 4 && isdigit(psz[0]) && isdigit(psz[1]) &&
            isdigit(psz[2]) && isdigit(psz[3]) )
        {
        *ppsz += 4;
        *plen += 4;
        pszUnderlying[0] = psz[0];
        pszUnderlying[1] = psz[1];
        pszUnderlying[2] = psz[2];
        pszUnderlying[3] = psz[3];
        return TRUE;
        }
    else if ( bCanBeMangled && 3 <= *plen && psz[0] == '1' &&
            isdigit(psz[1]) && isdigit(psz[2]) )
        {
        // At the year 2000, Shoebox 3 starts mangling datestamp fields
        // because it has a call to sprintf that overruns its buffer.
        *ppsz += 3;
        *plen += 3;
        pszUnderlying[0] = '2';
        pszUnderlying[1] = '0';
        pszUnderlying[2] = psz[1];
        pszUnderlying[3] = psz[2];
        *pbMinorVariation = TRUE;
        return TRUE;
        }
    else if ( bCanBeTwoDigits && *plen == 2 &&
            isdigit(psz[0]) && isdigit(psz[1]) )
        {
        *ppsz += 2;
        *plen += 2;
        if ( psz[0] <= '3' )  // Consider 00-39 to be truncated from 20yy
            {
            // At the year 2000, Shoebox 2 datestamps fields look like this.
            pszUnderlying[0] = '2';
            pszUnderlying[1] = '0';
            }
        else
            {
            pszUnderlying[0] = '1';
            pszUnderlying[1] = '9';
            }
        pszUnderlying[2] = psz[0];
        pszUnderlying[3] = psz[1];
        *pbMinorVariation = TRUE;
        return TRUE;
        }
 
    return FALSE;
}  // s_bSurfaceYearToUnderlying

BOOL CDateCon::s_bSurfaceToUnderlying(const char* pszSurface,
        size_t lenSurface, BOOL bCanBePartial, BOOL bCanBeMangled,
        char* pszUnderlying, BOOL* pbMinorVariation)
{
    ASSERT( pszSurface );
    // Kind of date             bCanBePartial   bCanBeMangled
    // Filter date condition    TRUE            FALSE
    // Matching a date field    FALSE           FALSE
    // Upgrading a date field   FALSE           TRUE
    ASSERT( !bCanBePartial || !bCanBeMangled );  // Can't both be TRUE
    ASSERT( pszUnderlying );
    ASSERT( pbMinorVariation );

    const char* psz = pszSurface;
    size_t len = lenSurface;
    char pszU[maxlenUnderlying + 1] = "\0\0\0\0\0\0\0\0";
    BOOL bMinorVariation = FALSE;

    BOOL bDay = s_bSurfaceDayToUnderlying(&psz, &len,
            pszU, &bMinorVariation);
    if ( !bDay && !bCanBePartial )
        return FALSE;

    BOOL bMonth = s_bSurfaceMonthToUnderlying(&psz, &len,
            pszU, &bMinorVariation);
    // If there's a day, then the month is obligatory.
    if ( !bMonth && (bDay || !bCanBePartial) )
        return FALSE;

    // If the date has a month, it can have a two-digit year;
    // if a partial date is a year alone, it must have four digits.
    if ( !s_bSurfaceYearToUnderlying(&psz, &len, bCanBeMangled, bMonth,
            pszU, &bMinorVariation) )
        return FALSE;

    size_t lenU = strlen(pszU);
    ASSERT( 4 <= lenU && lenU <= 8 );
    strcpy_s(pszUnderlying, maxlenUnderlying + 1, pszU);
    *pbMinorVariation = bMinorVariation;
    return TRUE;
}  // CDateCon::s_bSurfaceToUnderlying

void CDateCon::s_UnderlyingToSurface(const char* pszUnderlying,
        Str8& sSurface)
{
    ASSERT( pszUnderlying );
    size_t lenUnderlying = strlen(pszUnderlying);
    ASSERT( 4 <= lenUnderlying && lenUnderlying <= 8 );

    const size_t maxlenSurface = 11;  // dd/Mmm/yyyy or Mmm/yyyy or yyyy
    char pszSurface[maxlenSurface + 1];
    char* psz = pszSurface;

    if ( lenUnderlying == 8 )
        {
        ASSERT( isdigit(pszUnderlying[6]) && isdigit(pszUnderlying[7]) );
        *psz++ = pszUnderlying[6];
        *psz++ = pszUnderlying[7];
        *psz++ = '/';
        }

    if ( 6 <= lenUnderlying )
        {
        ASSERT( isdigit(pszUnderlying[5]) );
        int iMonth = pszUnderlying[5] - '0';
        if ( pszUnderlying[4] == '1' )
            iMonth += 10;
        else
            ASSERT( pszUnderlying[4] == '0' );
        ASSERT( 1 <= iMonth && iMonth <= 12 );
        memcpy(psz, s_pszMonth[iMonth - 1], 3);
        psz += 3;
        *psz++ = '/';
        }

    ASSERT( isdigit(pszUnderlying[0]) && isdigit(pszUnderlying[1]) &&
        isdigit(pszUnderlying[2]) && isdigit(pszUnderlying[3]) );
    *psz++ = pszUnderlying[0];
    *psz++ = pszUnderlying[1];
    *psz++ = pszUnderlying[2];
    *psz++ = pszUnderlying[3];

    *psz = '\0';
    ASSERT( strlen(pszSurface) <= maxlenSurface );
    sSurface = pszSurface;
}  // CDateCon::s_UnderlyingToSurface

// **************************************************************************
