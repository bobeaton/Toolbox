// CorGuess.h Header for Correspondence Guesser
/* 
Correspondence Guesser version 1.5.1cb May 2007
(c) SIL International
Developer - Alan Buseman
Not to be used, modified or distributed to others in any form 
without written permission from Alan Buseman. alan_buseman@sil.org

The guesser is under active development. Improved versions will be 
available from time to time.
*/

/* Correspondence Guesser takes a list of source/target pairs and tries to guess 
the corresponding target for a source word that is not in the list.
*/

/* Documentation of how to use the correspondence guesser:

Compile and link CorGuess.cpp into your project.
Include CorGuess.h in the files in which you use the guesser.

Interface consists of one class and 3 functions:
	Guesser gue;
	void gue.ClearAll(); // Clear old correspondences, prepare to start new load
	void gue.AddCorrespondence( const wxChar* pszSrc, const wxChar* pszTar ); // Add a correspondence to the list
	bool gue.bTargetGuess( const wxChar* pszSrc, wxChar** ppszTar ); // Return target guess

To use the correspondence guesser:
1. To load a knowledge base into the guesser, call AddCorrespondence once for each correspondence pair.
2. To ask for a guess, call bTargetGuess.
3. To add a new correspondence pair to an existing set of correspondences, call AddCorrespondence with it.
4. To clear all correspondences to prepare for a fresh load, call ClearAll.

Sample calling code:
	#include "CorGuess.h"
	
	Guesser gue;
	for ( ... ) // For each knowledge base pair
		gue.AddCorrespondence( pszSrc, pszTar ); // Add pair to list correspondence guesser's list

	if ( ... ) // If word not found in knowledge base
		{
		wxChar* pszGuess = (wxChar*)_alloca( MAX_GUESS_LENGTH ); // Alloc space to pass as pointer, 100 is enough
		if ( gue.bTargetGuess( pszWord, &pszGuess ) ) // If guesser does a guess, show it to user in place of the source word
			...; // Give user the guess instead of the source word for editing into target form, possibly with special color or font to let user know it is a guess
		}

There is no way to delete or modify a correspondence. If the user edits an existing target form, the
easiest is not to tell the guesser anything. The guesser is statistical and needs multiple examples
to form a correspondence, so missing a modification is unlikely to make any significant difference.
The next time the user starts the program or the guesser is reloaded, the guesser's correspondence 
list will be freshened.

*/

#ifndef __WXWINDOWS__ // If not wxWidgets, define back to generic char and string
#define wxChar char
#define wxStrlen strlen
#define wxStrcpy strcpy
#define wxStrcmp strcmp
#endif

#define MAX_GUESS_LENGTH 200 // Maximum length of guess, length of buffer for return of guess

class Corresp // cor Correspondence struct, one for each correspondence noted
	{
public:
	wxChar* pszSrc; // Source of correspondence
	wxChar* pszTar; // Target of correspondence
	int iNumInstances; // Number of instances found
	int iNumExceptions; // Number of exceptions found
	Corresp* pcorNext; // Pointer to next correspondence in linked list
	Corresp();
	Corresp( const wxChar* pszSrc1, const wxChar* pszTar1 );
	~Corresp();
	};

class CorrespList // corlst List of correspondence structures
	{
public:
	int iRequiredSuccessPercent; // Required percentage of successes
	Corresp* pcorFirst; // First correspondence in the full list
private:
	Corresp* pcorLast; // Last correspondence in the full list
	void Add( Corresp *pcorNew ); // Helper function for Add new correspondence
public:
	CorrespList();
	~CorrespList();
	void ClearAll(); // 1.4vyd 
	void Add( const wxChar* pszSrc, const wxChar* pszTar, bool bCount ); // Add a new correspondence to the end ofthe list, if bCount is true, don't add if already found
	bool bIsEmpty() { return pcorFirst == NULL; } // Return true if list is empty
	Corresp* pcorFind( const wxChar* pszSrc, const wxChar* pszTar ); // Find the same pair, return NULL if not found
	Corresp* pcorDelete( Corresp* pcor, Corresp* pcorPrev ); // Delete a correspondence from the list, return next after deleted one
	};

class Guesser // gue Guesser, main class that holds everything
{
protected: // Not to be changed by user, carefully tuned for maximum performance
	int iRequiredSuccessPercent; // Required percentage of successes
	int iMaxSuffLen; // Longest suffix length to be considered
	int iMinSuffExamples; // Minimum number of examples of suffix to be considered
protected:
	CorrespList corlst; // Raw correspondences given to guesser
	CorrespList corlstSuff; // Guessed suffixes
	CorrespList corlstRoot; // Guessed roots
	CorrespList corlstPref; // Guess prefixes
public:
	Guesser();
	void ClearAll(); // Clear all guesses
	void AddCorrespondence( const wxChar* pszSrc, const wxChar* pszTar ); // Add a correspondence to the list
	bool bTargetGuess( const wxChar* pszSrc, wxChar** ppszTar ); // Return target guess
};