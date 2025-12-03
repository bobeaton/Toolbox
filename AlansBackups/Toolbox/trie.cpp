// trie.cpp CTrie structure Alan Buseman 12 Dec 95

#include "stdafx.h"
#include "toolbox.h"
#include "shwdoc.h" // For AddAll
#include "recset.h" // For AddAll
#include "intprc_d.h"

#include "trie.h"
#include "progress.h"
#include "obstream.h"  // Object_istream, Object_ostream

//------------------------------------------------------------------------------------------------------------
CTrieOut::CTrieOut( CRecPos rps, int iLen ) // Constructor
{
    prec = rps.prec;
    pfld = rps.pfld;
    iChar = rps.iChar;
    m_iLen = iLen;
    m_ptotNext = NULL;
};

CTrieOut::~CTrieOut() // Destructor
{
    // BJY 4-16-96 The 16-bit optimizing compiler generated a nasty bug
    // with the code below. Now using s_DeleteAll instead.
#if 0
    if ( m_ptotNext ) // If another in list, delete it. This calls its destructor and recursively deletes all members.
        delete m_ptotNext;
#endif
}

void CTrieOut::s_DeleteAll( CTrieOut* ptot ) // delete list starting at ptot
{
    while ( ptot ) // walk down the list, freeing as we go
        {
        CTrieOut* ptotN = ptot->ptotNext();
        delete ptot;
        ptot = ptotN;
        }
}

void CTrieOut::DeleteRest() // Delete rest of list
{
    s_DeleteAll( ptotNext() );
    m_ptotNext = NULL; // Clear next
}

CTrieOut* CTrieOut::ptotAdd( CRecPos rps, int iLen ) // Add result to list, longest first
{
    CTrieOut* ptotNew = new CTrieOut( rps, iLen ); // Make new trie result
    // If new goes at the top of the list, put it there and return it
    if ( !this || iLen >= m_iLen ) // If empty list, or new longer than top, put new at top
        {
        ptotNew->SetNext( this ); // Link to old list top
        return ptotNew; // Return new as top
        }
    // Else find the right place for it and insert it
    CTrieOut* ptot = this; // Get top of list
    while ( ptot->ptotNext() && ptot->ptotNext()->iLen() > ptotNew->iLen() ) // While next longer than new, move down list
        ptot = ptot->ptotNext(); // Move to next
    ptotNew->SetNext( ptot->ptotNext() ); // Link new to next
    ptot->SetNext( ptotNew ); // Link old to new
    return this; // Return same top of list as before
}

CTrieOut* CTrieOut::ptotDelete() // Delete element, return next
{
    CTrieOut* ptot = ptotNext(); // Remember next
    m_ptotNext = NULL; // Clear next to prevent deletion of others
    delete this; // Delete this one
    return ptot; // Return next
}

#ifdef _DEBUG
void CTrieOut::AssertValid() const
{
    ASSERT( m_iLen >= 0 );
}
#endif

//------------------------------------------------------------------------------------------------------------
// CTrieListEl member functions
CTrieListEl::CTrieListEl( const char* pszRest, CRecPos rps )    // Constructor
{
    ASSERT( pszRest );
    m_pszRest = strdup( pszRest ); // Make a copy of the string ??AB Consider equivalent that would use new
    m_rps = rps; // Store record info
    m_ptleNext = 0;
}

CTrieListEl::~CTrieListEl()                             // Destructor
{
    ASSERT( m_pszRest );
    free( m_pszRest ); // Free string space from strdup in constructor
}

void CTrieListEl::SetNext( CTrieListEl* ptle ) // Set next in list  
{
    m_ptleNext = ptle; // Set member variable
}

#ifdef _DEBUG
void CTrieListEl::AssertValid() const   // Assert valid routine
    {
    ASSERT( m_pszRest );
    }
#endif      

//------------------------------------------------------------------------------------------------------------
// CTrieList member functions
CTrieList::CTrieList()  // Constructor
{
    m_ptle = NULL;
    m_iListLen = 0;
}

CTrieList::CTrieList( const char* pszRest, CRecPos rps )    // Constructor
{
    m_ptle = new CTrieListEl( pszRest, rps );
    m_iListLen = 1;
}

CTrieList::~CTrieList()                             // Destructor, delete all elements
{
    while ( m_ptle ) // While there is an element, delete it
        {
        CTrieListEl* ptle = m_ptle; // Remember current to delete
        m_ptle = m_ptle->ptleNext(); // Get pointer to next before losing it
        delete ptle; // Delete current one
        } 
}

// CTrieList Add puts the shortest first so that the output will be built with the longest at the top
void CTrieList::Add( const char* pszRest, CRecPos rps, int iType ) // Add a new info to a list
{
    ASSERT( pszRest );
    unsigned int iLen = strlen( pszRest ); // Get length of rest string
    CTrieListEl* ptleNew = new CTrieListEl( pszRest, rps ); // Make new list node
    // If new goes at the top of the list, put it there and return it
    if ( !m_ptle || iLen <= strlen( m_ptle->pszRest() ) ) // If empty list, or new shorter than top, put new at top
        {
        ptleNew->SetNext( m_ptle ); // Link to old list top
        m_ptle = ptleNew; // Make new the top of the list
        }
    else // Else find the right place for it and insert it
        {
        CTrieListEl* ptle = m_ptle; // Get top of list
        while ( ptle->ptleNext()
                && iLen > strlen( ptle->ptleNext()->pszRest() ) ) // While next longer than new, move down list
            ptle = ptle->ptleNext(); // Move to next
        ptleNew->SetNext( ptle->ptleNext() ); // Link new to next
        ptle->SetNext( ptleNew ); // Link old to new
        }
    m_iListLen++; // Note increase in length    
}

CTrieListEl* CTrieList::ptleDelete( CTrieListEl* ptleDel ) // Delete an element, return next
{
    ASSERT( ptleDel );
    ASSERT( m_ptle ); // Shouldn't try to delete an element if none in list
    m_iListLen--; // Note decrease in length    
    if ( ptleDel == m_ptle ) // If element to delete is top, delete top
        {
        m_ptle = m_ptle->ptleNext(); // Point top to next
        ptleDel->SetNext( NULL ); // Clear next of the one to delete
        delete ptleDel; // Delete it
        return m_ptle; // Return next
        }
    else // Else, find somewhere else in list
        for ( CTrieListEl* ptle = m_ptle; ptle->ptleNext(); ptle = ptle->ptleNext() )
            if ( ptle->ptleNext() == ptleDel ) // If next is the one to delete
                {
                ptle->SetNext( ptleDel->ptleNext() ); //  Link around it
                ptleDel->SetNext( NULL ); // Clear next of the one to delete
                delete ptleDel; // Delete it
                return ptle->ptleNext(); // Return next
                }
    ASSERT( FALSE ); // Must find the one to delete         
    return NULL; // Keep compiler happy
}

void CTrieList::DeleteRec( CRecord* prec ) // Delete all references to record
{
    ASSERT( prec );
    CTrieListEl* ptle = m_ptle; // Get first element
    while ( ptle ) // For each element, delete if it refers to record
        {
        if ( ptle->m_rps.prec == prec ) // If refers to record
            ptle = ptleDelete( ptle ); // Delete and move forward
        else
            ptle = ptle->ptleNext(); // Else just move forward
        }
}

CTrieOut* CTrieList::ptotLookup( const char* pszRest, CTrieOut* ptot, int iLenMatched ) const // Find a string in a list, Add any new matches to the list passed in, iLenMatched is length of match reached by caller
{
    CTrieListEl* ptle = m_ptle; // Get top of list
    for ( ; ptle; ptle = ptle->ptleNext() ) // For each trie string in the list
        {
        BOOL bMatch = TRUE; // Assume match until proved false
        const char* pszIn = pszRest; // Start input string at beginning of rest, and remember how much is matched
        for ( const char* pszTrie = ptle->pszRest(); *pszTrie; pszTrie++, pszIn++ ) // For each char in trie string 
            {
            while ( *pszIn == ' ' && *(pszIn + 1) == ' ' ) // Step over multiple spaces in input
                pszIn++;
            if ( *pszTrie != *pszIn ) // If char of trie string doesn't match char on input, fail
                { 
                bMatch = FALSE;
                break;
                }
            }
        if ( bMatch )
            ptot = ptot->ptotAdd( ptle->rps(), pszIn - pszRest + iLenMatched ); 
        }
    return ptot;
}

#ifdef _DEBUG
void CTrieList::AssertValid() const   // Assert valid routine
    {
    ASSERT( m_iListLen >= 0 );
    CTrieListEl* ptle = m_ptle; // Get top of list
    for ( ; ptle; ptle = ptle->ptleNext() )
        ptle->AssertValid();
    }
#endif      

static void Reverse( Str8& s ) // Reverse a Str8, helper for reverse CTrie
{
	strcpy( pszBigBuffer, s ); // 1.4qzfn
    char* psz = s.GetBuffer( 1 ); // 1.4qzfm GetBuffer OK because immediately written
	strcpy( psz, pszBigBuffer ); // 1.4qzfn Make sure the content is available after GetBuffr
    strrev( psz );
    s.ReleaseBuffer();
}

//------------------------------------------------------------------------------------------------------------
// CTrie member functions

#define GROWBY 5 // Granularity for growth of trie arrays
#define MAXLISTLEN 25 // Maximum list length, length at which a CTrieList is converted to a trie

void CTrie::Init()                          // Initiailization helper for constructor and unload
{
    m_pszLetters = new char [ GROWBY + 1 ]; // Allocate letters array, allowing 1 extra for null terminator
    *m_pszLetters = '\0'; // Init to empty string
    m_pptbsSubtries = new CTrieBase* [ GROWBY ]; // Allocate subtries array
    m_ptls = NULL; // Init info to null
}

void CTrie::DeleteAll()             // Delete all helper for unload
{
    delete m_ptls; // Delete info for word that terminates here
    int iLen = strlen( m_pszLetters ); // Get length of arrays
    for ( int i = 0; i < iLen; i++ ) // Delete each subtrie or sublist in subtries array
        delete m_pptbsSubtries[ i ];
    delete [] m_pptbsSubtries; // Delete subtries array
    delete [] m_pszLetters; // Delete letters array
}

void CTrie::Add( const char* pszRest, CRecPos rps, int iType )      // Add a new piece of information to a trie
{
    ASSERT( pszRest );
    if ( !*pszRest ) // If null string, store info for it here
        {
        if ( !m_ptls )
            m_ptls = new CTrieList( pszRest, rps );
        else    
            m_ptls->Add( pszRest, rps );    
        return;
        }
    Str8 sRev;   
    if ( iType == SUFF ) // If suffix trie, reverse string
        {
        sRev = pszRest; // Make a copy to reverse
        Reverse( sRev ); // Reverse the copy
        pszRest = (const char*)sRev;
        }
    char* pszFind = strchr( m_pszLetters, *pszRest ); // Search for letter in letters array
    if ( pszFind )                                           // If letter in trie, add info to subtrie for letter  
        {
        int iFind = pszFind - m_pszLetters;
        m_pptbsSubtries[ iFind ]->Add( pszRest + 1, rps );
        // If list length over max length, convert to trie
        if ( m_pptbsSubtries[ iFind ]->iListLen() > MAXLISTLEN ) 
            {
            CTrie* ptri = new CTrie();                // Make new trie
            CTrieList* ptls = (CTrieList*) m_pptbsSubtries[ iFind ]; // Get trie list
            CTrieListEl* ptle = ptls->ptle(); // Get top of list
            for ( ; ptle; ptle = ptle->ptleNext() ) // For each element of trie list
                ptri->Add( ptle->pszRest(), ptle->rps() ); // Add it to the new trie
            m_pptbsSubtries[ iFind ] = ptri;                        // Put new trie where list was
            delete ptls; // Delete trie list    
            }
        }
    else                                            // If letter not in trie yet, put it in
        {
        int iLen = strlen( m_pszLetters ); // Get length of array
        if ( iLen && !(iLen % GROWBY) ) // If size up to granularity, add more space
            {
            // Enlarge letters array by granularity
            char* m_ppszOldLetters = m_pszLetters; // Save pointer to old place
            m_pszLetters = new char[ iLen + 1 + GROWBY ]; // Get new place
            memmove( m_pszLetters, m_ppszOldLetters, iLen + 1 ); // Move content of old to new ??Unicode needs add sizeof char, this assumes a char is one byte
            delete [] m_ppszOldLetters; // Delete old
            // Enlarge subtries array by granularity
            CTrieBase** ptbsOldSubtries = m_pptbsSubtries; // Save pointer to old place
            m_pptbsSubtries = new CTrieBase*[ iLen + GROWBY ]; // Get new place
            memmove( m_pptbsSubtries, ptbsOldSubtries, iLen * sizeof( CTrieBase* ) ); // Move content of old to new
            delete [] ptbsOldSubtries; // Delete old
            }
        m_pszLetters[ iLen ] = *pszRest;              // Put letter into trie
        m_pszLetters[ iLen + 1 ] = '\0';
        m_pptbsSubtries[ iLen ] = (CTrieBase*) new CTrieList( pszRest + 1, rps );   // Make a new list for the letter
        }       
}

void CTrie::DeleteRec( CRecord* prec ) // Delete all references to record
{   
    if ( m_ptls ) // If info for word that terminates at this level
        {
        m_ptls->DeleteRec( prec ); // Delete any refs in info for word that terminates at this level
        if ( m_ptls->bIsEmpty() ) // If list becomes empty, delete it and clear it
            {
            delete m_ptls;
            m_ptls = NULL;
            }
        }
    int iLen = strlen( m_pszLetters ); // Find out how many pointers in trie
    for ( int i = 0; i < iLen; i++ ) // For each pointer
        {
        m_pptbsSubtries[ i ]->DeleteRec( prec ); // Delete all references to it in subtries
        if ( m_pptbsSubtries[ i ]->bIsEmpty() ) // If subtrie or list becomes empty, delete it and move down higher letters and pointers
            {
            delete [] m_pptbsSubtries[ i ]; // Delete empty subtrie or list
            for ( int j = i + 1; j < iLen; j++ ) // For all above, move down
                {
                m_pszLetters[ j - 1 ] = m_pszLetters[ j ];
                m_pptbsSubtries[ j - 1 ] = m_pptbsSubtries[ j ];
                }
            iLen--; // Reduce number of letters
            m_pszLetters[ iLen ] = '\0'; // Move trailing null of string down
            i--; // Compensate loop variable for deleted letter
            }
        }
}

CTrieOut* CTrie::ptotLookup( const char* pszRest, CTrieOut* ptot, int iLenMatched ) const // Get info for a string from a trie
{
    ASSERT( pszRest );
    if ( m_ptls ) // Add any info from this node to output list
        for ( CTrieListEl* ptle = m_ptls->ptle(); ptle; ptle = ptle->ptleNext() ) // For each piece of info, add it to the output list
            ptot = ptot->ptotAdd( ptle->rps(), iLenMatched );
    if ( *pszRest ) // If more letters, look up next
        {
        while ( *pszRest == ' ' && *(pszRest + 1) == ' ' ) // Step over multiple spaces
            {
            pszRest++;
            iLenMatched++; // Note that we moved past a char of input
            }
        char* pszFind = strchr( m_pszLetters, *pszRest ); // Search for letter in letters array
        if ( pszFind ) // If letter in trie, add its info from its subtrie to output list
            {
            int iFind = pszFind - m_pszLetters;
            CTrieBase* ptbs = m_pptbsSubtries[ iFind ]; // Find subtrie for letter
            ptot = ptbs->ptotLookup( pszRest + 1, ptot, iLenMatched + 1 ); // Add its info to output list
            }
        }
    return ptot; // Return resulting output list
}

//---------------------------------------------------------------------------------------------------
void CDbTrie::ClearAll() // Unload trie and clear all settings
{
    Unload(); // Clear content of trie
    m_drflst.DeleteAll(); // Clear drflst
    MakeRefs(); // Make all references, because drflst is empty this clears everything  
}

void CDbTrie::CopySettings( CDbTrie* ptri ) // Copy settings from another trie, used for interface to set all affix tries from one dialog box     
{
    ClearAll(); // Clear content of trie, since this copy will invalidate earlier settings
    m_trf.SetType( ptri->m_trf.sType() ); // Copy database type
    for ( const CDatabaseRef* pdrf = ptri->pdrflst()->pdrfFirst(); pdrf; pdrf = ptri->pdrflst()->pdrfNext( pdrf ) )
        m_drflst.Add( pdrf->sDatabasePath(), pdrf->sKey( ) );
    m_mrflst = *ptri->pmrflst(); // Copy input marker list, uses operator=
    m_sMkrOut = ptri->sMkrOut(); // Copy output marker
    MakeRefs(); // Make all references  
}

static char cClassDelimOpen = '['; // !!! Will be made a member of language
static char cClassDelimClose = ']';

static BOOL bHasClass( const char* psz ) // True if string contains a class reference
{
    return strchr( psz, cClassDelimOpen ) != NULL;
}

static BOOL bHasDup( const char* psz ) // True if string contains DUP, Dup, or dup
{
    return strstr( psz, "DUP" ) || strstr( psz, "Dup" ) || strstr( psz, "dup" );
}

