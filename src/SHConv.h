#ifndef _MAC
#pragma once

#include "AFXPRIV.H"

#define W2U8(lpw) (\
	((_lpw = lpw) == NULL) ? NULL : (\
		_convert = (lstrlenW(_lpw)+1)*4,\
		AtlW2AHelper((LPSTR) alloca(_convert), _lpw, _convert, CP_UTF8)))

#define U82W(lpa) (\
	((_lpa = lpa) == NULL) ? NULL : (\
		_convert = (lstrlenA(_lpa)+1),\
		AtlA2WHelper((LPWSTR) alloca(_convert*3), _lpa, _convert, CP_UTF8)))

#define U82CW(lpa)      ((LPCWSTR)U82W(lpa))
#define W2CU8(lpw)      ((LPCSTR)W2U8(lpw))
#endif // _MAC