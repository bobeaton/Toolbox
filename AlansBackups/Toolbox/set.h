// set.h  Sets and set elements
//
// Abstract base class from which to derive set and element classes
// for "settings" data entities (e.g. filters, markers; but not records).
// Factor out into this class common characteristics which have similar
// visual presentation and manipulative behavior in the user interface.

// See data entity classes CFilter and CFilterSet (fil.h).
// See interface base clases CSetComboBox (cbo.h), CSetListBox (lbo.h),
// CSetDlg and CSetPage (set_d.h)


#ifndef SET_H
#define SET_H

#include "cdbllist.h" // Base class for derivation
#include "cdblel.h" // For derivation
#include "ref.h"
#include <limits.h> // INT_MAX

class CNote;  // not.h
class CSet;  // forward reference

#define BITMASK 0xFFFFF
#define BITMASK_DIGITS 5
#define DBAKEY unsigned long  // Hungarian: dbakey
#define SUBSET_KEY DBAKEY     // Hingarian: subsetkey

// **************************************************************************

class CDynamicBitArray   // Hungarian: dba
{
private:
    unsigned long* m_pulBits;
    unsigned int m_uiSize;
    BOOL bExtend(unsigned int uiNewSize);

protected:
    unsigned int uiSize() {return m_uiSize;}
    unsigned int uiSizeInBits() {return m_uiSize * sizeof(unsigned long) * 8;}

public:
    CDynamicBitArray(unsigned int uiSize = 0);
    virtual ~CDynamicBitArray();
    BOOL operator[] (DBAKEY dbakey) const;
    BOOL bSet(DBAKEY dbakey, BOOL bSet);

};  // class CDynamicBitArray

// **************************************************************************

class CSubsetMembershipArray : public CDynamicBitArray   // Hungarian: sma
{
public:
    ~CSubsetMembershipArray() {};
private:
    friend class CSetEl; // No other object can construct one of these
    CSubsetMembershipArray() : CDynamicBitArray() {};
    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CSubsetMembershipArray(const CSubsetMembershipArray& sma);
    CSubsetMembershipArray& operator=(const CSubsetMembershipArray& sma);

};  // class CSubsetMembershipArray

// **************************************************************************

class CSubsetAllocationArray : public CDynamicBitArray   // Hungarian: saa
{
private:
    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CSubsetAllocationArray(const CSubsetAllocationArray& saa);
    CSubsetAllocationArray& operator=(const CSubsetAllocationArray& saa);

public:
    CSubsetAllocationArray() {};
    ~CSubsetAllocationArray();
    
    // Function to assign a unique bit to the newly allocated object.
    BOOL bAllocateSubset(SUBSET_KEY& subsetkey); 
    // Function to make the bit available again.
    void DeAllocateSubset(SUBSET_KEY subsetkey);

};  // class CSubsetAllocationArray

// **************************************************************************

class CSetEl : public CDblListEl   // Hungarian: sel
{
friend class CSet;
friend class CSubSet;
friend class CSubsetAllocationArray;
private:
    Str8 m_sKey;   // Key
    CRefCount m_ref;  // number of other entities referring to this
    CSubsetMembershipArray m_sma; // All sets can be subsetted - TLB 1999-05-03
    
    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CSetEl(const CSetEl& sel);
    CSetEl& operator=(const CSetEl& sel);
 
protected:
    CSetEl(const char* pszName);  // constructor

public:
    virtual ~CSetEl();  // destructor
    
    const char* pszKey() const { return m_sKey; }
    const Str8& sKey() const { return m_sKey; }
    void SetKey(const char* pszNewKey);  // 1995-05-05 MRP: make private!!!

    const Str8& sName() const { return sKey(); }  // its key is its name
    void ChangeNameTo(const char* pszNewName);  
    virtual CSet* psetMyOwner() = 0;  // Required for ChangeNameTo

    NumberOfRefs numRefs() const { return m_ref.numRefs(); }
    virtual BOOL bHasRefs() const { return m_ref.bHasRefs(); }
    void IncrementNumberOfRefs() { m_ref.IncrementNumberOfRefs(); }
    void DecrementNumberOfRefs() { m_ref.DecrementNumberOfRefs(); }
    // 1996-03-06 MRP: THE FOLLOWING ARE TO BE PHASED OUT
    void IncrementNumRefs() { m_ref.IncrementNumRefs(); }
    void DecrementNumRefs() { m_ref.DecrementNumRefs(); }

