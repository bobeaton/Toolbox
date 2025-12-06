// sfstream.cpp  SIL Standard Format iostream classes

// Read and write SIL Standard Format records and fields from an iostream,
// which can represent a Shoebox database or other Standard Format file.

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "sfstream.h"
#include "mkr.h"  // class CMarkerSet, CMarker
#include "crecord.h"  // classes CRecord, CField
#include "patch.h"  // Shw_pszWhiteSpace, Shw_bAtWhiteSpace
#include "not.h"  // class CNote
#include "crecpos.h"  // s_ReadSubfields, s_WriteSubfields
#include "typ.h"
#include "ddx.h"  // Shw_bNeedsToBeTrimmed

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

static const Length s_maxlenField = 320000; // 1.4pw Increase maximum field length to 320,000
static const Length s_maxlenLine = 320000; // 1.4pw Extend
static const char* s_pszInsertableMarker = "??";


SF_istream::SF_istream(std::istream& ios, CMarkerSet* pmkrset, CMarker* pmkrRecord,
        BOOL bSkipToFirstRecordMarker, BOOL bRemoveSpaces) :
    m_ios(ios),
    m_pmkrset(pmkrset),
    m_pmkrRecord(pmkrRecord),
    m_sRecordMarker(m_pmkrRecord->sMarker()),
    m_lenRecordMarker(m_sRecordMarker.GetLength()),
    m_bRemoveSpaces(bRemoveSpaces),
    m_lenInsertableMarker(strlen(s_pszInsertableMarker)),
    m_maxlenInsertion(1 + m_lenInsertableMarker + 1)
{
    ASSERT(m_pmkrset);
    ASSERT(m_pmkrRecord);

    m_pfldNext = NULL;
    m_pszFieldBuf = new char[s_maxlenField + 1];
    m_pszFieldEnd = NULL;
    m_lenFieldRemaining = 0;

    m_pszNextLineBuf = new char[m_maxlenInsertion + s_maxlenLine + 1];
    m_pszNextLine = NULL;
    m_bNextLine = FALSE;

    m_bSh1 = FALSE;
    m_bSkippedLines = FALSE;
    m_lFieldsSkipped = 0;
    m_lUsefulLinesSkipped = 0;

    m_bFieldTooLong = FALSE;  // 07-26-1997   

    // Read ahead; sets m_bNextLine
    if (bSkipToFirstRecordMarker)
    {
        while (true)
        {
            int c = m_ios.peek();
            if (c == std::char_traits<char>::eof())
                break;
            if (c == '\\' || c == '\n')
                break;
            // consume one char to advance (was previous logic)
            int iSucc = m_ios.get();
            if (iSucc == std::char_traits<char>::eof())
                break;
        }
        SkipToNextRecordMarker();
    }
    else
        SkipToNextMarker();
}

SF_istream::~SF_istream()
{
    delete m_pfldNext;
    delete [] m_pszFieldBuf;
    delete [] m_pszNextLineBuf;
}



BOOL SF_istream::bReadRecord(CRecord** pprec)
// constructs a new CRecord
{
    CField* pfldFirst = NULL;
    if (!bReadField(&pfldFirst))
        return FALSE;           // 08-04-1997
    
    ASSERT (pfldFirst );
    ASSERT( pfldFirst->pmkr() == m_pmkrRecord );
    CRecord* prec = new CRecord(pfldFirst);
    
    while ( !bAtEnd() )
        {
        CField* pfld = NULL;
        if (!bReadField(&pfld))
            {
            delete prec;                 // 08-04-1997
            return FALSE;
            }
        
        ASSERT( pfld );
        if ( pfld->pmkr() == m_pmkrRecord )
            {           
            UnReadField(&pfld);  // First field of the next record
            break;
            }
        prec->Add(pfld);
        }

    // Shoebox 2.0 trims any extra newlines at end of last field of record.
    // It separates records by a single newline when saving the database.
    CField* pfldLast = prec->pfldLast();
    ASSERT( pfldLast );
//    pfldLast->TrimContents(); // 1.4qzfj
	pfldLast->TrimRight(); // 1.4qzfj Update TrimContents to TrimRight for Unicode build

    ASSERT( pprec );        
    *pprec = prec;
    return TRUE;
}  // SF_istream::bReadRecord


