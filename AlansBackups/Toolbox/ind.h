// ind.h  Interface for index to database records

#ifndef IND_H
#define IND_H

#include "crlookel.h"
#include "cdbllist.h" // Base class for derivation
#include "fil.h"
#include "ptr.h"

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h
class Field_ostream;  // sfstream.h
class SF_ostream;  // sfstream.h
class CProgressIndicator;  // progress.h


// --------------------------------------------------------------------------

// A numbered record element pointer is responsible for keeping
// the current element of a view consistent with its number in the index.

// Background reading:
// "Smart Pointers", pp. 149ff, section 7.4, in _C++ Strategy and Tactics_
// "Proxy", pp. 207-217, in _Design Patterns_

class CNumberedRecElPtr  // Hungarian: prel
{
private:
    CRecLookEl* m_prel;  // a NULL pointer is valid
    long m_lNum;  // The zero-based C-style index of this record element,
        // i.e. the number of record elements prev to this in its index.
        // NOTE: On the status bar the current number is one-based.
    
public:
    void Set(CRecLookEl* prel, long lNum)
        { m_prel = prel; m_lNum = lNum; /* ASSERT( 0 <= m_lNum ); */ }
    void Set(CNumberedRecElPtr nrl)
        { Set(nrl.m_prel, nrl.m_lNum); }

    CNumberedRecElPtr(CRecLookEl* prel=NULL, long lNum=0L)
        { Set(prel, lNum); }
    CNumberedRecElPtr(const CNumberedRecElPtr& nrl)  // copy constructor
        { Set(nrl.m_prel, nrl.m_lNum); }
    const CNumberedRecElPtr& operator=(const CNumberedRecElPtr& pnrl);

    CRecLookEl* operator->() const { return m_prel; }

    operator CRecLookEl*() const { return m_prel; } 

    // operator BOOL() const    { return (m_prel != NULL); }
    BOOL operator<(const CNumberedRecElPtr& nrl) const
        { return( m_lNum < nrl.m_lNum ); }
    BOOL operator>(const CNumberedRecElPtr& nrl) const
        { return( m_lNum > nrl.m_lNum ); }
    BOOL operator==(const CNumberedRecElPtr& nrl) const
        { return (nrl.m_prel == m_prel );}
    BOOL operator!=(const CNumberedRecElPtr& nrl) const { return !(*this == nrl); }
    BOOL operator<=(const CNumberedRecElPtr& nrl) const { return !(*this > nrl); }
    BOOL operator>=(const CNumberedRecElPtr& nrl) const { return !(*this < nrl); }
    long operator-(const CNumberedRecElPtr& nrl) const
        { return( m_lNum - nrl.m_lNum ); }
    
    // CRecLookEl* prel() { return m_prel; }
    long lNum() const { return m_lNum; }
};  // class CNumberedRecElPtr


// ==========================================================================

class CIndex;

// The element is a member of a list consisting of numbered record element
// pointers which have just been inserted into the index set, or are about
// to be deleted from it.

class CNumberedRecElPtrListEl : public CDblListEl  // Hungarian: nrl
{
private:
    CNumberedRecElPtr m_pnrl;
    
    friend class CNumberedRecElPtrList;
    CNumberedRecElPtrListEl(CNumberedRecElPtr& pnrl);

public:
    const CNumberedRecElPtr& prel() const { return m_pnrl; }
    CNumberedRecElPtr& prel() { return m_pnrl; }
    
    const UIdMChar* puchSortKey() const { return prel()->puchSortKey(); }
    
private:
    void IncrementRecElNumber(const CNumberedRecElPtrList& nrllstInserted);
};  // class CNumberedRecElPtrListEl


// --------------------------------------------------------------------------

// The list consists of numbered record element pointers which have just
// been inserted into the index set, or are about to be deleted from it.
// The views which depend on the index set use it to determine whether
// their current record element is obsolete, or whether its number
// is to be incremented or decremented due to insertions or deletions.
 
class CNumberedRecElPtrList : public CDblList  // Hungarian: nrllst
{
public:

    // CNumberedRecElPtrList();
    // RNE  What's the purpose of the constructor?  Don't we start
    // with an empty list anyway?
    ~CNumberedRecElPtrList();

    BOOL bIsMember(const CNumberedRecElPtr prel) const;

    void AddRecElPtrs(CIndex* pind, CRecord* prec);
    // Add every record element that points to this record.
    
    void AddRecElPtr(CNumberedRecElPtr& pnrl);
    
    CNumberedRecElPtr prelFind_RemoveIfFound(UIdMChar* puchSortKey,
            CField* pfldPriKey);
    
    CNumberedRecElPtrListEl* pnrlFirst() const
        { return (CNumberedRecElPtrListEl*)pelFirst(); }
    CNumberedRecElPtrListEl* pnrlNext(const CNumberedRecElPtrListEl* pnrlCur) const
        { return (CNumberedRecElPtrListEl*)pelNext(pnrlCur); }
        
    void Delete(CNumberedRecElPtrListEl** ppnrl)
        { CDblList::Delete((CDblListEl**)ppnrl); }

    void IncrementRecElNumbers(const CNumberedRecElPtrList& nrllstInserted);
        // Increment the number of each record element in this list
        // for every element inserted prev to it in its index.
        // NOTE: We do this when updating the record elements of a record
        // which has been modified because unlike Database Insert Record
        // and Database Delete Record there can be elements _both_
        // inserted _and_ deleted. We increment the record numbers of
        // the elements to be deleted according to those inserted.
        // And we increment the view's current record elements
        // according to those inserted before decrementing them according to
        // those being deleted.
        // 1995-10-31 MRP: MAKE SURE THIS IS CORRECT!
        
#ifdef _DEBUG
    void AssertValid() const;
#endif  // _DEBUG
};  // class CNumberedRecElPtrListEl


// RNE this forward ref will no longer be needed when we dispense with the class.
class CIndexRecordRefList;  // Forward reference-- see below
//class CNumberedRecElPtrList;
class CKey; // Forward reference

// **************************************************************************

class CIndex : public CDblList, public CSetEl  // Hungarian: ind
{
private:
    CMarkerPtr m_pmkrPriKey;
    CSortOrderPtr m_psrtPriKey;
    BOOL m_bSortPriKeyFromEnd;
    CMarkerRefList m_mrflstSecKeys;
    Length m_maxlenSortKeyComponent;

    // Filter properties: NULL if not using a filter
    CFilterPtr m_pfil; 
    CIndex* m_pindUnfiltered;
    
    long m_lRecEls;

    CRefCount m_ref;  // number of views referring to this
        
    Length m_maxsizSortKey;
    UIdMChar* m_puchSortKeyA;
    UIdMChar* m_puchSortKeyB;

    CNumberedRecElPtrList m_nrllstInserted;
    CNumberedRecElPtrList m_nrllstToBeDeleted;
    
    CField* m_pfldSubstituteForNullPriKey;  // Used by CFieldIterator

    BOOL m_bNotIncludeUndefined;  // 1997-12-18 MRP
    BOOL m_bModifiedSinceFiltered; // 2000-04-27 TLB

#ifdef _DEBUG
    BOOL m_bHasBeenSorted;  // 2000-06-21 MRP
#endif
    
    friend class CIndexSet;
    CIndexSet* m_pindsetMyOwner;

    // An index is constructed by the set which owns it.
    CIndex(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSec,
            Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
            CIndexSet* pindsetMyOwner,
            CFilter* pfil = NULL, CIndex* pindUnfiltered = NULL);
    static BOOL s_bReadProperties(Object_istream& obs,
            CIndexSet* pindsetMyOwner, CIndex** ppind, CIndex** ppindUnfiltered);

public:
    ~CIndex();  // Destructor
        // Each index destructor deletes its record _elements_.
        // The index set destructor deletes the *records* themselves.

