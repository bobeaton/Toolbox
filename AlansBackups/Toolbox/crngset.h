// CRngSet.h CRangeEl and CRangeSet classes  Brian Yoder 11 Jul 95

// This class is derived from CSet and includes all of its public
// member functions. See cset.h for documentation of them.

#ifndef RNGSET_H
#define RNGSET_H

// CRangeEl.h CRangeEl class, single element of Range Set Brian Yoder 12 July 95

#include "set.h" // Base class for derivation
#include "crecpos.h"
#include "ind.h"

class CMarker;   //08-25-1997
class CIndex;    //08-25-1997
class CProgressIndicator;    //08-25-1997

class CField;  // cfield.h
class CShwView;  // shwview.h

////////////////////////////////////////////////////////////////////////////////////

class CDataItemEl : public CSetEl { // Index element, derived from doubly linked list element base class
friend class CDataItemSet;
protected:
    CDataItemEl( const char* pszKey ); // *** Constructor with content
public:
    const Str8& sContent() const // Get content
        { return sKey(); }
    const char* pszContent() const // Get content char* flavor
        { return pszKey(); }
        
    virtual CSet* psetMyOwner() { return NULL; }  // Required for CSet::ChangeNameTo

#ifdef _DEBUG
    void AssertValid() const { } // *** Assert validity of element
#endif      
}; // class CDataItemEl

class CRangeEl : public CDataItemEl { // Just renaming the CDataItemEl for consistency reasons
friend class CRangeSet;
private:
    CRangeEl( const char* pszKey ) : CDataItemEl( pszKey ) {} // *** Constructor with content
}; // class CRangeEl


//------------------------------------------------------------------------

class CDataItemSet : public CSet { // Data Item Set class, derived from lookup base class

protected:
    BOOL m_bCharacterItems; //TRUE if data item elements are single Characters

public:
    CDataItemSet(BOOL bCharacterItems = FALSE); // Empty set constructor
    CDataItemSet(const CDataItemSet& datset);  // Copy constructor
    CDataItemSet(const CDataItemSet* pdatset);  // Alternate copy constructor (from potentially null pointer)
    void operator=(const CDataItemSet& datset);
    void Append(const CDataItemSet& datset); // Add() the items in pdatset
    void TransferDataItems(CDataItemSet& datset); // Transfer in pdatset
    virtual BOOL bAdd(const Str8& s, CDataItemEl** ppdat = NULL);  // string item
    void AddItems( const char* pszStr );  // string of whitespace-separated data item set elements
    void RemoveElement(const Str8& s);

    // Use DeleteAll() to clear list
    // Use bIsEmpty() to check for empty Data Item Set

private:
    int iAddElementsFromField(CRecord* prec, CField* pfld,
                              BOOL bMultipleItemData, BOOL bMultipleWordItems,
                              CDataItemSet& datsetTemp);
    void Add( const Str8& s, CDataItemEl** ppdat = NULL );  // string item

public:
#ifdef typWritefstream // 1.6.4ac
    void WritePropertiesUnwrapped(Object_ofstream& obs) const;
#else
    void WritePropertiesUnwrapped(Object_ostream& obs) const;
#endif
    BOOL bReadPropertiesVer43( Object_istream& obs);

    // Overrides of First, Next, etc. needed for type safe return value
    const CDataItemEl* pdatFirst() const // First element
        { return (CDataItemEl*) pselFirst(); }
    const CDataItemEl* pdatLast() const // Last element
        { return (CDataItemEl*) pselLast(); }
    const CDataItemEl* pdatNext( const CDataItemEl* pdatCur ) const // Next element after current
        { return (CDataItemEl*) pselNext( pdatCur ); }
    const CDataItemEl* pdatPrev(const CDataItemEl* pdatCur ) const // Previous element after current
        { return (CDataItemEl*) pselPrev( pdatCur ); }
    
    BOOL bMatch( const char* psz ) const // Perform a search
        { return ( CSet::pselSearch( psz, whole ) != NULL ); }

