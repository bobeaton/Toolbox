// ref.cpp  Implementation of object reference counting (1995-05-01)


#include "stdafx.h"
#include "toolbox.h"
#include "ref.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CRefCount::~CRefCount()
{
	ASSERT( m_numRefs == 0 );
}


void CRefCount::DecrementNumRefs()
{
	ASSERT( m_numRefs != 0 );
	m_numRefs -= 1;
}
