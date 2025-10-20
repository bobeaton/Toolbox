// key_d.h  Interface for record key dialog box  (1995-05-24)

#ifndef KEY_D_H
#define KEY_D_H

#include "resource.h"  // IDD_KEY
#include "srt.h"  // MatchSetting
#include "EditLng.h"

class CKey;  // recset.h
class CMarker;  // mkr.h


/////////////////////////////////////////////////////////////////////////////
// CKeyDlg dialog

class CKeyDlg : public CDialog
{
private:
	CKey* m_pkey;
	int m_iCommand;  // the user's command, e.g. Record Insert/Search
	CMarker* m_pmkr;

	// Edit control for the key entry area.
	CEditLng	m_elcKey; // 1.4qpn

// Dialog Data
	//{{AFX_DATA(CKeyDlg)
	enum { IDD = IDD_KEY };
	Str8	m_sKey;
	Str8	m_sLabel;
	BOOL	m_bMatchWhole;
	int		m_iMatchSetting;
	//}}AFX_DATA
	MatchSetting m_matset;

public:
	enum { eInsert, eSearch }; // for iCommand parameter in ctor
	CKeyDlg(CKey* pkey, int iCommand, CMarker* pmkr);	

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CKeyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif  // not KEY_D_H
