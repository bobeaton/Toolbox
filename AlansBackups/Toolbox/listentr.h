//File: ListEntr.h
//Class definition for the CWordInfo class.
//This file stores information on a word in the word list.  It stores the
//word count and a list of references to that word (in one big binary-encoded
//string).  Note that the word itself is not stored in this class.  Instead,
//CWordInfo is designed to work in close conjunction with CWordList, a
//derivative of CMapStringToOb that associates a word (stored as a Str8) 
//with the related info.
//By Josh Kelley, 6/97

#ifndef _LISTENTR_H
#define _LISTENTR_H

#include "hugestr.h"

class CWordInfo : public CObject {
public:
	CWordInfo();
	CWordInfo(unsigned uNewFile, unsigned long ulNewOffset, 
              int nNewBook, unsigned uNewChapter, unsigned uNewVerse);
	CWordInfo(long lNewCount, CHugeString& chsNewFileRefs, 
		      CHugeString& chsNewBCVRefs);
	void SetAt(long lNewCount, CHugeString& chsNewFileRefs, 
		       CHugeString& chsNewBCVRefs);
	long lGetCount() const;
	const CHugeString& hsGetFileRefs();
	const CHugeString& hsGetBCVRefs();
	long lAddReference(unsigned uNewFile, unsigned long ulNewOffset, 
		              int nNewBook, unsigned uNewChapter,			//3/24/98
                      unsigned uNewVerse);
	void RemoveReferences(unsigned uFileToRemove);
	CWordInfo& operator= (CWordInfo& cwiNewInfo);
	void Clear();


// these static members, plus the lAddReference function, define the handling
//  of BCV references stored together in a CHugeString
	static const unsigned s_nBookRefMax;	//ref max values // 1.4ca Raise limit of wordlist and conc refs to 65500
	static const unsigned s_uChapterRefMax;
	static const unsigned s_uVerseRefMax;
	static const int      s_nBCVRefNBytes;	//num of bytes for each BCV
	static inline unsigned s_nGetBookCode (const CHugeString& hsBCVRefs, DWORD dwIndex)
		// dwIndex is the offset in the hugestring at which the BCV ref begins
		{return hsBCVRefs.wGetWord(dwIndex);}
	static inline unsigned s_uGetChapter (const CHugeString& hsBCVRefs, DWORD dwIndex)
		// dwIndex is the offset in the hugestring at which the BCV ref begins
		{return hsBCVRefs.wGetWord(dwIndex+2);}
	static inline unsigned s_uGetVerse (const CHugeString& hsBCVRefs, DWORD dwIndex)
		// dwIndex is the offset in the hugestring at which the BCV ref begins
		{return hsBCVRefs[dwIndex+4];}


protected:
	void SaveRefs();
	BOOL bGetFileBlockInfo (WORD wFileNum, DWORD& dwStart, 
                           DWORD& dwRefBefore);
	long m_lCount;
    long m_lFileCount;
	CHugeString m_hsFileRefs;
    CHugeString m_hsBCVRefs;
	CHugeString m_hsCurFileRefs;
    CHugeString m_hsCurBCVRefs;
	unsigned m_uFile;
};

#endif
