//File: ListEntr.cpp
//Implementation of the CWordInfo class.
//By Josh Kelley, 6/97
// 01/2000 BDW enabled 16bit pri & sec references
//There's some byte-size specific code here.  Be careful.

#include "stdafx.h"
#include "toolbox.h"
#include "ListEntr.h"

// initialize public static members at file scope
const unsigned CWordInfo::s_nBookRefMax     = 65500; // 1.4ca Raise limit of wordlist and conc refs to 65500
const unsigned CWordInfo::s_uChapterRefMax  = 65500;
const unsigned CWordInfo::s_uVerseRefMax    =   255;
const int      CWordInfo::s_nBCVRefNBytes   =     5;

// Constructor.
CWordInfo::CWordInfo() : CObject()
{
	m_lCount = m_lFileCount = 0;
	m_uFile = 0;
}

// Constructor.  Creates a CWordInfo and automatically adds its first reference.
CWordInfo::CWordInfo (unsigned uNewFile, unsigned long ulNewOffset,
	int nNewBook, unsigned uNewChapter, unsigned uNewVerse) 
	: CObject()
{
	m_lCount = m_lFileCount = 0;
	m_uFile = 0;
	lAddReference (uNewFile, ulNewOffset, nNewBook, uNewChapter, uNewVerse);
}

CWordInfo::CWordInfo (long lNewCount, CHugeString& chsNewFileRefs, 
	CHugeString& chsNewBCVRefs)
{
	SetAt (lNewCount, chsNewFileRefs, chsNewBCVRefs);
}

// Assigns a completely different set of data to a CWordInfo.
void CWordInfo::SetAt (long lNewCount, CHugeString& chsNewFileRefs, 
					   CHugeString& chsNewBCVRefs)
{
	m_lCount = lNewCount;
	m_hsFileRefs = chsNewFileRefs;
	m_hsBCVRefs = chsNewBCVRefs;
	m_uFile = 0;
	m_lFileCount = 0;
	m_hsCurFileRefs.Empty();
	m_hsCurBCVRefs.Empty();
}

// Data access.
long CWordInfo::lGetCount () const
{
	return m_lCount;
}

// Data access.
const CHugeString& CWordInfo::hsGetFileRefs()
{
	SaveRefs();
	return m_hsFileRefs;
}

// Data access.
const CHugeString& CWordInfo::hsGetBCVRefs()
{
	SaveRefs();
	return m_hsBCVRefs;
}

// Increments the word count and adds the new reference given by 
// uNewFile:ulNewOffset and nNewBook uNewChapter:uNewVerse.  nNewBook is a
// book code whose corresponding book name is given in a CBookList somewhere.
//NOTE:
// This code assumes that all references for any one file will be added as
// one block (no switching back and forth between files, and no interruptions
// by calling other non-const CWordInfo functions).
long CWordInfo::lAddReference(unsigned uNewFile, unsigned long ulNewOffset,
	int nNewBook, unsigned uNewChapter, unsigned uNewVerse)
{
	// Perform new file handling as needed.
	if (uNewFile != m_uFile) 
		{
		SaveRefs();
		// Save space for the new file's header in m_hsCurFileRefs.
		m_hsCurFileRefs.AppendWord ((WORD) uNewFile);
		m_hsCurFileRefs.AppendDWord (0);
		// Save the new file's number.
		m_uFile = uNewFile;
		}

	// Add the file references.
	m_hsCurFileRefs.AppendDWord (ulNewOffset);
	m_lFileCount++;
	
	// Add the chapter/verse references.
	ASSERT (nNewBook>=0 && nNewBook<=s_nBookRefMax);
	ASSERT (uNewChapter <= s_uChapterRefMax);
	ASSERT (uNewVerse <= s_uVerseRefMax);
	ASSERT (s_nBCVRefNBytes == 5);
	m_hsCurBCVRefs.AppendWord ((WORD)nNewBook);	// 01/2000
	m_hsCurBCVRefs.AppendWord ((WORD)uNewChapter);
	m_hsCurBCVRefs += LOBYTE(uNewVerse);
	
	// Update the ref count and return.
	return ++m_lCount;
}

