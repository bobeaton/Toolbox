// cdbllist.h CDblList Double linked list base class definitions Alan Buseman 25 Jan 95

// This list class is more memory efficient than a CObList because this one avoids
//  the extra heap overhead of a separate CNode structure for each object.
// This list class is also cleaner to use than a CObList because
//  this structure does not require a POSITION variable along with the pointer
//  to the data object. With this class, each list element has position information
//  included in it, so a pointer to a list element is all that is needed.

// Change History
// 26 Jan 95 0.1.01 AB First draft, based on design developed with Mark P, and Rod E.
// 10 May 95 0.1.02 MRP Make functions to be overriden in derived classes protected

#ifndef CDblList_H
#define CDblList_H

#include "cdblel.h"

class CDblList { // Doubly linked list base class
protected: // Delete in multiply derived class needs to manipulate these
    CDblListEl* m_pelFirst; // First list element
    CDblListEl* m_pelLast;  // Last list element
public:
    CDblList() // Constructor
        { m_pelFirst = m_pelLast = NULL; }
        
    ~CDblList() // Destructor
        { DeleteAll(); }
                                    
    CDblListEl* pelFirst() const // First element
        { return m_pelFirst; }
        
    CDblListEl* pelLast() const // Last element
        { return m_pelLast; }
    
    CDblListEl* pelNext( const CDblListEl* pelCur ) const // Next element after current
        { ASSERT( pelCur ); return pelCur->m_pelNext; }
        
    CDblListEl* pelPrev( const CDblListEl* pelCur ) const // Previous element after current
        { ASSERT( pelCur ); return pelCur->m_pelPrev; }
    
    BOOL bIsFirst( const CDblListEl* pelCur ) const // True if current element is first   
        { ASSERT( pelCur ); return pelCur == m_pelFirst; }
        
    BOOL bIsLast( const CDblListEl* pelCur ) const // True if current element is last
        { ASSERT( pelCur ); return pelCur == m_pelLast; }
        
    BOOL bIsMember( const CDblListEl* pelCur ) const
        {
        ASSERT( pelCur );
        for ( CDblListEl* pel = pelFirst(); pel; pel = pelNext(pel) )
            if ( pel == pelCur )
                return TRUE;  // the element is a member of this list
                
        return FALSE;
        }
    
    void DeleteAll();   // Delete all elements
    
    void RemoveAll(); // Remove all elements without deleting them      

    long lGetCount() const; // Number of elements

    BOOL bIsEmpty() const // True if no elements
        { return m_pelFirst == NULL; }

        // Insert new after current 
    void InsertAfter( CDblListEl* pelCur, CDblListEl* pelNew );
        // Insert new before current
    void InsertBefore( CDblListEl* pelCur, CDblListEl* pelNew );

    void Add( CDblListEl* pelNew ); // Insert new at bottom
    
        // Delete element without changing pel, needed for doubly derived classes
    void Delete( CDblListEl* pel );

protected:
        // Delete element, return next pel, NULL if no more
    CDblListEl* pelDelete( CDblListEl* pel );

        // Delete element, pel is changed to Next if it exists, else Prev if it exists, else NULL
    void Delete( CDblListEl** ppel );
    
          // Remove, same as Delete, but does not delete element
    CDblListEl* pelRemove( CDblListEl** ppel );

         // Remove without changing pel, needed for doubly derived classes
    CDblListEl* pelRemove( CDblListEl* pel );

public: 
	void MoveElementsTo(CDblList* plst);
		// Empty this list by moving all its elements to the end of plst.

#ifdef _DEBUG
    void AssertValid() const;
#endif      
}; // class CDblList

/* Discussion             
Hungarian notation used in this file:
    m_ is Microsoft standard for member variable
    i is for integer
    p is for pointer
    lis is for CDblList
    el is for CDblListEl
    
cdbllist.cpp contains sample code with further discussion.
*/

#endif // CDblList_H
