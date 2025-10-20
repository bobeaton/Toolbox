// kdb.h  Interface for a language encoding's keyboarding mechanism

#ifndef KBD_H
#define KBD_H

#include "set.h"  // classes CSet and CSetEl
#include "km95api.h"

#include "cbo.h"  // class CSetComboBox

class Object_istream;  // obstream.h // 1.6.1cb 
class Object_ostream;  // obstream.h // 1.6.1cb 
class Object_ofstream;  // obstream.h // 1.6.1cb  // 1.6.4aa 


// **************************************************************************

// Class CKeyboard encapsulates the conversion from user input keystrokes
// on the standard [physical] keyboard to the appropriate character codes
// to be inserted into text in a particular language encoding.
// A language encoding's properties include its keyboard (or no keyboard);
// more that one language encoding can refer to the same keyboard.

// The only keyboarding mechanisn supported at present is Keyman,
// which actually does the work of converting keystrokes. This class
// corresponds to a .kmn Keyman program file, but is responsible only
// to cause Keyman to select the corresponding keyboard. 
// See Marc Durdin, Tavultesoft Keyboard Manager User's Guide and Reference.
// Tavultesoft, 1994. (Version 3.1)

class CKeyboard : public CSetEl  // Hungarian: kbd
{
private:
    // Note: the name is in the base classes, see: m_sKey and sName()
    
    friend class CKeyboardSet;
    CKeyboardSet* m_pkbdsetMyOwner;

#ifdef _MAC
    // Macintosh
    CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner);
#else
    // Windows 95 and later, Keyman 4 and later
    BOOL m_bLoaded;
    HKL m_hkl;
	Str8 m_sKeyboardNumber; // 1.1vc Store keyboard number

    CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner);
    CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner, HKL hkl);
    CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner, HKL hkl, const char* pszKeyboardNumber); // 1.1vc Store keyboard number
#endif
    
public:
    ~CKeyboard() {}
    
    CKeyboardSet* pkbdsetMyOwner() const { return m_pkbdsetMyOwner; }
    virtual CSet* psetMyOwner();
    
    virtual void Activate(); // Activate this keyboard

	void SetKeyboardNumber( LPCSTR s ) { m_sKeyboardNumber = s; } // 1.1vc Store keyboard number
	LPCSTR sKeyboardNumber() { return m_sKeyboardNumber; } // 1.1vc Store keyboard number

    // To be called in a language encoding's Write and ReadProperties()
#ifdef lngWritefstream // 1.6.4ab 
    void WriteRef(Object_ofstream& obs) const;
    static void s_WriteKeyboardWin32(Object_ofstream& obs, const Str8& sName);
    static void s_WriteKeyboardWin16(Object_ofstream& obs, const Str8& sName);
#else
    void WriteRef(Object_ostream& obs) const;
    static void s_WriteKeyboardWin32(Object_ostream& obs, const Str8& sName);
    static void s_WriteKeyboardWin16(Object_ostream& obs, const Str8& sName);
#endif

    static BOOL s_bReadKeyboardWin32(Object_istream& obs, Str8& sName);
    static BOOL s_bReadKeyboardWin16(Object_istream& obs, Str8& sName);

    // Define these if we eventually integrate TIKE with Shoebox
    BOOL bCopy(CSetEl** ppselNew) { return FALSE; }
    BOOL bModify() { return FALSE; }
    BOOL bDeletable() { return FALSE; }
};  // class CKeyboard

// class CKeyboardPtr is declared in ptr.h but implemented in kbd.cpp


// --------------------------------------------------------------------------

// Typedefs for references to Keyman DLL functions.
typedef BOOL (WINAPI *T_Keyman_ForceKeyboard)(PCSTR FileName);
typedef BOOL (WINAPI *T_Keyman_StopForcingKeyboard)();

class CKeyboardSet : public CSet  // Hungarian: kbdset
{
private:
    BOOL m_bAutoSwitching;

#ifndef _MAC
    // Windows 95 and later, Keyman 4 and later
    CKeyboard* m_pkbdDefault;
    HINSTANCE m_hKeyman;
	UINT m_wm_kmselectlang;
public:
    T_Keyman_ForceKeyboard m_f_ForceKeyboard;
    T_Keyman_StopForcingKeyboard m_f_StopForcingKeyboard;
private:
#endif
    
    void EnumerateLoadedKeyboards();
    
public:
    CKeyboardSet();
    ~CKeyboardSet();


#ifdef prjWritefstream // 1.6.4aa 
    void WriteAutoSwitching(Object_ofstream& obs);
#else
    void WriteAutoSwitching(Object_ostream& obs);
#endif
    BOOL bReadAutoSwitching(Object_istream& obs);
    BOOL bReadKeyboardRef(Object_istream& obs, const char* pszQualifier,
            CKeyboard*& pkbd);
        // To be called in a language encoding's s_bReadProperties()
    
    CKeyboard* pkbdFirst() const
        { return (CKeyboard*)pselFirst(); }
    CKeyboard* pkbdLast() const
        { return (CKeyboard*)pselLast(); }
    CKeyboard* pkbdNext( const CKeyboard* pkbdCur ) const
        { return (CKeyboard*)pselNext( pkbdCur ); }
    CKeyboard* pkbdPrev( const CKeyboard* pkbdCur ) const
        { return (CKeyboard*)pselPrev( pkbdCur ); }
    CKeyboard* pkbdSearch( const char* pszName, BOOL bWhole = whole ) const
        { return (CKeyboard*)pselSearch( pszName, bWhole ); }
    CKeyboard* pkbdSearch_AddIfNew(const Str8& sName);
	CKeyboard* pkbdSearch_DefaultIfNew(const Str8& sName); // Search, return default if not found

    CKeyboard* pkbdGetCurrent();
        // Called to obtain the current Keyman keyboard.
        // Will return NULL if Keyman has no keyboard or if the current Keyman
        // keyboard is not available in the Keyboard set.

    BOOL bAutoSwitching() const { return m_bAutoSwitching; }
    void SetAutoSwitching(BOOL b) { m_bAutoSwitching = b; }

    void ActivateDefaultKeyboard();  // 1999-09-30 MRP
    void DeActivateKeyboards();
        // Cause Keyman to stop converting the user's input keystrokes

#ifndef _MAC
    void ActivateKeyboard(HKL hkl);
#endif

    // Define this if we eventually integrate TIKE with Shoebox
    BOOL bAdd(CSetEl** ppselNew) { return FALSE; }
};  // class CKeyboardSet

// The classes in this file model the keyboard data. A keyboard manager
// uses them to encapsulate an application's dynamic keyboard state.
// See class CKeyboardManager in kmn.h


// --------------------------------------------------------------------------

class CKeyboardSetComboBox : public CSetComboBox  // Hungarian: cbo
{
public:
    CKeyboardSetComboBox(CKeyboardSet* pkbdset) :
#ifdef _MAC
        CSetComboBox(pkbdset, TRUE)
#else
        CSetComboBox(pkbdset, TRUE, NULL) // 1.4gg Show [none] if no keyboard selected
#endif
        {}
    
    void UpdateElements(CKeyboard* pkbdToSelect)
        { CSetComboBox::UpdateElements( pkbdToSelect ); }
    CKeyboard* pkbdSelected()
        { return (CKeyboard*)CSetComboBox::pselSelected(); }
};  // class CKeyboardSetComboBox

// **************************************************************************

#endif  // KBD_H
