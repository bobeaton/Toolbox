// recset.cpp  Implementation of filtered set of records  (1995-04-18)

#include "stdafx.h"
#include "toolbox.h"
#include "recset.h"
#include "obstream.h"  // classes Object_ostream, Object_istream

#include "key_d.h"  // CKeyDlg: Database Search...
#include "multmat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CRecordSet::CRecordSet(CIndex* pind)
{
    ASSERT( pind );
    m_pind = pind;
    m_pind->IncrementNumberOfRefs();
}

CRecordSet::CRecordSet(const CRecordSet& recset)
{
    m_pind = recset.m_pind;
    m_pind->IncrementNumberOfRefs();
}

CRecordSet::~CRecordSet()
{
    m_pind->DecrementNumberOfRefs();
}


#if 0
void CRecordSet::SetIndex(CIndex* pind, CRecLookEl** pprelCur)
{
    ASSERT( pind );
    if ( pind == m_pind )
        return;

    // Access the record element first, since it will be deleted below
    // if its index's reference count becomes zero.
    CRecord* precCur = (*pprelCur)->prec();
        
    m_pind->DecrementNumRefs();
    m_pind = pind;
    m_pind->IncrementNumRefs();
        
    // Return the first record element which represents the _record_.
    *pprelCur = m_pind->prelFind(precCur);
    ASSERT( (*pprelCur)->prec() == precCur );
}
#endif

CRecLookEl* CRecordSet::prelFirst() const
{
    return prelMatching_Next(m_pind->prelFirst());
}

CRecLookEl* CRecordSet::prelLast() const
{
    return prelMatching_Prev(m_pind->prelLast());
}
 
CRecLookEl* CRecordSet::prelNext(const CRecLookEl* prelCur) const
{
    CRecLookEl* prel = prelMatching_Next(m_pind->prelNext(prelCur));
    return prel;
}

CRecLookEl* CRecordSet::prelPrev(const CRecLookEl* prelCur) const
{
    CRecLookEl* prel = prelMatching_Prev(m_pind->prelPrev(prelCur));
    return prel;
}


CRecLookEl* CRecordSet::prelMatching_Next(const CRecLookEl* prelCur) const
{
    CRecLookEl* prel = (CRecLookEl*)prelCur;
    while ( (prel != NULL) && !bIsMatch(prel) )
        prel = m_pind->prelNext(prel);  
    return prel;
}


CRecLookEl* CRecordSet::prelMatching_Prev(const CRecLookEl* prelCur) const
{
    CRecLookEl* prel = (CRecLookEl*)prelCur;
    while ( (prel != NULL) && !bIsMatch(prel) )
        prel = m_pind->prelPrev(prel);
    return prel;
}

// --------------------------------------------------------------------------

CKey::CKey(const char* pszKey, BOOL bMatchWhole, MatchSetting matset)
{
    SetKey(pszKey, bMatchWhole, matset);
}

CKey::CKey(const CKey& key) 
{
    m_sKey = key.m_sKey;
    m_bMatchWhole = key.m_bMatchWhole;
    m_matset = key.m_matset;
}


static const char* psz_key = "key";
static const char* psz_fldPri = "fldPri";
static const char* psz_MatchWhole = "MatchWhole";

#ifdef prjWritefstream // 1.6.4aa 
void CKey::WriteProperties(Object_ofstream& obs, const char* pszQualifier) const
#else
void CKey::WriteProperties(Object_ostream& obs, const char* pszQualifier) const
#endif
{
    obs.WriteBeginMarkerWithQualifier(psz_key, pszQualifier);
    obs.WriteString(psz_fldPri, m_sKey);
    obs.WriteBool(psz_MatchWhole, m_bMatchWhole);

    // 1995-09-06 MRP: Need to encapsulate this during revision of matching
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
    
    obs.WriteEndMarkerWithQualifier(psz_key, pszQualifier);
}

