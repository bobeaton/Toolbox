// fil.cpp Implementation of Shoebox filter classes (1995-01-30)


#include "stdafx.h"
#include "toolbox.h"
#include "fil.h"
#include "loc.h"  // CMStringLoc
#include "ind.h"  // CIndex
#include "date.h"  // 1998-04-08 MRP: CDateCon
#include "not.h"  // CNote
#include "obstream.h"  // Object_istream, Object_ostream
#include "nlstream.h"
#include "shwnotes.h"
#include <ctype.h>  // DateToLong

#include "fil_d.h"
#include "shw.h"  // Shw_papp()
#include "shwdoc.h"
#include "shwview.h"
#include "typ_d.h"
#include "project.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

         
#ifdef _DEBUG
#define ASSERT_VALID_NONAFX(pEntity) ((pEntity)->AssertValid())
#else
#define ASSERT_VALID_NONAFX(pEntity) ((void)0)
#endif  // _DEBUG

// **************************************************************************                                                    

CFilterMatch::CFilterMatch(const CIndex* pind, BOOL bMatchWhole,
                            MatchSetting matset, BOOL bEmbeddedMarkup) :
        CPatMCharMatch(matset, bEmbeddedMarkup)
{
    ASSERT( pind != NULL );
    m_pind = pind;  // 1995-06-30 MRP: need to rethink this, cf. recset
    m_bMatchWhole = bMatchWhole;
}


#ifdef prjWritefstream // 1.6.4aa 
void CFilterMatch::WriteProperties(Object_ofstream& obs) const
#else
void CFilterMatch::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBool("match_whole", m_bMatchWhole);
    CPatMCharMatch::WriteProperties(obs);
}


const CRecLookEl* CFilterMatch::prelPrev(const CRecLookEl* prelCur)
{
    ASSERT( prelCur != NULL );
    return m_pind->prelPrev((CRecLookEl*)prelCur);
}


const CRecLookEl* CFilterMatch::prelNext(const CRecLookEl* prelCur)
{
    ASSERT( prelCur != NULL );
    return m_pind->prelNext((CRecLookEl*)prelCur);
}


// **************************************************************************                                                    

CFilter::CFilter(const char* pszName, const char* pszCondition,
//      CFilterCon* pcon, CFilterSet* pfilsetMyOwner) : CLookEl(pszName)
        CFilterCon* pcon, CFilterSet* pfilsetMyOwner,
        BOOL bMatchWhole, MatchSetting matset) : CSetEl(pszName)
{
    ASSERT( pszName != NULL );
    ASSERT( pszCondition != NULL );
    ASSERT( pcon != NULL );
    ASSERT( pfilsetMyOwner != NULL );
    m_sCondition = pszCondition;
    m_pcon = pcon;
    m_pfilsetMyOwner = pfilsetMyOwner;
    
    m_bMatchWhole = bMatchWhole;
    m_matset = matset;
}


CFilter::~CFilter()
{
    delete m_pcon;
}


CSet* CFilter::psetMyOwner()
{
    return m_pfilsetMyOwner;
}


static const char* psz_fil = "fil";
static const char* psz_con = "con";
static const char* psz_MatchWhole = "MatchWhole";

#ifdef typWritefstream // 1.6.4ac
void CFilter::WriteProperties(Object_ofstream& obs) const 
#else
void CFilter::WriteProperties(Object_ostream& obs) const 
#endif
{
    obs.WriteBeginMarker(psz_fil, sName());
//  obs.WriteString(psz_con, sCondition());
    m_pcon->WriteProperties(obs);
    // Match settings
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
    
    obs.WriteEndMarker(psz_fil);
    obs.WriteNewline();
}

#ifdef typWritefstream // 1.6.4ac
void CFilter::WriteRef(Object_ofstream& obs, const char* pszQualifier) const
#else
void CFilter::WriteRef(Object_ostream& obs, const char* pszQualifier) const
#endif
{
    obs.WriteString(psz_fil, pszQualifier, sName());
}

void CFilter::CopyToList(CFilterElList* pfellst) const
{
#ifdef ostrWritefstream // 1.6.4ad
	Str8 sFile = Shw_pProject()->sSettingsDirPath(); // 1.6.4ad Get settings folder
	sFile += "Temp.txt"; // 1.6.4ad Add file name
	FILE* pf = pfFileOpenWrite( sFile ); // 1.6.4ad Open FILE for write
    Object_ofstream obs(pf); // 1.6.4ad 
    m_pcon->WriteProperties(obs); // 1.6.4ad 
	fclose( pf ); // 1.6.4ad 

    CNoteList notlst;
    ifstream ios(sFile, ios::nocreate);
    Newline_istream iosnl(ios);
    Object_istream obsIn(iosnl, notlst);
    BOOL bRead = pfellst->bReadProperties(obsIn); // 1.6.4ad 
    remove( sFile ); // 1.6.4ad delete file
#else
    strstream ios;
    Object_ostream obsWrite(ios);
    m_pcon->WriteProperties(obsWrite);
    CNoteList notlst;
    Object_istream obsRead(ios, notlst);
    ASSERT( pfellst );
    BOOL bRead = pfellst->bReadProperties(obsRead);
    ASSERT( bRead );
#endif // 1.6.4ad 
}

void CFilter::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pcon->SetRecordMarker(pmkrRecord);
}


BOOL CFilter::bMatch(const CRecLookEl* prel, CIndex* pind) const
{
    CFilterMatch filmat(pind, m_bMatchWhole, m_matset);
    BOOL bMatched = m_pcon->bMatch(prel, filmat);
    return bMatched;
}

BOOL CFilter::bFilterDependsOnRecordContext() const
{
    return m_pcon->bFilterDependsOnRecordContext();
}

BOOL CFilter::bModifyProperties(const char* pszName, const CFilterElList& fellst,
        BOOL bMatchWhole, MatchSetting matset, CNote** ppnot)
{
    // NOTE NOTE NOTE: We must construct the update notification
    // *before* changing any member data, but must wait to use it
    // until *after* the modifications have been committed. 
    Str8 sCondition2 = m_sCondition;
    sCondition2 += " ";  // For now, force it to be modified
    CFilterUpdate fup(this, pszName, sCondition2, bMatchWhole, matset);

    // Validate the name and condition first.  If they fail, don't change pfil's
    // member data.     

    // If the name is being changed, verify that it will be valid
    BOOL bUpdatingName = !bEqual(pszName, sName());
    if ( bUpdatingName && !m_pfilsetMyOwner->bValidNewName(pszName, ppnot) )
        {
        (*ppnot)->SetStringContainingReferent(pszName);
        return FALSE;
        }

    // If the condition is being changed, attempt to update it
    if ( !bSetCondition(fellst, ppnot) )
        {
//      (*ppnot)->SetStringContainingReferent(pszCondition);
        return FALSE;
        }

    // Adjust the match settings.
    m_matset = matset;
    m_bMatchWhole = bMatchWhole;

    // If the name is being changed, update it
    if ( bUpdatingName )
        ChangeNameTo(pszName);

    extern void Shw_Update(CUpdate& up);        
    Shw_Update(fup);  // Notify other objects that this filter is modified
    
    return TRUE;
}


