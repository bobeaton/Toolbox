// Str8.cpp String class for UTF8 and 8 bit legacy encoded data

// #include "stdafx.h"
#include "generic.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "Str8.h"

#undef GetChar

#ifndef ASSERT
#define ASSERT Str8Assert
#endif

BOOL Str8Assert( BOOL b )
	{
	return TRUE;
	}

void Str8Validate()
	{
	/*
	Str8 s1( "abc" );
	s1.AssertValid();
	Str8 s2 = "abc";
	s2.AssertValid();
	ASSERT( s1 == s2 );
	ASSERT( s1 == "abc" );
	ASSERT( "abc" == s1 );
	ASSERT( !(s1 != s2) );
	ASSERT( !(s1 != "abc") );
	ASSERT( !("abc" != s1) );
	Str8 s3( "a" );
	ASSERT( s3 == "a" );
	ASSERT( s3 == 'a' );
	ASSERT( s3 != 'b' );
	ASSERT( s3 != "ab" );
	ASSERT( s1 > s3 );
	ASSERT( s1 > "ab" );
	ASSERT( "bc" > s1 );
	ASSERT( s3 < s1 );
	ASSERT( "ab" < s1 );
	ASSERT( s1 < "bc" );
	ASSERT( s1 >= s3 );
	ASSERT( s1 >= "ab" );
	ASSERT( "bc" >= s1 );
	ASSERT( s3 <= s1 );
	ASSERT( "ab" <= s1 );
	ASSERT( s1 <= "bc" );
	ASSERT( s1 >= "abc" );
	ASSERT( s1 <= "abc" );
	s1 = "ab";
	s2 = s1 + s3;
	ASSERT( s2 == "aba" );
	s1.Append( "c" );
	ASSERT( s1 == "abc" );
	s2 = s1 + "d";
	s2.AssertValid();
	ASSERT( s2 == "abcd" );
	s2 = "e" + s2;
	ASSERT( s2 == "eabcd" );
	s2 = sTestCopy( "abcd" );
	s2.AssertValid();
	ASSERT( s2 == "abcd" );
	s2 += "e";
	ASSERT( s2 == "abcde" );
	s3 = "f";
	s2 += s3;
	ASSERT( s2 == "abcdef" );
	s1 = "ab";
	s3.Prepend( s1 );
	ASSERT( s3 == "abf" );
	s3.Prepend( "c" );
	ASSERT( s3 == "cabf" );
	s1 = "a";
	ASSERT( s1 == 'a' );
	s1 = s1 + 'z';
	ASSERT( s1 == "az" );
	ASSERT( 'y' + s1 == "yaz" );
	s1 = "a";
	ASSERT( s1 == 'a' );
	ASSERT( s1 != 'b' );
	s1 = "abcabcdef";
	ASSERT( s1.Find( 'a' ) == 0 );
	ASSERT( s1.Find( 'b' ) == 1 );
	ASSERT( s1.Find( 'x' ) == -1 );
	ASSERT( s1.Find( "a" ) == 0 );
	ASSERT( s1.Find( "bc" ) == 1 );
	ASSERT( s1.Find( "bx" ) == -1 );
	ASSERT( s1.Find( 'a', 1 ) == 3 );
	ASSERT( s1.Find( "bc", 4 ) == 4 );
	ASSERT( s1.Find( "bc", 5 ) == -1 );
	s2 = s1.Mid( 1, 2 );
	ASSERT( s2 == "bc" );
	ASSERT( s1.Mid( 0 ) == "abcabcdef" );
	ASSERT( s1.Mid( 1 ) == "bcabcdef" );
	ASSERT( s1.Mid( 2 ) == "cabcdef" );
	ASSERT( s1.Mid( 5 ) == "cdef" );
	ASSERT( s1.Mid( 8 ) == "f" );
	ASSERT( s1.Mid( 9 ) == "" );
	ASSERT( s1.Mid( 10 ) == "" );
	ASSERT( s1.Mid( 0, 0 ) == "" );
	ASSERT( s1.Mid( 0, 1 ) == "a" );
	ASSERT( s1.Mid( 0, 9 ) == "abcabcdef" );
	ASSERT( s1.Mid( 0, 10 ) == "abcabcdef" );
	ASSERT( s1.Mid( 1, 1 ) == "b" );
	ASSERT( s1.Mid( 3, 4 ) == "abcd" );
	ASSERT( s1.Mid( 4, 10 ) == "bcdef" );

	s1 = "abcabcdef";
	s1.Truncate( 3 );
	ASSERT( s1 == "abc" );

	s1 = "abcabcdef";
	s1.Replace( "abc", "xyz" );
	ASSERT( s1 == "xyzxyzdef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "xy" );
	ASSERT( s1 == "xyxydef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "xyzw" );
	ASSERT( s1 == "xyzwxyzwdef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "abc" );
	ASSERT( s1 == "abcabcdef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "abcd" );
	ASSERT( s1 == "abcdabcddef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "" );
	ASSERT( s1 == "def" );
	s1 = "abcabcdef";
	s1.Replace( "a", "" );
	ASSERT( s1 == "bcbcdef" );
	s1 = "abcabcdef";
	s1.Replace( "abc", "abc" );
	ASSERT( s1 == "abcabcdef" );
	s1 = "abcabcdef";
	s1.Replace( "a", "a" );
	ASSERT( s1 == "abcabcdef" );
	s1 = "abcabcdef";
	s1.Replace( "b", "" );
	ASSERT( s1 == "acacdef" );
	s1 = "abcabcdef";
	s1.Replace( "a", "x" );
	ASSERT( s1 == "xbcxbcdef" );
	s1 = "abcabcdef";
	s1.Replace( "", "xyz" );
	ASSERT( s1 == "abcabcdef" );

	ASSERT( s1[1] == 'b' ); // Not implemented, but works

	s1 = "abc";
	s1.SetAt( 5, 'z' );
	ASSERT( s1 == "abc" );
	s1.SetAt( 0, 'x' );
	ASSERT( s1 == "xbc" );
	s1 = "abc";
	s1.SetAt( 5, 'z' );
	ASSERT( s1 == "abc" );
	s1.SetAt( 2, 'z' );
	ASSERT( s1 == "abz" );

	s1 = " \nabc";
	s1.TrimLeft();
	ASSERT( s1 == "abc" );
	s1 = " \nabc \n";
	s1.TrimLeft();
	s1.TrimRight();
	ASSERT( s1 == "abc" );
	s1 = "";
	s1.TrimLeft();
	s1.TrimRight();
	ASSERT( s1 == "" );

	s1 = "abc";
	s1.Insert( 0, "z" );
	ASSERT( s1 == "zabc" );
	s1 = "abc";
	s1.Insert( 1, "xyz" );
	ASSERT( s1 == "axyzbc" );
	s1 = "abc";
	s1.Insert( 10, "z" );
	ASSERT( s1 == "abcz" );

	s1 = "abc";
	s1.Delete( 0, 0 );
	ASSERT( s1 == "abc" );
	s1 = "abc";
	s1.Delete( 1, 0 );
	ASSERT( s1 == "abc" );
	s1 = "abc";
	s1.Delete( 10, 0 );
	ASSERT( s1 == "abc" );
	s1 = "abc";
	s1.Delete( 0, 1 );
	ASSERT( s1 == "bc" );
	s1 = "abc";
	s1.Delete( 0, 2 );
	ASSERT( s1 == "c" );
	s1 = "abc";
	s1.Delete( 0, 3 );
	ASSERT( s1 == "" );
	s1 = "abc";
	s1.Delete( 1, 1 );
	ASSERT( s1 == "ac" );
	s1 = "abc";
	s1.Delete( 1, 2 );
	ASSERT( s1 == "a" );
	s1 = "abc";
	s1.Delete( 1, 3 );
	ASSERT( s1 == "a" );
	s1 = "abc";
	s1.Delete( 3, 0 );
	ASSERT( s1 == "abc" );
	s1 = "abc";
	s1.Delete( 3, 10 );
	ASSERT( s1 == "abc" );

	Str8 sFormat = "a%sc";
	s1 = "b";
	s2.Format( "a%sc", "b" );
	ASSERT( s2 == "abc" );
	s2.Format( "a%db%s", 1, "xyz" );
	ASSERT( s2 = "a1bxyz" );

	s1 = "a b";
	int i = 100;
	Str8 sWord;
	s1.bNextWord( sWord, i );
	i = 0;
	ASSERT( s1.bNextWord( sWord, i ) );
	ASSERT( sWord == "a" );
	ASSERT( s1.bNextWord( sWord, i ) );
	ASSERT( sWord == "b" );
	ASSERT( !s1.bNextWord( sWord, i ) );
	s1 = "\n abc.txt    1.23\n4.56\n   ";
	i = 0;
	ASSERT( s1.bNextWord( sWord, i ) );
	ASSERT( sWord == "abc.txt" );
	ASSERT( s1.bNextWord( sWord, i ) );
	ASSERT( sWord == "1.23" );
	ASSERT( s1.bNextWord( sWord, i ) );
	ASSERT( sWord == "4.56" );
	ASSERT( !s1.bNextWord( sWord, i ) );
	ASSERT( !s1.bNextWord( sWord, i ) );

	s1 = "ab";
	s1 += 'c';
	ASSERT( s1 == "abc" );

	AfxMessageBox( "Str8 validate done" );
	*/
	}

