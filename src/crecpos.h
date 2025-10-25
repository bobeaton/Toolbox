// crecpos.h  CRecPos class for maintaining position within a document

#ifndef CRECPOS_H
#define CRECPOS_H

//#include "crlookel.h"
#include "crecord.h"


// The data members are intended to be accessed
//  directly rather than through access functions.
class CRecPos  // Hungarian: rps
{
public:
    CRecord* prec;
    CField* pfld;
    int iChar; // Char position within field Str8

#ifdef SUBFIELD
private:
    CMarker* m_pmkrSubfield;
        // Subfields in Standard Format are like character styles in RTF.
        // A subfield is an element distinguished from the surrounding
        // field contents because it differs in language or function.
        // A subfield consists of its text contents enclosed in
        // matching begin and end tags, which indicate its marker.
        // In a file the subfield tag delimiters are curly braces,
        // but when the contents of fields are read into the program
        // these delimiters are converted to character codes 1 and 2.
        //
        // Example of a difference in language:
        // \ng Replicated form from root {fv}ahw{-fv}.
        // Example of same language, but a difference in function:
        // \ee This is {ew}not{-ew} limited to coconut groves
        //     but is used for mangoes, etc.
        //
        // Examples of the parts:
        // Subfield:   {fv}ahw{-fv}
        // Contents:   ahw
        // Marker:     fv
        // Begin tag:  {fv}
        // End tag:    {-fv}
        // Delimiters: {}
    void SetSubfieldMarker();
        // Set the field or subfield marker at this position
        // when it is constucted and after it is moved.
#endif // SUBFIELD

public:
    CRecPos( int iCharPar = 0, CField* pfldPar = NULL, CRecord* precPar = NULL )
        {
        prec = precPar;
        pfld = pfldPar;
        iChar = iCharPar;
#ifdef SUBFIELD
        SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }
    ~CRecPos() {}  // destructor

    void SetPos( int iCharPar, CField* pfldPar, CRecord* precPar )
        {                             
        prec = precPar;
        pfld = pfldPar;
        iChar = iCharPar;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
#ifdef SUBFIELD
        SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }
        
    void SetPos( int iCharPar, CField* pfldPar )
        {                             
        pfld = pfldPar;
        iChar = iCharPar;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
#ifdef SUBFIELD
		SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }
        
    void SetPos( int iCharPar )
        {                             
        iChar = iCharPar;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
#ifdef SUBFIELD
		SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }

    void SetPos( const char* psz, CField* pfldPar )
        {
        pfld = pfldPar;
        iChar = psz - *pfld;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
#ifdef SUBFIELD
		SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }

    void SetPos( const char* psz )
        {
        iChar = psz - *pfld;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
#ifdef SUBFIELD
		SetSubfieldMarker();  // 1996-10-08 MRP
#endif // SUBFIELD
        }
   
    // SetPos with extend in case field is not long enough
    void SetPosEx( int iCharPar, CField* pfldPar )
        {                             
        pfld = pfldPar;
        ASSERT( pfld );
        pfld->Extend( iCharPar ); // Pad with spaces to make sure field is long enough to reach desired position
        iChar = iCharPar;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
        }

    void SetPosEx( int iCharPar )
        {                             
        ASSERT( pfld );
        pfld->Extend( iCharPar ); // Pad with spaces to make sure field is long enough to reach desired position
        iChar = iCharPar;
        ASSERT( iChar >= 0 );
//        ASSERT( iChar < 32000 ); // 1.4ps Remove assert, vals in 32 bit are long // ???
        }

    BOOL bIsWhite() const  // Return true if at white space, based on language definition
        {
        return pfld->plng()->bIsWhite( psz() ); // Use language definition of white space
        }
        
    BOOL bLeftIsWhite() const  // Return true if char to left is white space, based on language definition
        {
        return iChar && pfld->plng()->bIsWhite( psz( iChar - 1 ) ); // Use language definition of white space
        }
    
    BOOL bIsWdEnd() const // Return true if at white space or end of string
        {
        return ( !*psz() || bIsWhite() );
        }

    BOOL bIsNonWhite() const // Return true if at non-white char    
        {
        return ( *psz() && !bIsWhite() );
        }