#ifdef _DEBUG
void CFilter::AssertValid() const
{
    CNote* pnot = NULL;
    ASSERT( m_pfilsetMyOwner->bValidName(sName(), &pnot) );
    ASSERT( m_pcon != NULL );
    ASSERT_VALID_NONAFX( m_pcon );  // 1995-05-17 MRP
}
#endif  // _DEBUG


// ==========================================================================                                                    


BOOL CFilter::s_bReadProperties(Object_istream& obs,
        CFilterSet* pfilsetMyOwner, CFilter** ppfil)
{
    Str8 sName;
    if ( !obs.bReadBeginMarker(psz_fil, sName) )
        return FALSE;

    CFilterElList fellst(pfilsetMyOwner);
    CFilterCon* pcon = NULL;
    Str8 sMatchChar;
    BOOL bMatchWhole = FALSE;
    MatchSetting matset = CMCharOrder::matsetSamePrimaryGroup;
    
    while ( !obs.bAtEnd() )
        {
        if ( fellst.bReadProperties(obs) )
            ;
        // Match settings
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
        else if ( obs.bEnd(psz_fil) )
            break;
        }

    if ( fellst.bIsEmpty() )
        return TRUE;        

    CNote* pnot = NULL;
    if ( !s_bConstruct(sName, fellst, bMatchWhole, matset, pfilsetMyOwner,
            ppfil, &pnot) )
//      obs.notlst().Add(pnot);
        delete pnot;
        
    return TRUE;
}

BOOL CFilter::s_bConstruct(const char* pszName, const CFilterElList& fellst,
        BOOL bMatchWhole, MatchSetting matset, CFilterSet* pfilsetMyOwner,
        CFilter **ppfil, CNote** ppnot)
{
    ASSERT( pfilsetMyOwner );
    if ( !pfilsetMyOwner->bValidName(pszName, ppnot) )
        { 
        (*ppnot)->SetStringContainingReferent(pszName);
        return FALSE;
        }
        
    CFilterCon* pcon = NULL;
    if ( !CFilterCon::s_bConstruct(fellst, &pcon, ppnot) )
        {
//      (*ppnot)->SetStringContainingReferent(pszCondition);
        return FALSE;
        }
    
    *ppfil = new CFilter(pszName, "", pcon, 
        pfilsetMyOwner, bMatchWhole, matset);
    return TRUE;
}

BOOL CFilter::bSetCondition(const CFilterElList& fellst, CNote** ppnot)
{
    CFilterCon* pcon = NULL;
    if ( !CFilterCon::s_bConstruct(fellst, &pcon, ppnot) )
        {
//      (*ppnot)->SetStringContainingReferent(pszCondition);
        return FALSE;
        }
    
    delete m_pcon;
    m_pcon = pcon;
    return TRUE;
}

            
BOOL CFilter::bCopy(CSetEl** ppselNew)
{
//  CFilterDlg dlg(this, (CFilter**)ppselNew);
    CNewFilterDlg dlg(this, (CFilter**)ppselNew);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilter::bModify()
{
//  CFilterDlg dlg(this);
    CNewFilterDlg dlg(this);
    return ( dlg.DoModal() == IDOK );  // show a modal view of the filter
}
            
BOOL CFilter::bDeletable()
{
    return !bHasRefs();  // 1996-04-02 MRP: For now at least
//  return TRUE;
}

void CFilter::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int xLeft, int xRight) const
{
    m_pcon->DrawCondition(plbo, cDC, rcItem, &xLeft, xRight);
}

// --------------------------------------------------------------------------

void CFilterPtr::IncrementNumberOfRefs(CFilter* pfil)
{
    m_pfil = pfil;
    if ( m_pfil )
        m_pfil->IncrementNumberOfRefs();
}

void CFilterPtr::DecrementNumberOfRefs()
{
    if ( m_pfil )
        m_pfil->DecrementNumberOfRefs();
}


const CFilterPtr& CFilterPtr::operator=(CFilter* pfil)
{
    if ( m_pfil != pfil )
        {
        DecrementNumberOfRefs();
        IncrementNumberOfRefs(pfil);
        }
        
    return *this;
}


// --------------------------------------------------------------------------

CFilterUpdate::CFilterUpdate(CFilter* pfil, BOOL bToBeDeleted) :
    m_pindToWhichFilterIsBeingReapplied(NULL)
{
    m_pfil = pfil;
    ASSERT( m_pfil );
    m_bToBeDeleted = bToBeDeleted;
    m_bModified = FALSE;
    m_bNameModified = FALSE;
    m_bMatchCriteriaModified = FALSE;
}

CFilterUpdate::CFilterUpdate(CFilter* pfil, const char* pszName,
                             const char* pszCondition, BOOL bMatchWhole, MatchSetting matset) :
    m_pindToWhichFilterIsBeingReapplied(NULL)
{
    m_pfil = pfil;
    ASSERT( m_pfil );
    m_bToBeDeleted = FALSE;
    m_bModified = TRUE;
    ASSERT( pszName );
    m_bNameModified = !bEqual(pszName, m_pfil->sName());
    ASSERT( pszCondition );
//  m_bMatchCriteriaModified = (
//      !bEqual(pszCondition, m_pfil->sCondition()) ||
//      bMatchWhole != m_pfil->bMatchWhole() ||
//      matset != m_pfil->GetMatchSetting()
//      );
    m_bMatchCriteriaModified = TRUE;  // 1996-07-30 MRP
}

CFilterUpdate::CFilterUpdate(CIndex* pindToWhichFilterIsBeingReapplied) : // Reapply Filter
    m_pfil(pindToWhichFilterIsBeingReapplied->pfil()),
    m_bToBeDeleted(FALSE),
    m_bModified(FALSE),
    m_bNameModified(FALSE),
    m_bMatchCriteriaModified(FALSE),
    m_pindToWhichFilterIsBeingReapplied(pindToWhichFilterIsBeingReapplied)
{
    ASSERT(m_pindToWhichFilterIsBeingReapplied);
    ASSERT(m_pfil);
}

BOOL CFilterUpdate::bReapplyingFilterToIndex(const CIndex* pind) const
{
    ASSERT(pind);
    return (pind == m_pindToWhichFilterIsBeingReapplied);
}


                                                    
// **************************************************************************

CFilterSet::CFilterSet(CMarkerSet* pmkrset) :
    m_pmkrset(pmkrset)
{
    ASSERT( m_pmkrset );
}

static const char* psz_filset = "filset";

#ifdef typWritefstream // 1.6.4ac
void CFilterSet::WriteProperties(Object_ofstream& obs) const
#else
void CFilterSet::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_filset);
    obs.WriteNewline();
    
    const CFilter* pfil = pfilFirst();
    for ( ; pfil; pfil = pfilNext(pfil) )
        pfil->WriteProperties(obs);

    obs.WriteEndMarker(psz_filset);
}

BOOL CFilterSet::bReadProperties(Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_filset) )
        return FALSE;
        
    while ( !obs.bAtEnd() )
        {
        CFilter* pfil = NULL;
//      if ( CFilter::s_bReadProperties(obs, this, pfil) )
//          Add(pfil);
        if ( CFilter::s_bReadProperties(obs, this, &pfil) )
            {
            if ( pfil )
                Add(pfil);
            }
        else if ( obs.bEnd(psz_filset) )
            break;
        }
        
    return TRUE;
}

