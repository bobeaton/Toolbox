// srt.h Interface for Shoebox sort orders (1995-04-03)

// Change history:
// 1995-04-03 0.10  MRP: Separated from language encodings

#ifndef SRT_H
#define SRT_H

#ifdef ESCES
// ESCES Language encoding function library
typedef int TLength;  // len
typedef unsigned short LngSortCode;  // cod
#endif

#include "set.h"  // classes CSet and CSetEl
#include "ref.h"
#if UseCct
#include "ChangeTable.h"  // class ChangeTable
#endif
#include <fstream>

#include "update.h"  // CUpdate
#include "cbo.h"
#include "lbo.h"

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h
class Object_ofstream;  // 1.6.4ab 

// **************************************************************************

typedef unsigned short Order;  // Hungarian: ord
    // Numeric value for comparison of multi-chars according to a sort order.

typedef unsigned short UIdMChar;  // Hungarian: uch
    // Unique identity of a multicharacter designator in its language encoding,
    // which is used in CIndex sort keys and as the index to into the array
    // CMCharOrder objects in each sort order.

// The Match group of option buttons [in the Search Settings dialog box]
// gives you control over how Shoebox determines that a character in a key
// matches one in the search text. The option buttons in the Match group
// interact with the sort orders. [Sh2, pp. 249, 124]
// The match setting determines which characters are regarded in a match;
// and which order code is used for sorting and matching comparisions,
// that is, to what extent are related characters distinguished.

typedef unsigned char MatchSetting;  // Hungarian: matset
typedef unsigned char MCharType;  // Hungarian: mchtyp

// --------------------------------------------------------------------------

// The sorting properties of a character in a language encoding:
// numeric order at three match settings and the type used to determine
// whether to include the character in a match or key comparison.

class CMCharOrder  // Hungarian: mchord
{
private:
    Order m_ordPri;
        // Match characters "By primary grouping only"
        // The secondary order of the first multichar in the primary group
        // containing this. 
        // Characters in a group enclosed in curly braces are not included
        // in a primary-only match; they have the value ordSecOnly.
    
    Order m_ordSecDisregardingCase;
        // Match characters "Disregarding case"
        // The secondary order of the upper case counterpart of this;
        // otherwise, the secondary order of this 
     
    Order m_ordSec; 
        // Match characters
        //     "Exactly by secondary ordering"
        //     "Even those normally ignored"
        // Each non-ignore element has a unique value corresponding to
        // its position in the sort order, from one up to a maximum of 253.
        // Ignore and Undefined characters have 254 and 255, respectively.
    
    MCharType m_mchtyp;  // multichar designator type
    
public:
    static const Order ordSecOnly;  // 0: not included, if primary only
    static const Order ordFirstOrdered;  // 1: first ordered value
    static const Order ordLastOrdered;  // 253: last ordered value
    static const Order ordIgnore;  // 254: after all ordered elements
    static const Order ordUndefined;  // 255: not in sort order
    // NOTE: Unlike Sh2, in Sh1.x, ignores did have a secondary ordering,
    // and undefined chars sorted to the beginning.
    
    static const MCharType mchtypPri;
    static const MCharType mchtypSecOnly;
    static const MCharType mchtypIgnore;
    static const MCharType mchtypUndefined;
    
    static const MatchSetting matsetSamePrimaryGroup;
    static const MatchSetting matsetExactDisregardCase;
    static const MatchSetting matsetExact;
    static const MatchSetting matsetEvenIgnore;

    CMCharOrder();  // default constructor
        // 1. The sort order constructs an array of undefined order objects.

    void Set(Order ordPri, Order ordSec, MCharType mchtyp);
        // 2. It then sets the primary and secondary order of characters
        //    in its \order field   

    void SetOrderDisregardingCase(Order ord);
        // 3. After that, it sets the disregarding case order of lower case
        //    characters in its language encoding's \case field.
    void ClearOrderDisregardingCase();

    BOOL bIncludeInMatch(MatchSetting matset) const
            { return ((m_mchtyp & matset) != 0); }
        // Return whether to include the corresponding multichar in matches
        // and key comparisons at the specified setting.

    Order ord(MatchSetting matset) const;
        // Return the proper numeric order for comparisons at the setting.
    Order ordPri() const { return m_ordPri; }
    Order ordSecDisregardingCase() const { return m_ordSecDisregardingCase; }
    Order ordSec() const { return m_ordSec; }

    // Multichar type classifications-- they are mutually exclusive.
    BOOL bPrimary() const { return ((m_mchtyp & mchtypPri) != 0); }
    BOOL bSecondaryOnly() const { return ((m_mchtyp & mchtypSecOnly) != 0); }
    BOOL bIgnore() const { return ((m_mchtyp & mchtypIgnore) != 0); }
    BOOL bUndefined() const { return ((m_mchtyp & mchtypUndefined) != 0); }

#ifdef _DEBUG
    void AssertValid() const;
#endif  // _DEBUG

private:    
    void SetMCharType(MCharType mchtyp) { m_mchtyp |= mchtyp; }
    void ClearMCharType(MCharType mchtyp) { m_mchtyp &= (~mchtyp); }
};  // class CMCharOrder

class CLangEnc;

// --------------------------------------------------------------------------

// The sorting properties of characters in a language encoding.
// The array of order elements is indexed by the unique identifier code
// contained in the multichars of the language encoding.

// NOTE: Since a language encoding can contain more than one sort order,
// it designates one as the default, whose properties are used for
// text matching and comparing secondary fields in sort keys.

class CMChar;

class CSortOrder : public CSetEl  // Hungarian: srt
{
private:
    Str8 m_sDescription;
    Str8 m_sPrimaryOrder;  // contents of the \order field  RNE Repository for primary order characters.
    Str8 m_sSecondariesPreceding; // RNE Secondaries preceding unmarked case.
    Str8 m_sSecondariesFollowing; // RNE Secondaries following unmarked case.
    Str8 m_sIgnore;  // contents of the \ignore field
    BOOL m_bSecAfterBase;  // "secondary-onlys" follow their base primaries
    BOOL m_bSpacePrecedes;  // Spaces " " come first in collation.

    // Sorting properties of multichars indexed by unique identity (uch)
    CMCharOrder* m_pmchord;  // dynamically allocated array     

    Order m_ordPri;  // of current group of multichars being added
    Order m_ordSecNext;  // of next ordered multichar to be added
    Order m_ordFirstPri;  // of the first primary multichar 

    // Making changes to sort keys for Non-Roman Script encodings
#if UseCct
    Str8 m_sPathCCT;
    ChangeTable m_cct;
#endif
    Str8 m_sPathDLL;
    HINSTANCE m_hDLL;
    void* m_pfMakeSortKey;
    BOOL m_bSkipSeq; // Skip sequencing for cases where key is fully built by CCT or DLL

    friend class CSortOrderSet; 
    CSortOrderSet* m_psrtsetMyOwner;  // reference to language encoding owning this

    CSortOrder(const char* pszName, const char* pszDescription,
            const char* pszPrimaryOrder,
            const char* pszSecondariesPreceding, const char* pszSecondariesFollowing,
            const char* pszIgnore, BOOL bSecAfterBase,
            const char* pszPathCCT, const char* pszPathDLL,
            BOOL bSkipSeq,
            CSortOrderSet* psrtsetMyOwner);
    
    static BOOL s_bConstruct(const char* pszName, const char* pszDescription,
            const char* pszPrimaryOrder, 
            const char* pszSecondariesPreceding, const char* pszSecondariesFollowing,
            const char* pszIgnore, BOOL bSecAfterBase,
            const char* pszPathCCT, const char* pszPathDLL,
            BOOL bSkipSeq,
            CSortOrderSet* psrtsetMyOwner,
            CSortOrder** ppsrt, CNote** ppnot);
        // Construct a new sort order object from its properties.
        // Returns it via ppsrt if TRUE; otherwise returns a note via ppnot.
        // NOTE: Used to read from a .enc file and by the Add... button
        // on the Sort Orders dialog box.

    static void s_ConstructDefault(const char* pszName,
            CSortOrderSet* psrtsetMyOwner, CSortOrder** ppsrt);
        // Construct and return a new sort order with default properties.
        // NOTE: Used to resolve a reference to a non-existing sort order.
    
public:
    static const UIdMChar uchMultiKeySeparator;

    ~CSortOrder();

