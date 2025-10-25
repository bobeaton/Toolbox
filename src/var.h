// var.h Interface for Shoebox variable classes (1995-02-23)

// "A variable in Shoebox is a short name that can be defined to represent
// a set or class of characters. Shoebox's functionality as an analytical
// tool is significantly enhanced by the ability to define variables
// to represent classes of characters. For example, you can easily define
// a variable that contains all the prenasalized stops in the language
// in which you're working. Once defined, this variable can function as
// a metacharacter in filter searches, editor Finds, or parser \dup fields."
// [The Linguist's Shoebox, p. 201. Read Chapter 17 and the "Variables"
// section of Chapter 26.]
 

#ifndef VAR_H
#define VAR_H

#include <iostream>
using namespace std;
#include "set.h"  // classes CSet, CSetEl
#include "ref.h"
#include "not.h"

#include "update.h"  // CUpdate
#include "cbo.h"
#include "lbo.h"

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h

class CMChar;
class CVarSet;
class CVar;

// **************************************************************************

class CMCharsInVarSubSet  // Hungarian: mchsubset
{
private:
    friend class CVar;
    CVarSet* const m_pvarset;

    long m_lBit;  // The bit that represents this subset

    CMCharsInVarSubSet(CVarSet* pvarset);
    ~CMCharsInVarSubSet();

    // Prevent use of the copy constructor and assignment operator
    // by making them private and omitting their definitions.
    CMCharsInVarSubSet(const CMCharsInVarSubSet& mchsubset);
    CMCharsInVarSubSet& operator=(const CMCharsInVarSubSet& mchsubset);

    // These two functions implement the connection to class CMChar.
    BOOL bIncludes(const CMChar* pmch) const;
    void Include(CMChar* pmch, BOOL bInclude);
};  // class CMCharsInVarSubSet

// **************************************************************************

class CVar : public CSetEl  // Hungarian: var    
{
private:
    CMCharsInVarSubSet m_mchsubsetMCharsInVar;  // 1998-01-30 MRP
    CVarSet* m_pvarsetMyOwner;  // 1998-01-30 MRP

    CRefCount m_ref;  // number of object instances referring to this
    
public:
    CVar(const char* pszName, CVarSet* pvarsetMyOwner);
    ~CVar();

    virtual CSet* psetMyOwner();

    NumRefs numRefs() const { return m_ref.numRefs(); }
    BOOL bHasRefs() const { return m_ref.bHasRefs(); }
    void IncrementNumRefs() { m_ref.IncrementNumRefs(); }
    void DecrementNumRefs() { m_ref.DecrementNumRefs(); }

    // Validate or match a variable name
    static const char* s_pszInvalidNameChars;
    static BOOL s_bMatchNameAt(const char** ppszName,
            const char* pszValidDelimiters, Str8& sName, CNote** ppnot);
            
    void Text(Str8& sText) const;

    // 1998-01-30 MRP
    // These two functions implement the connection to class CMChar.
    BOOL bIncludes(const CMChar* pmch) const;
    void Include(CMChar* pmch, BOOL bInclude);
    
#ifdef _DEBUG
    virtual void AssertValid() const;
#endif  // _DEBUG   
};  // class CVar


// --------------------------------------------------------------------------

class CVarSet : public CSet  // Hungarian: varset
{
private:
    CVar* m_pvarPunct;  // 1998-01-31 MRP: Had been global (yuck)
    long m_lSubAllocMCharsInVarm;
        // Each bit that's set represents an allocated CMCharsInVarSubSet object.

    friend class CMCharsInVarSubSet;    
    void AllocateSubsetBit(long& lBit);
        // Called by CMCharsInVarSubSet's constructors to assign a unique bit
        // to the newly allocated object.
    void DeAllocateSubsetBit(long& lBit);
        // Called by CMCharsInVarSubSet's destructor to make the bit available again.
    
public:
    CVarSet();
    virtual ~CVarSet() {}
    
    // Search for variable by name (case is not significant), e.g. "nasal"
    CVar* pvarSearch( const char* pszName ) const
        { return (CVar*) pselSearch( pszName ); }
        
    CVar* pvarFirst() const  // First variable
        { return (CVar*) pelFirst(); }
    CVar* pvarLast() const  // Last variable
        { return (CVar*) pelLast(); }
    CVar* pvarNext( const CVar* pvarCur ) const  // after current
        { return (CVar*) pelNext( pvarCur ); }
    CVar* pvarPrev( const CVar* pvarCur ) const  // before current
        { return (CVar*) pelPrev( pvarCur ); }

    CVar* pvarAdd( const char* pszNameNew );  // Add new variable in correct order  
    CVar* pvarSearch_AddIfNew( const char* pszName );
    // NOTE: InsertBefore/After omitted, since each variable must be unique.    
    // NOTE: Cannot delete a variable to which any CPatElVar instances refer.
    void Delete( CVar** ppvar )  // Delete variable, set pointer to NULL
	{ CSet::Delete( (CSetEl**)ppvar ); }
        