    virtual const Str8& sItem() const;
        // The string which represents the element when it appears as
        // an item in a combo (drop-down list) box.
        // Defaults to the element's name.

    virtual BOOL bCopy(CSetEl** ppselNew);
        // Let the user view, rename, and possibly otherwise modify
        // a copy of this element's properties, and then have it added to
        // the owner set as a new element.
        // Returns whether the user clicked OK (vs. Cancel).
        // TRUE (OK): Added a new element to the set.
        // FALSE (Cancel): The set is unchanged.

    virtual BOOL bModify();
        // Let the user view and possibly modify this element's properties.
        // Returns whether the user clicked OK (vs. Cancel).
        // TRUE (OK): This element may have been modified (if it was,
        //     an update hint should have been broadcast to all views).
        // FALSE (Cancel): This element definitely was not modified.

    virtual BOOL bDeletable();
        // Return whether this element is eligible to be deleted,
        // i.e. when it is selected, should the Delete button be enabled?
        // By default, it cannot be if it has references to it.
#ifdef _DEBUG
	void AssertValid() const;
#endif		
};  // class CSetEl


// --------------------------------------------------------------------------

class CSet : public CDblList  // Hungarian: set
{
private:
    friend class CSetEl;  // For the sake of CSetEl::ChangeNameTo()

    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CSet(const CSet& set);
    CSet& operator=(const CSet& set);
    CSubsetAllocationArray* m_psaa; // All sets can be subsetted - TLB 1999-05-03

public:
	enum { partial = 0x00, whole = 0x01}; // Partial or whole match for pelSearch

protected:
    CSet();  // constructor

public:
    ~CSet(); // destructor

    CSetEl* pselFirst() const
            { return (CSetEl*)pelFirst(); }
        // First element of this set; otherwise NULL, if this set is empty.
    CSetEl* pselLast() const
            { return (CSetEl*)pelLast(); }
        // Last element of this set; otherwise NULL, if this set is empty.
    CSetEl* pselNext(const CSetEl* pselCur) const
            { return (CSetEl*)pelNext(pselCur); }
        // Next element after current; otherwise NULL, if current is last.
    CSetEl* pselPrev(const CSetEl* pselCur) const
            { return (CSetEl*)pelPrev(pselCur); }
        // Previous element before current; otherwise NULL, if current is first.
    
    CSetEl* pselSearch( const char* pszName, BOOL bWhole = whole ) const;
        // Returns the element whose name begins with pszName,
        // or, if bWhole is CSet::whole, whose entire name matches psz;
        // otherwise, NULL if none match.
        
    CSetEl* pselFindClosestMatch( const char* psz ) const;
        // Returns the element which matches psz, if any. Otherwise, returns the
        // element which would appear immediately before psz (closest < match), if
        // any. Otherwise, returns the element which would appear immediately
        // after psz (closest > match), if any. Otherwise, returns NULL.        

	CSetEl* CSet::pselSearch( CSetEl* psel, const char* psz, BOOL bWhole ) const;
		// This function is for accelerated searches in derived class CFastSet

    CSetEl* pselSearchForWholeSubString( const char* pszName, Length len ) const;
        // Returns the element whose entire name matches the substring
        // beginning at psz of length len (not necessarily null-terminated);
        // otherwise, NULL if none match.

    CSetEl* pselSearchLongest( const char** ppsz ) const;
        // Returns the element whose entire name is the longest match for
        // all possible substrings beginning at *ppsz;
        // otherwise, NULL if none match.
        // Returns via *ppsz the character position in the string
        // immediately after the matched element's name;
        // otherwise leaves it unchanged if none match.

    // Function to assign a unique bit to the newly allocated object.
    BOOL bAllocateSubset(SUBSET_KEY& subsetkey); 
    // Function to make the bit available again.
    void DeAllocateSubset(SUBSET_KEY subsetkey);

