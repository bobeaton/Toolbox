// patmch.cpp Implementation of Shoebox multi-char pattern classes (1995-02-27)


#include "stdafx.h"
#include "toolbox.h"
#include "patmch.h"
#include "patch.h"
#include "loc.h"  // CMStringLoc
#include "not.h"  // CNote
#include "obstream.h"  // Object_istream, Object_ostream
#include "shwnotes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


// **************************************************************************

BOOL bMemberOfCharSet(char ch, const char* pszCharSet);

CPatMCharMatch::CPatMCharMatch(MatchSetting matset, BOOL bEmbeddedMarkup)
{
    m_matset = matset;
    m_bEmbeddedMarkup = bEmbeddedMarkup;
}


#ifdef prjWritefstream // 1.6.4aa 
void CPatMCharMatch::WriteProperties(Object_ofstream& obs) const
#else
void CPatMCharMatch::WriteProperties(Object_ostream& obs) const
#endif
{
    // 1995-07-25 MRP: Review all character, field, and record match settings
    // and improve the encapsulation!!!
    const char* pszProperty;
    if ( m_matset == CMCharOrder::matsetSamePrimaryGroup )
        pszProperty = "p";
    else if ( m_matset == CMCharOrder::matsetExactDisregardCase )
        pszProperty = "c";
    else if ( m_matset == CMCharOrder::matsetExact )
        pszProperty = "e";
    else if ( m_matset == CMCharOrder::matsetEvenIgnore )
        pszProperty = "i";
    else
        ASSERT( FALSE );
    obs.WriteString("match_char", pszProperty);
}


// **************************************************************************

BOOL CPatMChar::bMatch(const CMString* pmks, CPatMCharMatch& patmat) const
{
    CMStringLoc loc(pmks, patmat.bEmbeddedMarkup());
    return bMatch(loc, patmat);
}


BOOL CPatMChar::bMatch(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    for ( ; loc.bNotAtEnd(); loc.Next())
        if( bMatchAt(loc, patmat) )
            return TRUE;
    
    // Match possible zero-length pattern (e.g. boundary) at end of text
    return bMatchAt(loc, patmat);
}


BOOL CPatMChar::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{   
    CPatElMChar* ppalA = ppalFirst();
//  ASSERT( ppalA != NULL );  // 1995-07-10 MRP: permit empty string
    if ( ppalA == NULL )
        return TRUE;
        
    return bMatchAt(ppalA, loc, patmat);
}


int CPatMChar::iMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{   
    CMStringLoc locEnd = loc;
    if ( !bMatchAt(locEnd, patmat) )
        return 0;
    else
        return locEnd.iPos() - loc.iPos(); // return length of match
}

BOOL CPatMChar::bMatchAt(CPatElMChar* ppalA, CMStringLoc& loc,
                                CPatMCharMatch& patmat) const
{
    if ( ppalA == NULL )  // end of pattern
        return TRUE;

    CMStringLoc locAfterA = loc;
    if ( !ppalA->bMatchAt(locAfterA, patmat) )  // this pattern element
        return FALSE;
    
    CMStringLoc locAfterMatch = locAfterA;          
    CPatElMChar* ppalBC = ppalNext(ppalA);
    if ( !bMatchAt(ppalBC, locAfterMatch, patmat) )  // rest of pattern
        {
        BOOL bMatched = FALSE;
        // Can this match be shortened so that the rest does match?
            // This can be done only for repeated pattern elements
            // such as spaces and skip.
        while ( ppalA->bShortenMatch(loc, locAfterA) )
            {
            // This is a match if the rest matches and the location
                // to which this pattern element match had been shortened
                // does indeed correspond to a valid multi-character.
            locAfterMatch.Relocate(&locAfterA);
            bMatched = (bMatchAt(ppalBC, locAfterMatch, patmat) &&
                            locAfterA.bAtValidLoc());
            if ( bMatched )
                break;
            }
        if ( !bMatched )  // shortening did not produce a match
            return FALSE;
        }
    
    loc.Relocate(&locAfterMatch);
    return TRUE;
}


