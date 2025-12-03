// set.cpp  Sets and set elements

#include "stdafx.h"
#include "toolbox.h"
#include "malloc.h"
#include "set.h"
#include "not.h"  // CNote
#include "shwnotes.h"  // IDN_...
#include <ctype.h>  // isdigit

#include "set_d.h"  // DlgNotYetImplemented
#include "ddx.h"


// **************************************************************************
CDynamicBitArray::CDynamicBitArray(unsigned int uiSize)
{
    if ( uiSize > 0 )
        {
        m_pulBits = (unsigned long*) malloc(uiSize * sizeof(DBAKEY));
        ASSERT( m_pulBits );
        if ( m_pulBits )
            {
            m_uiSize = uiSize;
            for (unsigned int i = 0; i < m_uiSize; i++)
                {
                m_pulBits[i] = 0;
                }
            }
        }
    else
        {
        m_uiSize = uiSize;
        m_pulBits = 0;
        }
}

CDynamicBitArray::~CDynamicBitArray()
{
    if ( m_pulBits )
        free(m_pulBits);
}

BOOL CDynamicBitArray::operator[] (DBAKEY dbakey) const
{
    unsigned long ulMask = 1;
    ulMask <<= (dbakey & BITMASK);
    dbakey >>= BITMASK_DIGITS;
    ASSERT(dbakey < INT_MAX);
    if ( dbakey < m_uiSize )
        return (ulMask & m_pulBits[dbakey])? TRUE: FALSE;
    else
        return FALSE;
}

BOOL CDynamicBitArray::bExtend(unsigned int uiNewSize)
{
    ASSERT(m_uiSize < uiNewSize);
    ASSERT(uiNewSize <= INT_MAX);
    if ( m_uiSize > 0 )
        {
        if ( m_uiSize == INT_MAX )
            return FALSE;
        m_pulBits = (DBAKEY*) realloc(m_pulBits, uiNewSize * sizeof(DBAKEY));
        }
    else
        {
        m_pulBits = (DBAKEY*) malloc(uiNewSize * sizeof(DBAKEY));
        }
    ASSERT( m_pulBits );
    if ( m_pulBits )
        {
        for (; m_uiSize < uiNewSize; m_uiSize++)
            m_pulBits[m_uiSize] = 0L;
        return TRUE;
        }
    else
        {
        m_uiSize = 0;
        return FALSE;
        }
}

BOOL CDynamicBitArray::bSet(DBAKEY dbakey, BOOL bSet)
{
    unsigned long ulMask = 1;
    ulMask <<= (dbakey & BITMASK);
    dbakey >>= BITMASK_DIGITS;
    if ( dbakey < m_uiSize )
        {
        if ( bSet )
            m_pulBits[dbakey] |= ulMask;
        else
            m_pulBits[dbakey] &= ~ulMask;
        }
    else if ( bSet )
        {
        ASSERT(dbakey + 1 <= INT_MAX);
        if ( bExtend((unsigned int)(dbakey + 1)) )
            m_pulBits[dbakey] |= ulMask;
        else
            return FALSE;
        }
    return TRUE;
}

// **************************************************************************

CSubsetAllocationArray::~CSubsetAllocationArray()
{
    // Make sure all allocated subsets have been deallocated before destroying this object.
    for (SUBSET_KEY subsetkey = 0; subsetkey < uiSizeInBits(); subsetkey++)
        ASSERT(!operator[](subsetkey));
}

BOOL CSubsetAllocationArray::bAllocateSubset(SUBSET_KEY& subsetkey)
{
    // Find the least significant clear (unallocated) bit
    for (subsetkey = 0; (subsetkey >> BITMASK_DIGITS) < INT_MAX; subsetkey++)
        if ( !operator[](subsetkey) )
            return bSet(subsetkey, TRUE);
    return FALSE;
}

void CSubsetAllocationArray::DeAllocateSubset(SUBSET_KEY subsetkey)
{
    ASSERT(operator[](subsetkey));
    bSet(subsetkey, FALSE); // Clear the bit = available 
}


// **************************************************************************

