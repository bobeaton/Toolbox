// loc.cpp  Implementation of marked string locations  (1995-04-19)

// Change history:
// 1995-04-19 0.11  MRP: separate from rest of marker classes


#include "stdafx.h"
#include "toolbox.h"
#include "loc.h"
#ifdef _DEBUG
#include <strstrea.h>
#endif  // _DEBUG

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CMarkupElOpening::CMarkupElOpening(CMarker* pmkr)
{
	ASSERT( pmkr != NULL );
	m_pmkr = pmkr;
	m_pmelPrevUnclosedOpening = NULL;
}


CMarkupElOpening::CMarkupElOpening(CMarker* pmkr,
		CMarkupElOpening* pmelPrevUnclosedOpening)
{
	ASSERT( pmkr != NULL );
	ASSERT( pmelPrevUnclosedOpening != NULL );
	m_pmkr = pmkr;
	m_pmelPrevUnclosedOpening = pmelPrevUnclosedOpening;
}


CMarkupElClosing::CMarkupElClosing(CMarkupElOpening* pmelOpening)
{
	ASSERT( pmelOpening != NULL );
	m_pmelOpening = pmelOpening;
}


CMarkupElOpening* CMarkupElClosing::pmelCurUnclosedOpening() const
{
	CMarkupElOpening* pmel = m_pmelOpening->pmelPrevUnclosedOpening();
	ASSERT( pmel != NULL );
	
	return pmel;
}


// --------------------------------------------------------------------------

CMarkupList::CMarkupList(CMarker* pmkrMString)
{
	ASSERT( pmkrMString != NULL );
	Add(new CMarkupElOpening(pmkrMString));
}


CMarkupEl* CMarkupList::pmelNext_AddIfNew( CMarkupEl* pmelCur,
		CMarker* pmkr, BOOL bOpening )
{
	ASSERT( pmelCur != NULL );
	ASSERT( !bOpening || (pmkr != NULL) );
	CMarkupEl* pmel = pmelNext(pmelCur);
	if ( pmel != NULL )
		{
		// The next element had already been added
		ASSERT( (pmkr == NULL) || (pmel->pmkrEl() == pmkr) );
		ASSERT( pmel->bOpening() == bOpening );
		}
	else
		{
		// Actually construct new element and add it to list
		CMarkupElOpening* pmelCurUnclosedOpening =
			pmelCur->pmelCurUnclosedOpening();
		ASSERT( pmelCurUnclosedOpening != NULL );
		if ( bOpening )
			pmel = new CMarkupElOpening(pmkr, pmelCurUnclosedOpening);
		else
			pmel = new CMarkupElClosing(pmelCurUnclosedOpening);
		CDblList::Add(pmel);
		}
		
	return pmel;
}


// --------------------------------------------------------------------------

CMStringLoc::CMStringLoc(const CMString* pmks, BOOL bEmbeddedMarkup, int iPos)
{
	ASSERT( pmks != NULL );
	m_psz = *pmks;
#ifdef _DEBUG
	m_pszBeginning = m_psz;
#endif  // _DEBUG
	m_psz += iPos;
	m_ps = pmks;
	m_pmkr = (CMarker*)pmks->pmkr();  // 1995-04-15 MRP: analyze const-ness
	ASSERT( m_pmkr != NULL );
	m_plng = pmks->pmkr()->plng();
	ASSERT( m_plng != NULL );
	m_bEmbeddedMarkup = bEmbeddedMarkup;
	if ( m_bEmbeddedMarkup )
		{
		m_pmellst = new CMarkupList(m_pmkr);
		m_pmellst->IncrementNumRefs();
		m_pmelCur = m_pmellst->pmelFirst();
		}
	else
		{
		m_pmellst = NULL;
		m_pmelCur = NULL;
		}
}


CMStringLoc::CMStringLoc(const CMStringLoc& loc, const Str8* ps)
{
	ASSERT( ps != NULL );
	m_psz = *ps;
	m_ps = ps;
	m_pmkr = loc.m_pmkr;
	m_plng = loc.m_plng;
	m_bEmbeddedMarkup = FALSE;
	m_pmellst = NULL;
	m_pmelCur = NULL;
#ifdef _DEBUG
	m_pszBeginning = m_psz;
#endif  // _DEBUG
}