BOOL SF_istream::bReadField(CField** ppfld)
// constructs a new CField; updates the marker set
{
    ASSERT( ppfld );
    if ( m_pfldNext )  // If a field has been "unread"
        {
        *ppfld = m_pfldNext;  // Then "reread" it
        m_pfldNext = NULL;
        return TRUE;
        }

    const char* pszMarker = NULL;
    Length lenMarker = 0;
    const char* pszContents = NULL;
    if (!ReadField(&pszMarker, &lenMarker, &pszContents))
        return FALSE;         // 08-04-1997
    
    CMarker* pmkr = m_pmkrset->
        pmkrSearchForWholeSubString(pszMarker, lenMarker);
    if ( !pmkr )
        {
        Str8 sMarker(pszMarker, lenMarker);
        pmkr = m_pmkrset->pmkrAdd_MarkAsNew(sMarker);
        }
    ASSERT( pmkr );
    
    // if ( m_pmkrset->bSubfields() )
    //     CRecPos::s_ReadSubfields(m_pmkrset, (char*)pszContents);

    // 07-30-1997
    // If a fieldline is typed with no wrapping, and it exceeds 4000
    // characters, a bug occures. CheckLineLength will wrap the line,
    // if this case happens.
    CheckLineLength(pszContents);      //07-30-1997
    if (m_bFieldTooLong) 
        return FALSE;

    *ppfld = new CField(pmkr, pszContents);
    return TRUE;
}  // SF_istream::bReadField



void SF_istream::UnReadField(CField** ppfld)
// hold this Cfield until it's needed
{
    // NOTE: The marker of a Standard Format field has already been added
    // to the marker set (if it was new) when it was read. No attempt is made
    // to delete such a marker from the set if its field is unread, even if
    // it is never actually reread before the stream is deleted.
    ASSERT( !m_pfldNext );  // Can only "unread" one field,
    ASSERT( ppfld );
    m_pfldNext = *ppfld;
    *ppfld = NULL;  // Caller is relinquishing "ownership" of this field
}


BOOL SF_istream::ReadField(const char** ppszMarker, Length* plenMarker,
        const char** ppszContents)
// appends lines into m_pszFieldBuf; validates marker; 
//     returns ptrs to marker and contents
{    
    ASSERT( !m_pfldNext && m_bNextLine && *m_pszNextLine == '\\' );
    m_pszFieldEnd = m_pszFieldBuf;
    m_lenFieldRemaining = s_maxlenField;    
    do AppendNextLineToField();
        while ( bReadNextLine() && *m_pszNextLine != '\\' );

    // 07-28-1997
    // It could be, that one field is to long (Bigger than 32KBytes).
    // Then m_bFieldToLong = TRUE and the "File-open" process has to be
    // interrupted
    if (m_bFieldTooLong) return FALSE;
    
    // Match and validate the Standard Format marker
    ASSERT( *m_pszFieldBuf == '\\' );
    const char* pszMarker = m_pszFieldBuf + 1;
    const char* pszAfterMarker = pszMarker; 
    Length lenMarker = 0;
    CNote* pnot = NULL;
    if ( !CMarker::s_bMatchNameAt(&pszAfterMarker, Shw_pszWhiteSpace,
            &lenMarker, &pnot) )
        {
        lenMarker = strcspn(pszMarker, Shw_pszWhiteSpace);
        pszAfterMarker = pszMarker + lenMarker;
        pszMarker = s_pszInsertableMarker;
        lenMarker = strlen(pszMarker);
        delete pnot;
        }
    if ( *pszAfterMarker == ' ' )
        pszAfterMarker++;

    // Return pointers to field marker and contents
    ASSERT( ppszMarker );
    *ppszMarker = pszMarker;
    ASSERT( plenMarker );
    *plenMarker = lenMarker;
    ASSERT( ppszContents );     
    *ppszContents = pszAfterMarker; 

    return TRUE;
}  // SF_istream::ReadField