// Removes all references to a particular file and updates the word count
// accordingly.  Used before updating a file, to remove old references
// relating to that file.
void CWordInfo::RemoveReferences (unsigned m_uFileToRemove)
{
	DWORD dwRemStart = 0, dwRemStop;  //Starting and stopping byte offsets to
		// remove from m_hsFileRefs
	DWORD dwRefBefore = 0, dwRefIn;  //Number of references before the block to
		// be removed.  Number of references in a particular block
		// (eventually, the block to be removed).
	SaveRefs();
	// Locate the section of file references to remove.
	if (!bGetFileBlockInfo(m_uFileToRemove, dwRemStart, dwRefBefore))
		return;  // Nothing to remove.
	dwRefIn = m_hsFileRefs.dwGetDWord ((DWORD) dwRemStart + 2);
	dwRemStop = dwRemStart + dwRefIn * 4 + 6;
	// Perform the removal for m_hsFileRefs and m_hsBCVRefs.  Update the word count.
	m_hsFileRefs.RemoveSubString (dwRemStart, dwRemStop - dwRemStart);
	m_hsBCVRefs.RemoveSubString (dwRefBefore * s_nBCVRefNBytes, 
								      dwRefIn * s_nBCVRefNBytes); 
	m_lCount -= dwRefIn;
}

// Copies one CWordInfo to another.
CWordInfo& CWordInfo::operator= (CWordInfo& cwiNewInfo)
{
	m_lCount = cwiNewInfo.m_lCount;
	m_uFile = cwiNewInfo.m_uFile;
	m_hsFileRefs = cwiNewInfo.m_hsFileRefs;
	m_hsBCVRefs = cwiNewInfo.m_hsBCVRefs;
	m_hsCurFileRefs = cwiNewInfo.m_hsCurFileRefs;
	m_hsCurBCVRefs = cwiNewInfo.m_hsCurBCVRefs;
	return *this;
}

// Set the word count to 0 and erase the list of references.
void CWordInfo::Clear ()
{
	m_lCount = 0;
	m_uFile = 0;
	m_hsFileRefs.Empty();
	m_hsBCVRefs.Empty();
	m_hsCurFileRefs.Empty();
	m_hsCurBCVRefs.Empty();
}

// Saves all references for the current file (assuming we have a current file)
// into the main reference lists.  Further lAddReference calls relating to this
// file would then cause problems - be careful.
void CWordInfo::SaveRefs()
{
	if (m_uFile != 0) {
		DWORD dwInsertAt, dwRefBefore;
		bGetFileBlockInfo (m_uFile, dwInsertAt, dwRefBefore);
		m_hsCurFileRefs.SetDWord(2, m_lFileCount);
		m_hsFileRefs.InsertSubString(m_hsCurFileRefs, dwInsertAt);
		m_hsBCVRefs.InsertSubString(m_hsCurBCVRefs, dwRefBefore * s_nBCVRefNBytes);
		m_uFile = 0;
		m_lFileCount = 0;
		m_hsCurFileRefs.Empty();
		m_hsCurBCVRefs.Empty();
	}
}
                       
// Returns information on the requested "block" (series of references from one
// file) in the m_hsFileRefs.  
//  wFileNum is the file to retrieve info for
//  dwStart, on exit, contains the offset of the first byte of the block found
//								in m_hsFileRefs
//  dwRefBefore, on exit, contains the number of references before the
//								block found.  
// If bGetFileBlockInfo finds the block of references for
// wFileNum, it returns TRUE.  Otherwise, it returns FALSE and dwStart
// and dwRefBefore contain information about the block after where wFileNum 
// would be.
BOOL CWordInfo::bGetFileBlockInfo (WORD wFileNum, DWORD& dwStart, 
	DWORD& dwRefBefore)
{
	DWORD dwStringLen = m_hsFileRefs.dwGetLength(), dwRefIn;
	dwStart = dwRefBefore = 0;
	while ((dwStart < dwStringLen) && 
			(m_hsFileRefs.wGetWord(dwStart) < wFileNum)) {
		dwRefIn = m_hsFileRefs.dwGetDWord(dwStart+2);
		dwRefBefore += dwRefIn;
		dwStart += dwRefIn * 4 + 6;
	}
	if ((dwStart < dwStringLen) && (m_hsFileRefs.wGetWord(dwStart) == wFileNum))
		return TRUE;
	else
		return FALSE;
}
