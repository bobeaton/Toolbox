// cdblel.h CDblListEl Double linked list element base class definitions Alan Buseman 26 Jan 95
// This class is the list element for the CDblListEl class. See cdbllist.h for more information.

// Change History
// 26 Jan 95 0.1.01 AB First draft, based on design developed with Mark P, and Rod E.

#ifndef CDblListEll_H
#define CDblListEll_H

class CDblListEl {	// Doubly linked list element base class
friend class CDblList;
private:
	CDblListEl* m_pelPrev;	// Prev list element
	CDblListEl* m_pelNext;	// Next list element
public:
	CDblListEl() { m_pelPrev = m_pelNext = NULL; }	// Constructor
	virtual ~CDblListEl() {}	// Destructor
	
protected:
	// The following members are made public in derived class CRecLook
	// for preserving order stability of otherwise equal record elements	
	// when sorting an index.
	long lNum() const { return (long)m_pelPrev; };
	void SetNum(long lNum);
	void ClearNum();
			
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif
}; // class CDblListEl

#endif // CDblListEll_H
