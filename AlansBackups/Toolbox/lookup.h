//File: lookup.h
// Definition for the CLookupMatchList and CLookupMatch classes.
// CLookupMatch gives a single occurance of a particular word in a particular file.
// CLookupMatchList is a list of CLookupMatches plus a few functions to manipulate
// that list.

#ifndef _LOOKUP_H
#define _LOOKUP_H

#include "stdiofil.h"

class CLookupMatch : public CObject {

public:
	CLookupMatch (const Str8& sNewRef, const Str8& sNewPreBefore,
		const Str8& sNewBefore, const Str8& sNewTarget, 
		const Str8& sNewPreAfter, const Str8& sNewAfter);

	const Str8& GetRef()       { return m_sRef; }
	const Str8& GetPreBefore() { return m_sPreBefore; }
	const Str8& GetBefore()    { return m_sBefore; }
	const Str8& GetTarget()    { return m_sTarget; }
	const Str8& GetPreAfter()  { return m_sPreAfter; }
	const Str8& GetAfter()     { return m_sAfter; }

private:
	Str8 m_sRef, m_sPreBefore, m_sBefore, m_sTarget, m_sPreAfter, m_sAfter;
};



class CLookupMatchList : public CObList {

public:
	CLookupMatchList();
	~CLookupMatchList();
	void DeleteAll();

	void AddLookupMatch (const Str8& sNewRef, const Str8& sNewPreBefore, 
		const Str8& sNewBefore, const Str8& sNewTarget, 
		const Str8& sNewPreAfter, const Str8& sNewAfter);

	void WriteToFile (CStdioFileEx& fOutput, const char* pszRefMkr,
				const char* pszBefMkr, const char* pszTargetMkr, 
				const char* pszAftMkr, BOOL bMakeConcRefz=FALSE);
};

#endif