    virtual void Add(CSetEl* psel);
    virtual CSetEl* pselRemove(CSetEl* psel);
    virtual void Delete(CSetEl** ppsel) // Delete element, update pel
	    { CDblList::Delete( (CDblListEl**) ppsel ); }
        // Delete the element.
        // Return via *ppsel the next element after the one deleted;
        // otherwise the previous (i.e. deleted the last element);
        // otherwise NULL (i.e. deleted the only element).
    // 1996-03-06 MRP: Make other Delete and Removes unavailable
    // Likewise for InsertBefore and InsertAfter

    virtual BOOL bValidName(const char* pszName, CNote** ppnot);
        // Returns whether pszName has the correct form for a name
        // of an element in this kind of set.
        // Returns a note via *ppnot if pszName doesn't have correct form.
        // Note: Derived set classes can override this function.
    BOOL bValidNewName(const char* pszName, CNote** ppnot);
        // Returns whether pszName has the correct form for a name
        // of an element in this kind of set (using bValidName above) AND
        // whether it is unique with regard to this set's current contents.
        // I.e. this function is the precondition for adding an element.
        // Returns a note via *ppnot if pszName is incorrect or non-unique.

    Str8 sUniqueName(const char* pszBaseName);
        // Return pszBaseName if it is unique with regard to this set's
        // present contents; otherwise return first unique name
        // which can be derived from pszBaseName by appending 2, 3, etc.
        // Note: This function does NOT validate the name, since it expects
        // Shoebox, rather than the user, to provide pszBaseName.

    virtual BOOL bAdd(CSetEl** ppselNew);
        // Let the user add a new element to this set.
        // (Show the same dialog box used to view an existing element.)
        // Returns whether the user clicked OK (vs. Cancel).
        // TRUE (OK): ppselNew returns the new element (an update hint
        //     should have been broadcast to all views).
        // FALSE (Cancel): No element was added to this set.

protected:

    void InsertAfter( CSetEl* pelCur, CSetEl* pelNew );
	// Insert new after current, both must have same key	
    void InsertBefore( CSetEl* pelCur, CSetEl* pelNew );
	// Insert new before current, both must have same key

    void SetElementKey( CSetEl* pel, const char* pszNewKey );
	// Change the element's key and reorder it in this list
	// NOTE: Named differently from CSetEl::SetKey in order to avoid
	// name ambiguity in multiply derived classes (e.g. CMarkerSet)
		
    // Comparison
    int iCompare( const char* s, const char* t,
     	BOOL bWhole, Length* plenMatched ) const;
    int iCompareWholeSubString( const char* s, Length len, const char* t ) const;

	// These functions are for accelerated searches in derived class CFastSet
	CSetEl* pselSearchForWholeSubString( CSetEl* psel,
			const char* psz, Length len ) const;
	CSetEl* CSet::pselSearchLongest( CSetEl* psel, const char** ppsz ) const;	

public:
    void AssertValid() const;
};  // class CSet


// --------------------------------------------------------------------------

// A fast set accelerates searching for elements.
// See CLangEncSet (lng.h) and, before much longer, CMarkerSet (mkr.h).
// Note: Add and Delete are not faster.

class CFastSet : public CSet  // Hungarian: set
{
private:
    CSetEl** m_ppselTable;
    
    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CFastSet(const CFastSet& set);
    CFastSet& operator=(const CFastSet& set);

protected:  
    CFastSet();

public:
    ~CFastSet();
    
    CSetEl* pselSearch( const char* pszName, BOOL bWhole = whole ) const; 
        // Returns the element whose name begins with psz,
        // or, if bWhole is CSet::whole, whose entire name matches psz;
        // otherwise, NULL if none match.
        
    CSetEl* pselSearchForWholeSubString( const char* pszName, Length len ) const;
        // Returns the element whose entire name matches the substring
        // beginning at psz of length len (not necessarily null-terminated);
        // otherwise, NULL if none match.

    CSetEl* pselSearchLongest( const char** ppsz ) const;   
        // Returns the element whose entire name is the longest match for
        // all possible substrings beginning at *ppsz;
        // otherwise, NULL if none match.
        // Returns via *ppsz the character position in the string
        // immediately after the matched element's name;
        // otherwise leaves it unchanged if none match.
    
