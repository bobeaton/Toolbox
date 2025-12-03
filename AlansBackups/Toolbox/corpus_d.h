//File: Corpus_d.h

#ifndef CORPUS_D_H
#define CORPUS_D_H

#include "Resource.h"
#include "Corpus.h"
#include "Lng.h"

// --------------------------------------------------------------------------

class CCorpusDlg : public CDialog
{
// Construction
public:
	CCorpusDlg(CCorpus* pcorToEdit);

// Member variables
private:
	CCorpus*	m_pcor;
	Str8Ex	m_sFilesList;
	Str8		m_sFilesPath;	//current path used for selecting corpus files

// Dialog Data
	//{{AFX_DATA(CCorpusDlg)
	enum { IDD = IDD_CORPUS };
	Str8 m_sName;
	Str8	m_sWordMarkers;
	Str8	m_sBookMarker;
	Str8	m_sChapMarker;
	Str8	m_sVerseMarker;
	CLangEncSetComboBox m_cboLangEncSet;
	CLangEncPtr m_plng;
	//}}AFX_DATA

// Implementation
	Str8 sFirstFolder() const;  //returns folder of first file in m_sFilesList

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCorpusDlg)
	virtual void OnOK();
	afx_msg void OnEditLangEnc();
	afx_msg void OnFilesList();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// --------------------------------------------------------------------------

class CFilesListBox : public CListBox
{
public:
    CFilesListBox();

    void GetFullPath ( int iIndex, Str8& sString ) const 
		{ 
//		char buffer[DLGBUFMAX+1]; // 1.4qvm
		CString swText; // 1.4qvm Upgrade GetText for Unicode build
        GetText(iIndex, swText); // 1.4ed Change listbox get text to not use CStrg // 1.4qvm
		sString =  sUTF8( swText );  // 1.4qvm
		}
    void GetFileOnly ( int iIndex, Str8& sString ) const;
    
    void Dir (const char* pszPath, BOOL bDirsOnly = FALSE);

    void ContentsFromString(const Str8& sFilesList);
    void ContentsToString(Str8& sFilesList);

    BOOL bShowWholePath() { return m_bShowWholePath; }
    void ShowWholePath(BOOL bShowWholePath) { m_bShowWholePath = bShowWholePath; }

protected:
    BOOL m_bShowWholePath;
    virtual void DrawItem(LPDRAWITEMSTRUCT);
};

// --------------------------------------------------------------------------

class CDriveComboBox : public CComboBox
{
public:
    void ShowDrives();
    void GetDrive (int iIndex, Str8& sString) const;
    void SelectDrive (const char* pszNewDrive);
};

// --------------------------------------------------------------------------

class CFilesListDlg : public CDialog
{
// Construction
public:
    CFilesListDlg (const char* pszFilesList, const char* pszPath);

// Dialog Data
	//{{AFX_DATA(CFilesListDlg)
	enum { IDD = IDD_FILESLIST };
    CFilesListBox m_lboSelectedList;
    CFilesListBox m_lboAvailableList;
    CFilesListBox m_lboPathList;
    CDriveComboBox m_cboDriveList;
	CButton	m_chkShowPath;
    CStatic m_lblPath;
    Str8 m_sFilesList;
    Str8 m_sPath;
    Str8 m_sDrive;
	//}}AFX_DATA

protected:
// Implementation
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
    void ShowNewPath (const char* pszNewPath);
    void BatchMove(int iMoveTo);

	// Generated message map functions
	//{{AFX_MSG(CFilesListDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnShowPath();
    afx_msg void OnDriveChange();
    afx_msg void OnPathChange();
    afx_msg void OnUpDir();
    afx_msg void OnMoveTo();
    afx_msg void OnMoveFrom();
    afx_msg void OnFirstTo();
    afx_msg void OnLastTo();
    afx_msg void OnClear();
    afx_msg void OnAvailableDblClk();
    afx_msg void OnSelectedDblClk();
    //afx_msg void OnChangePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif




