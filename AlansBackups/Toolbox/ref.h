// ref.h  Interface for object reference counting (1995-05-01)

// An object reference count is the total number of objects referring to
// the object which contains it as member data. Updating the count is the
// responsibility of every class which can refer to a "counting" class,
// and must be done whenever such a reference is added or removed.

// Recall that there are two kinds of associations between objects:
// _owning_ (e.g. a marker set owns its markers),
// and _referencing_ (e.g. zero or more marked strings refer to a marker).
// The unique owning collection object, which has the right and duty
// to delete its elements, is not to be counted as a reference.

// NOTE: Entities which can be deleted by the user require a reference count.
// If it is not zero, the object cannot simply be deleted, since that would
// leave dangling pointers to it. Shw must either disable the delete action,
// or in some way cause the references to be removed.

// NOTE: Shw must delete global collections in an order which causes counted
// references to be removed (e.g. close databases before marker sets, so that
// Standard Format fields, which refer to markers, are deleted first.)


#ifndef REF_H
#define REF_H


typedef unsigned long NumRefs;
typedef unsigned long NumberOfRefs;  // Hungarian: num

class CRefCount  // Hungarian: ref
{
private:
	NumRefs m_numRefs;
	
public:
	CRefCount() { m_numRefs = 0; }
	~CRefCount();
	
	NumRefs numRefs() const { return m_numRefs; }
	BOOL bHasRefs() const { return (m_numRefs != 0); }
	
	void IncrementNumberOfRefs() { m_numRefs += 1; }
	void DecrementNumberOfRefs() { DecrementNumRefs(); }

	void IncrementNumRefs() { m_numRefs += 1; }
	void DecrementNumRefs();
};  // class CRef


// For examples of using reference counts, see:
// 1. class CMarker (mkr.h, mkr.cpp)
// 2. class CFilter (fil.h, fil.cpp)

// To add reference counting to a class, insert the following fragments:
/*
private:
	CRefCount m_ref;  // number of object instances referring to this
	
public:
	NumRefs numRefs() const { return m_ref.numRefs(); }
	BOOL bHasRefs() const { return m_ref.bHasRefs(); }
	
	void IncrementNumRefs() { m_ref.IncrementNumRefs(); }
	void DecrementNumRefs() { m_ref.DecrementNumRefs(); }
*/

#endif  // REF_H