    const Str8& sDescription() const { return m_sDescription; }
    const char* pszPrimaryOrder() const { return m_sPrimaryOrder; }
    const char* pszSecondariesPreceding() const { return m_sSecondariesPreceding; }
    const char* pszSecondariesFollowing() const { return m_sSecondariesFollowing; }
    const char* pszIgnore() const { return m_sIgnore; }
    BOOL bSecAfterBase() const { return m_bSecAfterBase; }  
    BOOL bSeparateDiacriticsAfter() const { return m_bSecAfterBase; }

    // Defaults
    static const char* s_pszDescription;
    static const char* s_pszPrimary;  // contents of the \order field  RNE Repository for primary order characters.
    static const char* s_pszSecondaryBefore; // RNE Secondaries preceding unmarked case.
    static const char* s_pszSecondaryAfter; // RNE Secondaries following unmarked case.
    static const char* s_pszIgnore;  // contents of the \ignore field
    static const BOOL s_bSecondariesFollowBase;  // "secondary-onlys" follow their base primaries
    static const BOOL s_bSpaceFirst;    // Spaces " " come first in collation.

    CSortOrderSet* psrtsetMyOwner() const { return m_psrtsetMyOwner; }
    virtual CSet* psetMyOwner();
    CLangEnc* plngMyOwner() const;

	BOOL bUnicodeSequenceSort() const; // 1.4eg Add Unicode sequence sort
	BOOL bUnicodeSequenceIgnore( LONG lUTF32Char ) const; // 1.4eh
	void SkipUnicodeSequenceIgnores( const char* &psz ) const; // 1.4eh
	int iCompareUnicodeSequence( const char* pszA, const char* pszB, MatchSetting matset, BOOL bMatchWhole ) const; // 1.4eg Add Unicode sequence sort // 1.4gy

#if UseCct
	const char* pszPathCCT() const { return m_sPathCCT; }
#endif

    static void s_SortableSubString(const char** ppsz, Length* plen);
        // From the null-terminated string starting at *ppsz,
        // return the start *ppsz and length *plen of the substring
        // from which to make the sort key, using the rules:
        // 1. Skip over leading white space;
        // 2. If there are any embedded newlines, then stop at the first;
        // 3. Back up over trailing white space.
    static void s_SortableSubString(Str8* ps);

    BOOL bSortKey(const char* pszKey, UIdMChar** ppuchSortKeyBuf,
            Length maxlenSortKey, BOOL bSortPriKeyFromEnd = FALSE);
        // Return the sort key corresponding to pszKey via ppuchSortKeyBuf.
        // Each multichar is represented by its unique identity code (uch).
        // Returns FALSE if any sort key component had to be truncated.
    BOOL bAppendToSortKey(const char* pszKey, UIdMChar** ppuchSortKeyBuf,
            Length maxlenSortKey);
        // updates buffer pointer
    
    int iCompare(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                    MatchSetting matset = CMCharOrder::matsetEvenIgnore,
                    BOOL bMatchWhole = TRUE) const;
        // Compare sort keys at the match setting:
        // return negative if A < B; zero if A == B, positive if A > B.
        // Whole match requires equivalent lengths for equality; otherwise
        // (i.e. partial match) A is permitted to match a prefix of B.
        // E.g. whole match: "ab" < "abc"; partial match: "ab" == "abc".

    BOOL bIncludeInMatch(const CMChar* pmch, MatchSetting matset) const;
    BOOL bIncludeInMatch(UIdMChar uch, MatchSetting matset) const
        { return m_pmchord[uch].bIncludeInMatch(matset); }
        // Return whether to include the multichar in matches or comparisons
        // at the specified setting, according to this sort order.
    
    static BOOL bIsTerminator(const UIdMChar uch)
        { return ( (uch == 0) || (uch == uchMultiKeySeparator)); }
        // Return whether at the end of a sort key component.
    
    // Compare multichars at the match setting according to this sort order:
        // returns negative if A < B; zero if A == B, positive if A > B.
    int iCompareMChars(const CMChar* pmchA, const CMChar* pmchB,
                                                MatchSetting matset) const;
    int iCompareMChars(UIdMChar uchA, UIdMChar uchB, MatchSetting matset) const
        { return m_pmchord[uchA].ord(matset) - m_pmchord[uchB].ord(matset); }

    // Attributes of the multichar according to this sort order
    const CMCharOrder* pmchord(const CMChar* pmch) const;

