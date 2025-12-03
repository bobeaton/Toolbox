//File: HugeString.h
//Declaration of the CHugeString class.
//In Win 3.x, the CStrg class is limited in size to 32k.  The WordList needs
//bigger strings than that.  The CHugeString class contains a subset of
//CStrg's functionality and can handle strings up to 4GB in size.  Note
//that CHugeString does not work under Windows' real mode.
//Josh Kelley, 6/97

//09-25-1997 Changed all variables and functions into hungarian 
//notation (including .cpp File)

#ifndef _HUGESTR_H
#define _HUGESTR_H

// On a 16-bit machine, these should be defined as "__huge"
// On a 32-bit machine, they should be defined as "FAR"
#define BIGPTR  unsigned char FAR*
#define BIGCPTR char FAR*

class CHugeString : public CObject
{
public:
//Constructors/destructors
	CHugeString();
	CHugeString(CHugeString& chsOther);
	CHugeString(const char* lpsz);
	CHugeString(const char* lpch, DWORD dwLen);
	~CHugeString();
//Attributes
	BOOL bIsEmpty() const;
	DWORD dwGetLength() const;
	int iGetSubString (Str8& csTarget, DWORD dwOffset, int nLength) const;
	DWORD dwGetSubString (CHugeString& chsTarget, DWORD dwOffset, DWORD dwLength) const;
	unsigned char operator[] (DWORD dwOffset) const;
	WORD wGetWord (DWORD dwOffset) const;
	DWORD dwGetDWord (DWORD dwOffset) const;
//Operations
	void Empty();
	CHugeString& operator= (const CHugeString& chsOther);
	CHugeString& operator= (const char* lpszOther);
	CHugeString& operator+= (const CHugeString& chsOther);
	CHugeString& operator+= (const char* lpszOther);
	CHugeString& operator+= (char cOther);
	void SetWord (DWORD dwOffset, WORD wValue);
	void SetDWord (DWORD dwOffset, DWORD dwValue);
	void AppendWord (WORD wValue);
	void AppendDWord (DWORD dwValue);
	void RemoveSubString (DWORD dwOffset, DWORD dwLength);
	void InsertSubString (const Str8 &csNewValue, DWORD dwOffset);
	void InsertSubString (const CHugeString &chsNewValue, DWORD dwOffset);
	void ReplaceSubString (const Str8 &csNewValue, DWORD dwOffset, 
		                   DWORD nLength);
	void WriteToFile (CFile& cfOutput) const;
	void ReadFromFile (CFile& cfInput, DWORD dwLength);
//Implementation
protected:
	void *LockMem(DWORD dwMinSize);
	const void *LockMem() const;
	void UnlockMem() const;
	void UnlockMem(DWORD dwNewSize);
	void AllocMem (DWORD dwNewSize);
	void ReAllocMem (DWORD dwNewSize);
	void FreeMem();
	DWORD m_dwDataLen;
    DWORD m_dwAllocLen;
	HGLOBAL m_hData;
	void FAR *m_pLockedData;
};

inline BOOL CHugeString::bIsEmpty() const {
	return (m_dwDataLen == 0);
}
inline DWORD CHugeString::dwGetLength() const {
	return m_dwDataLen;
}

#endif
