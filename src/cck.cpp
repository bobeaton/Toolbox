// cck.cpp  Consistency Checks

#include "stdafx.h"
#include "toolbox.h"
#include "cck.h"
#include "shwdoc.h"  // CShwDoc
#include "shwview.h"  // CShwView
#include "progress.h"

#include "cck_d.h"  // CCheckConsistencyDlg

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// ==========================================================================

CFieldConsistencyChecker::CFieldConsistencyChecker(CShwView* pviewMyOwner) :
    m_pviewMyOwner(pviewMyOwner),
    m_subsetFieldsToCheck(pviewMyOwner->pdoc()->pmkrset())
{
    ASSERT( m_pviewMyOwner );
    m_pcks = NULL;
    Init();
}

void CFieldConsistencyChecker::Init()
{
    m_bShowProperties = TRUE;
    m_bSingleField = FALSE;     // Default: All Fields
    m_bSelectedFields = FALSE;  // Default: All Fields
#ifdef RECORD_ORDER_CHECKING_ENABLED
    m_bRecordByRecord = FALSE;  // Default: Check one field marker at-a-time
#endif
    m_bCheckDataDefn = TRUE;
    m_bCheckRangeSets = TRUE;
    m_bCheckRefConsistency = TRUE;
}

CFieldConsistencyChecker::~CFieldConsistencyChecker()
{
    delete m_pcks;
}

static const char* psz_cck = "cck";
static const char* psz_bSingleField = "SingleField";
static const char* psz_mkrSingle = "mkrSingleField";
static const char* psz_bSelectedFields = "SelectedFields";
static const char* psz_subsetFTC = "subsetSelectedFields";
static const char* psz_bSkipDataDefn = "DontCheckDataDefn";
static const char* psz_bSkipRangeSets = "DontCheckRangeSets";
static const char* psz_bSkipRefConsistency = "DontCheckRefConsistency";

#ifdef prjWritefstream // 1.6.4aa 
void CFieldConsistencyChecker::WriteProperties(Object_ofstream& obs) const
#else
void CFieldConsistencyChecker::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_cck);

    obs.WriteBool(psz_bSingleField, m_bSingleField);
    obs.WriteString(psz_mkrSingle, m_sSingleFieldMarker);
    obs.WriteBool(psz_bSelectedFields, m_bSelectedFields);
    m_subsetFieldsToCheck.WriteProperties(obs);

    obs.WriteBool(psz_bSkipDataDefn, !m_bCheckDataDefn);
    obs.WriteBool(psz_bSkipRangeSets, !m_bCheckRangeSets);
    obs.WriteBool(psz_bSkipRefConsistency, !m_bCheckRefConsistency);

	obs.WriteEndMarker(psz_cck);
}

BOOL CFieldConsistencyChecker::s_bReadProperties(Object_istream& obs, CShwView* pviewMyOwner, CFieldConsistencyChecker** ppcck)
{
    if ( !obs.bReadBeginMarker(psz_cck) )
        return FALSE;

    ASSERT(ppcck);
	ASSERT(!*ppcck);
    if ( *ppcck )
        delete *ppcck; // This should never happen!
    *ppcck = new CFieldConsistencyChecker( pviewMyOwner );
    ASSERT(*ppcck);
    (*ppcck)->ReadProperties(obs);

    return TRUE;
}

void CFieldConsistencyChecker::ReadProperties(Object_istream& obs)
{
    BOOL b;

    while ( !obs.bAtEnd() )
        {
	    if ( obs.bReadBool( psz_bSingleField, m_bSingleField ) )
            m_bSelectedFields = !m_bSingleField;
        else if ( obs.bReadString( psz_mkrSingle, m_sSingleFieldMarker ) )
            ;
        else if ( obs.bReadBool( psz_bSelectedFields, m_bSelectedFields ) )
            m_bSingleField = !m_bSelectedFields;
        else if ( m_subsetFieldsToCheck.bReadProperties(obs) )
			;
        else if ( obs.bReadBool( psz_bSkipDataDefn, b ) )
            m_bCheckDataDefn = !b;
        else if ( obs.bReadBool( psz_bSkipRangeSets, b ) )
            m_bCheckRangeSets = !b;
        else if ( obs.bReadBool( psz_bSkipRefConsistency, b ) )
            m_bCheckRefConsistency = !b;
        else if ( obs.bEnd(psz_cck) )
            break;
        }
}