    // 2000-04-17 MRP
    // Return the first primary character in the input string s or
    // substring range [pch,pch+len). Because this function is intended
    // for section headings in exported dictionaries, it disregards
    // undefined characters (although, strictly speaking, they are considered
    // primary characters in sorting).
    // If there is no primary character, the function returns FALSE and
    // doesn't modify the last argument ppmch or sMChar.
    BOOL bFirstPrimary(const Str8& s, const CMChar** ppmch);
    BOOL bFirstPrimary(const char* pch, int len, const CMChar** ppmch);
    BOOL bFirstPrimaryLowerCase(const char* pch, int len, Str8& sMChar);
    
    // ------------------------------------------------------------------------
#ifdef ESCES
    // ESCES: External Scripture Checking Exchange Standard
    int iCompareStrings(const char* pchrA, const char* pchrB) const;
    BOOL bMatchStrings(const char* pchrA, const char* pchrB,
            BOOL bDistinguishCase, BOOL bWhole) const;
        // QUESTION: Provide all four character matching options?
    int iCompareSubstrings(const char* pchrA, TLength lenA,
            const char* pchrB, TLength lenB) const;
    BOOL bMatchSubstrings(const char* pchrA, TLength lenA,
            const char* pchrB, TLength lenB,
            BOOL bDistinguishCase, BOOL bWhole) const;
    int iCompareSortKeys(const LngSortCode* pcodA,
            const LngSortCode* pcodB) const;
    BOOL bMatchSortKeys(const LngSortCode* pcodA,
            const LngSortCode* pcodB,
            BOOL bDistinguishCase, BOOL bWhole) const;
    TLength lenMakeSortKey(const char* pchr, TLength len,
            LngSortCode* pcod, TLength maxlenKey) const;
        // QUESTION: Tell the caller if any chars are undefined or unordered?
        // QUESTION: Version which takes care of leading/trailing white space?
    static TLength s_lenCopySortKey(const LngSortCode* pcodFrom,
            LngSortCode* pcodTo, TLength maxlenKeyTo);
    static TLength s_lenSortKey(const LngSortCode* pcod);
#endif
    // ------------------------------------------------------------------------

#ifdef lngWritefstream // 1.6.4ab 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
            CSortOrderSet* psrtsetMyOwner, CSortOrder** ppsrt);
#ifdef lngWritefstream // 1.6.4ab 
    void WriteRef(Object_ofstream& obs, const char* pszQualifier) const;
#else
    void WriteRef(Object_ostream& obs, const char* pszQualifier) const;
#endif

    void SetOrderDisregardingCase();
        // Called when first constructing this sort order and also
        // when the Case property of its language encoding is modified.

#if UseCct
	void WritePaths( std::ofstream& ostr )
		{
		if ( m_sPathCCT.GetLength() > 0 )
			ostr << m_sPathCCT << "\n";
		};
#endif

    BOOL bModifyProperties(const char* pszName,
            const char* pszDescription,
            const char* pszPrimaryOrder, const char* pszSecondariesPreceding, const char* pszSecondariesFollowing,
            const char* pszIgnore, BOOL bSecAfterBase,
            CNote** ppnot);
        // Modify the properties of an existing sort order object.
        // If FALSE this object is unchanged and a note is returned via ppnot.
        // NOTE: Used by the Modify... button on the Sort Orders dialog box.

    virtual BOOL bCopy(CSetEl** ppselNew);
    virtual BOOL bModify(); 
    
#ifdef _DEBUG
    void AssertValid() const;
#endif  // _DEBUG

