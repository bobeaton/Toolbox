// fil.h Interface for Shoebox filter classes (1995-01-30)

// "You might want to think of a Shoebox filter as a set of one or more
// conditions. If a database record satisfies the condition(s), it passes
// through the filter." [The Linguist's Shoebox, p. 143. Read Chapter 11
// and the "Filters" section of Chapter 23.]


#ifndef FIL_H
#define FIL_H

#include "set.h"  // classes CSet and CSetEl
#include "patmch.h"
#include "patch.h"
#include "mkr.h"  // CMarkerSet
#include "toolbox.h" // 1.6.4ac 

class CField;  // cfield.h
class CIndex;  // ind.h
class CRecLookEl;  // crlookel.h
class CDateCon;  // 1998-04-08 MRP: date.h

class CNote;  // not.h

#include "lbo.h"  // CSetListBox, CDblListListBox
#include "update.h"  // CUpdate

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h


// **************************************************************************

class CFilterMatch : public CPatMCharMatch  // Hungarian: filmat, mat
{
private:
    const CIndex* m_pind;
    BOOL m_bMatchWhole;
    
public:
    CFilterMatch(const CIndex* pind, BOOL bMatchWhole = FALSE,
        MatchSetting matset = CMCharOrder::matsetExactDisregardCase,
        BOOL bEmbeddedMarkup = FALSE);
    virtual ~CFilterMatch() {}

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
 #endif
   
    BOOL bMatchWhole() const { return m_bMatchWhole; }
    void SetMatchWhole(BOOL bMatchWhole)
        { m_bMatchWhole = bMatchWhole; }
    const CIndex* pind() const { return m_pind; }
    
    const CRecLookEl* prelPrev(const CRecLookEl* prelCur);
    const CRecLookEl* prelNext(const CRecLookEl* prelCur);
};  // class CFilterMatch

class CFilterCon;
class CFilterEl;
class CFilterElList;

// **************************************************************************

class CFilter : public CSetEl   // Hungarian: fil
{
private:
    Str8 m_sCondition;       // e.g. "[KEY|kat] AND [p|v]"
    CFilterCon *m_pcon;         // which corresponds to the contents

    // Match settings.
    BOOL m_bMatchWhole;
    MatchSetting m_matset;

    friend class CFilterSet;
    CFilterSet* m_pfilsetMyOwner;
    
    CFilter(const char* pszName, const char* pszCondition, CFilterCon *pcon,
            CFilterSet *pfilsetMyOwner, 
            BOOL bMatchWhole, 
            MatchSetting matset);
            
    static BOOL s_bConstruct(const char* pszName, const CFilterElList& fellst,
            BOOL bMatchWhole, MatchSetting matset, CFilterSet* pfilsetMyOwner,
            CFilter **ppfil, CNote** ppnot);
    // To change the name, see CFilterSet::bSetFilterName()
    BOOL bSetCondition(const CFilterElList& fellst, CNote** ppnot);
    
public:
    ~CFilter();  // destructor

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
            CFilterSet* pfilsetMyOwner, CFilter** ppfil);
#ifdef typWritefstream // 1.6.4ac
    void WriteRef(Object_ofstream& obs, const char* pszQualifier = "") const;
#else
    void WriteRef(Object_ostream& obs, const char* pszQualifier = "") const;
#endif
    void CopyToList(CFilterElList* pfellst) const;
    void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
//  const Str8& sCondition() const { return m_sCondition; }
    
    CFilterSet* pfilsetMyOwner() const { return m_pfilsetMyOwner; }
    virtual CSet* psetMyOwner();
    
    BOOL bMatchWhole() const { return m_bMatchWhole; }

    MatchSetting GetMatchSetting() const { return m_matset; }
    
    // Return whether the record matches the filter condition
    BOOL bMatch(const CRecLookEl* prel, CIndex* pind) const;
    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const;

    BOOL bCopy(CSetEl** ppselNew);
    BOOL bModify();
    BOOL bModifyProperties(const char* pszName, const CFilterElList& fellst,
            BOOL bMatchWhole, MatchSetting matset, CNote** ppnot);
    virtual BOOL bDeletable();
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int xLeft, int xRight) const;

