//File: HugeString.cpp
//Implementation for the CHugeString class.
//See HugeStr.h for further details.
//Joshua Kelley, 6/97

#include "stdafx.h"
#include "toolbox.h"
#include "HugeStr.h"

#include <string.h>

const DWORD dwHugeThreshold = 65500;  // Requests for memory at or below this size
	// are fulfilled using malloc-style functions.  Requests for memory above
	// this size are fulfilled using GlobalAlloc-style functions.  (VC++
	// doesn't like it if you get too close to 65535.  I do not know why.)
const int dwReallocLen = 128;  // Amount of additional space to allocate each time
	// a reallocate is needed.

// Helper function.  Copies a block of data up to 4GB in size from lpSrc to
// lpDst.
void HugeMemCopy (void FAR *lpDst, const void FAR *lpSrc, DWORD dwLen)
{
	ASSERT(lpDst);
	ASSERT(lpSrc);
	while (dwLen--) 
        {
		*(BIGPTR)lpDst = *(BIGPTR)lpSrc;
		lpDst = (BIGPTR) lpDst + 1;
		lpSrc = (BIGPTR) lpSrc + 1;
	    }
}

// Helper function.  Copies a block of data up to 4GB in size from lpSrc to
// lpDst.  Unlike HugeMemCopy, HugeMemMove checks for partially overlapping
// data.
void HugeMemMove (void FAR *lpDst, const void FAR *lpSrc, DWORD dwLen)
{
	ASSERT(lpDst);
	ASSERT(lpSrc);
	if (lpDst <= lpSrc || (BIGPTR)lpDst >= ((BIGPTR)lpSrc + dwLen)) 
        {
		// Non-Overlapping Buffers or overlap with destination lower
		// copy from lower addresses to higher addresses
		while (dwLen--) 
            {
			*(BIGPTR)lpDst = *(BIGPTR)lpSrc;
			lpDst = (BIGPTR)lpDst + 1;
			lpSrc = (BIGPTR)lpSrc + 1;
		    }
	    }
	else 
        {
		// Overlapping Buffers with destination higher
		// copy from higher addresses to lower addresses
		lpDst = (BIGPTR)lpDst + dwLen - 1;
		lpSrc = (BIGPTR)lpSrc + dwLen - 1;

		while (dwLen--) 
            {
			*(BIGPTR)lpDst = *(BIGPTR)lpSrc;
			lpDst = (BIGPTR)lpDst - 1;
			lpSrc = (BIGPTR)lpSrc - 1;
		    }
	    } // else -if
}

// Default constructor.  Initializes CHugeString to be empty, with no memory
// allocated.
CHugeString::CHugeString ()
{
	m_dwDataLen = 0;
	m_dwAllocLen = 0;
}

// Copy constructor.
CHugeString::CHugeString (CHugeString& hsOther)
{
	void FAR *lpDst;
	const void FAR *lpSrc;
	m_dwDataLen = hsOther.m_dwDataLen;
	AllocMem (m_dwDataLen);
	lpDst = LockMem(0);
	lpSrc = hsOther.LockMem();
	HugeMemCopy (lpDst, lpSrc, m_dwDataLen);
	UnlockMem();
	hsOther.UnlockMem();
}

// Constructor.  Initializes CHugeString and sets its contents to those of
// lpsz, which must be a short (64k or less) zero-terminated string.
CHugeString::CHugeString (const char* psz)
{
	ASSERT(psz);
	void FAR *lpDst;
	m_dwDataLen = strlen(psz);  // Do not include the zero.
	AllocMem (m_dwDataLen);
	lpDst = LockMem(0);
	HugeMemCopy (lpDst, psz, m_dwDataLen);
	UnlockMem();
}

// Constructor.  Initializes CHugeString and sets its contents to those of
// lpch, which must a string (any contents, including zeroes) of length dwLen.
CHugeString::CHugeString (const char* pch, DWORD dwLen)
{
	void FAR *lpDst;
	m_dwDataLen = dwLen;
	AllocMem (m_dwDataLen);
	lpDst = LockMem(0);
	HugeMemCopy (lpDst, pch, m_dwDataLen);
	UnlockMem();
}

// Destructor.  Releases the allocated memory.
CHugeString::~CHugeString ()
{
	if (m_dwAllocLen != 0)
		FreeMem();
}

