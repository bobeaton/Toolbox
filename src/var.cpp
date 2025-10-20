// var.cpp  Implementation of Shoebox variable classes (1995-02-23)
 

#include "stdafx.h"
#include "toolbox.h"
#include "var.h"
#include "patch.h"
#include "lng.h"
#include "obstream.h"  // classes Object_istream, Object_ostream
#include <strstrea.h>
#include "not.h"
#include "shwnotes.h"
#include "project.h"  // Shw_pProject()->iLangCtrlHeight()

#include "var_d.h"
#include "set_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// **************************************************************************

CMCharsInVarSubSet::CMCharsInVarSubSet(CVarSet* pvarset) :
    m_pvarset(pvarset)
{
    m_pvarset->AllocateSubsetBit(m_lBit);
}

CMCharsInVarSubSet::~CMCharsInVarSubSet()
{
    m_pvarset->DeAllocateSubsetBit(m_lBit);
}

BOOL CMCharsInVarSubSet::bIncludes(const CMChar* pmch) const
{
    ASSERT( pmch );
    return (m_lBit & pmch->m_lSubMCharsInVar) != 0L; // Bit is set = included
}

void CMCharsInVarSubSet::Include(CMChar* pmch, BOOL bInclude)
{
    ASSERT( pmch );
    if ( bInclude )
        pmch->m_lSubMCharsInVar |= m_lBit;    // Set the bit   = included
    else  // Exclude
        pmch->m_lSubMCharsInVar &= (~m_lBit); // Clear the bit = excluded
}

// **************************************************************************                                                    

CVar::CVar(const char* pszName, CVarSet* pvarsetMyOwner) :
    CSetEl(pszName),
    m_mchsubsetMCharsInVar(pvarsetMyOwner),
    m_pvarsetMyOwner(pvarsetMyOwner)
{
    ASSERT( pszName != NULL );
    ASSERT( *pszName != '\0' );
    // no embedded white space
    // 1995-04-10 MRP: I need a standard way to validate this condition!!!
    ASSERT( strcspn(pszName, Shw_pszWhiteSpace) == strlen(pszName) );
}

CVar::~CVar()
{
}

CSet* CVar::psetMyOwner()
{
    return NULL;
}


const char* CVar::s_pszInvalidNameChars = "";

BOOL CVar::s_bMatchNameAt(const char** ppszName,
            const char* pszValidDelimiters, Str8& sName, CNote** ppnot)
{
    return Shw_bMatchNameAt(ppszName, s_pszInvalidNameChars,
        pszValidDelimiters, sName, ppnot);
}


void CVar::Text(Str8& sText) const
{
    sText += Shw_pszLMeta;
    sText += sName();
    sText += Shw_pszRMeta;
}


BOOL CVar::bIncludes(const CMChar* pmch) const
{
    ASSERT( pmch );
    return m_mchsubsetMCharsInVar.bIncludes(pmch);
}

void CVar::Include(CMChar* pmch, BOOL bInclude)
{
    ASSERT( pmch );
    m_mchsubsetMCharsInVar.Include(pmch, bInclude);
}


#ifdef _DEBUG
void CVar::AssertValid() const
{
}
#endif  // _DEBUG

// --------------------------------------------------------------------------

static const char s_pszVarPunct[] = "!";  // 1998-01-31 MRP: Had been global (yuck)

CVarSet::CVarSet()
{
    m_lSubAllocMCharsInVarm = 0L;  // 2000-03-17 MRP
    // Must initialize the bits before assigning one to the punctuation variable.
    m_pvarPunct = pvarAdd(s_pszVarPunct);  // 1998-01-31 MRP: Had been global (yuck)
}

void CVarSet::AllocateSubsetBit(long& lBit)
{
    // Find the least significant clear (unallocated) bit
    lBit = 1L;
    for ( ; lBit != 0L; lBit <<= 1 )
        if ( (lBit & m_lSubAllocMCharsInVarm) == 0L )
            break;

    // Note: If there are no unallocated bits left, lBit == 0L.
    // This means that the subset can't include or exclude
    // individual elements.

    m_lSubAllocMCharsInVarm |= lBit;  // Set the bit = allocated
}

void CVarSet::DeAllocateSubsetBit(long& lBit)
{
    m_lSubAllocMCharsInVarm &= (~lBit);  // Clear the bit = available
}

