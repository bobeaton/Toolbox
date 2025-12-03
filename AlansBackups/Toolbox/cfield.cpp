// cfield.cpp Implementation of Standard format field (1995-02-28)

#include "stdafx.h"
#include "toolbox.h"
#include "cfield.h"
#include "crecpos.h"  // s_DecrementRefsToSubfieldMarkers
#include "crngset.h"
#include "jmp.h"
#include "cck.h" // CCheckingState

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// ==========================================================================

CField::~CField()
{
    if ( pmkr()->pmkrsetMyOwner()->bSubfields() )
        CRecPos::s_DecrementRefsToSubfieldMarkers(this);
}

// If data is defined to be a single "word," return FALSE and set nID to 
// IDSx_cck_ExpectsSingleWord if any embedded whitespace is found. If data field
// can't be empty and it is, return FALSE and set nID to IDSx_cck_DataRequired.
// If data field is a single item and has explicit embedded newlines, return
// FALSE and set nID to IDSx_cck_ExplicitNewlines.
// If there is any inconsistency, it also sets the rpsStart and rpsEnd positions
// to indicate the portion of the data that has the error. Note that if data
// has multiple words and shouldn't the inconsistent data spans from the first
// non-white character to the last non-white character. For missing data, the
// start and end positions are both set to 0, and any trailing whitespace is
// left untouched. The values of rpsStart, rpsEnd, and nID are undefined if this
// function returns TRUE.
BOOL CField::bValidItemCount(CRecord* prec, CRecPos& rpsStart, CRecPos& rpsEnd, Str8& sMessage, BOOL& bExplicitNewlines) // 1.4heq Get rid of ID arg
{
	bExplicitNewlines = FALSE; // 1.4heq
    rpsStart.SetPos(0, this, prec);
    while ( rpsStart.bIsWhite() ) // While not at beginning of first word...
        rpsStart.iChar++;
    
    if ( rpsStart.iChar < GetLength() ) // String is neither "empty" nor all whitespace
        {
        if ( pmkr()->bMultipleItemData() )
            return TRUE; // Don't need to set rpsEnd because there is no inconsistency.
        // Look backward from end of field to find last non-white character, if any.
        rpsEnd.SetPos(GetLength(), this, prec);
        while ( rpsEnd.bLeftIsWhite() )
            rpsEnd.iChar--;
        // If string is only 1 or 2 charcters, then there's no room for embedded whitespace
        if ( rpsEnd.iChar < rpsStart.iChar + 2 )
            return TRUE;
        if ( sItem(rpsStart, rpsEnd, FALSE).Find((pmkr()->bNoWordWrap())?"\n":"\n\n") != -1 )
            {
			sMessage = _("Explicit line break in data item."); // 1.4heq
			bExplicitNewlines = FALSE; // 1.4heq
            return FALSE;
            }
        if ( pmkr()->bMultipleWordItems() )
            return TRUE;
        CLangEnc* plng = pmkr()->plng();
        ASSERT( plng );
		sMessage = _("This data field expects a single word."); // 1.4heq
        return !plng->bContainsWhite( sItem(rpsStart, rpsEnd, FALSE) );
        }

    // Now we've determined that field contains no non-white characters
    if ( pmkr()->bAllowEmptyContents() )
        return TRUE;
	sMessage = _("This field requires data."); // 1.4heq
    rpsStart.SetPos(0, this, prec);
    rpsEnd = rpsStart;
    return FALSE;
}

// Set rpsStart to the first non-whitespace character. Set rpsEnd to the position just
// beyond the last non-whitespace character in the first item, which may be
// either a word or the entire field contents (based on pmkr->bMultipleItemData). If
// no data item exists (i.e., field is empty or has only whitespace), the values of
// rpsStart and rpsEnd are undefined and the function returns FALSE.
BOOL CField::bParseFirstItem(CRecord* prec, CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData)
{
    rpsStart.SetPos(0, this, prec);  // Beginning of the data field
    rpsStart.bSkipWhite();
    return bParseCurItem(rpsStart, rpsEnd, bMultipleItemData);
}

// Set rpsStart to the first non-whitespace character on or after the passed-in rpsEnd value.
// Set rpsEnd to the position just beyond the last non-whitespace character in the next
// item. If no more data items exist, the values of rpsStart and rpsEnd are
// undefined, and this function returns FALSE.
BOOL CField::bParseNextItem(CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData)
{
    ASSERT(this == rpsEnd.pfld);
    rpsStart = rpsEnd;
    if ( !*rpsStart.psz() ) // This is an optimization for the common case
        return FALSE;
    return bParseCurItem(rpsStart, rpsEnd, bMultipleItemData);
}

// Set rpsStart to the first non-whitespace character on or after the passed-in rpsStart
// value. Set rpsEnd to the position just beyond the last non-whitespace character in the
// next item. If no more data items exist, the values of rpsStart and rpsEnd are undefined,
// and this function returns FALSE.
BOOL CField::bParseCurItem(CRecPos& rpsStart, CRecPos& rpsEnd, BOOL bMultipleItemData)
{
    ASSERT(this == rpsStart.pfld);
    if ( bMultipleItemData )
        {
        rpsStart.bSkipWhite();
        rpsEnd = rpsStart;
        return rpsEnd.bSkipNonWhite(); // Skip to end of word
        }
    else
        {
        while ( rpsStart.bIsWhite() ) // While not at beginning of first word...
            {
            if ( rpsStart.bEndOfItem() )
                return FALSE;
            rpsStart.iChar++;
            }
        rpsEnd = rpsStart;
        // 2000-08-11 TLB: Added code to trim trailing whitespace.
        int iLastNonWhite = rpsEnd.iChar;
        while ( !rpsEnd.bEndOfItem() ) // While not at end of item...
            {
            rpsEnd.iChar++;
            if ( rpsEnd.bLeftIsNonWhite() )
                {
                iLastNonWhite = rpsEnd.iChar;
                }
            }
        rpsEnd.iChar = iLastNonWhite;
        return ( rpsStart.iChar != rpsEnd.iChar );
        }
}

// Return a Str8 object representing the item delimited by rpsStart and rpsEnd.
// Return value is undefined (function will assert) if rpsEnd <= rpsStart.
Str8 CField::sItem(const CRecPos& rpsStart, const CRecPos& rpsEnd, BOOL bHandleMultiLineData)
{
    ASSERT( this == rpsStart.pfld);
    ASSERT( this == rpsEnd.pfld);
    int lenWord = rpsEnd.iChar - rpsStart.iChar;
    ASSERT(lenWord >= 0);
    Str8 s(rpsStart.psz(), lenWord);
    // If caller said to handle multi-line data, we have a potentially multi-word string,
    // which we need to check for newlines. If this is a word-wrap field, single newlines
    // get converted to spaces (since they presumably represent Shoebox's work of converting
    // spaces to newlines), and the first double-newline forces truncation of the string.
    // If this is NOT a word-wrap field, then the first newline forces truncation of the
    // string.
    if ( bHandleMultiLineData )
        {
        for (int i = 0; i < s.GetLength(); i++)
            if ( s[i] == '\n' )
                s.SetAt(i, ' ');
        }
    return s;
}
