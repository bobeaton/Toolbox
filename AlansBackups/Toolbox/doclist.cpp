// Imlementation of Doclist.h

#include "stdafx.h"
#include "toolbox.h"
#include "Doclist.h"
#include "shw.h"
#include "shwdoc.h"

CDocList::CDocList() : // Constructor
	m_posTemplate(NULL),
	m_pTemplate(NULL),
	m_posDoc(NULL)
	{
	}

CShwDoc* CDocList::pdocFirst() // return first open document
	{
		m_posTemplate = Shw_papp()->GetFirstDocTemplatePosition();
		if ( !m_posTemplate ) // 1.2mf Fix bug of crash on jump from Paratext if no document open
			return NULL;
		m_pTemplate = Shw_papp()->GetNextDocTemplate(m_posTemplate);
		ASSERT(m_pTemplate);
		m_posDoc = m_pTemplate->GetFirstDocPosition();
		return pdocNext();
	}

CShwDoc* CDocList::pdocNext() // return next open document
	{
		ASSERT( m_pTemplate );
		if ( !m_posDoc )
			return NULL;
		CShwDoc* pdoc = (CShwDoc*)m_pTemplate->GetNextDoc(m_posDoc);
		ASSERT( pdoc->IsKindOf(RUNTIME_CLASS(CShwDoc)) );
		return pdoc;
	}

CTypedDocList::CTypedDocList(CDatabaseType* ptyp) : // Constructor
    m_ptyp(ptyp),
    m_pdocExclude(NULL)
	{
	}

CShwDoc* CTypedDocList::pdocFirst(CShwDoc* pdocExclude) // return first open document
	{
		m_pdocExclude = pdocExclude;

        return CDocList::pdocFirst();
	}

CShwDoc* CTypedDocList::pdocNext() // return next open document
	{
        CShwDoc* pdocNextDoc = CDocList::pdocNext();
        while ( pdocNextDoc &&
                (( pdocNextDoc->ptyp() != m_ptyp ) ||
                 ( pdocNextDoc == m_pdocExclude )))
            pdocNextDoc = CDocList::pdocNext();
		return pdocNextDoc;
	}