CVar* CVarSet::pvarAdd( const char* pszNameNew )
{
    ASSERT( !pvarSearch(pszNameNew) );
    CVar* pvarNew = new CVar(pszNameNew, this);
    CSet::Add( pvarNew );
    
    return pvarNew;
}


CVar* CVarSet::pvarSearch_AddIfNew( const char* pszName )
{
    CVar* pvar = pvarSearch(pszName);
    if ( pvar == NULL )
        pvar = pvarAdd(pszName);

    return pvar;
}


#ifdef _DEBUG
void CVarSet::AssertValid() const
{
    const CVar* pvarPrev = NULL;    
    for ( CVar* pvar = pvarFirst(); pvar; pvar = pvarNext( pvar ) )
        {
        // Set's lookup key must be identical with Var's string
        ASSERT( bEqual(pvar->sName(), pvar->pszKey()) );
        
        // Each Var in the set must be unique
        if ( pvarPrev != NULL )
            ASSERT( !bEqual(pvar->sName(), pvarPrev->sName()) );
        pvarPrev = pvar;
        
        // Var itself must be valid-- a "deep" AssertValid on the set
        pvar->AssertValid();    
        }
}
#endif  // _DEBUG

// --------------------------------------------------------------------------

    
CVarInstance::CVarInstance(const char* pszName, CVar* pvar, const char* pszChars,
        CVarInstanceSet* pvinsetMyOwner) :
    CSetEl(pszName)
{
    m_pvar = pvar;
    ASSERT( m_pvar );
    ASSERT( pszChars );
    m_sChars = pszChars;
    m_pvinsetMyOwner = pvinsetMyOwner;
    ASSERT( m_pvinsetMyOwner );
}

CSet* CVarInstance::psetMyOwner()
{
    return m_pvinsetMyOwner;
}


static const char* psz_var = "var";
static const char* psz_chars = "chars";

#ifdef lngWritefstream // 1.6.4ab 
void CVarInstance::WriteProperties(Object_ofstream& obs) const
#else
void CVarInstance::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_var, sName());
    
    obs.WriteString(psz_chars, m_sChars);
    
    obs.WriteEndMarker(psz_var);
    obs.WriteNewline();
}

BOOL CVarInstance::s_bReadProperties(Object_istream& obs,
        CVarInstanceSet* pvinsetMyOwner, CVarInstance** ppvin)
{
    Str8 sName;
    if ( !obs.bReadBeginMarker(psz_var, sName) )
        return FALSE;

    // Default properties
    Str8 sChars;     
    
    while ( !obs.bAtEnd() )
        {
        if ( obs.bReadString(psz_chars, sChars) )
            ;
        else if ( obs.bEnd(psz_var) )
            break;  // Stop at unexpected begin or end
        }

    CNote* pnot = NULL;
    if ( !s_bConstruct(sName, sChars, pvinsetMyOwner, ppvin, &pnot) )
        {
        delete pnot;  // 1998-06-17 MRP
        }
        
    return TRUE;  // that the properties were read
}


//BOOL CLangEnc::bAddCharsToVar(const char* pszChars, const CVar* pvar,
//      CNote** ppnot)
static BOOL s_bAddCharsToVar(const char* pszChars, CLangEnc* plng, const CVar* pvar, CNote** ppnot)
{
    ASSERT( pvar );
    // NOTE NOTE NOTE: First check the syntax here, then update the chars!!!
    
    // Remove all characters from this variable
    CMChar* pmch = plng->pmchFirst();
    for ( ; pmch; pmch = plng->pmchNext(pmch) )
        ((CVar*)pvar)->Include(pmch, FALSE);  // 1998-01-30 MRP

    ASSERT( pszChars );
    const char* psz = pszChars; 
    Shw_MatchOptionalWhiteSpaceAt(&psz);
    
    // 1995-02-25 MRP: Upon failure, will also have to remove varref
    // from all the multichars to which it had been added.
    while ( *psz != '\0' )
        {
        Str8 sMChar;
        // Match next meta multi-character
        if( !CLangEnc::s_bMatchMetaMCharAt(&psz, sMChar, ppnot) )
            {
            // Augment the note
            return FALSE;
            }
        ASSERT( !sMChar.IsEmpty() );
            
        // MRP: Eventually it might be an error if the char
        // is not already defined in the language encoding
        CMChar* pmch = plng->pmchSearch_AddIfNew(sMChar);
        
//      if ( pmch->bMemberOfVar(varref) )       
//          { sErr = s_pszErrMCharRepeated; return FALSE; }

        // MRP: Does this belong in the constructor???          
        ((CVar*)pvar)->Include(pmch, TRUE);  // 1998-01-30 MRP
        Shw_MatchOptionalWhiteSpaceAt(&psz);  // any white space after multichar
        }
        
    return TRUE;
}