#ifdef _DEBUG
    void AssertValid() const;
    void AssertValidName() const;
#endif  // _DEBUG   
};  // class CFilter


// --------------------------------------------------------------------------

class CFilterPtr  // Hungarian: pfil
{
private:
    CFilter* m_pfil;
    
    void IncrementNumberOfRefs(CFilter* pfil);
    void DecrementNumberOfRefs();
    
public:
    CFilterPtr(CFilter* pfil = NULL)
        { IncrementNumberOfRefs(pfil); }
    CFilterPtr(const CFilterPtr& pfil)  // copy constructor
        { IncrementNumberOfRefs(pfil.m_pfil); }
    
    // Decrement the reference count of the index to which
    // this currently refers and increment pfil's reference count
    const CFilterPtr& operator=(CFilter* pfil);
    const CFilterPtr& operator=(const CFilterPtr& pfil)
        { return operator=(pfil.m_pfil); }

    ~CFilterPtr() { DecrementNumberOfRefs(); }
    
    CFilter* operator->() const { return m_pfil; }
    operator CFilter*() const { return m_pfil; } 
};  // class CFilterPtr


// --------------------------------------------------------------------------

class CLangEncType;
class CDatabaseType;
class CShwDoc;
class CShwView;

class CFilterUpdate : public CUpdate  // Hungarian: fup
{
private:
    CFilter* m_pfil;
    BOOL m_bToBeDeleted;
    BOOL m_bModified;
    BOOL m_bNameModified;
    BOOL m_bMatchCriteriaModified;
    CIndex* m_pindToWhichFilterIsBeingReapplied;
    
public:
    CFilterUpdate(CFilter* pfil, BOOL bToBeDeleted = FALSE);  // Add and Delete
    CFilterUpdate(CFilter* pfil, const char* pszName,
            const char* pszCondition, BOOL bMatchWhole, MatchSetting matset);
    CFilterUpdate(CIndex* pindToWhichFilterIsBeingReapplied);  // Reapply Filter
    
    CFilter* pfil() const { return m_pfil; }

    BOOL bToBeDeleted() const { return m_bToBeDeleted; }
    BOOL bModified() const { return m_bModified; }  
    BOOL bNameModified() const { return m_bNameModified; }
    BOOL bMatchCriteriaModified() const { return m_bMatchCriteriaModified; }
    BOOL bReapplyingFilterToIndex(const CIndex* pind) const;

        // Having modified either the Condition or Match Whole or
        // Match Characters affects which record elements match.
     

    // NOTE: Implementated in update.cpp rather than in fil.cpp
    virtual void UpdateLangEnc(CLangEnc* plng);
    virtual void UpdateDatabaseType(CDatabaseType* ptyp);
    virtual void UpdateShwDoc(CShwDoc* pdoc);
    virtual void UpdateShwView(CShwView* pview);
};  // class CFilterUpdate


// --------------------------------------------------------------------------

