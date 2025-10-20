// wdf.cpp  Word formulas for selecting valid parses automatically

#include "stdafx.h"
#include "toolbox.h"
#include "wdf.h"
#include "typ.h"  // CDatabaseType
#include "interlin.h"  // CLookupProc
#include "ind.h"  // CIndexSet
#include "obstream.h"  // Object_istream, Object_ostream

#include "set_d.h"  // CSetDlg
#include "wdf_d.h"  // CWordFormulaDlg
#include "help_ids.h"  // HT_WORD_FORMULAS_RECURSION

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------

CWordPattern::CWordPattern(const Str8& sPattern) :
    m_sPattern(sPattern)
{
    ASSERT( !m_sPattern.IsEmpty() );
    m_nDataItems = 0;
}

CWordPattern::CWordPattern(const CWordPattern& wdp) :
    m_sPattern(wdp.m_sPattern)
{
    ASSERT( !m_sPattern.IsEmpty() );
    m_nDataItems = wdp.m_nDataItems;
    ASSERT( 0 <= m_nDataItems );
}

static const char* psz_wdp = "wdp";

#ifdef typWritefstream // 1.6.4ac
void CWordPattern::WriteProperties(Object_ofstream& obs) const
#else
void CWordPattern::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteString(psz_wdp, m_sPattern);
}

BOOL CWordPattern::s_bReadProperties(Object_istream& obs,
        CWordPattern** ppwdp)
{
    ASSERT( ppwdp );
    ASSERT( !*ppwdp );

    Str8 sPattern;
    if ( !obs.bReadString(psz_wdp, sPattern) )
        return FALSE;

    if ( sPattern.IsEmpty() )
        return TRUE;

    *ppwdp = new CWordPattern(sPattern);
    return TRUE;
}

void CWordPattern::CountDataItems(CWordFormulaSet* pwdfset)
{
    ASSERT( pwdfset );
    m_nDataItems = 0;
    const char* psz = m_sPattern;
    const char* pszItem = NULL;
    int lenItem = 0;
    BOOL bOptional = FALSE;
    while ( bNextItem(&psz, &pszItem, &lenItem, &bOptional) )
        {
        CWordFormula* pwdf = pwdfset->pwdfSearch(pszItem, lenItem);
        if ( pwdf )
            pwdf->SetUsedInPatterns();
        else if ( !bOptional )
            m_nDataItems++;
        }
}

BOOL CWordPattern::bNextItem(const char** ppsz,
        const char** ppszItem, int* plenItem, BOOL* pbOptional)
{
    ASSERT( ppsz );
    const char* psz = *ppsz;
    ASSERT( psz );
    ASSERT( ppszItem );
    ASSERT( plenItem );
    ASSERT( pbOptional );

    while ( *psz == ' ' )
        psz++;

    if ( !*psz )
        return FALSE;

    BOOL bOptional = (*psz == '(');
    if ( bOptional )
        {
        psz++;
        while ( *psz == ' ' )
            psz++;
        }

    const char* pszItem = psz;
    while ( *psz && *psz != ' ' )
        {
//        ASSERT( *psz != '\n' ); // 1.4vxu Remove bad assert in word formulas
        if ( *psz == '\n' ) // 1.4vxu 
			break; // 1.4vxu 
        if ( *psz == ')' )
            break;

        psz++;
        }

    int lenItem = psz - pszItem;
    while ( *psz == ' ' )
        psz++;

    if ( *psz == ')' ) // 6.0r Fix bug of hang on extra close parend on word formula
        psz++;

    *ppsz = psz;
    *ppszItem = pszItem;
    *plenItem = lenItem;
    *pbOptional = bOptional;
    return TRUE;
}

// --------------------------------------------------------------------------

CWordPatternList::CWordPatternList()
{
}

CWordPatternList::CWordPatternList(const CWordPatternList& wdplst)
{
    CWordPattern* pwdp = wdplst.pwdpFirst();
    for ( ; pwdp; pwdp = wdplst.pwdpNext(pwdp) )
        Add(new CWordPattern(*pwdp));
}