BOOL CPatMChar::s_bConstruct(const char** ppsz,
        const char* pszInvalidChars, const char* pszValidDelimiters, 
        CVarSet* pvarset, CPatMChar** pppat, CNote** ppnot)
{
    CPatMChar* ppat = new CPatMChar;
    const char* psz = *ppsz;
    while ( *psz != '\0' )  // Note: element constructor increments pointer
        if ( bMemberOfCharSet(*psz, pszInvalidChars) )
            break;
        else
            {
            CPatElMChar* ppal = NULL;
            // how to handle invalids/delimiters in pattern elements             
            if ( !CPatElMChar::s_bConstruct(&psz, pszInvalidChars,
                    pvarset, &ppal, ppnot) )
                {
                delete ppat;
                return FALSE;
                }
                
            ppat->Add(ppal);
            }
        
    BOOL bValidDelimiter = ((*psz == '\0') ||
            bMemberOfCharSet(*psz, pszValidDelimiters) );
    if ( !bValidDelimiter )
        {
        delete ppat;
        Str8 sChar = *psz;
        *ppnot = new CNote(_("Not expecting the character:"), sChar, psz, 1); // 1.5.0ft 
        (*ppnot)->Add(_("in a text pattern string"));
        return FALSE;
        }

    // 1996-07-11 MRP: We now allow an empty pattern, which matches any text;
    // or only the empty string if the "Match whole field" option is checked.
    *ppsz = psz;  // just after the text pattern    
    *pppat = ppat;
    return TRUE;
}

void CPatMChar::Add(CPatElMChar* ppal)
{
    ASSERT( ppal );
    if ( !ppal->bSkip() )
        m_bSkip = FALSE;
    CDblList::Add(ppal);
}

void CPatMChar::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    const CPatElMChar* ppal = ppalFirst();
    for ( ; ppal; ppal = ppalNext(ppal) )
        ppal->Draw(plbo, cDC, rcItem, px, xEnd, plng);
}

#ifdef _DEBUG
void CPatMChar::AssertValid() const
{
    ASSERT(! bIsEmpty());
    CPatElMChar* ppal;
    for (ppal = ppalFirst(); ppal != NULL; ppal = ppalNext(ppal))
        ppal->AssertValid();
}
#endif  // _DEBUG

 
// **************************************************************************

// Default implementation-- only repeated patterns can be shortened.
BOOL CPatElMChar::bShortenMatch(const CMStringLoc& matMatchedHere, CMStringLoc& loc) const
{
    return FALSE;
}                           

BOOL CPatElMChar::bSkip() const
{
    return FALSE;
}


// Elements of the textual form of text pattern element objects.
static char s_pszAny[] = {Shw_chLMeta, '?', Shw_chRMeta, '\0'};
static char s_pszWordBound[] = {Shw_chLMeta, '#', Shw_chRMeta, '\0'};
static char s_pszMorphBound[] = {Shw_chLMeta, '-', Shw_chRMeta, '\0'};
static char s_pszSkip[] = {Shw_chLMeta, '.', '.', '.', Shw_chRMeta, '\0'};
static char s_pszSpaces[] = {' ', '\n', '\0'};
static char s_pszMeta[] = {Shw_chLMeta, Shw_chRMeta, ' ', '\n', '\0' };

static const CPatChar s_patLMeta(new CPatElString(Shw_pszLMeta));
static CPatChar s_patAny(new CPatElString(s_pszAny));
static CPatChar s_patWordBound(new CPatElString(s_pszWordBound));
static CPatChar s_patMorphBound(new CPatElString(s_pszMorphBound));
static CPatChar s_patSkip(new CPatElString(s_pszSkip));
static const CPatChar s_patMeta(new CPatElRep(new CPatElNotSet(s_pszMeta), 1));
static const CPatChar s_patRMeta(new CPatElString(Shw_pszRMeta));