#define iStr8_Malloc_Increment 32 // 1.4ytb Change from 16 to 32 to make Str8 grow faster for higher speed

static BOOL bSpace( const char c ) // 1.4qzjh Fix U problem of jump not including all letters
	{
	return ( c == ' ' || c == '\n' );
	}

static int iMallocSize( int iSize )
	{
	int iMallocUnits = iSize / iStr8_Malloc_Increment + 1;
	return iMallocUnits * iStr8_Malloc_Increment;
	}

void Str8::Init()
	{
	int iMallocSiz = iMallocSize( 0 );
	m_psz = (char*)malloc( iMallocSiz ); // Allocate string buffer
	m_iAlloc = iMallocSiz; // Init allocated size
	m_iLen = 0; // Init length
	*m_psz = '\0'; // Terminate string
	}

void Str8::MakeSpace( int iSize ) // Make room for possibly larger size
	{
	if ( iSize + 1 > m_iAlloc ) // If new size larger than allocated, move to bigger space
		{
		char* pszOld = m_psz; // Remember old place
		int iMallocSiz = iMallocSize( iSize * 2 ); // 1.4ytb Double to make Str8 grow faster for higher speed
		m_psz = (char*)malloc( iMallocSiz ); // Allocate string buffer
		m_iAlloc = iMallocSiz; // Init allocated size
		strcpy( m_psz, pszOld ); // Copy old to new place
		free( pszOld ); // Free old place
		}
	}

Str8::Str8() // Default constructor // 1.4qzfv Start Str8
	{
	Init();
	AssertValid();
	}

Str8::Str8( const char* pszInit, int iCount ) // Constructor with initializing string
	{
	if ( !pszInit ) // 1.4qzpf Don't crash if null passed in as empty string
		pszInit = "";
	Init();
	int iLen = strlen( pszInit );
	MakeSpace( iLen ); // Init to large enough size
	strcpy( m_psz, pszInit ); // Copy initializing string to buffer
	m_iLen = iLen;
	if ( iCount >= 0 ) // If limited count, then shorten to that length
		Truncate( iCount );
	AssertValid();
	}

Str8::Str8( const char c ) // Constructor from char
	{
	Init(); // Init to large enough size
	MakeSpace( 1 );
	*m_psz = c;
	*(m_psz + 1) = '\0';
	m_iLen = 1;
	AssertValid();
	}

Str8::Str8( const Str8& s ) // Copy constructor
	{
	Init();
	MakeSpace( s.GetLength() );
	strcpy( m_psz, (const char*)s ); // Copy initializing string to buffer
	m_iLen = s.GetLength();
	AssertValid();
	}

Str8::~Str8() // Destructor
	{
	AssertValid();
	free( m_psz ); // Free the string buffer
	}

