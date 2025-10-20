// CRangeEl Index element class implementation Brian Yoder 11-Jul-1995

#include "stdafx.h"
#include "toolbox.h"
#include "patch.h"
#include "crngset.h"
#include "CRngSt_d.h"  // CRangeSetDlg
#include "Shwdoc.h"
#include "cck.h"  // 1999-06-12 MRP: CCheckingState
#include "lng.h"

#include "shwview.h"

//08-25-1997
#include "Progress.h"
#include "Doclist.h"   // is needed in BuildRangeSet() to have access to all open databases
#include "Ind.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// ==========================================================================

CDataItemEl::CDataItemEl( const char* pszKey ) : CSetEl( pszKey )
{
    ASSERT(*pszKey != 0);
}

// ==========================================================================

// CDataItemSet index class

CDataItemSet::CDataItemSet(BOOL bCharacterItems) : // Empty set constructor
    CSet(),
    m_bCharacterItems(bCharacterItems)
{
}

CDataItemSet::CDataItemSet(const CDataItemSet& datset) : // Copy constructor
    CSet()
{
    m_bCharacterItems = datset.m_bCharacterItems;
    Append(datset);
}

CDataItemSet::CDataItemSet(const CDataItemSet* pdatset) : // Alternate copy constructor (from potentially null pointer)
    CSet()
{
    if ( pdatset )
        {
        m_bCharacterItems = pdatset->m_bCharacterItems;
        Append(*pdatset);
        }
    else
        m_bCharacterItems = FALSE;
}

void CDataItemSet::operator=(const CDataItemSet& datset) // assignment operator
{
    if ( this == &datset )
        return;

    DeleteAll();
    m_bCharacterItems = datset.m_bCharacterItems;
    Append(datset);
}

void CDataItemSet::Append(const CDataItemSet& datset) // Add() the items in pdatset
{
    ASSERT(m_bCharacterItems == datset.m_bCharacterItems);
    for (const CDataItemEl* pdat = datset.pdatFirst(); pdat; pdat = datset.pdatNext(pdat))
        Add(pdat->sContent());
}

// Transfer all elements from datset to this object (which must be initially empty)
void CDataItemSet::TransferDataItems(CDataItemSet& datset)
{
    ASSERT(m_bCharacterItems == datset.m_bCharacterItems);
    ASSERT(bIsEmpty());
    m_pelFirst = datset.m_pelFirst;
    m_pelLast = datset.m_pelLast;
    datset.m_pelFirst = NULL;
    datset.m_pelLast = NULL;
}

void CDataItemSet::Add(const Str8& s, CDataItemEl** ppdat)  // string item
{
    CDataItemEl* pdatNew = new CDataItemEl(s);
    ASSERT(pdatNew);
    CSet::Add(pdatNew);
    if ( ppdat )
        *ppdat = pdatNew;
}

BOOL CDataItemSet::bAdd(const Str8& s, CDataItemEl** ppdat)  // string item
{
    if ( m_bCharacterItems && s.GetLength() != 1)
        return FALSE;
    if ( s.IsEmpty() )
        return FALSE;
    if ( bMatch(s) )
        return FALSE;
    Add(s, ppdat);
    return TRUE;
}

// Parse a string of whitespace-separated elements and add them to a data item set.
// Note: this function is needed for converting older settings.
void CDataItemSet::AddItems(const char* pszStr)
{
    ASSERT( pszStr );
    const char* p = pszStr;
    Str8 sName;
    CNote* pnote = NULL;    // This should never get used
    while ( *p )
        {
        Shw_MatchOptionalWhiteSpaceAt( &p );
        if ( !*p || !Shw_bMatchNameAt( &p, "", Shw_pszWhiteSpace, sName, &pnote ) )
            break;
		bAdd( sName );
        }
    ASSERT( !pnote );       // pnote should never get set
}

void CDataItemSet::RemoveElement(const Str8& s)
{
    CSetEl* pselTemp = pselSearch(s);
    if ( pselTemp)
        Delete(&pselTemp);
}

