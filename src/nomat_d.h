// nomat_d.h : header file
//

#ifndef Nomat_d_h
#define Nomat_d_h

#include "resource.h"
#include "EditLng.h"

/////////////////////////////////////////////////////////////////////////////
// CNoMatchesDlg dialog

class CNoMatchesDlg : public CDialog
{
private:
    CLangEnc* m_plng; // language of key
    BOOL m_bEnableInsert;
    BOOL m_bEnableTryAgain;

	// Edit control for the key entry area.
	CEditLng	m_elcKey; // 1.4qpn

// Dialog Data
    //{{AFX_DATA(CNoMatchesDlg)
	enum { IDD = IDD_NOMATCHES };
    CButton m_btnInsert;
    CButton m_btnTryAgain;
	Str8	m_sKey;
	//}}AFX_DATA

// Construction
public:
    CNoMatchesDlg( const char* pszKey, const CLangEnc* m_plng,
            BOOL bEnableInsert, BOOL bEnableTryAgain = TRUE ); // pass in key not found and font to display it with
	const char* pszKey(); // Return modified key

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CNoMatchesDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnNomatchesInsert();
    afx_msg void OnNomatchesTryagain();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // Nomat_d_h