BOOL CFieldConsistencyChecker::bCheckConsistency(CIndex* pind,
        CNumberedRecElPtr pnrl, CField* pfld, BOOL bContinueCheckInProgress)
{
    ASSERT( pind );
    ASSERT( pind->pindsetMyOwner() == m_pviewMyOwner->pdoc()->pindset() );
    if ( !m_pcks || !bContinueCheckInProgress )
        {
        // Starting a new check, not continuing an interrupted one
        if ( m_bShowProperties )
            {
            CCheckConsistencyDlg dlg(this, pind);
            if ( dlg.DoModal() != IDOK )
                return FALSE;  // Cancel
            }

        if ( m_pcks )
            delete m_pcks;

#ifdef RECORD_ORDER_CHECKING_ENABLED
        if ( m_bRecordByRecord )
            m_pcks = new CRecordOrderCheckingState(this);
        else
#endif
        m_pcks = new CMarkerOrderCheckingState(this);
        }

    BOOL bConsistent = m_pcks->bCheckConsistency(pind, pnrl, pfld);

    if ( m_pcks->bCancelledAfterInconsistencyFound() )
        return FALSE;  // Keep the state of the interrupted check

    if ( bConsistent )
        AfxMessageBox(_("Toolbox has completed the checks you requested."), MB_ICONINFORMATION);

    delete m_pcks;
    m_pcks = NULL;
    return bConsistent;
}

BOOL CFieldConsistencyChecker::bRecheckConsistency(CIndex* pind)
{
	CNumberedRecElPtr pnrl;
	CField* pfld = NULL;
    return bCheckConsistency(pind, pnrl, pfld, FALSE);
}

BOOL CFieldConsistencyChecker::bCheckConsistencyBeforeExporting(CIndex* pind)
{
    // Export Process Properties
    // [X] Before exporting, check consistency
    // Don't show the dialog box.
    // Check all fields in database order.
    Init();
    m_bShowProperties = FALSE;
#ifdef RECORD_ORDER_CHECKING_ENABLED
	// m_bRecordByRecord = TRUE;
#endif
    CNumberedRecElPtr pnrl;
	CField* pfld = NULL;
    return bCheckConsistency(pind, pnrl, pfld, FALSE);
}

// --------------------------------------------------------------------------

CCheckingState::CCheckingState(CFieldConsistencyChecker* pcckMyOwner) :
    m_pcckMyOwner(pcckMyOwner)
{
    ASSERT( m_pcckMyOwner );
    m_pview = m_pcckMyOwner->pviewMyOwner();
    m_sFileName = sGetFileName(  sUTF8( m_pview->pdoc()->GetTitle() ) ); // 1.4qvj Upgrade sGetFileName for Unicode build
#ifdef RECORD_ORDER_CHECKING_ENABLED
    m_bRecordByRecord = m_pcckMyOwner->m_bRecordByRecord;
#endif
    m_nInconsistenciesSkipped = 0;
    m_bCancelledAfterInconsistencyFound = FALSE;
}

CCheckingState::~CCheckingState()
{
}

void CCheckingState::IncrementInconsistenciesSkipped()
{
    m_nInconsistenciesSkipped++;
}

BOOL CCheckingState::bCancelledAfterInconsistencyFound() const
{
    return m_bCancelledAfterInconsistencyFound;
}

BOOL CCheckingState::bMarkerToCheck(CMarker* pmkr)
{
    ASSERT( pmkr );
    if ( m_pcckMyOwner->bSingleField() && !bEqual(pmkr->sMarker(), m_pcckMyOwner->sSingleFieldMarker()) )
        return FALSE;

    if ( m_pcckMyOwner->bSelectedFields() && !m_pcckMyOwner->psubsetFieldsToCheck()->bIncludes(pmkr) )
        return FALSE;

    if ( !pmkr->bHasFieldRefs() )
        return FALSE;

    return TRUE;
}

BOOL CCheckingState::bNextField(CIndex* pind,
        CNumberedRecElPtr& pnrl, CField*& pfld)
{
    ASSERT( pnrl );
    ASSERT( pfld );

    // The next field in the current record [element]
    pfld = pnrl->prec()->pfldNext(pfld);
    if ( pfld )
        return TRUE;

    pnrl = m_pview->pnrlNextToCheck(pnrl);  // 1999-06-28 MRP
    if ( !pnrl )
        return FALSE;  // At the end of the database

    // The first field in the next record [element]
    pfld = pnrl->prec()->pfldFirst();
    return TRUE;
}