class CFilterSet : public CSet  // Hungarian: filset
{
private:
    CMarkerSet* m_pmkrset;
    
public:
    CFilterSet(CMarkerSet* pmkrset);
    ~CFilterSet() {}

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);
    BOOL bReadFilterRef(Object_istream& obs, CFilter*& pfil);
    void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
        // Upgrade old Sh2 Key-String conditions to Sh3 Marker-Text elements
            
    BOOL bAddFilter(const char* pszName, const CFilterElList& fellst,
            BOOL bMatchWhole, MatchSetting matset,  
            CFilter** ppfil, CNote** ppnot);

    CMarkerSet* pmkrset() const { return m_pmkrset; }
    CLangEncSet* plngset() const { return m_pmkrset->plngset(); }                                           
    CVarSet* pvarset() const { return m_pmkrset->plngset()->pvarset(); }
    
    // Overrides of First, Next, etc. needed for type safe return value
    CFilter* pfilSearch( const char* psz ) const // Perform a search
        { return (CFilter*) pselSearch( psz ); }
    
    CFilter* pfilFirst() const // First filter
        { return (CFilter*) pselFirst(); }
    CFilter* pfilLast() const // Last filter
        { return (CFilter*) pselLast(); }
    CFilter* pfilNext( const CFilter* pfilCur ) const // after current
        { return (CFilter*) pselNext( pfilCur ); }
    CFilter* pfilPrev( const CFilter* pfilCur ) const // before current
        { return (CFilter*) pselPrev( pfilCur ); }
        
    // InsertBefore/After omitted, since each filter must be unique.    
    // Add new filter in correct order.
    void Add( CFilter* pfilNew );
    
    // Removes the filter from the filter set without deleting the filter.
    CFilter* pfilRemove( CFilter** ppfil )
        { return (CFilter*)CDblList::pelRemove( (CDblListEl**)ppfil); }
    
    void Delete( CFilter** ppfil ) // Delete filter, set pointer to NULL
	{ CSet::Delete( (CSetEl**) ppfil ); }
            
    BOOL bModalView_Elements(CFilter* pfilSelect);
    
    virtual BOOL bAdd(CSetEl** ppselNew);
    virtual void Delete(CSetEl** ppsel);
};  // class CFilterSet

class CFilterConMarkerString;  // forward reference

// **************************************************************************

// Abstract Base Class from which a filter condition class is derived
// for each particular kind.

typedef BOOL (* PFNbConstruct)(const char**, CFilterCon**, Str8& sErr);

class CFilterCon : public CObject  // Hungarian: con
{
public:                                                   
    static BOOL s_bConstruct(const CFilterElList& fellst,
            CFilterCon **ppcon, CNote** ppnot);
                                
    virtual ~CFilterCon() {}
    
    // Return whether the record matches the filter condition.
    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const = 0;
    BOOL bMatchField(const CField* pfld, const CPatMChar *m_ppat,
            CFilterMatch& filmat) const;

    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    virtual BOOL bFilterDependsOnRecordContext() const { return FALSE; } // default implementation

    virtual BOOL bCol() const;

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual CFilterCon* pconCopy() const;
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual BOOL bModify();  // Override in modifiable derived classes

    virtual void DrawElementText(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int xElement, int xText) const;
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;
    void DrawSpace(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

#ifdef _DEBUG
    virtual void AssertValid() const = 0;
#endif  // _DEBUG   

protected:
    // Syntax of the textual form of filter condition objects.
    // NOTE: the constructors really correspond to levels of precedence--
    // if the named token is not found, they default to the next level.
    // NOTE: white space (zero or more of space or newline) is implied
    // before every literal token, except inside square brackets.
    // Case is not significant in filter condition keywords.
    
    // AndOr := Not (("AND" | "OR") Not)*   
    // Not := ("NOT" | "!") Paren
    // Paren := "(" AndOr ")" | With
    // With := (BracketMarkerString ("WITH" BracketMarkerString)*) | Bracket
    // NOTE NOTE NOTE: The column WITH filter can consist only of
    //      Marker-String filter conditions. If, however, there is no WITH,
    //      any bracketed condition must be parsed.
    // Bracket := "[" {
    //                      "KEY|" Pattern
    //                  |   "SEARCH|" Pattern
    //                  |   "FIELD|" Marker
    //                  |   Marker "|" Pattern
    //                  |   "DATE|" Condition "|" Date
    //                  |   "NON-UNIQUE"
    //                                                  } "]"
    // NOTE:
    //      Pattern corresponds to CPatMChar::bConstruct()
    //      In patterns backslash must be used to escape itself,
    //      square brackets, and vertical bar.
    //      Marker corresponds to the CMarker::s_bMatchNameAt().
    
    static BOOL s_bConstructAndOr(const CFilterElList& fellst,
            const CFilterEl** ppfel, CFilterCon **ppcon, CNote** ppnot);
    static BOOL s_bConstructNot(const CFilterElList& fellst,
            const CFilterEl** ppfel, CFilterCon **ppcon, CNote** ppnot);
    static BOOL s_bConstructParen(const CFilterElList& fellst,
            const CFilterEl** ppfel, CFilterCon **ppcon, CNote** ppnot);    
    static BOOL s_bConstructWith(const CFilterElList& fellst,
            const CFilterEl** ppfel, CFilterCon **ppcon, CNote** ppnot);
    static BOOL s_bConstructTerminal(const CFilterElList& fellst,
            const CFilterEl** ppfel, CFilterCon **ppcon, CNote** ppnot);
};  // class CFilterCon

class CPatMChar;

// --------------------------------------------------------------------------

// Abstract Base Class for conditions which can constrain a text pattern
// to match at a particular column offset in the fields of the record.

typedef unsigned int COL;  // zero-based offset into lines of field contents

class CFilterConCol : public CFilterCon
{
public:
    virtual ~CFilterConCol() {}
    