CSetEl::CSetEl(const char* pszName) :
    CDblListEl()
{
    m_sKey = pszName;
}  // constructor

CSetEl::~CSetEl()
{
#ifdef _DEBUG
    // Identify which element it is whose reference count is ASSERTing.
    const Str8& s = sName();
#endif  // _DEBUG
}

void CSetEl::SetKey(const char* pszNewKey)
{
    ASSERT( pszNewKey != NULL );
    m_sKey = pszNewKey;
}

void CSetEl::ChangeNameTo(const char* pszNewName)
{
    ASSERT( pszNewName );
    if ( !bEqual( sName(), pszNewName ) )
        {
        // If psel's name is actually being changed, and if it is contained
        // in the set, then it must be removed and added back into
        // its set, since elements in a set are ordered by their names.
        // Note: When the user chooses OK to confirm an Add or Copy
        // the element's name gets changed just before it is added to its set. 
        CSet* pset = psetMyOwner();
        BOOL bContainedInSet = pset->pselPrev( this ) ||
            pset->pselNext( this ) ||
            this == pset->pselFirst();
        if ( bContainedInSet )
            (void) pset->pselRemove( this );  // remove this element
        SetKey( pszNewName );  // change its key
        if ( bContainedInSet )
            pset->Add( this );  // add it again in correct order
        }
}


const Str8& CSetEl::sItem() const
{
    return sName();
}

BOOL CSetEl::bCopy(CSetEl** ppselNew)
{
    DlgNotYetImplemented();
    return FALSE;
}

BOOL CSetEl::bModify()
{
    DlgNotYetImplemented();
    return FALSE;
}

BOOL CSetEl::bDeletable()
{
    return !bHasRefs();
}


// --------------------------------------------------------------------------
CSet::CSet()
{
    m_psaa = 0;
}

CSet::~CSet()
{
    if ( m_psaa )
        {
        delete m_psaa;
        m_psaa = 0;
        }
}

BOOL CSet::bValidName(const char* pszName, CNote** ppnot)
{
    ASSERT( pszName );
    Length lenName = strlen(pszName);
    ASSERT( ppnot );
    if ( lenName == 0 )  // Assuming it's been trimmed
        {
        *ppnot = new CNote(_("Expecting a name"), "", pszName, 0);
        return FALSE;
        }
    
    return TRUE;
}

BOOL CSet::bValidNewName(const char* pszName, CNote** ppnot)
{
    if ( !bValidName(pszName, ppnot) )
        return FALSE;
        
    if ( pselSearch(pszName) )
        {
        // 1996-03-06 MRP: How to generalize this note???
        *ppnot = new CNote(_("This name has already been used:"), pszName, // 1.5.0ft 
            pszName, strlen(pszName));
        return FALSE;
        }

    return TRUE;    
}

Str8 CSet::sUniqueName(const char* pszBaseName)
{
    ASSERT( pszBaseName );
    const char* psz = pszBaseName + strlen(pszBaseName);
    while ( psz != pszBaseName && isdigit(*(psz-1)) )
        psz -= 1;
    long n = ( *psz ? atol(psz) : 1 );
    int lenBase = psz - pszBaseName;
    Str8 sName = pszBaseName;
    while ( pselSearch(sName) )  // An element already has this name
        sName.Format("%.*s%ld", lenBase, pszBaseName, ++n);  // Base2, Base3, etc.
            
    return sName;
}

void CSet::Add(CSetEl* psel)
{ // *** Maybe this code can use Trie access for higher speed insertion
    ASSERT( psel );
    if ( bIsEmpty() ) // If list is empty, use Add
		{ CDblList::Add( psel ); return; }
    for ( CSetEl* pselCur = pselLast(); pselCur; pselCur = pselPrev( pselCur ) ) // For all, starting from last
#ifdef _MPPC_ 
		if ( strcmpu( psel->m_sKey, pselCur->m_sKey ) >= 0 ) // If same or greater, insert after
#else
        if ( strcmp( psel->m_sKey, pselCur->m_sKey ) >= 0 ) // If same or greater, insert after
#endif // _MPPC_
			{ CDblList::InsertAfter( pselCur, psel ); return; }
    CDblList::InsertBefore( pselFirst(), psel ); // If place not found, insert at top	
}