    BOOL bIsDigit() const // 5.99r Return true if at digit, solves problem of isascii    
        {
        return ( *psz() && __isascii( *psz() ) && isdigit( *psz() ) );
        }

    BOOL bLeftIsNonWhite() const // Return true if at non-white char    
        {
        return iChar && pfld->plng()->bIsNonWhite( psz( iChar - 1 ) ); // Use language definition of white space
        }

    BOOL bSkipWhite() // Move forward to non-white
        {
        int i = iChar; // Remember start
        while ( bIsWhite() ) // While in whitespace, move
            iChar++;
        return ( iChar != i ); // If moved, return true
        }

    BOOL bSkipNonWhite() // Move to end of word 
        {
        int i = iChar; // Remember start
        while ( bIsNonWhite() ) // While not at end of word, move
            iChar++;
        return ( iChar != i ); // If moved, return true
        }

    BOOL bEndOfItem()
        {
        if ( !*psz() )
            return TRUE;
        if ( *psz() == '\n')
            {
            if ( pfld->pmkr()->bNoWordWrap() )
                {
                return TRUE;
                }
            else
                {
                if ( *(psz()+1) == '\n' )
                    return TRUE;
                }
            }
        return FALSE;
        }

    int iWdEnd() // Return end of current word
        {
        CRecPos rps = *this; // Get temp rps
        rps.bSkipNonWhite(); // Move it to end of word
        return rps.iChar; // Return its iChar
        }

    BOOL bSkipWhiteLeft() // Move left to non-white
        {
        int i = iChar; // Remember start
        while ( bLeftIsWhite() ) // While in whitespace, move
            iChar--;
        return ( iChar != i ); // If moved, return true
        }

    BOOL bSkipNonWhiteLeft() // Move left to begin of word 
        {
        int i = iChar; // Remember start
        while ( bLeftIsNonWhite() ) // While non-white to left, move
            iChar--;
        return ( iChar != i ); // If moved, return true
        }
        
    BOOL bMoveToNextWd() // Move to next word
        {
        int i = iChar; // Remember start
        bSkipNonWhite(); // Move out of current word
        bSkipWhite(); // Move to start of next word     
        return ( iChar != i ); // If moved, return true
        } 
    
    BOOL bMoveToPrevWd() // Move to prev word start
        {
        int i = iChar; // Remember start
        bSkipWhiteLeft(); // Move to end of prev word       
        bSkipNonWhiteLeft(); // Move to start of word
        return ( iChar != i ); // If moved, return true
        } 

    BOOL bMove() // Move forward one character unless at end of string
        {
        const char* psz = CRecPos::psz();
        if ( !*psz )  // If end of string
            return FALSE;
            
        const CMChar* pmch = NULL;
        pfld->plng()->bMChar( &psz, &pmch );
        iChar += psz - CRecPos::psz(); // advance position
        return TRUE;
        }

      //09-08-1997
    BOOL bMoveBack() // Move back one character unless at begin of string
        {
        if (iChar == 0)      
            return FALSE;      // Begin of string
       
          // Go one step back
        pfld->pmkr()->plng()->PrevEditingUnit(pfld->sContents(), &iChar);
        return TRUE;
        }
     

    BOOL bIsWdSepMove() // Return true if word separator, and move past it
        {
        const char* psz = CRecPos::psz(); // Get temp pointer for move
        if ( pfld->plng()->bIsWdSepMove( &psz ) ) // If word separator
            {
            iChar += psz - CRecPos::psz(); // Advance by difference
            return TRUE;
            }
        return FALSE;   
        }
    
    BOOL bIsWdCharMove() // Return true if word char, and move past it
        {
        const char* psz = CRecPos::psz(); // Get temp pointer for move
        if ( pfld->plng()->bIsWdCharMove( &psz ) ) // If word char
            {
            iChar += psz - CRecPos::psz(); // Advance by difference
            return TRUE;
            }
        return FALSE;   
        }
    
    BOOL bIsWdSep() // Return true if at word sep, don't move
        {
        const char* psz = CRecPos::psz(); // Get temp pointer for move
        return pfld->plng()->bIsWdSepMove( &psz ); // Return true if word char
        }

