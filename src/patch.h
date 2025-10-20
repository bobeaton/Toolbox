// patch.h Interface for Shoebox single char pattern classes (1995-02-27)


// Change history:
// 1995-02-28 0.07a MRP: actually distinction is between single and multi-char
// 1995-02-27 0.07  MRP: separate internal program patterns from end-user ones


#ifndef PATCH_H
#define PATCH_H

#include "cdbllist.h"
#include "not.h"

class CPatElChar;

// **************************************************************************

// A text pattern consists of a list of pattern elements.

class CPatChar : private CDblList
{
public:
	CPatChar() {}
	CPatChar(CPatElChar* ppatel);
	CPatChar(CPatElChar* ppatel1, CPatElChar* ppatel2);
	virtual ~CPatChar() {}
	
	// Return whether the text pattern matches a string at *ppsz.
		// If it does, update the pointer to the first position
		// in the string after the matched pattern. If not, leave it alone.
	virtual BOOL bMatchAt(const char** ppsz) const;
	// Also extract a copy of the substring matched, if successful.
	BOOL bMatchAt(const char** ppsz, Str8& sMatched) const;
	// Return whether the text pattern matches the string anywhere at all.
	BOOL bMatch(const char* psz) const;

#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG
									
protected:
	// Match the pattern element at the specified position in the list.
	BOOL bMatchAt(CPatElChar* ppatelA, const char** ppsz) const;	
	
	CPatElChar* ppatelFirst() const // First element
		{ return (CPatElChar*) CDblList::pelFirst(); }
	CPatElChar* ppatelNext( CPatElChar* pelCur ) const // element after current
		{ return (CPatElChar*) CDblList::pelNext( (CDblListEl*) pelCur ); }
		
public:
	// Shared resources for constructing objects from textual representations.
	// E.g. filters, variables.
	static BOOL s_bMatchCase;
};  // class CPatChar


extern const char* Shw_pszWhiteSpace;  // white space: space, tab, newline
extern const char* Shw_pszNL;			// NewLine
extern const char* Shw_pszSpaceNoNL;   // white space: space, tab, NOT newline.
extern const char* Shw_pszOtherWhiteSpace;  // above, other than space itself
BOOL Shw_bAtWhiteSpace(const char* psz);
// Match zero or more occurrences of white space	
void Shw_MatchOptionalWhiteSpaceAt(const char** ppsz);
// Match one or more occurrences of white space	
BOOL Shw_bMatchWhiteSpaceAt(const char** ppsz);
void Shw_MatchOptionalWhiteSpaceNoNLAt(const char** ppsz);
BOOL Shw_bMatchWhiteSpaceNoNLAt(const char** ppsz);
BOOL Shw_bMatchOptionalWhiteSpaceAt(const char** ppsz);
BOOL Shw_bAtWhiteSpaceNoNL(const char* psz);

BOOL Shw_bAtNewLine(const char* psz);
// Match one or more occurrences of newline	
BOOL Shw_bMatchNewLineAt(const char** ppsz);

BOOL Shw_bMatchNameAt(const char** ppsz, Str8& sName);
BOOL Shw_bMatchNameAt(const char** ppsz);


// The name of an entity in Shw is a non-empty string which does not contain
// white space, nor any characters designated as invalid. A name can appear:
// 1. individually in an interface control, where it is _validated_
//    after the entity has been edited interactively within Shoebox; or
// 2. in context within the textual description of an enclosing entity,
//    where it is _matched_ as a substring when the text is read or edited.

// Which characters are _invalid_ in each kind of name depend on the contexts
// where the name appears in Shoebox.
// E.g. Since vertical bar | is the metacharacter which separates the marker
// from the text pattern in a Marker-String filter conditon, it must be
// considered invalid in a marker [name].

// The _valid delimiters_ are a subset of invalid and white space characters,
// which are expected to follow the name matched in a particular context.
// E.g. The valid delimiter for the marker in a Marker-String condition is
// vertical bar |, but in a Field-Marker condition it is right bracket ].
// No valid delimiters means that the entire string is expected to be a name.
// E.g. For AssertValid(); see the implementation of Shw_bValidName().

