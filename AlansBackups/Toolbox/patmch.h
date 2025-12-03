// patmch.h Interface for Shoebox multi-char pattern classes (1995-02-27)


#ifndef PATMCH_H
#define PATMCH_H

#include "cdbllist.h"
#include "lng.h"

class CNote;  // not.h
class CMString;  // mkr.h
class CMStringLoc;  // loc.h
class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h


// **************************************************************************

class CPatMCharMatch  // Hungarian: patmat, mat
{
private:
    MatchSetting m_matset;
    BOOL m_bEmbeddedMarkup;
    
public:
    CPatMCharMatch(MatchSetting matset, BOOL bEmbeddedMarkup = FALSE);
    virtual ~CPatMCharMatch() {}

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    
    MatchSetting matset() const { return m_matset; }
    void SetMatchSetting(MatchSetting matset) { m_matset = matset; }
    
    BOOL bEmbeddedMarkup() const { return m_bEmbeddedMarkup; }
};  // class CPatMCharMatch


// **************************************************************************

// Abstract Base Class from which a class is derived for each kind
// of multi-character text pattern.

class CPatElMChar: public CDblListEl  // Hungarian pal
{
public:
    virtual ~CPatElMChar() {}
                
    // Construct a pattern element object from its textual representation.
        // EXAMPLE:
        // char pszPat[] = "a<<...>>e in me";
        // char* psz = pszPat;
        // CPatElMChar* ppal = NULL;
        // BOOL bOK = CPatEl::bConstruct(&psz, &ppal, sErr);
        // ASSERT(psz = pszPat + 1);
        // NOTE NOTE NOTE: This object is constructed by the function,
        // but the *caller* is responsible to delete it!
    static BOOL s_bConstruct(const char** ppsz, const char* pszPatDelimChars,
                                CVarSet* pvarset,
                                CPatElMChar** pppal, CNote** ppnot);
//  static BOOL bConstruct(const char** ppsz, const char* pszPatDelimChars,
//                              CVarSet* pvarset,
//                              CPatElMChar** pppal, Str8& sErr);
    
    // Return whether the text pattern element matches a string at *ppsz.
        // If it does, update the pointer to the first position
        // in the string after the matched pattern. If not, leave it alone.
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const = 0;
    // Shorten by one a successful match if a repeated pattern (e.g. <<...>>).
    virtual BOOL bShortenMatch(const CMStringLoc& locMatchedHere, CMStringLoc& loc) const;
    virtual BOOL bSkip() const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const = 0;
        // 1998-02-13 MRP: Change from pfntText to plng to allow
        // context-sensitive rendering and right-to-left scripts.
                                
#ifdef _DEBUG
    virtual void AssertValid() const = 0;
#endif  // _DEBUG                                   
};  // class CPatElMChar


// **************************************************************************

// A multi-chararacter text pattern consists of a list of pattern elements.

class CPatMChar : private CDblList  // Hungarian: pat
{
private:
    BOOL m_bSkip;  // The pattern consisted only of skip element(s).
    
public:
    CPatMChar() : m_bSkip(TRUE) {}
    virtual ~CPatMChar() {}

    // Construct a pattern object from its textual representation.
        // EXAMPLE:
        // char pszPattern[] = "a<<...>>e in me";
        // const char* psz = pszPattern;
        // CPat* ppat = NULL;
        // BOOL bOK = CPatMChar::bConstruct(&psz, &ppat, sErr);
        // ASSERT(*psz == '\0');
    // NOTE NOTE NOTE: This object is constructed by the function,
        // but the *caller* is responsible to delete it!
    static BOOL s_bConstruct(const char** ppsz, 
            const char* pszInvalidChars, const char* pszValidDelimiters,
            CVarSet* pvarset, CPatMChar** pppat, CNote** ppnot);
//  static BOOL bConstruct(const char** ppsz, CVarSet* pvarset,
//                              CPatMChar** pppat, Str8& sErr);