void CCheckingState::CancelledWhileChecking(CNumberedRecElPtr& pnrl)
{
    m_pview->ConsistencyCheckingCancelled(pnrl);
}

// --------------------------------------------------------------------------

CAssociationEl::CAssociationEl(const Str8& sLookup, const Str8& sAssociation) : // *** Constructor with content
        CSetEl( sLookup ),
        m_sAssociation(sAssociation)
{
    ASSERT(!sLookup.IsEmpty());
    ASSERT(sLookup != sAssociation);
}

void CAssociationEl::CascadeAssociation(const Str8& sOldAssociation, const Str8& sNewAssociation)
{
    if ( m_sAssociation == sOldAssociation )
        {
        //Need to "cascade" this element to the new association value
        ASSERT(sKey() != sNewAssociation);
        m_sAssociation = sNewAssociation;
        }
}

BOOL CAssociationEl::bMatch(const Str8& sLookup) const
{
    return (sKey() == sLookup);
}

BOOL CAssociationEl::bMatch(const Str8& sLookup, Str8& sAssociation) const
{
    if ( sKey() == sLookup )
        {
        sAssociation = m_sAssociation;
        return TRUE;
        }
    return FALSE;
}

// --------------------------------------------------------------------------

// Perform a search and retrieve association
BOOL CAssociationSet::bLookup(const char* pszLookup, Str8& sAssociation) const
{
    const CAssociationEl* pase = (const CAssociationEl*) pselSearch( pszLookup, whole );
    if ( pase != NULL )
        {
        sAssociation = pase->m_sAssociation;
        return TRUE;
        }
/*
    for (const CAssociationEl* pase = paseFirst(); pase; pase = paseNext(pase))
        {
        if ( pase->bMatch(sLookup, sAssociation) )
            return TRUE;
        }
*/
    return FALSE;
}

CAssociationSet::EAddReplaceAllResult CAssociationSet::araAddAssociation(const Str8& sLookup, Str8 sAssociation)
{
    // To understand this function, consider the following
    // Existing associations: b -> c
    //                        d -> c
    //                        e -> m
    if ( sLookup.IsEmpty() )
        // If requested association is "" -> n, we can't do it.
        return araCallerError;
    if ( sLookup == sAssociation )
        // If requested association is n -> n, we can't do it.
        return araCallerError;
    Str8 sExistingAssoc;
    for (const CAssociationEl* pase = paseFirst(); pase; pase = paseNext(pase))
        {
        if ( pase->bMatch(sLookup) )
            {
            // If requested association is b -> n, we can't do it.
            // This is an attempt to redefine an association, which shouldn't
            // be able to happen explicitly (only by cascading).
            return araCallerError;
            }
        if ( pase->bMatch(sAssociation, sExistingAssoc) )
            {
            if ( sExistingAssoc == sLookup )
                {
                // If requested association is c -> b, we can't do it.
                // This is an an attempt to create a circular association.
                return araCircularReplacement;
                }
            else
                {
                // If requested association is a -> b, we will actually add a -> c
                // since b is already supposed to turn into c
                sAssociation = sExistingAssoc;
                }
            }
        }

    // Now we have either a -> c
    //                 or a -> n
    //                 or c -> n
    // Before adding this new association, we need to check for the case of c -> n,
    // because any exisiting associations that change into c must be "cascaded"
    // so that they go to n instead.
    for (CAssociationEl* paseCasc = paseFirst(); paseCasc; paseCasc = paseNext(paseCasc))
        {
        paseCasc->CascadeAssociation(sLookup, sAssociation);
        }

    CAssociationEl* paseNew = new CAssociationEl(sLookup, sAssociation);

    ASSERT(paseNew);
    Add(paseNew);
    return (paseNew) ? araAdded : araSystemError;
}

// --------------------------------------------------------------------------

CMarkerOrderCheckingState::CMarkerOrderCheckingState(CFieldConsistencyChecker* pcckMyOwner) :
    CCheckingState(pcckMyOwner)
{
#ifdef RECORD_ORDER_CHECKING_ENABLED
    ASSERT( !m_bRecordByRecord );
#endif
    m_pmkrset = m_pview->pdoc()->ptyp()->pmkrset();
    m_bSkipAllMissingData = FALSE;
}

