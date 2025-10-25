//File: stringex.cpp
// Implementation of the Str8Ex class.
// See stringex.h for more information.

#include "stdafx.h"
#include "toolbox.h"
#include "stringex.h"
#undef Str8Ex // 1.4qzga Try using Str8 for StringEx

#include <limits.h>
#include <ctype.h>

// This function searches for ch in this, starting at offset iStartAt
// within this.  It returns the zero-based offset within this of the next
// occurance; -1 if not found.
int Str8Ex::iFindFrom (char ch, int iStartAt) const
{
	char* pszResult;
    const char* pszThis = operator const char*();
	pszResult = (char*)strchr(pszThis + iStartAt, ch);
	return (pszResult == NULL) ? -1 : (int)(pszResult - pszThis);
}

// This function searches for pszSub in this, starting at offset iStartAt
// within this.  It returns the zero-based offset within this of the next
// occurance; -1 if not found.
int Str8Ex::iFindFrom (const char* pszSub, int iStartAt) const
{
    const char* pszThis = operator LPCSTR();
	char* pszResult;
	pszResult = (char*)strstr(pszThis + iStartAt, pszSub);
	return (pszResult == NULL) ? -1 : (int)(pszResult - pszThis);
}

// This function searches for pszSub in this, starting at the end and
// working its way backwards.  It returns the zero-based offset within this
// of the next occurance; -1 if not found.
int Str8Ex::iReverseFind (const char* pszSub) const
{
	int iSubLen = strlen(pszSub);
    int iThisLen = GetLength(); 
	int	iStrPos = iThisLen - 1;
	const char* pszThisBase = operator const char*(); 
    const char* pszThisLoop;
	const char* pszInThis;
	const char* pszInSub;
    ASSERT(pszSub != NULL);
    if (iSubLen > iThisLen)
    	return -1;
    // The following code is taken from the source code for strstr, with
    // modifications.
    pszThisLoop = pszThisBase + iThisLen - iSubLen + 1;
    // pszThisLoop is kept higher by 1 to keep it from wrapping around and
    // always being >= pszThisBase.
	while (pszThisLoop > pszThisBase)
	    {
		pszThisLoop--;
		pszInThis = pszThisLoop;
		pszInSub = (char*) pszSub;
		while (*pszInSub && !(*pszInThis-*pszInSub) )
			pszInThis++, pszInSub++;
		
        if (!*pszInSub)
			return(pszThisLoop - pszThisBase);
	}
	return(-1);
}
 
// This function is similar to strncmp - it compares the first in characters
// of this with pszOther.
int Str8Ex::iCompareN (const char* pszOther, int in) const
{
	return strncmp(operator const char*(), pszOther, in);
}

// Finds the next word in this, starting from offset iStartAt, and stores it 
// in sRetVal.  Returns the offset of the space immediately following sRetVal.
// Note that this function determines word boundaries based solely upon 
// whitespace (_istspace), not upon any character set, punctuation 
// considerations, etc.  If no word is found, sRetVal is set to the empty 
// string and the return value is -1.
int Str8Ex::iFindNextWord (int iStartAt, Str8& sRetVal) const
{
	int iWordEnd;
    int iLineLength = GetLength();
	while ((iStartAt < iLineLength) && _istspace(GetChar(iStartAt)))
		iStartAt++;

	if (iStartAt == iLineLength) 
        {
		sRetVal.Empty();
		return -1;
	    } 
    else 
        {
		for (iWordEnd = iStartAt; (iWordEnd < iLineLength) && 
				!_istspace(GetChar(iWordEnd)); iWordEnd++)
			;

		sRetVal = Mid(iStartAt, iWordEnd - iStartAt);
		return iWordEnd;
	    } // else - if
}

// This function is similar to iFindNextWord, except that it uses cBoundary
// to determine where to break strings rather than using _istspace.  On exit,
// sRetVal contains the sub-string that is extracted (may be empty).  The
// function returns the offset of the character following the chBoundary
// that terminates sRetVal.  (Note that the return value may be greater than
// the length of this.)
int Str8Ex::iExtractNext(int iStartAt, Str8& sRetVal, char cBoundary) const
{
    ASSERT(iStartAt < GetLength());
    int iEndAt = iFindFrom(cBoundary, iStartAt);
    if (iEndAt == -1)
        iEndAt = GetLength();

    sRetVal = Mid(iStartAt, iEndAt - iStartAt);
    return iEndAt+1;
}




