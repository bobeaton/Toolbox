// cdbllist.cpp CDblList Double linked list class implementation Alan Buseman 25 Jan 95

// #include "stdafx.h" // 1.5.1na 
#include "all.h" // 1.5.1na 
// #include "toolbox.h" // 1.5.1na 
// #include "cdbllist.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

    // Insert new after current 
void CDblList::InsertAfter( CDblListEl* pelCur, CDblListEl* pelNew )
{
    ASSERT( pelCur );
    ASSERT( pelNew ); 
    ASSERT( !pelNew->m_pelPrev ); // Don't link something that was already linked somewhere else
    ASSERT( !pelNew->m_pelNext ); // Don't link something that was already linked somewhere else
    // To insert, four links must be changed
    pelNew->m_pelPrev = pelCur; // New points back to cur
    pelNew->m_pelNext = pelCur->m_pelNext; // New points fwd to cur->next
    pelCur->m_pelNext = pelNew; // Cur points fwd to new
    if ( pelNew->m_pelNext ) // If not end of list, new->next points back to new
        pelNew->m_pelNext->m_pelPrev = pelNew;
    else // If end of list, new is now last
        m_pelLast = pelNew; 
}       
    
    // Insert new before current, or at top if current is NULL
void CDblList::InsertBefore( CDblListEl* pelCur, CDblListEl* pelNew )
{
    ASSERT( pelCur);
    ASSERT( pelNew );
    ASSERT( !pelNew->m_pelPrev ); // Don't link something that was already linked somewhere else
    ASSERT( !pelNew->m_pelNext ); // Don't link something that was already linked somewhere else
    // To insert, four links must be changed
    pelNew->m_pelPrev = pelCur->m_pelPrev; // New points back to cur->prev
    pelNew->m_pelNext = pelCur; // New points fwd to cur
    pelCur->m_pelPrev = pelNew; // Cur points back to new
    if ( pelNew->m_pelPrev ) // If not top of list, new->prev points fwd to new
        pelNew->m_pelPrev->m_pelNext = pelNew;
    else // If top of list, new is now first
        m_pelFirst = pelNew;    
}       
    
void CDblList::Add( CDblListEl* pelNew ) // Insert new at bottom
{
    ASSERT( pelNew );
    ASSERT( !pelNew->m_pelPrev ); // Don't link something that was already linked somewhere else
    ASSERT( !pelNew->m_pelNext ); // Don't link something that was already linked somewhere else
        
    if ( bIsEmpty() ) // If list is empty, new becomes first and last
        {
        m_pelFirst = m_pelLast = pelNew; // Set first and last to new
        pelNew->m_pelPrev = pelNew->m_pelNext = NULL; // Make sure next & prev are null
        }
    else // Else (list not empty) insert after last
        InsertAfter( pelLast(), pelNew ); // Use InsertAfter code, only minor speed penalty
}
    
    // Delete element, pel is changed to Next if it exists, else Prev if it exists, else NULL
void CDblList::Delete( CDblListEl** ppel )
{
    delete pelRemove( ppel ); // Remove and free element
}
        
    // Helper function for Delete, Remove element, pel is changed to Next if it exists, else Prev if it exists, else NULL
    // I originally had this code in Delete, but doing so prevented the derived class
    //  destructor from being called.
CDblListEl* CDblList::pelRemove( CDblListEl** ppel )
{
    ASSERT( ppel );
    CDblListEl* pelCur = *ppel;
    ASSERT( pelCur );
    CDblListEl* pelNext = pelCur->m_pelNext; // Temp for next
    CDblListEl* pelPrev =  pelCur->m_pelPrev; // Temp for prev
        
        // Adjust passed in pel pointer     
    if ( pelNext ) // If next, move pel to next
        *ppel = pelNext;
    else // Else (no next), move pel to prev (or NULL if no prev)
        *ppel = pelPrev;

    return pelRemove( pelCur );
}
        
    // Helper function for Delete, Remove element
    // I originally had this code in Delete, but doing so prevented the derived class
    //  destructor from being called.
CDblListEl* CDblList::pelRemove( CDblListEl* pel )
{
    CDblListEl* pelCur = pel;
    ASSERT( pelCur );
    CDblListEl* pelNext = pelCur->m_pelNext; // Temp for next
    CDblListEl* pelPrev =  pelCur->m_pelPrev; // Temp for prev
        
    if ( pelNext ) // If next exists, point it at prev
        pelNext->m_pelPrev = pelPrev;
    else // Else (no next) set last to prev
        m_pelLast = pelPrev;    
            
    if ( pelPrev ) // If prev exists, point it at next
        pelPrev->m_pelNext = pelNext;   
    else // Else (no prev) set first to next
        m_pelFirst = pelNext;
            
    pelCur->m_pelNext = pelCur->m_pelPrev = NULL; // Clear pointers to make eligible
        
    return pelCur; // Return removed element for deletion in derived class  
}
        
    // Delete element, return next pel, NULL if no more
CDblListEl* CDblList::pelDelete( CDblListEl* pel )
{
	CDblListEl* pelN = pelNext( pel ); // Remember next
	Delete( pel ); // Delete current
	return pelN; // Return next
}

    // Delete element without changing pel, needed for doubly derived classes
void CDblList::Delete( CDblListEl* pel )
{
    delete pelRemove( pel ); // Remove and free element
}
        
void CDblList::DeleteAll() // Delete all elements       
{
    for ( CDblListEl* pel = m_pelFirst; pel; ) // Start at first
        Delete( &pel ); // Delete cur and move pel to next
}

void CDblList::RemoveAll() // Remove all elements without deleting them      
{
    for ( CDblListEl* pel = m_pelFirst; pel; ) // Start at first
        pelRemove( &pel ); // Delete cur and move pel to next
}

long CDblList::lGetCount() const // Number of elements
{
    long lCount = 0;
    for ( CDblListEl* pel = pelFirst(); pel; pel = pelNext( pel ) )
        lCount++;
    return lCount;  
}

void CDblList::MoveElementsTo(CDblList* plst)
{
	if ( bIsEmpty() )  // If no elements to move
		return;
	
	// Remove all the elements from this list
	CDblListEl* pelF = m_pelFirst;
	ASSERT( !pelF->m_pelPrev );
	CDblListEl* pelL = m_pelLast;
	ASSERT( !pelL->m_pelNext );
	m_pelFirst = NULL;
	m_pelLast = NULL;
	
	// Add the first from this list to the end of plst
	ASSERT( plst );
	if ( plst->bIsEmpty() )
		{
		ASSERT( !plst->m_pelLast );
		plst->m_pelFirst = pelF;
		}
	else
		{
		CDblListEl* pel = plst->m_pelLast;
		ASSERT( pel );
		pel->m_pelNext = pelF;
		pelF->m_pelPrev = pel;
		}
		
	// Make the last from this list the last in plst
	plst->m_pelLast = pelL;
	pelL->m_pelNext = NULL;
}
    
#ifdef _DEBUG
void CDblList::AssertValid() const
{ // Check that all nexts & prevs point at each other
    for ( CDblListEl* pel = m_pelFirst; pel && pel->m_pelNext; pel = pel->m_pelNext )
        ASSERT( pel->m_pelNext->m_pelPrev == pel );
}
#endif	// _DEBUG
