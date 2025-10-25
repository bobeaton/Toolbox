// crecpos.cpp  Implementation of record position

#include "stdafx.h"
#include "toolbox.h"
#include "crecpos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////
BOOL CRecPos::operator<(const CRecPos& rps) const
{
    ASSERT( prec == rps.prec ); // this could change to allow comparisons across records  BJY
    
    if ( pfld != rps.pfld ) // see if rps follows pfld in field list
        {
        for ( CField* pfield = prec->pfldNext( pfld ); pfield; pfield = prec->pfldNext( pfield ) )
            if ( pfield == rps.pfld )
                return TRUE;
        return FALSE;
        }
    else    // same field, just compare position within field
        return( iChar < rps.iChar );
}

////////////////////////////////////////////////////////////
BOOL CRecPos::operator>(const CRecPos& rps) const
{
    ASSERT( prec == rps.prec ); // this could change to allow comparisons across records  BJY
    
    if ( pfld != rps.pfld ) // see if rps precedes pfld in field list
        {
        for ( CField* pfield = prec->pfldPrev( pfld ); pfield; pfield = prec->pfldPrev( pfield ) )
            if ( pfield == rps.pfld )
                return TRUE;
        return FALSE;
        }
    else    // same field, just compare position within field
        return( iChar > rps.iChar );
}

////////////////////////////////////////////////////////////
BOOL CRecPos::operator==(const CRecPos& rps) const
{
    ASSERT( prec == rps.prec ); // this could change to allow comparisons across records  BJY
    return ( pfld == rps.pfld && iChar == rps.iChar );
}

#ifdef _DEBUG
void CRecPos::AssertValid() const
{   
    prec->AssertValid();
    pfld->AssertValid();
    ASSERT( iChar >= 0 );
    ASSERT( iChar <= pfld->GetLength() );
}
#endif      

// --------------------------------------------------------------------------
// Subfields in Standard Format are like character styles in RTF.

static const char s_chLeftDelimiter = '\x01';
static const char s_chRightDelimiter = '\x02';
static const char s_pszSubfieldTagsFormat[] =
    {
    s_chLeftDelimiter, '%', 's', s_chRightDelimiter,  // Begin tag
    s_chLeftDelimiter, '-', '%', 's', s_chRightDelimiter  // End tag
    };
static const char s_pszViewSubstringDelimiters[] =
    {
    '\n',
#ifdef SUBFIELD
    s_chLeftDelimiter,
#endif // SUBFIELD
    '\0'
    };

static const char s_pszWritingSubstringDelimiters[] =
    {
#ifdef SUBFIELD
    s_chLeftDelimiter,
#endif // SUBFIELD
    '\\',
    '{',
    '\0'
    };

#ifdef SUBFIELD
void CRecPos::SetSubfieldMarker()
{
    m_pmkrSubfield = NULL;
    if ( !pfld )
        return;

    ASSERT( 0 <= iChar );
    int iPos = iChar;
    iChar = 0;
    while ( iChar != iPos )
        {
        ASSERT( iChar < iPos );
        if ( bAtSubfieldTag() )
            {
            Length lenTag = 0;
            CMarker* pmkrSubfield = NULL;
            BOOL bEndTag = FALSE;
            (void) bAtSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag);
            SetSubfieldMarker(pmkrSubfield, bEndTag, TRUE);
            iChar += lenTag;
            }
        else
            iChar++;
        }
    iChar = iPos;
}

void CRecPos::SetSubfieldMarker(CMarker* pmkrSubfield, BOOL bEndTag,
        BOOL bMovingRight)
{
    if ( bEndTag && bMovingRight || !bEndTag && !bMovingRight )
        {
        // Pop the subfield marker
        ASSERT( m_pmkrSubfield );
        ASSERT( m_pmkrSubfield == pmkrSubfield );
        m_pmkrSubfield = NULL;
        }
    else
        {
        // Push the subfield marker
        ASSERT( !m_pmkrSubfield );
        m_pmkrSubfield = pmkrSubfield;
        ASSERT( m_pmkrSubfield );
        }
}


BOOL CRecPos::bMoveRightToNextSubfieldTag(Length* plenTag,
        CMarker** ppmkrSubfield, BOOL* pbEndTag)
{
    const char* pszPos = strchr(psz(), s_chLeftDelimiter);
    if ( !pszPos )
        return FALSE;

    SetPos(pszPos);
    BOOL b = bAtSubfieldTag(plenTag, ppmkrSubfield, pbEndTag);
    ASSERT( b );
    return TRUE;
}

