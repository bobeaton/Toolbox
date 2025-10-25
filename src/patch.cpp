// patch.cpp Implementation of Shoebox single char pattern classes (1995-02-27)


// Change history:
// 1995-02-28 0.07a MRP: actually distinction is between single and multi-char
// 1995-02-27 0.07  MRP: separate internal program patterns from end-user ones


#include "stdafx.h"
#include "toolbox.h"
#include "patch.h"
#include "shwnotes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


#define new DEBUG_NEW


// **************************************************************************

BOOL bMemberOfCharSet(char ch, const char* pszCharSet)
    { return (strchr(pszCharSet, ch) != NULL); }

BOOL CPatChar::s_bMatchCase = FALSE;


CPatChar::CPatChar(CPatElChar* ppatel)
{
	ASSERT(ppatel != NULL);
	Add(ppatel);
}


CPatChar::CPatChar(CPatElChar* ppatel1, CPatElChar* ppatel2)
{
	ASSERT(ppatel1 != NULL);
	ASSERT(ppatel2 != NULL);
	Add(ppatel1);
	Add(ppatel2);
}

    
BOOL CPatChar::bMatch(const char* psz) const
{
	for ( ; *psz != '\0'; psz++)
		if(bMatchAt(&psz))
			return TRUE;
		
	return FALSE;
}


BOOL CPatChar::bMatchAt(const char** ppsz) const
{   
	CPatElChar* ppatelA = ppatelFirst();
	BOOL bMatched = bMatchAt(ppatelA, ppsz);
	
	return bMatched;
}


BOOL CPatChar::bMatchAt(const char** ppsz, Str8& sMatched) const
{   
	const char* pszMatchStart = *ppsz;
	BOOL bMatched = bMatchAt(ppsz);
	if (! bMatched) { return FALSE; }
	
	int nMatched = *ppsz - pszMatchStart;
	// Note; in general, the matched substring is not null terminated.
	char* pchMatched = sMatched.GetBuffer(nMatched); // 1.4qzfv GetBuffer OK because written immediately
	strncpy_s(pchMatched, nMatched, pszMatchStart, _TRUNCATE);
	sMatched.ReleaseBuffer(nMatched);
	
	return TRUE;
}


BOOL CPatChar::bMatchAt(CPatElChar* ppatelA, const char** ppsz) const
{
	// End of pattern
	if (ppatelA == NULL)
		return TRUE;
                     
    // Match this pattern element
	const char* psz = *ppsz;
	const char* pszAtA = psz;
	BOOL bMatched = ppatelA->bMatchAt(&psz);
	if (! bMatched) { return FALSE; }
    
    // Match rest of pattern
	const char* pszAfterA = psz;			
	CPatElChar* ppatelBC = ppatelNext(ppatelA);
	bMatched = bMatchAt(ppatelBC, &psz);
	if (! bMatched) { return FALSE; }
	
	*ppsz = psz;
	return TRUE;
}
 

#ifdef _DEBUG
void CPatChar::AssertValid() const
{
	ASSERT(! bIsEmpty());
	CPatElChar* ppatel;
	for (ppatel = ppatelFirst(); ppatel != NULL; ppatel = ppatelNext(ppatel))
		ppatel->AssertValid();
}
#endif  // _DEBUG


const char* Shw_pszNL = "\n";
const char* Shw_pszSpaceNoNL =	" \t";   // White space w/o newline.
const char* Shw_pszWhiteSpace = " \t\n";  // white space: space, tab, newline
const char* Shw_pszOtherWhiteSpace = "\t\n";  // above, other than space itself
// Zero or more occurrences of white space
static const CPatChar s_patWhiteSpace0(
						new CPatElRep(new CPatElSet(Shw_pszWhiteSpace)));
// One or more occurrences of white space
static const CPatChar s_patWhiteSpace1(
						new CPatElRep(new CPatElSet(Shw_pszWhiteSpace), 1));
// No newline -- Zero or more occurrences of white space
static const CPatChar s_patWhiteSpaceNoNL0(
						new CPatElRep(new CPatElSet(Shw_pszSpaceNoNL)));
// No newline --  One or more occurrences of white space
static const CPatChar s_patWhiteSpaceNoNL1(
						new CPatElRep(new CPatElSet(Shw_pszSpaceNoNL), 1));
static const CPatChar s_patNewLine(
						new CPatElRep(new CPatElSet(Shw_pszNL), 1));

static const CPatChar s_patName(
						new CPatElRep(new CPatElNotSet(Shw_pszWhiteSpace), 1));

void Shw_MatchOptionalWhiteSpaceAt(const char** ppsz)
{
	(void) s_patWhiteSpace0.bMatchAt(ppsz);
}


