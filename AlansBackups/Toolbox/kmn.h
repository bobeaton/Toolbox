// kmn.h  Interface for an application's keyboarding manager

#ifndef KMN_H
#define KMN_H

class CKeyboardSet;  // kbd.h
class CKeyboard;  // kbd.h
class CMarker;  // mkr.h
class CLangEnc;  // lng.h

//#include "update.h"  // CUpdate


// **************************************************************************

// Class CKeyboardManager encapsulates an application's keyboard state
// and automates what would otherwise be the manual interaction between
// the user and the Keyman keyboarding utility.
// It is to be informed by the parts of the program which can cause
// the caret insertion point's current Standard Format marker or
// language encoding (in a dialog box's control) to change, and
// it is responsible to activate the appropriate keyboard.
// See classes CKeyboard and CKeyboardSet in kbd.h

class CKeyboardManager  // Hungarian: kmn
{
private:
    CKeyboardSet* m_pkbdset;
    
    CMarker* m_pmkrCur;
    CLangEnc* m_plngCur;
    CKeyboard* m_pkbdCur;
    
public:
    CKeyboardManager(CKeyboardSet* pkbdset);
    ~CKeyboardManager();
    
    CKeyboardSet* pkbdset() const { return m_pkbdset; }

    void ActivateKeyboard(CMarker* pmkrCur);
        // Call whenever the current Standard Format marker changes.
    void ActivateKeyboard(CLangEnc* plngCur);
        // Call whenever the current language encoding changes and
        // there is no current marker (i.e. some dialox box controls).

    void ActivateKeyboard(CKeyboard* pkbd);
        // Call to switch back to a keyboard.

    void DeActivateKeyboards();
        // Call whenever there is no current language encoding,
        // i.e. when there are no CShwView windows nor dialog boxes open.
        
    CKeyboard* pkbdGetCurrent();
        // Called to obtain the current Keyman keyboard.
        // Will return NULL if Keyman has no keyboard or if the current Keyman
        // keyboard is not available in the Keyboard set.

private:   
#ifdef _MAC
    void SetMacScript(CFont* pfnt); // select script based on font
#endif
};  // class CKeyboardManager

// **************************************************************************

#endif  // KMN_H