static const char* psz_wdplst = "wdplst";

#ifdef typWritefstream // 1.6.4ac
void CWordPatternList::WriteProperties(Object_ofstream& obs) const
#else
void CWordPatternList::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_wdplst);

    const CWordPattern* pwdp = pwdpFirst();
    for ( ; pwdp; pwdp = pwdpNext(pwdp) )
        pwdp->WriteProperties(obs);

    obs.WriteEndMarker(psz_wdplst);
}

BOOL CWordPatternList::bReadProperties(Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_wdplst) )
        return FALSE;

    while ( !obs.bAtEnd() )
        {
        CWordPattern* pwdp = NULL;
        if ( CWordPattern::s_bReadProperties(obs, &pwdp) )
            {
            if ( pwdp )
                Add(pwdp);
            }
        else if ( obs.bEnd(psz_wdplst) )
            break;
        }
        
    return TRUE;
}

void CWordPatternList::CountDataItems(CWordFormulaSet* pwdfset)
{
    CWordPattern* pwdp = pwdpFirst();
    for ( ; pwdp; pwdp = pwdpNext(pwdp) )
        pwdp->CountDataItems(pwdfset);
}

void CWordPatternList::AddWordPatterns(const Str8& sPatterns)
{
    ASSERT( bIsEmpty() );
    const char* psz = sPatterns;
    while ( *psz )
        {
        size_t lenLine = strcspn(psz, "\n");
        Str8 sPattern(psz, lenLine);

        if ( !sPattern.IsEmpty() )
            Add(new CWordPattern(sPattern));
        
        psz += lenLine;
        if ( *psz == '\n' )
            psz++;
        }
}

void CWordPatternList::ReplaceWordPatterns(const Str8& sPatterns)
{
    DeleteAll();
    AddWordPatterns(sPatterns);
}

Str8 CWordPatternList::sPatterns() const
{
    Str8 s;
    BOOL bAfterFirst = FALSE;
    const CWordPattern* pwdp = pwdpFirst();
    for ( ; pwdp; pwdp = pwdpNext(pwdp) )
        {
        if ( bAfterFirst )
            s += '\n';
        s += pwdp->sPattern();
        bAfterFirst = TRUE;
        }

    return s;
}

// --------------------------------------------------------------------------

CWordFormula::CWordFormula(const Str8& sSymbol,
        CWordFormulaSet* pwdfsetMyOwner) :
    CSetEl(sSymbol),
    m_bUsedInPatterns(FALSE),
    m_pwdfsetMyOwner(pwdfsetMyOwner)
{
    Initialize();
}

CWordFormula::CWordFormula(const CWordFormula& wdf,
        CWordFormulaSet* pwdfsetMyOwner) :
    CSetEl(wdf.sName()),
    m_wdplst(wdf.m_wdplst),
    m_bUsedInPatterns(wdf.m_bUsedInPatterns),
    m_pwdfsetMyOwner(pwdfsetMyOwner)
{
    Initialize();
}

void CWordFormula::Initialize()
{
    m_bReplaced = FALSE;
    m_nItemsWhenReplaced = 0;
    m_bEndlessRecursion = FALSE;
    ASSERT( m_pwdfsetMyOwner );

    // Validity conditions for a formula's symbol name:
    ASSERT( sName().Find('\n') < 0 );  // No newlines
    ASSERT( sName().Find(' ') < 0 );  // No spaces
    if ( sName().IsEmpty() )
        ASSERT( FALSE );  // Not empty
    else
        {
        // Doesn't begin or end with parentheses
        ASSERT( sName().GetChar(0) != '(' );
        ASSERT( sName().GetChar(sName().GetLength()-1) != ')' );
        }
    ASSERT( !m_pwdfsetMyOwner->pselSearch(sName()) );  // No duplicates
}

