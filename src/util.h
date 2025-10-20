// util.h  

#ifndef	UTIL_H
#define UTIL_H

#include "str8.h"

#undef ASSERT // Use our own ASSERT for release versions, reports the line number of the problem
#define ASSERT(f) ((f) ? (void)0 : ::MyAssertFailedLine(__FILE__, __LINE__))
void MyAssertFailedLine( char* pszFileName, int nLine);
Str8 sGetFileName( Str8 sFile );

#endif  // UTIL_H
