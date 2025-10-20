// kmn.cpp  Implementation of an application's keyboarding manager

#ifdef _MAC
#include "script.h" // This gave major compile errors for some reason
#include "fonts.h"
#endif

#include "stdafx.h"
#include "toolbox.h"
#include "kmn.h"
#include "kbd.h"  // CKeyboardSet
#include "mkr.h"  // CMarker (and CLangEnc)


// **************************************************************************

CKeyboardManager::CKeyboardManager(CKeyboardSet* pkbdset)
{
    m_pkbdset = pkbdset;
    ASSERT( m_pkbdset );
    m_pmkrCur = NULL;
    m_plngCur = NULL;
    m_pkbdCur = NULL;
}

CKeyboardManager::~CKeyboardManager()
{
}

void CKeyboardManager::DeActivateKeyboards()
{
    // When the keyboard is deactivated, the language and marker become irrelevant.
    m_pmkrCur = NULL;
    m_plngCur = NULL;
    m_pkbdCur = NULL;
        
    m_pkbdset->DeActivateKeyboards();
}

void CKeyboardManager::ActivateKeyboard(CKeyboard* pkbd)
{
	if ( pkbd )
		pkbd->Activate();
	else
		DeActivateKeyboards();
}

#ifdef _MAC
void CKeyboardManager::SetMacScript( CFont* pfnt ) // select script based on font
{
    LOGFONT lf;
    pfnt->GetObject( sizeof(lf), &lf ); // get parameters from font
    short iFont;
    _c2pstr( lf.lfFaceName ); // convert to Pascal string
    GetFNum( (const char*)lf.lfFaceName, &iFont );
    int iScript = FontToScript( iFont ); // find out what script this font uses
    if ( iScript >= 0 )
        KeyScript( iScript ); // tell system to change script and keyboard
}
#endif
    
void CKeyboardManager::ActivateKeyboard(CMarker* pmkrCur)
{
    ASSERT( pmkrCur );
	ActivateKeyboard( pmkrCur->plng()->pkbd() );
}

void CKeyboardManager::ActivateKeyboard(CLangEnc* plngCur)
{
    ASSERT( plngCur );
	ActivateKeyboard( plngCur->pkbd() );
}

CKeyboard* CKeyboardManager::pkbdGetCurrent()
{
    return m_pkbdset->pkbdGetCurrent();
}