BOOL Shw_bValidName(const char* pszName, const char* pszInvalidChars,
		CNote** ppnot);
BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, CNote** ppnot);
BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, Str8& sName, CNote** ppnot);
BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszInvalidChars,
		const char* pszValidDelimiters, Length* plenName, CNote** ppnot);
BOOL Shw_bMatchNameAt(const char** ppszName, const char* pszWhiteSpace,
		const char* pszInvalidChars, const char* pszValidDelimiters,
		Length* plenName, CNote** ppnot);

// To add an entity name to a class, insert the following fragments:
/*
private:
	Str8 m_sName;
	
public:
	static const char* s_pszInvalidNameChars;
	static BOOL s_bValidName(const char** ppszName, Str8& sErr)
		{ return Shw_bValidName(ppszName, s_pszInvalidNameChars, sErr); }
	static BOOL s_bMatchNameAt(const char** ppszName,
			const char* pszValidDelimiters, Str8& sName, Str8& sErr)
		{ return Shw_bMatchNameAt(ppszName, s_pszInvalidNameChars,
			pszValidDelimiters, sName, sErr); }
	static BOOL s_bMatchNameAt(const char** ppszName,
			const char* pszValidDelimiters, Length* plenName, Str8& sErr)
		{ return Shw_bMatchNameAt(ppszName, s_pszInvalidNameChars,
			pszValidDelimiters, plenName, sErr); }
*/


// **************************************************************************

// Abstract Base Class from which a text pattern element class is derived
// for each particular kind.

class CPatElChar : public CDblListEl
{
public:
	virtual ~CPatElChar() {}
	
	// Return whether the text pattern element matches a string at *ppsz.
		// If it does, update the pointer to the first position
		// in the string after the matched pattern. If not, leave it alone.
	virtual BOOL bMatchAt(const char** ppsz) const = 0;
								
#ifdef _DEBUG
	virtual void AssertValid() const = 0;
#endif  // _DEBUG									
};  // class CPatElChar


// **************************************************************************
                                                                          
// Match a specified literal string.
class CPatElString: public CPatElChar
{
protected:
	Str8 m_sString;  // not empty
	int m_lenString;
	
public:
	CPatElString(const char* pszString);
	virtual ~CPatElString() {}
	
	virtual BOOL bMatchAt(const char** ppsz) const;
									
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG									
};  // class CPatElString


// --------------------------------------------------------------------------
                                                  
// Match any single character from a specified set
class CPatElSet: public CPatElChar
{
private:
	Str8 m_sSet;
	
public:
	CPatElSet(const char* pszSet);
	virtual ~CPatElSet() {}
	
	virtual BOOL bMatchAt(const char** ppsz) const;
									
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG									
};  // class CPatElSet

// --------------------------------------------------------------------------
                                                  
// Match any single character NOT from a specified set.
class CPatElNotSet: public CPatElChar
{
private:
	Str8 m_sSet;
	
public:
	CPatElNotSet(const char* pszSet);
	virtual ~CPatElNotSet() {}
	
	virtual BOOL bMatchAt(const char** ppsz) const;
									
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG									
};  // class CPatElNotSet


// --------------------------------------------------------------------------
                    
// Match at least nMatchMin repeated occurences of a single character pattern
class CPatElRep: public CPatElChar
{
protected:
	CPatElChar* m_ppat;
	size_t m_nMatchMin;
	
public:
	CPatElRep(CPatElChar* ppat, size_t nMatchMin = 0);
	virtual ~CPatElRep();
	
	virtual BOOL bMatchAt(const char** ppsz) const;
									
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG									
};  // class CPatElRep


// --------------------------------------------------------------------------
                    
// Match either the first, or otherwise the second pattern element.
class CPatElOr: public CPatElChar
{
protected:
	CPatElChar* m_ppatA;
	CPatElChar* m_ppatB;
	
public:
	CPatElOr(CPatElChar* ppatA, CPatElChar* ppatB);
	virtual ~CPatElOr();
	
	virtual BOOL bMatchAt(const char** ppsz) const;
									
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif  // _DEBUG									
};  // class CPatElOr

// **************************************************************************

#endif  // PATCH_H