void SF_istream::AppendNextLineToField()
// appends a line into m_pszFieldBuf from m_pszNextLineBuf
{
    if ( m_lenFieldRemaining == 0 ) return;
            
    // Insert a newline when appending any lines after the first
    if ( m_pszFieldEnd != m_pszFieldBuf )
        {
        *m_pszFieldEnd++ = '\n';
        m_lenFieldRemaining--;
        *m_pszFieldEnd = '\0';
        }

    ASSERT( m_bNextLine );
    Length len = strlen(m_pszNextLine);
    if ( m_lenFieldRemaining < len )
        len = m_lenFieldRemaining;
    memcpy(m_pszFieldEnd, m_pszNextLine, len);
    m_pszFieldEnd += len;
    m_lenFieldRemaining -= len;
    *m_pszFieldEnd = '\0';
    m_bNextLine = FALSE;
}  // SF_istream::AppendNextLineToField


BOOL SF_istream::bReadNextLine()
// puts a useable line in m_pszNextLineBuf; 
//  skips over internal & old deleted fields; removes prec/fol spaces
{
    BOOL bNextLineMustBeMarked = FALSE;
    
    // 07-28-1997
    // It could be, that one field is to long (Bigger than 32KBytes).
    // Then m_bFieldToLong = TRUE and the "File-open" process has to be
    // interrupted. In older programversions the ASSERT command occured!
    if (m_lenFieldRemaining == 0) 
       {
        m_bFieldTooLong=TRUE;
        return FALSE;
       }
    
    ASSERT(!m_bNextLine);
    while ( !m_bNextLine && m_ios.peek() != std::char_traits<char>::eof())
        {
        GetNextLine();
        
        if ( *m_pszNextLine == '\\' )  // Line begins with a backslash
            {
            const char* pch = m_pszNextLine + 1;
            char ch = *pch;
            
            if ( ch == '_' )  // Shoebox internal field
                {
                const char* pszMarker = m_pszNextLine + 2;
                Length lenMarker = strcspn(pszMarker, Shw_pszWhiteSpace);
                
                if ( lenMarker == 2 && strncmp(pszMarker, "no", 2) == 0 )
                    {
                    m_bSkippedLines = TRUE;  // Skipping this \_no line
                    bNextLineMustBeMarked = TRUE;
                    }
                else
                    SkipToNextMarker();  // Sets m_bNextLine
                }
                
            else if ( ch == '\\' && m_bSh1 )  // Uncompressed deleted record
                SkipToNextRecordMarker();  // Sets m_bNextLine
                
            else if ( ch == '\\' || ch == '\0' || Shw_bAtWhiteSpace(pch) )
                {
                // Consider a backslash without a marker to be field content
                // Insert a space before the backslash
                ASSERT( m_pszNextLineBuf < m_pszNextLine );
                *--m_pszNextLine = ' ';
                m_bNextLine = TRUE;
                }
            
            else  // An ordinary Standard Format marker
                m_bNextLine = TRUE;
            }
            
        else  // Line does not begin with a backslash
            {
            if ( m_bRemoveSpaces )
                {
                // Preceding spaces
                Length lenSpaces = strspn(m_pszNextLine, " ");
                if ( lenSpaces != 0 && m_pszNextLine[lenSpaces] == '\\' )
                    lenSpaces--;  // Leave one space if a backslash follows
                m_pszNextLine += lenSpaces;
                    
                // Following spaces
                Length len = strlen(m_pszNextLine);
                char* psz = m_pszNextLine + len;
                lenSpaces = 0;
                while ( m_pszNextLine < psz && *--psz == ' ' )
                    lenSpaces++;
                ASSERT( lenSpaces <= len );
                len -= lenSpaces;
                m_pszNextLine[len] = '\0';
                }
            m_bNextLine = TRUE;
            }
        }

    if ( m_bNextLine && *m_pszNextLine != '\\' && bNextLineMustBeMarked )
        {
        // Insert \?? marker at the beginning of an unmarked line
        // which immediately follows and internal \_no line,
        // so that the text doesn't get appended to the record's key field.
        BOOL bWhiteSpace = Shw_bAtWhiteSpace(m_pszNextLine);
        char* pszMarked = m_pszNextLine;
        pszMarked -= (1 + m_lenInsertableMarker);
        pszMarked -= (bWhiteSpace ? 0 : 1);
        ASSERT( m_pszNextLineBuf <= pszMarked );
        char* psz = pszMarked;
        *psz++ = '\\';
        memcpy(psz, s_pszInsertableMarker, m_lenInsertableMarker);
        psz += m_lenInsertableMarker;
        if ( !bWhiteSpace )
            *psz++ = ' ';
        ASSERT( psz == m_pszNextLine );
        m_pszNextLine = pszMarked;
        }
        
    return m_bNextLine;
}  // SF_istream::bReadNextLine

