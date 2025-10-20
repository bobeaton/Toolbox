/////////////////////////////////////////////////////////////////////////////
// rundos.h
// Interface of the CRunDos class
// This class manages a dialog the has the ability to run any dos program.
// Author: Doug Eberman
// copyright 1997 JAARS Inc. SIL
/////////////////////////////////////////////////////////////////////////////

#define MAX_COMMAND_MEMORY 20

/////////////////////////////////////////////////////////////////////////////
// CRunDos dialog

class CRunDos : public CDialog
{
// Construction
public:
	CRunDos(CWnd* pParent = NULL);	// standard constructor     
	virtual ~CRunDos();

	BOOL DidACommand(void) { return m_didCommand; };
	BOOL bReadProperties(Object_istream& obs, const char* pszQualifier);
#ifdef prjWritefstream // 1.6.4aa 
	void WriteProperties(Object_ofstream& obs, const char* pszQualifier) const;
#else
	void WriteProperties(Object_ostream& obs, const char* pszQualifier) const;
#endif

// Implementation
protected:

// Dialog Data
	//{{AFX_DATA(CRunDos)
	enum { IDD = IDD_RunBatchFile };
	CComboBox m_CommandList;
	//}}AFX_DATA       
	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    virtual BOOL OnInitDialog();
	
	BOOL m_isRunning;                               
	BOOL m_didCommand; 
	Str8 * m_apStrCommands[MAX_COMMAND_MEMORY];
	
	void MemorizeCommand(const Str8 & str);

	// Generated message map functions
	//{{AFX_MSG(CRunDos)
	afx_msg void OnRundos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
