// cdblel.cpp CDblListEl Double linked list element class implementation Alan Buseman 25 Jan 95

#include "stdafx.h" // 1.5.1na 
#include "all.h" // 1.5.1na 
// #include "toolbox.h" // 1.5.1na 
// #include "cdblel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void CDblListEl::SetNum(long lNum)
{
	ASSERT( !m_pelPrev );
	m_pelPrev = (CDblListEl*)lNum;
	ASSERT( !m_pelNext );
}

void CDblListEl::ClearNum()
{
	m_pelPrev = (CDblListEl*)0L;
	ASSERT( !m_pelNext );
}

#ifdef _DEBUG // 1.5.1na 
	void CDblListEl::AssertValid() const
		{}
#endif		
