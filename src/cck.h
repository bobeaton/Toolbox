// cck.h  Consistency Checks

#ifndef CCK_H
#define CCK_H

#include "obstream.h"
#include "mkr.h"
#include "crngset.h"  // CRangeSet

// --------------------------------------------------------------------------

class CAssociationEl : private CSetEl { // Index element, derived from doubly linked list element base class
friend class CAssociationSet;
private:
    Str8 m_sAssociation;
    CAssociationEl(const Str8& sLookup, const Str8& sAssociation); // *** Constructor with content
    void CascadeAssociation(const Str8& sOldAssociation, const Str8& sNewAssociation);
    BOOL bMatch(const Str8& sLookup) const;
    BOOL bMatch(const Str8& sLookup, Str8& sAssociation) const;
    CSet* psetMyOwner() { return NULL; }  // Required for CSet::ChangeNameTo

}; // class CAssociationEl

//------------------------------------------------------------------------

class CAssociationSet : private CSet { // Association Set class, derived from lookup base class

public:
    CAssociationSet() : // Empty set constructor
        CSet() {}
    enum EAddReplaceAllResult { araCallerError, araCircularReplacement, araSystemError, araAdded };
    EAddReplaceAllResult araAddAssociation(const Str8& sLookup, Str8 sAssociation);
    void DeleteAll()
        { CSet::DeleteAll(); }
    BOOL bSearch(const char* pszLookup) const // Perform a search
        { return ( CSet::pselSearch( pszLookup, whole ) != NULL ); }
    BOOL bLookup(const char* pszLookup, Str8& sAssociation) const; // Perform a search and retrieve association

private:
    // Overrides of First, Next, etc. needed for type safe return value
    CAssociationEl* paseFirst() // First element
        { return (CAssociationEl*) m_pelFirst; }
    CAssociationEl* paseLast() // Last element
        { return (CAssociationEl*) m_pelLast; }
    CAssociationEl* paseNext(const CAssociationEl* paseCur) // Next element after current
        { return (CAssociationEl*) pselNext(paseCur); }
    
}; // class CAssociationSet

// --------------------------------------------------------------------------

class CShwView;  // shwview.h
class CIndex;  // ind.h
class CNumberedRecElPtr;  // ind.h
class CField;  // cfield.h
class CCheckingState; // Forward declaration

class CFieldConsistencyChecker  // Hungarian: cck
{
protected:
    CShwView* m_pviewMyOwner;

    friend class CCheckConsistencyDlg;  // cck_d.h
    BOOL m_bShowProperties;
    BOOL m_bSingleField;
    Str8 m_sSingleFieldMarker;
    BOOL m_bSelectedFields;
    CMarkerSubSet m_subsetFieldsToCheck;

#ifdef RECORD_ORDER_CHECKING_ENABLED
    BOOL m_bRecordByRecord;
#endif
    BOOL m_bCheckDataDefn;
    BOOL m_bCheckRangeSets;
    BOOL m_bCheckRefConsistency;

    CCheckingState* m_pcks;

public:
    CFieldConsistencyChecker(CShwView* pviewMyOwner);
    virtual ~CFieldConsistencyChecker();

    // Read-only member access functions
    CShwView* pviewMyOwner() const { return m_pviewMyOwner; }
    BOOL bCheckInProgress() const { return (m_pcks)? TRUE: FALSE; }
    BOOL bShowProperties() const { return m_bShowProperties; }
    BOOL bSingleField() const { return m_bSingleField; }
    Str8 sSingleFieldMarker() const { return m_sSingleFieldMarker; }
    BOOL bSelectedFields() const { return m_bSelectedFields; }
    const CMarkerSubSet* psubsetFieldsToCheck() const { return &m_subsetFieldsToCheck; }
    BOOL bCheckDataDefn() const { return m_bCheckDataDefn; }
    BOOL bCheckRangeSets() const { return m_bCheckRangeSets; }
    BOOL bCheckRefConsistency() const { return m_bCheckRefConsistency; }
    
#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CShwView* pviewMyOwner, CFieldConsistencyChecker** ppcck);

    BOOL bCheckConsistency(CIndex* pind, CNumberedRecElPtr pnrl, CField* pfld, BOOL bContinueCheckInProgress = TRUE);
    BOOL bRecheckConsistency(CIndex* pind);
    BOOL bCheckConsistencyBeforeExporting(CIndex* pind);

private:
	void Init();
    void ReadProperties( Object_istream& obs );
};  // class CFieldConsistencyChecker

// --------------------------------------------------------------------------

