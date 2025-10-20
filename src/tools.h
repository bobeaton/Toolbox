// tools.h -  various "helper" functions
// 1994-11-29  Rod Early

#ifndef TOOLS_H
#define TOOLS_H

void SendParallelMoveRefExternal( Str8 sFocus ); // 1.4kr

BOOL bValidFldLen( int iLen ); // return TRUE if iLen is a valid field length

void InitTimeLimit( int iTimeLimit, BOOL bMessage = TRUE ); // 1.3bh Make separate timer init function // 1.4qzjm Add argument to prevent message for interlinear check
BOOL bTimeLimitExceeded(); // Return true if time limit has been exceeded // 1.3bh Make time limit test a separate function
void WaitSeconds( int iSeconds ); // 1.4kzc Wait for a number of seconds

void RemoveExtraSpaces( Str8& s ); // Remove all multiple spaces and trailing spaces to prepare for comparison // 1.3br move to tools

// Shorthand for error message box (OK) built from string resource and one argument
// void AfxMessageBoxOK1( CString s, const char* lpsz1 ); // 1.3bd Bring in string resources

#ifdef _MAC
Str8 sGetFullPath( const char* pszFormattedBuffer ); // convert formatted buffer to full pathname
Str8 sGetFormattedBuffer( const char* pszFullPath ); // convert full pathname to formatted buffer suitable for WPL functions
void SetFileTypeRTF( const char* pszFullPath ); // set file to type 'RTF ' and creator 'MSWD'
void SetFileTypeSF( const char* pszFullPath ); // set file to type 'TEXT' and creator '????'
void SetDocFileType( const char* pszFullPath ); // set file to type 'TEXT' and creator 'SHBX'
void SetProjectFileType( const char* pszFullPath ); // set file to type 'SHBX' and creator 'SHBX'
void XferFileLabel( const char* pszFromPath, const char* pszToPath); // get/set Mac file label attribute
void MacSplitPath( const char* pszSpec, char* pszPath, char* pszFName, char* pszExt ); // Mac equivalent (more or less) to _splitpath()
#endif

Str8 sChangeFileExt(const char* pszFile, const char* pszExt);
Str8 sGetFileName(const char* pszFile, BOOL bIncludeExt=TRUE);
Str8 sGetFileExt(const char* pszFile);
Str8 sGetDirPath(const char* pszFile); // Returns drive+folder portion of the given path
Str8 sGetDrive(const char* pszFile);

// sPath() functions return a full path built from the components provided
Str8 sPath(const char* pszDirPath, const char* pszName, const char* pszExt);
Str8 sPath(const char* pszDirPath, const char* pszNameExt);

BOOL bXMLFileType( Str8 sPath ); // 1.5.9ge Return true if XML file type xml or xhtml

void RemoveExtraSeparatorInFolderPath(Str8& sFolderPath);
    // 1999-03-11 MRP: If there is an extra separator at the end
    // of a folder's path, remove it (e.g. C:\Program Files\Shoebox\MDF\).

BOOL bEquivFilePath(const char* psz1, const char* psz2); 	// T if the file name/path strings are equivalent

void Path1632DLL(Str8& sPathDLL);
    // Return the path to the 16-bit or 32-bit DLL file appropriate for
    // this instance of Shoebox, no matter which file path appears
    // in the setting file property.
    // DLL paths have names whose common part is up to six characters long,
    // ending with "16" for SHW and "32" for SHW32.
    // For example, you might name non-Roman Script sorting modules for the
    // Devnagari Language Encoding DEVNAG16.DLL and DEVNAG32.DLL.

Str8 sOnlyAlphanumeric( Str8 s ); // 1.4vzc Make a string only alphanumeric
BOOL bAllASCII( Str8 s ); // 1.4vzc Check string for all ASCII chars
BOOL bAllASCIIComplain( Str8 sPath ); // 1.4vze Check string for all ASCII chars, complain and return false if not all

Str8 sUniqueFileName(const char* pszDirPath, const char* pszName,
        const char* pszExt, BOOL bIncludeExt=FALSE, BOOL bLongFN = TRUE, // 1.4qc Allow long file names for typ and lng files (change default to TRUE)
        BOOL bAllChars = FALSE);
    // Returns a valid file name derived from an object's name pszName,
    // which does not already exist in directory pszDirPath
    // among files with the extension pszExt.
    // bLongFN could be set if longer-than-8.3 file names are permitted
    // bAllChars could be set if pszName is already a valid file name

