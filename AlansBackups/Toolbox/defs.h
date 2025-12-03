// defs.h General definitions header // 1.5.1na 

#ifndef	DEFS_H
#define DEFS_H

#ifndef _WINDOWS // 1.5.1na If not MFC, do a different assert
#undef ASSERT
#define ASSERT(f) ((void)0)

typedef int BOOL;  // 1.5.1na If not MFC, define constants
#define FALSE   0
#define TRUE    1

#else // _WINDOWS
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions (common and tabbed dialogs)
// #define NULL 0
typedef unsigned short ID;  // Hungarian: id
	// Identifier of a Windows resource

#endif // _WINDOWS

typedef unsigned int Length;  // Hungarian: len
	// Length of a string

typedef unsigned long NumberOf;  // Hungarian: num
	// Count of the number of some kind of objects.

typedef unsigned short IDString;  // Hungarian: ids
    // A resource identifier (also known as a symbol) consists of a name
    // mapped to an integer value. It provides a descriptive way of
    // referring to a resource in source code.
    // More details on string resources appear at the end of this file.
    
typedef int ItemIndex;  // Hungarian: itm
	// Index of a combo or list box item.

#define DLGBUFMAX 1000 // 1.4dw
#define BIGBUFMAX 100000 // 1.4kh Make big buffer for use where needed
extern char* pszBigBuffer; // 1.4kh
extern unsigned short* pswzBigBuffer; // 1.4qpt Make unsigned short pointer to buffer // 1.4rab

#endif  // DEFS_H