CSet* CWordFormula::psetMyOwner()
{
    return m_pwdfsetMyOwner;
}

BOOL CWordFormula::bPrimary() const
{
    return (this == m_pwdfsetMyOwner->pwdfPrimary());
}

static const char* psz_wdf = "wdf";

#ifdef typWritefstream // 1.6.4ac
void CWordFormula::WriteProperties(Object_ofstream& obs) const
#else
void CWordFormula::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_wdf, sName());
    m_wdplst.WriteProperties(obs);
    obs.WriteEndMarker(psz_wdf);
}

BOOL CWordFormula::s_bReadProperties(Object_istream& obs,
        CWordFormulaSet* pwdfsetMyOwner, CWordFormula** ppwdf)
{
    ASSERT( pwdfsetMyOwner );
    ASSERT( ppwdf );
    ASSERT( !*ppwdf );

    Str8 sSymbol;
    if ( !obs.bReadBeginMarker(psz_wdf, sSymbol) )
        return FALSE;

    // When reading properties, automatically correct problems
    // that might have been caused by editing the .typ file.
    pwdfsetMyOwner->MakeValidSymbol(sSymbol);

    CWordFormula* pwdf = new CWordFormula(sSymbol, pwdfsetMyOwner);

    while ( !obs.bAtEnd() )
        {
        if ( pwdf->m_wdplst.bReadProperties(obs) )
            ;
        else if ( obs.bEnd(psz_wdf) )
            break;
        }

    *ppwdf = pwdf;
    return TRUE;
}

void CWordFormula::AddWordPatterns(const Str8& sPatterns)
{
    m_wdplst.AddWordPatterns(sPatterns);
}

void CWordFormula::CountDataItems()
{
    m_wdplst.CountDataItems(m_pwdfsetMyOwner);
}

BOOL CWordFormula::bCopy(CSetEl** ppselNew)
{
    CWordFormulaDlg dlg(this, (CWordFormula**)ppselNew);
    // return ( dlg.DoModal() == IDOK );
    if ( dlg.DoModal() != IDOK )
        return FALSE;

    m_pwdfsetMyOwner->CountDataItems();
    return TRUE;
}

BOOL CWordFormula::bModify()
{
    CWordFormulaDlg dlg(this);
    // return (dlg.DoModal() == IDOK);
    if ( dlg.DoModal() != IDOK )
        return FALSE;

    m_pwdfsetMyOwner->CountDataItems();
    return TRUE;
}

BOOL CWordFormula::bModifyProperties(const Str8& sSymbol,
        const Str8& sPatterns)
{
    Str8 sMessage;
    if ( !bEqual(sSymbol, sName()) )
        {
        if ( !m_pwdfsetMyOwner->bValidSymbol(sSymbol, sMessage) )
            {
      	    AfxMessageBox(sMessage, MB_ICONINFORMATION);
            return FALSE;
            }

        if ( m_bUsedInPatterns )
            {
			sMessage = _("Delete word formula?"); // 1.5.0fg 
            if ( AfxMessageBox(sMessage, MB_YESNO|MB_ICONQUESTION) != IDYES )
                return FALSE;
            }

        ChangeNameTo(sSymbol);
        }

    m_wdplst.ReplaceWordPatterns(sPatterns);

    return TRUE;
}

BOOL CWordFormula::bDeletable()
{
    return !bPrimary();
}

// --------------------------------------------------------------------------

CWordFormulaSet::CWordFormulaSet(CLookupProc* pintprcMyOwner)
{
    m_bEnabled = TRUE;
    m_pwdfPrimary = NULL;
    m_pwdfSelected = NULL;
    m_sDefaultPrimarySymbol = "Word";
    m_bShowExpandedList = FALSE;
    m_pintprcMyOwner = pintprcMyOwner;
    ASSERT( m_pintprcMyOwner );
}

CWordFormulaSet::CWordFormulaSet(const CWordFormulaSet& wdfset)
{
    CopyFrom(wdfset);
}