void SF_istream::GetNextLine()
// puts a line in m_pszNextLineBuf from the iostream
{
    m_pszNextLine = m_pszNextLineBuf + m_maxlenInsertion;
    m_ios.get(m_pszNextLine, s_maxlenLine);
    
    if ( m_ios.peek() == '\n' )
        (void) m_ios.get();			// move past the new line
}


void SF_istream::SkipToNextRecordMarker()
// skips lines until a useable record marker line is in m_pszNextLineBuf
{
    ASSERT( !m_bNextLine );
	BOOL bShHeaderLinePrev = FALSE; // 1.4pck Skip blank lines after shoebox header line
	BOOL bShHeaderLine = FALSE; // 1.4pcg Don't put header line into skipped fields
	m_sSkippedFields = ""; // 1.4pcp Clear skipped fields string at each new start
    while ( !m_bNextLine && m_ios.peek() != std::char_traits<char>::eof())
        {
        GetNextLine();
		bShHeaderLinePrev = bShHeaderLine; // 1.4pck Skip blank lines after shoebox header line
		bShHeaderLine = FALSE; // 1.4pcg Don't put header line into skipped fields
        if ( *m_pszNextLine == '\\' )
            {
            const char* pszMarker = m_pszNextLine + 1;
            Length lenMarker = strcspn(pszMarker, Shw_pszWhiteSpace);
            if ( lenMarker == m_lenRecordMarker && strncmp(pszMarker, m_sRecordMarker, lenMarker) == 0 )
                m_bNextLine = TRUE; // Note record marker coming next
            else
                {
                const char* pch = m_pszNextLine + 1;
                char ch = *pch;
                if ( ch == '_' )
                    {
                    const char* pszMarker = m_pszNextLine + 2;
                    Length lenMarker = strcspn(pszMarker, Shw_pszWhiteSpace);
                    if ( lenMarker == 2 && strncmp(pszMarker, "sh", 2) == 0 )
                        {
						bShHeaderLine = TRUE; // 1.4pcg Don't put header line into skipped fields
                        if ( 6 < strlen(m_pszNextLine) && m_pszNextLine[6] == '1' ) // Remember if Shoebox 1
                            m_bSh1 = TRUE;
                        }
					if ( strncmp(pszMarker, "DateStampHasFourDigitYear", 25 ) == 0 ) // 1.4pcj Don't put date stamp line into skipped fields
						bShHeaderLine = TRUE; // 1.4pcj Don't put date stamp line into skipped fields
                    m_bSkippedLines = TRUE;
                    }
                else if ( ch == '\\' || ch == '\0' || Shw_bAtWhiteSpace(pch) )
                    m_bSkippedLines = TRUE;  // Skipping this line
                else
                    {
                    m_lFieldsSkipped++;  // Skipping this field
                    m_lUsefulLinesSkipped++;
                    m_bSkippedLines = TRUE;
                    }
                }
            }
        else	// no marker
			{
            if ( *m_pszNextLine ) // non-empty
                {
                m_bSkippedLines = TRUE;
                m_lUsefulLinesSkipped++;
                }
			}
		if ( !bShHeaderLine && !m_bNextLine ) // 1.4pcg If skipped line is not header or record marker, store it
			{
			if ( strlen( m_pszNextLine ) != 0 ) // 1.4pf Fix bug of extra and missing blank lines // 1.4pck Skip blank lines after shoebox header line
				{
				m_sSkippedFields += m_pszNextLine; // 1.4pcg Have stream collect skipped fields
				m_sSkippedFields += "\n"; // 1.4pf Put nl after line // 1.4pcg Have stream collect skipped fields
				}
			}
        }
}  // SF_istream::SkipToNextRecordMarker

