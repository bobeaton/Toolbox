// ind.cpp  Implementation of index to database records

#include "stdafx.h"
#include "toolbox.h"
#include "ind.h"
#include "typ.h"
#include "obstream.h"  // classes Object_istream, Object_ostream
#include "sfstream.h"  // class Field_ostream. SF_ostream
#include "progress.h"
#include "recset.h"  // class CKey
#include "srt.h"
#include "project.h" // needed for Shw_ptypset()
#include <stddef.h>  // ptrdiff_t

#include "dlgsort.h"
#include "dlgrcins.h"
#include "filsel_d.h"
#include "multmat.h"
#include "nomat_d.h"    
#include "key_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

static const Length s_maxlenSortKeyComponent = 100;

CIndex::CIndex(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
        const CMarkerRefList* pmrflstSec,
        Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
        CIndexSet* pindsetMyOwner,
        CFilter* pfil, CIndex* pindUnfiltered) :
        CSetEl(""),
        m_bModifiedSinceFiltered(FALSE)
{
    m_pmkrPriKey = pmkrPriKey;
    m_psrtPriKey = psrtPriKey;

    // 1995-08-08 MRP: USE A COPY CONSTRUCTOR   
    // Copy in the marker reference list.
    if (pmrflstSec != NULL)
        for (CMarkerRef* pmrf = pmrflstSec->pmrfFirst(); pmrf; 
                pmrf = pmrflstSec->pmrfNext(pmrf))
            m_mrflstSecKeys.AddMarkerRef(pmrf->pmkr());
        
    m_maxlenSortKeyComponent = s_maxlenSortKeyComponent;  // 1996-07-25 MRP
    int numSortKeyComponents = 1 + (int)(m_mrflstSecKeys.lGetCount());
    m_maxsizSortKey = numSortKeyComponents * (m_maxlenSortKeyComponent + 1);
    m_puchSortKeyA = new UIdMChar[m_maxsizSortKey];
    m_puchSortKeyB = new UIdMChar[m_maxsizSortKey];
    m_bSortPriKeyFromEnd = bSortPriKeyFromEnd;
    
    m_lRecEls = 0;

    m_pfil = pfil;
    m_pindUnfiltered = pindUnfiltered;
    
    m_pfldSubstituteForNullPriKey = new CField(m_pmkrPriKey);
    
    m_pindsetMyOwner = pindsetMyOwner;
    ASSERT( m_pindsetMyOwner );
    m_bNotIncludeUndefined = m_pindsetMyOwner->bNotIncludeUndefined();  // 1997-12-18 MRP

#ifdef _DEBUG
    m_bHasBeenSorted = FALSE;  // 2000-06-21 MRP
#endif  // _DEBUG
}

    
CIndex::~CIndex()
{
#ifdef _DEBUG
    AssertValid();
#endif  // _DEBUG

    delete m_puchSortKeyA;
    delete m_puchSortKeyB;
    delete m_pfldSubstituteForNullPriKey;
}

CSet* CIndex::psetMyOwner() 
{
    return NULL;
}

void CIndex::DecrementNumberOfRefs()
{
    m_ref.DecrementNumberOfRefs();
    if ( !m_ref.bHasRefs() )
        m_pindsetMyOwner->Delete(this);
}


static const char* psz_ind = "ind";
static const char* psz_PriKey = "PriKey";
static const char* psz_srtPriKey = "srtPriKey";
static const char* psz_SortPriKeyFromEnd = "SortPriKeyFromEnd";
//static const char* psz_maxlenKeyComponent = "maxlenKeyComponent";
//static const char* psz_mrflstSecKeys = "mkrlstSecKeys";
static const char* psz_SecKey = "SecKey";

static const char* psz_recset = "recset";
static const char* psz_fil = "fil";

#ifdef typWritefstream // 1.6.4ac
void CIndex::WriteProperties(Object_ofstream& obs) const
#else
void CIndex::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_ind);
    
    m_pmkrPriKey->WriteRef(obs, psz_PriKey);
    m_psrtPriKey->WriteRef(obs, psz_PriKey);
    obs.WriteBool(psz_SortPriKeyFromEnd, m_bSortPriKeyFromEnd);
    
//  m_mrflstSecKeys.WriteProperties(obs, psz_mrflstSecKeys);
    const CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        pmrf->pmkr()->WriteRef(obs, psz_SecKey);
    
//  obs.WriteLength(psz_maxlenKeyComponent, m_maxlenSortKeyComponent);
    
    if ( m_pfil )
        m_pfil->WriteRef(obs);
    
    obs.WriteEndMarker(psz_ind);
}

BOOL CIndex::s_bReadProperties(Object_istream& obs,
        CIndexSet* pindsetMyOwner, CIndex** ppind, CIndex** ppindUnfiltered)
{
    if ( !obs.bReadBeginMarker(psz_ind) )
        return FALSE;

    // Set default attributes
    CMarkerSet* pmkrset = pindsetMyOwner->pmkrRecord()->pmkrsetMyOwner();
    CMarker* pmkrPriKey = NULL;
    Str8 sPriKeySortOrder;
        // We read the sort order name rather than its reference in case
        // its property field precedes the primary marker field.
    BOOL bSortPriKeyFromEnd = FALSE;
    CMarkerRefList mrflstSecKeys;
    CMarker* pmkrSecKey = NULL;
//  Length maxlenSortKeyComponent = 10;
    CFilterSet* pfilset = pindsetMyOwner->pfilset();
    CFilter* pfil = NULL;

    while ( !obs.bAtEnd() )
        {
        if ( pmkrset->bReadMarkerRef(obs, psz_PriKey, pmkrPriKey) )
            ;
        else if ( obs.bReadString(psz_srtPriKey, sPriKeySortOrder) )
            ;
        else if (obs.bReadBool(psz_SortPriKeyFromEnd, bSortPriKeyFromEnd) )
            ;
//      else if ( obs.bReadLength(psz_maxlenKeyComponent, maxlenSortKeyComponent) )
//          ;
//      else if ( mrflstSecKeys.bReadProperties(obs, psz_mrflstSecKeys, pmkrset) )
        else if ( pmkrset->bReadMarkerRef(obs, psz_SecKey, pmkrSecKey) )
            {
            if ( pmkrSecKey )
                mrflstSecKeys.AddMarkerRef(pmkrSecKey);
            pmkrSecKey = NULL;
            }
        else if ( pfilset->bReadFilterRef(obs, pfil) )
            ;
        else if ( obs.bEnd(psz_ind) )
            break;
        }

    if ( pmkrPriKey )
        {
        CLangEnc* plng = pmkrPriKey->plng();
        CSortOrderSet* psrtset = plng->psrtset();
        CSortOrder* psrtPriKey = NULL;
        if ( !sPriKeySortOrder.IsEmpty() )
            psrtPriKey = psrtset->psrtSearch(sPriKeySortOrder);
        if ( !psrtPriKey )
            psrtPriKey = plng->psrtDefault();
        pindsetMyOwner->GetIndex(pmkrPriKey, psrtPriKey,
//          &mrflstSecKeys, maxlenSortKeyComponent, bSortPriKeyFromEnd,
            &mrflstSecKeys, s_maxlenSortKeyComponent, bSortPriKeyFromEnd,
            pfil, ppind, ppindUnfiltered);
        }
    else
        // If the primary marker was not specified, we ignore any other
        // properties and return the index corresponding to the database file.
        *ppind = *ppindUnfiltered = pindsetMyOwner->pindRecordOwner();

    ASSERT( *ppind );
    ASSERT( *ppindUnfiltered );
    return TRUE;
}


static const char* psz_rel = "rec";
static const char* psz_fld = "fld";

#ifdef prjWritefstream // 1.6.4aa 
void CIndex::WriteRecordRef(Object_ofstream& obs, const char* pszQualifier,
        const CNumberedRecElPtr& prel)
#else
void CIndex::WriteRecordRef(Object_ostream& obs, const char* pszQualifier,
        const CNumberedRecElPtr& prel)
#endif
{
    if ( !prel )
        return;
        
    obs.WriteBeginMarker(psz_rel);
    
    const CField* pfld = prel->pfldPriKey();
    const char* pszKey = ""; // 1.4vnf 
	if ( pfld ) // 1.4vnf 
		pszKey = pfld->sContents(); // 1.4vnf 
    obs.WriteObligatoryString(psz_fld, psz_PriKey, pszKey);
                                    
    CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) ) // For each secondary key marker (there may be none) 
        { // Get contents as above
        pfld = prel->pfldFirstInSubRecord(pmrf->pmkr());
		pszKey = ""; // 1.4vnf Fix U bug of writing jnk in prj file as filSecKey
		if ( pfld ) // 1.4vnf 
			pszKey = pfld->sContents(); // 1.4vnf  Fix U bug of writing jnk in prj file as filSecKey
        obs.WriteObligatoryString(psz_fld, psz_SecKey, pszKey);     
        }
        
    obs.WriteEndMarker(psz_rel);
}

BOOL CIndex::bReadRecordRef(Object_istream& obs, const char* pszQualifier,
        CNumberedRecElPtr& prel)
{
    if ( !obs.bReadBeginMarker(psz_rel) )
        return FALSE;

    Str8 sKey;
    (void) obs.bReadString(psz_fld, psz_PriKey, sKey);
    UIdMChar* puchSortKey = m_puchSortKeyA;
    (void) m_psrtPriKey->bSortKey(sKey, &puchSortKey,
        m_maxlenSortKeyComponent, m_bSortPriKeyFromEnd);
                                    
    // For each secondary key marker (there may be none) 
    CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        {
        // Get contents as above
        sKey.Empty();
        (void) obs.bReadString(psz_fld, psz_SecKey, sKey);
            
        // Append sort key based on default sort order of lang enc of marker
            // Note: Right-to-left option applies only to primary key                  
        (void) pmrf->psrtDefault()->bAppendToSortKey(sKey, &puchSortKey,
            m_maxlenSortKeyComponent);
        }
        
    if ( !obs.bReadEndMarker(psz_rel) )
        return FALSE;

    CNumberedRecElPtr prelFound = pnrlFirst();
    for ( ; prelFound; prelFound = pnrlNext(prelFound) )
        if ( iCompare(m_puchSortKeyA, prelFound->puchSortKey()) == 0 )
            {
            prel = prelFound;
            return TRUE;
            }

    return FALSE;
}


void  CIndex::SetFilter(CFilter* pfil, BOOL bUseFilter,
        CRecLookEl** pprelCur)
{
    if ( bUseFilter )
        ASSERT( pfil != NULL );
        
    if ( m_pfil != pfil )
        {
        m_pfil = pfil;
        }

    // RNE change to be made later.
    // *pprelCur = prelUpdated(*pprelCur);
}

BOOL CIndex::bIsMatch(const CRecLookEl* prelCur)
{
    ASSERT( prelCur != NULL );  // 1995-06-30 MRP
    return ( bUsingFilter() ? m_pfil->bMatch(prelCur, this) : TRUE );
}

BOOL CIndex::bReadOnly() const
{
    return m_pindsetMyOwner->bReadOnly();
}



BOOL CIndex::bUsesMarker(const CMarker* pmkr)
{
    CRecLookEl* prel = prelFirst();
    for( ; prel; prel = prelNext(prel) )
        if ( prel->prec()->pfldFirstWithMarker(pmkr) )
            return TRUE;
        
    return FALSE;
}

