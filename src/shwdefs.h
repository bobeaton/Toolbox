// shwdefs.h

#ifndef SHWDEFS_H
#define SHWDEFS_H

typedef unsigned int Length;  // Hungarian: len
	// Length of a string

typedef unsigned long NumberOf;  // Hungarian: num
	// Count of the number of some kind of objects.

typedef unsigned short ID;  // Hungarian: id
	// Identifier of a Windows resource
typedef unsigned short IDString;  // Hungarian: ids
    // A resource identifier (also known as a symbol) consists of a name
    // mapped to an integer value. It provides a descriptive way of
    // referring to a resource in source code.
    // More details on string resources appear at the end of this file.
    
typedef int ItemIndex;  // Hungarian: itm
	// Index of a combo or list box item.

inline BOOL bEqual(const char* pszA, const char* pszB)
   { return (strcmp(pszA, pszB) == 0); }
    
#define DLGBUFMAX 1000 // 1.4dw
#define BIGBUFMAX 100000 // 1.4kh Make big buffer for use where needed
extern char* pszBigBuffer; // 1.4kh
extern unsigned short* pswzBigBuffer; // 1.4qpt Make unsigned short pointer to buffer // 1.4rab

// #include "crecord.h" // 1.4gza Include some general things everywhere // 1.5.0gk Move to toolbox.h

#endif  // SHWDEFS_H