BOOL CFilterSet::bReadFilterRef(Object_istream& obs, CFilter*& pfil)
{
    Str8 sName;
    if ( !obs.bReadString(psz_fil, sName) )
        return FALSE;
        
    CFilter* pfilFound = pfilSearch(sName);
    if ( !pfilFound )
        return FALSE;
        
    pfil = pfilFound;
    return TRUE;
}

void CFilterSet::SetRecordMarker(CMarker* pmkrRecord)
{
    CFilter* pfil = pfilFirst();
    for ( ; pfil; pfil = pfilNext(pfil) )
        pfil->SetRecordMarker(pmkrRecord);
}

BOOL CFilterSet::bAddFilter(const char* pszName, const CFilterElList& fellst,
        BOOL bMatchWhole, MatchSetting matset,
        CFilter** ppfil, CNote** ppnot)
{
    CFilter* pfil = NULL;
    if ( !CFilter::s_bConstruct(pszName, fellst, bMatchWhole, matset, 
            this, &pfil, ppnot) )
        return FALSE;
        
    if ( pfilSearch(pszName) != NULL )
        {
        *ppnot = new CNote(_("A filter already exists with this name:"), pszName, // 1.5.0ft 
            pszName, strlen(pszName), pszName);
        delete pfil;
        return FALSE;
        }
        
    Add(pfil);
    
    CFilterUpdate fup(pfil);
    extern void Shw_Update(CUpdate& up);        
    Shw_Update(fup);  // Notify other objects that this filter is added
    
    *ppfil = pfil;
    return TRUE;
}


void CFilterSet::Add( CFilter* pfilNew )
{
//  1996-07-16 MRP: Restore after making sure that duplicate names
//  are made unique when reading properties.
//  ASSERT( pfilSearch(pfilNew->sName()) == NULL );
	CSet::Add( pfilNew );
}



void CFilterSetListBox::InitLabels()
{
    CSetListBox::InitLabels();
    m_xCondition = xSubItem_Show(IDC_FILSET_CONDITION);
}

int CFilterSetListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CFilterSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CFilter* pfil = (CFilter*)pel;
//  DrawSubItem(cDC, rcItem, m_xName, m_xCondition, pfil->sName());
//  DrawSubItem(cDC, rcItem, m_xCondition, 0, pfil->sCondition());
    DrawSubItemJustify(cDC, rcItem, m_xName, m_xCondition, pfil->sName());
    pfil->DrawCondition(this, cDC, rcItem, m_xCondition, 0);
}

BOOL CFilterSetListBox::bDelete(CSetEl* psel)
{
    Str8 sMessage = _("Delete filter?"); // 1.5.0fg 
    if ( AfxMessageBox(sMessage, MB_YESNO|MB_ICONQUESTION) == IDNO )
        return FALSE;  // not deleted
    
    return CSetListBox::bDelete(psel);
}

void CFilterSet::Delete(CSetEl** ppsel)
{
    ASSERT( ppsel );
    CFilter* pfilToBeDeleted = (CFilter*)*ppsel;
    ASSERT( pfilToBeDeleted );
    CFilterUpdate fup(pfilToBeDeleted, TRUE);
    extern void Shw_Update(CUpdate& up);        
    Shw_Update(fup);  // Notify other objects that this filter is added
            
    CSet::Delete(ppsel);
}


BOOL CFilterSet::bModalView_Elements(CFilter* pfilSelect)
{
    CShwView* pview = Shw_papp()->pviewActive();
    ASSERT(pview);
    // BJY 2-26-96 Should be using pfilSelect???
    pview->ViewDatabaseProperties(CDatabaseTypeSheet::eFilterSetPage);
    return TRUE;
}

BOOL CFilterSet::bAdd(CSetEl** ppselNew)
{
//  CFilterDlg dlg(this, (CFilter**)ppselNew);
    CNewFilterDlg dlg(this, (CFilter**)ppselNew);
    return ( dlg.DoModal() == IDOK );  // show a modal view of a new filter
}

// **************************************************************************

BOOL CFilterCon::bMatchField(const CField* pfld, const CPatMChar *m_ppat,
        CFilterMatch& filmat) const
{
    CMStringLoc mksloc(pfld);
    BOOL bMatched = ( filmat.bMatchWhole() ?
        (m_ppat->bMatchAt(mksloc, filmat) && mksloc.bAtEnd()) :
        m_ppat->bMatch(mksloc, filmat)
    );
    
    return bMatched;
}


CFilterCon* CFilterCon::pconCopy() const
{
    ASSERT( FALSE );
    return NULL;
}

#ifdef prjWritefstream // 1.6.4aa 
void CFilterCon::WriteProperties(Object_ofstream& obs) const 
#else
void CFilterCon::WriteProperties(Object_ostream& obs) const 
#endif
{
}

void CFilterCon::SetRecordMarker(CMarker* pmkrRecord)
{
}

BOOL CFilterCon::bCol() const { return FALSE; }

    
// Override in modifiable derived classes.
BOOL CFilterCon::bModify()
{
    ASSERT( FALSE );
    return FALSE;
}

void CFilterCon::DrawElementText(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int xElement, int xText) const
{
}

void CFilterCon::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
}

void CFilterCon::DrawSpace(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, " ");
}


// Patterns which correspond to elements of filter conditions.
static const char* s_pszInvalidPatternChars = "[|]\n\t";
    
static const char* s_psz_LFilter = "[";  // optional preceding ws
static const char* s_pszMFilter = "|";
static const char* s_pszRFilter = "]";
                    
static const char* s_pszNonUnique = "NonUnique";
static const char* s_pszNon_Unique = "Non-Unique";
static const char* s_pszKey = "KEY|";
static const char* s_pszSearch = "SEARCH|";
static const char* s_pszField = "FIELD|";

static const char* s_pszDate = "DATE|";
static const char* s_pszLess = "|LESS|";
static const char* s_pszBefore = "|BEFORE|";
static const char* s_pszEqual = "|EQUAL|";
static const char* s_pszGreater= "|GREATER|";
static const char* s_pszAfter = "|AFTER|";

static const CPatChar s_patRestOfFilter(
                    new CPatElRep(new CPatElNotSet("]"))
                    );

static const char* s_psz_With= "WITH";  // optional preceding ws
static const char* s_psz_LParen = "(";  // optional preceding ws
static const char* s_psz_RParen = ")";  // optional preceding ws
static const char* s_psz_Not = "NOT";  // optional preceding ws
static const char* s_psz_Not1 = "!";  // optional preceding ws
static const char* s_psz_And = "AND";  // optional preceding ws
static const char* s_psz_Or = "OR";  // optional preceding ws


// 1995-05-17 MRP: These might be moved to patch.cpp
// Return whether there is a match at ppsz of the literal string pszString,
// optionally preceded by white space, and disregarding case.
static BOOL s_bMatchStringAt(const char** ppsz, const char* pszString)
{
    if ( strnicmp(*ppsz, pszString, strlen(pszString)) != 0 )
        return FALSE;
;
    *ppsz += strlen(pszString);  // after the matched string
    return TRUE;
}

static BOOL s_bMatch_StringAt(const char** ppsz, const char* pszString)
{
    const char* psz = *ppsz;
    Shw_MatchOptionalWhiteSpaceAt(&psz);
    if ( !s_bMatchStringAt(&psz, pszString) )
        return FALSE;
;
    *ppsz = psz;  // after the matched string
    return TRUE;
}


// --------------------------------------------------------------------------

static long DateToLong(const char* pszDate);