void CWordFormulaSet::operator=(const CWordFormulaSet& wdfset)
{
    if ( this == &wdfset )
        return;

    DeleteAll();
    CopyFrom(wdfset);
}

void CWordFormulaSet::CopyFrom(const CWordFormulaSet& wdfset)
{
    m_bEnabled = wdfset.m_bEnabled;
    m_pwdfPrimary = NULL;
    m_pwdfSelected = NULL;
    CWordFormula* pwdf = wdfset.pwdfFirst();
    for ( ; pwdf; pwdf = wdfset.pwdfNext(pwdf) )
        {
        CWordFormula* pwdfCopy = new CWordFormula(*pwdf, this);
        Add(pwdfCopy);
        if ( pwdf == wdfset.m_pwdfPrimary )
            m_pwdfPrimary = pwdfCopy;
        if ( pwdf == wdfset.m_pwdfSelected )
            m_pwdfSelected = pwdfCopy;
        }
    ASSERT( m_pwdfPrimary );
    m_sDefaultPrimarySymbol = wdfset.m_sDefaultPrimarySymbol;
    m_sLangEncPatterns = wdfset.m_sLangEncPatterns;
    m_bShowExpandedList = wdfset.m_bShowExpandedList;
    m_pintprcMyOwner = wdfset.m_pintprcMyOwner;
    ASSERT( m_pintprcMyOwner );
}

BOOL CWordFormulaSet::s_bConvertOlderWordFormulas(const CIndexSet* pindset,
        CLookupProc* pintprc, CWordFormulaSet** ppwdfset)
{
    ASSERT( ppwdfset );
    ASSERT( !*ppwdfset );
    CWordFormulaSet* pwdfset = new CWordFormulaSet(pintprc);
    if ( !pwdfset->bConvertOlderWordFormulas(pindset) )
        return FALSE;

    *ppwdfset = pwdfset;
    return TRUE;
}

BOOL CWordFormulaSet::bConvertOlderWordFormulas(const CIndexSet* pindset)
{
    ASSERT( pindset );
    const CIndex* pind = ((CIndexSet*)pindset)->pindRecordOwner();
    const CRecLookEl* prel = pind->prelFirst();
    const CRecord* prec = prel->prec();
    const CField* pfld = prec->pfldFirst();
    for ( ; pfld; pfld = prec->pfldNext(pfld) )
        {
        const Str8& sMarker = pfld->pmkr()->sMarker();
        if ( bEqual(sMarker, "def") )
            {
            const Str8& sFormula = pfld->sContents();
            CWordFormula* pwdf = NULL;
            if ( bConvertOlderWordFormula(sFormula, &pwdf) )
                {
                Add(pwdf);
                if ( !m_pwdfPrimary )
                    m_pwdfPrimary = pwdf;
                }
            }
        }

    if ( !m_pwdfPrimary )
        AddDefaultPrimaryWordFormula();

    CountDataItems();

    return TRUE;
}

BOOL CWordFormulaSet::bConvertOlderWordFormula(const Str8& sFormula,
        CWordFormula** ppwdf)
{
    ASSERT( ppwdf );
    ASSERT( !*ppwdf );

    Str8 sSymbol;
    Str8 sPatterns;
    int lenPrecedingNewline = sFormula.Find('\n');
    if ( 0 <= lenPrecedingNewline )
        {
        sSymbol = sFormula.Left(lenPrecedingNewline);
        sPatterns = sFormula.Mid(lenPrecedingNewline+1);
        }
    else
        sSymbol = sFormula;

    MakeValidSymbol(sSymbol);
    CWordFormula* pwdf = new CWordFormula(sSymbol, this);

    pwdf->AddWordPatterns(sPatterns);
    *ppwdf = pwdf;
    return TRUE;
}