    BOOL bLeftIsWdSep() // Return true if at word sep, don't move
        {
        if ( iChar == 0 )
            return FALSE;
        const char* psz = CRecPos::psz( iChar - 1 ); // Get temp pointer for move
        return pfld->plng()->bIsWdSepMove( &psz ); // Return true if word char
        }

    BOOL bIsWdChar() // Return true if at word char, don't move
        {
        const char* psz = CRecPos::psz(); // Get temp pointer for move
        return pfld->plng()->bIsWdCharMove( &psz ); // Return true if word char
        }

    BOOL bLeftIsWdChar() // Return true if at word char, don't move
        {
        if ( iChar == 0 )
            return FALSE;
        const char* psz = CRecPos::psz( iChar - 1 ); // Get temp pointer for move
        return pfld->plng()->bIsWdCharMove( &psz ); // Return true if word char
        }

    BOOL bSkipWdSep() // Move forward to non-white
        {
        int i = iChar; // Remember start
        while ( bIsWdSepMove() ) // While in whitespace, move
            ;
        return ( iChar != i ); // If moved, return true
        }

    BOOL bSkipWdSepLeft() // Move left to non-word-separator
        {
        int i = iChar; // Remember start
        while ( bLeftIsWdSep() ) // While in word-separator, move
            iChar--;
        return ( iChar != i ); // If moved, return true
        }

    BOOL bSkipPunc() // Move past punc to word or white
        {
        int i = iChar; // Remember start
        while ( bIsNonWhite() && bIsWdSepMove() ) // While in whitespace, move
            ;
        return ( iChar != i ); // If moved, return true
        }

   BOOL bSkipWdChar() // Move to end of word 
        {
        int i = iChar; // Remember start
        while ( bIsWdCharMove() ) // While not at end of word, move
            ;
        return ( iChar != i ); // If moved, return true

	   }
        
    int iWdLen() // Length of current word, counting only word formation chars
        {
        CRecPos rpsWdEnd = *this; // Temp for word end
        rpsWdEnd.bSkipWdChar(); // Skip to end of word formation chars
        return rpsWdEnd.iChar - iChar; // Return distance moved
        }   
        
    BOOL bMoveToNextWdWfc() // Move to next word 
        {
        int i = iChar; // Remember start
        bSkipWdChar(); // Move out of current word
        bSkipWdSep(); // Move to start of next word     
        return ( iChar != i ); // If moved, return true
        } 
    
    BOOL bMoveToPrevWdWfc() // Move to prev word start
        {
        int i = iChar; // Remember start
        bMoveToPrevWd(); // Move to previous word
        bSkipWdSep(); // Skip over word separators
        if ( iChar == i ) // If didn't go anywhere, try to skip punc at begin word
            {
            bMoveToPrevWd(); // Move to previous word to get to front of punc
            bMoveToPrevWd(); // Move to real previous word
            bSkipWdSep(); // Skip over word separators
            }
        return ( iChar < i ); // If moved, return true
        }
        
    BOOL bMatch( const char* pszMatch ) // Match string at current position
        {
        const char* pszT = psz();
        while ( *pszMatch )
            if ( *pszMatch++ != *pszT++ )
                return FALSE;
        return TRUE;        
        }    

    const char* psz() const { return (const char*)*pfld + iChar; }
    const char* psz( int i ) const { return (const char*)*pfld + i; }
    const char* pszFieldBeg() const { return (const char*)*pfld; }
    const char* pszFieldEnd() const { return (const char*)*pfld + pfld->GetLength(); }

    BOOL operator<(const CRecPos& rps) const;
    BOOL operator>(const CRecPos& rps) const;
    BOOL operator<=(const CRecPos& rps) const { return !( *this > rps ); }
    BOOL operator>=(const CRecPos& rps) const { return !( *this < rps ); }
    BOOL operator==(const CRecPos& rps) const;
    BOOL operator!=(const CRecPos& rps) const { return !( *this == rps ); } 

    BOOL bEndOfField() const 
		{ 
		return iChar >= (int)pfld->GetLength();  // 1.4hfd Fix bug of hang on browse wordlist refs
		}