static const char* s_pszAnd = "And";
static const char* s_pszOr = "Or";
static const char* s_pszNot = "Not";
static const char* s_pszLParen = "(";
static const char* s_pszRParen = ")";
static const char* s_pszWith = "With";
//static const char* s_pszNonUnique = "non-Unique";

// Bracket := "[" {
//                      "NON-UNIQUE"
//                  |   "DATE|" Condition "|" Date
//                  |   "KEY|" Pattern
//                  |   "SEARCH|" Pattern
//                  |   "FIELD|" Marker
//                  |   Marker "|" Pattern
//                                                  } "]"
BOOL CFilterCon::s_bConstructTerminal(const CFilterElList& fellst,
        const CFilterEl** ppfel, CFilterCon** ppcon, CNote** ppnot)
{
    CFilterCon* pcon = NULL;
    if ( fellst.bMatchAt(ppfel, s_pszNonUnique) ||
            fellst.bMatchAt(ppfel, s_pszNon_Unique) )
        pcon = new CFilterConNonUnique;
    else
        {
        ASSERT( ppfel );
        const CFilterEl* pfel = *ppfel;
        if ( !pfel )
            {
            *ppnot = new CNote(_("Expecting a Text or Date Element"), "");
            return FALSE;
            }
        else if ( !pfel->pcon() )
            {
            const char* pszRef = pfel->sElement();
            ASSERT( *pszRef );
            *ppnot = new CNote(_("Expecting a Text or Date Element"), "", pszRef, 0, pszRef);
            return FALSE;
            }
            
        pcon = pfel->pcon()->pconCopy();
        *ppfel = fellst.pfelNext(pfel);
        }
            
    *ppcon = pcon;
    return TRUE;    
}


// --------------------------------------------------------------------------

// With := Bracket ("WITH" Bracket)*
// Column filter WITH condition: not implemented yet
// NOTE NOTE NOTE: The column WITH filter can consist only of
// Marker-String filter conditions. In the grammar however,
// if there is no WITH, any bracketed condition must be parsed here.
BOOL CFilterCon::s_bConstructWith(const CFilterElList& fellst,
        const CFilterEl** ppfel, CFilterCon** ppcon, CNote** ppnot)
{
    ASSERT( ppfel );
    const CFilterEl* pfel = *ppfel;
    CFilterCon* pcon = NULL;
    if ( !s_bConstructTerminal(fellst, ppfel, &pcon, ppnot) )
        return FALSE;
    
    if ( fellst.bMatchAt(ppfel, s_pszWith) )
        {
        // Ensure that the first bracket was a Marker-String.
        if ( !pcon->bCol() )
            {
            delete pcon;
            const char* pszRef = pfel->sElement();
            ASSERT( *pszRef );
            *ppnot = new CNote(_("With expects Marker Text Elements"), "", // 1.4ve 
                pszRef, 0, pszRef);
            return FALSE;
            }
        
        CFilterConCol* pconA = (CFilterConCol*)pcon;
        // 1995-02-20 MRP: Uh, oh-- maybe I did want it right recursive!!!
        // For now, I will reverse the role of A and B in the With condition.
        do
            {
            pfel = *ppfel;
//          CFilterConMarkerString* pconB = NULL;
            CFilterCon* pconB = NULL;
            if ( !s_bConstructTerminal(fellst, ppfel, &pconB, ppnot) )
                { delete pcon; return FALSE; }

            if ( !pconB->bCol() )
                {
                delete pconB;
                const char* pszRef = pfel->sElement();
                ASSERT( *pszRef );
                *ppnot = new CNote(_("With expects Marker Text Elements"), "", // 1.4ve 
                    pszRef, 0, pszRef);
                return FALSE;
                }
            
            pconA = new CFilterConWith(pconA, (CFilterConMarkerString*)pconB);
            } while ( fellst.bMatchAt(ppfel, s_pszWith) );
            
        pcon = pconA;
        }
        
    *ppcon = pcon;  
    return TRUE;
}


// --------------------------------------------------------------------------

// Parentheses allow explicit grouping of filter conditions
// Paren := "(" AndOr ")" | With
BOOL CFilterCon::s_bConstructParen(const CFilterElList& fellst,
        const CFilterEl** ppfel, CFilterCon** ppcon, CNote** ppnot)
{   
    if ( !fellst.bMatchAt(ppfel, s_pszLParen) )
        return s_bConstructWith(fellst, ppfel, ppcon, ppnot);

    CFilterCon* pcon = NULL;
    if ( !s_bConstructAndOr(fellst, ppfel, &pcon, ppnot) )
        return FALSE;
    
    if ( !fellst.bMatchAt(ppfel, s_pszRParen) )
        {
        delete pcon;
        const CFilterEl* pfel = *ppfel;
        // 1998-12-02 MRP
        if ( pfel )
            {
            const char* pszRef = pfel->sElement();
            *ppnot = new CNote(_("Expecting a right parenthesis )"), "", pszRef, 0, pszRef);
            }
        else
            *ppnot = new CNote(_("Expecting a right parenthesis )"), "");
        return FALSE;
        }
        
    *ppcon = new CFilterConParen(pcon); 
    return TRUE;
}


// --------------------------------------------------------------------------

// Not := ("NOT" | "!") Paren
BOOL CFilterCon::s_bConstructNot(const CFilterElList& fellst,
        const CFilterEl** ppfel, CFilterCon** ppcon, CNote** ppnot)
{
    BOOL bNot = fellst.bMatchAt(ppfel, s_pszNot);

    CFilterCon* pcon = NULL;
    if ( bNot )
        {
        if ( !s_bConstructNot(fellst, ppfel, &pcon, ppnot) )
            return FALSE;
            
        *ppcon = new CFilterConNot(pcon);
        }
    else
        {
        if ( !s_bConstructParen(fellst, ppfel, &pcon, ppnot) )
            return FALSE;
            
        *ppcon = pcon;
        }
    
    return TRUE;
}


// --------------------------------------------------------------------------

// AndOr := Not (("AND" | "OR") Not)*
// NOTE NOTE NOTE: AND and OR are left-associative with *same* precedence
BOOL CFilterCon::s_bConstructAndOr(const CFilterElList& fellst,
        const CFilterEl** ppfel, CFilterCon** ppcon, CNote** ppnot)
{   
    CFilterCon* pcon = NULL;
    if ( !s_bConstructNot(fellst, ppfel, &pcon, ppnot) )
        return FALSE;

    BOOL bAnd;
    while ( (bAnd = fellst.bMatchAt(ppfel, s_pszAnd)) ||
            fellst.bMatchAt(ppfel, s_pszOr) )
        {
        CFilterCon* pconB = NULL;
        if ( !s_bConstructNot(fellst, ppfel, &pconB, ppnot) )
            { delete pcon; return FALSE; }
        
        if (bAnd)
            pcon = new CFilterConAnd(pcon, pconB);
        else
            pcon = new CFilterConOr(pcon, pconB);
        }
    
    *ppcon = pcon;
    return TRUE;
} 