private:
    // 2000-04-17 MRP
    // Make changes to the sort key (optional step for non-Roman scripts).
    // For example, change "ihndi" to "hindi".
    // The input is the portion of the data field that contains the key
    // [pchKey,pchKey+lenKey) which is not necessarily null-terminated.
    // The output changed key will be null-terminated.
    // The output length doesn't include the null character.
    // Because the output is a pointer to a fixed-length buffer,
    // it might get truncated (if it is, the function returns FALSE).
    // Because the buffer is static, the caller must be completely done
    // with the output of a function call before calling the function again.
    BOOL bChangeKeyCCT(const char* pchKey, int lenKey,
            char** ppszChangedKey, int* plenChangedKey);
        // Consistent Change tables are used by SIL South Asia Group.
    BOOL bChangeKeyDLL(const char* pchKey, int lenKey,
            char** ppszChangedKey, int* plenChangedKey);
        // Dynamic Link Libraries are used by New Life Computer Institute.

    // Compare sort keys at a specified match setting-- used by iCompare above.
    int iCompare1(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                    MatchSetting matset, BOOL bMatchWhole) const;
    int iCompareSkipSeq(const UIdMChar* puchKeyA, const UIdMChar* puchKeyB,
                               BOOL bMatchWhole) const;

    BOOL bSetPrimaries(const char* pszPrimaryOrder, CNote** ppnot);
    BOOL bSetSecondaries(const char* pszSecondaries, CNote** ppnot);
    BOOL bSetIgnore(const char* pszIgnore, CNote** ppnot);
    BOOL bSetPri(const char** ppszPri, CNote** ppnot);
    BOOL bSetSec(const char** ppszSec, CNote** ppnot);
    
    BOOL bSetPri(CMChar* pmch, BOOL bFirstInGroup);
    BOOL bSetSec(CMChar* pmch);
    BOOL bSetIgnore(CMChar* pmch);
    BOOL bSetMCharOrder(CMChar* pmch, Order ordPri, Order ordSec,
            MCharType mchtyp);
    void ClearOrderDisregardingCase();
    void SetOrderDisregardingCase(CMChar* pmchUpperCase, CMChar* pmchLowerCase);
};  // class CSortOrder


// --------------------------------------------------------------------------

class CSortOrderPtr  // Hungarian: psrt
{
private:
    CSortOrder* m_psrt;
    
    // NOTE NOTE NOTE: It also counts as a reference to the language encoding.
    void IncrementNumberOfRefs(CSortOrder* psrt);
    void DecrementNumberOfRefs();
    
public:
    CSortOrderPtr(CSortOrder* psrt = NULL)
        { IncrementNumberOfRefs(psrt); }
    CSortOrderPtr(const CSortOrderPtr& psrt)  // copy constructor
        { IncrementNumberOfRefs(psrt.m_psrt); }
    
    // Decrement the reference count of the index to which
    // this currently refers and increment psrt's reference count
    const CSortOrderPtr& operator=(CSortOrder* psrt);
    const CSortOrderPtr& operator=(const CSortOrderPtr& psrt)
        { return operator=(psrt.m_psrt); }

    ~CSortOrderPtr() { DecrementNumberOfRefs(); }
    
    CSortOrder* operator->() const { return m_psrt; }
    operator CSortOrder*() const { return m_psrt; } 
};  // class CSortOrderPtr


// --------------------------------------------------------------------------

class CSortOrderInternalPtr  // Hungarian: psrt
{
private:
    CSortOrder* m_psrt;

    // NOTE NOTE NOTE: It does NOT count as a reference to the language encoding.
    // Use for CSortOrderSet::m_psrtDefault
    void IncrementNumberOfRefs(CSortOrder* psrt);
    void DecrementNumberOfRefs();
    
public:
    CSortOrderInternalPtr(CSortOrder* psrt = NULL)
        { IncrementNumberOfRefs(psrt); }
    CSortOrderInternalPtr(const CSortOrderInternalPtr& psrt)  // copy constructor
        { IncrementNumberOfRefs(psrt.m_psrt); }
    
    // Decrement the reference count of the index to which
    // this currently refers and increment psrt's reference count
    const CSortOrderInternalPtr& operator=(CSortOrder* psrt);
    const CSortOrderInternalPtr& operator=(const CSortOrderInternalPtr& psrt)
        { return operator=(psrt.m_psrt); }

    ~CSortOrderInternalPtr() { DecrementNumberOfRefs(); }
    
    CSortOrder* operator->() const { return m_psrt; }
    operator CSortOrder*() const { return m_psrt; } 
};  // class CSortOrderInternalPtr


// --------------------------------------------------------------------------

class CLangEncType;
class CDatabaseType;
class CShwDoc;
class CShwView;

class CSortOrderUpdate : public CUpdate  // Hungarian: sup
{
private:
    CSortOrder* m_psrt;
    BOOL m_bModified;
    BOOL m_bNameModified;
    BOOL m_bOrderModified;
    
public:
    CSortOrderUpdate(CSortOrder* psrt);
    CSortOrderUpdate(CSortOrder* psrt, const char* pszName,
        const char* pszPrimaryOrder, const char* pszSecondariesPreceding,
        const char* pszSecondariesFollowing,
        const char* pszIgnore, BOOL bSecAfterBase);
    
