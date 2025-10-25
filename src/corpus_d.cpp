//File: Corpus_d.cpp

#ifdef _MAC
#include <files.h>
#endif

#include "stdafx.h"
#include "toolbox.h"
#include "Corpus_d.h"

#include "Corpus.h"
#include "Not.h"
#include "Shw.h"
#include "Project.h"
#include "Tools.h"
#include "dirlist.h"
#include "shwdoc.h"
#include "shwview.h"
#include <direct.h>

#ifdef _MAC
const char chPathSep = ':';
#else
const char chPathSep = '\\';
#endif

// --------------------------------------------------------------------------
// CCorpusDlg

BEGIN_MESSAGE_MAP(CCorpusDlg, CDialog)
	//{{AFX_MSG_MAP(CCorpusDlg)
    ON_BN_CLICKED(IDC_CORPUS_EDITLANGENC, OnEditLangEnc)
	ON_BN_CLICKED(IDC_CORPUS_FILESLIST, OnFilesList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCorpusDlg::CCorpusDlg(CCorpus* pcorToEdit) : CDialog(CCorpusDlg::IDD, NULL),
    m_pcor(pcorToEdit), m_cboLangEncSet(Shw_plngset())
{
	//{{AFX_DATA_INIT(CCorpusDlg)
	m_sName = pcorToEdit->sKey();
	m_sFilesList = pcorToEdit->sFilesList();
	m_sWordMarkers = pcorToEdit->sWordMarkers();
	m_sBookMarker = pcorToEdit->sBookMarker();
	m_sChapMarker = pcorToEdit->sChapMarker();
	m_sVerseMarker = pcorToEdit->sVerseMarker();
	m_plng = pcorToEdit->plng();
	//}}AFX_DATA_INIT
	m_sFilesPath = sFirstFolder();
}
  
void CCorpusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCorpusDlg)
    DDX_Control(pDX, IDC_CORPUS_LANGENC, m_cboLangEncSet);
//	DDX_Txt(pDX, IDC_CORPUS_NAME, m_sName);
//	DDX_Txt(pDX, IDC_CORPUS_WORDS, m_sWordMarkers);
//	DDX_Txt(pDX, IDC_CORPUS_BOOK, m_sBookMarker);
//	DDX_Txt(pDX, IDC_CORPUS_CHAPTER, m_sChapMarker);
//	DDX_Txt(pDX, IDC_CORPUS_VERSE, m_sVerseMarker);
	//}}AFX_DATA_MAP
    if (pDX->m_bSaveAndValidate)
        m_plng = m_cboLangEncSet.plngSelected();	// save m_plng
    else
        m_cboLangEncSet.UpdateElements(m_plng);
}

BOOL CCorpusDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    SetDlgItemText( IDC_CORPUS_NAME, swUTF16( m_sName ) ); // 1.4qpv
	SetDlgItemText( IDC_CORPUS_WORDS, swUTF16( m_sWordMarkers ) ); // 1.4qpv
	SetDlgItemText( IDC_CORPUS_CHAPTER, swUTF16( m_sChapMarker ) ); // 1.4qpv
	SetDlgItemText( IDC_CORPUS_BOOK, swUTF16( m_sBookMarker ) ); // 1.4qpv
	SetDlgItemText( IDC_CORPUS_VERSE, swUTF16( m_sVerseMarker ) ); // 1.4qpv
    
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCorpusDlg::OnOK ()
{
//    GetDlgItemText( IDC_CORPUS_NAME, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_CORPUS_NAME, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_CORPUS_WORDS, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_CORPUS_WORDS, sDlgItem ); // 1.4qpw
	m_sWordMarkers = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_CORPUS_BOOK, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_CORPUS_BOOK, sDlgItem ); // 1.4qpw
	m_sBookMarker = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_CORPUS_CHAPTER, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_CORPUS_CHAPTER, sDlgItem ); // 1.4qpw
	m_sChapMarker = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_CORPUS_VERSE, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_CORPUS_VERSE, sDlgItem ); // 1.4qpw
	m_sVerseMarker = sUTF8( sDlgItem ); // 1.4qpw

    UpdateData(TRUE);
	CNote* pnot = NULL;

	if (m_sFilesList.IsEmpty())
		{
        AfxMessageBox(_("The list of Files to Process must contain at least one file."));
        //m_edxxxxx.SetFocus();
        return;
		}

	BOOL bOK = m_pcor->bModifyProperties(m_sName, m_sFilesList,
                                 m_sWordMarkers, m_sBookMarker, 
                                 m_sChapMarker, m_sVerseMarker, 
                                 m_plng, &pnot);
	if ( !bOK ) 
        {
		ASSERT( pnot );
        AfxMessageBox(pnot->sNote(), MB_OK); // 1.4qpr
		MessageBeep(0);
		delete pnot;
		return;
        } 
    else
        EndDialog(IDOK);
}

