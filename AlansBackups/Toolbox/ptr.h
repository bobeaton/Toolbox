// ptr.h  Pointer classes  BJY 2/16/96

#ifndef PTR_H
#define PTR_H

// --------------------------------------------------------------------------

class CDatabaseType; // typ.h

class CDatabaseTypePtr  // Hungarian: ptyp
{
private:
	CDatabaseType* m_ptyp;
	
	void IncrementNumberOfRefs(CDatabaseType* ptyp);
	void DecrementNumberOfRefs();
	
public:
	CDatabaseTypePtr(CDatabaseType* ptyp = NULL)
		{ IncrementNumberOfRefs(ptyp); }
	CDatabaseTypePtr(const CDatabaseTypePtr& ptyp)  // copy constructor
		{ IncrementNumberOfRefs(ptyp.m_ptyp); }
	
	// Decrement the reference count of the index to which
	// this currently refers and increment ptyp's reference count
	const CDatabaseTypePtr& operator=(CDatabaseType* ptyp);
	const CDatabaseTypePtr& operator=(const CDatabaseTypePtr& ptyp)
		{ return operator=(ptyp.m_ptyp); }

	~CDatabaseTypePtr() { DecrementNumberOfRefs(); }
	
	CDatabaseType* operator->() const { return m_ptyp; }
	operator CDatabaseType*() const { return m_ptyp; } 
};  // class CDatabaseTypePtr

// --------------------------------------------------------------------------
class CDatabaseTypeRef // Hungarian trf, Database type reference, written for CDbTrie, loads pointer automatically, based on global type set
{
private:
	Str8 m_sType; // Marker string for deferred loading
	CDatabaseTypePtr m_ptyp; // Pointer to actual type
public:
	CDatabaseTypeRef( const char* psz = "" ); // Constructor
	CDatabaseType* ptyp(); // Return the type pointer, looking up the name if necessary
	void SetType( const char* psz ); // Set the type name and clear the pointer
	Str8 sType() // Return the name of the type
		{ return m_sType; }
	void ClearPtr() // Clear the pointer, for cleanup before deletion of database types
		{ m_ptyp = NULL; }
}; // class CDatabaseTypeRef

// --------------------------------------------------------------------------

class CIndex; // ind.h

class CIndexPtr  // Hungarian: pind
{
private:
	CIndex* m_pind;
	
	void IncrementNumberOfRefs(CIndex* pind);
	void DecrementNumberOfRefs();
	
public:
	CIndexPtr(CIndex* pind=NULL)	// RNE   Added to make this a default constructor.
		{ IncrementNumberOfRefs(pind); }
	CIndexPtr(const CIndexPtr& pind)  // copy constructor
		{ IncrementNumberOfRefs(pind.m_pind); }
	
	// Decrement the reference count of the index to which
	// this currently refers and increment pind's reference count
	const CIndexPtr& operator=(CIndex* pind);
	const CIndexPtr& operator=(const CIndexPtr& pind)
		{ return operator=(pind.m_pind); }

	~CIndexPtr() { DecrementNumberOfRefs(); }
	
	CIndex* operator->() const { return m_pind; }

	// RNE
	operator CIndex*() const { return m_pind; } 
};  // class CIndexPtr


// --------------------------------------------------------------------------

// Class CKeyboardPtr maintains the count of references to the keyboard
// it points to. Use it instead of a CKeyboard* in any class (e.g. CLangEnc)
// which refers to a keyboard.

class CKeyboard; // kbd.h

class CKeyboardPtr  // Hungarian: pkbd
{
private:
	CKeyboard* m_pkbd;
	
	void IncrementNumberOfRefs(CKeyboard* pkbd);
	void DecrementNumberOfRefs();
	
public:
	CKeyboardPtr(CKeyboard* pkbd = NULL)
		{ IncrementNumberOfRefs(pkbd); }
	CKeyboardPtr(const CKeyboardPtr& pkbd)  // copy constructor
		{ IncrementNumberOfRefs(pkbd.m_pkbd); }
	
	// Decrement the reference count of the index to which
	// this currently refers and increment pkbd's reference count
	const CKeyboardPtr& operator=(CKeyboard* pkbd);
	const CKeyboardPtr& operator=(const CKeyboardPtr& pkbd)
		{ return operator=(pkbd.m_pkbd); }

	~CKeyboardPtr() { DecrementNumberOfRefs(); }
	
	CKeyboard* operator->() const { return m_pkbd; }
	operator CKeyboard*() const { return m_pkbd; } 
};  // class CKeyboardPtr


#endif // PTR_H