void Str8::AssertValid() const // Assert that all is well
	{
	ASSERT( m_iLen >= 0 ); // Length greater than zero
	ASSERT( m_iLen < m_iAlloc ); // Length less than alloc size (allows for terminating null)
	ASSERT( *(m_psz + m_iLen) == '\0' ); // Terminating null at length
	}
	
int Str8::GetLength() const // Length
	{
	AssertValid();
	return m_iLen;
	}

Str8::operator const char*() const  // Read access to buffer
	{ 
	AssertValid();
	return m_psz; 
	}

Str8& Str8::operator=( const char* pszSource )
{
	if ( !pszSource ) // 1.4qzpf Don't crash if null passed in as empty string
		pszSource = "";
	int iNewLen = strlen( pszSource );
	MakeSpace( iNewLen );
	m_iLen = iNewLen;
	strcpy( m_psz, pszSource );
	AssertValid();
	return *this;
}

Str8& Str8::operator=( const Str8& sSource )
{
	MakeSpace( sSource.GetLength() );
	m_iLen = sSource.GetLength();
	strcpy( m_psz, (const char*)sSource );
	AssertValid();
	return *this;
}

Str8& Str8::operator+=( const char* pszSource )
{
	if ( !pszSource ) // 1.4qzpf Don't crash if null passed in as empty string
		pszSource = "";
	Append( pszSource );
	AssertValid();
	return *this;
}

Str8& Str8::operator+=( const Str8& sSource )
{
	Append( sSource );
	AssertValid();
	return *this;
}

Str8& Str8::operator+=( const char c ) // 1.4qzkb Add Str8 += char to prevent crash from default behaviour
	{
	Str8 s( c );
	Append( s );
	AssertValid();
	return *this;
	}

Str8& Str8::operator +=( int iAdd ) // 1.4tec Add a number, if numeric
	{
	int i = atoi( *this );
	i += iAdd;
	char buffer[20];
	itoa( i, buffer, 10 );
//	this->Empty(); // 1.6.1ck Fix bug of bad write of settings
	this->Append( buffer );
	return *this;
	}

char* Str8::GetBuffer( int iSize ) // Write access to buffer
	{
	MakeSpace( iSize ); // Make sure enough space
	AssertValid();
	return m_psz;
	}

void Str8::ReleaseBuffer( int iLen ) // Release buffer after writing
	{
	ASSERT( iLen < m_iAlloc ); // Should have made large enough space at GetBuffer call
	if ( iLen >= 0 ) // If iLen given, null terminate string in case it was not terminated
		*(m_psz + iLen) = '\0';
	m_iLen = strlen( m_psz ); // Set correct length of string
	AssertValid();
	}

Str8& Str8::Append(const char* psz) // Append string
	{
	if ( !psz ) // 1.4qzpf Don't crash if null passed in as empty string
		psz = "";
	int iNewSize = m_iLen + strlen( psz ); // Calculate new length
	MakeSpace( iNewSize ); // Make enough space
	strcpy( m_psz + m_iLen, psz ); // Copy onto end
	m_iLen = iNewSize; // Set new length
	AssertValid();
	return *this;
	}

Str8& Str8::Prepend(const char* psz) // Prepend string
	{
	if ( !psz ) // 1.4qzpf Don't crash if null passed in as empty string
		psz = "";
	*this = psz + *this;
	AssertValid();
	return *this;
	}

int Str8::Find(const char* psz, int iStart) const
	{
	if ( !psz ) // 1.4qzpf Don't crash if null passed in as empty string
		psz = "";
	if ( iStart >= m_iLen )
		return -1;
	const char* pszFound = strstr( m_psz + iStart, psz );
	if ( !pszFound )
		return -1;
	else
		return pszFound - m_psz;
	}