    BOOL bEndOfFieldOrLine() const
		{ 
		return bEndOfField() || *psz() == '\n'; // 1.4hfe Handle possible invalid iChar
		}

    BOOL bEndOfBundle() const { return bLastInBundle() && bEndOfField(); }
    
    BOOL bInterlinear() const // True if interlinear line, note special definition
		{
        if ( pfld->pmkr()->bFirstInterlinear() ) // If first interlinear, consider it interlinear only if it is followed by a field that looks like it was interlinearized from it
            {
            CField* pfldNext = prec->pfldNext( pfld ); // Look at next field
            if ( !pfldNext ) // If no next field, do not consider interlinear
                return FALSE;
            else // Else (next field), treat this as interlinear if next is interlinear, but not another first interlinear
                return ( pfldNext->pmkr()->bInterlinear() && !pfldNext->pmkr()->bFirstInterlinear() );
            }   
        else // Else (not first interlinear), consider it interlinear if it is marked as such
            return pfld->pmkr()->bInterlinear(); // True if interlinear marker
		}
    
    BOOL bFirstInterlinear() const { return pfld->pmkr()->bFirstInterlinear(); }
    
    CField* pfldNext() const { return prec->pfldNext( pfld ); }
    CField* pfldPrev() const { return prec->pfldPrev( pfld ); }

    CRecPos rpsNextField( int iPos = -1 ) const
    {
        CRecPos rps = *this;
        rps.pfld = rps.pfldNext();
        if ( iPos != -1 ) // If  desired position given, use it
            rps.iChar = iPos;
        if ( rps.pfld && (int)rps.pfld->GetLength() < rps.iChar ) // Don't let iChar be past end of field
            rps.iChar = rps.pfld->GetLength();
        return rps;
    }

    void NextField( int iPos = -1 )
    {
        if ( iPos == -1 ) // If no desired position given, try to keep same
            iPos = iChar;
        pfld = pfldNext();
        ASSERT( pfld ); // Don't walk off bottom with this
        iChar = __min( iPos, (int)pfld->GetLength() );
    }

    void NextFieldExtend()
    {
        pfld = pfldNext();
        ASSERT( pfld ); // Don't walk off bottom with this
        pfld->Extend( iChar );
    }

    CRecPos rpsPrevField( int iPos = -1 ) const
    {
        CRecPos rps = *this;
        rps.pfld = rps.pfldPrev();
        if ( iPos != -1 ) // If  desired position given, use it
            rps.iChar = iPos;
        if ( rps.pfld && (int)rps.pfld->GetLength() < rps.iChar ) // Don't let iChar be past end of field
            rps.iChar = rps.pfld->GetLength();
        return rps;
    }

    void PrevField( int iPos = -1 )
    {
        if ( iPos == -1 ) // If no desired position given, try to keep same
            iPos = iChar;
        pfld = pfldPrev();
        ASSERT( pfld ); // Don't walk off bottom with this
        iChar = __min( iPos, (int)pfld->GetLength() );
    }

