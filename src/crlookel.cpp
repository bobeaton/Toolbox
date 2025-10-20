// CRecLookel.cpp CRecLookEl record lookup element class implementation Alan Buseman 25 Jan 95

#include "stdafx.h"
#include "toolbox.h"
#include "crlookel.h"
#include "sfstream.h"  // class Field_ostream

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CRecLookEl::CRecLookEl( const UIdMChar* puch, CRecord* prec, CField* pfldPriKey )
{
    ASSERT( prec );
    m_prec = prec;
    m_pfldPriKey = pfldPriKey;
    // 1999-04-30 MRP: Replace m_sKey
    m_puchSortKey = NULL;  // Must do this before calling SetSortKey
    SetSortKey(puch);
}

CRecLookEl::~CRecLookEl() // Destructor
{
    delete m_puchSortKey;  // 1999-04-30 MRP
}

void CRecLookEl::SetSortKey(const UIdMChar* puch)
{
	ASSERT( puch );

    // Compute the length of the sort key
    const UIdMChar* puchFrom = puch;
    while ( *puchFrom )
        puchFrom++;
    int len = puchFrom - puch;

    // Delete the previous sort key (if there was one)
    delete m_puchSortKey;  // Must have been set to NULL in the constructor

    // Allocate a new buffer for the sort key
    m_puchSortKey = new UIdMChar[len + 1];

    // Copy the sort key into the buffer
    UIdMChar* puchTo = m_puchSortKey;
    puchFrom = puch;
    while ( *puchFrom )
        *puchTo++ = *puchFrom++;
    *puchTo = 0;  // Sort keys must be zero-terminated
}

CRecord* CRecLookEl::prec() const // Get content
{
    return m_prec;
}

const CField* CRecLookEl::pfldFirstInSubRecord(const CMarker* pmkr) const
{
    return m_prec->pfldFirstInSubRecord(pmkr, m_pfldPriKey);
}

void CRecLookEl::DeleteRecord() // Delete content record
    {
    delete m_prec;
    m_prec = NULL;
    m_pfldPriKey = NULL;  // 1998-01-26 MRP: This was a dangling pointer
    }
        
#ifdef _DEBUG
void CRecLookEl::AssertValid() const
    {
    m_prec->AssertValid(); // Assert validity of element
    }
#endif      