void CDbTrie::AddField( CRecord* prec, CField* pfld, CField* pfldOut ) // Add a field to the trie       
{
#ifdef _MAC
    if ( pfld->plng()->bRightToLeft() ) // If Arabic field
            pfld->OverlayAll( '\xA0', ' ' ); // Change any Arabic space 160 to regular space
#endif
    pfld->TrimBothEnds(); // Clean up search feild before putting it into trie
    if ( strstr( pfld->psz(), "  " ) ) // If multiple spaces, squeeze out extras
        {
        Str8 s = pfld->sContents(); // Get temporary copy
        int iPos;
        while ( ( iPos = s.Find( "  " ) ) >= 0 ) // While multiple spaces found, replace with single
            s = s.Left( iPos ) + s.Mid( iPos + 1 );
        pfld->SetContent( s ); // Put fixed version back into field
        }
    pfldOut->TrimBothEnds();
	pfldOut->Replace( "\n", " " ); // 1.5.0fk Fix bug of hang on interlinearize from multi-line gloss
    Str8 s; // Temporary string
    BOOL bHyphEnd = FALSE; // Hyphen at end
    BOOL bHyphStart = FALSE; // Hyphen at start
    const char* pszAdd = pfld->psz(); // String pointer for trie load
    if ( !*pszAdd ) // If null, don't load, and don't try to look at last char // Could be assert, since caller checks also 
        return;
    BOOL bAdd = FALSE; // Switch for hyphens OK for adding
    if ( m_iType == LOOK ) // If lookup trie, don't mess with hyphens
        bAdd = TRUE;
    else
        {   
                // If ends with hyphen, cut it off and remember
        if ( m_pintprcMyOwner->bIsMorphBreakChar( *( pszAdd + pfld->GetLength() - 1 ) ) )
            {
            s = pfld->psz(); // Get temporary string
            s = s.Left( s.GetLength() - 1 ); // Shorten by 1 to cut off hyphen
            pszAdd = s;
            bHyphEnd = TRUE; // Remember ends with hyphen 
            }
                // If starts with hyphen, cut it off and remember
        if ( m_pintprcMyOwner->bIsMorphBreakChar( *pszAdd ) ) 
            {
            pszAdd++; // Move load pointer past hyphen
            bHyphStart = TRUE; // Remember starts with hyphen 
            }
        if ( !*pszAdd ) // Don't load null fields into trie
            return;
        switch ( m_iType )
            {
            case ROOT: // Root must not have hyphens
                bAdd = !bHyphStart && !bHyphEnd;
                break;
            case PREF: // Prefix must have only end hyphen
                bAdd = !bHyphStart && bHyphEnd;
                break;
            case SUFF: // Suffix must have only start hyphen
                bAdd = bHyphStart && !bHyphEnd;
                break;
            case INF: // Infix must have both hyphens
                bAdd = bHyphStart && bHyphEnd;
                break;
            }
        }               
    if ( bAdd ) // If hyphens are right for this trie
        {
        CRecPos rps( 0, pfldOut, prec ); // Make RecPos to load into trie
        if ( bHasClass( pszAdd ) // If it has classes and dup, it must be redup
                && bHasDup( pfldOut->sContents() ) ) // Check for DUP, Dup, or dup in underlying form
            {
            if ( m_iType == SUFF ) // If suffix, reverse, accounting for classes            
                {
                Str8 sRev;
                while ( *pszAdd )
                    {
                    if ( *pszAdd == cClassDelimOpen )
                        {
                        char* pszEndClass = strchr( pszAdd, cClassDelimClose ); // Find end of class name
                        if ( !pszEndClass ) // If no close, fail
                            break;
                        pszEndClass++; // Step to after delimiter close 
                        char c = *pszEndClass; // Remember next char
                        *pszEndClass = '\0'; // Terminate class name
                        sRev = pszAdd + sRev; // Add class to reversal
                        *pszEndClass = c; // Restore char 
                        pszAdd = pszEndClass; // Move past class
                        }
                    else // Else (not class), add next char to reversal
                        sRev = *pszAdd++ + sRev;
                    }
                m_tlsRedup.Add( sRev, rps, m_iType ); // Add reversed form to redup list
                }
            else // Else (not suffix), add normal form to redup list
                m_tlsRedup.Add( pszAdd, rps, m_iType ); // Add it to redup list
            }
        else // Else it is a normal morpheme
            Add( pszAdd, rps, m_iType ); // Add it to trie
        }
}

void CDbTrie::AddRec( CRecord* prec ) // Add a record to the trie
{   // The trie knows what markers it cares about. It has a list of search markers and an output marker.
    // For each search field, make an entry for each output field between it and the next search field.
    // If no output field is found for a search field, use the first search field in the record.
    CField* pfldFirstSearch = NULL; // Remember first search field in the record
    CMarker* pmkrOut = CDbTrie::pmkrOut(); // Get output marker
	BOOL bLookupProc = m_pintprcMyOwner->bLookupProc(); // Note whether this is a lookup process
    // For each desired field found, add it to the trie
    for ( CField* pfld = prec->pfldFirst(); pfld; pfld = prec->pfldNext( pfld ) ) // For each field
        {
        if ( m_mrflst.bSearch( pfld->pmkr() ) ) // If search marker, add its output field to trie
            {
            if ( pfld->bIsEmptyOrWhite() ) // Don't load null fields into trie
                continue;
            BOOL bFound = FALSE; // True if an output field is found
            if ( !pfldFirstSearch )
                pfldFirstSearch = pfld; // Remember first search field in record
				// 1.6.0ae First pass looks only below search marker, second pass looks above as well
				// 1.6.0ae Do second pass if first pass doesn't find any output fields
			for ( int iPass = 1; (iPass <= 2) && !bFound; iPass++ ) // 1.6.0ae Fix bug 1.6.0 of interlinear not finding some fields
				{
				CField* pfldOut = prec->pfldNext( pfld ); // For fields up to the next search field, look for output fields 
				if ( bLookupProc && iPass == 2 ) // If lookup process, start at first field to look all the way through the record // 1.5.9a  // 1.6.0ae Start at top on second pass
					pfldOut = prec->pfldFirst(); // 1.5.9a Make lookup not show all output fields in record
				for ( ; pfldOut; pfldOut = prec->pfldNext( pfldOut ) )
					{
					if ( m_mrflst.bSearch( pfldOut->pmkr() ) && !bLookupProc ) // If another search marker found in parse, don't look any further // 1.5.9a  // 1.6.0ac Fix bug of interlinear not finding output of variant // 1.6.0ae Fix bug 1.6.0ac of parse not stopping at next a
						break;
					if ( m_mrflst.bSearch( pfldOut->pmkr() ) && bFound ) // If another search marker found in lookup, don't look any further // 1.5.9a  // 1.6.0ac Fix bug of interlinear not finding output of variant
						break;
					if ( pfldOut->pmkr() == pmkrOut  // At each output marker found, load it
							&& !pfldOut->bIsEmptyOrWhite() ) // Don't load null output fields
						{
						bFound = TRUE; // Remember that one has been found
						AddField( prec, pfld, pfldOut ); // Add field to trie
						}
					}   
				}
            if ( !bFound && !bLookupProc ) // If no output field found for parsing, use first search field in record
                AddField( prec, pfld, pfldFirstSearch );
            }
        }       
}