BOOL CRecPos::bAtSubfieldTag() const
{
    return *psz() == s_chLeftDelimiter;
}

BOOL CRecPos::s_bAtSubfieldTag(const char* psz)
{
    ASSERT( psz );
    return *psz == s_chLeftDelimiter;
}

BOOL CRecPos::bAtSubfieldTag(Length* plenTag,
        CMarker** ppmkrSubfield, BOOL* pbEndTag) const
{
    const char* pszLeftDelimiter = psz();
    if ( *pszLeftDelimiter != s_chLeftDelimiter )
        return FALSE;

    const char* pszMarker = pszLeftDelimiter + 1;
    BOOL bEndTag = (*pszMarker == '-');
    if ( bEndTag ) 
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
    ASSERT( ppmkrSubfield );
    *ppmkrSubfield = pmkr;
    ASSERT( pbEndTag );
    *pbEndTag = bEndTag;
    return TRUE;
}

BOOL CRecPos::s_bAtSubfieldTag(CMarkerSet* pmkrset,
        const char** ppsz, CMarker** ppmkrSubfield, BOOL* pbEndTag)
{
    ASSERT( ppsz );
    const char* pszLeftDelimiter = *ppsz;
    ASSERT( pszLeftDelimiter );
    if ( *pszLeftDelimiter != s_chLeftDelimiter )
        return FALSE;

    const char* pszMarker = pszLeftDelimiter + 1;
    BOOL bEndTag = (*pszMarker == '-');
    if ( bEndTag ) 
        pszMarker++;
    const char* pszRightDelimiter = strchr(pszMarker, s_chRightDelimiter);
    ASSERT( pszRightDelimiter );

    Length lenMarker = pszRightDelimiter - pszMarker;
    ASSERT( pmkrset );
    CMarker* pmkr = pmkrset->pmkrSearchForWholeSubString(pszMarker, lenMarker);
    ASSERT( pmkr );
    ASSERT( pmkr->bSubfield() );

    *ppsz = pszRightDelimiter + 1;
    ASSERT( ppmkrSubfield );
    *ppmkrSubfield = pmkr;
    ASSERT( pbEndTag );
    *pbEndTag = bEndTag;
    return TRUE;
}

BOOL CRecPos::bAfterSubfieldTag() const
{
    if ( iChar == 0 )
        return FALSE;

    const char* pszPos = psz() - 1;
    return *pszPos == s_chRightDelimiter;
}

BOOL CRecPos::bAfterSubfieldTag(Length* plenTag,
        CMarker** ppmkrSubfield, BOOL* pbEndTag) const
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
    BOOL bEndTag = (*pszMarker == '-');
    if ( bEndTag ) 
        pszMarker++;

    Length lenMarker = pszRightDelimiter - pszMarker;
    ASSERT( pfld );
    CMarker* pmkr = pfld->pmkr()->pmkrsetMyOwner()->
        pmkrSearchForWholeSubString(pszMarker, lenMarker);
    ASSERT( pmkr );
    ASSERT( pmkr->bSubfield() );

    ASSERT( plenTag );
    *plenTag = pszRightDelimiter + 1 - pszLeftDelimiter;
    ASSERT( ppmkrSubfield );
    *ppmkrSubfield = pmkr;
    ASSERT( pbEndTag );
    *pbEndTag = bEndTag;
    return TRUE;
}

BOOL CRecPos::s_bAtExternalSubfieldTag(const char* psz, CMarkerSet* pmkrset,
        Length* plenTag, CMarker** ppmkrSubfield, BOOL* pbEndTag)
{
    ASSERT( psz );
    if ( *psz != '{' )
        return FALSE;

    const char* pszLeftDelimiter = psz;
    BOOL bEndTag = (*++psz == '-');
    if ( bEndTag ) 
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

    ASSERT( plenTag );
    *plenTag = pszRightDelimiter + 1 - pszLeftDelimiter;
    ASSERT( ppmkrSubfield );
    *ppmkrSubfield = pmkr;
    ASSERT( pbEndTag );
    *pbEndTag = bEndTag;
    return TRUE;
}