//static CPatChar s_patSpaces(new CPatElString(s_pszSpaces));
static CPatChar s_patSpaces(new CPatElRep(new CPatElSet(s_pszSpaces), 1));
static CPatChar s_patEscape(new CPatElString("\\"));


BOOL CPatElMChar::s_bConstruct(const char** ppsz, const char* pszDelimiters,
                                CVarSet* pvarset,
                                CPatElMChar** pppal, CNote** ppnot)
{
    CPatElMChar* ppal = NULL;   
    const char* psz = *ppsz;    
    if (s_patLMeta.bMatchAt(&psz))  // look ahead for a left meta
        {
        ASSERT( pvarset != NULL );
        psz = *ppsz;  // match including the left meta
        if ( s_patAny.bMatchAt(&psz) )
            ppal = new CPatElAny;
        else if ( s_patWordBound.bMatchAt(&psz) )
            {
            ASSERT( pvarset );
            CVar* pvarPunct = pvarset->pvarPunct();  // 1998-01-31 MRP
            ppal = new CPatElBound(pvarPunct, FALSE);
            }
        else if ( s_patMorphBound.bMatchAt(&psz) )
            {
            ASSERT( pvarset );
            CVar* pvarPunct = pvarset->pvarPunct();  // 1998-01-31 MRP
            ppal = new CPatElBound(pvarPunct, TRUE);
            }
        else if ( s_patSkip.bMatchAt(&psz) )
            ppal = new CPatElSkip;
        else  // variable
            {
            BOOL bMatched = s_patLMeta.bMatchAt(&psz);
            ASSERT(bMatched);

            const char* pszVarName = psz;           
            Str8 sVarName;
            // Base on name with invalids/delimiters
            if ( !s_patMeta.bMatchAt(&psz, sVarName) )
                {
                *ppnot = new CNote(_("Expecting a variable name after the left bracket"), "", psz, 0); // 1.5.0fj 
                (*ppnot)->Add(_("in a text pattern string"));
                return FALSE;
                }
                
            if ( !s_patRMeta.bMatchAt(&psz) )
                {
                *ppnot = new CNote(_("Expecting right bracket after a variable name"), "", psz, 0); // 1.5.0fj 
                (*ppnot)->Add(_("in a text pattern string"));
                return FALSE;
                }
            
            CVar* pvar = pvarset->pvarSearch(sVarName);
            if ( pvar == NULL )
                {
                *ppnot = new CNote(_("Variable name not defined:"), sVarName, // 1.5.0fj  // 1.5.0ft 
                    pszVarName, strlen(sVarName));
                (*ppnot)->Add(_("in a text pattern string"));
                return FALSE;
                }
                
            ppal = new CPatElVar(pvar);
            }
        }
    else if ( s_patSpaces.bMatchAt(&psz) )
        ppal = new CPatElSpaces;
    else  // literal chars
        {
        Str8 sMChars;
        while (     (*psz != '\0') &&
                    !bMemberOfCharSet(*psz, pszDelimiters) &&
                    !bMemberOfCharSet(*psz, s_pszMeta)
                )
            {
            if ( s_patEscape.bMatchAt(&psz) )
                if (*psz == '\0')
                    {
                    *ppnot = new CNote(_("Expecting a character after the backslash"), "", psz, 0); // 1.5.0fj 
                    (*ppnot)->Add(_("in a text pattern string"));
                    return FALSE;
                    }
            sMChars += *psz++;
            }
        if ( *psz == Shw_chRMeta )
            {
            *ppnot = new CNote(_("This right bracket doesn't enclose a variable or metacharacter"), "", psz, 0); // 1.5.0fj 
            (*ppnot)->Add(_("in a text pattern string"));
            return FALSE;
            }
        ASSERT( !sMChars.IsEmpty() );

        ppal = new CPatElMChars(sMChars);
        }
    
    *ppsz = psz;
    *pppal = ppal;
    return TRUE;
}

// **************************************************************************

CPatElMChars::CPatElMChars(const char* pszMChars)
{
    ASSERT( pszMChars != NULL );
    ASSERT( *pszMChars != '\0' );
    m_sMChars = pszMChars;
}