    // Return whether the record matches the filter condition.
    virtual BOOL bMatchCol(const CRecLookEl* prel, CFilterMatch& filmat,
            COL colMatchAt) const = 0;  
    virtual BOOL bCol() const;
};


// ==========================================================================

// Match the record if the pattern matches the contents of
// any of its fields which have the specified marker.
// NOTE NOTE NOTE: Specify the marker withOUT a backslash.
class CFilterConMarkerString : public CFilterConCol  // e.g. [d|in]
{
protected:
    friend class CFilterConWith;
    Str8 m_sMarker;          // e.g. d
    CPatMChar *m_ppatString;    // e.g. in
    Str8 m_sText;
    CFilterSet* m_pfilsetMyOwner;

public:
    CFilterConMarkerString(const char* pszMarker, CPatMChar *ppatString,
            const char* pszText = "", CFilterSet* pfilsetMyOwner = NULL);
    virtual ~CFilterConMarkerString();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    virtual BOOL bMatchCol(const CRecLookEl* prel, CFilterMatch& filmat,
                                            COL colMatchAt) const;  

    static BOOL s_bConstruct(const char* pszMarker, const char* pszText,
            CFilterSet* pfilsetMyOwner,
            CFilterCon **ppcon, CNote** ppnot);
#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual CFilterCon* pconCopy() const;
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual BOOL bModify();
    BOOL bModifyProperties(const char* pszMarker, const char* pszText,
            CNote** ppnot);
    const Str8& sMarker() const { return m_sMarker; }
    CFilterSet* pfilsetMyOwner() const { return m_pfilsetMyOwner; }
    const Str8& sText() const { return m_sText; }

    virtual void DrawElementText(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int xElement, int xText) const;
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

private:
    CLangEnc* plngText() const;  // 1998-02-13
    
public:
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConMarkerString


// --------------------------------------------------------------------------

// Match the record if the pattern matches the contents of
// any of its fields which have the specified language encoding.
class CFilterConLangEncString : public CFilterCon
{
protected:
    friend class CLangEncTextDlg;
    CLangEnc* m_plng;
    CPatMChar *m_ppatString;
    Str8 m_sText;
    CFilterSet* m_pfilsetMyOwner;

public:
    CFilterConLangEncString(CLangEnc* plng, CPatMChar *ppatString,
            const char* pszText = "", CFilterSet* pfilsetMyOwner = NULL);
    virtual ~CFilterConLangEncString();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;

    static BOOL s_bConstruct(CLangEnc* plng, const char* pszText,
            CFilterSet* pfilsetMyOwner,
            CFilterCon **ppcon, CNote** ppnot);
#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual CFilterCon* pconCopy() const;
    
    virtual BOOL bModify();
    BOOL bModifyProperties(CLangEnc* plng, const char* pszText,
            CNote** ppnot);
    CLangEnc* plng() const { return m_plng; }
    CFilterSet* pfilsetMyOwner() const { return m_pfilsetMyOwner; }
    const Str8& sText() const { return m_sText; }