int Str8::Find(const char c, int iStart) const
	{
	if ( iStart >= m_iLen )
		return -1;
	const char* pszFound = strchr( m_psz + iStart, c );
	if ( !pszFound )
		return -1;
	else
		return pszFound - m_psz;
	}

int Str8::FindAtEndOfWord(const char* psz, int iStart) const // 1.4ytc Add find at end of word (followed by sp or nl)
	{
	Str8 sFind = psz;
	Str8 sSp = sFind + " ";
	Str8 sNl = sFind + "\n";
	int iFindSp = Find( sSp, iStart ); // Find first with space
	int iFindNl = Find( sNl, iStart ); // Find first with nl
	if ( iFindSp < 0 ) // If none found with space, return find with nl (maybe no find of either)
		return iFindNl;
	if ( iFindNl < 0 ) // If none found with nl, return find with sp
		return iFindSp;
	if ( iFindSp < iFindNl ) // If both found, return earliest
		return iFindSp;
	else
		return iFindNl;
	}
	
int Str8::ReverseFind(const char c) const
	{
	const char* pszFound = strrchr( m_psz, c );
	if ( !pszFound )
		return -1;
	else
		return pszFound - m_psz;
	}

Str8 Str8::Mid( int iStart, int iCount ) const
	{
	if ( iStart < 0 ) // Protect against negative
		iStart = 0;
	if ( iStart > m_iLen ) // If start beyond end, start at end // ab 01
		iStart = m_iLen;
	if ( iCount < 0 || iCount > m_iLen - iStart ) // If count too many, do end
		iCount = m_iLen - iStart;
	Str8 s;
	char* psz = s.GetBuffer( iCount );
	strncpy( psz, m_psz + iStart, iCount );
	*(psz + iCount) = '\0';
	s.ReleaseBuffer();
	return s;
	}

void Str8::SetAt( int iPos, const char c )
	{
	if ( iPos < 0 || iPos > m_iLen )
		return;
	*(m_psz + iPos) = c;
	AssertValid();
	}

void Str8::Truncate( int iCount ) // Cut off end at iCount
	{ 
	if ( iCount < m_iLen ) // 1.4qzhx Fix U crash on jump (from Truncate)
		{
		m_iLen = iCount; 
		*(m_psz + iCount) = '\0'; 
		}
	}

void Str8::Replace( const char* pszFrom, const char* pszTo, BOOL bFeed ) // 1.4vyt 
	{
	int iFromLen = strlen( pszFrom );
	int iToLen = strlen( pszTo );
	if ( iFromLen == 0 )
		return;
	int iStart = 0;
	while ( TRUE )
		{
		int iFind = Find( pszFrom, iStart );
		if ( iFind == -1 )
			break;
		*this = Mid( 0, iFind ) + pszTo + Mid( iFind + iFromLen );
		iStart = iFind; // 1.4vyt 
		if ( !bFeed ) // 1.4vyt 
			iStart += iToLen; // 1.4vyt 
		}
	AssertValid();
	}

void Str8::TrimLeft()
	{
	while ( bSpace( *m_psz ) ) // 1.4qzjh
		*this = Mid( 1 );
	AssertValid();
	}

void Str8::TrimRight()
	{
	while ( m_iLen > 0 && bSpace( *( m_psz + m_iLen - 1 ) ) ) // 1.4qzjh
		Truncate( m_iLen - 1 );
	AssertValid();
	}

void Str8::Insert( int iPos, const char* psz )
	{
	if ( iPos < 0 )
		iPos = 0;
	if ( iPos > m_iLen )
		iPos = m_iLen;
	*this = Left( iPos ) + psz + Mid( iPos );
	AssertValid();
	}