void CWordFormulaSet::MakeValidSymbol(Str8& sSymbol)
{
    // 1. If the symbol consists of multiple lines, keep only the first.
    int lenPrecedingNewline = sSymbol.Find('\n');
    if ( 0 <= lenPrecedingNewline )
        sSymbol = sSymbol.Left(lenPrecedingNewline);

    // 2. If the symbol contains spaces, remove them.
	sSymbol.Replace( " ", "" ); // 1.4qzfk Upgrade GetBuffr for Unicode build

    // 3. If the symbol begins or ends with parentheses, remove them.
    int len = sSymbol.GetLength();
    if ( len != 0 && sSymbol.GetChar(0) == '(' )
        {
        sSymbol = sSymbol.Mid(1);
        len--;
        }

    if ( len != 0 && sSymbol.GetChar(len-1) == ')' )
        {
        sSymbol = sSymbol.Left(len-1);
        len--;
        }

    // 4. If the symbol is empty, use MissingSymbol.
    if ( sSymbol.IsEmpty() )
        sSymbol = "MissingSymbol";

    // 5. If the symbol already exists, make it unique.
    if ( pselSearch(sSymbol) )
        sSymbol = sUniqueName(sSymbol);
}

const Str8& CWordFormulaSet::sPrimarySymbol() const
{
    ASSERT( m_pwdfPrimary );
    return m_pwdfPrimary->sName();
}

CWordFormula* CWordFormulaSet::pwdfPrimary()
{
    ASSERT( m_pwdfPrimary );
    return m_pwdfPrimary;
}

void CWordFormulaSet::SetPrimaryWordFormula(CWordFormula* pwdf)
{
    ASSERT( pwdf );
    ASSERT( this == pwdf->m_pwdfsetMyOwner );
    m_pwdfPrimary = pwdf;
}

CWordFormula* CWordFormulaSet::pwdfSelected()
{
    if ( m_pwdfSelected )
        return m_pwdfSelected;

    ASSERT( m_pwdfPrimary );
    return m_pwdfPrimary;
}

void CWordFormulaSet::SelectedWordFormula(CWordFormula* pwdf)
{
    ASSERT( pwdf );
    ASSERT( this == pwdf->m_pwdfsetMyOwner );
    m_pwdfSelected = pwdf;
}

CLangEnc* CWordFormulaSet::plngPatterns() const
{
    ASSERT( m_pintprcMyOwner );
    CDatabaseType* ptyp = m_pintprcMyOwner->pintprclst()->ptyp();
    CLangEncSet* plngset = ptyp->pmkrset()->plngset();
    CLangEnc* plng = plngset->plngSearch(m_sLangEncPatterns);
    if ( !plng )
        plng = plngset->plngDefault();
    ASSERT( plng );

    return plng;
}

void CWordFormulaSet::LangEncPatterns(CLangEnc* plng)
{
    ASSERT( plng );
    m_sLangEncPatterns = plng->sName();
}

static const char* psz_wdfset = "wdfset";
static const char* psz_Disabled = "Disabled";
static const char* psz_wdfPrimary = "wdfPrimary";
static const char* psz_lngPatterns = "lngPatterns";

#ifdef typWritefstream // 1.6.4ac
void CWordFormulaSet::WriteProperties(Object_ofstream& obs) const
#else
void CWordFormulaSet::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_wdfset);

    obs.WriteBool(psz_Disabled, !m_bEnabled);
    ASSERT( m_pwdfPrimary );
    obs.WriteString(psz_wdfPrimary, m_pwdfPrimary->sName());

    const CWordFormula* pwdf = pwdfFirst();
    for ( ; pwdf; pwdf = pwdfNext(pwdf) )
        pwdf->WriteProperties(obs);

    CLangEnc* plng = plngPatterns();
    obs.WriteString(psz_lngPatterns, plng->sName());

    obs.WriteEndMarker(psz_wdfset);
}