// Returns the character found at dwOffset.
unsigned char CHugeString::operator[] (DWORD dwOffset) const
{
	ASSERT(dwOffset <= m_dwDataLen);
	unsigned char tcRetVal;
	tcRetVal = *((BIGCPTR)LockMem() + dwOffset);
	UnlockMem();
	return tcRetVal;
}

// Returns the word (16-bit value) stored, starting at byte offset dwOffset,
// in CHugeString in little-Endian (Intel) format.
WORD CHugeString::wGetWord (DWORD dwOffset) const
{
	ASSERT(dwOffset+1 <= m_dwDataLen);
	const void FAR *lpSrc;
	WORD wRetVal;
	lpSrc = LockMem();
	wRetVal = *((BIGPTR) lpSrc + dwOffset) + 
		(*((BIGPTR) lpSrc + dwOffset + 1) << 8);
	UnlockMem();
	return wRetVal;
}

// Returns the dword (32-bit value) stored, starting at byte offset dwOffset,
// in CHugeString in little-Endian (Intel) format.
DWORD CHugeString::dwGetDWord (DWORD dwOffset) const
{
	ASSERT(dwOffset+3 <= m_dwDataLen);
	const void FAR *lpSrc;
	DWORD dwRetVal;
	lpSrc = LockMem();
	dwRetVal = *((BIGPTR) lpSrc + dwOffset) +
		((DWORD) *((BIGPTR) lpSrc + dwOffset + 1) << 8) +
		((DWORD) *((BIGPTR) lpSrc + dwOffset + 2) << 16) +
		((DWORD) *((BIGPTR) lpSrc + dwOffset + 3) << 24);
	UnlockMem();
	return dwRetVal;
}

// Extracts a sub-string of iLength characters from CHugeString, starting at 
// dwOffset, and stores it (plus a terminating null) in a Str8.  If iLength 
// is greater than the number of characters available, CHugeString copies as 
// many as it can.  If dwOffset is greater than the length of CHugeString, the 
// Str8 is emptied but no characters are copied.  The return value is the 
// number of characters in the Str8 (not counting the terminating null).
int CHugeString::iGetSubString(Str8& sTarget, DWORD dwOffset, int iLength) const
{                                           
	ASSERT(iLength >= 0);
	const void FAR *lpSrc;
	char* pszBuffer;
	if (dwOffset > m_dwDataLen) 
        {
		sTarget.Empty();
		return 0;
	    }
	iLength = (int) min((DWORD) iLength, m_dwDataLen - dwOffset);
	lpSrc = LockMem();
	pszBuffer = sTarget.GetBuffer (iLength+1); // 1.4qzfq GetBuffer OK because immediately written
	memcpy (pszBuffer, ((BIGPTR) lpSrc) + dwOffset, iLength);
	pszBuffer[iLength] = 0;
	sTarget.ReleaseBuffer();
	UnlockMem();
	return iLength;
}

// Extracts a sub-string of nLength characters from CHugeString, starting at 
// dwOffset, and stores it in another CHugeString.  If dwLength is greater 
// than the number of characters available, CHugeString copies as many as it
// can.  If dwOffset is greater than the length of CHugeString, the CHugeString
// CHugeString is emptied but no characters are copied.  The return value is the 
// number of characters in the CHugeString.
DWORD CHugeString::dwGetSubString(CHugeString& hsTarget, DWORD dwOffset, 
	                              DWORD dwLength) const
{
	const void FAR *lpSrc;
	void FAR *lpDst;
	if (dwOffset > m_dwDataLen) 
        {
		hsTarget.Empty();
		return 0;
	    }
	dwLength = (int) min((DWORD) dwLength, m_dwDataLen - dwOffset);
	lpSrc = LockMem();
	lpDst = hsTarget.LockMem(dwLength);
	HugeMemCopy (lpDst, ((BIGPTR) lpSrc) + dwOffset, dwLength);
	hsTarget.UnlockMem(dwLength);
	UnlockMem();
	return dwLength;
}

// Sets this to be a zero-length string.  Does not release any memory.
void CHugeString::Empty() 
    {
	m_dwDataLen = 0;
	FreeMem();
    }

