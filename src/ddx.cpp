// ddx.cpp  Shoebox custom dialog data exchange

#include "stdafx.h"
#include "toolbox.h"
#include "ddx.h"
#include "patch.h"  // Shw_bAtWhiteSpace


BOOL Shw_bNeedsToBeTrimmed(const char* pszText,
        int *plenPrecedingWS, int* plenFollowingWS)
{
    // Preceding white space
    int lenPrecedingWS = 0;
    const char* psz = pszText;
    ASSERT( psz );
    for ( ; Shw_bAtWhiteSpace(psz); psz++ )
        lenPrecedingWS += 1;

    // Following white space
    int lenFollowingWS = 0;
    if ( *psz )
        {
        psz = pszText + strlen(pszText);
        while ( pszText < psz && Shw_bAtWhiteSpace(--psz) )
            lenFollowingWS += 1;
        }
    
    ASSERT( plenPrecedingWS );
    *plenPrecedingWS = lenPrecedingWS;
    ASSERT( plenFollowingWS );
    *plenFollowingWS = lenFollowingWS;
    return ( lenPrecedingWS != 0 || lenFollowingWS != 0 );
}