BOOL CPatElMChars::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    CMStringLoc locText(loc);
    const CLangEnc* plngText = locText.plng();
    MatchSetting matset = patmat.matset();
    CMStringLoc locPat(locText, &m_sMChars);
    const CMChar* pmchText = NULL;
    const CMChar* pmchPat = NULL;
    
    while ( locPat.bNotAtEnd() && locText.bNotAtEnd() )
        {
        // A substring marker breaks a literal text pattern match
        if ( locText.bAtMarkup() )
            return FALSE;

        // Next multichar of text and pattern to included at this match setting
        Length lenMCharText;        
        if ( !locText.bMChar_NextIncluded(&pmchText, &lenMCharText, matset) )
            return FALSE;           
        Length lenMCharPat;
        if ( !locPat.bMChar_NextIncluded(&pmchPat, &lenMCharPat, matset) )
            return FALSE;
            
        // Compare multichars according to their definitions in the
        // default sort order of the current language encoding
        // at this text location.
//      BOOL bMCharTextDefined = plngText->bDefined(pmchText);
//      BOOL bMCharPatDefined = plngText->bDefined(pmchPat);
        BOOL bMCharTextDefined = !plngText->pmchordDefault(pmchText)->bUndefined();
        BOOL bMCharPatDefined = !plngText->pmchordDefault(pmchPat)->bUndefined();
        if ( bMCharTextDefined && bMCharPatDefined )
            {
            if ( !plngText->bEqualMChars(pmchText, pmchPat, matset) )
                return FALSE;
            }
        else if ( bMCharTextDefined || bMCharPatDefined )
            return FALSE;
        else  // both multichars are undefined in the sort order
            {
            // match literal designators of undefined multichars
            if ( lenMCharText != lenMCharPat )
                return FALSE;
            const char* pszMCharText = locText.psz() - lenMCharText;
            const char* pszMCharPat = locPat.psz() - lenMCharText;
            if ( strncmp(pszMCharText, pszMCharPat, lenMCharText) != 0 )
                return FALSE;
            }
        }  // while
            
    if ( locPat.bNotAtEnd() )  // text ended before entire pattern matched
        return FALSE;

    loc.Relocate(&locText);  // update location after a successful match
    return TRUE;
}

void CPatElMChars::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, m_sMChars, plng);
}

#ifdef _DEBUG
void CPatElMChars::AssertValid() const
{
    ASSERT( !m_sMChars.IsEmpty() );
}
#endif  // _DEBUG


// ==========================================================================

BOOL CPatElAny::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    if ( loc.bAtEnd() || loc.bAtMarkup() )
        return FALSE;

    // 1995-04-07 MRP: Not yet considering meaning of match setting
    const CMChar* pmch = loc.pmch_Next();
    
    return TRUE;
}


void CPatElAny::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszLMeta);
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, "?");
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszRMeta);
}

// --------------------------------------------------------------------------

CPatElVar::CPatElVar(CVar* pvar) :
    m_pvar(pvar)
{
    ASSERT( m_pvar );
    m_pvar->IncrementNumRefs();
}


CPatElVar::~CPatElVar()
{
    ASSERT( m_pvar );
    m_pvar->DecrementNumRefs();
}


BOOL CPatElVar::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    if ( loc.bAtEnd() )
        return FALSE;
    else if ( loc.bAtMarkup() )
        return FALSE;

    CMStringLoc locVar = loc;
    const CMChar* pmch = NULL;
    Length lenMChar;
    MatchSetting matset = patmat.matset();
    if ( !locVar.bMChar_NextIncluded(&pmch, &lenMChar, matset) )
        return FALSE;

    ASSERT( pmch );
    if ( !m_pvar->bIncludes(pmch) )  // 1998-01-31 MRP
        return FALSE;
        
    loc.Relocate(&locVar);
    return TRUE;
}


void CPatElVar::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszLMeta);
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, m_pvar->sName());
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszRMeta);
}