    virtual void Add(CSetEl* psel);
    virtual CSetEl* pselRemove(CSetEl* psel);
    virtual void Delete(CSetEl** ppsel);
        // Delete the element.
        // Return via *ppsel the next element after the one deleted;
        // otherwise the previous (i.e. deleted the last element);
        // otherwise NULL (i.e. deleted the only element).
    void DeleteAll();

private:
    // Fast table operations
    CSetEl* pselHash(const char* psz) const;
    void SetHash(CSetEl* pselToSet);
    void ClearHash(const char* psz);
    void ClearAll();
    void UpdateTableBeforeRemoving(CSetEl* pselAboutToBeRemoved);
    
#ifdef _DEBUG
public:
    void AssertValid() const;
#endif // _DEBUG
}; // class CFastSet


// **************************************************************************

// Use CSubSet to represent an association of set elements of a set.

// Currently, the only set that makes use of this kind of subset is CMarker
// (CMChar also has subsetting, but it does not use the base-class framework
// because allocation of subsets is administered by the set's owner, not the
// set itself). Some of the marker subsets used are:
// 1. The subset of field markers to be included when exporting
//    a database by each of a set of named export processes.
//    In the visual user interface this is the "All fields" check box
//    and the "Select Fields" button and dialog box.
// 2. The subset of markers used for fields whose text is to be broken in
//    Break and Number Text.
// 3. The subset of markers belonging to a jump path.
// The number of elements in the subset (e.g. markers) is unlimited, and
// because the relationship is implemented using a dynamic bit array, the
// number of objects referring to subsets is also unlimited.
// The benefits of this approach: updates aren't needed when objects
// are added and deleted; testing whether an object is included
// in a particular subset is efficient.

// Here's a diagram of the way these classes are used to represent
// the subset of fields markers to be included when exporting.
// -------------------------              -----------------------
// |                       |<--------     |                     |------
// |       CMarkerSet      |        |     |  CExportProcessSet  |     |
// |                       |------  |     |                     |<--- |
// |  -------------------  |     |  |     |                     |   | |
// |  |      CSet       |  |<--- |  |     |                     |   | |
// |  |      m_psaa     |  |   | |  |     |                     |   | |
// |  -------------------  |   | |  |     -----------------------   | |
// -------------------------   | |  |                               | |
//                             | |  |                               | |
// -------------------------   | |  |                               | |
// |                       |---- |  |                               | |
// |        CMarker        |     |  |     -----------------------   | |
// |                       |<<----  
// |  -------------------  |        |     |                     |---- |
// |  |    CSetEl       |  |        |     |    CExportProcess   |     |
// |  |     m_sma       |  |        |     |                     |<<----
// |  -------------------  |        |     |       m_subset      |
// |                       |        ------------- m_pmkrset     |
// -------------------------              |                     |
//                                        -----------------------
//
// See mkr.h/.cpp and export.h/.cpp for the full details.
// --------------------------------------------------------------------------

// Base class for deriving all kinds of subsets.

class CSubSet  // Hungarian: subset
{
private:
    SUBSET_KEY m_subsetkey;
        // The key that represents this subset in the set's allocation array

protected:
    CSet* m_pset;
        // The set of which this object is a subset
    BOOL m_bIncludeAllElements;
        // E.g. for markers, corresponds to the "All fields" check box.
        // If TRUE, CSubSetEl::m_lBits doesn't need to be examined,
        // and in fact the value for this subset's bit is indeterminate.
    BOOL m_bAutoIncludeNewElements;
        // E.g. Corresponds to "Automatically add newly added markers".
        // If TRUE, a clear bit = included.
        // If FALSE, a set bit = included.

public:    
    CSubSet(CSet* pset, BOOL bIncludeAllElements, BOOL bAutoIncludeNewElements);
    CSubSet(const CSubSet& subset);
    virtual ~CSubSet();

    void operator=(const CSubSet& subset);

    BOOL bIncludesElement(const CSetEl* psel) const;
    void IncludeElement(CSetEl* psel, BOOL bInclude);
    void IncludeSubsetElements(CSubSet &subset);

    BOOL bIncludeAllElements() const { return m_bIncludeAllElements; }
    BOOL bAutoIncludeNewElements() const { return m_bAutoIncludeNewElements; }

    void IncludeAllElements(BOOL bIncludeAll);
    void ExcludeAllElements();
    void AutoIncludeNewElements(BOOL bAutoInclude);
};  // class CSubSet

// **************************************************************************

#endif  // not SET_H