BOOL CRecPos::bInsertSubfield(CMarker* pmkrSubfield)
{
    if ( m_pmkrSubfield )
        return FALSE;

    ASSERT( pmkrSubfield );
    ASSERT( pmkrSubfield->bSubfield() );
    const char* pszMarker = pmkrSubfield->sMarker();
    int lenMarker = strlen(pszMarker);
    Str8 sSubfieldTags;
    sSubfieldTags.Format(s_pszSubfieldTagsFormat, pszMarker, pszMarker);
    ASSERT( sSubfieldTags.GetLength() == 5 + lenMarker + lenMarker );
    ASSERT( pfld );
    pfld->Insert(sSubfieldTags, iChar);
    pmkrSubfield->IncrementNumberOfFieldRefs();
    BOOL b = bMoveRightPastSubfieldTag();
    ASSERT( b );
    return TRUE;
}

BOOL CRecPos::bMoveRightPastSubfieldTag()
{
    Length lenTag = 0;
    CMarker* pmkrSubfield = NULL;
    BOOL bEndTag = FALSE;
    if ( bAtSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag) )
        {
        iChar += lenTag;
        SetSubfieldMarker(pmkrSubfield, bEndTag, TRUE);
        return TRUE;
        }

    return FALSE;
}

BOOL CRecPos::bMoveRightPastSubfieldTags()
{
    BOOL bMoved = FALSE;
    Length lenTag = 0;
    CMarker* pmkrSubfield = NULL;
    BOOL bEndTag = FALSE;
    while ( bAtSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag) )
        {
        iChar += lenTag;
        SetSubfieldMarker(pmkrSubfield, bEndTag, TRUE);
        bMoved = TRUE;
        }

    return bMoved;
}

BOOL CRecPos::bMoveLeftPastSubfieldTags()
{
    BOOL bMoved = FALSE;
    Length lenTag = 0;
    CMarker* pmkrSubfield = NULL;
    BOOL bEndTag = FALSE;
    while ( bAfterSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag) )
        {
        iChar -= lenTag;
        SetSubfieldMarker(pmkrSubfield, bEndTag, FALSE);
        bMoved = TRUE;
        }

    return bMoved;
}
#endif // SUBFIELD

BOOL CRecPos::bMoveHome()
{
    BOOL bMoved = FALSE;
    const char* pszPos = psz();
    while ( iChar != 0 && *(pszPos - 1) != '\n' )
        {
        ASSERT( 0 <= iChar );
#ifdef SUBFIELD
		if ( bMoveLeftPastSubfieldTags() )
            pszPos = psz();  // Character offset iChar has changed
        else
#endif // SUBFIELD
            {
            iChar--;
            pszPos--;
            }
        bMoved = TRUE;
        }

    return bMoved;
}

BOOL CRecPos::bMoveEnd()
{
    BOOL bMoved = FALSE;
    const char* pszPos = psz();
    while ( *pszPos != '\0' && *pszPos != '\n' )
        {
        ASSERT( 0 <= iChar );
#ifdef SUBFIELD
		if ( bMoveRightPastSubfieldTags() )
            pszPos = psz();  // Character offset iChar has changed
        else
#endif // SUBFIELD
            {
            iChar++;
            pszPos++;
            }
        bMoved = TRUE;
        }

    return bMoved;
}

void CRecPos::MoveRightPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE)
{
    ASSERT( prpsB ); // These must point to something
	ASSERT( prpsE );
   *prpsB = *this; // Remember starting point
    const char* pszPos = psz();
    Length len = strcspn(pszPos, s_pszViewSubstringDelimiters); // Find next substring start tag or nl
    iChar += len; // Move past 
    *prpsE = *this;

    if ( *psz() == '\n' )
		{
		if ( iChar < (int)pfld->GetLength() ) // 1.4hfe Fix bug of hang on browse wordlist ref
	        iChar++;
		}
#ifdef SUBFIELD
    else if ( bAtSubfieldTag() )
        {
        Length lenTag = 0;
        CMarker* pmkrSubfield = NULL;
        BOOL bEndTag = FALSE;
        while ( bAtSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag) )
            {
            iChar += lenTag;
            SetSubfieldMarker(pmkrSubfield, bEndTag, TRUE);
            }
        }
#endif // SUBFIELD
    else
        ASSERT( *psz() == '\0' );
}

BOOL CRecPos::bMoveRightPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE)
{
    if ( bEndOfField() ) // Fail only at end of field, otherwise MoveRight... is guaranteed to move
        return FALSE;
	MoveRightPastViewSubstring( prpsB, prpsE );
	return TRUE;
}