void CDbTrie::MakeRefs() // Make database and marker pointers from names
{   
    if ( m_drflst.bIsEmpty() || m_mrflst.bIsEmpty() || m_sMkrOut.IsEmpty() ) // If missing something, clear the database type
        SetDrfType( "" );
    CDatabaseType* ptyp = ptypDrfType(); // Get type of databases in drflst
    if ( !ptyp ) // If no database type, delete any drfs and mrfs
        {
        m_drflst.DeleteAll(); // Empty database list (only has any if type file is messed up)
        m_mrflst.DeleteAll(); // Empty marker list
        SetMkrOut( "" ); // Clear output marker
        }
    else // Else set marker pointers of mrflst and output marker
        {
        m_mrflst.bMakeRefs( ptyp->pmkrset() ); // Make marker references (if false, can warn user that some markers were not found and were removed from list)
        m_pmkrOut = ptyp->pmkrset()->pmkrSearch_AddIfNew( m_sMkrOut ); // Get output marker pointer
        }
}

void CDbTrie::AddAll() // Add all records in all databases to trie  
{   
    MakeRefs(); // Make database and marker pointers from names
    m_bLoaded = TRUE; // Note that trie is now loaded
    CDatabaseRef* pdrf = m_drflst.pdrfFirst(); // For all databases, call pdoc to make sure they can be opened
    while ( pdrf )  // For each document
        {
        if ( !pdrf->pdoc() ) // If doc file cannot be opened
            m_drflst.Delete( &pdrf ); // Delete it from the path
        else
            pdrf = m_drflst.pdrfNext( pdrf );           
        }
    for ( pdrf = m_drflst.pdrfFirst(); pdrf; pdrf = m_drflst.pdrfNext( pdrf ) ) // For all databases, add all records to trie
        {
        CIndex* pind = pdrf->pind(); // Get owning index of document // 1.4ad Eliminate progress bar duting trie build for parse
                // For each record, add it to trie  
        for ( CNumberedRecElPtr pnrl = pind->pnrlFirst(); pnrl; pnrl = pind->pnrlNext( pnrl ) )
            AddRec( pnrl->prec() ); // Add record to trie, pass output marker for optimization
        }
}

static int iTemplateLen( const char* psz ) // Measure length of redup template, counting each class as one char
{
    for ( int iLen = 0; *psz; iLen++ )
        {
        if ( *psz == cClassDelimOpen )
            {
            psz = strchr( psz, cClassDelimClose ); // Find end of class name
            if ( !psz ) // If no close, stop
                break;
            }
        psz++;
        }   
    return iLen;
}

static int iClassMatch( // Attempt to match class starting at pszTemplate with char at pszRedup
        CVarInstanceSet* pvinset, 
        const char* pszTemplate,
        const char* pszRedup ) // Returns len of class if success, 0 if fail. Use it to set len, which will shorten the non-class stepping code also
{ 
    if ( !*pszRedup ) // If redup is at end of string, don't match
        return 0;
    char* pszEndClass = strchr( pszTemplate, cClassDelimClose ); // Find end of class name
    if ( !pszEndClass ) // If no close, fail
        return 0;
    *pszEndClass = '\0'; // Terminate class name
    CVarInstance* pvin = pvinset->pvinSearch( pszTemplate + 1 ); // Find class from class name
    *pszEndClass = cClassDelimClose; // Restore close class delim
    if ( !pvin ) // If no such class, fail
        return 0;
    const char* pszClass = pvin->sChars(); // Get class content
    if ( !strchr( pszClass, *pszRedup ) ) // If next char not found in class, fail. AB Doesn't consider multichars, but I think the failure mode from this will be OK.
        return 0;
    return pszEndClass + 1 - pszTemplate; // Return len of class, including delimiters
}

