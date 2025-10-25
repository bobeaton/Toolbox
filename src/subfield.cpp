// subfield.cpp  Sub-fields: Character styles in Standard Format

#include "stdafx.h"
#include "toolbox.h"
#ifdef SUBFIELD
#include "subfield.h"


static const char s_chLeftDelimiter = '\x01';
static const char s_chRightDelimiter = '\x02';
static const char s_pszViewSubStringDelimiters[] = {
    '\n',
    s_chLeftDelimiter,
    '\0'
    };
static const char s_pszWritingSubStringDelimiters[] = {
    s_chLeftDelimiter,
    '\\',
    '{',
    '\0'
    };


CSubFieldRecPos::CSubFieldRecPos(int iCharPar, CField* pfldPar, CRecord* precPar) :
    CRecPos(iCharPar, pfldPar, precPar)
{
    m_pmkrSubField = NULL;
    SetMarker();
}

void CSubFieldRecPos::SetMarker()
{
    ASSERT( 0 <= iChar );
    int i = 0;
    Length lenTag = 0;
    CMarker* pmkrSubField = NULL;
    BOOL bEndMarker = FALSE;
    while ( i != iChar )
        if ( bAtSubFieldTag(&lenTag, &pmkrSubField, &bEndMarker) )
            {
            ASSERT( i < iChar );
            i += lenTag;
            SetSubFieldMarker(pmkrSubField, bEndMarker, TRUE);
            }
        else
            i++;
}

CMarker* CSubFieldRecPos::pmkr() const
{
    return m_pmkrSubField ? m_pmkrSubField : pfld->pmkr();
}

void CSubFieldRecPos::SetSubFieldMarker(CMarker* pmkrSubField, BOOL bEndMarker,
        BOOL bMovingRight)
{
    if ( bEndMarker && bMovingRight || !bEndMarker && !bMovingRight )
        {
        // Pop the subfield marker
        ASSERT( m_pmkrSubField );
        ASSERT( m_pmkrSubField == pmkrSubField );
        m_pmkrSubField = NULL;
        }
    else
        {
        // Push the subfield marker
        ASSERT( !m_pmkrSubField );
        m_pmkrSubField = pmkrSubField;
        ASSERT( m_pmkrSubField );
        }
}


BOOL CSubFieldRecPos::bMoveRightToNextSubFieldTag(Length* plenTag,
        CMarker** ppmkrSubField, BOOL* pbEndMarker)
{
    const char* pszPos = strchr(psz(), s_chLeftDelimiter);
    if ( !pszPos )
        return FALSE;

    SetPos(pszPos);
    BOOL b = bAtSubFieldTag(plenTag, ppmkrSubField, pbEndMarker);
    ASSERT( b );
    return TRUE;
}

BOOL CSubFieldRecPos::bAtSubFieldTag() const
{
    return *psz() == s_chLeftDelimiter;
}

BOOL CSubFieldRecPos::bAtSubFieldTag(Length* plenTag,
        CMarker** ppmkrSubField, BOOL* pbEndMarker) const
{
    const char* pszLeftDelimiter = psz();
    if ( *pszLeftDelimiter != s_chLeftDelimiter )
        return FALSE;

    const char* pszMarker = pszLeftDelimiter + 1;
    BOOL bEndMarker = (*pszMarker == '-');
    if ( bEndMarker ) 
        pszMarker++;
    const char* pszRightDelimiter = strchr(pszMarker, s_chRightDelimiter);
    ASSERT( pszRightDelimiter );

    Length lenMarker = pszRightDelimiter - pszMarker;
    ASSERT( pfld );
    CMarker* pmkr = pfld->pmkr()->pmkrsetMyOwner()->
        pmkrSearchForWholeSubString(pszMarker, lenMarker);
    ASSERT( pmkr );
    ASSERT( pmkr->bSubfield() );

    ASSERT( plenTag );
    *plenTag = pszRightDelimiter + 1 - pszLeftDelimiter;
    ASSERT( ppmkrSubField );
    *ppmkrSubField = pmkr;
    ASSERT( pbEndMarker );
    *pbEndMarker = bEndMarker;
    return TRUE;
}