static void s_ShiftStringLeft1(char* psz)
{
    ASSERT( psz );
    Length len = strlen(psz);
    memmove(psz, psz + 1, len - 1);
    psz[len] = '\0';
}

void CRecPos::s_ReadSubfields(CMarkerSet* pmkrset, char* pszContents)
{
    CMarker* pmkrSubfield = NULL;
    char* pchBeginTag = NULL;
    Length lenBeginTag = 0;
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
            BOOL bTag = FALSE;
            Length lenTag = 0;
            CMarker* pmkr = NULL;
            BOOL bEndTag = FALSE;
            if ( s_bAtExternalSubfieldTag(psz, pmkrset, &lenTag, &pmkr, &bEndTag) )
                {
                if ( bEndTag )
                    {
                    if ( pmkr == pmkrSubfield )
                        {
                        // Convert this end tag's delimiters
                        *psz = s_chLeftDelimiter;
                        psz += lenTag;
                        char* pchRightDelimiter = psz - 1;
                        ASSERT( *pchRightDelimiter == '}' );
                        *pchRightDelimiter = s_chRightDelimiter;
                        
                        // Convert its matching begin tag's delimiters
                        *pchBeginTag = s_chLeftDelimiter;
                        pchRightDelimiter = pchBeginTag + lenBeginTag - 1;
                        ASSERT( *pchRightDelimiter == '}' );
                        *pchRightDelimiter = s_chRightDelimiter;
                        
                        pmkrSubfield->IncrementNumberOfFieldRefs();
                        pmkrSubfield = NULL;
                        pchBeginTag = NULL;
                        lenTag = 0;
                        bTag = TRUE;
                        }
                    }
                else  // Begin tag
                    {
                    if ( !pmkrSubfield )
                        {
                        // Remember this begin tag's location
                        pmkrSubfield = pmkr;
                        pchBeginTag = psz;
                        lenBeginTag = lenTag;
                        
                        psz += lenTag;
                        bTag = TRUE;
                        }
                    }
                }
            if ( !bTag )
                psz++;
            }
        else
            psz++;
}

void CRecPos::s_WriteSubfields(std::ostream& ios, const Str8& sContents)
{
    const char* psz = sContents;
#ifdef SUBFIELD
	do  {
        Length len = strcspn(psz, s_pszWritingSubstringDelimiters);
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
#else
        ios.write(psz, strlen(psz));
#endif // SUBFIELD
}

#ifdef SUBFIELD
void CRecPos::s_DecrementRefsToSubfieldMarkers(CField* pfld)
{
    ASSERT( pfld );
    CRecPos rps(0, pfld);
    Length lenTag = 0;
    CMarker* pmkrSubfield = NULL;
    BOOL bEndTag = FALSE;
    while ( rps.bMoveRightToNextSubfieldTag(&lenTag, &pmkrSubfield, &bEndTag) )
        {
        rps.SetPos(rps.psz() + lenTag);
        ASSERT( pmkrSubfield );
        if ( !bEndTag )
            pmkrSubfield->DecrementNumberOfFieldRefs();
        }
}
#endif // SUBFIELD

//09-05-1997
BOOL CRecPos::bMoveLeftPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE)
{
    if (iChar == 0)
        return FALSE;             // begin of field 
            
      // These must point to something
    ASSERT(prpsB);
    ASSERT(prpsE);

    *prpsE = *this;               // Mark the end of the string
    
      //Construct a string with delimiters (Need Right Delimiter for Subfields)
     char pszSubstrDelimiters[] =
     {
        '\n',
#ifdef SUBFIELD
        s_chRightDelimiter,
#endif // SUBFIELD
        '\0'
     };

      // Search for a delimiter
    do iChar--;
        while ( (iChar != 0) && (!strchr(pszSubstrDelimiters, *psz())) );

    *prpsB = *this;
    
    // We have now to destinguish between begin of Field or not
    if ( iChar != 0 || *psz() == '\n' )         // second condition provides right
                                                // behavior in the special case
                                                // of '\n' at the begin of field
        prpsB->iChar++; //We want the begin of the string and not the delimiter
    

#ifdef SUBFIELD
    if (bAtSubfieldTag() )
        {
        //???
        // TODO : Implement the right behavior, if the recordposition 
        //        reaches a Subfieldmarker
        }
#endif //SUBFIELD

    return TRUE;

} //bMoveLeftPastViewSubstring 