CTrieOut* CDbTrie::ptotSearch( const char* pszRest, const int iSettings ) // External interface for trie lookup
{
    if ( bIsEmpty() ) // If empty trie, fail immediately to save time
        return FALSE;
    if ( !m_bLoaded ) // If trie not loaded, load it now
        AddAll(); // Add all records of all databases to trie 
    
    CLangEnc* plng = m_mrflst.pmrfFirst()->pmkr()->plng(); // Get language of first search marker
    ASSERT( plng );

	Str8 sLower = pszRest; // Lower case version of string
	BOOL bLower = FALSE; // Signal that there is a different lower case version of string
    const char* psz = pszRest; // Prepare for multichar lookup
    const CMChar* pmch = NULL;
    if ( *psz && plng->bMChar(&psz, &pmch) && pmch->bUpperCase() && pmch->bHasOtherCase() ) // If upper case multichar found and it has other case, lower it
        {    
        pmch = pmch->pmchOtherCase(); // Change to lower case
        sLower = pmch->sMChar() + psz; // Make lower case version of string by adding lower case letter to rest of string
		bLower = TRUE;
		}

    Str8 sRev; // Reversed version of string
    if ( m_iType == SUFF ) // If suffix trie, reverse string before lookup
        {
        sRev = pszRest; // Make a copy to reverse
        Reverse( sRev ); // Reverse the copy
        pszRest = (const char*)sRev;
		Reverse( sLower ); // Also reverse the lower case version
        }
    
    CTrieOut* ptotTop = ptotLookup( pszRest ); // Look up
	if ( iSettings & CASE_FOLD && bLower ) // If case fold and lower case is different, look up lower case as well
	{
		CTrieOut* ptotTopLower = ptotLookup( sLower ); // Look up
        for ( CTrieOut* ptot = ptotTopLower; ptot; ptot = ptot->ptotDelete() ) // Add any found to main result
			ptotTop = ptotTop->ptotAdd( *ptot, ptot->iLen() );
	}
        
    if ( !m_tlsRedup.bIsEmpty() && !( iSettings & WHOLE_WORD ) ) // If redup list and not whole word, check for reduplication
        {
		pszRest = (const char*)sLower; // Look up lower case version
        CVarInstanceSet* pvinset = plng-> pvinset(); // Set up for variables
        for ( CTrieListEl* ptle = m_tlsRedup.ptle(); ptle; ptle = ptle->ptleNext() ) // For each element of redup list
            {
            const char* pszTemplate = ptle->pszRest();
            const char* pszRedup = pszRest;
            BOOL bTemplateMatch = FALSE;
            // Establish place to check for redup, based on template, or full redup from general
            if ( *pszTemplate == cClassDelimOpen && *( pszTemplate + 1 ) == '.' ) // If general redup
                {
                int iLen = strlen( pszRest );
                if ( iLen >= 4 ) // Do general dup only if total length of 4 or more
                    {
                    int iRedupLen = ( iLen + 1 ) / 2;  // Find length to be duplicated. If odd, use higher number to allow for hyphen.
                    pszRedup += iRedupLen; // Find position of second half of redup
                    if ( ( iLen % 2 ) && m_pintprcMyOwner->bIsMorphBreakChar( *( pszRedup - 1 ) ) ) // If redup pos is at hyphen, don't require it to match
                        iRedupLen--;        
                    bTemplateMatch = ( strncmp( pszRest, pszRedup, iRedupLen ) == 0 ); // If match, remember it
                    }
                }
            else
                {
                const char* pszRedupMatch = pszRedup + iTemplateLen( pszTemplate );
                if ( m_pintprcMyOwner->bIsMorphBreakChar( *pszRedupMatch ) ) // If redup pos is at hyphen, move past it
                    pszRedupMatch++;
                bTemplateMatch = TRUE; // Assume match until proven false
                while ( *pszTemplate && bTemplateMatch )
                    {
                    if ( *pszTemplate == cClassDelimOpen ) // If class, see if it matches
                        {
                        int iClassLen = iClassMatch( pvinset, pszTemplate, pszRedup ); // Test for class match
                        if ( !iClassLen ) // If no match, fail
                            bTemplateMatch = FALSE;
                        if ( !( *pszRedup++ == *pszRedupMatch++ ) ) // If char is not reduplicated, fail
                            bTemplateMatch = FALSE;
                        pszTemplate += iClassLen; // Move past class
                        }
                    else if ( *pszTemplate++ == *pszRedup++ ) // Else (not class), see if char matches  
                        pszRedupMatch++; // Move redup match pos forward one char   
                    else // Else (no match), fail
                        bTemplateMatch = FALSE;
                    }
                }   
            if ( bTemplateMatch ) // If redup is found, make entry
                ptotTop = ptotTop->ptotAdd( ptle->rps(), pszRedup - pszRest );
            }
        }
        
    if ( ptotTop && iSettings & WHOLE_WORD ) // If whole word match, make sure only whole words
        {
        CLangEnc* plng = m_mrflst.pmrfFirst()->pmkr()->plng(); // Get language of first search marker
        ASSERT( plng ); // Language can't be default if iSettings was present
                // Delete all that don't match up to word boundary
        CTrieOut* ptotPrev = NULL;      
        for ( CTrieOut* ptot = ptotTop; ptot; )
            {         
            const char* psz = pszRest + ptot->iLen(); // Get position of end of match
            if ( plng->bIsWdCharMove( &psz ) ) // If more letters in word, didn't match whole word, so delete
                {
                if ( !ptotPrev ) // If top, move top to next
                    ptotTop = ptot->ptotNext();
                else // Else (not top), fix next of prev
                    ptotPrev->SetNext( ptot->ptotNext() ); // Point prev to next after deleted one  
                ptot = ptot->ptotDelete(); // Delete this
                }
            else // Else (this is whole word) move to next
                { 
                ptotPrev = ptot; // Remember this as previous
                ptot = ptot->ptotNext(); // Move to next
                }
            }
        }    
    if ( ptotTop && iSettings & LONGEST ) // If only longest wanted, remove all shorter ones
        {       
        int iLenMatched = ptotTop->iLen(); // Find length of first match (longest)
                // Cut off any matches shorter than longest
        for ( CTrieOut* ptot = ptotTop; ptot->ptotNext(); ptot = ptot->ptotNext() )
            if ( ptot->ptotNext()->iLen() < iLenMatched ) // Find first that is shorter and delete it
                {
                ptot->DeleteRest(); // Delete it and rest of list
                break;
                }
        }
    return ptotTop;
}