    CDataItemEl* pdatFindClosestMatch( const char* psz ) const
        { return (CDataItemEl*) pselFindClosestMatch( psz ); }

    void ConvertFromCharacterBasedSet() { m_bCharacterItems = FALSE; }
    BOOL bCharacterItems() const { return m_bCharacterItems; } //TRUE if data item elements are single Characters
    BOOL bHasMultiwordElements() const;

    void BuildDataItemSet(const CMarker *pmkr, BOOL bMultipleItemData, BOOL bMultipleWordItems,
                          int& iAddedItems, int& iItemsTotal, int& iItemsNotFound);
      // 08-25-1997 This function builds a DataItemSet from all open databases of the
      // same type as that of the marker passed. It guards against duplicate entries.
      // Post-conditions:
      //   iAddedItems shows how many new entries were added;
      //   iItemsNotFound shows how many of the existing elements were not found in the data;
      //   iItemsTotal shows how many elements the data item set has after building.

}; // class CDataItemSet

class CRangeSet : public CDataItemSet {

private:
    //08-19-1997
    //BOOL m_bRangeSetSpaceAllowed;  //TRUE if field may contain spaces

    //09-17-1997
    CMarker* m_pmkr;  // m_pmkr points to the marker coresponding to this range set

    // Prevent use of Copy constructor and assignment operator by making them private
    CRangeSet(const CRangeSet& datset);
    CRangeSet& operator=(const CRangeSet& datset);

public:
    CRangeSet(CMarker* pmkr, BOOL bRangeSetForCharacters = FALSE);

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties( Object_istream& obs, CMarkerSet* pmkrset);

    // Overrides of First, Next, etc. needed for type safe return value
    const CRangeEl* prngFirst() const // First element
        { return (CRangeEl*) pdatFirst(); }
    const CRangeEl* prngLast() const // Last element
        { return (CRangeEl*) pdatLast(); }
    const CRangeEl* prngNext( const CRangeEl* prngCur ) const // Next element after current
        { return (CRangeEl*) pdatNext( prngCur ); }
    const CRangeEl* prngPrev( const CRangeEl* prngCur ) const // Previous element after current
        { return (CRangeEl*) pdatPrev( prngCur ); }
  
    CRangeEl* prngFindClosestMatch( const char* psz ) const
        { return (CRangeEl*) pselFindClosestMatch( psz ); }

    // I don't think this is needed now    
//    BOOL CRangeSet::bAdd( const char* pszStr ); // string contents interpreted based on data definition
    BOOL bAdd(const Str8& s);  // Calls base class bAdd after doing a range-set-specific check
    
    // Use DeleteAll() to clear list

    BOOL bRangeSetForCharacters() const           //08-18-1997
        { return m_bCharacterItems; }
// TLB 08-19-1999: Range sets no longer have jurisdiction over whitespace issues.
//    BOOL bRangeSetSpaceAllowed() const          //08-19-1997
//        { return TRUE; }
//    void SetRangeSetSpaceAllowed(BOOL spaceallowed)  // 08-19-1997
//        { m_bRangeSetSpaceAllowed = spaceallowed; }

    void Reset(CDataItemSet& datset); // Transfer in elements from pdatset
      //09-17-1997
    CMarker* pmkr() { return m_pmkr; }

	BOOL bModalView_Insert( Str8& sText );	// select and return a range set element

    BOOL bEnabled() const { return TRUE; } // Range sets are now always enabled

    // Here are the possible situations and the function return value.
    // 1. This data field doesn't have a range set (TRUE, iChar undefined).
    // 2. It has a range set and the data is consistent (TRUE, iChar undefined).
    // 3. It has a character-based range set and there is at least
    //    one inconsistent character (FALSE, iChar set to first invalid character).
    // 4. It has an item-based range set and the data is not in the range
    //    set (FALSE, iChar undefined)
    BOOL CRangeSet::bConsistent(const Str8& sData, int& iChar);

}; // class CRangeSet

#endif  // RNGSET_H