BOOL CWordFormulaSet::s_bReadProperties(Object_istream& obs,
        CLookupProc* pintprc, CWordFormulaSet** ppwdfset)
{
    ASSERT( ppwdfset );

    if ( !obs.bReadBeginMarker(psz_wdfset) )
        return FALSE;

    CWordFormulaSet* pwdfset = new CWordFormulaSet(pintprc);
    pwdfset->ReadProperties(obs);

    if ( ppwdfset )
        delete *ppwdfset;
    *ppwdfset = pwdfset;
    return TRUE;
}

void CWordFormulaSet::ReadProperties(Object_istream& obs)
{
    Str8 sPrimarySymbol;
    while ( !obs.bAtEnd() )
        {
        BOOL b = FALSE;
        CWordFormula* pwdf = NULL;
        if ( obs.bReadBool(psz_Disabled, b) )
            m_bEnabled = FALSE;
        else if ( obs.bReadString(psz_wdfPrimary, sPrimarySymbol) )
            ;
        else if ( CWordFormula::s_bReadProperties(obs, this, &pwdf) )
            {
            if ( pwdf )
                Add(pwdf);
            }
        else if ( obs.bReadString(psz_lngPatterns, m_sLangEncPatterns) )
            ;
        else if ( obs.bEnd(psz_wdfset) )
            break;
        }

    if ( sPrimarySymbol.IsEmpty() )
        sPrimarySymbol = m_sDefaultPrimarySymbol;
    m_pwdfPrimary = (CWordFormula*)pselSearch(sPrimarySymbol);
    if ( !m_pwdfPrimary )
        AddDefaultPrimaryWordFormula();
    ASSERT( m_pwdfPrimary );

    CountDataItems();
}

void CWordFormulaSet::AddDefaultPrimaryWordFormula()
{
    ASSERT( !m_pwdfPrimary );
    ASSERT( !m_sDefaultPrimarySymbol.IsEmpty() );
    m_pwdfPrimary = new CWordFormula(m_sDefaultPrimarySymbol, this);
    Add(m_pwdfPrimary);
}

CWordFormula* CWordFormulaSet::pwdfSearch(const char* pszSymbol, int lenSymbol) const
{
    return (CWordFormula*)pselSearchForWholeSubString(pszSymbol, lenSymbol);
}

void CWordFormulaSet::CountDataItems()
{
    CWordFormula* pwdf = pwdfFirst();
    for ( ; pwdf; pwdf = pwdfNext(pwdf) )
        {
        pwdf->m_bUsedInPatterns = FALSE;
        pwdf->m_bEndlessRecursion = FALSE;
        }

    pwdf = pwdfFirst();
    for ( ; pwdf; pwdf = pwdfNext(pwdf) )
        pwdf->CountDataItems();
}

void CWordFormulaSet::Delete(CSetEl** ppsel)
{
    CSet::Delete(ppsel);
    CountDataItems();
}

void CWordFormulaSet::s_EnableWordFormulas(CLookupProc* pintprc,
        CWordFormulaSet** ppwdfset)
{
    ASSERT( ppwdfset );
    ASSERT( !*ppwdfset );
    CWordFormulaSet* pwdfset = new CWordFormulaSet(pintprc);
    pwdfset->AddDefaultPrimaryWordFormula();
    ASSERT( pwdfset->m_pwdfPrimary );
    *ppwdfset = pwdfset;
}

BOOL CWordFormulaSet::bModify()
{
    if ( !CWordFormulaSetDlg::s_bDoModal(this) )
        return FALSE;

    CountDataItems();
    return TRUE;
}

BOOL CWordFormulaSet::bAdd(CSetEl** ppselNew)
{
    CWordFormulaDlg dlg(this, (CWordFormula**)ppselNew);
    // return ( dlg.DoModal() == IDOK );
    if ( dlg.DoModal() != IDOK )
        return FALSE;

    CountDataItems();
    return TRUE;
}

