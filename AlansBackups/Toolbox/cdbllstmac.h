// cdbllstmac.h CDblLst Double linked list base class definitions Alan Buseman 21 Oct 06

#ifndef CDblLstMac_H
#define CDblLstMac_H

class CDblLstEl {	// Doubly linked list element base class
friend class CDblLst;
public:
	CDblLstEl* pelPrev;	// Prev list element
	CDblLstEl* pelNext;	// Next list element
public:
	CDblLstEl() { pelPrev = pelNext = NULL; }	// Constructor
	virtual ~CDblLstEl() {}	// Destructor
}; // class CDblLstEl

class CDblLst { // Doubly linked list base class
public:
    CDblLstEl* pelFirst; // First list element
    CDblLstEl* pelLast;  // Last list element
public:
    CDblLst() { pelFirst = pelLast = NULL; }
    ~CDblLst() { DeleteAll(); }
    void Add( CDblLstEl* pelNew, CDblLstEl* pelInsertAfter = NULL, BOOL bBefore = FALSE ); // Insert new at bottom
    void Remove( CDblLstEl* pel ); // Remove element from list
    void Delete( CDblLstEl* pel ); // Remove and delete
    void DeleteAll(); // Delete all elements
    long lGetCount() const; // Number of elements
}; // class CDblLst

#endif // CDblLstMac_H