CMarkerOrderCheckingState::~CMarkerOrderCheckingState()
{
}

BOOL CMarkerOrderCheckingState::bSkipAllElement(const CMarker* const pmkr, const Str8& s) const
{
    if ( s.IsEmpty() )
        return m_bSkipAllMissingData;
    return m_datsetSkipAll.bMatch(s);
}

void CMarkerOrderCheckingState::AddSkipAllElement(const CMarker* const pmkr, const Str8& s)
{
    if ( s.IsEmpty() )
        m_bSkipAllMissingData = TRUE;
    else if ( !m_datsetSkipAll.bMatch(s) )
        m_datsetSkipAll.bAdd(s);
}

BOOL CMarkerOrderCheckingState::bReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) const
{
    return m_asetReplaceAll.bLookup(s, sReplacement);
}

CAssociationSet::EAddReplaceAllResult CMarkerOrderCheckingState::araAddReplaceAllElement(const CMarker* const pmkr,
                                                                                         const Str8& s,
                                                                                         Str8& sReplacement)
{
    return m_asetReplaceAll.araAddAssociation(s, sReplacement);
}

BOOL CMarkerOrderCheckingState::bCheckConsistency(CIndex* pind,
        CNumberedRecElPtr pnrl, CField* pfld)
{
    ASSERT( pind );
    ASSERT( pind->pindsetMyOwner() == m_pview->pdoc()->pindset() );

    CMarker* pmkr = NULL;
    if ( m_bCancelledAfterInconsistencyFound )
        {
        m_bCancelledAfterInconsistencyFound = FALSE;
        pmkr = m_pmkrset->pmkrSearch(m_sInconsistencyMarker);
        }
    else
        {
        bResetToBeginning(pind, pnrl, pfld);

        if ( m_pcckMyOwner->bSingleField() )
            pmkr = m_pmkrset->pmkrSearch(m_pcckMyOwner->sSingleFieldMarker());
        else
            {
            pmkr = m_pmkrset->pmkrFirst();
            while ( pmkr && !bMarkerToCheck(pmkr) )
                pmkr = m_pmkrset->pmkrNext(pmkr);
            }
        }

    if ( !pmkr || !pnrl )
        return TRUE;

    do
        {
        ASSERT( pnrl );
        const Str8& sMarker = pmkr->sMarker();
        CProgressIndicator prg(pind->lNumRecEls(), NULL, sMarker, m_sFileName, TRUE); // 1.4ad Eliminate resource messages sent to progress bar

        if ( !prg.bUpdateProgress(pnrl.lNum()) )
            {
            CancelledWhileChecking(pnrl);
            return FALSE;
            }

        if ( pmkr->bHasConsistencyCheckConditions( bCheckDataDefn(), bCheckRangeSets(), bCheckRefConsistency()) )
            {
            do
                {
                ASSERT( pfld );
                if ( pfld->pmkr() == pmkr && !pnrl->prec()->bCheckFieldConsistency(pfld, pnrl, m_pview, this) )
                    {
                    CancelledAfterInconsistencyFound(pmkr, pnrl);
                    return FALSE;
                    }

                if ( !prg.bUpdateProgress(pnrl.lNum()) )
                    {
                    CancelledWhileChecking(pnrl);
                    return FALSE;
                    }

                } while ( bNextField(pind, pnrl, pfld) );
            }
        
        m_datsetSkipAll.DeleteAll();  // Skip All and Replace All
        m_bSkipAllMissingData = FALSE;
        m_asetReplaceAll.DeleteAll(); // apply to a specific marker

        } while ( bNextMarker(pmkr) && bResetToBeginning(pind, pnrl, pfld) );

    return TRUE;
}

void CMarkerOrderCheckingState::CancelledAfterInconsistencyFound(CMarker* pmkr,
        CNumberedRecElPtr& pnrl)
{
    ASSERT( pmkr );
    m_sInconsistencyMarker = pmkr->sMarker();
    m_bCancelledAfterInconsistencyFound = TRUE;
    m_pview->ConsistencyCheckingCancelled(pnrl);
}

BOOL CMarkerOrderCheckingState::bNextMarker(CMarker*& pmkr)
{
    ASSERT( pmkr );

    if ( m_pcckMyOwner->bSingleField() )
        {
        ASSERT( bEqual(pmkr->sMarker(), m_pcckMyOwner->sSingleFieldMarker()) );
        return FALSE;
        }

    do
        pmkr = m_pmkrset->pmkrNext(pmkr);
        while ( pmkr && !bMarkerToCheck(pmkr) );

    return (pmkr)? TRUE: FALSE;
}