CSetEl* CSet::pselRemove(CSetEl* psel)
{
    return (CSetEl*)CDblList::pelRemove(psel);
}

// Return negative if s < t, zero if s == t, positive if s > t
// If partial match, s is considered equal if it matches a prefix of t
// If s == t, return the length matched (i.e. length of s)
int CSet::iCompare( const char* s, const char* t,
		BOOL bWhole, Length* plenMatched ) const
{
 	const char* start = s;
 	for ( ; *s; s += 1, t += 1 ) // While there is a char in *s
 		if ( *s != *t ) // If it does not match *t (includes if *t is null), fail
 			return (unsigned char)*s - (unsigned char)*t;
 	// If arrive here, all chars in s matched
 	if ( (*t != '\0') && (bWhole == whole) )
 		return -1;  // s < t, if not at end of t, and matching whole key
 	*plenMatched = (Length)(s - start);
 	return 0;
}

int CSet::iCompareWholeSubString( const char* s, Length len, const char* t ) const
{
 	for ( ; *s && len != 0; s += 1, t += 1, len -= 1 ) // While there is a char in *s
 		if ( *s != *t ) // If it does not match *t (includes if *t is null), fail
 			break;

 	int i = ( len == 0 && !*t ? 0 : (unsigned char)*s - (unsigned char)*t );
 	return i;
}

CSetEl* CSet::pselSearch( const char* psz, BOOL bWhole ) const // Perform a search
{ // *** Replace this code with trie access for high speed search
	CSetEl* psel = pselFirst();
		
	return pselSearch( psel, psz, bWhole );			
}


// Returns the element which matches psz, if any. Otherwise, returns the
// element which would appear immediately before psz (closest < match), if
// any. Otherwise, returns the element which would appear immediately
// after psz (closest > match), if any. Otherwise, returns NULL.        
CSetEl* CSet::pselFindClosestMatch( const char* psz ) const
{
	Length lenMatched;  // just a placeholder
	for ( CSetEl* psel = pselFirst(); psel; psel = pselNext( psel ) )
		{
		int i = iCompare( psz, psel->m_sKey, TRUE, &lenMatched );
		if ( i == 0 )
			return psel;
		else if ( i < 0 )  // if the search text is less than this key,
			return pselPrev( psel );  // then no key following this could match the text
		}
	return pselLast();			
}
     
CSetEl* CSet::pselSearch( CSetEl* psel, const char* psz, BOOL bWhole ) const // Perform a search
{ // *** Replace this code with trie access for high speed search
	Length lenMatched;  // just a placeholder
	for ( ; psel; psel = pselNext( psel ) ) // return first match
		{
		int i = iCompare( psz, psel->m_sKey, bWhole, &lenMatched );
		if ( i == 0 )
			return psel;
		else if ( i < 0 )  // if the search text is less than this key,
			break;  // then no key following this could match the text
		}
	return NULL;			
}

CSetEl* CSet::pselSearchForWholeSubString( const char* psz, Length len ) const
{
	CSetEl* psel = pselFirst();
		
	return pselSearchForWholeSubString( psel, psz, len );
}

CSetEl* CSet::pselSearchLongest( const char** ppsz ) const // Search for longest match
{ // *** Replace this code with trie access for high speed search
	CSetEl* psel = pselFirst();
		
	return pselSearchLongest( psel, ppsz );
}

// Insert new after current, both must have same key	
void CSet::InsertAfter( CSetEl* pselCur, CSetEl* pselNew )
{
	ASSERT( pselCur );
	ASSERT( pselNew );
	ASSERT( !strcmp( pselNew->m_sKey, pselCur->m_sKey ) ); // Must be same key
	CDblList::InsertAfter( pselCur, pselNew );
}