    virtual void DrawElementText(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int xElement, int xText) const;
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;
    
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   

private:
    const char* pszNullLangEncName() const;
    const char* pszAnyLangEncName() const;
};  // class CFilterConLangEncString


// --------------------------------------------------------------------------

// Match the record if the pattern matches its key,
// i.e. the contents of its key field.
class CFilterConKeyString : public CFilterCon  // e.g. [KEY|kat]
{
protected:
    CPatMChar *m_ppatString;            // e.g. kat

public:
    CFilterConKeyString(CPatMChar *ppatString);
    virtual ~CFilterConKeyString();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConKeyString


// --------------------------------------------------------------------------

// Match the record if the pattern matches the contents of
// any field within the record, including the key field.
class CFilterConSearchString : public CFilterCon  // e.g. [SEARCH|in]
{
protected:
    CPatMChar *m_ppatString;            // e.g. in

public:
    CFilterConSearchString(CPatMChar *ppatString);
    virtual ~CFilterConSearchString();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConSearchString


// --------------------------------------------------------------------------

// Match the record if specified field marker is present.
class CFilterConFieldMarker : public CFilterCon  // e.g. [FIELD|p]
{
protected:
    Str8 m_sMarker;          // e.g. p

public:
    CFilterConFieldMarker(const char* pszMarker);
    virtual ~CFilterConFieldMarker() {}

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConFieldMarker


// --------------------------------------------------------------------------

// Match the record if the date matches the contents of
// any of its fields which have the specified marker.
// NOTE NOTE NOTE: Specify the marker withOUT a backslash.

class CDateCon;  // date.h

class CFilterConDate : public CFilterCon  // e.g. [DATE|dt|BEFORE|27/Feb/95]
{
protected:
    Str8 m_sMarker;  // e.g. dt
    CDateCon* m_pdat;  // 1998-04-08 MRP
    CFilterSet* m_pfilsetMyOwner;

public:
    CFilterConDate(const char* pszMarker, const CDateCon& dat,
            CFilterSet* pfilsetMyOwner = NULL);
    virtual ~CFilterConDate();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;

    static BOOL s_bConstruct(const char* pszMarker,
            int iRelation, const Str8& sDate, CFilterSet* pfilsetMyOwner,
            CFilterCon** ppcon, CNote** ppnot);
#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, const Str8& sMarker,
            CFilterSet* pfilsetMyOwner, CFilterCon** ppcon);  // 1998-04-08 MRP
    virtual CFilterCon* pconCopy() const;
    
    virtual BOOL bModify();
    BOOL bModifyProperties(const char* pszMarker, int iRelation,
            const Str8& sDate, CNote** ppnot);
    const Str8& sMarker() const { return m_sMarker; }
    CFilterSet* pfilsetMyOwner() const { return m_pfilsetMyOwner; }
    int iRelation() const;
    const Str8& sDate() const;

