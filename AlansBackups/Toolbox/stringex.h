//File: stringex.h
// Definition for the Str8Ex class.
// Str8Ex is a class derived from Str8 with a few extra member 
// functions thrown in.

// 09-15-1997 changed the class into hungary notation style

#ifndef _STRINGEX_H
#define _STRINGEX_H

class Str8Ex : public Str8 
{
public:
	// Functions that simply call their base class version.
	Str8Ex();
	Str8Ex(const Str8& sSrc);
	Str8Ex(char ch, int iRepeat = 1);
	Str8Ex(const char* psz);
	Str8Ex(const char* pch, int iLength);
#ifdef _NEARDATA
	Str8Ex(LPCSTR psz);
	Str8Ex(LPCSTR pch, int iLength);
#endif
	int iReverseFind(char ch) const;
	// Added functionality - comparison
	BOOL bStartsWith(const char* pszSub) const;
	int iCompareN(const char* pszOther, int in) const;
	// Added functionality - extraction
	Str8 sRightBut(int iCount) const;
	Str8 sLeftBut(int iCount) const;
	int iFindNextWord(int iStartAt, Str8& sRetVal) const;
    int iExtractNext(int iStartAt, Str8& sRetVal, char cBoundary) const;
	// Added functionality - searching
	int iFindFrom (char ch, int iStartAt) const;
	int iFindFrom (const char* pszSub, int iStartAt) const;
	int iReverseFind(const char* pszSub) const;
};

inline Str8Ex::Str8Ex() : Str8() {}
inline Str8Ex::Str8Ex(const Str8& sSrc) : Str8(sSrc) {}
// inline Str8Ex::Str8Ex(char ch, int iRepeat) : Str8(ch, iRepeat) {}
inline Str8Ex::Str8Ex(const char* psz) : Str8(psz) {}
inline Str8Ex::Str8Ex(const char* pch, int iLength) : Str8(pch, iLength) {}
#ifdef _NEARDATA
inline Str8Ex::Str8Ex(LPCSTR psz) : Str8(psz) {}
inline Str8Ex::Str8Ex(LPCSTR pch, int iLength) : Str8(pch, iLength) {}
#endif
inline int Str8Ex::iReverseFind(char ch) const { return Str8::ReverseFind(ch); }
	// (iReverseFind is redeclared because overloading it here confuses the
	// compiler otherwise.)
inline BOOL Str8Ex::bStartsWith(const char* pszSub) const { return !iCompareN(pszSub, strlen(pszSub)); }
inline Str8 Str8Ex::sRightBut(int iCount) const { return Right(GetLength() - iCount); }
inline Str8 Str8Ex::sLeftBut(int iCount) const { return Left(GetLength() - iCount); }

// #define Str8Ex Str8 // 1.4qzga Use Str8 for Str8Ex
#endif