// Copies the contents of hsOther to this.
CHugeString& CHugeString::operator= (const CHugeString& hsOther)
{
	void FAR *lpDst;
	const void FAR *lpSrc;
	lpSrc = hsOther.LockMem();
	lpDst = LockMem(hsOther.m_dwDataLen);
	HugeMemCopy (lpDst, lpSrc, hsOther.m_dwDataLen);
	hsOther.UnlockMem();
	UnlockMem(hsOther.m_dwDataLen);
	return *this;
}

// Copies the contents of lpszOther (<64k, zero-terminated) to this.
CHugeString& CHugeString::operator= (const char* pszOther)
{
	void FAR *lpDst;
	DWORD dwNewSize = strlen(pszOther);
	lpDst = LockMem(dwNewSize);
	HugeMemCopy (lpDst, pszOther, dwNewSize);
	UnlockMem (dwNewSize);
	return *this;
}

// Appends a copy of chsOther to this.
CHugeString& CHugeString::operator+= (const CHugeString& hsOther)
{
	void FAR *lpDst;
	const void FAR *lpSrc;
	DWORD dwNewSize = m_dwDataLen + hsOther.m_dwDataLen;
	lpDst = (BIGPTR) LockMem(dwNewSize) + m_dwDataLen;
	lpSrc = hsOther.LockMem();
	HugeMemCopy (lpDst, lpSrc, hsOther.m_dwDataLen);
	UnlockMem (dwNewSize);
	hsOther.UnlockMem();
	return *this;
}

// Appends a copy of lpszOther (<64k, zero-terminated) to this.
CHugeString& CHugeString::operator+= (const char* pszOther)
{
	ASSERT(pszOther);
	void FAR *lpDst;
	DWORD dwOtherLen = strlen(pszOther);
	DWORD dwNewSize = m_dwDataLen + dwOtherLen;
	lpDst = (BIGPTR) LockMem(dwNewSize) + m_dwDataLen;
	HugeMemCopy (lpDst, pszOther, dwOtherLen);
	UnlockMem (dwNewSize);
	return *this;
}

// Appends a copy of cOther to this.
CHugeString& CHugeString::operator+= (char tcOther)
{
	void FAR *lpDst = ((BIGPTR) LockMem (m_dwDataLen + 1)) + m_dwDataLen;
	*((BIGCPTR)lpDst) = tcOther;
	UnlockMem (m_dwDataLen + 1);
	return *this;
}

// Stores wValue in CHugeString, starting at byte offset dwOffset, in
// little-Endian (Intel) format.
void CHugeString::SetWord (DWORD dwOffset, WORD wValue)
{
	ASSERT(dwOffset+1 <= m_dwDataLen);
	void FAR *lpDst = LockMem(0);
	*((BIGPTR) lpDst + dwOffset    ) = LOBYTE(wValue);
	*((BIGPTR) lpDst + dwOffset + 1) = HIBYTE(wValue);
	UnlockMem();
}

// Stores dwValue in CHugeString, starting at byte offset dwOffset, in
// little-Endian (Intel) format.
void CHugeString::SetDWord (DWORD dwOffset, DWORD dwValue)
{
	ASSERT(dwOffset+3 <= m_dwDataLen);
	void FAR *lpDst = LockMem(0);
	*((BIGPTR) lpDst + dwOffset    ) = LOBYTE(LOWORD(dwValue));
	*((BIGPTR) lpDst + dwOffset + 1) = HIBYTE(LOWORD(dwValue));
	*((BIGPTR) lpDst + dwOffset + 2) = LOBYTE(HIWORD(dwValue));
	*((BIGPTR) lpDst + dwOffset + 3) = HIBYTE(HIWORD(dwValue));
	UnlockMem();
}

// Adds wValue to the end of CHugeString in little-Endian (Intel) format.
void CHugeString::AppendWord (WORD wValue)
{
	void FAR *lpDst = LockMem(m_dwDataLen + 2);
	*((BIGPTR) lpDst + m_dwDataLen    ) = LOBYTE(wValue);
	*((BIGPTR) lpDst + m_dwDataLen + 1) = HIBYTE(wValue);
	UnlockMem(m_dwDataLen + 2);
}