    CField* pfldFindInBundle(const CMarker* pmkr) const;  // Find first field with a specified marker
    CField* pfldFindInBundleAdd( CMarker* pmkr, const CMarker* pmkrBefore = NULL ) const;  // Find first field with a specified marker, add after pmkrBefore if not present
        // True if this is the last field in interlinear bundle
    BOOL bLastInBundle() const
    {
        CRecPos rps = *this;
        if ( !( rps.bInterlinear() || rps.bFirstInterlinear() ) ) // Should only be asked on interlinear fields
			return FALSE; // 5.99p If not interlinear, don't go any further
        rps.pfld = rps.pfldNext(); // Look at next field
        if ( !rps.pfld  // If no more in record, no more in bundle
                || !rps.bInterlinear() // If next is not interlinear, no more in bundle
                || rps.bFirstInterlinear() ) // If next is first in another bundle, no more in this bundle
            return TRUE;
        return FALSE; // Otherwise there are more in bundle, this is not last   
    }
        // Return rps with first field of current bundle
    CRecPos rpsFirstInBundle( int iPos = 0 ) const
    {
        CRecPos rps = *this;
        if ( rps.bFirstInterlinear() ) // If this is first, return it immediately
            return rps;
        if ( !rps.bInterlinear() ) // 5.99p If this is not shown as interlinear, it may be the first line of interlinear without any annotations yet, return it immediately
			 return rps;
        while ( !rps.prec->bIsFirst( rps.pfld ) && rps.bInterlinear() && !rps.bFirstInterlinear() ) // While interlinear and not first in bundle, move to prev
            rps.pfld = rps.pfldPrev();
        if ( !rps.bInterlinear() ) // If walked out of interlinear, go back down to first one
            rps.pfld = rps.pfldNext();
        ASSERT( rps.pfld ); 
        ASSERT( rps.bInterlinear() );   // Must be an interlinear field
        rps.iChar = __min( iPos, (int)rps.pfld->GetLength() );
        return rps; 
    }       
        // Return rps with end of last field of current bundle
    CRecPos rpsEndOfBundle() const
    {
        CRecPos rps = *this;
        ASSERT ( rps.bInterlinear() ); // Should only be asked on interlinear fields
        for ( ; ; rps.pfld = rps.pfldNext() ) // Find last field
            {
            if ( rps.bLastInBundle() )
                break;
            }   
        ASSERT( rps.pfld ); 
        ASSERT( rps.bInterlinear() );   // Must be an interlinear field
        rps.iChar = rps.pfld->GetLength(); // Find end of field
        return rps; 
    }       
    /* Sample loop for walking a bundle with the above routines
    for ( CRecPos rps = rpsFirstInBundle(); ; rps.pfld = rps.pfldNext() )
        {
        // Work on pfld here
        if ( rps.bLastInBundle() )
            break;
        }   
    */  
    
	BOOL bAllGiven( CInterlinearProcList* pintprclst ); // 1.4kb Return true if bundle contains only Given lines
	BOOL bAlignWholeBundle(); // 1.4kb Correct interlinear alignment of bundle
	int iLongestWordNumChars( int iPos ); // 1.4kb Find longest word at a particular position
	int iAlignBundleAtPos( int iPos ); // 1.4kb Correct interlinear alignment of bundle at a position

    void TrimBundle( BOOL bAddNl = FALSE ); // Trim ends of all lines in bundle
    void ClearBundleBelow( CRecPos rpsEndCol ); // Clear space below up to end column
    void Clear( CRecPos rpsEndCol ); // Clear space up to end column or end
    void LoosenWordParse( int iNum, BOOL bEvenAtEnd = FALSE ); // Loosen next words by iNum spaces // 1.6.4s 
    void Loosen( int iNum, BOOL bEvenAtEnd = FALSE ); // Loosen next words by iNum spaces
    void TightenLeft( int* piAdjust = NULL ) const; // Tighten current words as much as possible
    void Tighten( int* piAdjust = NULL, BOOL bExtend = FALSE ) const; // Tighten next words as much as possible
    void InterlinReplaceWord( CField* pfldOutput, const char* pszWord, int* piLen, int iLenMatched = 1 ) const; // Replace, insert, or delete a word in interlinear
    
    CRecPos rpsBeginSentence() const; // Find beginning of sentence
    CRecPos rpsEndSentence() const; // Find end of sentence

    // following function implementations are in SHWVWRAP.CPP
    BOOL bBundleSameAs( CRecPos rps2 ) const; // return TRUE if bundles have same fields in same order
    BOOL bPrevBundle(); // move to beginning of previous bundle, return TRUE if successful
    BOOL bNextBundle(); // move to beginning of next bundle, return TRUE if successful
    int iFldLenWithoutNL() const; // return length of field minus ending nl
    int iBundleLength() const; // return length of longest line in bundle, minus ending nl