    CSet* CIndex::psetMyOwner();
	
#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
#ifdef prjWritefstream // 1.6.4aa 
    void WriteRecordRef(Object_ofstream& obs, const char* pszQualifier,
            const CNumberedRecElPtr& prel);
#else
    void WriteRecordRef(Object_ostream& obs, const char* pszQualifier,
            const CNumberedRecElPtr& prel);
#endif
    BOOL bReadRecordRef(Object_istream& obs, const char* pszQualifier,
            CNumberedRecElPtr& prel);
         
    BOOL bEqual(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
            CFilter* pfil);
        // Return whether this index has the specified sorting properties.

    // Filter operations.
    CFilter* pfil() const   { return m_pfil; }
    BOOL  bUsingFilter() const { return ( m_pfil != NULL ); }
    
    void  SetFilter(CFilter* pfil, BOOL bUseFilter, CRecLookEl** pprelCur);
    
    BOOL bIsMatch(const CRecLookEl* prelCur);
        // Always returns TRUE if filter not in use
        // 1995-06-30 MRP: prelCur cannot be null.

    
    long lNumRecEls() const { return m_lRecEls; }
    long lNumRecElsToBeDeleted() const { return m_nrllstToBeDeleted.lGetCount(); }
    CMarker* pmkrPriKey() const { return m_pmkrPriKey; }
    CSortOrder* psrtPriKey() const { return m_psrtPriKey; }
    CMarkerRefList* pmrflstSecKeys() { return &m_mrflstSecKeys; }
    Length maxlenSortKeyComponent() const { return m_maxlenSortKeyComponent; }
    Length maxsizSortKey() const { return m_maxsizSortKey; }
    BOOL bSortPriKeyFromEnd() const { return m_bSortPriKeyFromEnd; }
    const CField* pfldSubstituteForNullPriKey() const
        { return m_pfldSubstituteForNullPriKey; }
    CIndexSet* pindsetMyOwner() { return m_pindsetMyOwner; }
    BOOL bReadOnly() const;
    
    CNumberedRecElPtr prelInsertRecord(const Str8& sKeyToAdd);
        // Return the record element corresponding to precToAdd in this index.
        // Add the record to _all_ indexes in the same index list as this.
        // NOTE: The index list becomes the owner of precToAdd.

    CNumberedRecElPtr prelCopyRecord(const CRecord *precSrc);
        // Inserts a copy of precSrc into this.

    BOOL bUpdateSortKeys(const CNumberedRecElPtr& prelCur);
        // Update the sort keys of the record elements for the current
        // record in all indexes.
        // Returns whether the record element of the active view prelCur
        // is to be deleted. 
        // Returns the lists of record elements inserted and to be deleted
        // because of the modifications to the record's fields.

    CNumberedRecElPtr prelDifferentRecord(
                const CNumberedRecElPtr& prelCur) const;
        // Return the nearest matching record in the next direction
        // that refers to a different record from that of prelCur;
        // otherwise, return the nearest such in the previous direction;
        // otherwise, return NULL.
        // Used to get off a record about to be deleted.

    BOOL bRecElUpdated(CNumberedRecElPtr* pprelCur) const;
        // Return whether the current record element is being deleted
        // because the contents of the record has been modified and
        // if so change it to another record element to the same record.
        
    CNumberedRecElPtr CIndex::prel_UpToDate(const CNumberedRecElPtr& prelCur) const;
        // Return the current or nearest element, but not one on the deleted list.
    CNumberedRecElPtr prelNext_UpToDate(const CNumberedRecElPtr& prelCur) const;
        // Return the next element, but not one on the deleted list.
    CNumberedRecElPtr prelPrev_UpToDate(const CNumberedRecElPtr& prelCur) const;
        // Return the previous element, but not one on the deleted list.
        
    CNumberedRecElPtr prelSame_UpToDate(const CNumberedRecElPtr& prelCur) const;
        // Return a record element referring to prelCur's record,
        // but not one on the deleted list.
        
    CNumberedRecElPtr prelCorrespondingTo(const CRecLookEl* prelCur);