BOOL CFilterCon::s_bConstruct(const CFilterElList& fellst,
        CFilterCon** ppcon, CNote** ppnot)
{
    const CFilterEl* pfel = fellst.pfelFirst();
    if ( !pfel )
        {
        *ppnot = new CNote(_("Expecting a Filter Element"), "");
        return FALSE;
        }

    CFilterCon* pcon = NULL;
    if ( !s_bConstructAndOr(fellst, &pfel, &pcon, ppnot) )
        return FALSE;

    if ( pfel )
        {
        delete pcon;
        *ppnot = new CNote(_("Unexpected Element after end of Filter"), "");
        return FALSE;
        }

    *ppcon = pcon;      
    return TRUE;
}

// ==========================================================================                                                    
// A subrecord is a contiguous sublist of a record's fields
// which contains a record element's primary sort key field
// and extends in scope (according to the marker hierarchy)
// so as to contain occurrences of a specified marker.
// It is used by the Marker-String filter condition.

class CSubRecord  // Hungarian: rec
{
private:
    const CRecord* m_prec;
    const CField* m_pfldFirst;
    Level m_levFirst;

public:
    CSubRecord(const CRecLookEl* prel, const CMarker* pmkr);
    
    const CField* pfldFirst() const;
    const CField* pfldNext(const CField* pfldCur) const;
};  // class CSubRecord


CSubRecord::CSubRecord(const CRecLookEl* prel, const CMarker* pmkr)
{
    ASSERT( prel );
    m_prec = prel->prec();
    ASSERT( m_prec );
    ASSERT( pmkr );
    
    // Find the first field of the subrecord, which depends on the
    // hierarchical relationship of the primary field and the marker
    // as well as the actual contents of this record.
    m_pfldFirst = NULL;
    CField* pfldPriKey = prel->pfldPriKey();
    if ( !pfldPriKey || !pmkr->pmkrOverThis() )
        m_pfldFirst = m_prec->pfldFirst();  // Scope is entire record
    else
        {
        const CMarker* pmkrPriKey = pfldPriKey->pmkr();
        if ( !pmkrPriKey->pmkrOverThis() )
            m_pfldFirst = m_prec->pfldFirst();  // Scope is entire record
        else if ( pmkrPriKey == pmkr || pmkrPriKey->bOver(pmkr) )
            m_pfldFirst = pfldPriKey;
        else
            {
            const CMarker* pmkrOver = ( pmkr->bOver(pmkrPriKey) ?
                pmkr :
                pmkr->pmkrLowestOverBoth(pmkrPriKey)
                );
            if ( pmkrOver )  // 1996-07-26 MRP
                {
                Level lev = pmkrOver->lev();
                CField* pfld = m_prec->pfldPrev(pfldPriKey);
                for ( ; pfld; pfld = m_prec->pfldPrev(pfld) )
                    if ( pfld->pmkr() == pmkrOver || pfld->pmkr()->lev() < lev )
                        break;
                ASSERT( pfld );
                m_pfldFirst = pfld;
                }
            else
                m_pfldFirst = m_prec->pfldFirst();  // Scope is entire record
            }
        }
    ASSERT( m_pfldFirst );
    m_levFirst = m_pfldFirst->pmkr()->lev();
}

const CField* CSubRecord::pfldFirst() const
{
    return m_pfldFirst;
}

const CField* CSubRecord::pfldNext(const CField* pfldCur) const
{
    const CField* pfld = m_prec->pfldNext(pfldCur);
    // The scope of the subrecord is the first field and all fields
    // immediately after it at a lower level (i.e. higher value) than it.
    if ( pfld && m_levFirst != 0 && pfld->pmkr()->lev() <= m_levFirst )
        pfld = NULL;
        
    return pfld;
}

                                                    
// ==========================================================================

BOOL CFilterConCol::bCol() const { return TRUE; }

BOOL CFilterConMarkerString::s_bConstruct(const char* pszMarker,
        const char* pszText, CFilterSet* pfilsetMyOwner,
        CFilterCon **ppcon, CNote** ppnot)
{
    const char* psz = pszText;
    CVarSet* pvarset = pfilsetMyOwner->plngset()->pvarset();
    CPatMChar* ppatString = NULL;
    if ( !CPatMChar::s_bConstruct(&psz, "", "", pvarset, &ppatString, ppnot) )
        {
        (*ppnot)->Add(_("in a marker-string filter condition"));
        return FALSE;
        }
        
    *ppcon = new CFilterConMarkerString(pszMarker,
        ppatString, pszText, pfilsetMyOwner);
    return TRUE;
}

CFilterConMarkerString::CFilterConMarkerString(const char* pszMarker,
        CPatMChar* ppatString, const char* pszText,
        CFilterSet* pfilsetMyOwner)
{   
    ASSERT(pszMarker != NULL);
//  ASSERT(*pszMarker != '\0');  // 1996-08-28 MRP
    ASSERT(ppatString != NULL);
    m_sMarker = pszMarker;
    m_ppatString = ppatString;
    m_sText = pszText;
    m_pfilsetMyOwner = pfilsetMyOwner;
//  ASSERT( m_pfilsetMyOwner );
}

CFilterConMarkerString::~CFilterConMarkerString()
{
    delete m_ppatString;
}


#ifndef MRP_1995_12_12
CFilterConMarkerString::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    // Shoebox 2.0 did not include the key field, but Shw does 1995-06-23
    // 1995-12-12 MRP: We will most likely redesign this class to 
    // know the pointer to marker object, not just the marker string.
    ASSERT( prel );
    CMarkerSet* pmkrset = prel->prec()->pfldFirst()->pmkr()->pmkrsetMyOwner();
    CMarker* pmkr = pmkrset->pmkrSearch(m_sMarker);
    if ( !pmkr )
        return FALSE;
    
    CSubRecord rec(prel, pmkr);
    const CField* pfld = rec.pfldFirst();
    for ( ; pfld; pfld = rec.pfldNext(pfld) )
        if ( pfld->pmkr() == pmkr )
            if ( bMatchField(pfld, m_ppatString, filmat) )  // 1995-07-06 MRP
                return TRUE;
    
    return FALSE;
}
#else
// Original implementation which did not use marker hierarchy
CFilterConMarkerString::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst();
    // Shoebox 2.0 did not include the key field, but Shw does 1995-06-23
    for ( ; pfld != NULL; pfld = prec->pfldNext(pfld))
        if ( bEqual(pfld->sMarker(), m_sMarker) )  // MRP: exact or partial match???
//          if (m_ppatString->bMatch(pfld, filmat))
            if ( bMatchField(pfld, m_ppatString, filmat) )  // 1995-07-06 MRP
                return TRUE;
    
    return FALSE;
}
#endif  // MRP


CFilterConMarkerString::bMatchCol(const CRecLookEl* prel, CFilterMatch& filmat,
                                                    COL colMatchAt) const
{
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst();
    // Shoebox 2.0 does not include the key field
    ASSERT(pfld != NULL);
    pfld = prec->pfldNext(pfld);    
    for ( ; pfld != NULL; pfld = prec->pfldNext(pfld))
        if ( bEqual(pfld->sMarker(), m_sMarker) )
            {
            COL col = 0;
            for (CMStringLoc mksloc(pfld); mksloc.bNotAtEnd(); mksloc.Next())
                {
                if ((col == colMatchAt))
                    if (m_ppatString->bMatchAt(mksloc, filmat))
                        return TRUE;
                col = (mksloc.bAtNewline() ? 0 : col + 1);
                }
            }
    
    return FALSE;
}