// Insert new before current, both must have same key
void CSet::InsertBefore( CSetEl* pselCur, CSetEl* pselNew )
{
	ASSERT( pselCur );
	ASSERT( pselNew );
	ASSERT( !strcmp( pselNew->m_sKey, pselCur->m_sKey ) ); // Must be same key
	CDblList::InsertBefore( pselCur, pselNew );
}

void CSet::SetElementKey( CSetEl* psel, const char* pszNewKey )
{
	ASSERT( psel );
	ASSERT( pszNewKey );
	if ( !bEqual( psel->m_sKey, pszNewKey ) )
	{
		// If psel's key is actually being changed, and if it is
		// adjacent to other elements in the collection,
		// then it must be removed and added back into this collection,
		// since psels in a CSet collection are ordered by their keys.
		BOOL bAdjacentToOtherElements = pselPrev( psel ) || pselNext( psel );
		if ( bAdjacentToOtherElements )
			pselRemove( psel );  // remove the element
		psel->SetKey( pszNewKey );  // change its key
		if ( bAdjacentToOtherElements )
			Add( psel );  // add it again in correct order
	}
}

// If s matches front of t, return number of chars that match
static int iStrMatch( const char* s, const char* t ) 
{
 	const char* start = s;
 	while ( *s ) // While there is a char in *s
 		if ( *s++ != *t++ ) // If it does not match *t (includes if *t is null), fail
 			return -1;
 	 	// If arrive here, all chars in s matched
	return s - start;
}


CSetEl* CSet::pselSearchLongest( CSetEl* psel, const char** ppsz ) const // Search for longest match
{ // *** Replace this code with trie access for high speed search
	int iLongest = -1;
	CSetEl* pselLongest = 0;
	for ( ; psel; psel = pselNext( psel ) ) // For every key 
		{
		int iLen = 0;
		if ( ( iLen = iStrMatch( psel->m_sKey, *ppsz ) ) > iLongest ) // If this key matches
			{
			pselLongest = psel; // Remember this key 
			iLongest = iLen; // Remember its length
			}
		}
	if ( iLongest > -1 ) // If sucess
		*ppsz += iLongest; // Move past match
	return pselLongest; // Return first key with longest match, NULL if none
}

BOOL CSet::bAllocateSubset(SUBSET_KEY& subsetkey)
{
    if ( !m_psaa )
        {
        m_psaa = new CSubsetAllocationArray();
        ASSERT(m_psaa);
        }
    return m_psaa->bAllocateSubset(subsetkey);
}

void CSet::DeAllocateSubset(SUBSET_KEY subsetkey)
{
    ASSERT(m_psaa);
	CSetEl* psel = pselFirst();
	for ( ; psel; psel = pselNext( psel ) )
        psel->m_sma.bSet(subsetkey, FALSE);
    m_psaa->DeAllocateSubset(subsetkey);
}

CSetEl* CSet::pselSearchForWholeSubString( CSetEl* psel,
		const char* psz, Length len ) const
{
	for ( ; psel; psel = pselNext( psel ) ) // return first match
		{
		int i = iCompareWholeSubString( psz, len, psel->m_sKey );
		if ( i == 0 )
			return psel;
		else if ( i < 0 )  // if the search text is less than this key,
			break;  // then no key following this could match the text
		}
	return NULL;		
}

BOOL CSet::bAdd(CSetEl** ppselNew)
{
    DlgNotYetImplemented();
    return FALSE;
}

// --------------------------------------------------------------------------

static const int s_lenTable = 256;

CFastSet::CFastSet()
{
    m_ppselTable = new CSetEl* [s_lenTable];
    ClearAll();
}

CFastSet::~CFastSet()
{
    delete [] m_ppselTable;
}


CSetEl* CFastSet::pselSearch( const char* pszName, BOOL bWhole ) const
{
    CSetEl* psel = pselHash(pszName);
        
    return (CSetEl*)CSet::pselSearch( psel, pszName, bWhole );          
}

CSetEl* CFastSet::pselSearchForWholeSubString( const char* pszName, Length len ) const
{
    CSetEl* psel = pselHash(pszName);
        
    return (CSetEl*)CSet::pselSearchForWholeSubString( psel, pszName, len );            
}