    void IncrementRecElNumber(CNumberedRecElPtr* ppnrl);
        // Increment the number of a view's record element pointer
        // for every element inserted prev to it in its index.
    void DecrementRecElNumber(CNumberedRecElPtr* ppnrl);
        // Decrement the number of a view's record element pointer
        // for every element about to be deleted prev to it in its index.


    // Compare sort keys at the match setting:
    int iCompare(const CRecLookEl* prelA, const CRecLookEl* prelB,
			MatchSetting matset = CMCharOrder::matsetEvenIgnore,
            BOOL bMatchWhole = TRUE) const;

    int iCompare(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
            MatchSetting matset = CMCharOrder::matsetEvenIgnore,
            BOOL bMatchWhole = TRUE) const;
        // returns negative if A < B; zero if A == B, positive if A > B.
        // Whole match requires equivalent lengths for equality; otherwise
        // (i.e. partial match) A is permitted to match a prefix of B.
        // E.g. whole match: "ab" < "abc"; partial match: "ab" == "abc".
        
    BOOL bSort();
    BOOL bMarkerUpdateRequiresReSorting(CMarkerUpdate& mup);
    BOOL bLangEncUpdateRequiresReSorting(CLangEncUpdate& lup);
    BOOL bSortOrderUpdateRequiresReSorting(CSortOrderUpdate& sup);
    BOOL bMarkerUpdateModifiesDatabase(CMarkerUpdate& mup);

    BOOL bFilterUpdateRequiresReFiltering(CFilterUpdate& fup);
    BOOL bModifiedSinceFiltered() const { return m_bModifiedSinceFiltered; }
    void ReFilter(); // 2000-04-27 TLB - No longer private

    BOOL bSortKey_PriOnly(const char* pszKey, UIdMChar* puchSortKeyBuf);
    BOOL bEqual_PriOnly(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                    MatchSetting matset, BOOL bMatchWhole) const;
    
    BOOL bUsesMarker(const CMarker* pmkr);
    
    CNumberedRecElPtr prelFind(CRecord* prec); // Find this record in this index.
// 2000/04/28 TLB & MRP - Removed this function. Using nearly identical function (with better name): prelCorrespondingTo
//    void ReNumberElement(CNumberedRecElPtr* pprel);

    // Help keep track of reference counts.
    void IncrementNumberOfRefs() { m_ref.IncrementNumberOfRefs(); }
    void DecrementNumberOfRefs();
        // Delete any index --except the "record owning" index--
        // when no more record sets (i.e. windows) refer to it.
    
    CNumberedRecElPtr pnrlFirst() const;
    CNumberedRecElPtr pnrlLast() const;
    CNumberedRecElPtr pnrlNext(const CNumberedRecElPtr& pnrlCur) const;
    CNumberedRecElPtr pnrlPrev(const CNumberedRecElPtr& pnrlCur) const;
    
    void WriteElements(Field_ostream& sfs, CDatabaseType* ptyp, CProgressIndicator& prg); // 1.4qpb
    void WriteElement(Field_ostream& sfs, CDatabaseType* ptyp, const CRecLookEl* prel) const; // 1.4qpb

    void WriteElements(Field_ostream& sfs, CDatabaseType* ptyp, CProgressIndicator& prg, // 1.4qpb
            const CMarkerSubSet& subsetMarkersToExport,
            BOOL bPriKeyFieldFirst, BOOL bMDF, BOOL bAddAlphDiv );
    void WriteElement(Field_ostream& sfs, CDatabaseType* ptyp, const CRecLookEl* prel, // 1.4qpb
            const CMarkerSubSet& subsetMarkersToExport,
            BOOL bPriKeyFieldFirst, BOOL bMDF) const;
	BOOL bUnicodeSequenceSort() const { return m_psrtPriKey->bUnicodeSequenceSort(); } // 1.4eg Add Unicode sequence sort

private:
    CNumberedRecElPtr prelAddRecord(CRecord* prec);
    void AddRecEl_CopyFrom(const CRecLookEl* prel);
    CNumberedRecElPtr prelAddRecEl(const UIdMChar* puchSortKey,
            CRecord* prec, CField* pfld);