CFilterCon* CFilterConMarkerString::pconCopy() const
{
    CFilterCon* pcon = NULL;
    CNote* pnot = NULL;
    BOOL bCopied = CFilterConMarkerString::s_bConstruct(m_sMarker,
        m_sText, m_pfilsetMyOwner, &pcon, &pnot);
    ASSERT( bCopied );
            
    return pcon;
}

void CFilterConMarkerString::SetRecordMarker(CMarker* pmkrRecord)
{
    ASSERT( pmkrRecord );
    if ( m_sMarker.IsEmpty() )
        m_sMarker = pmkrRecord->sMarker();
}

    
void CFilterConMarkerString::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, "\\");
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, m_sMarker);
    if ( !m_ppatString->bEmpty() )
        DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_ppatString->Draw(plbo, cDC, rcItem, pxLeft, 0, plngText());  // 1998-02-13 MRP
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}

void CFilterConMarkerString::DrawElementText(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int xElement, int xText) const
{
    ASSERT( plbo );
    int xLeft = xElement;
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, xText, "\\");
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, xText, m_sMarker);
    xLeft = xText;
    m_ppatString->Draw(plbo, cDC, rcItem, &xLeft, 0, plngText());  // 1998-02-13 MRP
}

CLangEnc* CFilterConMarkerString::plngText() const
{
    CMarker* pmkr = m_pfilsetMyOwner->pmkrset()->pmkrSearch(m_sMarker);
    CLangEnc* plng = ( pmkr ? pmkr->plng() : NULL );  // 1998-02-13 MRP
    return plng;
}

BOOL CFilterConMarkerString::bModify()
{
    CMarkerTextDlg dlg(this);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilterConMarkerString::bModifyProperties(const char* pszMarker,
        const char* pszText, CNote** ppnot)
{
    const char* psz = pszText;
    CVarSet* pvarset = m_pfilsetMyOwner->plngset()->pvarset();
    CPatMChar* ppat = NULL;
    if ( !CPatMChar::s_bConstruct(&psz, "", "", pvarset, &ppat, ppnot) )
        return FALSE;

    ASSERT( pszMarker );
    m_sMarker = pszMarker;      
    delete m_ppatString;
    m_ppatString = ppat;
    m_sText = pszText;
    
    return TRUE;
}


#ifdef _DEBUG
void CFilterConMarkerString::AssertValid() const
{
    ASSERT( !m_sMarker.IsEmpty() );
    ASSERT_VALID_NONAFX( m_ppatString );
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

BOOL CFilterConLangEncString::s_bConstruct(CLangEnc* plng,
        const char* pszText, CFilterSet* pfilsetMyOwner,
        CFilterCon **ppcon, CNote** ppnot)
{
    CLangEncSet* plngset = pfilsetMyOwner->plngset();
    CVarSet* pvarset = plngset->pvarset();
    CPatMChar* ppatString = NULL;
    const char* psz = pszText;
    if ( !CPatMChar::s_bConstruct(&psz, "", "", pvarset, &ppatString, ppnot) )
        {
        (*ppnot)->Add(_("in a marker-string filter condition"));
        return FALSE;
        }
        
    *ppcon = new CFilterConLangEncString(plng,
        ppatString, pszText, pfilsetMyOwner);
    return TRUE;
}

CFilterConLangEncString::CFilterConLangEncString(CLangEnc* plng,
        CPatMChar* ppatString, const char* pszText,
        CFilterSet* pfilsetMyOwner)
{   
    m_plng = plng;
    m_ppatString = ppatString;
    ASSERT( m_ppatString );
    ASSERT( pszText );
    m_sText = pszText;
    m_pfilsetMyOwner = pfilsetMyOwner;
    ASSERT( m_pfilsetMyOwner );
}

CFilterConLangEncString::~CFilterConLangEncString()
{
    delete m_ppatString;
}


CFilterConLangEncString::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    ASSERT( prel );
    CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst();
    for ( ; pfld; pfld = prec->pfldNext(pfld) )
        if ( !m_plng || pfld->pmkr()->plng() == m_plng )
            if ( bMatchField(pfld, m_ppatString, filmat) )  // 1995-07-06 MRP
                return TRUE;
    
    return FALSE;
}

static const char* s_pszAny = "Any Lang.";

const char* CFilterConLangEncString::pszAnyLangEncName() const
{
    const char* psz = s_pszAny;
    if ( m_plng )
        psz = m_plng->sName();
    return psz;
}


CFilterCon* CFilterConLangEncString::pconCopy() const
{
    CFilterCon* pcon = NULL;
    CNote* pnot = NULL;
    BOOL bCopied = CFilterConLangEncString::s_bConstruct(m_plng, m_sText,
        m_pfilsetMyOwner, &pcon, &pnot);
    ASSERT( bCopied );
            
    return pcon;
}

    

void CFilterConLangEncString::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    ASSERT( plbo );
    const char* psz = pszAnyLangEncName();
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, psz);
    if ( !m_ppatString->bEmpty() )
        DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_ppatString->Draw(plbo, cDC, rcItem, pxLeft, 0, m_plng);  // 1998-02-13 MRP
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}

void CFilterConLangEncString::DrawElementText(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int xElement, int xText) const
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, xElement, xText, pszAnyLangEncName());
    int xLeft = xText;
    m_ppatString->Draw(plbo, cDC, rcItem, &xLeft, 0, m_plng);  // 1998-02-13 MRP
}

BOOL CFilterConLangEncString::bModify()
{
    CLangEncTextDlg dlg(this);
    return ( dlg.DoModal() == IDOK );

    return FALSE;
}

BOOL CFilterConLangEncString::bModifyProperties(CLangEnc* plng,
        const char* pszText, CNote** ppnot)
{
    const char* psz = pszText;
    CVarSet* pvarset = m_pfilsetMyOwner->plngset()->pvarset();
    CPatMChar* ppat = NULL;
    if ( !CPatMChar::s_bConstruct(&psz, "", "", pvarset, &ppat, ppnot) )
        return FALSE;

    m_plng = plng;
    delete m_ppatString;
    m_ppatString = ppat;
    m_sText = pszText;
    
    return TRUE;
}


#ifdef _DEBUG
void CFilterConLangEncString::AssertValid() const
{
    ASSERT( m_plng );
    ASSERT_VALID_NONAFX( m_ppatString );
}
#endif  // _DEBUG


// --------------------------------------------------------------------------


CFilterConKeyString::CFilterConKeyString(CPatMChar* ppatString)
{
    ASSERT(ppatString != NULL);
    m_ppatString = ppatString;
}


CFilterConKeyString::~CFilterConKeyString()
{
    delete m_ppatString;
}


CFilterConKeyString::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    const CRecord* prec = prel->prec();
    const CField* pfldKey = prec->pfldFirst();  
    ASSERT(pfldKey != NULL);
    return bMatchField(pfldKey, m_ppatString, filmat);  // 1995-07-06 MRP
}


#ifdef _DEBUG
void CFilterConKeyString::AssertValid() const
{
    ASSERT( m_ppatString != NULL );
    ASSERT_VALID_NONAFX( m_ppatString );
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

CFilterConSearchString::CFilterConSearchString(CPatMChar* ppatString)
{
    ASSERT(ppatString != NULL);
    m_ppatString = ppatString;
}


CFilterConSearchString::~CFilterConSearchString()
{
    delete m_ppatString;
}


CFilterConSearchString::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    // Shoebox 2.0 does include the key field
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst(); 
    for ( ; pfld != NULL; pfld = prec->pfldNext(pfld))
//      if ( m_ppatString->bMatch(pfld, filmat) )
        if ( bMatchField(pfld, m_ppatString, filmat) )  // 1995-07-06 MRP
            return TRUE;
    
    return FALSE;
}