CMStringLoc::CMStringLoc(const CMStringLoc& loc)  // copy constructor
{
	m_psz = loc.m_psz;
	m_ps = loc.m_ps;
	m_pmkr = loc.m_pmkr;
	m_plng = loc.m_plng;
	m_bEmbeddedMarkup = loc.m_bEmbeddedMarkup;
	m_pmellst = loc.m_pmellst;
	if ( m_bEmbeddedMarkup )
		{
		ASSERT( m_pmellst != NULL );
		m_pmellst->IncrementNumRefs();
		}
	m_pmelCur = loc.m_pmelCur;
#ifdef _DEBUG
	m_pszBeginning = loc.m_pszBeginning;
	AssertValid();
#endif  // _DEBUG
}


void CMStringLoc::Relocate(const CMStringLoc* ploc)
{
	// as if *this = *ploc
	ASSERT( ploc != this );  // disallow self-assignment
#ifdef _DEBUG
	ploc->AssertValid();
#endif  // _DEBUG
	ASSERT( m_ps == ploc->m_ps );
#ifdef _DEBUG
	ASSERT( m_pszBeginning == ploc->m_pszBeginning );
#endif
	ASSERT( m_bEmbeddedMarkup == ploc->m_bEmbeddedMarkup );
	ASSERT( m_pmellst == ploc->m_pmellst );

	m_psz = ploc->m_psz;
	m_pmkr = ploc->m_pmkr;
	m_plng = ploc->m_plng;
	m_pmelCur = ploc->m_pmelCur;
}


CMStringLoc::~CMStringLoc()
{
#ifdef _DEBUG
	AssertValid();
#endif  // _DEBUG
	if ( m_bEmbeddedMarkup )
		{
		ASSERT( m_pmellst != NULL );
		m_pmellst->DecrementNumRefs();
		if ( !m_pmellst->bHasRefs() )
			delete m_pmellst;
		}
}


BOOL CMStringLoc::bAtValidLoc() const
{
#ifdef _DEBUG
	AssertValid();
#endif  // _DEBUG
	CMStringLoc loc(*this, m_ps);
	for ( ; loc.bNotAtEnd() && (loc < *this); loc.Next() ) {}
	BOOL bValid = (loc == *this);

	return bValid;
}


BOOL CMStringLoc::operator==(const CMStringLoc& loc) const
{
	ASSERT( m_ps == loc.m_ps );
	return (m_psz == loc.m_psz);
}


BOOL CMStringLoc::operator!=(const CMStringLoc& loc) const
{
	ASSERT( m_ps == loc.m_ps );
	return (m_psz != loc.m_psz);
}


BOOL CMStringLoc::operator<(const CMStringLoc& loc) const
{
	ASSERT( m_ps == loc.m_ps );
	return (m_psz < loc.m_psz);
}


int CMStringLoc::operator-(const CMStringLoc& loc) const
{
	ASSERT( m_ps == loc.m_ps );
	return (m_psz - loc.m_psz);
}


BOOL CMStringLoc::bMarkup_Next()
{
	CMarker* pmkr;
	Length lenMarkup;
	return bMarkup_Next(&pmkr, &lenMarkup);
}


BOOL CMStringLoc::bMarkup_Next(CMarker** ppmkr, Length* plenMarkup)
{
	ASSERT( bNotAtEnd() );
	if ( !bAtMarkup() )
		return FALSE;

	// 1995-04-19 MRP
	const char* pszAfterMarkup = m_psz;
	const char* pszMarker = NULL;
	Length lenMarker;
	BOOL bOpening;
	if ( !s_bMarkup_Next(&pszAfterMarkup, &pszMarker, &lenMarker, &bOpening) )
		return FALSE;
		
	if ( !bMarkup(pszMarker, lenMarker, bOpening, TRUE, ppmkr) )
		return FALSE;

	*plenMarkup = pszAfterMarkup - m_psz;
	m_psz = pszAfterMarkup;  // after right curly brace
	return TRUE;
}


BOOL CMStringLoc::bMarkup_Prev()
{
	CMarker* pmkr;
	Length lenMarkup;
	return bMarkup_Prev(&pmkr, &lenMarkup);
}


BOOL CMStringLoc::bMarkup_Prev(CMarker** ppmkr, Length* plenMarkup)
{
	ASSERT( !bAtBeginning() );
	if ( !bAfterMarkup() )
		return FALSE;

	// 1995-04-19 MRP
	const char* psz = m_psz;
	const char* pszMarker = NULL;
	Length lenMarker;
	BOOL bOpening;
	if ( !s_bMarkup_Prev(*m_ps, &psz, &pszMarker, &lenMarker, &bOpening) )
		return FALSE;

	if ( !bMarkup(pszMarker, lenMarker, bOpening, FALSE, ppmkr) )
		return FALSE;

	*plenMarkup = m_psz - psz;
	m_psz = psz;  // at left curly brace
	return TRUE;
}


BOOL CMStringLoc::s_bMarkup_Next(const char** ppsz,
		const char** ppszMarker, Length* plenMarker, BOOL *pbOpening)
{
	ASSERT( **ppsz == '{' );
	const char* pszMarker = *ppsz + 1;  // move past opening curly brace
	BOOL bOpening = ( *pszMarker != '-' );  // whether opening or closing
	if ( !bOpening )
		pszMarker += 1;  // move past dash, which indicates a closing marker
		
	const char* pszAfterMarker = pszMarker;
	Str8 sMarker;
	if ( bOpening || (*pszAfterMarker != '}') )
		{
		// Opening markers cannot be empty, but closing ones can, e.g. "{-}".
		CNote* pnot = NULL;
		BOOL bMarker = CMarker::s_bMatchNameAt(&pszAfterMarker, "}",
			sMarker, &pnot);
		ASSERT( bMarker );  // 1995-04-19 MRP: error recovery!!!
		}

	ASSERT( *pszAfterMarker == '}' );
	*ppsz = pszAfterMarker + 1;  // after right curly brace
	*ppszMarker = pszMarker;  // at marker itself			
	*plenMarker = pszAfterMarker - pszMarker;
	*pbOpening = bOpening;
	return TRUE;
}


BOOL CMStringLoc::s_bMarkup_Prev(const char* pszAtBeginning, const char** ppsz,
		const char** ppszMarker, Length* plenMarker, BOOL *pbOpening)
{
	ASSERT( *(*ppsz - 1) == '}' );
	// Move to beginning of marker
	const char* psz = *ppsz - 1;
	for ( ; (psz != pszAtBeginning) && (*psz != '{'); psz -= 1 ) {}
	ASSERT( *psz == '{' );  // 1995-04-18 MRP: Error recovery!!!

	const char* pszAfterMarkup = psz;	
	if ( !s_bMarkup_Next(&pszAfterMarkup, ppszMarker, plenMarker, pbOpening) )
		return FALSE; 

	*ppsz = psz;  // at left curly brace
	return TRUE;
}


BOOL CMStringLoc::bMarkup(const char* pszMarker, Length lenMarker,
		BOOL bOpening, BOOL bMovingToNext, CMarker** ppmkr)
{
	ASSERT( !bOpening || (lenMarker != 0) );  // opening cannot be empty
	CMarker* pmkr = ( lenMarker != 0 ?
		pmkrSearch_AddIfNew(pszMarker, lenMarker) :
		m_pmelCur->pmkrEl()
		);
	// If closing markup when moving to next, or opening when moving to prev,
	// then it should be the same as the current marker _before_ moving.
	ASSERT( !(bMovingToNext ^ bOpening) || (pmkr == m_pmkr) ); 

	// NOTE NOTE NOTE: This loc and its markup list are synchronized here.
	if ( bMovingToNext )
		// Add this marker to markup element list, and make it current
		m_pmelCur = m_pmellst->pmelNext_AddIfNew(m_pmelCur, pmkr, bOpening);
	else
		m_pmelCur = m_pmellst->pmelPrev(m_pmelCur);
	ASSERT( m_pmelCur != NULL ); 
	m_pmkr = m_pmelCur->pmkrAtLocsAfter();  // update this loc's current marker	
	m_plng = m_pmkr->plng();  // update this loc's current language encoding

	*ppmkr = pmkr;		
	return TRUE;
}


CMarker* CMStringLoc::pmkrSearch_AddIfNew(const char* pszMarker, Length lenMarker)
{
	CMarkerSet* pmkrset = m_pmkr->pmkrsetMyOwner();
	ASSERT( pmkrset != NULL );
	// 1995-04-15 MRP: Search(psz, len) would avoid constructing a Str8
	Str8 sMarker(pszMarker, lenMarker);
	CMarker* pmkr = pmkrset->pmkrSearch_AddIfNew(sMarker);
	
	return pmkr;
}


const CMChar* CMStringLoc::pmch_Next()
{
	Length lenMChar;
	return pmch_Next(&lenMChar);
}


const CMChar* CMStringLoc::pmch_Next(Length* plenMChar)
{
	const CMChar* pmch = NULL;
	BOOL bDefined = bMChar_Next(&pmch, plenMChar);
	return pmch;
}