    CNumberedRecElPtr prelInsertRecord(CRecord* prec, BOOL bIgnoreTheFilter);
    CNumberedRecElPtr prelInsertRecEl(const UIdMChar* puchSortKey,
            CRecord* prec, CField* pfld);
    CNumberedRecElPtr prelInsertRecEl(CRecLookEl* prel);

    void RecordToBeDeleted(CRecord* prec);

    void RemoveRecord(CRecord* prec);
    void DeleteRecEl(CRecLookEl* prelToDelete);
    
    void DoneUpdatingViews();

    BOOL bSorted() const;   
    void SortFrom(const CIndex* pindRecordOwner);
    void MarkerUpdated(CMarkerUpdate& mup);
    void FilterUpdated(CFilterUpdate& fup);
    void LangEncUpdated(CLangEncUpdate& lup);
    void SortOrderUpdated(CSortOrderUpdate& sup);
    void ReSort();

    // Returns: TRUE == key is complete (not truncated.
    //          FALSE == key was truncated.
    BOOL bSortKey(const CRecord* prec, CField* pfld, 
            UIdMChar* pszSortKeyBuf);
    int iCompare_PriOnly(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                    MatchSetting matset, BOOL bMatchWhole) const;

public:
	// Overrides of First, Next, etc. needed for type safe return value
	CRecLookEl* prelFirst() const // First element
		{ return (CRecLookEl*) CDblList::pelFirst(); }
	CRecLookEl* prelLast() const // Last element
		{ return (CRecLookEl*) CDblList::pelLast(); }
	CRecLookEl* prelNext( const CRecLookEl* pelCur ) const // Next element after current
		{ return (CRecLookEl*) CDblList::pelNext( pelCur ); }
	CRecLookEl* prelPrev( const CRecLookEl* pelCur ) const // Previous element after current
		{ return (CRecLookEl*) CDblList::pelPrev( pelCur ); }

protected:  
    // Override of Delete needed because ** won't do automatic cast
	void Delete( CRecLookEl** ppel ) // Delete element, update pel
		{ CDblList::Delete( (CDblListEl**) ppel ); }

public:


    enum { eCancel=0, eSearch, eInsert }; // return codes for iSearch() and iInsertRecord()

    // Operations corresponding to items on the Database menu       
    // BOOL bInsertRecord(CRecLookEl** pprelInserted);
    int iInsertRecord(CKey* pkey, CNumberedRecElPtr* pprelInserted);

    // BOOL bSearch(CKey* pkey, CRecLookEl** pprelCur,  long *plRecNumCur);
    int iSearch(CKey* pkey, CNumberedRecElPtr* pprelCur, BOOL bFilter); // 1.5.5b Fix bug of insert duplicate when filtered
        // Database Search... dialog box for the key and match settings.
    int iSearchFocusTarget(CKey* pkey, CNumberedRecElPtr* pprelCur, BOOL bMatchWhole, BOOL bMultMatchMessage = TRUE );
        // 1998-10-13 MRP: Don't show Database Search dialog box.

#ifdef _DEBUG
    void AssertValid() const;

    int  iRefCount() const  { return (int)m_ref.numRefs(); }
#endif  // _DEBUG
};  // class CIndex


// --------------------------------------------------------------------------

class CLangEncType;
class CDatabaseType;
class CShwDoc;
class CShwView;

class CIndexUpdate : public CUpdate  // Hungarian: iup
{
private:
    CIndexSet* m_pindset;
    CRecord* m_prec;
    // Insert, Delete, Validate
    int m_iType; // Type of update, as defined below
        #define INDEX_UPDATE_DELETE 1
        #define INDEX_UPDATE_ADD    2
        #define INDEX_UPDATE_MODIFY 3 // Must be AND of previous two
        
public:
    CIndexUpdate( CIndexSet* pindset, CRecord* prec, int iType ) // Constructor
        { m_pindset = pindset; m_prec = prec; m_iType = iType; }
    
    CRecord* prec() const { return m_prec; }
    CIndexSet* pindset() const { return m_pindset; }
    int iType() const { return m_iType; }
     