#ifdef _DEBUG
void CFilterConSearchString::AssertValid() const
{
    ASSERT( m_ppatString != NULL );
    ASSERT_VALID_NONAFX( m_ppatString );
}
#endif  // _DEBUG

                                                    
// --------------------------------------------------------------------------

CFilterConFieldMarker::CFilterConFieldMarker(const char* pszMarker)
{
    m_sMarker = pszMarker;
}


#ifndef MRP_1995_12_12
CFilterConFieldMarker::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    // 1995-12-12 MRP: We will most likely redesign this class to 
    // know the pointer to marker object, not just the marker string.
    ASSERT( prel );
    CMarkerSet* pmkrset = prel->prec()->pfldFirst()->pmkr()->pmkrsetMyOwner();
    CMarker* pmkr = pmkrset->pmkrSearch(m_sMarker);
    if ( !pmkr )
        return FALSE;
    
    CSubRecord rec(prel, pmkr);
    const CField* pfld = rec.pfldFirst();
    for ( ; pfld; pfld = rec.pfldNext(pfld) )
        if ( pfld->pmkr() == pmkr )
            return TRUE;
    
    return FALSE;
}
#else
// Original implementation which did not use marker hierarchy
CFilterConFieldMarker::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst(); 
    // Shoebox 2.0 does not include the key field
    ASSERT( pfld != NULL );
    pfld = prec->pfldNext(pfld);
    for ( ; pfld != NULL; pfld = prec->pfldNext(pfld))
        if ( bEqual(pfld->sMarker(), m_sMarker) )
            return TRUE;
    
    return FALSE;
}
#endif  // MRP


#ifdef _DEBUG
void CFilterConFieldMarker::AssertValid() const
{
    ASSERT( !m_sMarker.IsEmpty() );
}
#endif  // _DEBUG


// --------------------------------------------------------------------------

BOOL CFilterConDate::s_bConstruct(const char* pszMarker,
        int iRelation, const Str8& sDate, CFilterSet* pfilsetMyOwner,
        CFilterCon **ppcon, CNote** ppnot)
{
    CDateCon* pdat = NULL;
    if ( !CDateCon::s_bConstruct(iRelation, sDate, &pdat, ppnot) )
        return FALSE;

    ASSERT( pdat );
    *ppcon = new CFilterConDate(pszMarker, *pdat, pfilsetMyOwner);
    delete pdat;
    return TRUE;
}

CFilterConDate::CFilterConDate(const char* pszMarker, const CDateCon& dat,
        CFilterSet* pfilsetMyOwner)
{   
    ASSERT(pszMarker != NULL);
    ASSERT(*pszMarker != '\0');
    m_sMarker = pszMarker;
    m_pdat = new CDateCon(dat);  // 1998-04-08 MRP
    m_pfilsetMyOwner = pfilsetMyOwner;
}

CFilterConDate::~CFilterConDate()
{
    delete m_pdat;  // 1998-04-08 MRP
}


CFilterCon* CFilterConDate::pconCopy() const
{
    ASSERT( m_pdat );
    CFilterConDate* pcon = new CFilterConDate(m_sMarker, *m_pdat,
        m_pfilsetMyOwner);
    return pcon;
}


CFilterConDate::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    // 1995-12-12 MRP: We will most likely redesign this class to 
    // know the pointer to marker object, not just the marker string.
    ASSERT( prel );
    CMarkerSet* pmkrset = prel->prec()->pfldFirst()->pmkr()->pmkrsetMyOwner();
    CMarker* pmkr = pmkrset->pmkrSearch(m_sMarker);
    if ( !pmkr )
        return FALSE;
    
    CSubRecord rec(prel, pmkr);
    const CField* pfld = rec.pfldFirst();
    for ( ; pfld; pfld = rec.pfldNext(pfld) )
        if ( pfld->pmkr() == pmkr )
            if ( m_pdat->bMatches(pfld->sContents()) )  // 1998-04-08 MRP
                return TRUE;
    
    return FALSE;
}

    
int CFilterConDate::iRelation() const
{
    return m_pdat->iRelation();
}

const Str8& CFilterConDate::sDate() const
{
    return m_pdat->sDate();
}

void CFilterConDate::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    ASSERT( plbo );
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, "\\");
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, m_sMarker);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, m_pdat->pszRelation());
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, m_pdat->sDate(), plngDate());  // 1998-02-13 MRP
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}

void CFilterConDate::DrawElementText(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int xElement, int xText) const
{
    ASSERT( plbo );
    int xLeft = xElement;
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, xText, "\\");
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, xText, m_sMarker);
    xLeft = xText;
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, 0, m_pdat->pszRelation());
    DrawSpace(plbo, cDC, rcItem, &xLeft, 0);
    plbo->DrawSubItemJustify(cDC, rcItem, &xLeft, 0, m_pdat->sDate(), plngDate());  // 1998-02-13 MRP
}

CLangEnc* CFilterConDate::plngDate() const
{
    CMarker* pmkr = m_pfilsetMyOwner->pmkrset()->pmkrSearch(m_sMarker);
    CLangEnc* plng = ( pmkr ? pmkr->plng() : NULL );  // 1998-02-13 MRP
    return plng;
}

BOOL CFilterConDate::bModify()
{
    CMarkerDateDlg dlg(this);
    return ( dlg.DoModal() == IDOK );
}

BOOL CFilterConDate::bModifyProperties(const char* pszMarker,
        int iRelation, const Str8& sDate, CNote** ppnot)
{
    // 1998-04-08 MRP
    if ( !m_pdat->bModifyProperties(iRelation, sDate, ppnot) )
        return FALSE;

    ASSERT( pszMarker );
    m_sMarker = pszMarker;      
    return TRUE;
}


#ifdef _DEBUG
void CFilterConDate::AssertValid() const
{
    ASSERT( !m_sMarker.IsEmpty() );
    // 1995-02-27 MRP: any validity conditions for dates???
}
#endif  // _DEBUG

                                                    
// ==========================================================================

CFilterConNonUnique::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    const CRecLookEl* prelPrev = filmat.prelPrev(prel);
    const CRecLookEl* prelNext = filmat.prelNext(prel);

    // If this rel has not been inserted into the index, we can't actually know if it
    // is unique or not because we can't check it's neighbors. So assume it is non-unique
    // for now. User can force a manual refresh later to reapply the filter when the index is
    // up-to-date.
    if ( !prelPrev && !prelNext && !filmat.pind()->bIsMember(prel) )
        return TRUE;

    // Assuming that the record list is sorted, this record is not unique
    // if its key matches exactly either that of the previous or next record.
    // 1995-05-17 MRP: Need to compare keys according to the sort order!!!
    if ( (prelPrev != NULL) && (filmat.pind()->iCompare(prelPrev, prel, filmat.matset()) == 0) )
        return TRUE;
    if ( (prelNext != NULL) && (filmat.pind()->iCompare(prelNext, prel, filmat.matset()) == 0) )
        return TRUE;
    
    return FALSE;
}