int CDataItemSet::iAddElementsFromField(CRecord* prec, CField* pfld, BOOL bMultipleItemData,
                                        BOOL bMultipleWordItems, CDataItemSet& datsetTemp)
{
    int iAddedItems = 0;

    if (!m_bCharacterItems) 
        // Set elements are 'whole data items'
        {
        // If data field consists of a single word, but actual data has multiple words,
        // then don't attempt to add to data set based on contents of this field.
        if ( !bMultipleWordItems && !bMultipleItemData )
            {
            CLangEnc* plng = pfld->pmkr()->plng();
            ASSERT( plng );
            Str8 sTrimmedData = plng->sTrimWhitespace( pfld->sContents() );
            if ( plng->bContainsWhite( sTrimmedData ) )
                return 0;
            }
        CRecPos rpsStart, rpsEnd;
        if ( pfld->bParseFirstItem(prec, rpsStart, rpsEnd, bMultipleItemData) ) //Word(s) in the string
            do
                {
                Str8 sItem = pfld->sItem(rpsStart, rpsEnd, bMultipleWordItems);
                CSetEl* pselTemp = pselSearch(sItem);
                if ( !pselTemp )
                    {
                    // There is an item which is not in the range set -> add it!
                    Add( sItem );
                    iAddedItems++;
                    }
                else // Delete this item from the temp range set (if it's still there)
                    datsetTemp.RemoveElement(sItem);
                }
                while ( pfld->bParseNextItem(rpsStart, rpsEnd, bMultipleItemData) );
        } // if - Range Set condition 'whole words'
    else
        // Range Set condition 'characters'
        {
        const char* psz = pfld->psz();
        while (*(pfld->plng()->pszSkipWhite(psz)) != 0)     
            // There are non-white characters in the string
            {
            Str8 sChar(*psz);
            CSetEl* pselTemp = pselSearch(sChar);
            if ( !pselTemp )
                {
                // There is a character item which is not in the range set -> add it!
                Add( sChar );
                iAddedItems++;
                }
            else // Delete this char from the temp range set (if it's still there)
                datsetTemp.RemoveElement(sChar);
            psz++;
            }  // while - there are characters in the string
        } // else - Range Set condition 'characters'
    return iAddedItems;
}

static const char* psz_datset = "datset";
static const char* psz_rngset = "rngset";
static const char* psz_rngsetCharacter = "rngsetCharacter";  //08-19-1997
static const char* psz_datsetCharacter = "datsetCharacter";  //08-19-1997
static const char* psz_rngsetMkr = "mkr";
static const char* psz_datsetEl = "dat";

#ifdef typWritefstream // 1.6.4ac
void CDataItemSet::WritePropertiesUnwrapped(Object_ofstream& obs) const
#else
void CDataItemSet::WritePropertiesUnwrapped(Object_ostream& obs) const
#endif
{
    Str8 s = "";
    ASSERT(!bHasMultiwordElements());
    ASSERT(!bIsEmpty());
    obs.WriteBool(psz_rngsetCharacter, m_bCharacterItems);
    for (const CDataItemEl* pdat = pdatFirst(); pdat; pdat = pdatNext(pdat))
        s += pdat->sContent() + " ";
    obs.WriteString(psz_rngset, s);
}

BOOL CDataItemSet::bReadPropertiesVer43( Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_datset) )
        return FALSE;

    m_bCharacterItems = FALSE;
    Str8 s;
    while ( !obs.bAtEnd() ) // While more in file
        {
        if ( obs.bReadBool(psz_datsetCharacter, m_bCharacterItems)) //08-19-1997
            ;
        else if ( obs.bReadString(psz_datsetEl, s) )
            bAdd(s);
        else if ( obs.bEnd(psz_datset) ) // If end marker or erroneous begin break
            break;
        }

    if ( m_bCharacterItems )
        for (CDataItemEl* pdat = (CDataItemEl*) pselFirst(); pdat; pdat = (CDataItemEl*) pselNext(pdat))
            if ( pdat->sContent().GetLength() != 1 )
                pdat = (CDataItemEl*)pelDelete(pdat);
    return TRUE;
}

