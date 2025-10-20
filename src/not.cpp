// not.cpp  Implementation of notes describing problems  (1995-05-08)

// Change history:
// 1995-05-08 0.15  MRP: Initial design sketch to replace simple sErr


#include "stdafx.h"
#include "toolbox.h"
#include "not.h"
#include "tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// **************************************************************************

CNoteElement::CNoteElement(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent)
{
    ASSERT( pszArgs != NULL );
    ASSERT( pszReferent != NULL );
    ASSERT( pszStringContainingReferent != NULL );
    Initialize(idsNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
}


CNoteElement::CNoteElement(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent)
{
    ASSERT( pszArgs != NULL );
    ASSERT( pszReferent != NULL );
    Initialize(idsNote, pszArgs, pszReferent, lenReferent);
}


CNoteElement::CNoteElement(IDString idsNote, const char* pszArgs)
{
    ASSERT( pszArgs != NULL );
    Initialize(idsNote, pszArgs);
}

void CNoteElement::Initialize(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent)
{
    // The constructors above have already asserted that their args are valid.
    m_idsNote = idsNote;
    m_sArgs = pszArgs;
    m_pszReferent = pszReferent;
    m_lenReferent = lenReferent;
    m_pszStringContainingReferent = pszStringContainingReferent;
}

CNoteElement::CNoteElement(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent) // 1.4ek
{
    ASSERT( pszArgs != NULL );
    ASSERT( pszReferent != NULL );
    ASSERT( pszStringContainingReferent != NULL );
    Initialize(pszNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
}


CNoteElement::CNoteElement(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent) // 1.4ek
{
    ASSERT( pszArgs != NULL );
    ASSERT( pszReferent != NULL );
    Initialize(pszNote, pszArgs, pszReferent, lenReferent);
}


CNoteElement::CNoteElement(const char* pszNote, const char* pszArgs) // 1.4ek
{
    ASSERT( pszArgs != NULL );
    Initialize(pszNote, pszArgs);
}


void CNoteElement::Initialize(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent)
{
    // The constructors above have already asserted that their args are valid.
    m_idsNote = 0; // 1.4ek
    m_sNote = pszNote;
    m_sArgs = pszArgs;
    m_pszReferent = pszReferent;
    m_lenReferent = lenReferent;
    m_pszStringContainingReferent = pszStringContainingReferent;
}

Str8 CNoteElement::sNote() const
{
    // 1995-05-15 MRP: Eventually fill arguments into placeholders
    // For now concatenation seems adequate.
    Str8 s;
	if ( !m_sNote.IsEmpty() ) // 1.4ek If string is initialized, use it instead of ID
		s = m_sNote;
//	else
//	    LoadStringx(s, m_idsNote);  // returns FALSE if no resource // 1.4gb Note never gets an ID
    s += m_sArgs;
    
    return s;
}


BOOL CNoteElement::bSubstitute(IDString idsIfThat, IDString idsSubstituteThis)
{
    if ( m_idsNote == idsIfThat )
        {
        m_idsNote = idsSubstituteThis;
        return TRUE;
        }
    return FALSE;
}

BOOL CNoteElement::bSubstitute(const char* pszIfThat, const char* pszSubstituteThis) // 1.4ek
{
    if ( m_sNote == pszIfThat )
        {
        m_sNote = pszSubstituteThis;
        return TRUE;
        }
    return FALSE;
}


void CNoteElement::SetStringContainingReferent(const char* pszString)
{
    ASSERT( pszString != NULL );
    ASSERT( m_pszStringContainingReferent == NULL );
    m_pszStringContainingReferent = pszString;
}


BOOL CNoteElement::bRefersToString(const char* pszString) const
{
    BOOL bSameString = (pszString == m_pszStringContainingReferent);
    return bSameString;
}


void CNoteElement::Referent(const char** ppszReferent,
            Length* plenReferent) const
{
    ASSERT( m_pszStringContainingReferent != NULL );
    ASSERT( m_pszReferent != NULL );
    *ppszReferent = m_pszReferent;  
    *plenReferent = m_lenReferent;
}


DWORD CNoteElement::dwSelReferent() const
{
    ASSERT( m_pszStringContainingReferent != NULL );
    ASSERT( m_pszReferent != NULL );
    Length offReferentStart = 
        m_pszReferent - m_pszStringContainingReferent;
    Length offReferentEnd = offReferentStart + m_lenReferent;   
    DWORD dw = MAKELONG(offReferentStart, offReferentEnd);
    return dw;
}

DWORD CNoteElement::dwMultiLineSelReferent() const
{
    ASSERT( m_pszStringContainingReferent != NULL );
    ASSERT( m_pszReferent != NULL );
    Length offReferentStart = 
        m_pszReferent - m_pszStringContainingReferent;
#ifndef _MAC
    const char* psz = m_pszStringContainingReferent;
    ASSERT( psz <= m_pszReferent );
    for ( ; psz != m_pszReferent; psz++ )
        {
        ASSERT( *psz );
        if ( *psz == '\n' )
            offReferentStart += 1;  // Account for the extra \r for every \n
        }
#endif

    Length offReferentEnd = offReferentStart + m_lenReferent;   
#ifndef _MAC
    const char* pszEnd = psz + m_lenReferent;
    ASSERT( psz <= pszEnd );
    for ( ; psz != pszEnd; psz++ )
        {
        ASSERT( *psz );
        if ( *psz == '\n' )
            offReferentEnd += 1;  // Account for the extra \r for every \n
        }
#endif

    DWORD dw = MAKELONG(offReferentStart, offReferentEnd);
    return dw;
}


// --------------------------------------------------------------------------

CNote::CNote(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
    Add(pnel);
}


CNote::CNote(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs, pszReferent, lenReferent);
    Add(pnel);
}


CNote::CNote(IDString idsNote, const char* pszArgs)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs);
    Add(pnel);
}