    CVar* pvarPunct() { return m_pvarPunct; }
    
#ifdef _DEBUG
    void AssertValid() const;
#endif      
};  // class CVarSet


class CVarSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CVarSetComboBox(CVarSet* pvarset) : CSetComboBox(pvarset) {}
    
    void UpdateElements(CVar* pvarToSelect)
        { CSetComboBox::UpdateElements( pvarToSelect ); }
    void ChangeSet(CVarSet* pvarset, CVar* pvarToSelect)
        { CSetComboBox::ChangeSet( pvarset, pvarToSelect ); }
    CVar* pvarSelected() { return (CVar*)CSetComboBox::pselSelected(); }
};  // class CVarSetComboBox


// --------------------------------------------------------------------------

class CLangEnc;

class CVarInstance : public CSetEl  // Hungarian: vin
{
private:
    CVar* m_pvar;
    Str8 m_sChars;
    
    friend class CVarInstanceSet;
    CVarInstanceSet* m_pvinsetMyOwner;
    
    CVarInstance(const char* pszName, CVar* pvar, const char* pszChars,
            CVarInstanceSet* pvinsetMyOwner);
    
#ifdef lngWritefstream // 1.6.4ab 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
            CVarInstanceSet* pvinsetMyOwner, CVarInstance** ppvin);
    static BOOL s_bConstruct(const char* pszName, const char* pszChars,
            CVarInstanceSet* pvinsetMyOwner,
            CVarInstance** ppvin, CNote** ppnot);

public:
    const Str8& sChars() const { return m_sChars; }

    CVarInstanceSet* pvinsetMyOwner() const { return m_pvinsetMyOwner; }
    virtual CSet* psetMyOwner();
    CLangEnc* plng() const;
    
    BOOL bModifyProperties(const char* pszName, const char* pszChars, CNote** ppnot);

    virtual BOOL bCopy(CSetEl** ppselNew);          
    virtual BOOL bModify();
        // Let the user view and possibly modify this element's properties.
        // Returns whether the user clicked OK (vs. Cancel).
        // TRUE (OK): This element may have been modified (if it was,
        //     an update hint should have been broadcast to all views).
        // FALSE (Cancel): This element definitely was not modified.

    virtual BOOL bDeletable();
        // Return whether this element may be deleted (i.e. when this element
        // is selected, should the Delete button be enabled?).
        // E.g. a marker which has references to it cannot be deleted.
};  // class CVarInstance

// --------------------------------------------------------------------------

class CVarInstanceSet : public CSet  // Hungarian: vinset
{
private:
    CLangEnc* m_plngMyOwner;
    
public:
    CVarInstanceSet(CLangEnc* plngMyOwner);
    
#ifdef lngWritefstream // 1.6.4ab 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);
        
    CVarInstance* pvinFirst() const  // First variable
        { return (CVarInstance*) pelFirst(); }
    CVarInstance* pvinLast() const  // Last variable
        { return (CVarInstance*) pelLast(); }
    CVarInstance* pvinNext( const CVarInstance* pvinCur ) const  // after current
        { return (CVarInstance*) pelNext( pvinCur ); }
    CVarInstance* pvinPrev( const CVarInstance* pvinCur ) const  // before current
        { return (CVarInstance*) pelPrev( pvinCur ); }
    CVarInstance* pvinSearch( const char* pszName ) const
        { return (CVarInstance*) pselSearch( pszName ); }
        
    CLangEnc* plngMyOwner() const { return m_plngMyOwner; }
    CVarSet* pvarset() const;
    
    virtual BOOL bValidName(const char* pszName, CNote** ppnot);
    BOOL bValidNewElementName(const char* pszName, CNote** ppnot);
    void SetVarInstanceName(CVarInstance* pvin, const char* pszNewName);

    BOOL bAddVarInstance(const char* pszName, const char* pszChars,
            CVarInstance** ppvin, CNote** ppnot);
    
    BOOL bViewElements();
    virtual BOOL bAdd(CSetEl** ppselNew);
        // Let the user add a new element to this set.
        // (Show the same dialog box used to view an existing element.)
        // Returns whether the user clicked OK (vs. Cancel).
        // TRUE (OK): ppselNew returns the new element (an update hint
        //     should have been broadcast to all views).
        // FALSE (Cancel): No element was added to this set.
};  // class CVarInstanceSet

class CVarInstanceSetListBox : public CSetListBox
{
public:
    CVarInstanceSetListBox(CVarInstanceSet* pvinset, CVarInstance** ppvin) :
        CSetListBox(pvinset, (CSetEl**)ppvin) {}

protected:
    int m_xName;
    int m_xChars;
    
    virtual void InitLabels();
    virtual int iItemHeight();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
};  // class CVarInstanceSetListBox

// **************************************************************************

#endif  // VAR_H