BOOL CWordFormulaSet::bAddWordFormula(const Str8& sSymbol,
        const Str8& sPatterns, CWordFormula** ppwdfNew)
{
    ASSERT( ppwdfNew );

    Str8 sMessage;
    if ( !bValidSymbol(sSymbol, sMessage) )
        {
      	AfxMessageBox(sMessage, MB_ICONINFORMATION);
        return FALSE;
        }

    CWordFormula* pwdf = new CWordFormula(sSymbol, this);
    pwdf->AddWordPatterns(sPatterns);
    Add(pwdf);

    *ppwdfNew = pwdf;
    return TRUE;
}

void CWordFormulaSet::Clear()
{
    DeleteAll();
    m_pwdfPrimary = NULL;
    AddDefaultPrimaryWordFormula();
    ASSERT( m_pwdfPrimary );
}

BOOL CWordFormulaSet::bValidSymbol(const Str8& sSymbol, Str8& sMessage)
{
    const char* pszSymbol = sSymbol;

    // 1. The visual user interface shouldn't allow multiple lines.
    ASSERT( !strchr(pszSymbol, '\n') );

    // 2. No spaces.
    const char* pszSpace = strchr(pszSymbol, ' ');
    if ( pszSpace )
        {
        sMessage = _("A word formula's symbol cannot contain spaces.");
        return FALSE;
        }

    // 3. Doesn't begin or end with parentheses.
    if ( *pszSymbol )
        {
        if ( *pszSymbol == '(' )
            {
            sMessage = _("A word formula's symbol cannot be enclosed in parentheses.");
            return FALSE;
            }

        const char* pszLastChar = pszSymbol + strlen(pszSymbol) - 1;
        ASSERT( pszSymbol <= pszLastChar );
        if ( *pszLastChar == ')' )
            {
            sMessage = _("A word formula's symbol cannot be enclosed in parentheses.");
            return FALSE;
            }
        }

    // 4. Not empty.
    if ( !*pszSymbol )
        {
        sMessage = _("A word formula's symbol cannot be empty.");
        return FALSE;
        }

    // 5. No duplicates.
    if ( pselSearch(pszSymbol) )
        {
        sMessage = _("This symbol is already used by another word formula.");
        return FALSE;
        }

    return TRUE;
}

// --------------------------------------------------------------------------

void CWordFormulaSet::MatchParses(CFieldList* pfldlst,
        CFieldList* pfldlstExcluded, BOOL bIncludeInvalidParses)
{
    ASSERT( pfldlst );
    ASSERT( pfldlstExcluded );

    pfldlst->MoveFieldsFrom(pfldlstExcluded);
    ASSERT( !pfldlst->bIsEmpty() );
    m_pintprcMyOwner->DeleteFailMarks(pfldlst);

    CField* pfld = pfldlst->pfldFirst();
	while ( pfld )
		{
    	CField* pfldNext = pfldlst->pfldNext(pfld);
        const Str8& sParse = pfld->sContents();
		if ( bTimeLimitExceeded() ) // 1.3bh Check elapsed time during word formulas
				break;
		if ( !bMatchParse(sParse) )
			pfldlstExcluded->Add(pfldlst->pfldRemove(pfld));
		pfld = pfldNext;
		}

    m_pintprcMyOwner->InsertFailMarks(pfldlstExcluded);
    if ( bIncludeInvalidParses || pfldlst->bIsEmpty() )
	 	pfldlst->MoveFieldsFrom(pfldlstExcluded);
}

BOOL CWordFormulaSet::bMatchParse(const Str8& sParse)
{
    ASSERT( m_pwdfPrimary );
    const char* pszParse = sParse;
    int nItems = m_pintprcMyOwner->nLookupDataGroupsInParse(sParse);
    const char* pszUnmatched = NULL;
    if ( !m_pwdfPrimary->bMatchParse(pszParse, nItems, &pszUnmatched) )
        return FALSE;
    ASSERT( pszUnmatched );
    return (*pszUnmatched == '\0');
}

