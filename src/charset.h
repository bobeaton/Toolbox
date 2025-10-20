//File: CharSet.h
//Class definition for the CCharSet class.
//This class stores the character set for a particular language - the allowed
//characters that make up a word, as opposed to whitespace, punctuation, etc.
//Used to determine word boundaries and uppercase/lowercase conversions when 
//parsing an input file.
//By Josh Kelley, 6/97

//09-26-1997 Changed all variable names into names matching with the
//hungarian notation

#ifndef _CHARSET_H
#define _CHARSET_H

#include "stringex.h"

const nCharSetSize = 256;

class CCharSet : public CObject
{
public:
	// Constructors/destructors.
	CCharSet ();
	CCharSet (const Str8& sNewCharSet);
	// Set operations.
	char cFindInSet (char cTest) const;
	BOOL operator== (const CCharSet& ccsOther) const;
	BOOL operator!= (const CCharSet& ccsOther) const;
	void EmptySet ();
	void AddToSet (const Str8& sNewCharSet);
	void AddToSet (char cChar);
	void AddConversion (char cChar, char cConvertTo);
	CCharSet& operator= (const CCharSet& ccsNewCharSet);
	CCharSet& operator= (const Str8& sNewCharSet);
	CCharSet& operator+= (const Str8& sNewCharSet);
	CCharSet& operator+= (char cChar);
	// Operations on words using this character set.
	int iFindNextWord (const Str8& sPhrase, int iStartAt, 
                       Str8& sRetVal);
	int iFindPrevWord (const Str8& csPhrase, int nStartAt, Str8& csRetVal);
	void WordToLower (Str8& csWord);
	BOOL bWordMinMaxLen (Str8Ex& csWord, int& nMinLen, int& nMaxLen);
	BOOL bMatchWord (const Str8& csWord, Str8Ex& csPattern, int nSearchType, BOOL bIgnoreCase);
	void SplitPhrase (const Str8& csPhrase, Str8& csBefore, 
		              Str8& csWord, Str8& csAfter);
	int iMatchPhraseStart (const Str8& csPhrase, const Str8& csPattern,
		                   BOOL bAtWordBoundary, BOOL bIgnoreCase);
	int iMatchPhraseEnd (const Str8& csPhrase, const Str8& csPattern,
		                 BOOL bAtWordBoundary, BOOL bIgnoreCase);
	enum eMatchType { matchMiddle, matchStart, matchEnd, matchWhole	};
	// File I/O.
	DWORD dwGetSizeInFile () const;
	void ReadFromFile (CFile& cfInput);
	void WriteToFile (CFile& cfOutput);
	BOOL bReadFromLangFile (const Str8& csLangFileName, BOOL bLoadVariables);

protected:
	BOOL bMatchSubWord(const Str8& csWord, const Str8& csPattern, 
		               int nWordStart, int nPatternStart, BOOL bMatchWhole, 
                       BOOL bIgnoreCase);
	BOOL bReverseMatchSubWord(const Str8& csWord, const Str8& csPattern,
		                     int nWordStart, int nPatternStart, 
                             BOOL bMatchWhole, BOOL bIgnoreCase);
	char pszCharSet[nCharSetSize];  
        // One entry for each possible ASCII value.
		// If an ASCII value's entry is 0, that means that the ASCII value is
		// not a valid character; otherwise, the entry gives the character that
		// corresponds to that ASCII value.  (For normal operation, the two
		// are the same; however, differing values can be used for
		// uppercase/lowercase conversions, etc.)
	CMapStringToString cmssVarSet;
		// A set of variables.  Used for concordance/lookup but not for 
		// word lists.
};

// Returns the amount of space that this (or any CCharSet) would take in
// a file.  Used when computing header size.  If CCharSet ever goes to a non-
// fixed length structure, then this function and anyone calling it will need
// to be changed.
inline DWORD CCharSet::dwGetSizeInFile () const
{
	return nCharSetSize;
}	

// Returns FALSE if cTest is not in this character set, otherwise returns how
// cTest appears in this character set.
inline char CCharSet::cFindInSet (char cTest) const
{
    unsigned char ucChar = (unsigned char) cTest;  //09-24-1997 (for prefenting bug)
    
    return pszCharSet[ucChar];
}

// Returns TRUE if this is not identical to ccsOther, FALSE if they are 
// identical.
inline BOOL CCharSet::operator!= (const CCharSet& ccsOther) const
{
	return !operator==(ccsOther);
}

// Adds cChar to this.  A simplified version of AddConversion.
inline void CCharSet::AddToSet (char cChar)
{
	AddConversion(cChar, cChar);
}

// Alias for AddToSet.
inline CCharSet& CCharSet::operator+= (const Str8& sNewCharSet)
{
	AddToSet(sNewCharSet);
	return *this;
}

// Alias for AddToSet.
inline CCharSet& CCharSet::operator+= (char cChar)
{
	AddToSet(cChar);
	return *this;
}

#endif