BOOL Shw_bMatchOptionalWhiteSpaceAt(const char** ppsz)
{
	BOOL b = s_patWhiteSpace0.bMatchAt(ppsz);

	return b;
}

BOOL Shw_bMatchWhiteSpaceAt(const char** ppsz)
{
	BOOL b = s_patWhiteSpace1.bMatchAt(ppsz);
	
	return b;
}

BOOL Shw_bAtWhiteSpace(const char* psz)
{
	BOOL b = s_patWhiteSpace1.bMatchAt(&psz);
	
	return b;
}


// White space w/o newline...
void Shw_MatchOptionalWhiteSpaceNoNLAt(const char** ppsz)
{
	(void) s_patWhiteSpaceNoNL0.bMatchAt(ppsz);
}



BOOL Shw_bMatchWhiteSpaceNoNLAt(const char** ppsz)
{
	BOOL b = s_patWhiteSpaceNoNL1.bMatchAt(ppsz);
	
	return b;
}

BOOL Shw_bAtWhiteSpaceNoNL(const char* psz)
{
	BOOL b = s_patWhiteSpaceNoNL1.bMatchAt(&psz);
	
	return b;
}


BOOL Shw_bAtNewLine(const char* psz)
{
	BOOL b = s_patNewLine.bMatchAt(&psz);
	
	return b;
}

BOOL Shw_bMatchNewLineAt(const char** ppsz)
{
	BOOL b = s_patNewLine.bMatchAt(ppsz);
	
	return b;
}


BOOL Shw_bMatchNameAt(const char** ppsz, Str8& sName)
{
	const char* psz = *ppsz;
	Shw_MatchOptionalWhiteSpaceAt(&psz);
	if ( !s_patName.bMatchAt(&psz, sName) )
		return FALSE;

	*ppsz = psz;	
	return TRUE;
}




BOOL Shw_bValidName(const char* pszName, const char* pszInvalidChars,
		CNote** ppnot)
{
	const char* psz = pszName;
	return Shw_bMatchNameAt(&psz, pszInvalidChars, "", ppnot);
}


BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, CNote** ppnot)
{
	Length lenName;
	return Shw_bMatchNameAt(ppszName, pszInvalidChars, pszValidDelimiters,
		&lenName, ppnot);
}


BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, Str8& sName, CNote** ppnot)
{
	const char* pszName = *ppszName;
	Length lenName;
	if ( !Shw_bMatchNameAt(ppszName, pszInvalidChars, pszValidDelimiters, &lenName, ppnot) )
		return FALSE;
		
	// Note; in general, the matched substring is not null terminated.
	char* pch = sName.GetBuffer(lenName); // 1.4qzfv GetBuffer OK because written immediately
	strncpy_s(pch, lenName, pszName, _TRUNCATE);
	sName.ReleaseBuffer(lenName);
	
	return TRUE;
}


BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, Length* plenName, CNote** ppnot)
{
	return Shw_bMatchNameAt(ppszName, Shw_pszWhiteSpace, pszInvalidChars,
		pszValidDelimiters, plenName, ppnot);
}


BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszWhiteSpace,
		const char* pszInvalidChars, const char* pszValidDelimiters,
		Length* plenName, CNote** ppnot)
{
	const char* psz = *ppszName;
	// The candidate name is the initial substring consisting entirely of
	// characters _not_ in the invalid string, nor white space.
	for ( ; *psz != '\0'; psz += 1)
		if ( bMemberOfCharSet(*psz, pszWhiteSpace) )
		    break;
		else if ( bMemberOfCharSet(*psz, pszInvalidChars) )
			break;

	// The candidate name is a match if it is the entire string, or
	// if the character following it is considered a valid delimiter.
	// 1995-05-03 MRP: Should WHITE SPACE be a valid delimiter by default???
	Length lenName = psz - *ppszName;
	BOOL bValidDelimiter = ((*psz == '\0') ||
			bMemberOfCharSet(*psz, pszValidDelimiters) );
	if ( !bValidDelimiter )
		{
		Str8 sChar = *psz;
		*ppnot = new CNote(_("Unexpected character:"), sChar, psz, 1); // 1.5.0ft 
		return FALSE;
		}
	else if ( lenName == 0 )
		{
		*ppnot = new CNote(_("Expecting a name"), "", *ppszName, 0);
		return FALSE;
		}

	*ppszName = psz;	
	*plenName = lenName;
	return TRUE;
}

 
// **************************************************************************