// This function changes to add record elements to the insertion list.
BOOL CIndex::bUpdateSortKeys(const CNumberedRecElPtr& prelCur)
{
    ASSERT(m_nrllstInserted.bIsEmpty() );
    ASSERT( m_nrllstToBeDeleted.bIsEmpty() );
    BOOL bPriKeyField = FALSE;
    CNumberedRecElPtr prel;
    ASSERT( (CRecLookEl*)prelCur != NULL );  // 1998-12-02 MRP
    m_nrllstToBeDeleted.AddRecElPtrs(this, prelCur->prec());
    CField* pfld = prelCur->prec()->pfldFirst();
    for ( ; pfld; pfld = prelCur->prec()->pfldNext(pfld) )
        if ( pfld->pmkr() == (const CMarker*)m_pmkrPriKey )
            {
            bPriKeyField = TRUE;
            (void) bSortKey(prelCur->prec(), pfld, m_puchSortKeyA);
            prel = m_nrllstToBeDeleted.prelFind_RemoveIfFound(m_puchSortKeyA, pfld);
            if ( prel )  // If the element is already in the index
                {
                if ( !bIsMatch(prel) )  // And it doesn't match the filter
                    // Let it be deleted, after all
                    m_nrllstToBeDeleted.AddRecElPtr(prel);
                }
            else
                {
//              prel = prelInsertRecEl(m_puchSortKeyA, prelCur->prec(), pfld);
//              m_nrllstInserted.AddRecElPtr(this, prel);
                CRecLookEl* prelToInsert = new CRecLookEl(m_puchSortKeyA,
                    prelCur->prec(), pfld);
                if ( bIsMatch(prelToInsert) )
                    {
                    prel = prelInsertRecEl(prelToInsert);
                    m_nrllstInserted.AddRecElPtr(prel);
                    }
                else
                    delete prelToInsert;
                }
            }
            
    if ( !bPriKeyField )
        {
        (void) bSortKey(prelCur->prec(), NULL, m_puchSortKeyA);
        prel = m_nrllstToBeDeleted.prelFind_RemoveIfFound(m_puchSortKeyA, pfld);
        if ( prel )  // If the element is already in the index
            {
            if ( !bIsMatch(prel) )  // And it doesn't match the filter
                // Let it be deleted, after all
                m_nrllstToBeDeleted.AddRecElPtr(prel);
            }
        else
            {
//          prel = prelInsertRecEl(m_puchSortKeyA, prelCur->prec(), NULL);
//          m_nrllstInserted.AddRecElPtr(this, prel);
            CRecLookEl* prelToInsert = new CRecLookEl(m_puchSortKeyA,
                prelCur->prec(), pfld);
            if ( bIsMatch(prelToInsert) )
                {
                prel = prelInsertRecEl(prelToInsert);
                m_nrllstInserted.AddRecElPtr(prel);
                }
            else
                delete prelToInsert;
            }
        }
        
    m_nrllstToBeDeleted.IncrementRecElNumbers(m_nrllstInserted);

    // 2000-04-27 TLB - Set m_bModifiedSinceFiltered if any changes occurred to index and there is a filter which cannot be evaluated based solely on record contents
    m_bModifiedSinceFiltered = (m_pfil && (!m_nrllstInserted.bIsEmpty() || !m_nrllstToBeDeleted.bIsEmpty()) && m_pfil->bFilterDependsOnRecordContext());
    
    // Is the record element of the active view (prelCur) being deleted?
    return m_nrllstToBeDeleted.bIsMember(prelCur);
}


    // Return the nearest matching record in the next direction
    // that refers to a different record from that of prelCur;
    // otherwise, return the nearest such in the previous direction;
    // otherwise, return NULL.
    // Used to get off a record about to be deleted.
