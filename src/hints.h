// hints.h
// RNE 1995-04=26

/***************
	We would like to have a generalized set of hint classes that
	are all derived from a base class
	
	One problem is that the objects we pass as hints have no common
	base class.  Therefore I have to use void* to refer to the objects.
	
	However, we can use a member variable to distinguish the types of
	objects that are passed as hint.  Another member variable describes
	the operation that performed on that object.
	
	From the CHint base class are derived hint classes that deal with specific
	types on hints.  For instance, consider CRecordHint.
	
	How might the hints be used: consider the following OnUpdate function:
	
	ProcessHint() -- a member of the view class (or one of its base classes).
	ProcessHint() in this example is polymorphic: different ProcessHints
	are called according to the type of hint.  ProcessHint has a switch
	statement according to the operations being performed.
	
	
CSomeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
...
	switch (pHint->iObjectType()
	{
		case CHint::Record :
			ProcessHint((CRecordHint*)pHint);
			break;

		case CHint::Filter :
			ProcessHint((CFilterHint*)pHint);
			break;

		case CHint::Marker :
			ProcessHint((CMarkerHint*)pHint);
			break;

	}

...
}

*******************/



#ifndef HINTS_H
#define HINTS_H

#include "hint.h"  // 1995-06-27 MRP: had to separate out the base class
#include "crecpos.h"
class CRecord;

class CRecordHint : public CHint
{
	const CRecord* m_prec; // Pointer to modified record
	CRecPos m_rps;
	int m_iRedraw;

public:
	enum // Establish constant values for kinds of operation
	{
		Validated = CHint::Deleted + 1
	};

	CRecordHint(const CRecord* prec, int iOp) : CHint(CHint::Record, iOp)
		{
		m_prec = prec;
		ASSERT( m_prec );
		}
		
	enum // type of redraw that should happen
	{
		eAll, eLine, eLineErase
	};

	CRecordHint(const CRecPos& rps, int iRedraw) : CHint(CHint::Record, Modified)
		{
			m_iRedraw = iRedraw;
			m_rps = rps;
			m_prec = rps.prec;
		}
		
	virtual ~CRecordHint()	{;}
	
	const CRecord* prec() const { return m_prec; }
	const CRecPos& rps() const { return m_rps; }
	int iRedraw() const { return m_iRedraw; }
};

#endif // HINTS_H

