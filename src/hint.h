// hint.h
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
	types on hints.  For instance, consider CHintRecord (hints.h).
*******************/



#ifndef _hint_H
#define _hint_H

class CHint : public CObject
{
	int  m_objectType;
	int  m_operation;

public:
	enum
	{
		Filter,
		Marker,
		Record
	};
	
	enum // Establish constant values for kinds of operation
	{
		New,
		Modified,
		Deleted
	};
	
public:
	CHint(const int iobjType, const int iop) : m_objectType(iobjType), m_operation(iop)
	 	{ }
	 	
	virtual ~CHint() {;}
	
	int iObjectType()	{ return m_objectType; }
	int iOperation()	{ return m_operation; }
	
};
#endif // _hint_H