BOOL CDbTrie::bAddToFieldList( CTrieOut* ptot, CFieldList* pfldlst, int* piLenMatched ) // Add ptot list to field list, set iLenMatched to longest
{
    if ( !ptot )
        return FALSE;
    char cGlossSeparator = *m_pintprcMyOwner->pszGlossSeparator(); // should just be one char
    BOOL bSucc = FALSE;
    for ( CTrieOut* ptotT = ptot; ptotT; ptotT = ptotT->ptotNext() ) // For each match from trie, add it to the field list
        {
        CField* pfldNew = new CField( *ptotT->pfld ); // Make a copy of field
        int iPos = 0;   
        if ( m_pintprcMyOwner->bStopAtSemicolon() && cGlossSeparator // If stop at semicolon and semicolon, cut off end
                    && ( iPos = pfldNew->sContents().Find( cGlossSeparator ) ) >= 0 )
            pfldNew->SetContent( pfldNew->sContents().Left( iPos ) ); // Keep only part before semicolon
        pfldNew->TrimBothEnds();
        char cForceStart = m_pintprcMyOwner->pintprclst()->cForcedGlossStartChar(); // Short names for forced gloss chars
        char cForceEnd = m_pintprcMyOwner->pintprclst()->cForcedGlossEndChar();
                // If this process is lookup, while next is lookup on same fields, prevent ambiguity by using forced gloss
        if ( m_iType == LOOK
                && !strchr( pfldNew->psz(), cForceStart ) ) // Don't add forced gloss if field it would add to already has one
            {
            CInterlinearProc* pintprcNext = m_pintprcMyOwner->pintprclst()->pintprcNext( m_pintprcMyOwner );
            for ( ; pintprcNext // While another proc
                    && pintprcNext->bLookupProc() // And it is lookup from same field
                    && pintprcNext->pmkrFrom() == m_pintprcMyOwner->pmkrFrom()
					&& !( !m_pintprcMyOwner->bAdapt() && pintprcNext->bAdapt() ); // And it is not the first adapt proc
                    pintprcNext = m_pintprcMyOwner->pintprclst()->pintprcNext( pintprcNext ) )
                {
                CDbTrie* ptriNext = ((CLookupProc*)pintprcNext)->ptri( LOOK ); // Get next trie
                if ( !( m_mrflst == *ptriNext->pmrflst() ) ) // If not the same fields, don't do forced gloss
                    break;  
                CMarker* pmkrOut = ptriNext->pmkrOut(); // Get output marker of next proc
                CField* pfldOut = ptotT->prec->pfldFirstInSubRecord( pmkrOut, ptotT->pfld ); // Find first output field in subrecord
                if ( pfldOut ) // If an output marker is found, if the next is the same, don't use it
                    {
                    CField* pfldOutNext = ptotT->prec->pfldNext( pfldOut );
                    if ( pfldOutNext && pfldOutNext->pmkr() == pfldOut->pmkr() ) // If another the same, it is ambiguous, so don't use it
                        break; // Don't let any others get attached, as they would be in the wrong place
                    }
                if ( pfldOut )
                    pfldOut->TrimBothEnds(); // Trim ends before checking for spaces
                if ( pfldOut  // If output field found, and it has no semicolons, use it, add it as forced gloss
                        && pfldOut->GetLength() > 0
                        && ( !cGlossSeparator || !strchr( pfldOut->psz(), cGlossSeparator ) ) ) // Check for no semicolons
                    {
                    Str8 sForce = cForceStart + pfldOut->sContents() + cForceEnd; // Make forced gloss
                    Str8 sNew; // Temp string to build
                    Str8 sRest = pfldNew->sContents(); // Get content of field
                            // Clean up the semicolons (not used, because test above skips this)
                    if ( cGlossSeparator )
                        {
                        if ( !sRest.IsEmpty() && sRest.GetChar( sRest.GetLength() - 1 ) == cGlossSeparator ) // If ends with semicolon, cut it off
                            sRest = sRest.Left( sRest.GetLength() - 1 );
                        int iSemi = 0;  
                        Str8 sSpaceSemi( " " ); // Prepare space and semicolon pattern for searching
                        sSpaceSemi += cGlossSeparator;
                        while ( ( iSemi = sRest.Find( sSpaceSemi ) ) >= 0 ) // While space before semicolon, remove it
                            sRest = sRest.Left( iSemi ) + sRest.Mid( iSemi + 1 );
                        int iPos = 0;
                        while ( ( iPos = sRest.Find( cGlossSeparator ) ) > 0 ) // While semicolon found, add forced gloss before it
                            {
                            sNew += sRest.Left( iPos ) + sForce + cGlossSeparator;
                            sRest = sRest.Mid( iPos + 1 ); // Keep part after semicolon
                            }
                        }
                    sNew += sRest;  
                    if ( sNew.GetLength() > 0
                            && sNew.GetChar( sNew.GetLength() - 1 ) != cGlossSeparator ) // If field does not end with semicolon, put forced gloss on the end
                        sNew += sForce;
                    pfldNew->SetContent( sNew ); 
                    }   
                else
                    break; // If we can't use this output field, don't do any more  
                }   
            }
                // Make entry
        if ( pfldNew->bIsEmptyOrWhite() ) // If nothing left in field, don't make entry
            delete pfldNew;
        else
            {
            pfldlst->Add( pfldNew ); 
            bSucc = TRUE;
            }
        }
    if ( bSucc ) // If valid fields found, succeed
        {
        *piLenMatched = ptot->iLen(); // Set length to len of first
                // Split fields with semicolons into multiple ambiguities (not used, since test above avoids semicolons; doesn't work correctly anyway)
        for ( CField* pfld = pfldlst->pfldFirst(); pfld; pfld = pfldlst->pfldNext( pfld ) ) // For each result of parse
            {
            Str8 sRest = pfld->sContents(); // Get content of field
            int iPos = 0;
            while ( cGlossSeparator && ( iPos = sRest.Find( cGlossSeparator ) ) > 0 ) // While semicolon found
                {
                if ( iPos == sRest.GetLength() - 1 ) // If semicolon is at end, don't make an ambiguity
                    break; 
                CField* pfldNew = new CField( *pfld );
                pfldNew->SetContent( sRest.Left( iPos ) ); // Get part before semicolon
                pfldNew->TrimBothEnds();
                sRest = sRest.Mid( iPos + 1 ); // Keep part after semicolon
                pfld->SetContent( sRest );
                pfld->TrimBothEnds();
                pfldlst->InsertBefore( pfld, pfldNew );
                }
            }
        }   
    return bSucc;
}