void SF_istream::SkipToNextMarker()
// skips lines until a useable marked line is in m_pszNextLineBuf
{
    ASSERT( !m_bNextLine );
    while ( !m_bNextLine && m_ios.peek() != std::char_traits<char>::eof())
        {
        GetNextLine();
        
        if ( *m_pszNextLine == '\\' )
            {
            const char* pch = m_pszNextLine + 1;
            char ch = *pch;
            if ( ch == '_' ||
                    ch == '\\' || ch == '\0' || Shw_bAtWhiteSpace(pch) )
                m_bSkippedLines = TRUE;  // Skipping this line
            else
                m_bNextLine = TRUE;
            }
        else	// no marker
            if (*m_pszNextLine)		// non-empty 3-4-98
                {
                m_bSkippedLines = TRUE;
                m_lUsefulLinesSkipped++;
                }
        }
}  // SF_istream::SkipToNextMarker


// 07-30-1997
void SF_istream::CheckLineLength(const char* pszCont)
{
    size_t lenRemainingInBuf = (m_pszFieldBuf + s_maxlenField) -
        (pszCont + strlen(pszCont));
    const int maxLineLength = 1000000; // 5.96b ALB Lengthen to prevent bug of chopping input
    char* psz = (char*) pszCont;
    
    while (strlen(psz) > maxLineLength)
        {
         size_t lenLine = strcspn(psz,"\n"); 
         if (lenLine > maxLineLength)            // wrapping necessary
             {
             if ( lenRemainingInBuf == 0 )
                {
                m_bFieldTooLong=TRUE;
                return;
                }

             lenRemainingInBuf--;
             psz += maxLineLength;
             memmove(psz+1,psz,strlen(psz)+1);
             *psz = '\n';
             psz++;
             }
         else
            {                           // No wrapping necessary
             psz += (lenLine+1);
            }  // if
         }  // while
}  //Sf_istream::CheckLineLength



// ==========================================================================

Field_ostream::Field_ostream(std::ostream& ios, BOOL bTrimSurroundingWS) :
    m_ios(ios),
    m_bTrimSurroundingWS(bTrimSurroundingWS)
{
}
    
Field_ostream::~Field_ostream()
{
}

void Field_ostream::s_TrimmedFieldContents(const CField* pfld,
        const char** ppchBegin, const char** ppchEnd)
{
    ASSERT( pfld );
    ASSERT( ppchBegin );
    ASSERT( ppchEnd );
    const char* psz = pfld->sContents();  // 1998-03-03 MRP
    int len = strlen(psz);
    const CMarker* pmkr = pfld->pmkr();
    if ( !pmkr->bNoWordWrap() )  // If this is a wrapped field
        {
        // Skip preceding and following white space
        int lenPrecedingWS = 0;
        int lenFollowingWS = 0;
        if ( Shw_bNeedsToBeTrimmed(psz, &lenPrecedingWS, &lenFollowingWS) )
            {
            psz += lenPrecedingWS;
            len -= (lenPrecedingWS + lenFollowingWS);
            }
        }
        
    *ppchBegin = psz;
    *ppchEnd = psz + len;
}


// --------------------------------------------------------------------------

SF_ostream::SF_ostream(std::ostream& ios, BOOL bTrimSurroundingWS) :
    Field_ostream(ios, bTrimSurroundingWS),
    m_bHaveWrittenField(FALSE)
{
}

SF_ostream::~SF_ostream()
{
}

void SF_ostream::WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst) // 1.1cm Write alphabetic divider
	{
	CMarker* pmkr = ptyp->pmkrset()->pmkrSearch("zzzAlphDiv"); // 1.1ed Change to add field and field name if necessary
	if ( !pmkr) 
		{
		pmkr = ptyp->pmkrset()->pmkrAdd_MarkAsNew("zzzAlphDiv");
	    CNote* pnot = NULL;
		pmkr->bSetFieldName("Alphabetic Divider", &pnot);
		}
    m_ios << '\\';  // Open an SF marker
	m_ios << "zzzAlphDiv "; // 1.1cm Write alphabetic divider marker // 1.1ed Change to zzz and capital letters
	m_ios << sAlphLet; // 1.1cm Write alphabetic divider
	m_ios << "\n"; // 1.1cm Write nl after marker
	}

