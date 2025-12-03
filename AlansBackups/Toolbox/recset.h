// recset.h  Interface for sets of records  (1995-04-18)


#ifndef RECSET_H
#define RECSET_H

#include "ind.h"  // class CIndex


// **************************************************************************
     
// A set of records combines
// 1. a record source, i.e. a reference to a database index, which provides
//    references to the records, in the order determined by its sort order;
// and, optionally (in derived CSearchRecordSet),
// 2. a selection criterion (or match condition), i.e.
//    A. a reference to a database record filter and filter match settings;
//    B. a search key string.
//	  C. match settings

// It behaves as if it were a database ordered according to the sort order,
// consisting only of the records --possibly none-- matching the criterion.
// It encapsulates record-handling behavior for Record and Browse windows,
// as well as File Output "Records to include".

// NOTE: Unlike most other "sets", e.g. CFilterSet, a record set
// does *not* own its "elements". (Notice that it is not derived from CLook.)
// Instead, it obtains references to elements by "delegation" to the index.
// Therefore, deleting a recset instance does *not* delete any records.

// For further details, see the classes: CIndex, CRecLook, and CDblList.

class CRecordSet  // abstract base class; Hungarian: recset
{
protected:  
    CIndex* m_pind;  // record source

    CRecordSet(CIndex* pind);
    CRecordSet(const CRecordSet& recset);  // copy constructor
        // To duplicate settings when Window New constructs a new CShwView

    void  SetIndex(CIndex* pind, CRecLookEl** pprelCur);
        // Change the index to pind and update the index reference counts.
        // Return via pprelCur the first record element in pind
        // which represents the same record it had referenced. 
    CIndexSet* pindset() { return m_pind->pindsetMyOwner(); }

public:
    virtual ~CRecordSet();  // destructor-- does not delete the records

    CIndex* pind() const { return m_pind; }

    virtual BOOL bIsMatch(const CRecLookEl* prelCur) const = 0;
        // Returns whether the record matches the selection criterion.

    // Return NULL if database is empty, or no records match filter in use
    // Member functions for record movement.
    virtual CRecLookEl* prelFirst() const;  
    virtual CRecLookEl* prelLast() const;
    virtual CRecLookEl* prelPrev(const CRecLookEl* prelCur) const;
    virtual CRecLookEl* prelNext(const CRecLookEl* prelCur) const;  
        // Return NULL if current record is respectively the first or last
        // matching the filter in use

    // MRP: would it be helpful to define the following for OnUpdateRecord...?
    // BOOL bIsFirst( const CRecLookEl* prelCur ) const;    
    // BOOL bIsLast( const CRecLookEl* prelCur ) const; 
        // Return whether the current record is respectively the first or last
        // matching the filter in use
    
    // NOTE: Use the Add, InsertBefore, and Delete functions of the database.

private:    
    virtual CRecLookEl* prelMatching_Next(const CRecLookEl* prelCur) const;
        //
    virtual CRecLookEl* prelMatching_Prev(const CRecLookEl* prelCur) const;
        //
        
public:
#ifdef _DEBUG
    virtual void AssertValid() const = 0;
#endif  // _DEBUG
};  // class CRecordSet


// --------------------------------------------------------------------------

class Object_ostream;  // obstream.h
class Object_istream;  // obstream.h

// A key contains a string to match, plus some match criteria
class CKey  // Hungarian: key
{
private:
    Str8 m_sKey;
    BOOL m_bMatchWhole;
    MatchSetting m_matset;

public:
    CKey(const char* pszKey = "", BOOL bMatchWhole = FALSE,
            MatchSetting matset = CMCharOrder::matsetSamePrimaryGroup);
    CKey(const CKey& key);  // copy constructor
    ~CKey() {}
    
#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs, const char* pszQualifier=NULL) const;
#else
    void WriteProperties(Object_ostream& obs, const char* pszQualifier=NULL) const;
#endif
    BOOL bReadProperties(Object_istream& obs, const char* pszQualifier=NULL);
    
    const Str8& sKey() const { return m_sKey; }
    BOOL bMatchWhole() const { return m_bMatchWhole; }
    MatchSetting matset() const { return m_matset; }
    
    void SetKey(const char* pszKey, BOOL bMatchWhole, MatchSetting matset);
    void SetKey(const CKey& key);
    void SetKeyText(const char* pszKey) { m_sKey = pszKey; } // set key text only
    void SetKeyCriteria(BOOL bMatchWhole, MatchSetting matset); // set key matching criteria only
    void SetKeyCriteria(const CKey& key); // set key matching criteria only

    BOOL bModalView(int iCommand, ID idHelp, CMarker* pmkr);
};  // class CKey

// --------------------------------------------------------------------------

class CMultipleMatchList; // forward ref
class CDatabaseRef;

class CSearchRecordSet : public CRecordSet  // Hungarian: recset
{
private:
    Str8 m_sKey; // 1.4gy Store key string in search record set
    UIdMChar* m_puchSortKey;
    MatchSetting m_matset;
    BOOL m_bMatchWhole;

public:
    CSearchRecordSet(CIndex* pind, CKey* pkey);
    virtual ~CSearchRecordSet(); // destructor-- does not delete the records

    BOOL bSearch(CMultipleMatchList* pmmtlst, CDatabaseRef* pdbref);

    virtual BOOL bIsMatch(const CRecLookEl* prelCur) const;
        // Returns whether the record matches the selection criterion.
	void SetMatchWhole( BOOL bMatchWhole ) { m_bMatchWhole = bMatchWhole; }
	void SetMatchSetting( MatchSetting matset ) { m_matset = matset; }
	Str8 sKey() const { return m_sKey; } // 1.4gy Store key string in search record set

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG
};  // class CSearchRecordSet

// **************************************************************************

#endif  // RECSET_H
