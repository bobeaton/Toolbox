// number.h  Renumbering/Text-breaking process

#ifndef NUMBER_H
#define NUMBER_H

#include "mkr.h"

#define MAX_START_REF_NUM 6400000
#define MAX_REF_NUM 6500000

class Object_ostream;  // obstream.h
class Object_istream;  // obstream.h
class CDatabaseType;   // typ.h
class CRecLookEl;      // creclookel.h
class CRecord;         // crecord.h
class CField;          // cfield.h
class CShwView;        // shwview.h
class CShwDoc;         // shwdoc.h
class CIndexSet;       // ind.h (added 1999-12-16 by TLB to support field
                       //        deletion via the index)

// ==========================================================================

class CNumberProcess // Hungarian: nbrproc
{

private:
	CDatabaseType* m_ptypMyOwner;
    CMarker* m_pmkrRef;
    CMarker* m_pmkrTxt;
	BOOL m_bWholeDocument;
	BOOL m_bBreakText;
	BOOL m_bGetNameFromMkr;
	Str8 m_sName;
	CMarker* m_pmkrUseContentsAsName;
	BOOL m_bForceName;
	unsigned int m_iStartRef;
	int m_iNumberOfDigits;
	Str8 m_sBreakChars;
    CMarkerSubSet m_subsetTextBreakMarkers;
	BOOL m_bDontPromptForSave;

protected:
    friend class CNumberDlg;
    friend class CReNumberDlg;

    // Prevent use of constructors and assignment operator
    // by making them private and omitting their definitions.
private:
	unsigned int m_iRefNum;

    CNumberProcess(CDatabaseType* ptypMyOwner, CMarker* pmkrRef = NULL, CMarker* pmkrTxt = NULL);
    CNumberProcess(const CNumberProcess& nbrproc);
    CNumberProcess& operator=(const CNumberProcess& nbrproc);

public:
    ~CNumberProcess();

    CMarker* pmkrRef() { return m_pmkrRef; }
    CMarker* pmkrTxt() { return m_pmkrTxt; }
    
#ifdef typWritefstream // 1.6.4ac
	void WriteProperties(Object_ofstream& obs) const;
#else
	void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CDatabaseType* ptypMyOwner, CNumberProcess** ppnbrproc);
	static void MarkerUpdated(CMarkerUpdate& mup, CNumberProcess** ppnbrproc);
	static void CreateInstance(CDatabaseType* ptyp, CNumberProcess** ppnbrproc, CMarker* pmkrRef, CMarker* pmkrTxt);

    static void CNumberProcess::s_CreateInstance(CDatabaseType* ptypMyOwner, CNumberProcess** ppnbrproc);
	BOOL bNumber(CShwView* pview, CRecLookEl* prelCur, BOOL bDoBreak); // Display dialog to get renumbering properties and then renumber per user's request.

	static void s_CreateInstance(CDatabaseType* ptypMyOwner, CNumberProcess** ppnbrproc, CMarker* pmkrRef, CMarker* pmkrTxt);
    BOOL bReNumberDocument( CShwDoc* pdoc ); // (re-)number entire document
    BOOL bReNumberRecord( CShwDoc* pdoc, CRecLookEl* prel ); // Attempt to (re-)number given record

private:
    void ReadProperties( Object_istream& obs );
    // 1999-12-16 - TLB: bReNumberRecord modified to delete bogus ref field via the index
	BOOL bReNumberRecord( CRecLookEl* prel, CIndexSet* pindset ); // (re-)number record, return TRUE if modified
    BOOL bBreakFieldText( CRecord* prec, CField** ppfld, BOOL bInsertRef ); // Attempt to break text of given field into "sentences"
	void GetNameFromMkr( CField* pfld, BOOL bNewRecord, BOOL bDiscardNumericName = FALSE ); //Extract "name" from contents of marker and set m_sName
	void BreakText( Str8& sTxt, Str8& sSentence );
    BOOL bUpdateRef( CField* pfld ); // Update field with new ref name and number, if needed
	void FormatRefNum( Str8& sContents ); // Format reference number and append to sContents; increment counter.
	BOOL bBreakChar( char ch );
};  // class CNumberProcess

#endif  // not NUMBER_H
