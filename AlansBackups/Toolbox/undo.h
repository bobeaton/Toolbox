// undo.h

#ifndef Undo_h
#define	Undo_h

#include "cdbllist.h" // For derivation
#include "cdblel.h" // For derivation


class CUndo : public CDblListEl // Hungarian: undo
{
friend class CUndoList;
private:
	long int m_lFieldNum; // index of field in record - can't save pointer to field
	int m_iChar; // index of character in field where action occurred
	long int m_lDeleteCount; // number of chars to be deleted, if zero then undo is an insert
	Str8 m_sText; // if m_iDeleteCount is zero, this is the text to be inserted into record

private:
	CUndo( long int lFieldNum, int iChar, long int lDeleteCount, const char* pszText="" )
		{ m_lFieldNum = lFieldNum, m_iChar = iChar, m_lDeleteCount = lDeleteCount, m_sText = pszText; }

public:
	long int lFieldNum() const { return m_lFieldNum; }
	int iChar() const { return m_iChar; }
	BOOL bIsInsert() const { return m_lDeleteCount == 0; }
	long int lDeleteCount() const { return m_lDeleteCount; }
	const char* pszText() const { return m_sText; }
};  // class CUndo


// --------------------------------------------------------------------------

class CUndoList : public CDblList  // Hungarian: undlst
{
private:
	BOOL m_bUsed; // signals item was retrieved from list for an undo operation, reset list on next Add()

public:
#if 0
	CUndo* pundFirst() const	{ return (CUndo*)pelFirst(); }
	CUndo* pundNext( const CUndo* pund) const { return (CUndo*)pelNext(pund); }
	CUndo* pundPrev( const CUndo* pund) const { return (CUndo*)pelPrev(pund); }
	CUndo* pundLast() const	{ return (CUndo*)pelLast(); }	
#endif
	void Add( CRecPos rpsBegin, CRecPos rpsEnd ); // stuff inserted between rpsBegin and rpsEnd
	void Add( CRecPos rps, const char* pszText ); // string deleted at rps
	void Add( CRecPos rps, char c ); // single char deleted at rps
	
	CUndo* pundGetUndo(); // get item to be undone from list, removes item from list, caller needs to delete!
};  // class CUndoList

#endif // Undo_h