    // Return whether the text pattern matches the string anywhere at all.
    // Especially for filter pattern match.
    BOOL bMatch(const CMString* pmks, CPatMCharMatch& patmat) const;
    BOOL bMatch(CMStringLoc& loc, CPatMCharMatch& patmat) const;
    BOOL bSkip() const { return m_bSkip; }

    // Return the next location where the text pattern matches the string,
    // as well as the location after the match.
    // Especially for Edit Find and Replace.
    //BOOL bMatchNext(CMStringLoc& locNext, CMStringLoc& locAfter) const;
    
    // Return whether the text pattern matches a string at the location.
        // If it does, update it to the first location in the string
        // after the matched pattern. If not, leave it alone.
    BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;
//  // Also extract a copy of the substring matched, if successful.
//  BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat,
//                                          Str8& sMatched) const;
    // Return length of match at loc or 0 if no match
    int iMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;

    void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
    BOOL bEmpty() const { return CDblList::bIsEmpty(); }

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG
                                    
protected:
    // Match the pattern element at the specified position in the list.
    BOOL bMatchAt(CPatElMChar* ppalA, CMStringLoc& loc,
                                    CPatMCharMatch& patmat) const;  
    
    CPatElMChar* ppalFirst() const  // first element
        { return (CPatElMChar*) CDblList::pelFirst(); }
    CPatElMChar* ppalNext( const CPatElMChar* pelCur ) const  // element after current
        { return (CPatElMChar*) CDblList::pelNext( pelCur ); }
        
private:
    void Add(CPatElMChar* ppal);
};  // class CPatMChar


// **************************************************************************
                                                                          
// Match specified literal characters.
class CPatElMChars: public CPatElMChar
{
protected:
    Str8 m_sMChars;
    
public:
    CPatElMChars(const char* pszMChars);
    virtual ~CPatElMChars() {}
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG                                   
};  // class CPatElMChars


// ==========================================================================
// Pattern elements which match any one character from various sets.
                
// Match any single multi-character
class CPatElAny: public CPatElMChar
{
public:
    CPatElAny() {}
    virtual ~CPatElAny() {}
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG                                   
};  // class CPatElAny


// --------------------------------------------------------------------------

class CPatElVar: public CPatElMChar
{
private:
    CVar* m_pvar;  // reference to the variable

public:
    CPatElVar(CVar* pvar);
    virtual ~CPatElVar();
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG
};  // class CPatElVar


// ==========================================================================
// Pattern elements which match any one character from various sets
// which may be customized by the user.
// Used to implement <<#>> word boundary and <<->> morpheme boundary.

class CPatElBound: public CPatElMChar
{
private:
    CVar* m_pvarPunct;  // 1998-01-31 MRP
    BOOL m_bMorph;
    
public:
    CPatElBound(CVar* pvarPunct, BOOL bMorph);
    virtual ~CPatElBound() {}
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG
};  // class CPatElBound


// ==========================================================================
// Match repeatedly single characters from a fixed set.
// "Longest match" can be shortened if rest of pattern does not match.

// Match zero or more of any character.                                                                          
class CPatElSkip: public CPatElMChar
{
public:
    CPatElSkip() {}
    virtual ~CPatElSkip() {}
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;
    virtual BOOL bShortenMatch(const CMStringLoc& locMatchedHere, CMStringLoc& loc) const;
    virtual BOOL bSkip() const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG                                   
};  // class CPatElSkip


// --------------------------------------------------------------------------
                                                                          
// Match one or more spaces.                                                                          
class CPatElSpaces: public CPatElMChar
{
public:
    CPatElSpaces() {}
    virtual ~CPatElSpaces() {}
    
    virtual BOOL bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const;
    virtual BOOL bShortenMatch(const CMStringLoc& locMatchedHere, CMStringLoc& loc) const;

    virtual void Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
            int* px, int xEnd, CLangEnc* plng) const;  // 1998-02-13 MRP
                                    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG                                   
};  // class CPatElSpaces

// **************************************************************************

#endif  // PATMCH_H
