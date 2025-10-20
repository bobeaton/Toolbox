// cdbllstmac.cpp CDblLst Double linked list class implementation Alan Buseman 21 Oct 06

// #include "stdafx.h" // 1.5.0gn Can compile without this
#include "generic.h"
#include "cdbllstmac.h"

#ifdef AFX_DATA
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif // AFX_DATA

// Add to list, default to end
// If pelInsertAfter is given, insert after it
// If bBefore, insert before it, or at top if it is null
void CDblLst::Add( CDblLstEl* pelNew, CDblLstEl* pelInsertAfter, BOOL bBefore )
	{
    pelNew->pelPrev = pelNew->pelNext = NULL; // Start next & prev of new element as null
	if ( !pelLast ) // If list is empty, new becomes first and last
		{
        pelFirst = pelLast = pelNew; // Set first and last to new
		return;
		}
	if ( !pelInsertAfter ) // If no after given, insert after last or before first
		{
		if ( bBefore ) // If before, insert before first
			pelInsertAfter = pelFirst;
		else // Else (not before), insert after last
			pelInsertAfter = pelLast;
		}
	if ( bBefore ) // Insert before
		{ // To insert, four links must be changed
		pelNew->pelPrev = pelInsertAfter->pelPrev; // New points back to cur->prev
		pelNew->pelNext = pelInsertAfter; // New points fwd to cur
		pelInsertAfter->pelPrev = pelNew; // Cur points back to new
		if ( pelNew->pelPrev ) // If not top of list, new->prev points fwd to new
			pelNew->pelPrev->pelNext = pelNew;
		else // If top of list, new is now first
			pelFirst = pelNew;    
		}
	else // Insert after
		{ // To insert, four links must be changed
		pelNew->pelPrev = pelInsertAfter; // New points back to cur
		pelNew->pelNext = pelInsertAfter->pelNext; // New points fwd to cur->next
		pelInsertAfter->pelNext = pelNew; // Cur points fwd to new
		if ( pelNew->pelNext ) // If not end of list, new->next points back to new
			pelNew->pelNext->pelPrev = pelNew;
		else // If end of list, new is now last
			pelLast = pelNew; 
		}
	}

void CDblLst::Remove( CDblLstEl* pel ) // Remove element from list
	{
    CDblLstEl* pelNextOld = pel->pelNext; // Temp for next
    CDblLstEl* pelPrevOld =  pel->pelPrev; // Temp for prev
    if ( pelNextOld ) // If next exists, point it at prev
        pelNextOld->pelPrev = pel->pelPrev;
    else // Else (no next) set last to prev
        pelLast = pel->pelPrev;    
    if ( pelPrevOld ) // If prev exists, point it at next
        pelPrevOld->pelNext = pel->pelNext;   
    else // Else (no prev) set first to next
        pelFirst = pel->pelNext;
    pel->pelNext = pel->pelPrev = NULL; // Clear pointers
	}

void CDblLst::Delete( CDblLstEl* pel ) // Remove and delete
	{
	Remove( pel ); // Remove from list
	delete pel; // Delete
	}

void CDblLst::DeleteAll() // Delete all elements
	{
	while ( pelFirst ) // While there is a first element, delete it
		Delete( pelFirst );
	}

long CDblLst::lGetCount() const // Number of elements
	{
    long lCount = 0;
    for ( CDblLstEl* pel = pelFirst; pel; pel = pel->pelNext )
        lCount++;
    return lCount;  
	}
