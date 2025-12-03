// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GUESSEREC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GUESSEREC_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef GUESSEREC_EXPORTS
#define GUESSEREC_API __declspec(dllexport)
#else
#define GUESSEREC_API __declspec(dllimport)
#endif

extern "C"
{
	GUESSEREC_API int ResetCorpus( LPVOID lpThis );

    GUESSEREC_API int AddPairToCorpus
    (
        LPVOID  lpThis,         // name of the corpus to use (in case of multiple instances)
        LPCTSTR	lpszSrcString,  // source word to be added to your corpus for guessing
        LPCTSTR	lpszTgtString   // target word to be added to your corpus for guessing
    );

    GUESSEREC_API int MakeAGuess
    (
        LPVOID  lpThis,             // name of the corpus to use (in case of multiple instances)
	    LPCTSTR	lpszInputString,	// input (wide) data to be converted
	    LPTSTR	lpszOutputString,	// pointer to the output buffer to write result in 2-byte characters
	    int*	pnNumCharsOut		// (in) # of chars in the output buffer; (out) fill in with the number of chars in the result
    );
}