BOOL CSubFieldRecPos::bAfterSubFieldTag() const
{
    if ( iChar == 0 )
        return FALSE;

    const char* pszPos = psz() - 1;
    return *pszPos == s_chRightDelimiter;
}

BOOL CSubFieldRecPos::bAfterSubFieldTag(Length* plenTag,
        CMarker** ppmkrSubField, BOOL* pbEndMarker) const
{
    if ( iChar == 0 )
        return FALSE;

    const char* pszRightDelimiter = psz() - 1;
    if ( *pszRightDelimiter != s_chRightDelimiter )
        return FALSE;

    const char* pszLeftDelimiter = pszRightDelimiter;
    const char* pszBegin = pszFieldBeg();
    ASSERT( pszBegin < pszLeftDelimiter );
    while ( *--pszLeftDelimiter != s_chLeftDelimiter )
        ASSERT( pszBegin < pszLeftDelimiter );

    const char* pszMarker = pszLeftDelimiter + 1;
    BOOL bEndMarker = (*pszMarker == '-');
    if ( bEndMarker ) 
        pszMarker++;

    Length lenMarker = pszRightDelimiter - pszMarker;
    ASSERT( pfld );
    CMarker* pmkr = pfld->pmkr()->pmkrsetMyOwner()->
        pmkrSearchForWholeSubString(pszMarker, lenMarker);
    ASSERT( pmkr );
    ASSERT( pmkr->bSubfield() );

    ASSERT( plenTag );
    *plenTag = pszRightDelimiter + 1 - pszLeftDelimiter;
    ASSERT( ppmkrSubField );
    *ppmkrSubField = pmkr;
    ASSERT( pbEndMarker );
    *pbEndMarker = bEndMarker;
    return TRUE;
}

BOOL CSubFieldRecPos::s_bAtExternalSubFieldTag(CMarkerSet* pmkrset,
        char** ppsz, CMarker** ppmkrSubField, BOOL* pbEndMarker)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );
    if ( *psz != '{' )
        return FALSE;

    BOOL bEndMarker = (*++psz == '-');
    if ( bEndMarker ) 
        psz++;
    char* pszRightDelimiter = strchr(psz, '}');
    if ( !pszRightDelimiter )
        return FALSE;

    Length lenMarker = pszRightDelimiter - psz;
    ASSERT( pmkrset );
    CMarker* pmkr = pmkrset->
        pmkrSearchForWholeSubString(psz, lenMarker);
    if ( !pmkr || !pmkr->bSubfield() )
        return FALSE;

    *ppsz = pszRightDelimiter + 1;
    ASSERT( ppmkrSubField );
    *ppmkrSubField = pmkr;
    ASSERT( pbEndMarker );
    *pbEndMarker = bEndMarker;
    return TRUE;
}

BOOL CSubFieldRecPos::bMoveRightPastSubFieldTags()
{
    BOOL bMoved = FALSE;
    Length lenTag = 0;
    CMarker* pmkrSubField = NULL;
    BOOL bEndMarker = FALSE;
    while ( bAtSubFieldTag(&lenTag, &pmkrSubField, &bEndMarker) )
        {
        iChar += lenTag;
        SetSubFieldMarker(pmkrSubField, bEndMarker, TRUE);
        bMoved = TRUE;
        }

    return bMoved;
}

BOOL CSubFieldRecPos::bMoveLeftPastSubFieldTags()
{
    BOOL bMoved = FALSE;
    Length lenTag = 0;
    CMarker* pmkrSubField = NULL;
    BOOL bEndMarker = FALSE;
    while ( bAfterSubFieldTag(&lenTag, &pmkrSubField, &bEndMarker) )
        {
        iChar -= lenTag;
        SetSubFieldMarker(pmkrSubField, bEndMarker, FALSE);
        bMoved = TRUE;
        }

    return bMoved;
}