#ifdef _DEBUG
#ifdef OMIT
void CPatEl::AssertMatchAt(BOOL bShouldMatch, const char* pszMatch, const char* pszRest) const
{
	ASSERT(pszMatch != NULL);
	ASSERT(pszRest != NULL);
	
	size_t lenMatch = strlen(pszMatch);
	Str8 s(pszMatch);
	s += pszRest;
	const char* pszMatchHere = s;
	const char* psz = pszMatchHere;
	
	BOOL bMatched = bMatchAt(&psz);

	ASSERT(bMatched == bShouldMatch);	                                                       	
	if (bMatched)
		ASSERT(psz == (pszMatchHere + lenMatch));
	else
		ASSERT(psz == pszMatchHere);  // should not have been changed
}
#endif  // OMIT
#endif  // _DEBUG


// **************************************************************************

CPatElString::CPatElString(const char* pszString)
{
	ASSERT(pszString != NULL);
	ASSERT(*pszString != '\0');
	m_sString = pszString;
	m_lenString = m_sString.GetLength();
}


BOOL CPatElString::bMatchAt(const char** ppsz) const
{
	BOOL bMatched = (CPatChar::s_bMatchCase ?
						(strncmp(*ppsz, m_sString, m_lenString) == 0) :
						(_strnicmp(*ppsz, m_sString, m_lenString) == 0)
					);
	if (! bMatched) { return FALSE; }
	
	*ppsz += m_lenString;
	return TRUE;
}


#ifdef _DEBUG
void CPatElString::AssertValid() const
{
	ASSERT(! m_sString.IsEmpty());
	ASSERT(m_lenString == m_sString.GetLength());
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

CPatElSet::CPatElSet(const char* pszSet)
{
	ASSERT(pszSet != NULL);
	// ASSERT(*pszSet != '\0');
	m_sSet = pszSet;
}


BOOL CPatElSet::bMatchAt(const char** ppsz) const
{                   
	BOOL bMatched = ((**ppsz != '\0') && (m_sSet.Find(**ppsz) != -1));
	if (! bMatched) { return FALSE; }
	
	*ppsz += 1;
	return TRUE;
}


#ifdef _DEBUG
void CPatElSet::AssertValid() const
{
	// ASSERT(! m_sSet.IsEmpty());
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

CPatElNotSet::CPatElNotSet(const char* pszSet)
{
	ASSERT(pszSet != NULL);
	// ASSERT(*pszSet != '\0');
	m_sSet = pszSet;
}


BOOL CPatElNotSet::bMatchAt(const char** ppsz) const
{                   
	BOOL bMatched = ((**ppsz != '\0') && (m_sSet.Find(**ppsz) == -1));
	if (! bMatched) { return FALSE; }
	
	*ppsz += 1;
	return TRUE;
}


#ifdef _DEBUG
void CPatElNotSet::AssertValid() const
{
	// ASSERT(! m_sSet.IsEmpty());
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

CPatElRep::CPatElRep(CPatElChar* ppat, size_t nMatchMin)
{
	ASSERT(ppat != NULL);
	m_ppat = ppat;
	m_nMatchMin = nMatchMin;
}


CPatElRep::~CPatElRep()
{
	delete m_ppat;
}


BOOL CPatElRep::bMatchAt(const char** ppsz) const
{   
	size_t nMatch = 0;
	const char* psz = *ppsz;
	while (m_ppat->bMatchAt(&psz))
		nMatch++;
    
  	BOOL bMatched = (m_nMatchMin <= nMatch);
  	if (! bMatched) { return FALSE; }
  	
	*ppsz = psz;  // after last successful match		
	return TRUE;
}


#ifdef _DEBUG
void CPatElRep::AssertValid() const
{
	ASSERT(m_ppat != NULL);
	m_ppat->AssertValid();
}
#endif  // _DEBUG									


// --------------------------------------------------------------------------

CPatElOr::CPatElOr(CPatElChar* ppatA, CPatElChar* ppatB)
{
	ASSERT(ppatA != NULL);
	ASSERT(ppatB != NULL);
	m_ppatA = ppatA;
	m_ppatB = ppatB;
}


CPatElOr::~CPatElOr()
{
	delete m_ppatA;
	delete m_ppatB;
}


BOOL CPatElOr::bMatchAt(const char** ppsz) const
{   
	BOOL bMatched = (m_ppatA->bMatchAt(ppsz) || m_ppatB->bMatchAt(ppsz));

	return bMatched;
}


#ifdef _DEBUG
void CPatElOr::AssertValid() const
{
	ASSERT(m_ppatA != NULL);
	m_ppatA->AssertValid();
	ASSERT(m_ppatB != NULL);
	m_ppatB->AssertValid();
}
#endif  // _DEBUG									

// **************************************************************************