void Str8::Delete( int iPos, int iCount )
	{
	if ( iPos < 0 )
		iPos = 0;
	if ( iCount > m_iLen )
		iCount = m_iLen;
	if ( iPos + iCount > m_iLen )
		iCount = m_iLen - iPos;
	*this = Left( iPos ) + Mid( iPos + iCount );
	AssertValid();
	}

void Str8::Format(const char* pszFormat, ...)
	{
	char* psz = GetBuffer( 5000 ); // Get plenty of space, no loss because this is probably a temp string	
    va_list argptr;
    va_start( argptr, pszFormat );
	vsprintf( psz, pszFormat, argptr );
    va_end(argptr);
	ReleaseBuffer();
	}

BOOL Str8::bNextWord( Str8& sWord, int& iPos ) // Find next space delimited word, return in sWord, return end of word in iPos
	{
	if ( iPos >= GetLength() ) // If iPos past end, return false
		return false;
	while ( iPos < GetLength() && bSpace( GetChar( iPos ) ) ) // Skip spaces // 1.4qzjh
		iPos++;
	if ( iPos == GetLength() ) // If no more non-spaces, return false
		return false;
	int iStart = iPos;
	while ( iPos < GetLength() && !bSpace( GetChar( iPos ) ) ) // Look for next space or end // 1.4qzjh
		iPos++;
	sWord = Mid( iStart, iPos - iStart ); // Collect word in return value
	return true;
	}

#define STR8BUFMAX 10000

BOOL Str8::bReadFile( Str8 sFileName ) // 1.4yta Add function to read file into Str8
	{
	char* pszStr8Buffer = (char*)malloc( STR8BUFMAX );
	FILE* pfil = fopen( sFileName, "r" );
	if ( !pfil )
		return FALSE;
	while ( fgets( pszStr8Buffer, STR8BUFMAX, pfil ) )
		Append( pszStr8Buffer );
	fclose( pfil );
	free( pszStr8Buffer );
	return TRUE;
	}

BOOL Str8::bGetSettingsTagSection( Str8& sTagSection, const char* pszTag, int& iStart, BOOL bIncludeTags ) // 1.4ytc Add functions to get tag section from file string // 1.5.0fe Add option to not include tags
	{
	Str8 sBeginTag = "\\+";
	sBeginTag += pszTag; // Make begin tag
	Str8 sEndTag = "\\-";
	sEndTag += pszTag; // Make end tag
	int iFindStart = FindAtEndOfWord( sBeginTag, iStart ); // Find begin tag
	if ( iFindStart < 0 ) // If not found, return false
		return FALSE;
	int iFindEnd = FindAtEndOfWord( sEndTag, iFindStart ); // Find end tag
	if ( iFindEnd < 0 ) // Should be found, but if not, set to end of string
		iFindEnd = GetLength();
	else
		iFindEnd += sEndTag.GetLength() + 1; // Set end to nl after end tag
	iStart = iFindEnd; // Update start of next find
	if ( !bIncludeTags ) // 1.5.0fe If not include tags, change begin and end to skip them
		{
		iFindStart += sBeginTag.GetLength(); // 1.5.0fe 
		iFindEnd -= sEndTag.GetLength() + 1; // 1.5.0fe 
		}
	sTagSection = Mid( iFindStart, iFindEnd - iFindStart ); // Collect tag section
	sTagSection.Trim(); // 1.5.0fe Get rid of any extra space and nl on end
	return TRUE; // Return success
	}

BOOL Str8::bGetSettingsTagContent( Str8& sTagContent, const char* pszTag, int iStart ) // 1.4ytc Add functions to get tag section from file string
	{
	Str8 sFullTag = "\\";
	sFullTag += pszTag; // Make full tag
	int iFindStart = FindAtEndOfWord( sFullTag, iStart ); // Find tag
	if ( iFindStart < 0 ) // If not found, return false
		return FALSE;
	int iFindEnd = Find( "\n", iFindStart ); // Find end of tag
	if ( iFindEnd < 0 ) // Should be found, but if not, set to length of tag
		iFindEnd = sFullTag.GetLength();
	iFindStart += sFullTag.GetLength(); // Don't include tag in content
	sTagContent = Mid( iFindStart, iFindEnd - iFindStart ); // Collect tag and content
	sTagContent.Trim(); // Trim off possible space or nl
	return TRUE; // Return success
	}