    CSortOrder* psrt() const { return m_psrt; }

    BOOL bModified() const { return m_bModified; }  
    BOOL bNameModified() const { return m_bNameModified; }
    BOOL bOrderModified() const { return m_bOrderModified; }

    // NOTE: Implementated in update.cpp rather than in srt.cpp
    virtual void UpdateLangEnc(CLangEnc* plng);
    virtual void UpdateDatabaseType(CDatabaseType* ptyp);
    virtual void UpdateShwDoc(CShwDoc* pdoc);
    virtual void UpdateShwView(CShwView* pview);
};  // class CSortOrderUpdate


// --------------------------------------------------------------------------

class CSortOrderSet : public CSet  // Hungarian: srtset
{
private:
    CSortOrderInternalPtr m_psrtDefault;  // sort order used in text pattern matching
    CLangEnc* m_plngMyOwner;  // reference to language encoding owning this
    
public:
    CSortOrderSet(CLangEnc* plngMyOwner);
    ~CSortOrderSet() {}

#ifdef lngWritefstream // 1.6.4ab 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);
    BOOL bReadSortOrderRef(Object_istream& obs, const char* pszQualifier,
            CSortOrder*& psrt);
    void AddDefaultSortOrder(const char* pszName);
    
    CLangEnc* plngMyOwner() const { return m_plngMyOwner; }

    CSortOrder* psrtDefault() const { return m_psrtDefault; }
    CSortOrder* psrtSearch( const char* pszName ) const
        { return (CSortOrder*) pselSearch( pszName ); }
        
    CSortOrder* psrtFirst() const  // First sort order
        { return (CSortOrder*) pselFirst(); }
    CSortOrder* psrtLast() const  // Last sort order
        { return (CSortOrder*) pselLast(); }
    CSortOrder* psrtNext( const CSortOrder* psrtCur ) const  // after current
        { return (CSortOrder*) pselNext( psrtCur ); }
    CSortOrder* psrtPrev( const CSortOrder* psrtCur ) const  // before current
        { return (CSortOrder*) pselPrev( psrtCur ); }
                                               
    // NOTE: InsertBefore/After omitted, since each sort order must be unique.  
    void Delete( CSortOrder** ppsrt )  // Delete sort order, set pointer to next
        { CSet::Delete( (CSetEl**) ppsrt ); }
            
    BOOL bAddSortOrder(const char* pszName,
        const char* pszDescription,
        const char* pszPrimaryOrder, const char* pszSecondariesPreceding, const char* pszSecondariesFollowing,
        const char* pszIgnore, BOOL bSecAfterBase,
        CSortOrder** ppsrt, CNote** ppnot);

    void SetDefaultSortOrder(CSortOrder* psrt);

    BOOL bViewProperties(CSortOrder** ppsrtSelected);

    BOOL bViewPropertiesOfNewSortOrder(CSortOrder** ppsrtNew,
            const char* pszName = "");
    BOOL bAdd(CSetEl** ppselNew);
    virtual void Delete(CSetEl** ppsel);

#ifdef _DEBUG
    void AssertValid() const;
#endif  // _DEBUG
};  // class CSortOrderSet


class CSortOrderSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CSortOrderSetComboBox(CSortOrderSet* psrtset) : CSetComboBox(psrtset) {}
    
    void UpdateElements(CSortOrder* psrtToSelect)
        { CSetComboBox::UpdateElements( psrtToSelect ); }
    void ChangeSet(CSortOrderSet* psrtset, CSortOrder* psrtToSelect)
        { CSetComboBox::ChangeSet( psrtset, psrtToSelect ); }
    CSortOrder* psrtSelected() { return (CSortOrder*)CSetComboBox::pselSelected(); }
};  // class CSortOrderSetComboBox

class CSortOrderSetListBox : public CSetListBox
{
public:
    CSortOrderSetListBox(CSortOrderSet* psrtset, CSortOrder** ppsrt) :
        CSetListBox(psrtset, (CSetEl**)ppsrt) {}

protected:
    int m_xName;
    int m_xDescription;
    
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
    virtual BOOL bIsBold(CSetEl* psel) { return psel->bHasRefs(); }
};  // class CSortOrderSetListBox

// **************************************************************************

#endif  // SRT_H