CSetEl* CFastSet::pselSearchLongest( const char** ppsz ) const // Search for longest match
{
    CSetEl* psel = pselHash(*ppsz);
    
    return (CSetEl*)CSet::pselSearchLongest( psel, ppsz );
}


void CFastSet::Add( CSetEl* pselNew )
{
    CSet::Add(pselNew);
    CSetEl* psel = pselHash(pselNew->sKey());
    if ( !psel || psel == pselNext(pselNew) )
        SetHash(pselNew);
}

CSetEl* CFastSet::pselRemove( CSetEl* psel )
{
    UpdateTableBeforeRemoving(psel);
    
    return CSet::pselRemove(psel);
}

#ifdef _DEBUG
	void CSetEl::AssertValid() const
		{
		// Assert validity of element
		}
#endif		

void CFastSet::Delete( CSetEl** ppsel )
{
    ASSERT( ppsel );
    CSetEl* psel = *ppsel;
    UpdateTableBeforeRemoving(psel);
    
    CSet::Delete(ppsel);
}

void CFastSet::DeleteAll()
{
    ClearAll();  // Clear the hash table
    CDblList::DeleteAll();  // Delete all the elements
}


CSetEl* CFastSet::pselHash(const char* psz) const
{
    ASSERT( psz );
    unsigned int u = (unsigned char)*psz;
    ASSERT( u < s_lenTable );
    ASSERT( m_ppselTable );
    return m_ppselTable[u];
}

void CFastSet::SetHash(CSetEl* psel)
{
    ASSERT( psel );
    unsigned int u = (unsigned char)*psel->pszKey();
    ASSERT( u < s_lenTable );
    ASSERT( m_ppselTable );
    m_ppselTable[u] = psel;
}

void CFastSet::ClearHash(const char* psz)
{
    ASSERT( psz );
    unsigned int u = (unsigned char)*psz;
    ASSERT( u < s_lenTable );
    ASSERT( m_ppselTable );
    m_ppselTable[u] = NULL;
}

void CFastSet::ClearAll()
{
    unsigned int u = 0;
    for ( ; u != s_lenTable; u += 1 )
        m_ppselTable[u] = NULL;
}

void CFastSet::UpdateTableBeforeRemoving(CSetEl* pselAboutToBeRemoved)
{
    ASSERT( pselAboutToBeRemoved );
    CSetEl* psel = pselHash(pselAboutToBeRemoved->sKey());
    
    // If we're about to remove a table element.
    if ( psel == pselAboutToBeRemoved )
        {
        CSetEl *pselSecond = pselNext(psel);
        if ( pselSecond && *pselSecond->pszKey() == *psel->pszKey() )
            SetHash(pselSecond);
        else
            ClearHash(psel->pszKey());
        }
}


#ifdef _DEBUG
void CFastSet::AssertValid() const
{
//  CSet::AssertValid();  // Elements should be in order and unique
    // Assert consistency of table and elements
}
void CSet::AssertValid() const
{
}
#endif  // _DEBUG

// --------------------------------------------------------------------------

CSubSet::CSubSet(CSet* pset, BOOL bIncludeAllElements, BOOL bAutoIncludeNewElements) :
    m_pset(pset),
    m_bIncludeAllElements(bIncludeAllElements),
    m_bAutoIncludeNewElements(bAutoIncludeNewElements)
{
    ASSERT( m_pset->bAllocateSubset( m_subsetkey ) );
}

CSubSet::CSubSet(const CSubSet& subset) :
    m_pset(subset.m_pset),
    m_bIncludeAllElements(subset.m_bIncludeAllElements),
    m_bAutoIncludeNewElements(subset.m_bAutoIncludeNewElements)
{
    // This is a new object, so we need to request allocation of a new bit
    if ( !m_pset->bAllocateSubset( m_subsetkey ) )
        {
        ASSERT( FALSE );
        return;
        }

    if ( !m_bIncludeAllElements )
        {    
        CSetEl* psel = m_pset->pselFirst();
        for ( ; psel; psel = m_pset->pselNext(psel) )
            IncludeElement(psel, subset.bIncludesElement(psel));
        }
}