BOOL CKey::bReadProperties(Object_istream& obs, const char* pszQualifier)
{
    if ( !obs.bReadBeginMarkerWithQualifier(psz_key, pszQualifier) )
        return FALSE;

    // Set default attributes   
    Str8 sKey;   
    BOOL bMatchWhole = FALSE;
    MatchSetting matset = CMCharOrder::matsetSamePrimaryGroup;
    Str8 sMatchChar;

    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadString(psz_fldPri, sKey) )
            ;
        else if ( obs.bReadBool(psz_MatchWhole, bMatchWhole) )
            ;
        else if ( obs.bReadString("match_char", sMatchChar) )
            {
            if ( bEqual(sMatchChar, "p") )
                matset = CMCharOrder::matsetSamePrimaryGroup;
            else if ( bEqual(sMatchChar, "c") )
                matset = CMCharOrder::matsetExactDisregardCase;
            else if ( bEqual(sMatchChar, "e") )
                matset = CMCharOrder::matsetExact;
            else if ( bEqual(sMatchChar, "i") )
                matset = CMCharOrder::matsetEvenIgnore;
            }
        else if ( obs.bEndWithQualifier(psz_key, pszQualifier) )
            break;
        }
        
    SetKey(sKey, bMatchWhole, matset);
    return TRUE;
}


void CKey::SetKey(const char* pszKey, BOOL bMatchWhole, MatchSetting matset)
{
    ASSERT( pszKey != NULL );
    m_sKey = pszKey;
    m_bMatchWhole = bMatchWhole;
    m_matset = matset;
}

void CKey::SetKey(const CKey& key)
{
    m_sKey = key.m_sKey;
    m_bMatchWhole = key.m_bMatchWhole;
    m_matset = key.m_matset;
}

void CKey::SetKeyCriteria(const CKey& key) // set key matching criteria only
{
    m_bMatchWhole = key.m_bMatchWhole;
    m_matset = key.m_matset;
}

void CKey::SetKeyCriteria(BOOL bMatchWhole, MatchSetting matset) // set key matching criteria only
{
    m_bMatchWhole = bMatchWhole;
    m_matset = matset;
}

BOOL CKey::bModalView(int iCommand, ID idHelp, CMarker* pmkr)
{
    CKeyDlg dlg(this, iCommand, pmkr);
    dlg.SetHelpID(idHelp);
    return ( dlg.DoModal() == IDOK );
}


// --------------------------------------------------------------------------

CSearchRecordSet::CSearchRecordSet(CIndex* pind, CKey* pkey) : // Do a search on pind from user's key and match settings in pkey
        CRecordSet(pind)  // initializes m_pind
{
    Length maxsizSortKey = m_pind->maxsizSortKey(); // Find max length of sort key
    m_puchSortKey = new UIdMChar[maxsizSortKey]; // Get buffere for sort key
    ASSERT( pkey );
    (void) m_pind->bSortKey_PriOnly(pkey->sKey(), m_puchSortKey); // Generate sort key from user's string
	m_sKey = pkey->sKey(); // 1.4gy Store key string in search record set
    m_matset = pkey->matset(); // Get user match setting
    m_bMatchWhole = pkey->bMatchWhole(); // Set whole word match
}

CSearchRecordSet::~CSearchRecordSet()
{
    delete m_puchSortKey;
}

#define MAX_MATCHES 500

BOOL CSearchRecordSet::bSearch( CMultipleMatchList* pmmtlst, CDatabaseRef* pdbref )
{
    ASSERT( pmmtlst );
    BOOL bMatch = FALSE;
    long lMatches = pmmtlst->lGetCount();
    long lRecNum = 0L;
    CRecLookEl* prel = (CRecLookEl*)m_pind->prelFirst();
    while (prel != NULL)
        {
        if ( prel->pfldPriKey() && bIsMatch(prel) ) // don't match non-existant field!
            {
            bMatch = TRUE;
            if ( lMatches++ >= MAX_MATCHES ) // need some reasonable limit!
                break;
            pmmtlst->Add( prel, lRecNum, pdbref ); // add to list of matches
            }
        lRecNum++;
        prel = m_pind->prelNext(prel); // search all records in this index
        }

    return bMatch; // return TRUE if one or more matches
}


BOOL CSearchRecordSet::bIsMatch(const CRecLookEl* prelCur) const
{
    ASSERT( prelCur != NULL );
	if ( pind()->bUnicodeSequenceSort() ) // 1.4gy If general Unicode sort, do it
		{
		int iCompareResult = pind()->psrtPriKey()->iCompareUnicodeSequence( sKey(), prelCur->pfldPriKey()->psz(), m_matset, m_bMatchWhole ); // 1.4gy
		return ( iCompareResult == 0 );
		}
	else
		{
	    const UIdMChar* puchSortKeyCur = prelCur->puchSortKey();
		return m_pind->bEqual_PriOnly(m_puchSortKey, puchSortKeyCur, m_matset, m_bMatchWhole);
		}
}


#ifdef _DEBUG
void CSearchRecordSet::AssertValid() const
{
}
#endif  // _DEBUG

// **************************************************************************