BOOL Str8::bDeleteSettingsTagSection( const char* pszTag ) // 1.5.0fe Add function to delete settings tag section from settings file string
	{
	Str8 sBeginTag = "\\+";
	sBeginTag += pszTag; // Make begin tag
	Str8 sEndTag = "\\-";
	sEndTag += pszTag; // Make end tag
	int iFindStart = FindAtEndOfWord( sBeginTag, 0 ); // Find begin tag
	if ( iFindStart < 0 ) // If not found, return false
		return FALSE;
	int iFindEnd = FindAtEndOfWord( sEndTag, iFindStart ); // Find end tag
	if ( iFindEnd < 0 ) // Should be found, but if not, set to end of string
		iFindEnd = GetLength();
	else
		iFindEnd += sEndTag.GetLength() + 1; // Set end to nl after end tag
	Delete( iFindStart, iFindEnd - iFindStart ); // 1.5.0fe 
	return TRUE; // Return success
	}

BOOL operator==( const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) == 0; }
BOOL operator==( const Str8& s, const char* psz ) { return strcmp( s, psz ) == 0; }
BOOL operator==( const char* psz, const Str8& s ) { return strcmp( s, psz ) == 0; }
BOOL operator==( const Str8& s, const char c )    { return strcmp( s, Str8( c ) ) == 0; }
BOOL operator!=( const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) != 0; }
BOOL operator!=( const Str8& s, const char* psz ) { return strcmp( s, psz ) != 0; }
BOOL operator!=( const char* psz, const Str8& s ) { return strcmp( s, psz ) != 0; }
BOOL operator!=( const Str8& s, const char c )    { return strcmp( s, Str8( c ) ) != 0; }
BOOL operator>(  const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) > 0; }
BOOL operator>(  const Str8& s, const char* psz ) { return strcmp( s, psz ) > 0; }
BOOL operator>(  const char* psz, const Str8& s ) { return strcmp( psz, s ) > 0; }
BOOL operator<(  const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) < 0; }
BOOL operator<(  const Str8& s, const char* psz ) { return strcmp( s, psz ) < 0; }
BOOL operator<(  const char* psz, const Str8& s ) { return strcmp( psz, s ) < 0; }
BOOL operator>=( const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) >= 0; }
BOOL operator>=( const Str8& s, const char* psz ) { return strcmp( s, psz ) >= 0; }
BOOL operator>=( const char* psz, const Str8& s ) { return strcmp( psz, s ) >= 0; }
BOOL operator<=( const Str8& s, const Str8& s2 )  { return strcmp( s, s2 ) <= 0; }
BOOL operator<=( const Str8& s, const char* psz ) { return strcmp( s, psz ) <= 0; }
BOOL operator<=( const char* psz, const Str8& s ) { return strcmp( psz, s ) <= 0; }

Str8 operator+(const Str8& s1, const Str8& s2)
{
	Str8 s( s1 );
	s.Append(s2);
	s.AssertValid();
	return s;
}

Str8 operator+(const Str8& s1, const char* psz)
{
	Str8 s( s1 );
	s.Append(psz);
	s.AssertValid();
	return s;
}

Str8 operator+(const char* psz, const Str8& s2)
{
	Str8 s( psz );
	s.Append(s2);
	s.AssertValid();
	return s;
}

Str8 operator+(const Str8& s1, const char c)
{
	Str8 s( s1 );
	s.Append( Str8( c ) );
	s.AssertValid();
	return s;
}

Str8 operator+(const char c, const Str8& s2)
{
	Str8 s( c );
	s.Append(s2);
	s.AssertValid();
	return s;
}

Str8 sTestCopy( const char* psz )
	{
	Str8 s = psz;
	return s;
	}