// Adds dwValue to the end of CHugeString in little-Endian (Intel) format.
void CHugeString::AppendDWord (DWORD dwValue)
{
	void FAR *lpDst = LockMem(m_dwDataLen + 4);
	*((BIGPTR) lpDst + m_dwDataLen    ) = LOBYTE(LOWORD(dwValue));
	*((BIGPTR) lpDst + m_dwDataLen + 1) = HIBYTE(LOWORD(dwValue));
	*((BIGPTR) lpDst + m_dwDataLen + 2) = LOBYTE(HIWORD(dwValue));
	*((BIGPTR) lpDst + m_dwDataLen + 3) = HIBYTE(HIWORD(dwValue));
	UnlockMem(m_dwDataLen + 4);
}

// Removes a sub-string from within the middle of CHugeString.
void CHugeString::RemoveSubString (DWORD dwOffset, DWORD dwLength)
{
	ASSERT(dwOffset + dwLength <= m_dwDataLen);
	void FAR *lpDst = LockMem(0);
	HugeMemMove ((BIGPTR) lpDst + dwOffset, 
		(BIGPTR) lpDst + dwOffset + dwLength, m_dwDataLen - dwOffset - dwLength);
	UnlockMem(m_dwDataLen - dwLength);
}

// Inserts a sub-string into the middle of CHugeString.
void CHugeString::InsertSubString (const Str8& sNewValue, DWORD dwOffset)
{
	ASSERT(dwOffset <= m_dwDataLen);
	DWORD dwLength = sNewValue.GetLength();
	void FAR *lpDst = LockMem(m_dwDataLen + dwLength);
	HugeMemMove ((BIGPTR) lpDst + dwOffset + dwLength, 
		(BIGPTR) lpDst + dwOffset, m_dwDataLen - dwOffset);
	HugeMemCopy ((BIGPTR) lpDst + dwOffset, sNewValue, dwLength);
	UnlockMem (m_dwDataLen + dwLength);
}

// Inserts a huge sub-string into the middle of CHugeString.
void CHugeString::InsertSubString (const CHugeString& hsNewValue, 
                                   DWORD dwOffset)
{
	ASSERT(dwOffset <= m_dwDataLen);
	DWORD dwLength = hsNewValue.dwGetLength();
	void FAR *lpDst = LockMem(m_dwDataLen + dwLength);
	const void FAR *lpSrc = hsNewValue.LockMem();
	HugeMemMove ((BIGPTR) lpDst + dwOffset + dwLength, 
		(BIGPTR) lpDst + dwOffset, m_dwDataLen - dwOffset);
	HugeMemCopy ((BIGPTR) lpDst + dwOffset, lpSrc, dwLength);
	UnlockMem (m_dwDataLen + dwLength);
	hsNewValue.UnlockMem();
}

// Replaces dwLength bytes, starting at dwOffset, with csNewValue.  This
// function is little more than a call to RemoveSubString followed by a call to
// InsertSubString.  Note that this is not as efficient as it could be, but
// considering that ReplaceSubString is currently unused, I really don't care.
void CHugeString::ReplaceSubString (const Str8& sNewValue, DWORD dwOffset, DWORD dwLength)
{
	RemoveSubString (dwOffset, dwLength);
	InsertSubString (sNewValue, dwOffset);
}

// Writes the entire contents of CHugeString to the CFile.
void CHugeString::WriteToFile (CFile& cfOutput) const
{
	if (m_dwDataLen > 0)					//1/10/2000
		{	
		cfOutput.Write(LockMem(), m_dwDataLen);
		UnlockMem();
		}
}

// Reads in dwLength bytes from CFile and stores them in CHugeString.
// TODO - Check the length of the file to get a better idea of how
// much space to allocate.
void CHugeString::ReadFromFile (CFile& cfInput, DWORD dwLength)
{
	if (dwLength > 0)					//1/10/2000
		{
		dwLength = cfInput.Read(LockMem(dwLength), dwLength);
		UnlockMem(dwLength);
		}
	else if (dwLength == 0)					//1/10/2000
		Empty();
}

// Locks CHugeString's block of memory in place with a call to GlobalLock.
// Returns a pointer to that block of memory.  The calling program should call
// UnlockMem as soon as possible.  This version of the function assumes that
// the caller will not modify the data.
inline const void *CHugeString::LockMem() const
{
	ASSERT(m_dwAllocLen);
	return m_pLockedData;
}

// Locks CHugeString's block of memory in place with a call to GlobalLock.
// Insures that the block of memory is at least dwMinSize bytes big.  Returns
// a pointer to that block of memory.  The calling program should call 
// UnlockMem as soon as possible.
inline void *CHugeString::LockMem (DWORD dwMinSize)
{
	ReAllocMem (dwMinSize);
	return m_pLockedData;
}