void CFilterConNonUnique::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    Str8 sExternalElement;  // 1997-12-01 MRP
    sExternalElement = "Non-Unique";
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, sExternalElement);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}
                                                    
// ==========================================================================

//CFilterConWith::CFilterConWith(CFilterConMarkerString* pconA, CFilterConCol* pconB)
CFilterConWith::CFilterConWith(CFilterConCol* pconA, CFilterConMarkerString* pconB)
{
    ASSERT(pconA != NULL);
    ASSERT(pconB != NULL);
    m_pconA = pconA;
    m_pconB = pconB;
}


CFilterConWith::~CFilterConWith()
{
    delete m_pconA;
    delete m_pconB;
}


// MRP: This implementation uses friendship with CFilterConMarkerString.
// TODO: Encapsulate better by adding to that class a member function
// to match next occurrence of its pattern and return column.
// 1995-02-20 MRP: temporarily reverse roles of A and B.
BOOL CFilterConWith::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst(); 
    ASSERT(pfld != NULL);
    
    // Column filter is not intended to match on key field
    pfld = prec->pfldNext(pfld);
    
    for ( ; pfld != NULL; pfld = prec->pfldNext(pfld))
        if ( bEqual(pfld->sMarker(), m_pconB->m_sMarker) )
            {
            COL col = 0;
            CMStringLoc mksloc(pfld);
            while ( mksloc.bNotAtEnd() )
                {
                // 2000-06-16 MRP: Use a temporary location variable,
                // because bMatchAt changes the location argument
                // if condition B is matched. If A is not matched,
                // this can skip potential locations or cause an overrun.
                // Note: The With filter element does not limit matches
                // to data fields that are related to each other
                // according to the marker hierarchy. Since the changes
                // would not have been trivial, they were not attempted.
                CMStringLoc mkslocMatch(mksloc);
                if (m_pconB->m_ppatString->bMatchAt(mkslocMatch, filmat) &&
                    m_pconA->bMatchCol(prel, filmat, col))
                        return TRUE;
                col = (mksloc.bAtNewline() ? 0 : col + 1);
                mksloc.Next();
                }
            }
    
    return FALSE;
}

BOOL CFilterConWith::bMatchCol(const CRecLookEl* prel, CFilterMatch& filmat,
                                                COL colMatchAt) const
{
    return (m_pconA->bMatchCol(prel, filmat, colMatchAt) &&
            m_pconB->bMatchCol(prel, filmat, colMatchAt));
}


void CFilterConWith::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pconA->SetRecordMarker(pmkrRecord);
    m_pconB->SetRecordMarker(pmkrRecord);
}


void CFilterConWith::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    m_pconA->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    Str8 sExternalElement;  // 1997-12-01 MRP
    sExternalElement = "With";
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, sExternalElement);
    // plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszWith);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_pconB->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}


#ifdef _DEBUG
void CFilterConWith::AssertValid() const
{
    ASSERT( m_pconA != NULL );
    ASSERT( m_pconB != NULL );
    ASSERT_VALID_NONAFX( m_pconA );
    ASSERT_VALID_NONAFX( m_pconB );
}
#endif  // _DEBUG

                                                    
// --------------------------------------------------------------------------

CFilterConAnd::CFilterConAnd(CFilterCon* pconA, CFilterCon* pconB)
{
    m_pconA = pconA;
    m_pconB = pconB;
}


CFilterConAnd::~CFilterConAnd()
{
    delete m_pconA;
    delete m_pconB;
}


BOOL CFilterConAnd::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    return (m_pconA->bMatch(prel, filmat) && m_pconB->bMatch(prel, filmat));
}


void CFilterConAnd::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pconA->SetRecordMarker(pmkrRecord);
    m_pconB->SetRecordMarker(pmkrRecord);
}


void CFilterConAnd::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    m_pconA->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    Str8 sExternalElement;  // 1997-12-01 MRP
    sExternalElement = "And";
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, sExternalElement);
    // plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszAnd);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_pconB->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}

#ifdef _DEBUG
void CFilterConAnd::AssertValid() const
{
    ASSERT( m_pconA != NULL );
    ASSERT( m_pconB != NULL );
    ASSERT_VALID_NONAFX( m_pconA );
    ASSERT_VALID_NONAFX( m_pconB );
}
#endif  // _DEBUG

                                                    
// --------------------------------------------------------------------------

CFilterConOr::CFilterConOr(CFilterCon* pconA, CFilterCon* pconB)
{
    m_pconA = pconA;
    m_pconB = pconB;
}


CFilterConOr::~CFilterConOr()
{
    delete m_pconA;
    delete m_pconB;
}


BOOL CFilterConOr::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    return (m_pconA->bMatch(prel, filmat) || m_pconB->bMatch(prel, filmat));
}


void CFilterConOr::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pconA->SetRecordMarker(pmkrRecord);
    m_pconB->SetRecordMarker(pmkrRecord);
}


void CFilterConOr::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    m_pconA->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    Str8 sExternalElement;  // 1997-12-01 MRP
    sExternalElement = "Or";
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, sExternalElement);
    // plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszOr);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_pconB->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}


#ifdef _DEBUG
void CFilterConOr::AssertValid() const
{
    ASSERT( m_pconA != NULL );
    ASSERT( m_pconB != NULL );
    ASSERT_VALID_NONAFX( m_pconA );
    ASSERT_VALID_NONAFX( m_pconB );
}
#endif  // _DEBUG

                                                    
// --------------------------------------------------------------------------

CFilterConNot::CFilterConNot(CFilterCon* pcon)
{
    m_pcon = pcon;
}


CFilterConNot::~CFilterConNot()
{
    delete m_pcon;
}


BOOL CFilterConNot::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    return (! m_pcon->bMatch(prel, filmat));
}


void CFilterConNot::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pcon->SetRecordMarker(pmkrRecord);
}


void CFilterConNot::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    Str8 sExternalElement;  // 1997-12-01 MRP
    sExternalElement = "Not";
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, sExternalElement);
    // plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszNot);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_pcon->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}


#ifdef _DEBUG
void CFilterConNot::AssertValid() const
{
    ASSERT( m_pcon != NULL );
    ASSERT_VALID_NONAFX( m_pcon );
}
#endif  // _DEBUG

                                                    
// --------------------------------------------------------------------------

CFilterConParen::CFilterConParen(CFilterCon* pcon)
{
    m_pcon = pcon;
}


CFilterConParen::~CFilterConParen()
{
    delete m_pcon;
}


BOOL CFilterConParen::bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const
{
    return m_pcon->bMatch(prel, filmat);
}


void CFilterConParen::SetRecordMarker(CMarker* pmkrRecord)
{
    m_pcon->SetRecordMarker(pmkrRecord);
}


void CFilterConParen::DrawCondition(CDblListListBox* plbo, CDC& cDC,
        const RECT& rcItem, int* pxLeft, int xRight) const
{
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszLParen);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    m_pcon->DrawCondition(plbo, cDC, rcItem, pxLeft, xRight);
//  DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
    plbo->DrawSubItemJustify(cDC, rcItem, pxLeft, xRight, s_pszRParen);
    DrawSpace(plbo, cDC, rcItem, pxLeft, xRight);
}


#ifdef _DEBUG
void CFilterConParen::AssertValid() const
{
    ASSERT( m_pcon != NULL );
    ASSERT_VALID_NONAFX( m_pcon );
}
#endif  // _DEBUG