BOOL CDataItemSet::bHasMultiwordElements() const
{
    for (const CDataItemEl* pdat = pdatFirst(); pdat; pdat = pdatNext(pdat))
        if ( pdat->sContent().Find(' ') != -1 )
            return TRUE;
    return FALSE;
}


//08-25-1997
void CDataItemSet::BuildDataItemSet(const CMarker *pmkr, BOOL bMultipleItemData, BOOL bMultipleWordItems,
                                    int& iAddedItems, int& iItemsTotal, int& iItemsNotFound)
{
    CDocList doclst; // list of currently open docs
    CShwDoc* pdoc;
    long lnumRecTotal = 0;  //How many records
    long lnumRec = 0;  //For showing progress
    iAddedItems = 0;
    
      // I use a little trick to detect how many items were not found
      // For this trick I need a copy of the current data item set. Undetected
      // entries can only be a part of the data item set before building it
    CDataItemSet datsetTemp(*this);  // 1999-06-07 MRP
                
    // Next few lines are only for the progress indicator
    for (pdoc = doclst.pdocFirst(); pdoc; pdoc = doclst.pdocNext())
        {
        if ( pmkr->pmkrsetMyOwner()->ptyp() == pdoc->ptyp() )
            lnumRecTotal += pdoc->pindset()->pindRecordOwner()->lNumRecEls();
        }    
    CProgressIndicator prg(lnumRecTotal, NULL, pmkr->sMarker()); // 1.4ad Eliminate resource messages sent to progress bar
    
    for (pdoc = doclst.pdocFirst(); pdoc; pdoc = doclst.pdocNext())
        //Look for all documents
        {
        if ( pmkr->pmkrsetMyOwner()->ptyp() == pdoc->ptyp() )
            {
            // do it only for documents from the type of the marker
            
            //Unfilterd index of the database
            CIndex* pindUnf = pdoc->pindset()->pindRecordOwner();
      
            //First record element
            CRecLookEl* prel = pindUnf->prelFirst();
    
            //Do the search for every record
            for(; prel != NULL; prel=pindUnf->prelNext(prel))
                {
                CRecord* prec = prel->prec();
                // Look for the first field with this marker
                CField* pfld = prec->pfldFirstWithMarker(pmkr);

                while (pfld)
                    // Look in all fields
                    {
                    if (pfld->pmkr() == pmkr)
                        // Found a field with this marker
                        {
                        iAddedItems += iAddElementsFromField(prec, pfld, bMultipleItemData,
                                                             bMultipleWordItems, datsetTemp);
                        } // if - There is a field with this
                    //Next Field (Maybe there are more fields with the same marker
                    pfld = prec->pfldNext(pfld);
                    } // while - Look for all fields in the record
                prg.bUpdateProgress(++lnumRec);
                }  // for
            }  // if 
        } // for
    iItemsTotal = (int)lGetCount();   // How many total entries in the data item set
    iItemsNotFound = (int)datsetTemp.lGetCount(); // Any entries left in temp data item set?
} // BuildRngSet

// ==========================================================================

// CRangeSet index class

CRangeSet::CRangeSet(CMarker* pmkr, BOOL bRangeSetForCharacters) :
    CDataItemSet(bRangeSetForCharacters),
    m_pmkr(pmkr)
{
    ASSERT( m_pmkr );
}

#ifdef typWritefstream // 1.6.4ac
void CRangeSet::WriteProperties(Object_ofstream& obs) const
#else
void CRangeSet::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_rngset);
    obs.WriteString(psz_rngsetMkr, m_pmkr->pszKey());
    obs.WriteBool(psz_rngsetCharacter, m_bCharacterItems);
    for (const CDataItemEl* pdat = pdatFirst(); pdat; pdat = pdatNext(pdat))
        obs.WriteString(psz_datsetEl, pdat->pszContent());
    obs.WriteEndMarker(psz_rngset);
}

