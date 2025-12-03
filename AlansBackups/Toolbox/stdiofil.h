//File: StdioFile.h
//Definition for the CStdioFileEx class.
//MFC 4.0's CStdioFile has a bug in its ReadString (Str8&) function.  This
//class is derived from CStdioFile and overrides the buggy function with a
//functional function.  Microsoft has confirmed that this bug exists in 4.0,
//and they say it was fixed in 4.1.  The ReadString function doesn't even
//exist in the 16-bit version, but it's a useful function, so I use it in
//16-bit code also.  I also added Read (Str8,UINT)
// Josh Kelly  July 1997
// Added GetPathName() and GetFileName(), which are missing from CFile in the
//   16-bit version
// B Wussow  May 2000

#ifndef _STDIOFIL_H
#define _STDIOFIL_H

class CStdioFileEx : public CStdioFile
{
public:
// Constructors, Destructors
	CStdioFileEx() {}
	//CStdioFileEx(FILE* pOpenStream); //not supported
	CStdioFileEx(LPCTSTR lpszFileName, UINT nOpenFlags); // 1.4qtw Upgrade CStdioFileEx for Unicode build
// Operations
	BOOL ReadString(Str8& rString);	// Overridden to fix bugs
	UINT Read(Str8& sBuf, UINT nCount);	// New member added
	UINT Read(void FAR* lpBuf, UINT nCount);
//Attributes

//Member Variables
};



inline UINT CStdioFileEx::Read(void FAR* lpBuf, UINT nCount)
// (Read (lpBuf,cnt) is redeclared here because overriding it confuses the compiler
	// otherwise.)
{
	return CStdioFile::Read(lpBuf, nCount); 
}

#endif
