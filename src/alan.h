#if !defined(AFX_ALAN_H__09836C39_86D2_4003_8BD1_3D516D7DC6A1__INCLUDED_)
#define AFX_ALAN_H__09836C39_86D2_4003_8BD1_3D516D7DC6A1__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// CAlanTestDlg dialog

class CAlanTestDlg : public CDialog
{
// Construction
public:
	CAlanTestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAlanTestDlg)
	enum { IDD = IDD_AlanTestDlg };
//	CLngEdCtrl	m_LngEdCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlanTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlanTestDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CAlanAdaptIt dialog

class CAlanAdaptIt : public CDialog
{
public:
	CAlanAdaptIt();
	BOOL m_bCalcCorr;
	BOOL m_bCollectCorr;
	Str8 m_sName;
	Str8 m_sComment;
	int m_iRequiredSuccessPercent;
	int m_iTestSequenceNumber;
private:
	enum { IDD = IDD_AlanAdaptIt };
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};

/////////////////////////////////////////////////////////////////////////////
// CCrashFixFileEdit dialog

class CCrashFixFileEdit : public CDialog
{
public:
	CCrashFixFileEdit();
	Str8 m_sName;
private:
	enum { IDD = IDD_CrashFixFilePathEdit };
protected:
//	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};

BOOL bCrashFixBrowseForFileName(Str8 &sOutPath, Str8 sInPath); // 1.5.8h 

/*
Notes on how to make new dialog box
	Insert, Resource, Dialog, New produces a new blank one
	Right click header, Properties opens to set IDD_Dialog and caption
	Make a class for it, derived from CDialog
	Include private: enum { IDD = IDD_AlanAdaptIt };
	Include a member variable for each control
	Make a DoDataExchange func to call DDX_ for each control
		It is called automatically on Init, and on OK
	Make a constructor that automatically includes CDialog(IDD)
	Optionally do a message map if any immediate action is needed

	See ambig_d.h for a more thorough example

	Apparently it is necessary to make member variables and use DDX_Check ,etc.
		to transfer data to and from the controls
		I don't see anywhere where I access the control ID directly
	I see that all this is the way it is done with MFC dialogs based on CDialog
		It will be interesting to see what wxWindows does instead
		It won't have the ClassWizard and such tools, I assume
*/

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALAN_H__09836C39_86D2_4003_8BD1_3D516D7DC6A1__INCLUDED_)