void CSubSet::operator=(const CSubSet& subset)
{
    if ( this == &subset )
        return;

    // This is an existing object, so we can continue to use
    // the previously allocated bit.
    m_pset = subset.m_pset;
    m_bIncludeAllElements = subset.m_bIncludeAllElements;
    m_bAutoIncludeNewElements = subset.m_bAutoIncludeNewElements;

    if ( !m_bIncludeAllElements )
        {    
        CSetEl* psel = m_pset->pselFirst();
        for ( ; psel; psel = m_pset->pselNext(psel) )
            IncludeElement(psel, subset.bIncludesElement(psel));
        }
}

CSubSet::~CSubSet()
{
    m_pset->DeAllocateSubset(m_subsetkey);
}

BOOL CSubSet::bIncludesElement(const CSetEl* psel) const
{
    ASSERT( psel );
    if ( m_bIncludeAllElements )
        return TRUE;
    ASSERT( ((CSetEl *)psel)->psetMyOwner() == m_pset ); // Casting away const-ness because psetMyOwner really should be declared const, but it's too much of a headache to make it so.
    return ( m_bAutoIncludeNewElements ?
             !psel->m_sma[m_subsetkey] :
             psel->m_sma[m_subsetkey]
//        (m_lBit & sub.m_lBits) == 0L :  // Bit is clear = included
//        (m_lBit & sub.m_lBits) != 0L    // Bit is set   = included
        );
}

void CSubSet::IncludeElement(CSetEl* psel, BOOL bInclude)
{
    ASSERT( psel );
    ASSERT( psel->psetMyOwner() == m_pset );
    ASSERT( !m_bIncludeAllElements );
    if ( bInclude )
        {
        ASSERT(psel->m_sma.bSet(m_subsetkey, !m_bAutoIncludeNewElements));
//        if ( m_bAutoIncludeNewElements )
//            sub.m_lBits &= (~m_lBit);  // Clear the bit = included
//        else
//            sub.m_lBits |= m_lBit;     // Set the bit   = included
        }
    else  // Exclude
        {
        ASSERT(psel->m_sma.bSet(m_subsetkey, m_bAutoIncludeNewElements));
//        if ( m_bAutoIncludeNewElements )
//            sub.m_lBits |= m_lBit;     // Set the bit   = excluded
//        else
//            sub.m_lBits &= (~m_lBit);  // Clear the bit = excluded
        }
}

void CSubSet::IncludeSubsetElements(CSubSet &subset)
{
    CSetEl* psel = m_pset->pselFirst();
    for ( ; psel; psel = m_pset->pselNext(psel) )
        if ( subset.bIncludesElement(psel) )
            IncludeElement(psel, TRUE);
}

void CSubSet::IncludeAllElements(BOOL bIncludeAll)
{
    if ( bIncludeAll == m_bIncludeAllElements )
        return;

    m_bIncludeAllElements = bIncludeAll;
    if ( !m_bIncludeAllElements )
        {
        CSetEl* psel = m_pset->pselFirst();
        for ( ; psel; psel = m_pset->pselNext(psel) )
            IncludeElement(psel, TRUE);
        }
}

void CSubSet::ExcludeAllElements()
{
    m_bIncludeAllElements = FALSE;
    m_bAutoIncludeNewElements = FALSE;
    CSetEl* psel = m_pset->pselFirst();
    for ( ; psel; psel = m_pset->pselNext(psel) )
        IncludeElement(psel, FALSE);
}


void CSubSet::AutoIncludeNewElements(BOOL bAutoInclude)
{
    ASSERT( !m_bIncludeAllElements );
    if ( bAutoInclude == m_bAutoIncludeNewElements )
        return;

    m_bAutoIncludeNewElements = bAutoInclude;
    CSetEl* psel = m_pset->pselFirst();
    for ( ; psel; psel = m_pset->pselNext(psel) )
        IncludeElement(psel, !bIncludesElement(psel));  // Toggle the bit
}