BOOL CSubFieldRecPos::bMovePastNextViewSubString(CSubFieldRecPos* prpsBegin,
        CSubFieldRecPos* prpsEnd)
{
    const char* pszPos = psz();
    if ( *pszPos == '\0' )
        return FALSE;

    ASSERT( prpsBegin );
    *prpsBegin = *this;

    Length len = strcspn(pszPos, s_pszViewSubStringDelimiters);
    iChar += len;
    ASSERT( prpsEnd );
    *prpsEnd = *this;

    if ( *psz() == '\n' )
        iChar++;
    else if ( bAtSubFieldTag() )
        {
        Length lenTag = 0;
        CMarker* pmkrSubField = NULL;
        BOOL bEndMarker = FALSE;
        while ( bAtSubFieldTag(&lenTag, &pmkrSubField, &bEndMarker) )
            {
            iChar += lenTag;
            SetSubFieldMarker(pmkrSubField, bEndMarker, TRUE);
            }
        }
    else
        ASSERT( *psz() == '\0' );

    return TRUE;
}

static void s_ShiftStringLeft1(char* psz)
{
    ASSERT( psz );
    Length len = strlen(psz);
    memmove(psz, psz + 1, len - 1);
    psz[len] = '\0';
}

void CSubFieldRecPos::s_ReadSubFields(CMarkerSet* pmkrset, char* pszContents)
{
    char* psz = pszContents;
    while ( *psz )
        if ( 0 < *psz && *psz < 32 && *psz != '\n' )
            s_ShiftStringLeft1(psz);
        else if ( *psz == '\\' )
            {
            char chNext = psz[1];
            if ( chNext == '\\' || chNext == '{' )
                s_ShiftStringLeft1(psz);
            psz++;
            }
        else if ( *psz == '{' )
            {
            char* pchLeftDelimiter = psz;
            CMarker* pmkrSubField = NULL;
            BOOL bEndMarker = FALSE;
            if ( s_bAtExternalSubFieldTag(pmkrset,
                    &psz, &pmkrSubField, &bEndMarker) )
                {
                *pchLeftDelimiter = s_chLeftDelimiter;
                char* pchRightDelimiter = psz - 1;
                ASSERT( *pchRightDelimiter == '}' );
                *pchRightDelimiter = s_chRightDelimiter;
                pmkrSubField->IncrementNumberOfFieldRefs();
                }
            else
                psz++;
            }
        else
            psz++;
}

void CSubFieldRecPos::s_WriteSubFields(std::ostream& ios, const Str8& sContents)
{
    const char* psz = sContents;
    do  {
        Length len = strcspn(psz, s_pszWritingSubStringDelimiters);
        ios.write(psz, len);
        psz += len;
        if ( *psz == s_chLeftDelimiter )
            {
            // Write the sub-field marker enclosed in curly braces
            ios << '{';
            const char* pszRightDelimiter = strchr(++psz, s_chRightDelimiter);
            ASSERT( pszRightDelimiter );
            ios.write(psz, pszRightDelimiter - psz);
            ios << '}';
            psz = pszRightDelimiter + 1;
            }
        else if ( *psz )  // If backslash or curly brace
            ios << '\\' << *psz++;
        } while ( *psz );
}

void CSubFieldRecPos::s_DecrementRefsToSubFieldMarkers(CField* pfld)
{
    ASSERT( pfld );
    CSubFieldRecPos rps(0, pfld);
    Length lenTag = 0;
    CMarker* pmkrSubField = NULL;
    BOOL bEndMarker = FALSE;
    while ( rps.bMoveRightToNextSubFieldTag(&lenTag, &pmkrSubField, &bEndMarker) )
        {
        rps.SetPos(rps.psz() + lenTag);
        ASSERT( pmkrSubField );
        pmkrSubField->DecrementNumberOfFieldRefs();
        }
}
#endif // SUBFIELD