BOOL CWordFormula::bMatchParse(const char* pszParse, int nItems,
        const char** ppszUnmatched)
{
    ASSERT( pszParse );
    ASSERT( 0 <= nItems );
    ASSERT( ppszUnmatched );

    if ( m_bReplaced )
        {
        ASSERT( nItems <= m_nItemsWhenReplaced );
        if ( m_nItemsWhenReplaced == nItems )
            {
            if ( !m_bEndlessRecursion )
                {
                Str8 sMessage = _("Word formulas have endless recursion on symbol:"); // 1.5.0fg 
				sMessage = sMessage + " " + sName(); // 1.5.0fg 
                AfxMessageBox(sMessage, MB_ICONINFORMATION);
                AfxGetApp()->WinHelp(HT_WORD_FORMULAS_RECURSION);
                }
            m_bEndlessRecursion = TRUE;
            return FALSE;
            }
        }

    BOOL bReplaced = m_bReplaced;
    int nItemsWhenReplaced = m_nItemsWhenReplaced;
    m_bReplaced = TRUE;
    m_nItemsWhenReplaced = nItems;
    BOOL bMatched = FALSE;
    const char* pszUnmatched = pszParse;
    CWordPattern* pwdp = m_wdplst.pwdpFirst();
    for ( ; pwdp; pwdp = m_wdplst.pwdpNext(pwdp) )
        {
        const char* pszUnmatchedByThisPattern = NULL;
        if ( pwdp->bMatchParse(m_pwdfsetMyOwner,
                pszParse, nItems, &pszUnmatchedByThisPattern) )
            {
            bMatched = TRUE;
            ASSERT( pszUnmatchedByThisPattern );
            if ( pszUnmatched < pszUnmatchedByThisPattern )
                pszUnmatched = pszUnmatchedByThisPattern;
            }
        }

    m_bReplaced = bReplaced;
    m_nItemsWhenReplaced = nItemsWhenReplaced;

    if ( !bMatched )
        return FALSE;

    *ppszUnmatched = pszUnmatched;
    return TRUE;
}

BOOL CWordPattern::bMatchParse(CWordFormulaSet* pwdfset,
        const char* pszParse, int nItems, const char** ppszUnmatched)
{
    ASSERT( pwdfset );
    ASSERT( pszParse );
    ASSERT( 0 <= nItems );
    ASSERT( ppszUnmatched );
    if ( nItems < m_nDataItems )
        return FALSE;

    int n = nItems - m_nDataItems;
    const char* psz = pszParse;
    const char* pszUnmatched = NULL;
    const char* pszPattern = m_sPattern;
    const char* pszItem = NULL;
    int lenItem = 0;
    BOOL bOptional = FALSE;
    while ( bNextItem(&pszPattern, &pszItem, &lenItem, &bOptional) )
        {
        if ( !pwdfset->bMatchParseItem(pszItem, lenItem, bOptional,
                psz, n, &pszUnmatched) )
            return FALSE;

        ASSERT( pszUnmatched );
        psz = pszUnmatched;
        }

    *ppszUnmatched = pszUnmatched;
    return TRUE;
}

BOOL CWordFormulaSet::bMatchParseItem(const char* pszItem, int lenItem,
        BOOL bOptional,
        const char* pszParse, int nItems, const char** ppszUnmatched)
{
    ASSERT( pszItem );
    ASSERT( *pszItem != '(' );
    ASSERT( 0 < lenItem );
    ASSERT( pszParse );
    ASSERT( ppszUnmatched );

    BOOL bMatched = FALSE;
    const char* pszUnmatched = NULL;
    CWordFormula* pwdf = pwdfSearch(pszItem, lenItem);
    if ( pwdf )
        bMatched = pwdf->bMatchParse(pszParse, nItems, &pszUnmatched);
    else
        bMatched = m_pintprcMyOwner->bMatchParseLookupData(pszItem, lenItem,
            pszParse, &pszUnmatched);

    if ( !bMatched )
        {
        if ( bOptional )
            pszUnmatched = pszParse;
        else
            return FALSE;
        }
    ASSERT( pszUnmatched );
    *ppszUnmatched = pszUnmatched;
    return TRUE;
}
