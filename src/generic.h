// generic.h General header // 1.5.0gn 

#ifndef	GENERIC_H
#define GENERIC_H

#ifndef AFX_DATA // 1.5.0gn If not MFC, do a different assert
#undef ASSERT
#define ASSERT(f) ((void)0)

typedef int BOOL;  // 1.5.0gn If not MFC, define constants
#define FALSE   0
#define TRUE    1
#define NULL    0

#endif // AFX_DATA


#endif  // GENERIC_H
