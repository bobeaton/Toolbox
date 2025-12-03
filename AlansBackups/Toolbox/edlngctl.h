// edlngctl.h
// Rod Early 1996-02-08

#ifndef EDLNGCTL_H
#define EDLNGCTL_H

class CLangEnc;  // lng.h
class CMarker;  // mkr.h
class CKeyboard;  // kbd.h


class CEditLngCombo : public CEdit	// Hungarian elc // 1.4vxp Change name of CLangEditCtrl to CEditLngCombo to reflect its use
{
private:
	CLangEnc* 	m_plng;
	CMarker*  	m_pmkr; // Marker need not be set if language is set
	CFont*		m_pfnt;
	BOOL		m_bKeyboardOverridden;  // Note: NULL is a valid override
	CKeyboard*	m_pkbd;

public:
	CEditLngCombo();
	~CEditLngCombo();
	
	void SetMarker(CMarker* pmkr);
	void SetLangEnc(CLangEnc* plng);
	
	// Use the following two functions to override the marker or language
	// only when the user can choose a font or select a keyboard
	// from within the same dialog box or property sheet as this edit box.
	void SetFont(CFont* pfnt);
	void SetKeyboard(CKeyboard* pkbd);

//	void Resize(); // Expand vertically to size necessary for language fonts // 1.4vxp 
	
private:
//	afx_msg void OnPaint(); // 1.4sr
	afx_msg void OnSetFocus(CWnd*);	
	afx_msg void OnKillFocus(CWnd*);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // 1.4qmr Experiment with find combo box drop
	DECLARE_MESSAGE_MAP()
};


// To use CLangEditCtrl in your dialog class definition:
// 1. Declare a member variable m_elc... of type CLangEditCtrl;
// 2. In DoDataExchange() use DDX_Control to map the corresponding
//    edit box control by its resource ID;
//    (Note: DDX_Control automatically subclasses the edit box
//    so that this derived class can handle the Focus messages.)
// 3. In OnInitDialog() call one of the CLangEditCtrl Set... functions
// 4. If the keyboard or font can be changed from within your dialog,
//    then call the appropriate Set... function whenever that happens.

#endif  // not EDLNGCTL_H