CNumberedRecElPtr CIndex::prelDifferentRecord(const CNumberedRecElPtr& prelCur) const
{
    CRecord* precDeleted = prelCur->prec();
    
    CNumberedRecElPtr prel = prelCur;
    prel = pnrlNext(prel);
    
    while ( ((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
            (prel->prec() == precDeleted) )
        prel = pnrlNext(prel);
        
    // If we can't go forward to find a record element not being deleted,
    // then back up.
    if ((CRecLookEl*)prel == NULL)  // 1998-12-02 MRP
        {
            prel = prelCur;
            prel = pnrlPrev(prel);
    
            while ( ((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
                    (prel->prec() == precDeleted) )
                prel = pnrlPrev(prel);
        }

    return prel;
}
    

BOOL CIndex::bRecElUpdated(CNumberedRecElPtr* pprelCur) const
{
    ASSERT( pprelCur );
    CNumberedRecElPtr prel = *pprelCur;
    ASSERT( (CRecLookEl*)prel != NULL );  // 1998-12-02 MRP
    if ( !m_nrllstToBeDeleted.bIsMember(prel) )
        return FALSE;

    *pprelCur = prelSame_UpToDate(prel);  // 1996-07-25 MRP
    return TRUE;
}


CNumberedRecElPtr CIndex::prel_UpToDate(const CNumberedRecElPtr& prelCur) const
{
    ASSERT((CRecLookEl*)prelCur != NULL);  // 1998-12-02 MRP
    if ( !m_nrllstToBeDeleted.bIsMember(prelCur) )
        return prelCur;
    CNumberedRecElPtr prel = prelNext_UpToDate(prelCur);
    if ((CRecLookEl*)prel == NULL)  // 1998-12-02 MRP
        prel = prelPrev_UpToDate(prelCur);
    return prel;
}

CNumberedRecElPtr CIndex::prelNext_UpToDate(const CNumberedRecElPtr& prelCur) const
{
    ASSERT((CRecLookEl*)prelCur != NULL);  // 1998-12-02 MRP
    CNumberedRecElPtr prel = pnrlNext(prelCur);
    
    while (((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
            m_nrllstToBeDeleted.bIsMember(prel))
        prel = pnrlNext(prel);
    
    if ((CRecLookEl*)prel == NULL &&  // 1998-12-02 MRP
            m_nrllstToBeDeleted.bIsMember(prelCur))
        {
        prel = pnrlPrev(prelCur);
        
        while (((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
                m_nrllstToBeDeleted.bIsMember(prel))
            prel = pnrlPrev(prel);
        }

    return prel;
}

CNumberedRecElPtr CIndex::prelPrev_UpToDate(const CNumberedRecElPtr& prelCur) const
{
    ASSERT((CRecLookEl*)prelCur != NULL);  // 1998-12-02 MRP
    CNumberedRecElPtr prel = pnrlPrev(prelCur);
    
    while (((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
            m_nrllstToBeDeleted.bIsMember(prel))
        prel = pnrlPrev(prel);
    
    if ((CRecLookEl*)prel == NULL &&  // 1998-12-02 MRP
            m_nrllstToBeDeleted.bIsMember(prelCur))
        {
        prel = pnrlNext(prelCur);
        
        while (((CRecLookEl*)prel != NULL) &&  // 1998-12-02 MRP
                m_nrllstToBeDeleted.bIsMember(prel))
            prel = pnrlNext(prel);
        }
    
    return prel;
}


CNumberedRecElPtr CIndex::prelSame_UpToDate(const CNumberedRecElPtr& prelCur) const
{
    ASSERT( (CRecLookEl*)prelCur != NULL );  // 1998-12-02 MRP
    CRecord* prec= prelCur->prec();
    ASSERT( prec != NULL );
    CField* pfldPriKey = prelCur->pfldPriKey();

    // If there is an up-to-date element referring to the same field, use it.   
    CNumberedRecElPtr prel = pnrlFirst();
    for ( ; prel; prel = pnrlNext(prel) )
        if ( prec == prel->prec() && pfldPriKey == prel->pfldPriKey() &&
                !m_nrllstToBeDeleted.bIsMember(prel) )
            break;
    if ( (CRecLookEl*)prel != NULL )  // 1998-12-02 MRP
        return prel;

    // If there is an up-to-date element referring to the same record, use it.  
    prel = pnrlFirst();
    for ( ; prel; prel = pnrlNext(prel) )
        if ( prec == prel->prec() && !m_nrllstToBeDeleted.bIsMember(prel) )
            break;
    if ( (CRecLookEl*)prel != NULL )  // 1998-12-02 MRP
        return prel;
        
    // Return the "nearest" up-to-date element, if there is one
    prel = prel_UpToDate(prelCur);
    
    return prel;  // Note: This can correspond to a null element
}
        
CNumberedRecElPtr CIndex::prelCorrespondingTo(const CRecLookEl* prelCur)
{
    ASSERT( prelCur );
    const CRecord* prec = prelCur->prec();
    const CField* pfldPriKey = prelCur->pfldPriKey();
    CNumberedRecElPtr prel = pnrlFirst();
    for ( ; prel; prel = pnrlNext(prel) )
        if ( prel->prec() == prec && prel->pfldPriKey() == pfldPriKey )
            break;
    
    return prel;
}

                  
void CIndex::IncrementRecElNumber(CNumberedRecElPtr* ppnrl)
{
    CNumberedRecElPtrListEl* pnrl = m_nrllstInserted.pnrlFirst();   
    for( ;pnrl; pnrl = m_nrllstInserted.pnrlNext(pnrl) )
        if ( pnrl->prel().lNum() <= ppnrl->lNum() )
             ppnrl->Set(*ppnrl, ppnrl->lNum() + 1L);
}

void CIndex::DecrementRecElNumber(CNumberedRecElPtr* ppnrl)
{
    CNumberedRecElPtrListEl* pnrl = m_nrllstToBeDeleted.pnrlFirst();    
    long lRecNum = ppnrl->lNum();
    for( ;pnrl; pnrl = m_nrllstToBeDeleted.pnrlNext(pnrl) )
        if ( pnrl->prel().lNum() < ppnrl->lNum() )
            lRecNum--;
             
    ppnrl->Set(*ppnrl, lRecNum);
}


CNumberedRecElPtr CIndex::prelAddRecord(CRecord* prec) // Add record to index, possibly multiple times
{
    CNumberedRecElPtr prelFirstKey = NULL;
    ASSERT( prec );
    CField* pfld = prec->pfldFirst();
    for ( ; pfld; pfld = prec->pfldNext(pfld) ) // For each field in the record
        if ( pfld->pmkr() == (const CMarker*)m_pmkrPriKey ) // If it is an instance of the primary key field
            {
            (void) bSortKey(prec, pfld, m_puchSortKeyA); // Make a sort key for this field and this record
            CNumberedRecElPtr prel = prelAddRecEl(m_puchSortKeyA, prec, pfld); // Add the prel to this index
            if ( !prelFirstKey ) // If this is the first key found, remember it
                prelFirstKey = prel;
            }

    if ( !prelFirstKey ) // If no key field was found
        {
        (void) bSortKey(prec, NULL, m_puchSortKeyA); // Make a sort key for no field
        prelFirstKey = prelAddRecEl(m_puchSortKeyA, prec, pfld); // Add the prel to this index
        }
    
    return prelFirstKey; // Return the first prel that was made
}

CNumberedRecElPtr CIndex::prelInsertRecord(CRecord* prec, BOOL bIgnoreTheFilter)
{
    ASSERT( m_nrllstInserted.bIsEmpty() );
    ASSERT( m_nrllstToBeDeleted.bIsEmpty() );
    CNumberedRecElPtr prelFirstKey = NULL;
    ASSERT( prec );
    CField* pfld = prec->pfldFirst();
    for ( ; pfld; pfld = prec->pfldNext(pfld) )
        if ( pfld->pmkr() == (const CMarker*)m_pmkrPriKey )
            {
            (void) bSortKey(prec, pfld, m_puchSortKeyA);
//          CNumberedRecElPtr prel = prelInsertRecEl(m_puchSortKeyA, prec, pfld);
            CRecLookEl* prelToInsert = new CRecLookEl(m_puchSortKeyA, prec, pfld);
            if ( bIgnoreTheFilter || bIsMatch(prelToInsert) )
                {
                CNumberedRecElPtr prel = prelInsertRecEl(prelToInsert);
                m_nrllstInserted.AddRecElPtr(prel);
                if ( !prelFirstKey )
                    prelFirstKey = prel;
                }
            else
                delete prelToInsert;
            }

    if ( !prelFirstKey )
        {
        (void) bSortKey(prec, NULL, m_puchSortKeyA);
//      prelFirstKey = prelInsertRecEl(m_puchSortKeyA, prec, pfld);
        CRecLookEl* prelToInsert = new CRecLookEl(m_puchSortKeyA, prec, pfld);
        if ( bIgnoreTheFilter || bIsMatch(prelToInsert) )
            {
            prelFirstKey = prelInsertRecEl(prelToInsert);
            m_nrllstInserted.AddRecElPtr(prelFirstKey);
            }
        else
            delete prelToInsert;
        }

//  m_nrllstInserted.AddRecElPtrs(this, prec);

    return prelFirstKey;
}


void CIndex::SortFrom(const CIndex* pindRecordOwner)
{
    // Add the elements to the end of this index and sort it
    ASSERT( pindRecordOwner );
    ASSERT( !pindRecordOwner->bUsingFilter() );
    
    {
    CProgressIndicator prg(pindRecordOwner->lNumRecEls(), NULL, m_pmkrPriKey->sMarker()); // 1.4ad Eliminate resource messages sent to progress bar
    long l = 0;
    CRecLookEl* prel = pindRecordOwner->prelFirst();
    for ( ; prel; prel = pindRecordOwner->prelNext(prel) )
        {
        (void) prelAddRecord(prel->prec());  // Add all elements
        (void) prg.bUpdateProgress(++l);
        }
    }
    
    (void) bSort();     
}

BOOL CIndex::bSort()
{
#ifdef _DEBUG
    m_bHasBeenSorted = TRUE;  // 2000-06-21 MRP
#endif

    if ( bSorted() )
        return FALSE;

    // NOTE: Although the number of exchanges can be as many as max
    // in the worst case, the average for shell sort is order n**1.5;
    // which is a small percentage for large n.
    // In order to give a sensible indication of progress, we augment
    // the accumulating total of exchanges done to account for the ones
    // which would have been done at that stage in a worst case sort.
    long n = m_lRecEls;
    long max = 0;
    long gap;
    for ( gap = n/2; gap != 0; gap /= 2 )
        max += (n - gap);  // Number of iterations of the outer two loops
    CProgressIndicator prg(max, NULL, m_pmkrPriKey->sMarker()); // 1.4ad Eliminate resource messages sent to progress bar
    long l = 0;
    CRecLookEl** pprelSort = new CRecLookEl*[n];
    ASSERT( pprelSort );

    long lNum = 0;
    CRecLookEl* prel = prelFirst();
    while ( prel )
        {
        CRecLookEl* prelRemoved = (CRecLookEl*)pelRemove((CDblListEl**)&prel);
        prelRemoved->SetNum(lNum);
        pprelSort[lNum++] = prelRemoved;
        }
    ASSERT( lNum == m_lRecEls );
        
    // Sort the array using shell sort
    // See Kernighan and Plauger, _Software_Tools_, p. 106.
    for ( gap = n/2; gap != 0; gap /= 2 )
        for ( long i = gap; i != n; i += 1 )
            {
            for ( long jB = i; gap <= jB; jB -= gap )
                {
                long jA = jB - gap;
                CRecLookEl* prelA = pprelSort[jA];
                CRecLookEl* prelB = pprelSort[jB];
                
                // Compare
                int i = iCompare(prelA, prelB);
// #ifdef TRYWITHOUT // 1.4qzpb See if skipping this fixes bug of flipping order of same elements // Didn't make any difference
                if ( i == 0 )
                    {
                    long lDiff = prelA->lNum() - prelB->lNum();
                    ASSERT( lDiff != 0 || prelA == prelB );
                    i = ( lDiff < 0 ? -1 : 1 );
                    }
// #endif // TRYWITHOUT // 1.4qzpb
                if ( i <= 0 )
                    break;
                
                // Exchange
                pprelSort[jA] = prelB;
                pprelSort[jB] = prelA;

                }  // for jB
            (void) prg.bUpdateProgress(++l);
            }  // for i
    
    // Add the elements in the sorted array to the index list
    ASSERT( bIsEmpty() );
    for ( lNum = 0; lNum != m_lRecEls; lNum += 1 )
        {
        prel = pprelSort[lNum];
        prel->ClearNum();
        CDblList::Add(prel);
        }
    
    delete [] pprelSort;

    return TRUE;
}

BOOL CIndex::bSorted() const
{
    const CRecLookEl* prelA = prelFirst();
    if ( !prelA )
        return TRUE;
        
    CRecLookEl* prelB = prelNext(prelA);
    for ( ; prelB; prelA = prelB, prelB = prelNext(prelA) )
        if ( 0 < iCompare(prelA, prelB) )
            return FALSE;
        
    return TRUE;
}

void CIndex::MarkerUpdated(CMarkerUpdate& mup)
{
    if ( bMarkerUpdateRequiresReSorting(mup) )
        {
        if ( (const CMarker*)m_pmkrPriKey == mup.pmkr() )
            m_psrtPriKey = m_pmkrPriKey->plng()->psrtDefault();
        ReSort();
        }
}

void CIndex::FilterUpdated(CFilterUpdate& fup)
{
    if ( bFilterUpdateRequiresReFiltering(fup) )
        ReFilter();
}

void CIndex::LangEncUpdated(CLangEncUpdate& lup)
{
    if ( bLangEncUpdateRequiresReSorting(lup) )
        ReSort();
}

void CIndex::SortOrderUpdated(CSortOrderUpdate& sup)
{
    if ( bSortOrderUpdateRequiresReSorting(sup) )
        ReSort();
}

BOOL CIndex::bMarkerUpdateRequiresReSorting(CMarkerUpdate& mup)
{
    if ( bIsEmpty() )
        return FALSE;
    if ( !mup.bLangEncModified() )
        return FALSE;

    // Having chosen a different language encoding for any of this index's 
    // sort key markers requires resorting.
    // NOTE NOTE NOTE: If this is the primary key marker's language,
    // the primary sort order will have to be changed to the default order.
    const CMarker* pmkr = mup.pmkr();
    if ( m_pmkrPriKey == pmkr )
        return TRUE;
    const CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        if ( pmrf->pmkr() == pmkr )
            return TRUE;
            
    return FALSE;
}

BOOL CIndex::bLangEncUpdateRequiresReSorting(CLangEncUpdate& lup)
{
    if ( bIsEmpty() )
        return FALSE;
    if ( !lup.bDefaultSortOrderChanged() )
        return FALSE;

    // Having chosen a different default sort order 
    // for any secondary marker's language requires resorting.
    // Note: Since the primary key's sort order is chosen explicitly,
    // changing the corresponding default does not affect this index.
    const CLangEnc* plng = lup.plng();
    const CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        if ( pmrf->pmkr()->plng() == plng )
            return TRUE;
            
    return FALSE;
}

BOOL CIndex::bSortOrderUpdateRequiresReSorting(CSortOrderUpdate& sup)
{
    if ( bIsEmpty() )
        return FALSE;
    if ( !sup.bOrderModified() )
        return FALSE;

    // Having modified the order properties of the primary sort order or
    // the default sort order of any secondary marker's language
    // requires resorting.
    const CSortOrder* psrt = sup.psrt();
    if ( m_psrtPriKey == psrt )
        return TRUE;
    const CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        if ( pmrf->pmkr()->plng()->psrtDefault() == psrt )
            return TRUE;
            
    return FALSE;
}

void CIndex::ReSort()
{
    if ( bUsingFilter() )
        {
        ReFilter();  // Assumes unfiltered index has already been resorted
        return;
        }
        
    // 1. Recompute each element's sort key
    {
    CProgressIndicator prg(lNumRecEls(), NULL, m_pmkrPriKey->sMarker()); // 1.4ad Eliminate resource messages sent to progress bar
    long l = 0;
    CRecLookEl* prel = prelFirst();
    for ( ; prel; prel = prelNext(prel) )
        {
        bSortKey(prel->prec(), prel->pfldPriKey(), m_puchSortKeyA);
        prel->SetSortKey(m_puchSortKeyA);  // 1999-04-30 MRP
        (void) prg.bUpdateProgress(++l);
        }
    }
    
    // 2. Sort all the elements     
    (void) bSort();
}

BOOL CIndex::bMarkerUpdateModifiesDatabase(CMarkerUpdate& mup)
{
    if ( !mup.bNameModified() )
        return FALSE;

    const CMarker* pmkr = mup.pmkr();
    
    // If the modified marker is not from the same set as this index's
    // primary sort marker, then this index could not refer to it.
    if ( pmkr->pmkrsetMyOwner() != m_pmkrPriKey->pmkrsetMyOwner() )
        return FALSE;

    // The properties of this index appearing in a database's header
    // will refer to the record marker and sorting markers.
    if ( m_pindsetMyOwner->pmkrRecord() == pmkr )
        return TRUE;
    if ( m_pmkrPriKey == pmkr )
        return TRUE;
    const CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        if ( pmrf->pmkr() == pmkr )
            return TRUE;

    // Does any record refer to the marker?     
    CRecLookEl* prel = prelFirst();
    for ( ; prel; prel = prelNext(prel) )
        if ( prel->prec()->pfldFirstWithMarker(pmkr) )
            return TRUE;
        
    return FALSE;
}

BOOL CIndex::bFilterUpdateRequiresReFiltering(CFilterUpdate& fup)
{
    // 1996-05-30 MRP: A filter modification certainly could cause
    // the number of matching elements to change from zero to non-zero,
    // therefore requiring that a filtered index be refiltered.
    // The original code seems to have been imitating ReSorting logic.
//  if ( bIsEmpty() )
//      return FALSE;
    if ( fup.bReapplyingFilterToIndex(this) )
        return TRUE;
    if ( !fup.bMatchCriteriaModified() )
        return FALSE;
        
    const CFilter* pfil = fup.pfil();
    return m_pfil == pfil;
}

void CIndex::ReFilter()
{
    m_pindsetMyOwner->MoveElementsFrom(this);
    m_lRecEls = 0;

    ASSERT( m_pindUnfiltered );
    ASSERT( !m_pindUnfiltered->bUsingFilter() );
    ASSERT( m_pfil );

    CProgressIndicator prg(m_pindUnfiltered->lNumRecEls(), NULL, m_pfil->sName()); // 1.4ad Eliminate resource messages sent to progress bar
    long l = 0;

    CRecLookEl* prel = m_pindUnfiltered->prelFirst();
    for ( ; prel; prel = m_pindUnfiltered->prelNext(prel) )
        {
        if ( m_pfil->bMatch(prel, m_pindUnfiltered) )
            AddRecEl_CopyFrom(prel);  // Add matching element
        (void) prg.bUpdateProgress(++l);
        }
    m_bModifiedSinceFiltered = FALSE;
}


// !!! RNE Does linear search for now.
// Later, build the key and use that to look up the record.
// (But watch out for more than one recEl with the same key.)
CNumberedRecElPtr CIndex::prelFind(CRecord* prec)
{
    for(CNumberedRecElPtr prel = pnrlFirst(); prel; prel = pnrlNext(prel))
        if (prel->prec() == prec)
            break;
        
    return prel;
}

BOOL CIndex::bEqual(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
            CFilter* pfil)
{
    if ( pmkrPriKey != m_pmkrPriKey )
        return FALSE;
    if ( psrtPriKey != m_psrtPriKey )
        return FALSE;
    if ( !pmrflstSecKeys->bEqual(m_mrflstSecKeys) )
        return FALSE;
//  if ( maxlenSortKeyComponent != m_maxlenSortKeyComponent )
//      return FALSE;
    if ( bSortPriKeyFromEnd != m_bSortPriKeyFromEnd )
        return FALSE;
    if (pfil != m_pfil)
        return FALSE;
        
    return TRUE;    
}


BOOL CIndex::bSortKey(const CRecord* prec, CField* pfld, 
        UIdMChar* puchSortKeyBuf)
{
    BOOL bNoComponentTruncated = TRUE;
    const char* pszKey = ""; // 1.4vng 
	if ( pfld ) // 1.4vng 
		pszKey = pfld->sContents(); // 1.4vng 
    UIdMChar* puchSortKey = puchSortKeyBuf;

    if ( !m_psrtPriKey->bSortKey(pszKey, &puchSortKey,
            m_maxlenSortKeyComponent, m_bSortPriKeyFromEnd) )
        bNoComponentTruncated = FALSE;  // the key was truncated
                                    
    ASSERT( prec );
    CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) ) // For each secondary key marker (there may be none) 
        { // Get contents as above
        const CField* pfldSec = prec->pfldFirstInSubRecord(pmrf->pmkr(), pfld);
		pszKey = "";  // 1.4vng Clean up other places like 1.4vnf
		if ( pfldSec ) // 1.4vng 
			pszKey = pfldSec->sContents(); // 1.4vng 

        // Append sort key based on default sort order of lang enc of marker
            // Note: Right-to-left option applies only to primary key
        if ( !pmrf->psrtDefault()->bAppendToSortKey(pszKey,
                                &puchSortKey, m_maxlenSortKeyComponent) )
            bNoComponentTruncated = FALSE;  // the sort key was truncated
        }

    ASSERT( puchSortKey - puchSortKeyBuf < (ptrdiff_t)m_maxsizSortKey );

    return bNoComponentTruncated;
}

    
BOOL CIndex::bSortKey_PriOnly(const char* pszKey, UIdMChar* puchSortKeyBuf)
{
    BOOL bNoComponentTruncated = TRUE;
    UIdMChar* puch = puchSortKeyBuf;
    if ( !m_psrtPriKey->bSortKey(pszKey, &puch,
            m_maxlenSortKeyComponent, m_bSortPriKeyFromEnd) )
        bNoComponentTruncated = FALSE;

    // Secondary key components of a primary-only key are all empty;
    // however they must still be appended so that the key contains
    // the proper number of separators.
//    static const char* pszEmpty = ""; // 1.4vng 
    CMarkerRef* pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; pmrf; pmrf = m_mrflstSecKeys.pmrfNext(pmrf) )
        (void) pmrf->psrtDefault()->bAppendToSortKey("", &puch, m_maxlenSortKeyComponent); // 1.4vng 

    ASSERT( puch - puchSortKeyBuf < (ptrdiff_t)m_maxsizSortKey );
    
    return bNoComponentTruncated;
}

int CIndex::iCompare(const CRecLookEl* prelA, const CRecLookEl* prelB, MatchSetting matset, BOOL bMatchWhole) const
{
	if ( !prelA->prec() && !prelB->prec() ) // 1.4gzq If comparing empty records, consider them equal, fixes false assert
		return 0;
    // Distinguish absence of a field from empty fields when sorting.
    ASSERT( prelA );
    CField* pfldPriKeyA = prelA->pfldPriKey();
    ASSERT( prelB );
    CField* pfldPriKeyB = prelB->pfldPriKey();
    if ( !pfldPriKeyA && pfldPriKeyB ) // Sort absent lower than empty
        return -1;
    if ( pfldPriKeyA && !pfldPriKeyB ) // Sort absent lower than empty
        return 1;
	if ( !bUnicodeSequenceSort() ) // If not general Unicode sort, do standard compare
	    return iCompare(prelA->puchSortKey(), prelB->puchSortKey(), matset, bMatchWhole);

	int iCompareResult = 0; // Do general Unicode sort
    if ( pfldPriKeyA && pfldPriKeyB ) // Protect against both fields absent
		iCompareResult = m_psrtPriKey->iCompareUnicodeSequence( pfldPriKeyA->psz(), pfldPriKeyB->psz(), matset, bMatchWhole ); // Use first sort sequence to compare the first sort keys
	if ( iCompareResult != 0 ) // If difference in primary sort, return result
		return iCompareResult;
	if ( m_mrflstSecKeys.pmrfFirst() ) // 1.4gze
		iCompareResult = iCompare(prelA->puchSortKey(), prelB->puchSortKey(), matset, bMatchWhole); // 1.4gze If general Unicode sort and primary key is equal, sort by secondary fields
	return iCompareResult;
}

int CIndex::iCompare(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                            MatchSetting matset, BOOL bMatchWhole) const
{
    const UIdMChar* puchA = puchKeyA;
    const UIdMChar* puchB = puchKeyB;
    
    // 1997-12-18 MRP: Emergency patch to provide compatibility with MDF-DOS
    // sorting of a reversed index (finderlist).
    if ( m_bNotIncludeUndefined )
        matset &= (~CMCharOrder::mchtypUndefined);
        
    // Compare on the primary sort order first.
    int iComp = iCompare_PriOnly(puchA, puchB, matset, bMatchWhole);
    
    // If the primary keys are equal, compare on each of the secondary keys.
    CMarkerRef*  pmrf = m_mrflstSecKeys.pmrfFirst();
    for ( ; (pmrf != NULL) && (iComp == 0); pmrf = m_mrflstSecKeys.pmrfNext(pmrf))
        {
        // Go to the terminator of this (previous) section of both the keys.
        for (; !CSortOrder::bIsTerminator(*puchA); puchA++);
        for (; !CSortOrder::bIsTerminator(*puchB); puchB++);
        // Then to the first item of the next section to be compared.
        puchA++; puchB++;
        
        // Compare the keys according to the sort order.
        iComp = pmrf->psrtDefault()->iCompare(puchA, puchB, matset, bMatchWhole);
        }
        
    return iComp;
}


int CIndex::iCompare_PriOnly(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                            MatchSetting matset, BOOL bMatchWhole) const
{
    return m_psrtPriKey->iCompare(puchKeyA, puchKeyB, matset, bMatchWhole);
}

BOOL CIndex::bEqual_PriOnly(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                            MatchSetting matset, BOOL bMatchWhole) const
{
    int i = m_psrtPriKey->iCompare(puchKeyA, puchKeyB, matset, bMatchWhole);
    return ( i == 0 );
}




void CIndex::DeleteRecEl(CRecLookEl* prelToDelete)
{
    ASSERT( prelToDelete );
    CDblList::Delete(prelToDelete);
    m_lRecEls -= 1;
}

void CIndex::RemoveRecord(CRecord* precToRemove)
{
    ASSERT( precToRemove );
    CRecLookEl* prel = prelFirst();
    while ( prel )
        {
        CRecLookEl* prelAfterThis = prelNext(prel);
        if ( prel->prec() == precToRemove )
            DeleteRecEl(prel);
        prel = prelAfterThis;
        }
}

void CIndex::DoneUpdatingViews()
{
    m_nrllstInserted.DeleteAll();

    CNumberedRecElPtrListEl* pnrl = m_nrllstToBeDeleted.pnrlFirst();
    while ( pnrl )
        {
        DeleteRecEl(pnrl->prel());
        m_nrllstToBeDeleted.Delete(&pnrl);
        }
    ASSERT( m_nrllstToBeDeleted.bIsEmpty() );
}

void CIndex::WriteElements(Field_ostream& sfs, CDatabaseType* ptyp, CProgressIndicator& prg) // 1.4qpb
{
    long lnElements = 0;
    const CRecLookEl* prel = prelFirst();
    for ( ; prel; prel = prelNext(prel) )
        {
        WriteElement(sfs, ptyp, prel); // 1.4qpb
        (void) prg.bUpdateProgress(++lnElements);
        }
}

void CIndex::WriteElement(Field_ostream& sfs, CDatabaseType* ptyp, const CRecLookEl* prel) const // 1.4qpb
{
    ASSERT( prel );
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst();
    BOOL bFirstField = TRUE;
    for ( ; pfld; pfld = prec->pfldNext(pfld), bFirstField = FALSE )
        sfs.WriteField(pfld, bFirstField);
}

void CIndex::WriteElements(Field_ostream& sfs, CDatabaseType* ptyp, CProgressIndicator& prg, // 1.4qpb
        const CMarkerSubSet& subsetMarkersToExport,
        BOOL bPriKeyFieldFirst, BOOL bMDF, BOOL bAddAlphDiv)
{
//	CIndexSet* pindset = pindsetMyOwner(); // 1.4qpb
//	CDatabaseType* ptyp = pindsetMyOwner()->ptyp(); // 1.4qpb
    const CMChar* pmchPrev = NULL;
    long lnElements = 0;
    const CRecLookEl* prel = prelFirst();
    for ( ; prel; ) // 1.1th prel = prelNext(prel) )
        {
		BOOL bAlphDivAdded = FALSE;
		if ( bAddAlphDiv )
			{
	        const Str8& sHeading = prel->prec()->pfldFirst()->sContents();
			const CMChar* pmch = NULL;
			if ( m_psrtPriKey->bFirstPrimary(sHeading, &pmch) )
				{
				if ( pmch != pmchPrev )
					{
					pmchPrev = pmch;
					const CMChar* pmchUpper = (pmch->bUpperCase() ?
						pmch :
						pmch->pmchOtherCase()
						);
					if ( pmch->bUpperCase() )
						pmch = pmch->pmchOtherCase();
					Str8 sLetter;
					if ( pmchUpper )
						{
						sLetter = pmchUpper->sMChar();
						sLetter += "  -  ";  // String will become an option
						}
					ASSERT( pmch );
					sLetter += pmch->sMChar();
					sfs.WriteAlphDiv(sLetter, ptyp, prel == prelFirst() );  // 1.1cm Write alphabetic divider
					bAlphDivAdded = TRUE;
					}
				}
			}
		WriteElement(sfs, ptyp, prel, subsetMarkersToExport, bPriKeyFieldFirst, bMDF); // 1.4qpb
        (void) prg.bUpdateProgress(++lnElements);
		if ( ptyp->bTextFile() && pmkrPriKey() == ptyp->pmkrRef() ) // 1.1th If text file sorted by reference, move to next record
			{
			CRecord* precCur = prel->prec(); // 1.1th If text file, move to next record
			do {
				prel = prelNext(prel);
				} while ( prel && prel->prec() == precCur );
			}
		else // If not text file, move to next index
			prel = prelNext(prel);
        }
	sfs.WriteMultipleColumnEnd(); // 1.2du For rtf export double column, even up columns on last page
}

void CIndex::WriteElement(Field_ostream& sfs, CDatabaseType* ptyp, const CRecLookEl* prel, // 1.4qpb Make database typ visible to XML export
        const CMarkerSubSet& subsetMarkersToExport,
        BOOL bPriKeyFieldFirst, BOOL bMDF) const
{
    BOOL bExcludeEmptyPriKeyField = bMDF;  // 1998-12-07 MRP
    ASSERT( prel );
    CRecord* prec = prel->prec();
    CField* pfld = prec->pfldFirst();
    if ( (const CMarker*)m_pmkrPriKey != pfld->pmkr() && bPriKeyFieldFirst )
        {
        // 1998-03-06 MRP: If you've selected Current Window's Records and
        // the primary sorting marker isn't the record marker, you can choose
        // to move the relevant sorting field to the top of the record.
        const CField* pfldPriKey = prel->pfldPriKey();
        if ( !pfldPriKey )
            return;

        if ( bExcludeEmptyPriKeyField )  // 1998-12-07 MRP
            {
            const UIdMChar* puch = prel->puchSortKey();
            if ( CSortOrder::bIsTerminator(*puch) )
                return;
            }

        if ( bMDF )  // 1998-12-07 MRP
            {
            BOOL bFirstField = TRUE;
            CFieldIterator qfld(prel, this);
            CFieldIterator qfldEnd;
            for ( ; qfld != qfldEnd; ++qfld, bFirstField = FALSE )
                    sfs.WriteField(*qfld, bFirstField);
            }
        else
            {
            // Write the primary sorting field first.    
            ASSERT( pfldPriKey->pmkr() == (const CMarker*)m_pmkrPriKey );
            if ( subsetMarkersToExport.bIncludes(m_pmkrPriKey) )
                sfs.WriteField(pfldPriKey, TRUE);
        
            for ( ; pfld; pfld = prec->pfldNext(pfld) )
                // Omit fields with the primary sorting marker.
                if ( subsetMarkersToExport.bIncludes(pfld->pmkr()) &&
                        pfld->pmkr() != (const CMarker*)m_pmkrPriKey )
                    sfs.WriteField(pfld, FALSE);
            }
        }
    else
        {
		CMarker* pmkrParse = NULL; // 1.4qpc Find marker that is output of parse
		CInterlinearProcList* pintprclst = ptyp->pintprclst(); // 1.4qpc
		if ( pintprclst ) // 1.4qpc If there is an interlinear proc list
			{
			for ( CInterlinearProc* pintprc = pintprclst->pintprcFirst(); pintprc; pintprc = pintprclst->pintprcNext( pintprc ) ) // 1.4qpc For each proc
				{
				if ( pintprc->bParseProc() ) // 1.4qpc If this is parse, remember its output marker
					{
					pmkrParse = pintprc->pmkrTo(); // 1.4qpc
					break; // 1.4qpc Don't look further, so as to remember the first one
					}
				}
			}
        BOOL bFirstField = TRUE;
        for ( ; pfld; pfld = prec->pfldNext(pfld), bFirstField = FALSE )
			{
			CRecPos rpsTemp( 0, pfld, prec);  // 1.5.7b Make temp rps to test for interlinear
			if ( sfs.bXML()  // 1.4qpa Make XML export do structured interlinear
					&& rpsTemp.bInterlinear() // 1.5.7b Fix bug of double word output on unannotated interlinear
					&& pfld->pmkr()->bFirstInterlinear() ) // 1.4qpa If XML and first interlinear line, do each word separately
				{
				CRecPos rpsStartWd( 0, pfld, prec); // 1.4qpa Set start of word
				CRecPos rpsOut = rpsStartWd; // 1.4qpa
				int iWdFldEnd = rpsStartWd.pfld->GetLength(); // 1.4qpa
				while ( rpsStartWd.iChar < iWdFldEnd ) // 1.4qpa For each word in first line of interlinear, output XML
					{
					CRecPos rpsEndWd = rpsStartWd.rpsNextWdBeginBundle(); // 1.4qpa Find end of word
					rpsOut = rpsStartWd; // 1.4qpa
					int iWdLen = rpsEndWd.iChar - rpsStartWd.iChar; // 1.4qpa
					Str8 sOut;
					while ( TRUE ) // 1.4qpd While this field is not the output of a parse, output the whole word
						{
						CMarker* pmkrOut = rpsOut.pfld->pmkr(); // 1.4qpd
						if ( pmkrOut == pmkrParse ) // 1.4qpd If parse proc, stop this loop
							break;
						int iLen = iWdLen; // 1.4qpf Fix bug (1.4qpe) of annotation cut short at end of line
						if ( rpsEndWd.bEndOfField() ) // 1.4qpf
							iLen = rpsOut.pfld->GetLength(); // 1.4qpf
						sOut = rpsOut.pfld->sContents(); // 1.4qpa Make temp string to output
						sOut = sOut.Mid( rpsOut.iChar, iLen ); // 1.4qpa
						CField fldOut( rpsOut.pfld->pmkr(), sOut ); // 1.4qpa Make temp field to output
						CField* pfldOut = &fldOut; // 1.4qpa
						if ( subsetMarkersToExport.bIncludes(pfldOut->pmkr()) )
							sfs.WriteField(pfldOut, bFirstField);
						bFirstField = FALSE; // 1.4qpe
						if ( !rpsOut.bLastInBundle() ) // 1.4qpd
							rpsOut.NextField(); // 1.4qpd
						else
							break;
						}
					CRecPos rpsStartMorph( rpsStartWd.iChar, rpsOut.pfld, prec); // 1.4qpg Fix bug (1.4qpf) of repeating morphs multiple times // 1.4qpe Set start of morpheme
					int iEndMorph = rpsEndWd.iChar; // 1.4qpe Use word length for length of morpheme
					if ( rpsEndWd.iChar >= iWdFldEnd ) // 1.4qpe If word is last one in line, use full length of morpheme line
						iEndMorph = rpsStartMorph.pfld->GetLength(); // 1.4qpe
					while ( rpsStartMorph.iChar < iEndMorph ) // 1.4qpe For each morph in morphs of current word, output XML
						{
						CRecPos rpsEndMorph = rpsStartMorph; // 1.4qpe Find end of morph
						if ( !rpsEndMorph.bEndOfField() ) // 1.4qpe If not at end, move one before looking
							rpsEndMorph.iChar++;
						while ( !rpsEndMorph.bEndOfField() && !rpsEndMorph.bAtWdBegInBundle() ) // 1.4qpe While more and not at wd begin bundle, move forward
							rpsEndMorph.iChar++; // 1.4qpe Move forward
						rpsOut = rpsStartMorph; // 1.4qpe
						int iMorphLen = rpsEndMorph.iChar - rpsStartMorph.iChar; // 1.4qpe
						while ( TRUE ) // 1.4qpe While this field is not the last of the bundle, output the morph annotations
							{
							int iLen = iMorphLen; // 1.4qpf Fix bug (1.4qpe) of annotation cut short at end of line
							if ( rpsEndMorph.bEndOfField() ) // 1.4qpf
								iLen = rpsOut.pfld->GetLength(); // 1.4qpf
							sOut = rpsOut.pfld->sContents(); // 1.4qpe Make temp string to output
							sOut = sOut.Mid( rpsOut.iChar, iLen ); // 1.4qpe
							CField fldOut( rpsOut.pfld->pmkr(), sOut ); // 1.4qpe Make temp field to output
							CField* pfldOut = &fldOut; // 1.4qpe
							if ( subsetMarkersToExport.bIncludes(pfldOut->pmkr()) )
								sfs.WriteField(pfldOut, bFirstField);
							if ( !rpsOut.bLastInBundle() ) // 1.4qpe
								rpsOut.NextField(); // 1.4qpe
							else
								break;
							}
						rpsStartMorph.iChar = rpsEndMorph.iChar; // 1.4qpe
						}
					rpsStartWd.iChar = rpsEndWd.iChar; // 1.4qpa
					}
				pfld = rpsOut.pfld; // 1.4qpd
				}
			else
				{
				if ( subsetMarkersToExport.bIncludes(pfld->pmkr()) )
					sfs.WriteField(pfld, bFirstField);
				}
			}
        }
}

int CIndex::iInsertRecord(CKey* pkey, CNumberedRecElPtr* pprelCur)
{
    int iResult=0;
    
    do  {
        CMarker* pmkrRecord = m_pindsetMyOwner->pmkrRecord();
        if ( !pkey->bModalView(CKeyDlg::eInsert, IDD_INSERT_RECORD, pmkrRecord) ) // Get key to insert
            return eCancel;
    
        // Show the results if any records match
		CIndex *pindSearch = this->pindsetMyOwner()->pindRecordOwner(); // 1.6.1cj Search owning index to: Fix bug of not asking about existing record if not sorted by key
        CSearchRecordSet recsetSearch(pindSearch, pkey);  // matching records // 1.6.1cj Change "this" to owning index
//        CSearchRecordSet recsetSearch(this, pkey);  // matching records // 1.6.1cj Maybe change "this" to owning index
        CMultipleMatchList mmtlst;
        CDatabaseRef dbref( pindSearch ); // temporary dbref to keep list users happy // 1.6.1cj Change "this" to owning index
//        CDatabaseRef dbref( this ); // temporary dbref to keep list users happy
        CMultipleMatch* pmmt=NULL;
        BOOL bMatch = recsetSearch.bSearch(&mmtlst, &dbref);
        if (bMatch)
            {
            ASSERT(!mmtlst.bIsEmpty());
            iResult = mmtlst.iModalView_Elements(&pmmt, TRUE, !bReadOnly()); // let user choose from list of matches
    
            if ( iResult == IDOK )
                {
                ASSERT( pmmt );
                pprelCur->Set(pmmt->prel()); // set prel for caller
                return eSearch; // insert turned into a successful search!
                }
            }
        if (!bMatch || iResult == IDC_INSERT)
            { // no matches or user selected INSERT from multiple match dialog, go ahead with insertion
            *pprelCur = prelInsertRecord(pkey->sKey()); // insert the record
            return eInsert;
            }
        
        } while ( iResult == IDC_TRYAGAIN ); // user wants to try search again

    return eCancel;
}

int CIndex::iSearch(CKey* pkey, CNumberedRecElPtr* pprelCur, BOOL bFilter) // 1.5.5b Fix bug of insert duplicate when filtered
{
    int iResult=0;

    do  {
        CMarker* pmkrKey = pmkrPriKey();
        if ( !pkey->bModalView(CKeyDlg::eSearch, IDD_DATABASE_SEARCH, pmkrKey) )
            return eCancel;
    
        // Show the results if other than a single record found
        CSearchRecordSet recsetSearch(this, pkey);  // matching records
        CMultipleMatchList mmtlst;
        CDatabaseRef dbref( this ); // temporary dbref to keep list users happy
        CMultipleMatch* pmmt=NULL;
        if (recsetSearch.bSearch(&mmtlst, &dbref))
            {
            if (mmtlst.lGetCount() > 1) // multiple matches?
                iResult = mmtlst.iModalView_Elements(&pmmt, TRUE, !bReadOnly()); // let user choose from list of matches

            else // return first (and only) match
                {
                iResult = IDOK;
                pmmt = mmtlst.pmmtFirst();
                }

            if ( iResult == IDOK )
                {
                ASSERT( pmmt );
                pprelCur->Set(pmmt->prel()); // set prel for caller
                return eSearch; // search succeeded
                }
            }
        else
            { // no matches
            CNoMatchesDlg dlg( pkey->sKey(), pmkrKey->plng(), !bReadOnly() && !bFilter ); // 1.5.5b Fix bug of insert duplicate when filtered
            iResult = dlg.DoModal();
			pkey->SetKey( dlg.pszKey() );
            }
        
        } while ( iResult == IDC_TRYAGAIN ); // user wants to try search again

    if ( iResult == IDC_INSERT ) // user opted to insert new record 
        {
        *pprelCur = prelInsertRecord(pkey->sKey()); // insert the record
        return eInsert; // search turned into an Insert!
        }

    return eCancel;
}

int CIndex::iSearchFocusTarget(CKey* pkey, CNumberedRecElPtr* pprelCur, BOOL bMatchWhole, BOOL bMultMatchMessage ) // 6.0zf Don't ask multiple matches on parallel jump
{
    // Show the results if multiple records found
    CSearchRecordSet recsetSearch(this, pkey);  // matching records
    CMultipleMatchList mmtlst;
    CDatabaseRef dbref( this ); // temporary dbref to keep list users happy
    CMultipleMatch* pmmt=NULL;
	recsetSearch.SetMatchWhole( bMatchWhole );
	recsetSearch.SetMatchSetting( CMCharOrder::mchtypIgnore ); // Try exact match first
    if ( !recsetSearch.bSearch(&mmtlst, &dbref) )
		{
		recsetSearch.SetMatchSetting( CMCharOrder::mchtypPri ); // If no exact match, try looser match
		if ( !recsetSearch.bSearch(&mmtlst, &dbref) ) // If no match even loose, fail
	        return eCancel;
		}

    int iResult = IDOK; // 1.1kg Don't move if current is already OK
    if ( mmtlst.lGetCount() > 1 ) // multiple matches?
		{
		if ( bMultMatchMessage ) // 6.0zf Don't ask multiple matches on parallel jump
			iResult = mmtlst.iModalView_Elements(&pmmt, TRUE, !bReadOnly()); // let user choose from list of matches
		else // 1.1kg See if current is already OK
			{
			for ( pmmt = mmtlst.pmmtFirst(); pmmt; pmmt = mmtlst.pmmtNext( pmmt ) ) // 1.1kg For each match, see if same as current
				{
				if ( pmmt->prel()->prec() == (*pprelCur)->prec() ) // If cur found, don't look further // 1.5.1nc 
					break;
				}
			if ( !pmmt ) // If cur not found, return first match
				pmmt = mmtlst.pmmtFirst();
			}
		}
    else // return first (and only) match
        pmmt = mmtlst.pmmtFirst();

    if ( iResult == IDOK )
        {
        ASSERT( pmmt );
        pprelCur->Set(pmmt->prel()); // set prel for caller
        return eSearch; // search succeeded
        }

    return eCancel;
}

void CIndex::RecordToBeDeleted(CRecord* prec)
{
    ASSERT( m_nrllstInserted.bIsEmpty() );
    ASSERT( m_nrllstToBeDeleted.bIsEmpty() );
    m_nrllstToBeDeleted.AddRecElPtrs(this, prec);
}


CNumberedRecElPtr CIndex::pnrlFirst() const
{
    return CNumberedRecElPtr(prelFirst(), 0);
}

CNumberedRecElPtr CIndex::pnrlLast() const
{
    return CNumberedRecElPtr(prelLast(), m_lRecEls-1L);
}

CNumberedRecElPtr CIndex::pnrlNext(const CNumberedRecElPtr& pnrlCur) const
{
    return CNumberedRecElPtr(prelNext(pnrlCur), pnrlCur.lNum()+1);
}

CNumberedRecElPtr CIndex::pnrlPrev(const CNumberedRecElPtr& pnrlCur) const
{
    CRecLookEl* prel = prelPrev(pnrlCur);
    if (prel != NULL)
        return CNumberedRecElPtr(prelPrev(pnrlCur), pnrlCur.lNum()-1);
    else
        return CNumberedRecElPtr(NULL, 0L);
}

const CNumberedRecElPtr& CNumberedRecElPtr::operator=(const CNumberedRecElPtr& pnrl)
{
    if ( this != &pnrl )
        Set(pnrl.m_prel, pnrl.m_lNum);

    return *this;   
}


// --------------------------------------------------------------------------



void CIndexPtr::IncrementNumberOfRefs(CIndex* pind)
{
    m_pind = pind;
    if ( m_pind )
        m_pind->IncrementNumberOfRefs();
}

void CIndexPtr::DecrementNumberOfRefs()
{
        if ( m_pind )
            m_pind->DecrementNumberOfRefs();
}


const CIndexPtr& CIndexPtr::operator=(CIndex* pind)
{
    if ( m_pind != pind )
        {
        DecrementNumberOfRefs();
        IncrementNumberOfRefs(pind);
        }
        
    return *this;
}




CIndexSet::CIndexSet(CDatabaseType* ptyp, int iMargin, BOOL bReadOnly,
        BOOL bNotIncludeUndefined)  // 1997-12-18 MRP
{
    m_ptyp = ptyp;
    m_precToBeDeleted = NULL;
    m_iMargin = iMargin;
    m_bReadOnly = bReadOnly;
    m_bDateStampHasFourDigitYear = FALSE;  // 1998-04-21 MRP
    m_bNotIncludeUndefined = bNotIncludeUndefined;  // 1997-12-18 MRP
    ASSERT( m_ptyp );
    if ( m_bReadOnly )
        m_ptyp->IncrementNumberOfRefsFromReadOnlyIndexSets();
    CMarker* pmkrPriKey = ptyp->pmkrRecord();
    CSortOrder* psrt = pmkrPriKey->plng()->psrtDefault();
    const CMarkerRefList* pmrflstSecKeys = &m_ptyp->mrflstSecKeys();
    m_pindRecordOwner = new CIndex(pmkrPriKey, psrt, pmrflstSecKeys,
        s_maxlenSortKeyComponent, FALSE, this, NULL);
    Add(m_pindRecordOwner);
}

CIndexSet::~CIndexSet()
{
    ASSERT( !m_precToBeDeleted );
    if ( m_pindRecordOwner )
        {
        CRecLookEl* prel = m_pindRecordOwner->prelFirst();
        for ( ; prel; prel = m_pindRecordOwner->prelNext( prel ) )
            prel->DeleteRecord(); // Delete record pointed to by pel
        m_pindRecordOwner = NULL;  // Decrement its count to zero
        } 
    if ( m_bReadOnly )
        m_ptyp->DecrementNumberOfRefsFromReadOnlyIndexSets();
}

// couldn't inline because of forward reference to CDatabaseType
CMarker* CIndexSet::pmkrRecord()
{
    return m_ptyp->pmkrRecord();
}

#ifdef typWritefstream // 1.6.4ac
void CIndexSet::WriteProperties(Object_ofstream& obs)
#else
void CIndexSet::WriteProperties(Object_ostream& obs)
#endif
{
    m_pindRecordOwner->WriteProperties(obs);
}


BOOL CIndexSet::bReadIndexProperties(Object_istream& obs,
    CIndexPtr& pind, CIndexPtr& pindUnfiltered)
{
    if ( !obs.bReadBeginMarker(psz_ind) )
        return FALSE;

    // Set default attributes
    CMarkerSet* pmkrset = pmkrRecord()->pmkrsetMyOwner();
    CMarker* pmkrPriKey = NULL;
    Str8 sPriKeySortOrder;
        // We read the sort order name rather than its reference in case
        // its property field precedes the primary marker field.
    BOOL bSortPriKeyFromEnd = FALSE;
    CMarkerRefList mrflstSecKeys;
    CMarker* pmkrSecKey = NULL;
//  Length maxlenSortKeyComponent = 10;
    CFilter* pfil = NULL;

    while ( !obs.bAtEnd() )
        {
        if ( pmkrset->bReadMarkerRef(obs, psz_PriKey, pmkrPriKey) )
            ;
        else if ( obs.bReadString(psz_srtPriKey, sPriKeySortOrder) )
            ;
        else if (obs.bReadBool(psz_SortPriKeyFromEnd, bSortPriKeyFromEnd) )
            ;
//      else if ( obs.bReadLength(psz_maxlenKeyComponent, maxlenSortKeyComponent) )
//          ;
//      else if ( mrflstSecKeys.bReadProperties(obs, psz_mrflstSecKeys, pmkrset) )
        else if ( pmkrset->bReadMarkerRef(obs, psz_SecKey, pmkrSecKey) )
            mrflstSecKeys.AddMarkerRef(pmkrSecKey);
        else if ( pfilset()->bReadFilterRef(obs, pfil) )
            ;
        else if ( obs.bEnd(psz_ind) )
            break;
        }

    if ( pmkrPriKey )
        {
        CLangEnc* plng = pmkrPriKey->plng();
        CSortOrderSet* psrtset = plng->psrtset();
        CSortOrder* psrtPriKey = NULL;
        if ( !sPriKeySortOrder.IsEmpty() )
            psrtPriKey = psrtset->psrtSearch(sPriKeySortOrder);
        if ( !psrtPriKey )
            psrtPriKey = plng->psrtDefault();
            
        CIndex* t_pind = NULL;
        CIndex* t_pindUnfiltered = NULL;
        GetIndex(pmkrPriKey, psrtPriKey,
//          &mrflstSecKeys, maxlenSortKeyComponent, bSortPriKeyFromEnd,
            &mrflstSecKeys, s_maxlenSortKeyComponent, bSortPriKeyFromEnd,
            pfil, &t_pind, &t_pindUnfiltered);
        pind = t_pind;
        pindUnfiltered = t_pindUnfiltered;
        }
    else
        // If the primary marker was not specified, we ignore any other
        // properties and return the index corresponding to the database file.
        pind = pindUnfiltered = pindRecordOwner();

    ASSERT( pind );
    ASSERT( pindUnfiltered );

    return TRUE;
}

void CIndexSet::Write(SF_ostream& sfs, CProgressIndicator& prg) // const // 1.4qpb
{
    WriteSh3Header(sfs);
	sfs.WriteString( m_sHeadInfo ); // 1.4kmb Write head extra info between header and first record // 1.4pcb Change var name
    m_pindRecordOwner->WriteElements(sfs, ptyp(), prg); // 1.4qpb
}

static const char* psz_sh = "sh";
static const char* psz__sh = "_sh";
static const char* psz_DateStamp = "DateStampHasFourDigitYear";
static const char* psz__DateStamp = "_DateStampHasFourDigitYear";
static const char* psz_Underscore = "_"; // 1.4kma

//void CIndexSet::WriteSh3Header(Object_ostream& obs) const
void CIndexSet::WriteSh3Header(SF_ostream& sfs) const
{
    Str8 sHeader = "v3.0  ";
    int i = m_iMargin;
    if (i < 0 || i > 9999)  //garbage check 2-17-98
        i = 400;
    char cBuf[6];
    sprintf( cBuf, "%-4d ", i ); // add margin setting
    sHeader += cBuf;
    sHeader += m_ptyp->sName();
    sfs.WriteInternalField(psz_sh, sHeader);
    
    if ( m_ptyp->pmkrDateStamp() )
        sfs.WriteInternalField(psz_DateStamp, "");  // 1998-04-21 MRP
}

BOOL CIndexSet::s_bReadPropertiesFromSh3Header (istream& ios,
        BOOL bReadOnly, CIndexSet** ppindset, BOOL* pbStringTooLong,
        Str8& sMissingDBType)
// returns FALSE if not a ready-to-load Shoebox 3 database
{
    CNoteList notlst;
    Object_istream obs(ios, notlst);
    if ( !obs.bAtBackslash() )
        return FALSE;

    Str8 sHeader;
    if ( !obs.bReadString(psz__sh, sHeader) ) 
        {
        *pbStringTooLong = obs.bStringTooLong();      // 07-26-1997
        return FALSE;
        }
    // 1998-04-21 MRP: Shoebox 4 (and later) writes this internal property marker. Shoebox 1, 2, and 3 don't and generic Standard Format files won't have it either.
    // 1998-06-05 MRP: Guard against reading when at end of file, which will happen if the database doesn't have any records.
    BOOL bDateStampHasFourDigitYear = FALSE;
    if ( !obs.bAtEnd() )
        obs.bReadBool(psz__DateStamp, bDateStampHasFourDigitYear);

    if (sHeader.GetLength() < 8)  // string too short  2-17-98
        return FALSE;
 
	sHeader.TrimRight(); // 1.4hez Fix bug of not recognizing header
    const char* pszHeader = sHeader;
    if ( strncmp(pszHeader, "v3.0 ", 5) ) // check version number
        return FALSE;

	// Read wrap margin
	int iWrapMarginStart = 5; // 1.4na Fix bug of possible failure on database type with space
	while ( sHeader.GetChar( iWrapMarginStart ) == ' ' ) // 1.4na Find start of wrap margin number
		iWrapMarginStart++; // 1.4na
    int iMargin = atoi( pszHeader + iWrapMarginStart ); // 1.4na // 1.2fw Fix to allow for no extra spaces in database header line // get margin setting
	if ( iMargin < 1 ) // 1.4na Protect against zero wrap margin
		iMargin = 400; // 1.4na Set to default number

    // Read database type
	int iDbTypeStart = iWrapMarginStart; // 1.4na Fix bug of possible failure on database type with space
	while ( iDbTypeStart < sHeader.GetLength() && sHeader.GetChar( iDbTypeStart ) != ' ' ) // 1.4na Find end of wrap margin number
		iDbTypeStart++; // 1.4na
	while ( sHeader.GetChar( iDbTypeStart ) == ' ' ) // 1.4na Find start of database type
		iDbTypeStart++; // 1.4na
    Str8 sDatabaseType = sHeader.Mid( iDbTypeStart ); // 1.4na Pick up database type

    CDatabaseType* ptyp=NULL;
    if ( !Shw_ptypset()->bSearch(sDatabaseType, &ptyp, notlst) ) // Does type exist?
        {           // nope, bail out
        sMissingDBType = sDatabaseType;     // 2-12-1998
        return FALSE; 
        }

    ASSERT( ptyp );
    CDatabaseTypePtr ptypRef = ptyp; // for discard of type if error below

    *ppindset = new CIndexSet(ptyp, iMargin, bReadOnly);
    // 1998-04-22 MRP: The following should become part of the constructor:
    (*ppindset)->m_bDateStampHasFourDigitYear = bDateStampHasFourDigitYear;
    return TRUE;
}

CDatabaseType* CIndexSet::ptyp()
{
    return m_ptyp;
}

CMarkerSet* CIndexSet::pmkrset()
{
    return m_ptyp->pmkrset();
}

CFilterSet* CIndexSet::pfilset()
{
    return m_ptyp->pfilset();
}


    
BOOL CIndexSet::s_bViewIndexProperties(CIndex** ppind, CIndex** ppindUnfiltered)
{
    CSortDlg dlg(ppind, ppindUnfiltered);
    return ( dlg.DoModal() == IDOK );
}


BOOL CIndexSet::s_bViewFilteringProperties(CIndex** ppind, CIndex** ppindUnfiltered)
{
    ASSERT( ppind );
    CIndex* pind = *ppind;
    ASSERT( pind );
    CIndexSet* pindsetMyOwner = pind->pindsetMyOwner();
    CFilterSet* pfilset = pindsetMyOwner->pfilset();
    CFilter* pfil = pind->pfil();
    CFilterSelectionDlg dlg(pfilset, &pfil);
    if ( dlg.DoModal() != IDOK )
        return FALSE;

    pindsetMyOwner->GetIndex(pfil, ppind, ppindUnfiltered);
    
    return TRUE;
}


void CIndexSet::AddRecord(CRecord* prec)
{
    CIndex* pind = pindFirst();
    ASSERT( pind );
    for ( ; pind; pind = pindNext(pind) )
        (void) pind->prelAddRecord(prec);
}

BOOL CIndexSet::bSort()
{
    return m_pindRecordOwner->bSort();
}


CIndex* CIndexSet::pindFind_AddIfNew(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
            CFilter* pfil)
{
    maxlenSortKeyComponent = s_maxlenSortKeyComponent;  // 1996-08-07 MRP
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bEqual(pmkrPriKey, psrtPriKey, pmrflstSecKeys,
                maxlenSortKeyComponent, bSortPriKeyFromEnd, pfil) )
            return pind; // Return an existing index
    
    if ( pfil )
        {
        CIndex* pindUnfiltered = pindFind_AddIfNew(pmkrPriKey,
            psrtPriKey, pmrflstSecKeys,
            maxlenSortKeyComponent, bSortPriKeyFromEnd, NULL);
        pind = new CIndex(pmkrPriKey, psrtPriKey, pmrflstSecKeys,
            maxlenSortKeyComponent, bSortPriKeyFromEnd, this,
            pfil, pindUnfiltered);
        pind->ReFilter();
        }
    else
        {       
        pind = new CIndex(pmkrPriKey, psrtPriKey, pmrflstSecKeys,
            maxlenSortKeyComponent, bSortPriKeyFromEnd, this, NULL, NULL);
        ASSERT( m_pindRecordOwner );
        pind->SortFrom(m_pindRecordOwner);
        }
    
    Add(pind);
    return pind;  // Return a new index added to the list
}

void CIndexSet::GetIndex(CMarker* pmkrPriKey, CSortOrder* psrtPriKey,
            const CMarkerRefList* pmrflstSecKeys, 
            Length maxlenSortKeyComponent, BOOL bSortPriKeyFromEnd,
            CFilter* pfil, CIndex** ppind, CIndex** ppindUnfiltered)
{
    *ppindUnfiltered = pindFind_AddIfNew(pmkrPriKey, psrtPriKey, pmrflstSecKeys,
            maxlenSortKeyComponent, bSortPriKeyFromEnd, NULL);
    *ppind = pindFind_AddIfNew(pmkrPriKey, psrtPriKey, pmrflstSecKeys,
            maxlenSortKeyComponent, bSortPriKeyFromEnd, pfil);
}

void CIndexSet::GetIndex(CFilter* pfil, CIndex** ppind, CIndex** ppindUnfiltered)
{
    // 1995-01-11 MRP: Use the properties of the unfiltered index because
    // deleting a filter in use can already have deleted the filtered one.
    ASSERT( ppindUnfiltered );
    CIndex* pind = *ppindUnfiltered;
    ASSERT( pind );
    CMarker* pmkrPriKey = pind->pmkrPriKey();
    CSortOrder* psrtPriKey = pind->psrtPriKey();
    const CMarkerRefList* pmrflstSecKeys = pind->pmrflstSecKeys();
    Length maxlenSortKeyComponent = pind->maxlenSortKeyComponent();
    BOOL bSortPriKeyFromEnd = pind->bSortPriKeyFromEnd();
    
    GetIndex(pmkrPriKey, psrtPriKey,
        pmrflstSecKeys, maxlenSortKeyComponent, bSortPriKeyFromEnd,
        pfil, ppind, ppindUnfiltered);
}

void CIndexSet::RecordToBeDeleted(CRecord* prec)
{
    ASSERT( !m_precToBeDeleted );
    m_precToBeDeleted = prec;
    ASSERT( m_precToBeDeleted );
    // All _all_ record elements from all indexes referring to this record.
    for ( CIndex* pind = pindFirst(); pind; pind = pindNext(pind) )
        pind->RecordToBeDeleted(prec);
}

void CIndexSet::MoveElementsFrom(CIndex* pind)
{
    ASSERT( pind );
    pind->MoveElementsTo(&m_rellstObsolete);
}


BOOL CIndexSet::bUsesMarker(const CMarker* pmkr)
{
    return m_pindRecordOwner->bUsesMarker(pmkr);
}


BOOL CIndexSet::bUpdateSortKeys(const CNumberedRecElPtr& prelCur)
{
    BOOL bCurToBeDeleted = FALSE;
    ASSERT( prelCur );
    CRecord* precModified = prelCur->prec();
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bUpdateSortKeys(prelCur) )
            bCurToBeDeleted = TRUE;

    return bCurToBeDeleted;
}


// Must the index which owns the records be resorted first???
// If another index has equal keys, then their relative order
// is to be determined by their order in the owning index,
// i.e. as if creating the index when reading the file.
// Make sure that this approach produces that same ordering!!!

void CIndexSet::MarkerUpdated(CMarkerUpdate& mup)
{
    // Update unfiltered indexes for filtered indexes, since the latter
    // depend upon the former for the order of their record elements.
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( !pind->bUsingFilter() )
            pind->MarkerUpdated(mup);
        
    pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bUsingFilter() )
            pind->MarkerUpdated(mup);
}

void CIndexSet::FilterUpdated(CFilterUpdate& fup)
{
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bUsingFilter() )
            pind->FilterUpdated(fup);
}

void CIndexSet::LangEncUpdated(CLangEncUpdate& lup)
{
    // Update unfiltered indexes for filtered indexes, since the latter
    // depend upon the former for the order of their record elements.
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( !pind->bUsingFilter() )
            pind->LangEncUpdated(lup);
        
    pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bUsingFilter() )
            pind->LangEncUpdated(lup);
}

void CIndexSet::SortOrderUpdated(CSortOrderUpdate& sup)
{
    // Update unfiltered indexes for filtered indexes, since the latter
    // depend upon the former for the order of their record elements.
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( !pind->bUsingFilter() )
            pind->SortOrderUpdated(sup);
        
    pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        if ( pind->bUsingFilter() )
            pind->SortOrderUpdated(sup);
}

void CIndexSet::DoneUpdatingViews()
{
    m_fldlstToDelete.DeleteAll();
    m_rellstObsolete.DeleteAll();
    CIndex* pind = pindFirst();
    for ( ; pind; pind = pindNext(pind) )
        pind->DoneUpdatingViews();
    if ( m_precToBeDeleted )
        {
        delete m_precToBeDeleted;
        m_precToBeDeleted = NULL;
        }
}

// ==========================================================================

CIndexRecordRef::CIndexRecordRef(CIndex* pind, CRecLookEl* prel) :
    m_pind(pind),
    m_prel(prel)
{
    ASSERT( m_pind );
    ASSERT( m_prel );
}


// --------------------------------------------------------------------------

CIndexRecordRefList::CIndexRecordRefList()
{
}


void CIndexRecordRefList::AddIndexRecordRefs(CIndex* pind, CRecord* prec)
{
    ASSERT( pind );
    ASSERT( prec );
    CRecLookEl* prel = pind->prelFirst();
    for ( ; prel ; prel = pind->prelNext(prel) )
        if ( prel->prec() == prec )
            Add(new CIndexRecordRef(pind, prel));
            
    ASSERT( !bIsEmpty() );
}

BOOL CIndexRecordRefList::bIsMember(const CRecLookEl* prel) const
{
    for ( CIndexRecordRef* prrf = prrfFirst(); prrf ; prrf = prrfNext(prrf) )
        if ( prel == prrf->prel() )
            return TRUE;
    
    return FALSE;
}

// 1999-05-25 TLB & MRP: Can't use strcmp anymore because UIdMChar is two bytes long.
static inline BOOL bEqual(const UIdMChar* puchA, const UIdMChar* puchB)
{ 
    for (; *puchA && *puchB; puchA++, puchB++)
        {
        if ( *puchA != *puchB )
            return FALSE;
        }
    return ( *puchA == *puchB );
}

CRecLookEl* CIndexRecordRefList::prelFind_RemoveIfFound(UIdMChar* puchSortKey,
        CField* pfldPriKey)
{
    ASSERT( puchSortKey );
    CRecLookEl* prel = NULL;
    
    CIndexRecordRef* prrf = prrfFirst();
    for ( ; prrf; prrf = prrfNext(prrf) )
        if ( bEqual(prrf->puchSortKey(), puchSortKey) &&
                prrf->prel()->pfldPriKey() == pfldPriKey )
            {
            prel = prrf->prel();
            Delete(&prrf);
            break;
            }
    
    return prel;
}


// ********* CIndex
#ifdef _DEBUG
void CIndex::AssertValid() const
{
    ASSERT( m_pindsetMyOwner );

    ASSERT( m_pmkrPriKey );
    CMarkerSet* pmkrset = m_pindsetMyOwner->pmkrRecord()->pmkrsetMyOwner();
    ASSERT( pmkrset->bIsMember(m_pmkrPriKey) );
    ASSERT( m_psrtPriKey );
    CSortOrderSet* psrtset = m_pmkrPriKey->plng()->psrtset();
    ASSERT( psrtset->bIsMember(m_psrtPriKey) );
    
    ASSERT( m_maxsizSortKey ==
        ((Length)m_mrflstSecKeys.lGetCount()+1)*(m_maxlenSortKeyComponent+1) );
    ASSERT( m_puchSortKeyA );
    ASSERT( m_puchSortKeyB );


    // Note: Here is a scenario when the following assertion code
    // should not be executed: if a database is being opened and
    // the user clicks Cancel on the Shoebox status bar,
    // the records will not have been sorted when the CIndex is destroyed.
    // This logic also causes the order of filtered indexes not to be checked.
    if ( m_bHasBeenSorted )  // 2000-06-21 MRP
        {
        // The record elements must be in non-decreasing order
        CRecLookEl* prelLesser = prelFirst();
        long lActual = ( prelLesser ? 1 : 0 );
        CRecLookEl* prelGreater = NULL;
        while ( prelLesser && (prelGreater = prelNext(prelLesser)) )
            {
            lActual += 1;
            int i = iCompare(prelLesser, prelGreater);
            ASSERT( i <= 0 );
            prelLesser = prelGreater;
            prelGreater = prelNext(prelLesser);
            }
        
        ASSERT( lActual == m_lRecEls );
        }
}



// ****************** CIndexSet

void CIndexSet::AssertValid() const
{
    ASSERT( bIsMember(m_pindRecordOwner) );
    for ( CIndex* pind = pindFirst(); pind; pind = pindNext(pind) )
        pind->AssertValid();
}
#endif  // _DEBUG


CNumberedRecElPtrListEl::CNumberedRecElPtrListEl(CNumberedRecElPtr& pnrl)
{
    m_pnrl = pnrl;
}


BOOL CNumberedRecElPtrList::bIsMember(const CNumberedRecElPtr prel) const
{
    for ( CNumberedRecElPtrListEl* pprnl = pnrlFirst(); pprnl ; pprnl = pnrlNext(pprnl) )
        if ( prel == pprnl->m_pnrl) 
            return TRUE;
    
    return FALSE;
}


void CNumberedRecElPtrList::AddRecElPtrs(CIndex* pind, CRecord* prec)
{
    ASSERT( pind );
    ASSERT( prec );
    CNumberedRecElPtr prel = pind->pnrlFirst();
    for ( ; prel ; prel = pind->pnrlNext(prel) )
        if ( prel->prec() == prec )
            Add(new CNumberedRecElPtrListEl(prel));
}


CNumberedRecElPtr CNumberedRecElPtrList::prelFind_RemoveIfFound(UIdMChar* puchSortKey,
            CField* pfldPriKey)
{
    ASSERT( puchSortKey );
    CNumberedRecElPtr prel; // Constructor defaults to NULL, 0L
    
    CNumberedRecElPtrListEl* pnrl = pnrlFirst();
    for ( ; pnrl; pnrl = pnrlNext(pnrl) )
        if ( bEqual(pnrl->puchSortKey(), puchSortKey) &&
                pnrl->prel()->pfldPriKey() == pfldPriKey )
            {     
            prel = pnrl->prel();
            Delete(&pnrl);
            break;
            }
    
    return prel;
}
    
void CNumberedRecElPtrList::AddRecElPtr(CNumberedRecElPtr& prel)
{
    // 1. Increment elements already in the list
//  CNumberedRecElPtrListEl* pnrl = pnrlFirst();    
//  for( ; pnrl; pnrl = pnrlNext(pnrl) )
//      if ( prel.lNum() <= pnrl->prel().lNum() )
//          {
//          CNumberedRecElPtr& prelSet = (CNumberedRecElPtr&)pnrl->prel();
//          prelSet.Set(prelSet, pnrl->prel().lNum() + 1L);
//          }
//
//  // 2. Then add this one in numeric order
    CNumberedRecElPtrListEl* pnrlAdd = new CNumberedRecElPtrListEl(prel);
//  pnrl = pnrlFirst(); 
//  for( ; pnrl; pnrl = pnrlNext(pnrl) )
//      if ( prel.lNum() < pnrl->prel().lNum() )
//          break;
//  if ( pnrl )
//      InsertBefore(pnrl, pnrlAdd);
//  else
        Add(pnrlAdd);
}

void CNumberedRecElPtrListEl::IncrementRecElNumber(
        const CNumberedRecElPtrList& nrllstInserted)
{
    CNumberedRecElPtrListEl* pnrl = nrllstInserted.pnrlFirst(); 
    for( ; pnrl; pnrl = nrllstInserted.pnrlNext(pnrl) )
        if ( pnrl->prel().lNum() <= m_pnrl.lNum() )
            m_pnrl.Set(m_pnrl, m_pnrl.lNum() + 1L);
}

void CNumberedRecElPtrList::IncrementRecElNumbers(
        const CNumberedRecElPtrList& nrllstInserted)
{
    CNumberedRecElPtrListEl* pnrl = pnrlFirst();    
    for( ; pnrl; pnrl = pnrlNext(pnrl) )
        pnrl->IncrementRecElNumber(nrllstInserted);
}

CNumberedRecElPtrList::~CNumberedRecElPtrList()
{
}

#ifdef _DEBUG
void CNumberedRecElPtrList::AssertValid() const
{
}
#endif  // _DEBUG


// Reduce the memory consumed by the DEBUG version by not remembering
// the source line where CRecords, CFields, or CRecLookEls were allocated. 
#undef new

// Add new at end and let it be sorted later
CNumberedRecElPtr CIndex::prelAddRecEl(const UIdMChar* puchSortKey,
        CRecord* prec, CField* pfld)
{   // *** Maybe this code can use Trie access for higher speed insertion
    // Add the element at the end now and let this index be sorted later.
    CRecLookEl* prelAdded = new CRecLookEl(puchSortKey, prec, pfld);
    CDblList::Add( prelAdded );
    return CNumberedRecElPtr(prelAdded, m_lRecEls++);
}

void CIndex::AddRecEl_CopyFrom(const CRecLookEl* prel)
{
    CRecLookEl* prelAdded = new CRecLookEl(prel->puchSortKey(),
        prel->prec(), prel->pfldPriKey());  // copy constructor
    CDblList::Add( prelAdded );
    m_lRecEls += 1;
}

CNumberedRecElPtr CIndex::prelInsertRecord(const Str8& sKeyToAdd)
{
    CMarker* pmkrRecord = m_pindsetMyOwner->pmkrRecord();
    CField* pfldKey = new CField(pmkrRecord, sKeyToAdd); // Create a field with the record marker
    CRecord* prec = new CRecord(pfldKey); // Create the record based on the key field *** AB Must add fields based on template

    CRecPos rps( (*pfldKey).GetLength(), pfldKey, prec ); // insert template at end of key field
    prec->Paste( &rps, m_pindsetMyOwner->ptyp()->sTemplate(), m_pindsetMyOwner->pmkrset() );

    CIndex* pind = m_pindsetMyOwner->pindFirst();
    for ( ; pind; pind = m_pindsetMyOwner->pindNext(pind) )
        if ( pind != this )
            (void) pind->prelInsertRecord(prec, FALSE);
            
    CNumberedRecElPtr prel = prelInsertRecord(prec, TRUE);
    
    ASSERT( prel );
    return prel;
}

CNumberedRecElPtr CIndex::prelCopyRecord (const CRecord* precSrc)
{
    ASSERT (precSrc != NULL);
    CField* pfld = precSrc->pfldFirst(); // Get a pointer to the key field
    CRecord* precDst = new CRecord(new CField(*pfld)); // Create the record based on the key field

    pfld = precSrc->pfldNext(pfld);
    while (pfld != NULL) {
        precDst->Add(new CField(*pfld));
        pfld = precSrc->pfldNext(pfld);
    }

    CIndex* pind = m_pindsetMyOwner->pindFirst();
    for ( ; pind; pind = m_pindsetMyOwner->pindNext(pind) )
        if ( pind != this )
            (void) pind->prelInsertRecord(precDst, FALSE);
            
    CNumberedRecElPtr prel = prelInsertRecord(precDst, TRUE);
    
    ASSERT( prel );
    return prel;
}

// Add new in correct order, after last with same key
CNumberedRecElPtr CIndex::prelInsertRecEl(const UIdMChar* puchSortKey,
        CRecord* prec, CField* pfld)
{
    CRecLookEl* prelAdded = new CRecLookEl(puchSortKey, prec, pfld);
    return prelInsertRecEl(prelAdded);
}

CNumberedRecElPtr CIndex::prelInsertRecEl(CRecLookEl* prelAdded)
{
    // For all, starting from last      
    for ( CNumberedRecElPtr prelCur = pnrlLast(); prelCur; prelCur = pnrlPrev( prelCur ) )
        // If same or greater, insert after
        if ( iCompare( prelAdded, prelCur ) >= 0 )
//      if ( iCompare( prelAdded->puchSortKey(), prelCur->puchSortKey(),
//              CMCharOrder::matsetEvenIgnore, TRUE) >= 0 )
            {
            CDblList::InsertAfter( prelCur, prelAdded ); 
            m_lRecEls += 1;
            return CNumberedRecElPtr(prelAdded, prelCur.lNum() + 1);
            }

    // If place not found, insert at top
    if ( bIsEmpty() ) // If list is empty, use Add
        CDblList::Add( prelAdded );
    else
        CDblList::InsertBefore( prelFirst(), prelAdded );
    m_lRecEls += 1;
    
    return CNumberedRecElPtr(prelAdded, 0L);
}