    // NOTE: Implementated in update.cpp rather than in mkr.cpp
    virtual void UpdateLangEnc(CLangEnc* plng);
    virtual void UpdateDatabaseType(CDatabaseType* ptyp);
    virtual void UpdateShwDoc(CShwDoc* pdoc);
    virtual void UpdateShwView(CShwView* pview);

};  // CIndexUpdate

// --------------------------------------------------------------------------

class CFilterSet;   // Forward ref.

class CDatabaseType;

class CIndexSet : public CSet  // Hungarian: indset
{
private:
    CDatabaseTypePtr m_ptyp;
    CIndexPtr m_pindRecordOwner;
        // The index whose sorting properties determine the order in which
        // the database's records are to be saved to a file.
    CFieldList m_fldlstToDelete; // list of fields to be deleted when indices are all updated
    CDblList m_rellstObsolete;  // made so by resorting or refiltering  
    CRecord* m_precToBeDeleted;
    int m_iMargin; // in pixels, width to wrap lines at
    BOOL m_bReadOnly;
    BOOL m_bDateStampHasFourDigitYear;  // 1998-04-21 MRP
    BOOL m_bNotIncludeUndefined;  // 1997-12-18 MRP
	Str8 m_sHeadInfo; // 1.4pca Add head info var to index set

public:
    CIndexSet(CDatabaseType* ptyp, int iMargin, BOOL bReadOnly,
            BOOL bNotIncludeUndefined = FALSE);  // 1997-12-18 MRP
        // Constructor used by File Open (Shw database file) and
        // called via s_bReadProperties below.
    ~CIndexSet();  // Destructor-- deletes the _records_ too
    
    void Write(SF_ostream& sfs, CProgressIndicator& prg); //  const; // 1.4qpb
    void WriteSh3Header(SF_ostream& sfs) const;

    static BOOL s_bReadPropertiesFromSh3Header(istream& ios,
            BOOL bReadOnly, CIndexSet** ppindset, BOOL* pbStringTooLong,
            Str8& sMissingDBType);
        // Read the Shoebox database header \_sh field from sfs.
        // If there is a header in the proper format, construct and return
        // via ppindlst the corresponding index list, which consists of
        // an owning index into which the records will be added.
        // Otherwise, return FALSE and add a note to the note list pnotlst.
        // Used by File Open (see also AddRecord below).
        // 07-26-1997 bStringToLong will be true if the size 
        // of the first line is bigger than 32765 Bytes.
        // 2-12-98 sMissingDBType will be set if the db type is not available
        
#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs);
#else
    void WriteProperties(Object_ostream& obs);
#endif
        // Write the properties of the index which owns the records.
        // Used by File Save to write the Shw database header.
//  static BOOL s_bReadProperties(Object_istream& obs, CDatabaseType* ptyp,
//          CIndexSet** ppindset);
        // Read the properties of the index which is to own the records.
        // Used by File Open to read the Shw database header.
    BOOL bReadIndexProperties(Object_istream& obs,
            CIndexPtr& pind, CIndexPtr& pindUnfiltered);
        // Read the sorting properties of a particular record window
        // and return via pind an appropriate index.
    
    static BOOL s_bViewIndexProperties(CIndex** ppind, CIndex** pindUnfiltered);
        // Shows in a dialog box the sorting properties of *ppind and
        // returns via ppind an index with the properties the user selected.
        // Returns FALSE if the user chose Cancel.
    static BOOL s_bViewFilteringProperties(CIndex** pind, CIndex** pindUnfiltered);
        // Database Filtering... dialog box for selecting a filter.
        
    void AddRecord(CRecord* prec);
        // Add the record to all indexes (used by File Open).
    BOOL bSort();

    void DeleteField( CNumberedRecElPtr prel, CField* pfld ) // used by editor. Add pfld to list of fields to be deleted when index set is freshened
        { m_fldlstToDelete.Add( prel->prec()->pfldRemove( pfld ) ); }

