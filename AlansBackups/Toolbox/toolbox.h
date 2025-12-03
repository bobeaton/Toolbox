// toolbox.h General Toolbox header // 1.5.0gh 

#ifndef	TOOLBOX_H
#define TOOLBOX_H

// #define FileStream // 1.6.1cb Set up define for activating new file stream  // 1.6.4aa Never define FileStream

#ifdef FileStream // 1.6.1cb  // 1.6.4aa Never define FileStream
#define Object_ostream Object_ofstream // 1.6.1cb  // 1.6.4aa Never do the full redefine of stream
#define Object_istream Object_ifstream // 1.6.1cm  // 1.6.4aa Never do the full redefine of stream
#endif // 1.6.1cb  // 1.6.4aa Never define FileStream

#define Activateofstream  // 1.6.4ad 
#ifdef Activateofstream // 1.6.4ad 
#define prjWritefstream // 1.6.4aa Set up to test prj file // 1.6.4aa Test prj ofstream
#define lngWritefstream // 1.6.4ab Set up to test prj file // 1.6.4aa Test prj ofstream
#define typWritefstream // 1.6.4ac Set up to test prj file // 1.6.4aa Test prj ofstream
#define ostrWritefstream // 1.6.4ad
#endif

#ifdef Samples // for copy and paste

#ifdef ostrWritefstream // 1.6.4ad
#else
#endif

#ifdef typWritefstream // 1.6.4ac
#else
#endif

#ifndef typWritefstream // 1.6.4ac
#endif

#ifdef lngWritefstream // 1.6.4ab 
#else
#endif

#ifndef lngWritefstream // 1.6.4ab 
#endif

#ifdef prjWritefstream // 1.6.4aa 
#else
#endif

#ifndef prjWritefstream // 1.6.4aa 
#endif

#endif //Samples

#include "generic.h"
#include "Str8.h"
#include "shwdefs.h"
#include "portable.h"
#include "crecord.h" // 1.4gza Include some general things everywhere

#define _(x) (x) // 1.3ac Prepare to put string resources in as translatable strings

#define new DEBUG_NEW

extern const char* pNL; // system dependent newline string

#ifndef _DEBUG
#undef ASSERT // Use our own ASSERT for release versions, reports the line number of the problem
#define ASSERT(f) ((f) ? (void)0 : ::MyAssertFailedLine(__FILE__, __LINE__))
void MyAssertFailedLine(LPCSTR lpszFileName, int nLine);
#endif // not _DEBUG

#endif  // TOOLBOX_H