BOOL CVarInstance::s_bConstruct(const char* pszName, const char* pszChars,
        CVarInstanceSet* pvinsetMyOwner,
        CVarInstance** ppvin, CNote** ppnot)
{
    ASSERT( pvinsetMyOwner );
    if ( !pvinsetMyOwner->bValidName(pszName, ppnot) )  // 1998-06-16 MRP
        { 
        (*ppnot)->SetStringContainingReferent(pszName);
        return FALSE;
        }

    CVar* pvar = pvinsetMyOwner->pvarset()->pvarSearch_AddIfNew(pszName);
    CLangEnc* plng = pvinsetMyOwner->plngMyOwner();
    ASSERT( plng );
    ASSERT( pvar );

    if ( !s_bAddCharsToVar(pszChars, plng, pvar, ppnot) )
//  if ( !plng->bAddCharsToVar(pszChars, pvar, ppnot) )
        return FALSE;

    *ppvin = new CVarInstance(pszName, pvar, pszChars, pvinsetMyOwner);
    return TRUE;
}


CLangEnc* CVarInstance::plng() const
{
    return pvinsetMyOwner()->plngMyOwner();
}



BOOL CVarInstance::bModifyProperties(const char* pszName, const char* pszChars,
        CNote** ppnot)
{
    // NOTE NOTE NOTE: We must construct the update notification
    // *before* changing any member data, but must wait to use it
    // until *after* the modifications have been committed. 
//  CVarUpdate vup(this, pszName, pszChars);
    
    // If the name is being changed, verify that it will be valid
    BOOL bUpdatingName = !bEqual(pszName, sName());
    if ( bUpdatingName && !m_pvinsetMyOwner->bValidNewName(pszName, ppnot) )
        {
        (*ppnot)->SetStringContainingReferent(pszName);
        return FALSE;
        }
    
    // If the characters are being changed, attempt to update it
    ASSERT( pszChars );
    CLangEnc* plng = m_pvinsetMyOwner->plngMyOwner();
    ASSERT( plng );
    if ( !bEqual(pszChars, m_sChars) )
        {
        if ( !s_bAddCharsToVar(pszChars, plng, m_pvar, ppnot) )
//      if ( !plng->bAddCharsToVar(pszChars, pvar, ppnot) )
            {
            (*ppnot)->SetStringContainingReferent(pszChars);
            return FALSE;
            }
        m_sChars = pszChars;
        }

    if ( bUpdatingName )
        {
        CNote* pnot = NULL;
        BOOL bRemoved = s_bAddCharsToVar("", plng, m_pvar, &pnot);
        ASSERT( bRemoved );
        ASSERT( !pnot );
        
        CVarSet* pvarset = m_pvinsetMyOwner->pvarset();
        m_pvar = pvarset->pvarSearch_AddIfNew(pszName);
        ASSERT( m_pvar );  // What if we run out of variables???

        BOOL bAdded = s_bAddCharsToVar(m_sChars, plng, m_pvar, &pnot);
        ASSERT( bAdded );
        ASSERT( !pnot );
        
        m_pvinsetMyOwner->SetVarInstanceName(this, pszName);
        }

    extern void Shw_Update(CUpdate& up);        
//  Shw_Update(vup);  // Notify other objects that this variable is modified
    
    return TRUE;
}

BOOL CVarInstance::bCopy(CSetEl** ppselNew)
{
    CVarInstanceDlg dlg(this, (CVarInstance**)ppselNew);
    return ( dlg.DoModal() == IDOK );
}

BOOL CVarInstance::bModify()
{
    CVarInstanceDlg dlg(this);
    return ( dlg.DoModal() == IDOK );
}

BOOL CVarInstance::bDeletable()
{
//  return !m_pvar->bHasRefs();  // CVar is not yet derived from CSetEl
    return TRUE;
}


// --------------------------------------------------------------------------