BOOL CRangeSet::s_bReadProperties( Object_istream& obs, CMarkerSet* pmkrset)
{
    if ( !obs.bReadBeginMarker(psz_rngset) )
        return FALSE;

    CDataItemSet* pdatset = new CDataItemSet;
    BOOL bCharacterItems = FALSE;
    CMarker* pmkr = NULL;
    Str8 s;
    while ( !obs.bAtEnd() ) // While more in file
        {
        if ( obs.bReadString(psz_rngsetMkr, s) )
            pmkr = pmkrset->pmkrSearch_AddIfNew(s);
        if ( obs.bReadBool(psz_rngsetCharacter, bCharacterItems) )
            {
            // Since we normally expect this to come ahead of the list and we only barely
            // maintain support for these range sets anyway, just blow it away and start over.
            // If any items were added manually ahead of this in the settings file, they will be lost.
            delete pdatset;
            pdatset = new CDataItemSet(bCharacterItems);
            }
        else if ( obs.bReadString(psz_datsetEl, s) )
            pdatset->bAdd(s);
        else if ( obs.bEnd(psz_rngset) ) // If end marker or erroneous begin break
            break;
        }

    if ( !pmkr || pmkr->bUseRangeSet() )
        {
        delete pdatset; // 2000-09-12 TLB: Have to delete this to prevent memory leak on early return
        return TRUE;
        }
                
    if ( !pmkr->bMultipleWordItems() && pdatset->bHasMultiwordElements() )
        pmkr->SetDataType(FALSE, TRUE);

    pmkr->TakeRangeSet(*pdatset);

    delete pdatset; // 2000-09-12 TLB: Have to delete this to prevent memory leak on early return
    return TRUE;
}

// Add to range set, checking it according to data field specifications. Note that this
// function does not prevent addition of duplicate elements.
// I don't think this is needed now    
#ifdef NOT_NEEDED_TLB_092099
BOOL CRangeSet::bAdd( const char* pszStr )
{
    ASSERT( pszStr );
    ASSERT(*pszStr); // Not an empty string
    if ( m_pmkr->bMultipleItemData() )
        {
        Add( pszStr );
        }
    else if ( bCharacterItems() )
        {
        if ( *(pszStr+1) != 0 )
            return FALSE;
        Add( pszStr );
        }
    else
        {
        CLangEnc* plng = m_pmkr->plng();
        ASSERT( plng );
        Str8 sTrimmedStr = plng->sTrimWhitespace( pszStr );
        if ( !m_pmkr->bMultipleWordItems() && plng->bContainsNonWhite( sTrimmedStr ) )
            return FALSE;
        Add( sTrimmedStr );
        }
    return TRUE;
}
#endif

BOOL CRangeSet::bAdd(const Str8& s)  // string item
{
    // If this is not a character-based range set and the marker does not allow
    // for multiple-word data, make sure we aren't trying to add multiple words.
    if ( !m_bCharacterItems && !m_pmkr->bMultipleWordItems() )
        {
        CLangEnc* plng = m_pmkr->plng();
        ASSERT(plng);
        Str8 sTrimmedData = plng->sTrimWhitespace(s);
        if ( plng->bContainsWhite(sTrimmedData) )
            return FALSE;
        }

    return CDataItemSet::bAdd(s);
}

void CRangeSet::Reset(CDataItemSet& datset) // Transfer in elements from pdatset
{
    DeleteAll();
    m_bCharacterItems = datset.bCharacterItems();
    TransferDataItems(datset);
}


// Return in sText the Range Set item selected by user
BOOL CRangeSet::bModalView_Insert( Str8& sText)
{
    CRangeSetDlg dlg(this, sText);
    return ( dlg.DoModal() == IDOK && !sText.IsEmpty() );
}

// --------------------------------------------------------------------------

BOOL CRangeSet::bConsistent(const Str8& sData, int& iChar)
{
    if ( !bEnabled() )
        return TRUE;
    // Check elements: characters or words.
    if ( m_bCharacterItems )
        {
        for (iChar = 0; iChar < sData.GetLength(); iChar++ )
            {
            // Skip white space
            if ( !pmkr()->plng()->bIsWhite(sData[iChar]) && !bMatch(Str8(sData[iChar])) )
                return FALSE;
            }
        return TRUE;
        }
    else
        return bMatch(sData);
}