    virtual void DrawElementText(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int xElement, int xText) const;
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

private:
    CLangEnc* plngDate() const;  // 1998-02-13 MRP
    
public:
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConDate


// ==========================================================================

// Match the record if has the same key as another record.
class CFilterConNonUnique : public CFilterCon  // i.e. [NON-UNIQUE]
{
public:
    CFilterConNonUnique() {}
    virtual ~CFilterConNonUnique() {}

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;

    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const { return TRUE; }

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;
    
#ifdef _DEBUG
    virtual void AssertValid() const {}
#endif  // _DEBUG   
};  // class CFilterConNonUnique


// ==========================================================================

// NOTE NOTE NOTE:
// Just as the filter "owns" its condition, conditions WITH, AND, OR, NOT
// "own" one or two sub-condition(s), and therefore the destructors delete
// the sub-condition object(s).
// So DO NOT embed a sub-condition instance in more than one condition!


class CFilterConWith : public CFilterConCol  // [mb|de] WITH [ps|PREP]
{
protected:
// 1995-02-20 MRP: reverse role of sub-conditions temporarily
// because I have with associating wrong in bConstructWith.
//  CFilterConMarkerString *m_pconA;    // e.g. [mb|de]
//  CFilterConCol *m_pconB;             // e.g. [ps|PREP]
    CFilterConCol *m_pconA;                 // e.g. [mb|de]
    CFilterConMarkerString *m_pconB;        // e.g. [ps|PREP]

public:
//  CFilterConWith(CFilterConMarkerString *pconA, CFilterConCol *pconB);
    CFilterConWith(CFilterConCol *pconA, CFilterConMarkerString *pconB);
    virtual ~CFilterConWith();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    virtual BOOL bMatchCol(const CRecLookEl* prel, CFilterMatch& filmat,
                                            COL colMatchAt) const;  

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConWith


// --------------------------------------------------------------------------

// NOTE NOTE NOTE:
// The textual representation of filters gives equal precedence to AND and OR.
// They are evaluated simply from left to right. This is different from
// the convention in logic, where and has higher precedence than or,
// in analogy with multiplication and addition.

// Match the record if BOTH subconditions match it.
// NOTE: This is a "conditional and"-- B is not used if A does not match.
class CFilterConAnd : public CFilterCon  // [KEY|kat] AND [p|v]
{
protected:
    CFilterCon *m_pconA;        // e.g. [KEY|kat]
    CFilterCon *m_pconB;        // e.g. [p|v]

public:
    CFilterConAnd(CFilterCon *pconA, CFilterCon *pconB);
    virtual ~CFilterConAnd();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;

    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const { return m_pconA->bFilterDependsOnRecordContext() || 
                                                        m_pconB->bFilterDependsOnRecordContext(); }

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConAnd


// --------------------------------------------------------------------------

// Match the record if EITHER subcondition matches it.
// NOTE: This is a "conditional or"-- B is not used if A matches.
class CFilterConOr : public CFilterCon  // e.g. [p|n] OR [p|v]
{
protected:
    CFilterCon *m_pconA;        // e.g. [p|n]
    CFilterCon *m_pconB;        // e.g. [p|v]

public:
    CFilterConOr(CFilterCon *pconA, CFilterCon *pconB);
    virtual ~CFilterConOr();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const { return m_pconA->bFilterDependsOnRecordContext() || 
                                                        m_pconB->bFilterDependsOnRecordContext(); }

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConOr


// --------------------------------------------------------------------------

// Match the record if the subcondition does NOT match it.
class CFilterConNot : public CFilterCon  // e.g. NOT [FIELD|p]
{
protected:
    CFilterCon *m_pcon;         // e.g. [FIELD|p]

public:
    CFilterConNot(CFilterCon *pcon);
    virtual ~CFilterConNot();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const { return m_pcon->bFilterDependsOnRecordContext(); }

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConNot


// --------------------------------------------------------------------------

// Match the record if the enclosed subcondition matches it.
class CFilterConParen : public CFilterCon
{
protected:
    CFilterCon *m_pcon;

public:
    CFilterConParen(CFilterCon *pcon);
    virtual ~CFilterConParen();

    virtual BOOL bMatch(const CRecLookEl* prel, CFilterMatch& filmat) const;
    // 2000-04-27 TLB - Return whether filter cannot be evaluated solely on the merits of the record itself.
    // The non-unique filter condition depends on the previous and next prel's in the index.
    BOOL bFilterDependsOnRecordContext() const { return m_pcon->bFilterDependsOnRecordContext(); }

#ifdef prjWritefstream // 1.6.4aa 
    virtual void WriteProperties(Object_ofstream& obs) const;
#else
    virtual void WriteProperties(Object_ostream& obs) const;
#endif
    virtual void SetRecordMarker(CMarker* pmkrRecord);  // 1996-08-28 MRP
    
    virtual void DrawCondition(CDblListListBox* plbo, CDC& cDC,
            const RECT& rcItem, int* pxLeft, int xRight) const;

#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CFilterConParen

// **************************************************************************


class CFilterSetListBox : public CSetListBox
{
public:
    CFilterSetListBox(CFilterSet* pfilset, CFilter** ppfil) :
        CSetListBox(pfilset, (CSetEl**)ppfil) {}

protected:
    int m_xCondition;
    virtual void InitLabels();
    virtual int iItemHeight();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
    virtual BOOL bIsBold(CSetEl* psel) { return psel->bHasRefs(); }
    virtual BOOL bDelete(CSetEl* psel);
};  // class CFilterSetListBox

#endif  // FIL_H