CVarInstanceSet::CVarInstanceSet(CLangEnc* plngMyOwner)
{
    m_plngMyOwner = plngMyOwner;
    ASSERT( m_plngMyOwner );
}

static const char* psz_varset = "varset";

#ifdef lngWritefstream // 1.6.4ab 
void CVarInstanceSet::WriteProperties(Object_ofstream& obs) const
#else
void CVarInstanceSet::WriteProperties(Object_ostream& obs) const
#endif
{
    obs.WriteBeginMarker(psz_varset);
    obs.WriteNewline();
    
    const CVarInstance* pvin = pvinFirst();
    for ( ; pvin; pvin = pvinNext(pvin) )
        pvin->WriteProperties(obs);
    
    obs.WriteEndMarker(psz_varset);
}

BOOL CVarInstanceSet::bReadProperties(Object_istream& obs)
{
    if ( !obs.bReadBeginMarker(psz_varset) )
        return FALSE;

    while ( !obs.bAtEnd() )
        {
        CVarInstance* pvin = NULL;
        if ( CVarInstance::s_bReadProperties(obs, this, &pvin) )
            {
            if ( pvin )  // 1998-06-17 MRP: If it had a valid name
                CSet::Add(pvin);
            }
        else if ( obs.bEnd(psz_varset) )
            break;
        }
            
    return TRUE;
}

CVarSet* CVarInstanceSet::pvarset() const
{
    return m_plngMyOwner->plngsetMyOwner()->pvarset();
}

    
static const char* s_pszInvalidNameChars =
        "[]"  // 1998-06-16 MRP: Square brackets enclose variables names
        ;
    
BOOL CVarInstanceSet::bValidName(const char* pszName, CNote** ppnot)
{
    // NOTE NOTE NOTE: Assumes that enclosing white space has already been trimmed
    return Shw_bValidName(pszName, s_pszInvalidNameChars, ppnot);
}


void CVarInstanceSet::SetVarInstanceName(CVarInstance* pvin,
        const char* pszNewName)
{
//  CNote* pnot = NULL;
//  ASSERT( bValidNewMarkerName(pszNewName, &pnot) );

    SetElementKey(pvin, pszNewName);
}

BOOL CVarInstanceSet::bAddVarInstance(const char* pszName, const char* pszChars,
        CVarInstance** ppvin, CNote** ppnot)
{
    CVarInstance* pvin = NULL;
    if ( !CVarInstance::s_bConstruct(pszName, pszChars, this, &pvin, ppnot) )
        return FALSE;
        
    if ( pvinSearch(pszName) )
        {
        *ppnot = new CNote(_("This name has already been used:"), pszName, // 1.5.0ft 
            pszName, strlen(pszName), pszName);
        delete pvin;
        return FALSE;
        }
        
    Add(pvin);
    
//  CVarUpdate vup(pvin);
//  extern void Shw_Update(CUpdate& up);        
//  Shw_Update(vup);  // Notify other objects that this marker is added
    
    *ppvin = pvin;
    return TRUE;
}
    
BOOL CVarInstanceSet::bViewElements()
{
    return FALSE;
}

BOOL CVarInstanceSet::bAdd(CSetEl** ppselNew)
{
    CVarInstanceDlg dlg(this, (CVarInstance**)ppselNew);
    return ( dlg.DoModal() == IDOK );  // show a modal view of a new variable
}


void CVarInstanceSetListBox::InitLabels()
{
    m_xName = xSubItem_Show(IDC_SET_NAME);
    m_xChars = xSubItem_Show(IDC_SET_CHARS);
}

int CVarInstanceSetListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CVarInstanceSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CVarInstance* pvin = (CVarInstance*)pel;
    DrawSubItemJustify(cDC, rcItem, m_xName, m_xChars, pvin->sName());

    // 1998-02-13 MRP: Change from pfntPropertiesDlg to m_plng
    // to allow context-sensitive rendering and right-to-left scripts.
    // Here's a problem: When a font is chosen on the Options page
    // of Language Encoding Properties, these list boxes must use it.
    // Before, we distinguished pfntDlg from pfngPropertiesDlg,
    // but that won't work in this new approach. Instead,
    // we'll consider the font choice to be un-Cancel-able.
    DrawSubItemJustify(cDC, rcItem, m_xChars, 0, pvin->sChars(), pvin->plng());
}

// **************************************************************************                                                    