BOOL CMarkerOrderCheckingState::bResetToBeginning(CIndex* pind,
        CNumberedRecElPtr& pnrl, CField*& pfld)
{
    pnrl = pind->pnrlFirst();
    if ( pnrl )
        pfld = pnrl->prec()->pfldFirst();
    else
        pfld = NULL;
    return TRUE;
}

// --------------------------------------------------------------------------
#ifdef RECORD_ORDER_CHECKING_ENABLED
CRecordOrderCheckingState::CRecordOrderCheckingState(CFieldConsistencyChecker* pcckMyOwner) :
    CCheckingState(pcckMyOwner)
{
    ASSERT( m_bRecordByRecord );
}

CRecordOrderCheckingState::~CRecordOrderCheckingState()
{
}

BOOL CRecordOrderCheckingState::bSkipAllElement(const CMarker* const pmkr, const Str8& s) const
{
    ASSERT( FALSE );
    return FALSE;
}

void CRecordOrderCheckingState::AddSkipAllElement(const CMarker* const pmkr, const Str8& s)
{
    ASSERT( FALSE );
}

BOOL CRecordOrderCheckingState::bReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement) const
{
    ASSERT( FALSE );
    return FALSE;
}

void CRecordOrderCheckingState::AddReplaceAllElement(const CMarker* const pmkr, const Str8& s, Str8& sReplacement)
{
    ASSERT( FALSE );
}

BOOL CRecordOrderCheckingState::bCheckConsistency(CIndex* pind,
        CNumberedRecElPtr pnrl, CField* pfld)
{
    ASSERT( pind );
    ASSERT( pind->pindsetMyOwner() == m_pview->pdoc()->pindset() );

    if ( m_bCancelledAfterInconsistencyFound )
        m_bCancelledAfterInconsistencyFound = FALSE;
    else
        {
        pnrl = pind->pnrlFirst();
        if ( pnrl )
            pfld = pnrl->prec()->pfldFirst();
        else
            pfld = NULL;
        }

    if ( !pnrl )
        return TRUE;

    CProgressIndicator prg(pind->lNumRecEls(), NULL, m_sFileName, TRUE); // 1.4ad Eliminate resource messages sent to progress bar

    if ( !prg.bUpdateProgress(pnrl.lNum()) )
        {
        CancelledWhileChecking(pnrl);
        return FALSE;
        }

    do
        {
        ASSERT( pfld );
        if ( bFieldToCheck(pfld) && !bConsistentField(pfld, pnrl) )
            {
            CancelledAfterInconsistencyFound(pnrl);
            return FALSE;
            }

        if ( !prg.bUpdateProgress(pnrl.lNum()) )
            {
            CancelledWhileChecking(pnrl);
            return FALSE;
            }

        } while ( bNextField(pind, pnrl, pfld) );

    return TRUE;
}

BOOL CRecordOrderCheckingState::bConsistentField(CField* pfld,
        CNumberedRecElPtr& pnrl)
{
    ASSERT( pfld );
    CMarker* pmkr = pfld->pmkr();

    if ( m_bCheckDataDefn && pfld->bValidItemCount() )


    if ( m_bCheckRangeSets &&
            pmkr->bRangeSetEnabled() &&
            !pmkr->prngset()->bConsistent(pfld, pnrl, m_pview, this) )
        return FALSE;  // Cancel

    // 1999-08-12 TLB
    if ( m_bCheckRefConsistency && pmkr->pjmpPrimary() )
        return pmkr->pjmpPrimary()->bCheckReferentialIntegrity(pfld, pnrl, m_pview, this);

    return TRUE;   
}

BOOL CRecordOrderCheckingState::bFieldToCheck(CField* pfld)
{
    ASSERT( pfld );
    return bMarkerToCheck(pfld->pmkr());
}

void CRecordOrderCheckingState::CancelledAfterInconsistencyFound(CNumberedRecElPtr& pnrl)
{
    m_bCancelledAfterInconsistencyFound = TRUE;
    m_pview->ConsistencyCheckingCancelled(pnrl);
}
#endif // RECORD_ORDER_CHECKING_ENABLED