BOOL CMStringLoc::bMChar_Next(const CMChar** ppmch, Length* plenMChar)
{
	ASSERT( bNotAtEnd() );
	const char* pszAtMCharBeginning = m_psz;
	BOOL bDefined = m_plng->bMChar(&m_psz, ppmch);
//	ASSERT( bMChar );

	*plenMChar = m_psz - pszAtMCharBeginning;
	ASSERT( *plenMChar != 0 );	
	return bDefined;  // 1995-04-26 MRP
}


const CMChar* CMStringLoc::pmch_Prev()
{
	Length lenMChar;
	return pmch_Prev(&lenMChar);
}


const CMChar* CMStringLoc::pmch_Prev(Length* plenMChar)
{
	const CMChar* pmch = NULL;
	BOOL bDefined = bMChar_Prev(&pmch, plenMChar);
	return pmch;
}


BOOL CMStringLoc::bMChar_Prev(const CMChar** ppmch, Length* plenMChar)
{
	ASSERT( !bAtBeginning() );
	const char* pszCur = m_psz;
		
	// Relocate this to the beginning of the string
	RelocateToBeginning();

	CMStringLoc locNext = *this;
	BOOL bDefined = FALSE;
	const CMChar* pmch = NULL;
	Length lenMChar;
	if ( !locNext.bMarkup_Next() )
		bDefined = locNext.bMChar_Next(&pmch, &lenMChar);
	while ( locNext.psz() < pszCur )
		{
		this->Relocate(&locNext);
		if ( locNext.bMarkup_Next() )
			pmch = NULL;
		else
			bDefined = locNext.bMChar_Next(&pmch, &lenMChar);
		}
	ASSERT( locNext.psz() == pszCur );
	ASSERT( pmch != NULL );

	*ppmch = pmch;	
	*plenMChar = lenMChar;
	return bDefined;  // 1995-04-26 MRP
}


BOOL CMStringLoc::bMChar_NextIncluded(const CMChar** ppmch, Length* plenMChar,
		MatchSetting matset)
{
	ASSERT( bNotAtEnd() );
	const char* psz = m_psz;
	do
		{
		if ( bAtMarkup() )
			return FALSE;

		const CMChar* pmch = NULL;
		BOOL bMChar = m_plng->bMChar(&psz, &pmch);

		if ( m_plng->bIncludeInMatch(pmch, matset) )
			{
			*plenMChar = psz - m_psz;
			ASSERT( *plenMChar != 0 );
			m_psz = psz;
			*ppmch = pmch;
			return TRUE;
			}  // if
		m_psz = psz;
		// Skip over any multi-chars not included at the match setting
		// which precede the first multi-char which is.
		// E.g. "-'e": If "-" is an ignore, and "'" a separate diacritic,
		// at SamePrimaryGroup, skip them both and return the multi-char "e".
		} while ( bNotAtEnd() );
	
	return FALSE;
}
 

void CMStringLoc::Next()
{
	if ( !bMarkup_Next() )  // returns TRUE if a marker was matched
		// if no marker at current location, match a multichar instead
		(void) pmch_Next();
}


void CMStringLoc::Prev()
{
	if ( !bMarkup_Prev() )  // returns TRUE if a marker was matched
		// if no marker at previous location, match a multichar instead
		(void) pmch_Prev();
}

void CMStringLoc::RelocateToBeginning()
{
	m_psz = *m_ps;
	if ( m_bEmbeddedMarkup )
		{
		m_pmelCur = m_pmellst->pmelFirst();
		m_pmkr = m_pmelCur->pmkrAtLocsAfter();
		m_plng = m_pmkr->plng();
		}
}


void CMStringLoc::RelocateToEnd()
{
	// Must "step" to the end rather than simply "jumping" there
	// in order to interpret substring markers correctly.
	while ( *m_psz != '\0' )
		Next();
}


char CMStringLoc::ch_Next()
{
	ASSERT( *m_psz != '\0' );
	return *m_psz++;
}


char CMStringLoc::ch_Prev()
{
#ifdef _DEBUG
	ASSERT( m_psz != m_pszBeginning );
#endif
	return *--m_psz;
}


#ifdef _DEBUG
void CMStringLoc::AssertValid() const
{
	ASSERT( m_plng == m_pmkr->plng() );
	// If the string's internal character buffer moves during the lifetime
	// of a marked string location, its location pointer will not be valid.
	ASSERT( *m_ps == m_pszBeginning );
}
#endif  // _DEBUG