LONG lFileSize( Str8 sFile ); // 1.5.9mj Change to Str8
BOOL bFileExists( Str8 sFile ); // 1.5.9mj Change to Str8
BOOL bFileReadOnly( Str8 sPath ); // 1.5.9mf Test file for read only // 1.5.9mj Change to Str8

// The following are edit utility functions used mostly in SHWVDIS
//------------------------------------------------------
inline const char* pszNextLineBrk( const char* psz ) // Find next line break position, return end of string if no CR
{
    const char* pszBrk = strchr( psz, '\n' ); // If CR found, return it 0.2.A1 
    if ( !pszBrk ) // If no CR found, return end of string
        pszBrk = psz + strlen( psz );
    return pszBrk;  
}
//------------------------------------------------------
inline const char* pszPrevLineBrk( const char* pszStart, const char* psz ) // Find next line break position
{                               
ASSERT( psz >= pszStart );
    for ( ; psz > pszStart; psz-- ) 
        if ( *(psz - 1) == '\n' ) // If CR found, return char after it 
            return psz;
    return psz; // If no CR found, return front of string
}

LONG lUTF8ToUTF32( const char* psz, int& iNumBytes ); // 1.4eg Add Unicode sequence sort
Str8 sUTF32ToUTF8( UINT lUTF32 );

BOOL bIsLocaleMetric();
BOOL bIsLocaleEnglish();
void LocaleFormatTime (CTime& timToFormat, Str8& sRetVal);
void LocaleFormatDate (CTime& timToFormat, Str8& sRetVal, 
                       BOOL bLongDate = FALSE);
BOOL bWinExecWait(const char* szCmdLine, UINT nCmdShow, BOOL bWait = TRUE);

void TrimBothEnds(  Str8& s ); // 1.4hdc Trim both ends of string
void RemoveBackslashAtFront( Str8& s ); // 1.4hdj Trim and remove any backslash at front

int FindAfter( Str8 s, Str8 sFind, int iStart ); // 1.4hen Make Find with start pos, not in Str8

Str8 sGetNextRef( Str8 s, int& iStart ); // 1.4qng Get next reference from a string of refs
BOOL bFindRef( Str8 s, Str8 sRef ); // 1.4qng Find a reference in a string of references

Str8 sGetClipBoardData( CWnd* w, BOOL bUnicode );  // 1.4rbc Encapsulate get clipboard data

void WriteProtect( const char* pszPath ); // 1.4qzhg Write protect a file
void UnWriteProtect( const char* pszPath ); // 1.4qzhg Clear write protect of a file

void AfxFormatString1( Str8& s, LPCSTR pszFormat, LPCSTR pszArg1 ); // 1.3ag Make string form of AfxFormatStrng
void AfxFormatString2( Str8& s, LPCSTR pszFormat, LPCSTR pszArg1, LPCSTR pszArg2 );
#ifdef _UNICODE
int AfxMessageBox( const char* psz, UINT nType = MB_OK ); // 1.4qpq Make message box for Unicode
#endif

Str8 sUTF8 ( CString swUTF16 ); // 1.4qpt Make string conversion from UTF16 to UTF8 and back
CString swUTF16( Str8 sUTF8 ); // 1.4qpt Make string conversion from UTF16 to UTF8 and back
CString swUTF16( const char* psz ); // 1.4qxj Make string conversion from char* directly to UTF16

void SetRegPossibleCrash(); // 1.5.8wa Set registry to watch for possible crash
void ClearRegPossibleCrash(); // 1.5.8wa Clear registry to watch for possible crash
int iDlgDoModalSetReg( CFileDialog* pdlg ); // 1.5.8wa Do dialog DoModal with reg set and clear for crash detect
BOOL bRegPossibleCrash(); // 1.5.8wa Read registry to see if possible crash
BOOL bReadFileIntoString( Str8 sPath, Str8& sInBuff ); // 1.5.9pd Read file into string
BOOL bWriteFileFromString( Str8 sPath, Str8 sOut ); // 1.5.9pd Write file from string

FILE* pfFileOpenWrite( const char* pszPath ); // 1.6.4ad Open FILE for write

#endif  // TOOLS_H
