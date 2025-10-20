// Dirlist.h  Interface for directory search list

#ifndef Dirlist_H
#define Dirlist_H

#include "cdbllist.h"

/////////////////////////////////////////////////////////////////////////////
// CFileListEl

class CFileListEl : public CDblListEl  // Hungarian: fle
{
private:
	Str8 m_sPath; // file pathname of file

    friend class CFileList;
        
    CFileListEl(const char* pszPath)
    	{ m_sPath = pszPath; }
    	
public:
    const char* pszPath() { return m_sPath; } // return text used for this find
};  // class CFileListEl


/////////////////////////////////////////////////////////////////////////////
// CFileList

class CFileList : public CDblList  // Hungarian: flst
{
public:
	CFileList( const char* pszDirPath, const char* pszSpec, BOOL bDirsOnly = FALSE ); // Constructor

    CFileListEl* pfleFirst() const { return (CFileListEl*)pelFirst(); }
    CFileListEl* pfleNext( const CFileListEl* pfle) const { return (CFileListEl*)pelNext(pfle); }
    CFileListEl* pflePrev( const CFileListEl* pfle) const { return (CFileListEl*)pelPrev(pfle); }
    CFileListEl* pfleLast() const  { return (CFileListEl*)pelLast(); }    

private:
	void Add( const char* pszPath ) // add a filename to list
		{ CDblList::Add( new CFileListEl( pszPath ) ); }

	void AddFilesWithExt( const char* pszDirPath, const char* pszExt, BOOL bDirsOnly ); // fill list with files with specified extension in specified directory
};  // class CFileList

#endif  // not Dirlist_H