    BOOL bAtWdBegInBundle() // this function assumes rps is on first field in bundle
    {
        // check this position in every field in bundle for a space followed by non-space at current position
        for ( CRecPos rpsTemp = *this; ; rpsTemp.pfld = rpsTemp.pfldNext() )
            {
            if ( rpsTemp.iChar > (int)rpsTemp.pfld->GetLength()
                || !rpsTemp.psz() || *rpsTemp.psz() == ' ' 
                || ( rpsTemp.iChar && *(rpsTemp.psz()-1) != ' ' ) )
                return FALSE;
            if ( rpsTemp.bLastInBundle() ) // all fields have a space at this position!
                return TRUE;
            }
    }
     
    CRecPos rpsNextWdBeginBundle() // Find next word that begins a bundle
    {
        CRecPos rps = rpsFirstInBundle(); // Find first in bundle
        rps.iChar = __min( iChar, (int)rps.pfld->GetLength() );
        if ( !rps.bEndOfField() ) // If not at end, move one before looking
            rps.iChar++;
        while ( !rps.bEndOfField() && !rps.bAtWdBegInBundle() ) // While more and not at wd begin bundle, move forward
            rps.iChar++; // Move forward
        return rps; 
    }

	void BreakBundle( int iChunkLen ) // Break bundle into two at position iLen, move rps to start of new bundle
	{
		CField* pfldInsertAfter = rpsEndOfBundle().pfld; // Place to insert tail of broken field, insert new bundle at end of this bundle
		CRecPos rps = null;
		for ( rps = rpsFirstInBundle(); ; rps.pfld = rps.pfldNext() ) // For each field in bundle, break it and move tail down to end of bundle // _Temp sounds like making a field, not interating through existing fields
			{
			const char* pszFld = rps.pfld->psz(); // Pointer to simplify expressions below // 1.4qzfm Upgrade GetBuffr for Unicode build
			int iLen = min( iChunkLen, rps.pfld->GetLength() ); // Find place to cut off
			CField* pfldNew = new CField( rps.pfld->pmkr(), pszFld + iLen ); // Make new field of tail // 1.4qzfm Upgrade GetBuffr for Unicode build
			rps.prec->InsertAfter( pfldInsertAfter, pfldNew ); // Insert tail after bundle
			pfldInsertAfter = pfldNew; // Note that next tail goes after this one
			rps.pfld->SetContent( rps.pfld->Left( iLen ) ); // Cut tail off this field // 1.4qzfm Upgrade GetBuffr for Unicode build
			rps.pfld->Trim(); // Trim off spaces at end // 1.4qzfm Upgrade GetBuffr for Unicode build
			if ( rps.bLastInBundle() ) // If last in bundle, add line break at end of field
				{
				if ( !rps.pfld->GetLength() || // Protect against invalid GetChar arg
						rps.pfld->GetChar( rps.pfld->GetLength() - 1 ) != '\n' )
					*rps.pfld += "\n"; // add line break at end of bundle
				break;
				}
			}
		SetPos( 0, rps.pfldNext() ); // goto start of bundle just created // 1.4qzfm Upgrade GetBuffr for Unicode build
	}

#ifdef _DEBUG
    void AssertValid() const;
#endif      

public:
    CMarker* pmkr() const
        // Returns the field or subfield marker of the substring
        // beginning at this position and ending at the next tag
        // or at the end of the field.
#ifdef SUBFIELD
		{ return m_pmkrSubfield ? m_pmkrSubfield : pfld->pmkr(); }
#else
		{ return pfld->pmkr(); }		
#endif // SUBFIELD
    BOOL bInSubfield() const
#ifdef SUBFIELD
        { return m_pmkrSubfield != NULL; }
#else
		{ return FALSE; };
#endif // SUBFIELD

    BOOL bInsertSubfield(CMarker* pmkrSubfield)
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
    BOOL bMoveRightPastSubfieldTag()
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;

    BOOL bMoveRightPastSubfieldTags()
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
        // Returns whether there is a subfield tag at this position.
        // If there is, this position moves to the right past it
        // (and past any other adjacent tags).
    BOOL bMoveLeftPastSubfieldTags()
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
        // Returns whether this position is just after a subfield tag.
        // If it is, this position moves to the left past it
        // (and past any other adjacent tags).
    
    BOOL bMoveHome();
        // Move this position to the beginning of its line.
        // Return whether it actually moved from where it had been.
    BOOL bMoveEnd();
        // Move this position to the end of its line.
        // Return whether it actually moved from where it had been.

