// kdb.cpp  Implementation of a language encoding's keyboarding mechanism

#include "stdafx.h"
#include "toolbox.h"
#include "kbd.h"
#include "ptr.h"  // CKeyboardPtr
#include "obstream.h"  // Object_istream, Object_ostream
#include "mkr.h"  // CMarker (and CLangEnc)
#include "project.h"

// **************************************************************************

#ifdef _MAC
// Macintosh
CKeyboard::CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner) :
    CSetEl(pszName)
{
    m_pkbdsetMyOwner = pkbdsetMyOwner;
    ASSERT( m_pkbdsetMyOwner );
}
#else
// Windows 95 and later, Keyman 4 and later
CKeyboard::CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner) :
    CSetEl(pszName)
{
    m_pkbdsetMyOwner = pkbdsetMyOwner;
    ASSERT( m_pkbdsetMyOwner );
    m_bLoaded = FALSE;
    m_hkl = 0;
}

CKeyboard::CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner, HKL hkl) :
    CSetEl(pszName)
{
    m_pkbdsetMyOwner = pkbdsetMyOwner;
    ASSERT( m_pkbdsetMyOwner );
    m_bLoaded = TRUE;
    m_hkl = hkl;
}

CKeyboard::CKeyboard(const char* pszName, CKeyboardSet* pkbdsetMyOwner, HKL hkl, const char* pszKeyboardNumber) :
    CSetEl(pszName)
{
    m_pkbdsetMyOwner = pkbdsetMyOwner;
    ASSERT( m_pkbdsetMyOwner );
    m_bLoaded = TRUE;
    m_hkl = hkl;
	m_sKeyboardNumber = pszKeyboardNumber;
}
#endif

static const char* psz_kbdWin16 = "kbdKeyman";
static const char* psz_kbdWin32 = "kbdWin32";

#ifdef lngWritefstream // 1.6.4ab 
void CKeyboard::WriteRef(Object_ofstream& obs) const
#else
void CKeyboard::WriteRef(Object_ostream& obs) const
#endif
{
    s_WriteKeyboardWin32(obs, sName());
}

#ifdef lngWritefstream // 1.6.4ab 
void CKeyboard::s_WriteKeyboardWin32(Object_ofstream& obs, const Str8& sName)
#else
void CKeyboard::s_WriteKeyboardWin32(Object_ostream& obs, const Str8& sName)
#endif
{
    obs.WriteString(psz_kbdWin32, sName);
}

#ifdef lngWritefstream // 1.6.4ab 
void CKeyboard::s_WriteKeyboardWin16(Object_ofstream& obs, const Str8& sName)
#else
void CKeyboard::s_WriteKeyboardWin16(Object_ostream& obs, const Str8& sName)
#endif
{
    obs.WriteString(psz_kbdWin16, sName);
}

BOOL CKeyboard::s_bReadKeyboardWin32(Object_istream& obs, Str8& sName)
{
    return obs.bReadString(psz_kbdWin32, sName);
}

BOOL CKeyboard::s_bReadKeyboardWin16(Object_istream& obs, Str8& sName)
{
    return obs.bReadString(psz_kbdWin16, sName);
}

CSet* CKeyboard::psetMyOwner()
{
    return m_pkbdsetMyOwner;
}

BOOL bKeymanKeyboard( CKeyboard* pkbd )
{
	Str8 sKeyboardName = pkbd->sName();
	return ( sKeyboardName.Find( ".kmx" ) > 0 ); // If it is a keyman file name, return true
}

void CKeyboard::Activate()
{
#ifndef _MAC
    // Windows 95 and later, Keyman 4 and later
	if ( m_pkbdsetMyOwner->m_f_StopForcingKeyboard ) // If a Keyman file was forced on, unforce it
		m_pkbdsetMyOwner->m_f_StopForcingKeyboard();
	if ( bKeymanKeyboard( this ) ) // If it is a Keyman file name, force it on
	{
		if ( m_pkbdsetMyOwner->m_f_ForceKeyboard )
			m_pkbdsetMyOwner->m_f_ForceKeyboard( sName() );
	}
    else 
		m_pkbdsetMyOwner->ActivateKeyboard(m_hkl);
#endif
}   


// --------------------------------------------------------------------------

void CKeyboardPtr::IncrementNumberOfRefs(CKeyboard* pkbd)
{
    m_pkbd = pkbd;
    if ( m_pkbd )
        m_pkbd->IncrementNumberOfRefs();
}

void CKeyboardPtr::DecrementNumberOfRefs()
{
    if ( m_pkbd )
        m_pkbd->DecrementNumberOfRefs();
}


const CKeyboardPtr& CKeyboardPtr::operator=(CKeyboard* pkbd)
{
    if ( m_pkbd != pkbd )
        {
        DecrementNumberOfRefs();
        IncrementNumberOfRefs(pkbd);
        }
        
    return *this;
}


// --------------------------------------------------------------------------

CKeyboardSet::CKeyboardSet()
{
    m_bAutoSwitching = TRUE; // AB 

#ifndef _MAC
    // Windows 95 and later, Keyman 4 and later
    m_pkbdDefault = NULL;
    m_hKeyman = NULL;
    m_f_ForceKeyboard = NULL;
    m_f_StopForcingKeyboard = NULL;
	Str8 sKeymanPath = "C:\\Program Files\\Tavultesoft\\Keyman"; // Default to standard location in case HKEY doesn't find it
    HKEY hkeyKeymanPath = 0; //(keymandllpath can be read from HKLM\Software\Tavultesoft\Keyman\5.0\root path).
    long lSucc = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,  swUTF16( "Software\\Tavultesoft\\Keyman\\5.0\\" ), 0, KEY_READ, &hkeyKeymanPath); // 1.4qyf Upgrade RegOpenKeyEx for Unicode build
    if ( lSucc == ERROR_SUCCESS )
	{
        DWORD dwType = 0;
        char pszName[256];
        DWORD len = 256;
        lSucc = ::RegQueryValueEx(hkeyKeymanPath, swUTF16( "root path" ), NULL, &dwType, (unsigned char*)pszName, &len); // 1.4qyf
        ::RegCloseKey(hkeyKeymanPath);
        if ( lSucc == ERROR_SUCCESS )
			sKeymanPath = pszName;
	}
	Str8 sKeymanDLLPath = sKeymanPath + "\\keyman32.dll";
    m_hKeyman = LoadLibrary( swUTF16( sKeymanDLLPath ) ); // Get a handle to the DLL module. // 1.4qye Upgrade LoadLibrary for Unicode build
	if ( m_hKeyman ) // If the handle is valid, try to get the function addresses
	{
		m_f_ForceKeyboard = (T_Keyman_ForceKeyboard)GetProcAddress(m_hKeyman, "Keyman_ForceKeyboard");
		m_f_StopForcingKeyboard = (T_Keyman_StopForcingKeyboard)GetProcAddress(m_hKeyman, "Keyman_StopForcingKeyboard");
		m_wm_kmselectlang = RegisterWindowMessage(  swUTF16( RWM_KMSELECTLANG ) ); // 1.4qyf
	}
#endif // not Mac

#ifdef KeymanWorkaround_not_enough
	CWnd cwnd; // Open a temp window so Keyman 5 will initialize correctly
	Str8 sWndClass = AfxRegisterWndClass( NULL );
	cwnd.CreateEx( WS_EX_TOPMOST, sWndClass, "Temp", WS_VISIBLE , 0, 0, 100, 100, NULL, 0 );
//	AfxMessageBox( "Temp window created" );
#endif
    EnumerateLoadedKeyboards();
}

CKeyboardSet::~CKeyboardSet()
{
#ifndef _MAC
    // Unload the Keyman module
    if ( m_hKeyman )
        {
        FreeLibrary(m_hKeyman);
        m_hKeyman = NULL;
		m_f_ForceKeyboard = NULL;
	    m_f_StopForcingKeyboard = NULL;
        }
#endif
}

static const char* psz_AutoSwitching = "AutomaticKeyboardSwitching";

#ifdef prjWritefstream // 1.6.4aa 
void CKeyboardSet::WriteAutoSwitching(Object_ofstream& obs)
#else
void CKeyboardSet::WriteAutoSwitching(Object_ostream& obs)
#endif
{
    obs.WriteBool(psz_AutoSwitching, m_bAutoSwitching);
}

BOOL CKeyboardSet::bReadAutoSwitching(Object_istream& obs)
{
    return obs.bReadBool(psz_AutoSwitching, m_bAutoSwitching);
}

CKeyboard* CKeyboardSet::pkbdSearch_AddIfNew(const Str8& sName)
{
    CKeyboard* pkbd = pkbdSearch(sName);
    if ( !pkbd )
        {
        // Note -- a newly found keyboard will not be in use in Keyman.
        pkbd = new CKeyboard(sName, this);
        Add(pkbd);
        }
    
    return pkbd;
}

// Search for keyboard, return default keyboard if not found
// This is to allow correction of keyboard name when lng moves to another system
CKeyboard* CKeyboardSet::pkbdSearch_DefaultIfNew(const Str8& sName) // 1.1hg Fix keyboard switch failures from missing or default keyboard
{
    CKeyboard* pkbd = pkbdSearch(sName); // See if full match of name
	if ( !pkbd ) // 1.1vc If not found, use default keyboard of machine
		pkbd = m_pkbdDefault;
    if ( !pkbd ) // 1.1vc If there was no default keyboard set, then search for a possible one
		pkbd = pkbdSearch( "US", FALSE ); // Look for known default keyboards
    if ( !pkbd )
		pkbd = pkbdSearch( "United", FALSE ); // United States 101 and United Kingdom
    if ( !pkbd )
		pkbd = pkbdSearch( "Europe", FALSE ); // European
    if ( !pkbd )
		pkbd = pkbdSearch( "English", FALSE ); // English-European
    if ( !pkbd )
		pkbd = pkbdLast(); // If all else fails, take the last keyboard in the list (list seems backward from listed in Language settings)
	if ( !pkbd ) // 1.4pd Fix bug of keyboard not found problem on Linux wine
		pkbd = pkbdSearch_AddIfNew( "Default" ); // 1.4pd If no keyboards available, make a default
	ASSERT( pkbd ); // Should always be a keyboard
    
    return pkbd;
}

#ifdef _MAC
// Macintosh
void CKeyboardSet::EnumerateLoadedKeyboards()
{
}
#else
// Windows 95 and later, Keyman 4 and later
void CKeyboardSet::EnumerateLoadedKeyboards()
{
    Str8 sKey1 = "System\\CurrentControlSet\\Control\\keyboard layouts\\";
    HKEY hkey = 0;
    long lReturn1 = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,  swUTF16( sKey1 ), 0, KEY_READ, &hkey); // 1.4qyf

    UINT nKeyboards = GetKeyboardLayoutList(0, NULL);
    if ( nKeyboards == 0 )
        return;

    HKL hklRestore = GetKeyboardLayout(0);

    HKL* phkl = new HKL[nKeyboards];
    UINT nGotten = GetKeyboardLayoutList(nKeyboards, phkl);
    ASSERT( nGotten == nKeyboards );

    UINT i = 0;
    for ( ; i != nKeyboards; i++ )
        {
        HKL hkl = phkl[i];

        ActivateKeyboardLayout(hkl, 0);
        TCHAR pszRegistry[KL_NAMELENGTH]; // Keyboard number, as a string, eg 00000409 // 1.4qyf
        if ( !GetKeyboardLayoutName(pszRegistry) ) // Get keyboard number
            continue;

        long lReturn = ERROR_SUCCESS;
        Str8 sKey = "System\\CurrentControlSet\\Control\\keyboard layouts\\";
        sKey +=  sUTF8( pszRegistry ); // 1.4qyf
        HKEY hkey = 0;
        lReturn = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,  swUTF16( sKey ), 0, KEY_READ, &hkey); // 1.4qyf
        if ( lReturn != ERROR_SUCCESS )
            continue;

        DWORD dwType = 0;
        char pszName[256];
        DWORD len = 256;
        lReturn = ::RegQueryValueEx(hkey, swUTF16( "layout text" ), NULL, &dwType, (unsigned char*)pszName, &len); // 1.4qyf
        ::RegCloseKey(hkey);
        if ( lReturn != ERROR_SUCCESS )
            continue;
		Str8 sName = pszName; // 1.4qzhn
#ifdef UNICODE // 1.4qzhn
		CString swName = (unsigned short*)pszName; // 1.4qzhn
		sName = sUTF8( swName ); // 1.4qzhn Fix U problem of not recognizing keyboard names
