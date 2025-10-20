// setsel.h  Interface for sets and set elements (1995-06-19)

// Abstract base class from which to derive set and element classes
// for "settings" data entities (e.g. filters, markers; but not records).
// Factor out into this class common characteristics which have similar
// visual presentation and manipulative behavior in the user interface.

// See data entity classes CFilter and CFilterSet (fil.h).
// See interface base clases CSetComboBox (cbo.h) and CSetListBox (lbo.h).

// Change history:
// 1995-06-19 0.15  MRP: Initial design sketch


#ifndef SETSEL_H
#define SETSEL_H

#include "clook.h"
#include "ref.h"


// **************************************************************************

class CSetEl : public CLookEl	// Hungarian: sel
{
private:
	CRefCount m_ref;  // number of other entities referring to this
 
protected:
	CSetEl(const char* pszKey) : CLookEl(pszKey) {}  // constructor

public:
	virtual ~CSetEl() {}  // destructor
	
	const Str8& sName() const { return sKey(); }  // the key is the name

	void ChangeNameTo(const char* pszNewName)
	{
		SetKey(pszNewName);
		// Independent objects can be renamed freely.
		// Elements of a set must also be reinserted in their proper position.
	}
	
	NumRefs numRefs() const { return m_ref.numRefs(); }
	BOOL bHasRefs() const { return m_ref.bHasRefs(); }
	void IncrementNumberOfRefs() { m_ref.IncrementNumberOfRefs(); }
	void DecrementNumberOfRefs() { m_ref.DecrementNumberOfRefs(); }
	void IncrementNumRefs() { m_ref.IncrementNumRefs(); }
	void DecrementNumRefs() { m_ref.DecrementNumRefs(); }

	virtual BOOL bCopy(CSetEl** ppselNew) = 0;
	virtual BOOL bModify() = 0;
		// Let the user view and possibly modify this element's properties.
		// Returns whether the user clicked OK (vs. Cancel).
		// TRUE (OK): This element may have been modified (if it was,
		//     an update hint should have been broadcast to all views).
		// FALSE (Cancel): This element definitely was not modified.

	virtual BOOL bDeletable() { return !bHasRefs(); }
		// Return whether this element may be deleted,
		// i.e. when it is selected, should the Delete button be enabled?
		// By default, it cannot be if it has references to it.
};  // class CSetEl


// --------------------------------------------------------------------------

class CSet : public CLook  // Hungarian: set
{
protected:
	CSet() {}  // constructor

public:
	virtual ~CSet() {}  // destructor

	CSetEl* pselFirst() const
			{ return (CSetEl*)pelFirst(); }
		// First element of the set; otherwise NULL, if the set is empty.
	CSetEl* pselNext(const CSetEl* pselCur) const
			{ return (CSetEl*)pelNext(pselCur); }
		// Next element after current; otherwise NULL, if it is the last.

	virtual void Delete(CSetEl** ppsel)
			{ CLook::Delete((CLookEl**)ppsel); }
		// Delete the element; ppsel is set to the next after the one deleted;
		//     otherwise the previous (i.e. deleted the last element);
		//     otherwise NULL (i.e. deleted the only element).

	Str8 sUniqueName(const char* pszBaseName)
		{
		ASSERT( pszBaseName );
		Str8 sName = pszBaseName;
		int i = 1;
		while ( pelSearch(sName) )  // An element already has this name
			sName.Format("%s%d", pszBaseName, ++i);  // Base2, Base3, etc.
			
		return sName;
		}

	virtual BOOL bAddElement(CSetEl** ppselNew) = 0;
		// Let the user add a new element to this set.
		// (Show the same dialog box used to view an existing element.)
		// Returns whether the user clicked OK (vs. Cancel).
		// TRUE (OK): ppselNew returns the new element (an update hint
		//     should have been broadcast to all views).
		// FALSE (Cancel): No element was added to this set.
};  // class CSet

// **************************************************************************

#endif  // SETSEL_H