// ==========================================================================

CPatElBound::CPatElBound(CVar* pvarPunct, BOOL bMorph) :
        m_pvarPunct(pvarPunct),
        m_bMorph(bMorph)
{
    ASSERT( m_pvarPunct );  // 1998-01-31 MRP
}


BOOL CPatElBound::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    // Beginning or end of string are boundaries
    if ( loc.bAtBeginning() || loc.bAtEnd() )
        return TRUE;
          
    CMStringLoc locBound = loc; 
    if ( locBound.bMarkup_Next() )
        ;
    else
        {
        const CMChar* pmch = NULL;
        Length lenMChar;
        MatchSetting matset = patmat.matset();      
//      if ( !loc.bMChar_NextIncluded(&pmch, &lenMChar, matset) )
        if ( !locBound.bMChar_NextIncluded(&pmch, &lenMChar, matset) )
            return FALSE;

        ASSERT( pmch );
        BOOL bMatched = m_pvarPunct->bIncludes(pmch);  // 1998-01-31 MRP
        // 1996-08-09 MRP: Eliminate the internal space variable
        const Str8& sMChar = pmch->sMChar();
        if ( !bMatched )
            bMatched = (sMChar.GetLength() == 1 && Shw_bAtWhiteSpace(sMChar));
        // 1996-07-27 MRP: Hard-code the morpheme break character
        // since there's no easy way right now to get it
        // from the interlinear properties.
        if ( !bMatched && m_bMorph )
            bMatched = bEqual(pmch->sMChar(), "-");
        if ( !bMatched )
            return FALSE;
        }
        
    loc.Relocate(&locBound);
    return TRUE;
}


void CPatElBound::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszLMeta);
    if ( m_bMorph )
        plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, "-");
    else
        plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, "#");
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszRMeta);
}

// ==========================================================================

BOOL CPatElSkip::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    loc.RelocateToEnd();    
    return TRUE;
}


BOOL CPatElSkip::bShortenMatch(const CMStringLoc& matMatchedHere, CMStringLoc& loc) const
{
    BOOL bCanShortenMatch = (matMatchedHere.psz() < loc.psz());
    if (! bCanShortenMatch)
        return FALSE;

    loc.Char_Prev();        
    return TRUE;
}               

BOOL CPatElSkip::bSkip() const
{
    return TRUE;
}


void CPatElSkip::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszLMeta);
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, "...");
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, Shw_pszRMeta);
}

// --------------------------------------------------------------------------

BOOL CPatElSpaces::bMatchAt(CMStringLoc& loc, CPatMCharMatch& patmat) const
{
    CMStringLoc locSpaces = loc;
    BOOL bMatched = FALSE;
    while ( locSpaces.bNotAtEnd() )
        {
        CMStringLoc locNext = locSpaces;
        const CMChar* pmch = locNext.pmch_Next();
//      if ( !pmch->bMemberOfVar(Shw_varrefWhiteSpace) )
//          break;
        // 1996-08-09 MRP: Eliminate the internal space variable
        const Str8& sMChar = pmch->sMChar();
        if ( sMChar.GetLength() != 1 || !Shw_bAtWhiteSpace(sMChar) )
            break;
            
        locSpaces.Relocate(&locNext);
        bMatched = TRUE;
        }

    if ( !bMatched)
        return FALSE;
    
    loc.Relocate(&locSpaces);
    return TRUE;
}


BOOL CPatElSpaces::bShortenMatch(const CMStringLoc& matMatchedHere, CMStringLoc& loc) const
{
    BOOL bCanShortenMatch = (matMatchedHere.psz() + 1 < loc.psz());
    if (! bCanShortenMatch)
        return FALSE;

    loc.Char_Prev();        
    return TRUE;
}               


void CPatElSpaces::Draw(CDblListListBox* plbo, CDC& cDC, const RECT& rcItem,
        int* px, int xEnd, CLangEnc* plng) const  // 1998-02-13 MRP
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, px, xEnd, " ");
}