BOOL CDbTrie::bRedup( const char* pszWord, const char* pszPrevWord, 
        CFieldList* pfldlst, int* piLenMatched ) // If the root is reduped, add it to analyses
{
    if ( m_tlsRedup.bIsEmpty() )
        return FALSE;
    CLangEnc* plng = m_mrflst.pmrfFirst()->pmkr()->plng(); // Get language of first search marker
    ASSERT( plng );
    CTrieOut* ptotTop = NULL;
    CVarInstanceSet* pvinset = plng->pvinset(); // Set up for variables
    for ( CTrieListEl* ptle = m_tlsRedup.ptle(); ptle; ptle = ptle->ptleNext() ) // For each element of redup list
        {
        const char* pszTemplate = ptle->pszRest();
        const char* pszRedup = pszPrevWord;
        BOOL bTemplateMatch = FALSE;
        // Establish place to check for redup, based on template, or full redup from general
        if ( *pszTemplate == cClassDelimOpen && *( pszTemplate + 1 ) == '.' ) // If general redup
            bTemplateMatch = ( strcmp( pszWord, pszRedup ) == 0 ); // If match, remember it
        else
            ; // ??? Maybe someday do template or [...o...] (for tick tock, flip flop, clip clop, hip hop, tip top), combine code with parse
        if ( bTemplateMatch ) // If redup is found, make entry
            ptotTop = ptotTop->ptotAdd( ptle->rps(), strlen( pszRedup ) );
        }
    BOOL bSucc =  bAddToFieldList( ptotTop, pfldlst, piLenMatched ); // If any successes, add them to field list and return success     
    CTrieOut::s_DeleteAll( ptotTop ); // Get rid of temporary struct built by search    
    return bSucc;
}

BOOL CDbTrie::bSearch( const char* pszRest, CFieldList* pfldlst, int* piLenMatched, const int iSettings ) // External interface for trie lookup
{
	if ( !*pszRest ) // If null string, don't try any further
		return FALSE;
    pfldlst->DeleteAll(); // Clear field list first
    CTrieOut* ptot = ptotSearch( pszRest, iSettings ); // Perform search
    if ( !ptot ) // If fail, return fail, leave iLenMatched alone
        return FALSE;   
    BOOL bSucc = bAddToFieldList( ptot, pfldlst, piLenMatched );
    CTrieOut::s_DeleteAll( ptot ); // Get rid of temporary struct built by search   
    return bSucc;
}

void CDbTrie::IndexUpdated( CIndexUpdate iup ) // Keep trie in step with databases when an index is updated
{
    if ( m_bLoaded && m_drflst.bRefersTo( iup.pindset() ) ) // If changed doc is used in this trie, update references to record
        {
        if ( iup.iType() & INDEX_UPDATE_DELETE ) // If delete or modify, remove record from trie
            {
            DeleteRec( iup.prec() ); // Delete references to record from trie class
            m_tlsRedup.DeleteRec( iup.prec() ); // Delete references to record from redup list
            }
        if ( iup.iType() & INDEX_UPDATE_ADD ) // If add or modify, add record to trie
            AddRec( iup.prec() ); // Add record to trie
        }
}

void CDbTrie::OnDocumentClose( CShwDoc* pdoc ) // Check for references to document being closed 
{
    if ( m_bLoaded && m_drflst.bRefersTo( pdoc ) ) // If this doc is used in this trie, unload and delete the trie
        {
        Unload(); // Unload the trie
        }
    m_drflst.OnDocumentClose( pdoc ); // Delete references to the document
}

void CDbTrie::MarkerUpdated( CMarkerUpdate& mup ) // update mrflsts in case of marker name change
{
    if ( mup.bNameModified() && m_trf.ptyp() == mup.pmkr()->pmkrsetMyOwner()->ptyp() )
        {
        m_mrflst.NameModified( mup.pmkr()->sName(), mup.pszOldName() ); // change marker text to new name
        if ( !strcmp( m_sMkrOut, mup.pszOldName() ) ) // handle name change for m_pmkrOut
            m_sMkrOut = mup.pmkr()->sName();
        }
}

static const char* psz_Out = "Out"; // Marker to output from database
static const char* psz_dbtyp = "dbtyp"; // Database type

#ifdef typWritefstream // 1.6.4ac
void CDbTrie::WriteProperties( Object_ofstream& obs, const char* pszMarker ) // Give trie marker so different tries can be marked differently
#else
void CDbTrie::WriteProperties( Object_ostream& obs, const char* pszMarker ) // Give trie marker so different tries can be marked differently
#endif
{
    obs.WriteNewline();
    obs.WriteBeginMarker( pszMarker );                   
    
    obs.WriteString( psz_dbtyp, m_trf.sType() ); // Write database type
    m_drflst.WriteProperties( obs ); // Write database list
    m_mrflst.WriteProperties( obs ); // Write marker list
    CMarker::s_WriteRef( obs, psz_Out, m_sMkrOut );

    obs.WriteEndMarker( pszMarker );
}   

BOOL CDbTrie::s_bReadProperties( Object_istream& obs,
        CDbTrie* ptri, const char* pszMarker, int iType )
{
    if ( !obs.bReadBeginMarker( pszMarker ) ) // If not the begin marker for this class, return false (it is some other class)
        return FALSE;
    Str8 s;
    while ( !obs.bAtEnd() ) // While more in file
        {
        if ( obs.bReadString( psz_dbtyp, s ) )
            ptri->SetDrfType( s );
        else if ( ptri->pdrflst()->bReadProperties( obs ) )
            ;
        else if ( ptri->pmrflst()->bReadProperties( obs ) )
            ;
        else if ( CMarker::s_bReadRef( obs, psz_Out, s ) )
            ptri->SetMkrOut( s );       
        else if ( obs.bEnd( pszMarker ) ) // If end marker or erroneous begin break, this is the standard ending of read loop
            break;
        }
    return TRUE; // True means we ate something         
}

BOOL CDbTrie::bModalView_Properties()
{
    CTrieDlg dlg( this );
    return dlg.DoModal();
}

#ifdef _DEBUG
void CTrie::AssertValid() const
{
}
void CDbTrie::AssertValid() const
{
}
#endif      