#endif
        if ( pkbdSearch(sName) ) // 1.4qzhn
            continue;

        Add(new CKeyboard(sName, this, hkl,  sUTF8( pszRegistry)) ); // 1.1vc Store keyboard number // 1.4qyf // 1.4qzhn
        }

    delete [] phkl;

    ActivateKeyboardLayout(hklRestore, 0);

    // 1999-09-29 MRP: Get the default keyboard layout from Keyboard Properties
    // instead of Regional Settings Properties, because the latter is intended
    // to affect the way that programs display and sort dates, etc.
    // LANGID lidDefault = GetUserDefaultLangID();  // Regional Settings
    // The first "preload" keyboard is the one the user has set as default.
    Str8 sKey = "Keyboard Layout\\Preload\\";
    hkey = 0;
    LONG lReturn = ::RegOpenKeyEx(HKEY_CURRENT_USER,  swUTF16( sKey ), 0, KEY_READ, &hkey); // 1.1vc Try to make find default keyboard work // 1.4qyf
    if ( lReturn != ERROR_SUCCESS )
        return;
    DWORD dwType = 0;
    char pszName[256];
    DWORD len = 256;
    lReturn = ::RegQueryValueEx(hkey, swUTF16( "1" ), NULL, &dwType, (unsigned char*)pszName, &len); // 1.4qyf
    ::RegCloseKey(hkey);
    if ( lReturn != ERROR_SUCCESS )
        return;

	Str8 sKeyboardNumberDefault( pszName ); // 1.1vc Find default keyboard number
    CKeyboard* pkbd = pkbdFirst(); // 1.1vc Find default keyboard number
    for ( ; pkbd; pkbd = pkbdNext(pkbd) )
		{
		Str8 sKeyboardNumber = pkbd->sKeyboardNumber();
//        if ( sKeyboardNumberDefault.CmpNoCase( pkbd->sKeyboardNumber() ) == 0 ) // 1.1wa Fix bug of not finding correct default keyboard because of case difference in hex digits in different registry entries // 1.4hej Change CompareNoCase to CmpNoCase for Str8 // 1.4qzgc
		if ( _stricmp( sKeyboardNumberDefault, pkbd->sKeyboardNumber() ) == 0 ) // 1.4qzgc Eliminate Cmp and CmpNoCase
            m_pkbdDefault = pkbd;
		}

/*    long lDefaultLanguage;
    if ( sscanf(pszName, "%lx", &lDefaultLanguage) != 1 )
        return;

    LANGID lidDefault = LOWORD(lDefaultLanguage);
    CKeyboard* pkbd = pkbdFirst();
    for ( ; pkbd; pkbd = pkbdNext(pkbd) )
        if ( lidDefault == LOWORD(pkbd->m_hkl) )
            m_pkbdDefault = pkbd;
*/
}
#endif

#ifndef _MAC
// Windows 95 and later, Keyman 4 and later
void CKeyboardSet::ActivateKeyboard(HKL hkl)
{
    if ( hkl )
        ::ActivateKeyboardLayout(hkl, 0);
}
#endif

void CKeyboardSet::DeActivateKeyboards()
{
#ifndef _MAC
	if ( m_f_StopForcingKeyboard )
		m_f_StopForcingKeyboard();
#endif
    ActivateDefaultKeyboard();  // 1999-09-30 MRP
}

void CKeyboardSet::ActivateDefaultKeyboard()
{
#ifndef _MAC
    // Windows 95 and later, Keyman 4 and later
	// If there is a general default, use it
    if ( m_pkbdDefault ) // && ( m_pkbdDefault->m_bLoaded || bKeymanKeyboard( m_pkbdDefault ) ) ) // AB Always call default activate
        m_pkbdDefault->Activate();
	else // Else (no general default), use the keyboard of the default language, if it has one
		{
		CLangEnc* plngDefault = Shw_plngset()->plngDefault();
		if ( plngDefault )
			{
			CKeyboard* pkbdDefault = plngDefault->pkbd();
			if ( pkbdDefault )
				pkbdDefault->Activate();
			}
		}
#endif
}

#ifdef _MAC
CKeyboard* CKeyboardSet::pkbdGetCurrent()
{
    return NULL;
}
#else
CKeyboard* CKeyboardSet::pkbdGetCurrent()
{
    HKL hkl = GetKeyboardLayout(0);
    CKeyboard* pkbd = pkbdFirst();
    for ( ; pkbd; pkbd = pkbdNext(pkbd) )
        if ( pkbd->m_hkl == hkl )
            break;

    return pkbd;
}
#endif

// **************************************************************************