    void MoveRightPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE);
    BOOL bMoveRightPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE);
        // Returns whether there is a view substring at this position.
        // If there is, this position moves to the right past it
        // (i.e. past the tags following it, to the beginning
        // of the next line, or to the end of the field) and
        // the substring is returned as the interval [*prpsB,*prpsE);
        // otherwise neither this position nor the arguments are changed.
        // A view substring is a text element that can be drawn or
        // whose horizontal extent can be measured in one operation.

    BOOL bMoveLeftPastViewSubstring(CRecPos* prpsB, CRecPos* prpsE);
        // Returns whether there is a view substing at this postion.
        // If there is, this position moves to the left before it and
        // the substring is returned. *prpsB showes to the first element
        // of the substing and *prpsE to the former postion of the
        // object.
        // If there is no substring before this position prpsB and prpsE
        // dont change their position

    static void s_ReadSubfields(CMarkerSet* pmkrset, char* pszContents);
        // Convert any curly brace subfield tag delimiters read from a file
        // to the internal codes used in the contents of field objects.
    static void s_WriteSubfields(std::ostream& ios, const Str8& sContents);
        // Write the contents of a field and convert the delimiters
        // of any subfield tags from the internal codes to curly braces.

    static void s_DecrementRefsToSubfieldMarkers(CField* pfld)
#ifndef SUBFIELD
		{}
#endif // SUBFIELD
		;
        // Decrement the references to any subfield markers in pfld
        // because the field is about to be deleted.
        // Both begin and end tags count as references to their markers.

    static BOOL s_bAtSubfieldTag(const char* psz)
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
        // Returns whether there is a subfield tag at psz.
    static BOOL s_bAtSubfieldTag(CMarkerSet* pmkrset,
            const char** ppsz, CMarker** ppmkrSubfield, BOOL* pbEndTag)
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
			;
        // Returns whether there is a subfield tag at *ppsz.
        // If there is, *ppsz moves past it and its marker, and
        // whether it is a begin or end tag are returned.

private:
    void SetSubfieldMarker(CMarker* pmkrSubfield, BOOL bEndTag,
            BOOL bMovingRight)
#ifndef SUBFIELD
		{}
#endif // SUBFIELD
			;
        // Set or clear this position's subfield marker
        // because it is moving past a subfield tag.

    BOOL bMoveRightToNextSubfieldTag(Length* plenTag,
            CMarker** ppmkrSubfield, BOOL* pbEndTag)
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
			;
        // Returns whether there is a subfield tag at or after this position.
        // If there is, this position moves to it and its length,
        // marker, and whether it is a begin or end tag are returned;
        // otherwise neither this position nor the arguments are changed.
        // The caller can then continue to move to the right past the tag
        // by incrementing this position's character offset by *plenTag.

    BOOL bAtSubfieldTag() const
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
        // Returns whether there is a subfield tag at this position.
    BOOL bAtSubfieldTag(Length* plenTag,
            CMarker** ppmkrSubfield, BOOL* pbEndTag) const
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
			;
        // Returns whether there is a subfield tag at this position.
        // If there is, its length, marker, and whether it is
        // a begin or end tag are returned.

    BOOL bAfterSubfieldTag() const
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
		;
        // Returns whether this position is just after a subfield tag.
    BOOL bAfterSubfieldTag(Length* plenTag,
            CMarker** ppmkrSubfield, BOOL* pbEndTag) const
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
			;
        // Returns whether this position is just after a subfield tag.
        // If there is, its length, marker, and whether it is
        // a begin or end tag are returned.

    static BOOL s_bAtExternalSubfieldTag(const char* psz, CMarkerSet* pmkrset,
            Length* plenTag, CMarker** ppmkrSubfield, BOOL* pbEndTag)
#ifndef SUBFIELD
		{ return FALSE; }
#endif // SUBFIELD
			;
        // Returns whether there is a subfield tag at psz.
        // Use this function when reading the fields from a file.
        // The caller should convert the external curly brace delimiters
        // to the internal character codes 1 and 2.
};  // class CRecPos
#endif  // CRECPOS_H