void CCorpusDlg::OnEditLangEnc ()
{
    UpdateData();
    m_plng->bViewProperties();
    UpdateData(FALSE); // 1.4kub May be needed
}

void CCorpusDlg::OnFilesList ()
{
    CFilesListDlg dlg(m_sFilesList, m_sFilesPath);
    if (dlg.DoModal() == IDOK) 
    {
        m_sFilesList = dlg.m_sFilesList;
        m_sFilesPath = dlg.m_sPath;
    }
}

//returns folder of first file in m_sFilesList,
//  if the list is empty, a default folder is provided
Str8 CCorpusDlg::sFirstFolder() const
{
	if (!m_sFilesList.IsEmpty())
		{
		Str8 sFile;
		m_sFilesList.iExtractNext(0, sFile, '\n');	// get first file path
		return sGetDirPath(sFile);		// get the drive+path
		}
	else
		{
		// get the best default folder
		Str8 sPath;
		CShwView* pView = Shw_papp()->pviewActive();
		if (pView)	// get folder of active window if present
			sPath = sGetDirPath( sUTF8( pView->GetDocument()->GetPathName() ) ); // 1.4qvu
		if (sPath.IsEmpty())
			{	// next try the project settings folder
			sPath = Shw_pProject()->sSettingsDirPath();
			}
		if (sPath.IsEmpty())
			{	// last resort - we must provide something
			char buffer[_MAX_PATH];
			sPath = _getcwd(buffer,_MAX_PATH);
			}
		return sPath;
		}
}

// --------------------------------------------------------------------------
// CFilesListBox

CFilesListBox::CFilesListBox() : CListBox()
{
    m_bShowWholePath = FALSE;
}

void CFilesListBox::GetFileOnly ( int iIndex, Str8& sString ) const
{
    Str8 sFullPath;
    GetFullPath(iIndex, sFullPath);
    sString = sFullPath.Mid(sFullPath.ReverseFind(chPathSep) + 1);
}

void CFilesListBox::Dir (const char* pszPath, BOOL bDirsOnly /*= FALSE*/)
{
    // Win and Mac can use "*" to match anything.
    CFileList filelst (pszPath, "", bDirsOnly);
    CFileListEl* pfle = filelst.pfleFirst();
    Str8 sPath;
    for ( ; pfle; pfle = filelst.pfleNext( pfle ) ) 
        {
#ifndef _MAC
        sPath = pfle->pszPath();
        if (sPath[sPath.GetLength()-1] != '.')
            AddString( swUTF16( sPath  ) ); // 1.4qvs
#else
        AddString(pfle->pszPath());
#endif
        } // for
}

void CFilesListBox::ContentsFromString (const Str8& sFilesList)
{
    int iStart = 0, iStop;
    ResetContent();
    while (iStart < sFilesList.GetLength()) 
        {
        iStop = iStart;
        while ((iStop < sFilesList.GetLength()) && (sFilesList[iStop] != '\n'))
            iStop++;   
        
        AddString( swUTF16( sFilesList.Mid(iStart, iStop - iStart)  ) ); // 1.4qvs Upgrade AddString for Unicode buile
        iStart = iStop + 1;
        }
}

void CFilesListBox::ContentsToString (Str8& sFilesList)
{
    int i; 
    int iCount = GetCount();
    Str8 sFileName;
    sFilesList.Empty();
    for (i = 0; i < iCount; i++) 
        {
//		char buffer[DLGBUFMAX+1]; // 1.4qvp
		CString swText; // 1.4qvp Upgrade GetText for Unicode build
        GetText(i, swText); // 1.4ed Change listbox get text to not use CStrg // 1.4qvp
		sFileName =  sUTF8( swText ); // 1.4qvp
        sFilesList += sFileName;
        sFilesList += '\n';
        }
}

void CFilesListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    COLORREF crBack;
    COLORREF crText;
    if ((lpDrawItemStruct->itemState) & ODS_SELECTED)
        {
        // Highlight if selected.
        crBack = GetSysColor(COLOR_HIGHLIGHT);
        crText = GetSysColor(COLOR_HIGHLIGHTTEXT);
        }
    else
        {
        crBack = GetSysColor(COLOR_WINDOW);
        crText = GetSysColor(COLOR_WINDOWTEXT);
        }

    CDC cDC;  // context with which to draw the item
    cDC.Attach(lpDrawItemStruct->hDC);
    const RECT& rcItem = lpDrawItemStruct->rcItem;  // rectangle containing it
    CBrush  brsSelected;
    brsSelected.CreateSolidBrush(crBack);
    cDC.FillRect(&rcItem, &brsSelected);        

    cDC.SetBkColor(crBack);
    cDC.SetTextColor(crText);

    if (lpDrawItemStruct->itemID != -1)
        {
//		char buffer[DLGBUFMAX+1]; // 1.4qvp
		CString swText; // 1.4qvp Upgrade GetText for Unicode build
        GetText(lpDrawItemStruct->itemID, swText);  // get full path from list // 1.4ed Change listbox get text to not use CStrg // 1.4qvp
        Str8 sText =  sUTF8( swText ); // 1.4qvp
        if (!bShowWholePath())
            sText = sGetFileName(sText);
		swText = swUTF16( sText ); // 1.4qta Upgrade all ExtTextOut for Uniocde build
        cDC.ExtTextOut(rcItem.left, rcItem.top, ETO_CLIPPED, &rcItem, swText, swText.GetLength(), NULL); // 1.4qta
        }
#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif      
    cDC.Detach();
}

// --------------------------------------------------------------------------
// CDriveComboBox

void CDriveComboBox::ShowDrives()
{
#ifdef _MAC
    HParamBlockRec paramBlock;
    int i = 1;
    char pszVolName[255];
    paramBlock.volumeParam.ioVolIndex = i;
    paramBlock.volumeParam.ioNamePtr = (unsigned char*) pszVolName;
    PBHGetVInfo(&paramBlock, FALSE);
    while (paramBlock.volumeParam.ioResult == noErr) 
        {								// byte 0 has string length
        pszVolName[pszVolName[0]+1] = '\x0';	//add trailing zero
        AddString(pszVolName+1);
        paramBlock.volumeParam.ioVolIndex = ++i;
        PBHGetVInfo(&paramBlock, FALSE);
        }
#else
    Dir(DDL_DRIVES | DDL_EXCLUSIVE,  swUTF16( "*") ); // 1.4qyc Upgrade Dir for Unicode build
#endif
}

void CDriveComboBox::GetDrive(int iIndex, Str8& sString) const
{
//	char buffer[DLGBUFMAX+1]; // 1.4qyd
	CString sw; // 1.4qyd Upgrade GetLBText for Unicode build
    GetLBText(iIndex, sw); // 1.4ed Change listbox get text to not use CStrg // 1.4qyd
    Str8 sDrive =  sUTF8( sw ); // 1.4qyd
#ifdef _MAC
    sString = sDrive;
#else
    sString = sDrive[2];
    sString += ':';
#endif
}

void CDriveComboBox::SelectDrive (const char* pszDrive)
{
#ifdef _MAC
    SetCurSel(FindStringExact(-1, pszDrive));
#else
    Str8 sDriveTitle;
    sDriveTitle.Format("[-%c-]", pszDrive[0]);
    SetCurSel(FindStringExact(-1, swUTF16( sDriveTitle) ) ); // 1.4qvr Upgrade FindStringExact for Unicode build
#endif
}

// --------------------------------------------------------------------------
// CFilesListDlg

BEGIN_MESSAGE_MAP(CFilesListDlg, CDialog)
	//{{AFX_MSG_MAP(CCorpusDlg)
    ON_BN_CLICKED(IDC_FILESLIST_SHOWPATH, OnShowPath)
    ON_BN_CLICKED(IDC_FILESLIST_MOVETO, OnMoveTo)
    ON_BN_CLICKED(IDC_FILESLIST_MOVEFROM, OnMoveFrom)
    ON_BN_CLICKED(IDC_FILESLIST_FIRSTTO, OnFirstTo)
    ON_BN_CLICKED(IDC_FILESLIST_LASTTO, OnLastTo)
    ON_BN_CLICKED(IDC_FILESLIST_CLEAR, OnClear)
    ON_BN_CLICKED(IDC_FILESLIST_UPDIR, OnUpDir)
    ON_CBN_SELCHANGE(IDC_FILESLIST_DRIVE, OnDriveChange)
    ON_LBN_DBLCLK(IDC_FILESLIST_LSTPATH, OnPathChange)
    ON_LBN_DBLCLK(IDC_FILESLIST_LSTAVAILABLE, OnAvailableDblClk)
    ON_LBN_DBLCLK(IDC_FILESLIST_LSTSELECTED, OnSelectedDblClk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFilesListDlg::CFilesListDlg(const char* pszFilesList, const char* pszPath) : 
    CDialog(CFilesListDlg::IDD, NULL)
{
    //{{AFX_DATA_INIT(CWordLstDlg)
    m_sFilesList = pszFilesList;
    m_sPath = pszPath;
    //}}AFX_DATA_INIT
}

BOOL CFilesListDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_chkShowPath.SetCheck( Shw_pProject()->bShowWholePath() );
    m_cboDriveList.ShowDrives();
    ShowNewPath(m_sPath);
    m_sDrive = m_sPath.Left(m_sPath.Find(chPathSep));
    m_cboDriveList.SelectDrive(m_sDrive);
    m_lboAvailableList.SetSel(0);
    m_lboSelectedList.SetCurSel(0);
    m_chkShowPath.SetCheck(Shw_pProject()->bShowWholePath());
    m_lboSelectedList.ShowWholePath(Shw_pProject()->bShowWholePath());
    m_lboPathList.SetFocus();
    return FALSE;
}

void CFilesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWordLstDlg)
    DDX_Control(pDX, IDC_FILESLIST_LSTSELECTED, m_lboSelectedList);
    DDX_Control(pDX, IDC_FILESLIST_LSTAVAILABLE, m_lboAvailableList);
    DDX_Control(pDX, IDC_FILESLIST_LSTPATH, m_lboPathList);
    DDX_Control(pDX, IDC_FILESLIST_DRIVE, m_cboDriveList);
    DDX_Control(pDX, IDC_FILESLIST_SHOWPATH, m_chkShowPath);
    DDX_Control(pDX, IDC_FILESLIST_PATH, m_lblPath);
	//}}AFX_DATA_MAP
    if (pDX->m_bSaveAndValidate)
        m_lboSelectedList.ContentsToString(m_sFilesList);
    else
        m_lboSelectedList.ContentsFromString(m_sFilesList);
}

void CFilesListDlg::OnShowPath()
{
    Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
    m_lboSelectedList.ShowWholePath(m_chkShowPath.GetCheck());
    m_lboSelectedList.Invalidate(); // force redraw
}

void CFilesListDlg::OnDriveChange()
{
    Str8 sNewDrive;
    m_cboDriveList.GetDrive(m_cboDriveList.GetCurSel(), sNewDrive);
    if (m_sDrive != sNewDrive) 
        {
        m_sDrive = sNewDrive;
        sNewDrive += chPathSep;
        ShowNewPath(sNewDrive);
        }
}

void CFilesListDlg::OnPathChange()
{
    Str8 sNewPath;
    m_lboPathList.GetFullPath(m_lboPathList.GetCurSel(), sNewPath);
    ShowNewPath(sNewPath);
}

void CFilesListDlg::OnUpDir()
{
    Str8 sOldPath = m_sPath.Left(m_sPath.GetLength() - 1);
    Str8 sNewPath = sOldPath.Left(sOldPath.ReverseFind(chPathSep)+1);
    if (!sNewPath.IsEmpty())
        ShowNewPath(sNewPath);
}

void CFilesListDlg::OnMoveTo()
{
    BatchMove(m_lboSelectedList.GetCurSel());
}

void CFilesListDlg::OnMoveFrom()
{
    int nCurSel = m_lboSelectedList.GetCurSel();
    if (m_lboSelectedList.DeleteString(nCurSel) == nCurSel)
        nCurSel--;
    m_lboSelectedList.SetCurSel(nCurSel);
}

void CFilesListDlg::OnFirstTo()
{
    BatchMove(-1);
}

void CFilesListDlg::OnLastTo()
{
    BatchMove(m_lboSelectedList.GetCount()-1);
}

void CFilesListDlg::OnClear()
{
    m_lboSelectedList.ResetContent();
}

void CFilesListDlg::OnAvailableDblClk()
{
    OnMoveTo();
}

void CFilesListDlg::OnSelectedDblClk()
{
    OnMoveFrom();
}

void CFilesListDlg::ShowNewPath (const char* pszNewPath)
{
    m_lboPathList.ResetContent();
    m_lboAvailableList.ResetContent();
    m_sPath = pszNewPath;
    m_lblPath.SetWindowText( swUTF16( m_sPath) ); // 1.4qpy
    m_lboPathList.Dir(m_sPath, TRUE);
    m_lboAvailableList.Dir(m_sPath, FALSE);
}

void CFilesListDlg::BatchMove(int iMoveTo)
{
    Str8 sFullPath;
    int i, iSelCount = m_lboAvailableList.GetSelCount(), *piSelList, iNewSel;
    piSelList = new int[iSelCount];
    m_lboAvailableList.GetSelItems(iSelCount, piSelList);
    iNewSel = iMoveTo;
    
    for (i = 0; i < iSelCount; i++) 
        {
        m_lboAvailableList.GetFullPath(piSelList[i], sFullPath);
        if (m_lboSelectedList.FindStringExact(-1, swUTF16( sFullPath) ) == LB_ERR ) // 1.4qvr
            m_lboSelectedList.InsertString(++iNewSel, swUTF16( sFullPath ) ); // 1.4qre
        }

    m_lboSelectedList.SetCurSel(iNewSel);
    delete piSelList;
}