CNote::CNote(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent) // 1.4ek
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
    Add(pnel);
}


CNote::CNote(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent) // 1.4ek
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs, pszReferent, lenReferent);
    Add(pnel);
}


CNote::CNote(const char* pszNote, const char* pszArgs)
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs); // 1.4ek
    Add(pnel);
}


void CNote::Add(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
    Add(pnel);
}


void CNote::Add(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs, pszReferent, lenReferent);
    Add(pnel);
}


void CNote::Add(IDString idsNote, const char* pszArgs)
{
    CNoteElement* pnel = new CNoteElement(idsNote, pszArgs);
    Add(pnel);
}


BOOL CNote::bSubstitute(IDString idsIfThat, IDString idsSubstituteThis)
{
    return pnelLast()->bSubstitute(idsIfThat, idsSubstituteThis);
}

void CNote::Add(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent) // 1.4ek
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs, pszReferent, lenReferent, pszStringContainingReferent);
    Add(pnel);
}


void CNote::Add(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent) // 1.4ek
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs, pszReferent, lenReferent);
    Add(pnel);
}


void CNote::Add(const char* pszNote, const char* pszArgs) // 1.4ek
{
    CNoteElement* pnel = new CNoteElement(pszNote, pszArgs);
    Add(pnel);
}

BOOL CNote::bSubstitute(const char* pszIfThat, const char* pszSubstituteThis) // 1.4ek
{
    return pnelLast()->bSubstitute(pszIfThat, pszSubstituteThis);
}


void CNote::Add( CNoteElement* pnelNew )
{
    ASSERT( pnelNew != NULL );
    CDblList::Add( pnelNew );
}

Str8 CNote::sNote() const
{
    CNoteElement* pnel = pnelFirst();
    Str8 s = pnel->sNote();
    
    // Append any additional "higher level" notes to the first
    Str8 sSeparator = pNL;
    for ( pnel = pnelNext(pnel); pnel != NULL; pnel = pnelNext(pnel) )
        {
        // The text of each element is separated from the previous
        // by a newline and one additional space of indentation.
        sSeparator += ' ';
        s += (sSeparator + pnel->sNote());
        }
        
    return s;
}


void CNote::SetStringContainingReferent(const char* pszString)
{
    CNoteElement* pnel = pnelFirst();
    for ( ; pnel != NULL; pnel = pnelNext(pnel) )
        pnel->SetStringContainingReferent(pszString);
}


BOOL CNote::bRefersToString(const char* pszString) const
{
    return pnelFirst()->bRefersToString(pszString);
}


void CNote::Referent(const char** ppszReferent, Length* plenReferent) const
{
    pnelFirst()->Referent(ppszReferent, plenReferent);
}


DWORD CNote::dwSelReferent() const
{
    return pnelFirst()->dwSelReferent();
}

DWORD CNote::dwMultiLineSelReferent() const
{
    return pnelFirst()->dwMultiLineSelReferent();
}


// --------------------------------------------------------------------------

void CNoteList::Add( CNote* pnotNew )
{
    ASSERT( pnotNew != NULL );
    CDblList::Add( pnotNew );
}

// **************************************************************************
