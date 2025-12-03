// Dirlist.cpp  Implementation of directory search list  BJY 4/30/96

#ifdef _MAC
#include <files.h>
#endif

#include "stdafx.h"
#include "toolbox.h"
#include "dirlist.h"
#include "tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileList

CFileList::CFileList( const char* pszDirPath, const char* pszExt, BOOL bDirsOnly)
{
	AddFilesWithExt( pszDirPath, pszExt, bDirsOnly );
}

#ifndef _MAC
// Win95, NT version
void CFileList::AddFilesWithExt( const char* pszDirPath, const char* pszExt, BOOL bDirsOnly )
{
    ASSERT( pszExt );
    BOOL bLiteralExt = ( *pszExt && strcmp(pszExt, ".*") != 0 );
    size_t lenExt = strlen(pszExt);
	Str8 sWildcardPath = sPath(pszDirPath, "*", pszExt);

	WIN32_FIND_DATA wfd;
	HANDLE hFile = FindFirstFile( swUTF16( sWildcardPath ), &wfd ); // 1.4qxy Upgrade FindFirstFile for Unicode build
	BOOL bFound = ( hFile != INVALID_HANDLE_VALUE );
	for ( ; bFound; bFound = FindNextFile( hFile, &wfd ) )
        if (bDirsOnly == ((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
            {
//		    const char* pszFileNameExt = wfd.cFileName; // 1.4qxz
			CString swFileNameExt = wfd.cFileName; // 1.4qxz Upgrade AddFilesWithExt for Unicode build
			Str8 sFileNameExt =  sUTF8( swFileNameExt ); // 1.4qxz
            if ( bLiteralExt )
                {
                // 1998-06-16 MRP: Reject partial match on a literal extension,
                // e.g. filename.lngx mustn't match *.lng.
                size_t len = strlen( sFileNameExt );  // 1.4qxz
                if ( len <= lenExt )
                    continue;

                const char* pszFileExt = (const char*)sFileNameExt + len - lenExt; // 1.4qxz
                ASSERT( (const char*)sFileNameExt < pszFileExt ); // 1.4qxz
                if ( stricmp(pszFileExt, pszExt) != 0 )
                    continue;
                }
		    
            Str8 sPathFound = sPath(pszDirPath, sFileNameExt); // 1.4qxz
		    Add( sPathFound ); // add to list
    		}

	if ( hFile != INVALID_HANDLE_VALUE )
		FindClose( hFile );
}
#endif // Win95, NT version


#ifdef _MAC
// Mac version
void CFileList::AddFilesWithExt( const char* pszDirPath, const char* pszExt, BOOL bDirsOnly )
{
    int iErr;
    long l;
    
	// the following code gets the directory id for the settings directory
	Str8 sDirPath = pszDirPath;
	if ( sDirPath[ sDirPath.GetLength() - 1 ] != ':' )
		sDirPath += ":";
	sDirPath += "a"; // fake filename
	ASSERT( sDirPath.GetLength() < 255 );
	_c2pstr( sDirPath.GetBuffer(sDirPath.GetLength()+1) );
	sDirPath.ReleaseBuffer(-1);
    struct FSSpec fss;
	const char* pszPath = sDirPath;
    iErr = FSMakeFSSpec( 0, 0, (unsigned char*)pszPath, &fss ); // get directory id by making an FSSpec for non-existant file in settings directory
    int iDirID = fss.parID;

    short vrefNum;
	short iVolSave;
	iErr = HGetVol( NULL, &iVolSave, &l ); // get current volume
	const char* p = strchr( pszDirPath, ':' ); // chop off everything past volume name
	if ( p )
		sDirPath = Str8(pszDirPath, p+1 - pszDirPath);
	else
		sDirPath = Str8(pszDirPath) + ":";
	int iLen = sDirPath.GetLength();
	_c2pstr( sDirPath.GetBuffer(iLen+1) );
	sDirPath.ReleaseBuffer(iLen+1);
	const char* pszVol = sDirPath;
	iErr = SetVol( (const unsigned char*)pszVol, 0 ); // set default volume to volume specified in settings path
	iErr = HGetVol( NULL, &vrefNum, &l ); // get current volume
    ASSERT( !iErr );

	CInfoPBRec cipbr; // local pb
    memset( &cipbr, 0, sizeof(cipbr) ); // clear structure
	HFileInfo *fpb = (HFileInfo *)&cipbr;
	Str255 filename;

	fpb->ioVRefNum = vrefNum;
	fpb->ioNamePtr = filename; // buffer to receive name

	for ( int i=1; TRUE; i++ ) // indexing loop
		{
		fpb->ioDirID = iDirID; // must set on each loop
		fpb->ioFDirIndex = i;

		if ( PBGetCatInfo( &cipbr, FALSE ) )
			break;  // exit when no more entries

		if (bDirsOnly == ((fpb->ioFlAttrib & 16) != 0)) // make sure it's of the desired type
			{
			int iLen = *(char*)filename; // pick up string length
			_p2cstr( filename ); // convert to C string
			int iExtLen = strlen( pszExt );
			if ( iLen > iExtLen && !stricmp( (char*)filename+iLen-iExtLen, pszExt ) )
				{ // file has extension matching pszExt
				Str8 sFileFound = sPath( pszDirPath, (char*)filename );
				Add( sFileFound ); // add to list
				}
			}
		}

    SetVol( NULL, iVolSave ); // restore default volume
}
#endif // Mac version
