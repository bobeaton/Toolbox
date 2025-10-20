//File: StdioFile.cpp
//Implementation of the CStdioFileEx class.
//See StdioFile.h for more information.

#include "stdafx.h"
#include "toolbox.h"
#include "StdioFil.h"

// Constructors, Destructors
CStdioFileEx::CStdioFileEx (LPCTSTR lpszPathName, UINT nOpenFlags ) : // 1.4qtw Upgrade CStdioFileEx for Unicode build
	CStdioFile ( lpszPathName, nOpenFlags)
{}

// Operations
BOOL CStdioFileEx::ReadString(Str8& sString)
{
	ASSERT_VALID(this);
    sString.Empty();
    const int nMaxSize = 128;
    char* lpsz = sString.GetBuffer(nMaxSize); // 1.4qzfq GetBuffer OK because immediately written into
    char* lpszResult;
    int nLen;
    for (;;)
		{
        lpszResult = fgets(lpsz, nMaxSize+1, m_pStream); // 1.4ha Change _fgetts so it won't mess up Unicode build
        sString.ReleaseBuffer();
        if (lpszResult == NULL && !feof(m_pStream)) // handle error/eof case
			{
            clearerr(m_pStream);
//            AfxThrowFileException(CFileException::generic, _doserrno); // 1.4qzgk Fix problem of _doserrno not defined in Unicode release build
			}
		nLen = sString.GetLength(); // 1.4qtx 
        if (lpszResult == NULL || nLen < nMaxSize || sString.GetChar( nLen - 1 ) == '\n' ) // 1.4qtx Upgrade lstrlen for Unicode build
            break; // if string is read completely or EOF, break
        nLen = sString.GetLength();
        lpsz = sString.GetBuffer(nMaxSize + nLen) + nLen; // 1.4qzfq GetBuffer OK because immediately written into
		}
    nLen = sString.GetLength();
	if ( nLen != 0 && sString.GetChar( nLen - 1 ) == '\n' ) // 1.4hb Replace use of set length to shorten string
		sString = sString.Left( nLen - 1 ); // remove '\n' from end of string if present
    return lpszResult != NULL;
}

UINT CStdioFileEx::Read(Str8& sBuf, UINT nCount)
{
	UINT uRetVal;
	char* pszBuf = sBuf.GetBuffer(nCount); // 1.4qzfq GetBuffer OK because written immediately
	uRetVal = Read((void FAR*) pszBuf, nCount);
	sBuf.ReleaseBuffer(uRetVal);
	return uRetVal;
}