class CCheckingState  // Hungarian: cks
{
protected:
    CFieldConsistencyChecker* m_pcckMyOwner;
    CShwView* m_pview;
    Str8 m_sFileName;

#ifdef RECORD_ORDER_CHECKING_ENABLED
    BOOL m_bRecordByRecord;
#endif

    long m_nInconsistenciesSkipped;
    BOOL m_bCancelledAfterInconsistencyFound;

    CCheckingState(CFieldConsistencyChecker* pcckMyOwner);

public:
    virtual ~CCheckingState();

    // TLB 08-12-1999
    BOOL bCheckingDatabase() const { return TRUE; }
    BOOL bCheckDataDefn() const { return m_pcckMyOwner->bCheckDataDefn(); }
    BOOL bCheckRangeSets() const { return m_pcckMyOwner->bCheckRangeSets(); }
    BOOL bCheckRefConsistency() const { return m_pcckMyOwner->bCheckRefConsistency(); }
    //virtual BOOL bSkipAllRangeSetWhiteSpace() const = 0;
    //virtual void SkipAllRangeSetWhiteSpace() = 0;
    virtual BOOL bSkipAllElement(const CMarker* const pmkr, const Str8& s) const = 0;
    virtual void AddSkipAllElement(const CMarker* const pmkr, const Str8& s) = 0;
    virtual BOOL bReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) const = 0;
    virtual CAssociationSet::EAddReplaceAllResult araAddReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) = 0;

    virtual BOOL bCheckConsistency(CIndex* pind,
            CNumberedRecElPtr pnrl, CField* pfld) = 0;

    void IncrementInconsistenciesSkipped();
    BOOL bCancelledAfterInconsistencyFound() const;

protected:
    BOOL bMarkerToCheck(CMarker* pmkr);
    BOOL bNextField(CIndex* pind,
            CNumberedRecElPtr& pnrl, CField*& pfld);
        // Pre-increment and then test (for use in do-while loops)
    void CancelledWhileChecking(CNumberedRecElPtr& pnrl);
};  // class CCheckingState

// --------------------------------------------------------------------------

class CMarkerOrderCheckingState : public CCheckingState  // Hungarian: cks
{
private:
    CMarkerSet* m_pmkrset;
    CDataItemSet m_datsetSkipAll;
    BOOL m_bSkipAllMissingData;
    CAssociationSet m_asetReplaceAll;
    Str8 m_sInconsistencyMarker;

public:
    CMarkerOrderCheckingState(CFieldConsistencyChecker* pcckMyOwner);
    ~CMarkerOrderCheckingState();

    // TLB 08-12-1999
    // BOOL bSkipAllRangeSetWhiteSpace() const;
    // void SkipAllRangeSetWhiteSpace();
    BOOL bSkipAllElement(const CMarker* const pmkr, const Str8& s) const;
    void AddSkipAllElement(const CMarker* const pmkr, const Str8& s);
    BOOL bReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) const;
    CAssociationSet::EAddReplaceAllResult araAddReplaceAllElement(const CMarker* const pmkr,
                                                                  const Str8& s, Str8& sReplacement);

    BOOL bCheckConsistency(CIndex* pind, CNumberedRecElPtr pnrl, CField* pfld);

private:
    void CancelledAfterInconsistencyFound(CMarker* pmkr,
            CNumberedRecElPtr& pnrl);
    BOOL bNextMarker(CMarker*& pmkr);
        // Pre-increment and then test (for use in do-while loops)
    BOOL bResetToBeginning(CIndex* pind, CNumberedRecElPtr& pnrl, CField*& pfld);
};  // class CMarkerOrderCheckingState

// --------------------------------------------------------------------------
#ifdef RECORD_ORDER_CHECKING_ENABLED
class CRecordOrderCheckingState : public CCheckingState  // Hungarian: cks
{
public:
    CRecordOrderCheckingState(CFieldConsistencyChecker* pcckMyOwner);
    ~CRecordOrderCheckingState();

    // TLB 08-12-1999
    // BOOL bSkipAllRangeSetWhiteSpace() const;
    // void SkipAllRangeSetWhiteSpace();
    BOOL bSkipAllElement(const CMarker* const pmkr, const Str8& s) const;
    void AddSkipAllElement(const CMarker* const pmkr, const Str8& s);
    BOOL bReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) const;
    void AddReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement);

    BOOL bCheckConsistency(CIndex* pind, CNumberedRecElPtr pnrl, CField* pfld);

private:
    BOOL bFieldToCheck(CField* pfld);
    void CancelledAfterInconsistencyFound(CNumberedRecElPtr& pnrl);
};  // class CRecordOrderCheckingState
#endif // RECORD_ORDER_CHECKING_ENABLED

#endif  // ifndef CCK_H