    CDatabaseType* ptyp();
    CMarkerSet* pmkrset();
    CFilterSet* pfilset();
    int iMargin() { return m_iMargin; }
    void SetMargin( int iMargin ) { m_iMargin = iMargin; }
    BOOL bReadOnly() const { return m_bReadOnly; }
//    void SetReadOnly( BOOL b ) { m_bReadOnly = b; } // 1.1ea Clear read-only on save as. Didn't work. Change to not read-only caused asset of number of references on close.
    BOOL bDateStampHasFourDigitYear() const { return m_bDateStampHasFourDigitYear; }
    
    CMarker* pmkrRecord();
    CIndex* pindRecordOwner() { return m_pindRecordOwner; }

    CIndex* pindFirst() const   { return (CIndex*)pselFirst(); }
    CIndex* pindNext(const CIndex* pind) const { return (CIndex*)pselNext(pind); }
    CIndex* pindPrev(const CIndex* pind) const { return (CIndex*)pselPrev(pind); }
    CIndex* pindLast() const    { return (CIndex*)pselLast(); }  

    CIndex* pindFind_AddIfNew(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bRightToLeft,
            CFilter* pfil=NULL);
        // Return an index from the list having the specified properties.
        // Add such an index to the list if it does not already exist.
    void GetIndex(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bRightToLeft,
            CFilter* pfil, CIndex** ppind, CIndex** ppindUnfiltered);
    void GetIndex(CFilter* pfil, CIndex** ppind, CIndex** ppindUnfiltered);

    BOOL bNotIncludeUndefined() const { return m_bNotIncludeUndefined; }  // 1997-12-18 MRP

    void Delete(CIndex* pind) { CDblList::Delete(pind); }

    void RecordToBeDeleted(CRecord* prec);
    void MoveElementsFrom(CIndex* pind);
    
    BOOL bUpdateSortKeys(const CNumberedRecElPtr& prelCur);

    void MarkerUpdated(CMarkerUpdate& mup);
    void FilterUpdated(CFilterUpdate& fup);
    void LangEncUpdated(CLangEncUpdate& lup);
    void SortOrderUpdated(CSortOrderUpdate& sup);
    void DoneUpdatingViews();

    BOOL bUsesMarker(const CMarker* pmkr);

	void SetHeadInfo( Str8 s ) { m_sHeadInfo = s; } // 1.4pca Add head info var to index set
	Str8 sGetHeadInfo() { return m_sHeadInfo; } // 1.4pe Add head info access in index set
	const char* pszHeadInfo() { return m_sHeadInfo; } // 1.4pca Add head info var to index set

#ifdef _DEBUG
    void AssertValid() const;
#endif  // _DEBUG
};  // class CIndexSet


// ==========================================================================

class CIndexRecordRef : public CDblListEl  // Hungarian: rrf
{
private:
    CIndex* m_pind;
    CRecLookEl* m_prel;
    
    friend class CIndexRecordRefList;
    CIndexRecordRef(CIndex* pind, CRecLookEl* prel);

public:
    CIndex* pind() { return m_pind; }
    CRecLookEl* prel() { return m_prel; }
    const UIdMChar* puchSortKey() const { return m_prel->puchSortKey(); }
};  // class CIndexRecordRef


// --------------------------------------------------------------------------

class CIndexRecordRefList : public CDblList  // Hungarian: rrflst
{
public:
    CIndexRecordRefList();

    void AddIndexRecordRefs(CIndex* pind, CRecord* prec);
    BOOL bIsMember(const CRecLookEl* prel) const;   
    CRecLookEl* prelFind_RemoveIfFound(UIdMChar* puchSortKey,
            CField* pfldPriKey);
    
    CIndexRecordRef* prrfFirst() const
        { return (CIndexRecordRef*)pelFirst(); }
    CIndexRecordRef* prrfNext(const CIndexRecordRef* prrfCur) const
        { return (CIndexRecordRef*)pelNext(prrfCur); }
        
    void Delete(CIndexRecordRef** pprrf)
        { CDblList::Delete((CDblListEl**)pprrf); }
};  // class CIndexRecordRefList

// **************************************************************************

#endif  // IND_H