// Unlocks CHugeString's block of memory with a call to GlobalUnlock.  This
// function should be called after the caller is done with the memory pointer
// returned by CHugeString's LockMem function.  This version of the function
// assumes that the data remains unchanged.
inline void CHugeString::UnlockMem() const
{
}

// Unlocks CHugeString's block of memory with a call to GlobalUnlock.  This
// function should be called after the caller is done with the memory pointer
// returned by CHugeString's LockMem function.  Since CHugeString really
// doesn't know what the user did to the memory during this time, the user
// passes the new length of the data (not including any terminating nulls).
inline void CHugeString::UnlockMem (DWORD dwNewLen)
{
	m_dwDataLen = dwNewLen;
}

// Allocates more memory by calling GlobalAlloc or malloc.
void CHugeString::AllocMem (DWORD dwNewSize)
{
	// Figure out how much space we'll want (round up to the next dwReallocLen).
	m_dwAllocLen = ((DWORD) (dwNewSize / dwReallocLen) + 1) * dwReallocLen;
	
    // Try allocating on the heap, if it's small enough to fit on the heap.
	if (m_dwAllocLen <= dwHugeThreshold) 
        {
		m_pLockedData = malloc ((size_t) m_dwAllocLen);
		if (m_pLockedData) 
            {
			m_hData = NULL;
			return;
		    } // if
	    } // if

	// If heap use failed, or if the new block is too big, use GlobalAlloc.
	m_hData = GlobalAlloc (GMEM_MOVEABLE, m_dwAllocLen);
	ASSERT(m_hData);
	m_pLockedData = GlobalLock(m_hData);
	ASSERT(m_pLockedData);
}

// Allocates more memory, if needed, with a call to GlobalReAlloc or realloc.
// If dwNewSize is smaller than the current allocated length, no action is
// taken.  If no memory was previously allocated, ReAllocMem allocates it.
void CHugeString::ReAllocMem (DWORD dwNewSize)
{
	// Figure out how much space we'll want (round up to the next dwReallocLen).
	if (dwNewSize <= m_dwAllocLen)
		return;
	
    if (m_dwAllocLen == 0) 
        {
		AllocMem(dwNewSize);
		return;
	    }

	m_dwAllocLen = (DWORD (dwNewSize / dwReallocLen) + 1) * dwReallocLen;
	
    void FAR *pNewLockedData;
	// There are three possible cases, depending on whether or not we're huge
	// now and whether or not we want to be huge.
	// Case 1: Start small, finish small
	if (!m_hData && (m_dwAllocLen <= dwHugeThreshold)) 
        {
		pNewLockedData = realloc(m_pLockedData, (size_t) m_dwAllocLen);
		if (pNewLockedData != NULL) 
            {
			m_pLockedData = pNewLockedData;
			return;
		    }  // If we couldn't allocate on the heap, 
	    }      // fall through to allocate globally.
	// Case 2: Start small, finish huge
	if (!m_hData) 
        {
		m_hData = GlobalAlloc (GMEM_MOVEABLE, m_dwAllocLen);
		ASSERT(m_hData);
		pNewLockedData = GlobalLock(m_hData);
		ASSERT(pNewLockedData);
		memcpy(pNewLockedData, m_pLockedData, (size_t) m_dwDataLen);
		free(m_pLockedData);
		m_pLockedData = pNewLockedData;
		return;
	    } // if
	// Since CHugeString never shrinks, barring a call to FreeMem, the "Start
	// huge, finish small" case does not exist.
	// Case 3: Start huge, finish huge
	GlobalUnlock(m_hData);
	m_hData = GlobalReAlloc (m_hData, m_dwAllocLen, 0);
	ASSERT(m_hData);
	m_pLockedData = GlobalLock(m_hData);
	ASSERT(m_pLockedData);
}
 
// Releases the memory associated with this string.
void CHugeString::FreeMem()
{
    if (m_dwAllocLen) 
        {
	    if (m_hData) 
            {
		    GlobalUnlock (m_hData);
		    GlobalFree (m_hData);
	        } 
        else 
            {
		    free(m_pLockedData);
	        }
	    m_dwAllocLen = 0;
        }
}