void SF_ostream::WriteField(const CField* pfld, BOOL bFirstField)
{
    if ( bFirstField && m_bHaveWrittenField )
        m_ios << '\n';  // Separate this record from previous output

    ASSERT( pfld );
    const char* pszBegin = NULL;
    const char* pszEnd = NULL;
    if ( m_bTrimSurroundingWS )
        Field_ostream::s_TrimmedFieldContents(pfld, &pszBegin, &pszEnd);
    else
        {
        pszBegin = pfld->sContents();
        pszEnd = pszBegin + strlen(pszBegin);
        }
    WriteField(pfld->pmkr()->sMarker(), pszBegin, pszEnd);
}

void SF_ostream::WriteField(const char* pszMarker, const char* pszContents)
{
    ASSERT( pszContents );
    const char* pszEnd = pszContents + strlen(pszContents);
    WriteField(pszMarker, pszContents, pszEnd);
}

BOOL bMemberOfCharSet(char ch, const char* pszCharSet);

void SF_ostream::WriteField(const char* pszMarker,
        const char* pszBegin, const char* pszEnd)
{
    ASSERT( pszMarker );
    ASSERT( pszBegin );
    ASSERT( pszEnd );
	CProject* pprj = Shw_papp()->pProject(); // 1.5.4b 
    m_ios << '\\' << pszMarker;
    if ( pszBegin != pszEnd )  // If non-empty
        {
        const char* psz = pszBegin;
        if ( !bMemberOfCharSet(*psz, Shw_pszOtherWhiteSpace) )
            m_ios << ' ';  // Separate contents from marker by a space

		if ( pprj->bSaveWithoutNewlines() ) // 1.5.4b If save without newlines
			{
			const char* pszNL = NULL; // 1.5.4b 
			while ( (pszNL = strchr(psz, '\n')) != NULL ) // 1.5.4b While a newline found
				{
				if ( pszNL - psz > 0 ) // 1.5.4b If there is content, write it
					m_ios.write(psz, pszNL - psz); // 1.5.4b Write up to newline
				if ( pszNL == pszEnd - 1 ) // 1.5.4b If final newline, write it
					m_ios << '\n';  // 1.5.4b Write newline
				else
					m_ios << ' ';  // 1.5.4b Write a space instead of newline
				psz = pszNL + 1; // 1.5.4b Move past newline
				}
			if ( pszEnd - psz > 0 ) // 1.5.4b If there is any more content, write it
					m_ios.write(psz, pszEnd - psz); // 1.5.4b Write up to newline
			}
		else // 1.5.4b Else, normal write of newlines
			{
			const char* pszBackslashAtBOL = NULL;
			while ( (pszBackslashAtBOL = strstr(psz, "\n\\")) != NULL )
				{
				pszBackslashAtBOL += 1;  // Point to the backslash;
				m_ios.write(psz, pszBackslashAtBOL - psz);
				m_ios << ' ';  // Write a space before the backslash
				psz = pszBackslashAtBOL;
				ASSERT( *psz == '\\' );
				}
			ASSERT( psz <= pszEnd );
			int lenRest = pszEnd - psz;  // 1998-12-07 MRP
			m_ios.write(psz, lenRest);  // Write the rest of the field
			}
        }
    m_ios << '\n';  // Implicit newline at end of the field contents.
    m_bHaveWrittenField = TRUE;
}

void SF_ostream::WriteInternalField(const char* pszMarker,
        const char* pszContents)
{
    ASSERT( pszMarker );
    m_ios << "\\_" << pszMarker;
    ASSERT( pszContents );
    if ( *pszContents )
        m_ios << ' ' << pszContents;
    m_ios << '\n';
    m_bHaveWrittenField = TRUE;
}
