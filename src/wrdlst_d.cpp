// wrdlst_d.cpp - Word list and concordance dialog boxes

// Change History
//    July 97 JK First draft, designed with input from Alan B
// 25 Sept 97 BW chgd names: CCorpus. TextListFile > WordListOutput
// 03 Apr 98  BW revised extensively
// 14 Apr 2000 BW moved file open from OnOK to DoWrdLst, DoCncordnce


#include "stdafx.h"
#include "toolbox.h"
#include "wrdlst_d.h"

#include <ctype.h>
#include <direct.h>
#include "shw.h"
#include "shwdoc.h"
#include "shwview.h"
#include "corpus.h"
#include "project.h"
#include "progress.h"
#include "stdiofil.h"
#include <strstream> 
#include "alan.h" // 1.5.8h 

/////////////////////////////////////////////////////////////////////////////
// Static variables and helper functions

static const char psz_fmtWordLstHdr[] = "\\_sh v3.0  400  %s\n\n";
// the word list markers match those normally output by WDL.exe
static const char psz_mkrWordLstWord[] = "w";
static const char psz_mkrWordLstCount[] = "c";
static const char psz_mkrWordLstRef[] = "r";
// the word list field names are gotten from resource strings by GetWordListType()

static const char psz_fmtConcHdr[] = "\\_sh v3.0  400  %s"; //no \n here; added by code
// the concordance markers are large and a bit strange to avoid conflicts with markers
//  from the corpus text - which may be placed in the concordance db (as data, sort of)
static const char psz_mkrConcRef[] = "concref";
static const char psz_mkrConcBefore[] = "concbef";
static const char psz_mkrConcTarget[] = "conctar";
static const char psz_mkrConcAfter[] = "concaft";
// the word list field names are gotten from resource strings by GetConcordanceType()

// 1.1kc Make new wordlist window automatically a jump target
static const char psz_WordLstWndSettings[] = "\
\\+db %s\n\
\\+wndlst \n\
\\+wnd \n\
\\z  %i\n\
\\ViewFieldNames\n\
\\widthMarkerArea 50\n\
\\FocusTarget\n\
\\JumpTarget\n\
\\Browse\n\
\\+brflst \n\
\\FieldEmptyEntry_White\n\
\\+brf \n\
\\mkr w\n\
\\width 75\n\
\\-brf\n%s%s\
\\-brflst\n\
\\+ind \n\
\\mkrPriKey w\n\
\\-ind\n\
\\-wnd\n\
\\-wndlst\n\
\\-db";
static const char psz_WordLstWndCountFld[] = "\
\\+brf \n\
\\mkr c\n\
\\width 50\n\
\\rightjustify\n\
\\-brf\n";
static const char psz_WordLstWndRefFld[] = "\
\\+brf \n\
\\mkr r\n\
\\width 600\n\
\\-brf\n";
static const char psz_LookupWndSettingsLeftToRightScript[] = "\
\\+db %s\n\
\\+wndlst \n\
\\+wnd \n\
\\z  %i\n\
\\ViewFieldNames\n\
\\widthMarkerArea 50\n\
\\Browse\n\
\\+brflst \n\
\\FieldEmptyEntry_White\n\
\\+brf \n\
\\mkr concref\n\
\\width 75\n\
\\-brf\n\
\\+brf \n\
\\mkr concbef\n\
\\width 200\n\
\\rightjustify\n\
\\-brf\n\
\\+brf \n\
\\mkr conctar\n\
\\width 75\n\
\\-brf\n\
\\+brf \n\
\\mkr concaft\n\
\\width 200\n\
\\-brf\n\
\\-brflst\n\
\\+ind \n\
\\mkrPriKey concref\n\
\\-ind\n\
\\-wnd\n\
\\-wndlst\n\
\\-db";
static const char psz_LookupWndSettingsRightToLeftScript[] = "\
\\+db %s\n\
\\+wndlst \n\
\\+wnd \n\
\\z  %i\n\
\\ViewFieldNames\n\
\\widthMarkerArea 50\n\
\\Browse\n\
\\+brflst \n\
\\FieldEmptyEntry_White\n\
\\+brf \n\
\\mkr concref\n\
\\width 75\n\
\\-brf\n\
\\+brf \n\
\\mkr concaft\n\
\\width 200\n\
\\rightjustify\n\
\\-brf\n\
\\+brf \n\
\\mkr conctar\n\
\\width 75\n\
\\rightjustify\n\
\\-brf\n\
\\+brf \n\
\\mkr concbef\n\
\\width 200\n\
\\-brf\n\
\\-brflst\n\
\\+ind \n\
\\mkrPriKey concref\n\
\\-ind\n\
\\-wnd\n\
\\-wndlst\n\
\\-db";


/////////////////////////////////////////////////////////////////////////////
// CWordListDlg dialog

CWordListDlg::CWordListDlg(CCorpusSet* pcorset, CWnd* pParent /*=NULL*/)
    : CDialog(CWordListDlg::IDD, pParent), m_cboCorpus(pcorset)
{
    // constructor gets initial info from CCorpusSet & CCorpus
    //{{AFX_DATA_INIT(CWordListDlg)
    m_bCount = pcorset->bWordLstCounts();
    m_bRefs = pcorset->bWordLstRefs();
    m_nLimitRefs = pcorset->bWordLstLimitCount();
    m_nRefCount = pcorset->nWordLstMaxCount();
    //}}AFX_DATA_INIT
    m_pcor = pcorset->pcorWordLstCorpus();
    if (!m_pcor)
        m_pcor = (CCorpus*) pcorset->pselFirst();
    if (m_pcor)     // could be NULL if no corpus in project yet
        {
        m_sOutput = m_pcor->sWordListOutput();
        SimplifyOutputFileName();
        }
}

void CWordListDlg::DoDataExchange(CDataExchange* pDX)
{                                       // called by UpdateData()
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CWordListDlg)
    DDX_Control(pDX, IDC_WORDLST_REFCOUNT, m_edRefCount);
    DDX_Control(pDX, IDC_WORDLST_UNLIMITED, m_btnUnlimited);
    DDX_Control(pDX, IDC_WORDLST_LIMITED, m_btnLimited);
    DDX_Control(pDX, IDC_WORDLST_CORPUS, m_cboCorpus);
    DDX_Control(pDX, IDC_WORDLST_OUTPUT, m_edOutput);
    DDX_Control(pDX, IDC_WORDLST_CURDIR, m_lblCorpusDir);
    DDX_Check(pDX, IDC_WORDLST_COUNT, m_bCount);
//    DDX_Txt(pDX, IDC_WORDLST_OUTPUT, m_sOutput);
    DDX_Check(pDX, IDC_WORDLST_REFS, m_bRefs);
    DDX_Radio(pDX, IDC_WORDLST_UNLIMITED, m_nLimitRefs);
    DDX_Text(pDX, IDC_WORDLST_REFCOUNT, m_nRefCount); // 1.4dw OK because not CStrg
    //}}AFX_DATA_MAP
    if (pDX->m_bSaveAndValidate)
        m_pcor = m_cboCorpus.pcorSelected();  // save m_pcor
    else
        m_cboCorpus.UpdateElements(m_pcor);
}

BEGIN_MESSAGE_MAP(CWordListDlg, CDialog)
    //{{AFX_MSG_MAP(CWordListDlg)
    ON_BN_CLICKED(IDC_WORDLST_REFS, OnRefs)
    ON_BN_CLICKED(IDC_WORDLST_UNLIMITED, OnUnlimited)
    ON_BN_CLICKED(IDC_WORDLST_LIMITED, OnLimited)
    ON_BN_CLICKED(IDC_WORDLST_BROWSE, OnBrowse)
    ON_BN_CLICKED(IDC_WORDLST_CORPUS_EDIT, OnCorpusEdit)
    ON_CBN_SELCHANGE(IDC_WORDLST_CORPUS, OnCorpusSelChange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWordListDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4qpv

    m_edRefCount.EnableWindow(m_nLimitRefs);
    m_btnLimited.EnableWindow(m_bRefs);
    m_btnUnlimited.EnableWindow(m_bRefs);
    ShowCorpusFolder();
    return TRUE;
}

void CWordListDlg::OnOK()
{
    CNote *pnotNoExist, *pnotDirty;
    CFileException ex;

    // Validate the data that the user entered.
    UpdateData(TRUE);
//    GetDlgItemText( IDC_WORDLST_OUTPUT, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_WORDLST_OUTPUT, sDlgItem ); // 1.4qpw
	m_sOutput = sUTF8( sDlgItem ); // 1.4qpw

    if (m_pcor == NULL) 
        {
        AfxMessageBox(_("Please select a text corpus from the list."));
        m_cboCorpus.SetFocus();
        return;
        }

    if (m_sOutput.IsEmpty()) 
        {
        AfxMessageBox(_("Please provide a name for the output file."));
        m_edOutput.SetFocus();
        return;
        }

    m_pcor->ValidateFilesList(&pnotNoExist, &pnotDirty, TRUE);  // save prompt???
    if (pnotNoExist != NULL) 
        {
        AfxMessageBox(pnotNoExist->sNote());
        delete pnotNoExist;
        return;
        }
    if (pnotDirty != NULL) 
        {
        Str8 sDirtyPrompt = pnotDirty->sNote(), sDirtySubPrompt;
        delete pnotDirty;
        if (!Shw_app().bSaveAllFiles())
            return;
        } // if

    // Validate the Word List output path
    const Str8 sOutputPath = sOutputFullPath(); // derive full path from m_sOutput
    BOOL bExists = ( bFileExists(sOutputPath) );
    BOOL bReadOnly = ( bExists ? ( bFileReadOnly(sOutputPath) ) : FALSE );
    if ( bReadOnly )
		UnWriteProtect( sOutputPath ); // 1.2df Turn off write protect of wordlist file before trying to write it // 1.4qzhg

    // Save the current dialog settings in CCorpusSet & CCorpus
    m_pcor->SetWordListOutput(m_sOutput);
    CCorpusSet *pcorset;
    pcorset = m_pcor->pcorsetMyOwner();
    pcorset->SetWordLstCorpus(m_pcor);
    pcorset->SetWordLstCounts(m_bCount);
    pcorset->SetWordLstRefs(m_bRefs);
    pcorset->SetWordLstLimitCount(m_nLimitRefs);
    pcorset->SetWordLstMaxCount(m_nRefCount);

    EndDialog(IDOK);
}

// DoWrdList() implements the Word List feature
void CWordListDlg::DoWordList()
{
	Shw_app().bSaveAllFiles(); // 1.4teg Save all files before making wordlist
    if (DoModal() != IDOK) // Display the Word List dialog
        return;
	if ( !m_pcor->bInitCorpusRead() )
		{
		AfxMessageBox( _("No corpus files found") );
		return;
		}

	LONG lSize = m_pcor->sFilesList().GetLength(); // 1.4ya Make word list progress bar show progress
    CProgressIndicator* pprg = new CProgressIndicator( lSize, NULL ); // 1.4vyb Add progress bar during word list build
	CProgressIndicator prg = *pprg; // 1.4vyb 
	int iCurProgress = m_pcor->iCurFileNameStart(); // 1.4ya 

	WrdInfoList wrdlst; // 1.4ted Place to build word list
	Str8 sCorpusLine;
	while ( m_pcor->bGetNextLineOfCorpus( sCorpusLine ) ) // 1.4ted Build word list
		{
		if ( m_pcor->plng()->bHasCase() ) // 1.4vyz 
			sCorpusLine = m_pcor->plng()->sLowerCase( sCorpusLine ); // 1.4vyz Fix U bug of word list not collapsing case
		Str8 sRef = m_pcor->sCurRef();
		Str8 sWord;
		int iPos = 0;
		while ( m_pcor->plng()->bNextWord( sCorpusLine, sWord, iPos ) )
			{
			WrdInfo* pwrdPrev = NULL;
			WrdInfo* pwrd = wrdlst.pwrdSearch( sWord, pwrdPrev );
			if ( pwrd ) // 1.4ted If found, increment count and move to top of list for faster search
				{
				pwrd->iCount += 1;
				wrdlst.MoveToTop( pwrd, pwrdPrev );
				}
			else // 1.4ted Else, not found, add to list
				pwrd = wrdlst.pwrdAdd( sWord, pwrdPrev );
			if ( m_bRefs ) // 1.4tec If refs, add ref
				{
				if ( m_nLimitRefs == 0 || pwrd->iCount <= m_nRefCount ) // 1.4vnk Allow word list to limit number of references
					{
					pwrd->sRefs += sRef;
					if ( pwrd->iCount % 10 == 0 ) // 1.4vxa Put nl in wordlist refs after every 10 refs
						pwrd->sRefs += "\n"; // 1.4vxa 
					else
						pwrd->sRefs += " ";
					}
				}
			}
		if ( iCurProgress != m_pcor->iCurFileNameStart() ) // 1.4ya If corpus file number has changed, update progress
			{
			iCurProgress = m_pcor->iCurFileNameStart(); // 1.4ya
			prg.bUpdateProgress( iCurProgress ); // 1.4ya 
			}
		}
	delete pprg; // 1.4vyb 

    const Str8 sOutputPath = sOutputFullPath(); // derive full path from m_sOutput
    CDatabaseType* ptypWL = ptypGetWordListType(m_pcor);
    Str8 sOutput;
    sOutput.Format( psz_fmtWordLstHdr, (const char*) ptypWL->sName() );
	FILE* pfil = nullptr;
	errno_t err = fopen_s(&pfil, sOutputPath, "w");	// 1.4tb Open word list file to write a new one
	if (err != 0 || pfil == nullptr) // 1.4ywf Fix U bug of crash if word list path does not exist
	{
		AfxMessageBox( _("Error: Output path does not exist.") ); // 1.4ywf 
		return; // 1.4ywf 
	}
    fputs( sOutput, pfil );
	for ( WrdInfo* pwrd = wrdlst.pwrdFirst; pwrd; pwrd = pwrd->pwrdNext )
		{
		char buffer[20];
		_itoa_s(pwrd->iCount, buffer, (int)sizeof(buffer), 10);
		Str8 sCount = buffer; // 1.4vza 
		Str8 sCountz = sCount; // 1.4vza Write cz count with zeroes in word list like previous did
		while ( sCountz.GetLength() < 6 ) // 1.4vza 
			sCountz = "0" + sCountz; // 1.4vza 
		sOutput = "";
		sOutput += "\n\\w " + pwrd->sWord;
		sOutput += "\n\\c " + sCount; // 1.4vza 
		sOutput += "\n\\cz " + sCountz; // 1.4vza 
		sOutput += "\n\\r " + pwrd->sRefs;
	    fputs( sOutput, pfil );
		}
    fclose( pfil );
    // On success, open the SF word list file in browse view.  
    CFileStatus filestat;
    VERIFY(CFile::GetStatus( swUTF16( sOutputPath ), filestat)); // 1.4qvx
	CString sw = filestat.m_szFullName; // 1.4qwf Upgrade pdocGetDocByPath for Unicode build
    CShwDoc *pdoc = Shw_app().pdocGetDocByPath( sUTF8( sw ) ); // 1.4qwf
    if (pdoc) // first, see if it's already open
        { // It's already open, refresh it.
        WINDOWPLACEMENT wp;
        Shw_app().pviewTop()->GetParent()->GetWindowPlacement(&wp);
        Shw_app().bRefreshADoc(pdoc);
        if (wp.showCmd == SW_SHOWMAXIMIZED)
            ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->ShowWindow(SW_SHOWMAXIMIZED);
        else
            ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->ShowWindow(SW_NORMAL);
    
        ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
        }
    else 
        { // It's not open, so open it using some predefined properties that will open it in browse mode.
        CNoteList notlst;
        char* pszBuffer = new char[strlen(psz_WordLstWndSettings) + strlen(sOutputPath) + 
				strlen(psz_WordLstWndCountFld) + strlen(psz_WordLstWndRefFld)];
        int z;
        CShwView *pshv;
        Shw_app().SetZ();
        pshv = Shw_app().pviewTop();
        if (pshv)
            z = pshv->z() + 1;
        else
            z = 0;
        wsprintfA(pszBuffer, psz_WordLstWndSettings, (LPCSTR) sOutputPath, z,
				(m_bCount ? psz_WordLstWndCountFld : ""), (m_bRefs ? psz_WordLstWndRefFld : "") );
        ASSERT(strlen(pszBuffer) < strlen(psz_WordLstWndSettings) + strlen(sOutputPath) + 
				strlen(psz_WordLstWndCountFld) + strlen(psz_WordLstWndRefFld)  );
        std::istrstream iosIn(pszBuffer);
        Object_istream obsIn(iosIn, notlst);
        CShwDoc::s_bReadProperties(obsIn);
        delete pszBuffer;
        }
	Shw_app().bSaveAllFiles(); // 1.4tej Save wordlist after it is loaded and sorted
}

void CWordListDlg::OnRefs()
{
    UpdateData(TRUE);
    if (m_bRefs) 
        {
        m_btnLimited.EnableWindow(TRUE);
        m_btnUnlimited.EnableWindow(TRUE);
        m_edRefCount.EnableWindow(m_nLimitRefs);
        } 
    else 
        {
        m_btnLimited.EnableWindow(FALSE);
        m_btnUnlimited.EnableWindow(FALSE);
        m_edRefCount.EnableWindow(FALSE);
        }   
}

void CWordListDlg::OnLimited()
{
    m_edRefCount.EnableWindow(TRUE);
    m_edRefCount.SetFocus();
    m_edRefCount.SetSel(0, -1);
}

void CWordListDlg::OnUnlimited()
{
    m_edRefCount.EnableWindow(FALSE);
}

void CWordListDlg::OnBrowse()
{
    UpdateData(TRUE);   //save into m vars
#ifdef CrashFixDidntWork // 1.5.8g 
	unsigned short* lpstrFileBuffer = new unsigned short[5000]; // 1.5.8g 
	lpstrFileBuffer[0] = 0; // 1.5.8g Initialize file name buffer to empty string
	OPENFILENAME ofn; // 1.5.8g Test non-MFC open file dialog box on WordList Browse
	ofn.lStructSize = sizeof( OPENFILENAME ); // 1.5.8g 
	ofn.hwndOwner = NULL; // 1.5.8g 
	ofn.hInstance = NULL; // 1.5.8g 
	ofn.lpstrFilter = NULL; // 1.5.8g 
	ofn.lpstrCustomFilter = NULL; // 1.5.8g 
	ofn.nFilterIndex = 0; // 1.5.8g 
	ofn.lpstrFile = lpstrFileBuffer; // 1.5.8g 
	ofn.nMaxFile = 5000; // 1.5.8g 
	ofn.lpstrFileTitle = NULL; // 1.5.8g 
	ofn.lpstrInitialDir = NULL; // 1.5.8g 
	ofn.lpstrTitle = NULL; // 1.5.8g 
	ofn.Flags = 0; // 1.5.8g 
	ofn.nFileOffset = 0; // 1.5.8g 
	ofn.nFileExtension = 0; // 1.5.8g 
	ofn.lpstrDefExt = NULL; // 1.5.8g 
	ofn.lCustData = NULL; // 1.5.8g 
	ofn.lpfnHook = NULL; // 1.5.8g 
	ofn.lpTemplateName = NULL; // 1.5.8g 
	BOOL bSucc = GetSaveFileName( &ofn ); // 1.5.8g 
	delete lpstrFileBuffer; // 1.5.8g 
#endif // CrashFixDidn't work // 1.5.8g 
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlg(FALSE, NULL, swUTF16( sOutputFullPath() ) ); // 1.4qrd // 1.5.8h 
		dlg.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlg ) != IDOK) 
			return; // 1.5.8h 
		sPath =  sUTF8( dlg.GetPathName() ); // 1.4qxf
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sOutput) ) // 1.5.8h 
			return; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return; // 1.4vze 
    m_sOutput = sPath; // 1.5.8h 
    SimplifyOutputFileName();
	SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder(); // prob unchanged I think
}

void CWordListDlg::OnCorpusSelChange()
{
    UpdateData(TRUE);   //save into m vars
    m_sOutput = m_pcor->sWordListOutput();
    SimplifyOutputFileName();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder();
}

void CWordListDlg::OnCorpusEdit()
{
    Shw_pcorset()->ViewElements(&m_pcor);
    m_cboCorpus.UpdateElements(m_pcor);
    UpdateData(TRUE);   //save into m vars
    m_sOutput = m_pcor ? m_pcor->sWordListOutput() : "";
    SimplifyOutputFileName();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder();
}

void CWordListDlg::ShowCorpusFolder()
{
	Str8 sCorpusFolder = ""; // 1.5.0fg 
	if ( m_pcor ) // 1.5.0fg 
		sCorpusFolder = m_pcor->sCorpusFolder(); // 1.5.0fg 
    Str8 sDirPrompt = "Corpus Folder:"; // 1.5.0fg 
	sDirPrompt = sDirPrompt + " " + sCorpusFolder; // 1.5.0fg 
    m_lblCorpusDir.SetWindowText( swUTF16( sDirPrompt) ); // 1.4qpy
}

// Returns the full path of SF word list output file
//  The user will typically enter only a file name into the m_sOutput string,
//  but he may enter a full path. We take either and return the full path here.
Str8 CWordListDlg::sOutputFullPath()
{
    if (sGetDirPath(m_sOutput).IsEmpty())   // if no path specified in m_sOutput
        if (m_pcor)     // if corpus available (always is unless none in prj)
            return sPath(m_pcor->sCorpusFolder(),m_sOutput);    // add corpus folder
    return m_sOutput;   // else use given path/file as it is
}

// Reduces m_sOutput to briefest possible file name of SF word list output file.
// If m_sOutput has path information, the path is removed if it matches
//  the corpus folder.
void CWordListDlg::SimplifyOutputFileName()
{
    if (m_pcor) // if corpus available (always is unless none in prj)
        {
        Str8 sPath = sGetDirPath(m_sOutput);
        if ( !sPath.IsEmpty() &&    // if path matches the the corpus folder
                    bEquivFilePath (sPath, m_pcor->sCorpusFolder()) )
            m_sOutput = sGetFileName (m_sOutput, TRUE); // remove unnecessary path
        }
    // else use given path/file as it is
}


// Returns a pointer to the appropriate word list db type
//   for word list of the given corpus.
//   Makes a new db type if necessary
CDatabaseType* CWordListDlg::ptypGetWordListType(CCorpus* pcor)
{
    Str8 sWLTypePrefix;                  // prefix for db type name
    const char pszWLTypeFNPrefix[] = "WL";  // prefix for file name
    CLangEnc* plngWL;                       // the wordlist language
    CLangEnc* plngDefault;                  // the Default language
    CLangEncSet* plngset = Shw_pProject()->plngset();

    // Init for std wordlist
    sWLTypePrefix = "Word List"; // 1.5.0ft Don't translate, db type name
	sWLTypePrefix += "- "; // 1.5.0ft 
    plngDefault = plngset->plngDefault();

    // get the target language object and name
    plngWL = pcor->plng();  
    ASSERT( plngWL );
    Str8 sWLLang = plngWL->sName();

    // Check for needed db type
    const Str8 sWLTypeName = sWLTypePrefix + sWLLang;
    CDatabaseTypeSet* ptypset = Shw_pProject()->ptypset();
    CNoteList notlst;
    CDatabaseType* ptypWL = NULL;

    if ( !ptypset->bSearch(sWLTypeName, &ptypWL, notlst) )  // if needed WLType missing
        {

        // Construct the appropriate word list database type
        const Str8 sWLTypeFNBase = pszWLTypeFNPrefix  + sWLLang;
        ptypWL = ptypset->ptypNew(sWLTypeName, psz_mkrWordLstWord, sWLTypeFNBase);
        ASSERT( ptypWL );
        Str8 sDescr;
        ptypWL->SetDescription(sDescr);
        CMarkerSet* pmkrset = ptypWL->pmkrset();

        // set appropriate field properties
        Str8 sFieldName;
        CNote* pnot = NULL;
        BOOL bOK;
        CMarker* pmkr_w = ptypWL->pmkrRecord();
        ASSERT( pmkr_w );
        sFieldName = _("Word");
        bOK = pmkr_w->bSetFieldName(sFieldName, &pnot);
        ASSERT( bOK );
        pmkr_w->SetLanguageEnc(plngWL);
        ASSERT( bOK );

        sFieldName = "Count";
        bOK = pmkrset->bAddMarker(psz_mkrWordLstCount, "", sFieldName, plngDefault, &pnot) ;
        ASSERT( bOK );

        Str8 smkrCountz = (Str8)psz_mkrWordLstCount + 'z';
        sDescr = "Count field with leading zeros, for sorting by count.";
        Str8 sTemp;
        sFieldName = "Count"; // 1.5.0fv 
        sFieldName = sFieldName + " " + "Sort"; // 1.5.0fv 
        bOK = pmkrset->bAddMarker(smkrCountz, sDescr, sFieldName, plngDefault, &pnot) ;
        ASSERT( bOK );
        
	    CLangEnc* plngRef = plngDefault; // 1.1sb         
	    CNoteList notlst;
        CDatabaseTypeProxy* ptrx = ptypset->ptrxFirst(); // 1.1sb If there is a text db type, use the lang of its refs for WL refs
        for ( ; ptrx; ptrx = ptypset->ptrxNext(ptrx) )
            {
            CDatabaseType* ptyp = ptrx->ptyp(notlst);
            if ( ptyp->bTextFile() ) // 1.1sb If this type is text, use its ref lang
				{
				CMarker* pmkr = ptyp->pmkrTextRef();
				if ( pmkr )
					plngRef = pmkr->plng();
				break; // 1.1sb One text type is all that is necessary
				}
            }
        sFieldName = _("References");
        bOK = pmkrset->bAddMarker(psz_mkrWordLstRef, "", sFieldName, plngRef, &pnot); // 1.1sb If there is a text db type, use its ref lang
        ASSERT( bOK );

        ptypWL->WritePropertiesIfModified(TRUE);  // Write the .typ file now
        }

    return ptypWL;
}




/////////////////////////////////////////////////////////////////////////////
// CConcordanceDlg dialog

CConcordanceDlg::CConcordanceDlg(CCorpusSet* pcorset, CWnd* pParent /*=NULL*/)
    : CDialog(CConcordanceDlg::IDD, pParent), m_cboCorpus(pcorset)
{
    // constructor gets initial info from CCorpusSet
    //{{AFX_DATA_INIT(CConcordanceDlg)
    m_sOutput = pcorset->sConcordanceOutput();
    m_nLimitMatches = pcorset->bConcordanceLimitCount();
    m_nMatchCount = pcorset->nConcordanceMaxCount();
    m_sSearch = pcorset->sConcordanceSearch();
    m_nSearchType = pcorset->nConcordanceSearchType();
    m_bMatchCase = !pcorset->bConcordanceIgnoreCase();
    m_bAlignMatchedStrings = pcorset->bConcordanceAlignMatchedStrings();
    //}}AFX_DATA_INIT
    m_pcor = pcorset->pcorConcordanceCorpus();
    if (!m_pcor)        // could be NULL if no corpus in project yet
        m_pcor = (CCorpus*) pcorset->pselFirst();
    SimplifyOutputFileName();
}

void CConcordanceDlg::DoDataExchange(CDataExchange* pDX)
{                                   // called by UpdateData()
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CConcordanceDlg)
    DDX_Control(pDX, IDC_CONCORD_MATCHCOUNT, m_edMatchCount);
    DDX_Control(pDX, IDC_CONCORD_SEARCH, m_elcSearch);
    DDX_Control(pDX, IDC_CONCORD_CORPUS, m_cboCorpus);
    DDX_Control(pDX, IDC_CONCORD_OUTPUT, m_edOutput);
    DDX_Control(pDX, IDC_CONCORD_CURDIR, m_lblCorpusDir);
//    DDX_Txt(pDX, IDC_CONCORD_OUTPUT, m_sOutput);
//    DDX_Txt(pDX, IDC_CONCORD_SEARCH, m_sSearch);
    DDX_Text(pDX, IDC_CONCORD_MATCHCOUNT, m_nMatchCount); // 1.4dw OK because not CStrg
    DDX_Radio(pDX, IDC_CONCORD_MIDDLE, m_nSearchType);
    DDX_Radio(pDX, IDC_CONCORD_UNLIMITED, m_nLimitMatches);
    DDX_Check(pDX, IDC_CONCORD_MATCHCASE, m_bMatchCase);
    DDX_Check(pDX, IDC_CONCORD_ALIGNMATCHED, m_bAlignMatchedStrings); // 1.4vyx 
    //}}AFX_DATA_MAP
    if (pDX->m_bSaveAndValidate)
        m_pcor = m_cboCorpus.pcorSelected();    // save m_pcor
    else
        m_cboCorpus.UpdateElements(m_pcor);
}

BOOL CConcordanceDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_CONCORD_OUTPUT, swUTF16( m_sOutput ) ); // 1.4qpv
//    SetDlgItemTxt( IDC_CONCORD_SEARCH, m_sSearch); // 1.4qpn
    m_edMatchCount.EnableWindow(m_nLimitMatches);
    SetSearchLangEnc();  // 1998-07-15 MRP: font and keyboard
	m_elcSearch.SetEditLngText( m_sSearch ); // 1.4qpn
    m_elcSearch.SetFocus();
    m_elcSearch.SetSel(0, -1);
    ShowCorpusFolder();
    return FALSE;
}

void CConcordanceDlg::SetSearchLangEnc()
{
    // Set appropriate font and keyboard.
    CLangEnc* plng = NULL;
    if ( m_pcor )
        plng = m_pcor->plng();
    if ( !plng )
        plng = Shw_pProject()->plngset()->plngDefault();
    m_elcSearch.SetLangEnc(plng);
}


BEGIN_MESSAGE_MAP(CConcordanceDlg, CDialog)
    //{{AFX_MSG_MAP(CConcordanceDlg)
    ON_BN_CLICKED(IDC_CONCORD_UNLIMITED, OnUnlimited)
    ON_BN_CLICKED(IDC_CONCORD_LIMITED, OnLimited)
    ON_BN_CLICKED(IDC_CONCORD_BROWSE, OnBrowse)
    ON_BN_CLICKED(IDC_CONCORD_CORPUS_EDIT, OnCorpusEdit)
    ON_CBN_SELCHANGE(IDC_CONCORD_CORPUS, OnCorpusSelChange)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CConcordanceDlg::OnOK()
{
    CNote *pnotNoExist, *pnotDirty;
    CFileException ex;

    // Validate the data that the user gave us.
    UpdateData(TRUE);
//    GetDlgItemText( IDC_CONCORD_OUTPUT, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_CONCORD_OUTPUT, sDlgItem ); // 1.4qpw
	m_sOutput = sUTF8( sDlgItem ); // 1.4qpw
	m_sSearch = m_elcSearch.sGetEditLngText(); // 1.4qpn

    if (m_pcor == NULL) 
        {
        AfxMessageBox(_("Please select a text corpus from the list."));
        return;
        }
    if (m_sOutput.IsEmpty()) 
        {
        AfxMessageBox(_("Please provide a name for the output file."));
        m_edOutput.SetFocus();
        return;
        }
    if (m_sSearch.IsEmpty()) 
        {
        AfxMessageBox(_("Please enter a word or phrase for search for."));
        m_elcSearch.SetFocus();
        return;
        }
    if (m_nLimitMatches && m_nMatchCount==0) 
        {
        AfxMessageBox(_("The number of Matches to Display must be greater than zero."));
        m_edMatchCount.SetFocus();
        m_edMatchCount.SetSel(0, -1);
        return;
        }

    m_pcor->ValidateFilesList(&pnotNoExist, &pnotDirty, TRUE);
    if (pnotNoExist != NULL) 
        {
        AfxMessageBox(pnotNoExist->sNote());
        delete pnotNoExist;
        return;
        }
    
    if (pnotDirty != NULL) 
        {        
        if (!Shw_app().bSaveAllFiles())
            return;
        }

    // Validate the Concordance output path
    const Str8 sOutputPath = sOutputFullPath(); // derive full path from m_sOutput
    BOOL bExists = ( bFileExists(sOutputPath) );
    BOOL bReadOnly = ( bExists ? ( bFileReadOnly(sOutputPath) ) : FALSE );
    if ( bReadOnly )
        {
		UnWriteProtect( sOutputPath ); // 1.2df Turn off write protect of concordance file before trying to write it // 1.4qzhg
        }

    // Save the current dialog settings in CCorpusSet
    CCorpusSet *pcorset;
    pcorset = m_pcor->pcorsetMyOwner();
    pcorset->SetConcordanceCorpus(m_pcor);
    pcorset->SetConcordanceSearch(m_sSearch);
    pcorset->SetConcordanceSearchType(m_nSearchType);
    pcorset->SetConcordanceLimitCount(m_nLimitMatches);
    pcorset->SetConcordanceMaxCount(m_nMatchCount);
    pcorset->SetConcordanceIgnoreCase(!m_bMatchCase);
    pcorset->SetConcordanceAlignMatchedStrings(m_bAlignMatchedStrings);
    pcorset->SetConcordanceOutput(m_sOutput);

    EndDialog(IDOK);
}


// DoConcordnce() implements the Concordance Lookup feature
void CConcordanceDlg::DoConcordance(const Str8& sCurrentWord)
{
	Shw_app().bSaveAllFiles(); // 5.98b Save all files before making concordance
    // Display the Concordance Lookup dialog
    if (!sCurrentWord.IsEmpty())
            m_sSearch = sCurrentWord;   // default to the current word, if available
    if (DoModal() != IDOK)
        return;
	if ( !m_pcor->bInitCorpusRead() ) // 1.4tga
		{
		AfxMessageBox( _("No corpus files found") );
		return;
		}
	LONG lSize = m_pcor->sFilesList().GetLength(); // 1.4ya Make word list progress bar show progress
    CProgressIndicator* pprg = new CProgressIndicator( lSize, NULL ); // 1.4vyb Add progress bar during word list build
	CProgressIndicator prg = *pprg; // 1.4vyb 
	int iCurProgress = m_pcor->iCurFileNameStart(); // 1.4ya 
	
	m_sSearch.Trim(); // 1.4wha Fix U bug of conc allowing space at begin and end of search
	m_sSearch.Replace( "  ", " ", TRUE ); // 1.4whb Fix U bug of conc fail if extra space or nl in search
	if ( m_sSearch.IsEmpty() ) // 1.4tgh Don't do concordance of empty string
		return;
	if ( !m_bMatchCase ) // 1.4tgk If not matching case, make lower case equivalent // 1.4we 
		m_sSearch = m_pcor->plng()->sLowerCase( m_sSearch ); // 1.4we Fix U bug of conc not matching upper case search string

	int iSeachType = m_nSearchType; // 1.4tgk
	WrdInfoList wrdlst; // 1.4tga Place to build concordance, word is line, count is position
	WrdInfo* pwrdPrev = NULL;
    int iMaxConcLines = m_nLimitMatches ? m_nMatchCount : 10000000; // 1.4tga If no limit, use a huge number
	int iNumConcLines = 0; // 1.4tga Number of concordance lines, in case there is a limit
	Str8 sCorpusLine;
	while ( m_pcor->bGetNextLineOfCorpus( sCorpusLine, TRUE ) ) // 1.4tga Build concordance // 1.4vye 
		{
		Str8 sLowerCaseLine; // 1.4tgk 
		if ( !m_bMatchCase ) // 1.4tgk If not matching case, make lower case equivalent
			sLowerCaseLine = m_pcor->plng()->sLowerCase( sCorpusLine ); // 1.4tgk 
		int iFound = -1;
		while ( TRUE ) // 1.4tga While search string found, if it fits match criteria, build concordance line
			{
			bool bFound = FALSE; // 1.4tgm 
			if ( !m_bMatchCase ) // 1.4tgk If not match case, search in lower case string
				iFound = sLowerCaseLine.Find( m_sSearch, iFound + 1 ); // 1.4tga Look for search string // 1.4tgc Fix problem of using current word instead of search from dlg box // 1.4tgk 
			else
				iFound = sCorpusLine.Find( m_sSearch, iFound + 1 ); // 1.4tga Look for search string // 1.4tgc Fix problem of using current word instead of search from dlg box
			if ( iFound >= 0 ) // If found, check for possible begin and end limitations
				bFound = TRUE;
			if ( !bFound ) // If not found, this line is done // 1.4yc Fix U bug of conc sometimes failing to find a whole word (moved this up here from after whole word check)
				break;
			if ( bFound && m_nSearchType > 0 ) // 1.4tgm If limited to begin or end, check it
				{
				if ( m_nSearchType == 1 || m_nSearchType == 3 ) // 1.4tgm If beginning or whole, check beginning
					{
					if ( iFound > 0 ) // 1.4tgm If not at the very beginning, check for punctuation
						{
						int iPrevChar = iFound;
						m_pcor->plng()->PrevEditingUnit( sCorpusLine, &iPrevChar ); // 1.4tgm Find previous character
						if ( !m_pcor->plng()->bIsWdSep( sCorpusLine, iPrevChar ) ) // 1.4tgm If not punctuation, fail // 1.4vyy Change to use bIsWdSep
							bFound = FALSE; // 1.4tgm Set to fail
						}
					}
				if ( bFound && m_nSearchType >= 2 ) // 1.4tgm If end or whole, check end
					{
					int iEndChar = iFound + m_sSearch.GetLength(); // 1.4tgm 
					if ( iEndChar < sCorpusLine.GetLength() ) // 1.4tgm If not at the very end, check for punctuation
						{
						if ( !m_pcor->plng()->bIsWdSep( sCorpusLine, iEndChar ) ) // 1.4tgm If not punctuation, fail // 1.4vyy Change to use bIsWdSep
							bFound = FALSE; // 1.4tgm Set to fail
						}
					}
				}
			if ( bFound ) // If not found, this line is done // 1.4yk Fix bug (1.4yc) of conc not limiting to whole word (needed this both here and above)
				{
				int iStart = 0; // 1.5.1pa 
				int iFoundShortened = iFound; // 1.5.1pa 
				int iEnd = iFoundShortened + 300; // 1.5.1pa 
				if ( iEnd < sCorpusLine.GetLength() ) // 1.5.1pa 
					m_pcor->plng()->NextEditingUnit( sCorpusLine, &iEnd ); // Find next character boundary // 1.5.1pa 
				if ( iFoundShortened > 300 ) // 1.5.1pa Don't include all of long field in concordance line
					{
					iStart = iFoundShortened - 300; // 1.5.1pa 
					m_pcor->plng()->PrevEditingUnit( sCorpusLine, &iStart ); // Find previous character boundary // 1.5.1pa 
					iFoundShortened = iFoundShortened - iStart; // 1.5.1pa 
					}
				Str8 sCorpusLineShortened = sCorpusLine.Mid( iStart, iEnd - iStart ); // 1.5.1pa // 1.5.8t Fix bug of concordance line trailing context too long
				WrdInfo* pwrd = wrdlst.pwrdAdd( sCorpusLineShortened, pwrdPrev ); // 1.4tga If found, build conc struct // 1.5.1pa Build with shortened line
				pwrd->iCount = iFoundShortened; // Note where in line the search string was found
				pwrd->sRefs = m_pcor->sCurRef(); // Put in reference
				iNumConcLines++; // 1.4tga Count the number of concordance lines built
				pwrdPrev = pwrd;  // 1.5.8vb Fix bug of concordance lines of same ref in reverse order
				}
			}
		if ( iNumConcLines >= iMaxConcLines ) // 1.4tga If number of lines has reached max, don't look further
			break;
		if ( iCurProgress != m_pcor->iCurFileNameStart() ) // 1.4ya If corpus file number has changed, update progress
			{
			iCurProgress = m_pcor->iCurFileNameStart(); // 1.4ya
			prg.bUpdateProgress( iCurProgress ); // 1.4ya 
			}
		}

    const Str8 sOutputPath = sOutputFullPath(); // derive full path from m_sOutput
	CDatabaseType* ptypConc = ptypGetConcordanceType(m_pcor);
    Str8 sOutput;
	sOutput.Format (psz_fmtConcHdr, (const char*) ptypConc->sName() ); // 1.4tgb Write db type header
	FILE* pfil = nullptr;
	errno_t err = fopen_s(&pfil, sOutputPath, "w");	// 1.4tgb Open concordance file to write a new one
	if (err != 0 || pfil == nullptr) // 1.4ywf Fix U bug of crash if concordance path does not exist
	{
		AfxMessageBox( _("Error: Output path does not exist.") ); // 1.4ywf 
		return; // 1.4ywf 
	}
    fputs( sOutput, pfil );
	for ( WrdInfo* pwrd = wrdlst.pwrdFirst; pwrd; pwrd = pwrd->pwrdNext ) // 1.4tgb Write conc lines to file
		{
		Str8 sMatch = m_sSearch; // 1.4vyv Add option for conc to align matched strings
		int iStartTarget = pwrd->iCount; // 1.4wf 
		int iEndTarget = iStartTarget + m_sSearch.GetLength(); // 1.4wf Fix U bug of conc not showing phrase as target
		Str8 sConcLine = pwrd->sWord;
		if ( !m_bAlignMatchedStrings ) // 1.4vyv If not align matched strings, show whole word in target field
			{
			while ( iStartTarget > 0 ) // 1.4vyv Move back to previous white space
				{
				int iCharLeft = m_pcor->plng()->iCharLeftNumBytes( (const char*)sConcLine + iStartTarget, iStartTarget ); // 1.4ywd 
				iStartTarget -= iCharLeft; // 1.4ywd Fix U bug of conc including extra word in target field
				if ( m_pcor->plng()->bIsWdSep( sConcLine, iStartTarget ) ) // >bIsWhite( (const char*)sConcLine + iStartTarget ) ) // 1.4wf 
					{
					iStartTarget += iCharLeft; // 1.4wf  // 1.4ywd 
					break;
					}
				}
			iEndTarget = iStartTarget; // 1.4wf  // 1.4ywd 
			m_pcor->plng()->bNextWord( sConcLine, sMatch, iEndTarget ); // 1.4wf Find end of last word
			}
		sMatch = sConcLine.Mid( iStartTarget, iEndTarget - iStartTarget ); // 1.4wf 
		Str8 sBefore = sConcLine.Left( iStartTarget ); // + " "; // 1.4tgn Don't add extra space on concordance
		Str8 sAfter = sConcLine.Mid( iEndTarget ); // 1.4tgc  // 1.4tgn  // 1.4vyv  // 1.4wf 
		sBefore.Replace( "  ", " ", TRUE ); // 1.4vyt Delete extra spaces from concordance line
		sAfter.Replace( "  ", " ", TRUE ); // 1.4vyt Delete extra spaces from concordance line
		if ( m_bAlignMatchedStrings ) // 1.4vyw Add extra space to separate word if aligned
			{
			if ( sBefore.Right( 1 ) == " " ) // 1.4vyw 
				sBefore += " "; // 1.4vyw 
			if ( sAfter.Left( 1 ) == " " ) // 1.4vyw 
				sAfter = " " + sAfter; // 1.4vyw 
			}
		sOutput = "";
		sOutput += "\n\\concref " + pwrd->sRefs;
		sOutput += "\n\\concbef " + sBefore;
		sOutput += "\n\\conctar " + sMatch; // 1.4tgc // 1.4vyv 
		sOutput += "\n\\concaft " + sAfter;
	    fputs( sOutput, pfil );
		}
    fclose( pfil );

            // On success, open the SF file in browse view.
            //  (first, see if it's already open)
            CFileStatus filestat;
            VERIFY(CFile::GetStatus( swUTF16( sOutputPath ), filestat)); // 1.4qvx Upgrade GetStatus for Unicode build
			CString sw = filestat.m_szFullName; // 1.4qwf Upgrade pdocGetDocByPath for Unicode build
            CShwDoc *pdoc = Shw_app().pdocGetDocByPath( sUTF8( sw ) ); // 1.4qwf
            if (pdoc) 
                {
                // If it's already open, refresh it.
                WINDOWPLACEMENT wp;
                Shw_app().pviewTop()->GetParent()->GetWindowPlacement(&wp);
                Shw_app().bRefreshADoc(pdoc);
                if (wp.showCmd == SW_SHOWMAXIMIZED)
                    ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->ShowWindow(SW_SHOWMAXIMIZED);
                else
                    ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->ShowWindow(SW_NORMAL);
            
                ((CFrameWnd*) Shw_app().GetMainWnd())->GetActiveFrame()->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
                } 
            else 
                {
                // If it's not open, open it using some predefined
                // properties that will open it in browse mode.
                CNoteList notlst;
                int z;
                CShwView *pshv;
                Shw_app().SetZ();
                pshv = Shw_app().pviewTop();
                if (pshv)
                    z = pshv->z() + 1;
                else
                    z = 0;
              
                //09-05-1997 Make a difference between languages (Becouse of justification)
                char* pszBuffer;
                if (m_pcor->plng()->bRightToLeft())
                    {
                    pszBuffer = new char[strlen(psz_LookupWndSettingsRightToLeftScript) +
                                         strlen(sOutputPath)];
                    wsprintfA(pszBuffer, psz_LookupWndSettingsRightToLeftScript,  // 1.4qwe
                             (LPCSTR) sOutputPath, z);
                    }
                else
                    {
                    pszBuffer = new char[strlen(psz_LookupWndSettingsLeftToRightScript) +
                                         strlen(sOutputPath)];
                    wsprintfA(pszBuffer, psz_LookupWndSettingsLeftToRightScript, // 1.4qwe Upgrade wsprintf for Unicode build
                             (LPCSTR) sOutputPath, z);
                    }
                std::istrstream iosIn(pszBuffer);
                Object_istream obsIn(iosIn, notlst);
                CShwDoc::s_bReadProperties(obsIn);
                delete pszBuffer;
                } // else - if pdoc
	Shw_app().bSaveAllFiles(); // 1.4tgf Save wordlist after it is loaded and sorted
}


void CConcordanceDlg::OnLimited()
{
    m_edMatchCount.EnableWindow(TRUE);
    m_edMatchCount.SetFocus();
    m_edMatchCount.SetSel(0, -1);
}

void CConcordanceDlg::OnUnlimited()
{
    m_edMatchCount.EnableWindow(FALSE);
}

void CConcordanceDlg::OnBrowse()
{
    UpdateData(TRUE);   //save into m vars
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlg(FALSE, NULL, swUTF16( sOutputFullPath() ) ); // 1.4qrd // 1.5.8h 
		dlg.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlg ) != IDOK) 
			return; // 1.5.8h 
		sPath =  sUTF8( dlg.GetPathName() ); // 1.4qxf
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sOutput) ) // 1.5.8h 
			return; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return; // 1.4vze 
    m_sOutput = sPath; // 1.5.8h 
    SimplifyOutputFileName();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder(); // prob unchanged I think
}

void CConcordanceDlg::OnCorpusSelChange()
{
    UpdateData(TRUE);   //save into m vars
    SimplifyOutputFileName();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder();
    SetSearchLangEnc();  // 1998-07-15 MRP: font and keyboard
}

void CConcordanceDlg::OnCorpusEdit()
{
    Shw_pcorset()->ViewElements(&m_pcor);
    m_cboCorpus.UpdateElements(m_pcor);
    UpdateData(TRUE);   // save into m vars
    SimplifyOutputFileName();
    SetDlgItemText( IDC_WORDLST_OUTPUT, swUTF16( m_sOutput ) ); // 1.4kt Fix bug of not updating file name in wordlist dialog // 1.4qpv
    ShowCorpusFolder();
}

void CConcordanceDlg::ShowCorpusFolder()
{
	Str8 sCorpusFolder = ""; // 1.5.0fg 
	if ( m_pcor ) // 1.5.0fg 
		sCorpusFolder = m_pcor->sCorpusFolder(); // 1.5.0fg 
    Str8 sDirPrompt = "Corpus Folder:"; // 1.5.0fg 
	sDirPrompt = sDirPrompt + " " + sCorpusFolder; // 1.5.0fg 
    m_lblCorpusDir.SetWindowText( swUTF16( sDirPrompt) ); // 1.4qpy
}

// Returns the full path of SF Concordance Lookup output file
//  The user will typically enter only a file name into the m_sOutput string,
//  but he may enter a full path. We take either and return the full path here.
Str8 CConcordanceDlg::sOutputFullPath()
{
    if (sGetDirPath(m_sOutput).IsEmpty())   // if no path specified in m_sOutput
        if (m_pcor)     // if corpus available (always is unless none in prj)
            return sPath(m_pcor->sCorpusFolder(),m_sOutput);    // add corpus folder
    return m_sOutput;   // else use given path/file as it is
}

// Reduces m_sOutput to briefest possible file name of SF concordance output file.
// If m_sOutput has path information, the path is removed if it matches
//  the corpus folder.
void CConcordanceDlg::SimplifyOutputFileName()
{
    if (m_pcor) // if corpus available (always is unless none in prj)
        {
        Str8 sPath = sGetDirPath(m_sOutput);
        if ( !sPath.IsEmpty() &&    // if path matches the the corpus folder
                    bEquivFilePath (sPath, m_pcor->sCorpusFolder()) )
            m_sOutput = sGetFileName (m_sOutput, TRUE); // remove unnecessary path
        }
    // else use given path/file as it is
}


// Returns a pointer to the appropriate concordance db type
//   for concordance lookup of the given corpus.
// Makes a new db type if necessary
// One of two types may be needed/created:
//   by default, the Concordance-lng type uses the Default lng for conc refs
//   if corpus has Book-Chapter-Verse refs, 
//               the Concord_Book-lng type uses the BookRef lng for conc refs
CDatabaseType* CConcordanceDlg::ptypGetConcordanceType(CCorpus* pcor)
{
    Str8 sConcTypePrefix;    // prefix for db type name
    Str8 sConcTypeFNPrefix;  // prefix for file name
    CLangEnc* plngConc;         // the conc target language
    CLangEnc* plngConcRef;      // the conc reference language
//    CLangEnc* plngConcRefz;     // the conc reference language (zero-filled) // 1.4ywc 
    CLangEncSet* plngset = Shw_pProject()->plngset();

    // Init for Default sort parameters
    sConcTypePrefix = "Concordance"; // 1.5.0ft Don't translate, db type name
	sConcTypePrefix += "- "; // 1.5.0ft 
    sConcTypeFNPrefix = "CD";       //for file name 
    plngConcRef = plngset->plngDefault();   // concref use Default lng
    CNoteList notlst;

    // Get the target language object & name
    plngConc = pcor->plng();    
    ASSERT( plngConc );
    Str8 sConcLang = plngConc->sName();

    // Check for needed db type
    const Str8 sConcTypeName = sConcTypePrefix + sConcLang;
    CDatabaseTypeSet* ptypset = Shw_pProject()->ptypset();
    CDatabaseType* ptypConc = NULL;

    if ( !ptypset->bSearch(sConcTypeName, &ptypConc, notlst) )  // if needed ConcType missing
        {

        // Construct the appropriate concordance database type
        const Str8 sConcTypeFNBase = sConcTypeFNPrefix  + sConcLang;
        ptypConc = ptypset->ptypNew(sConcTypeName, psz_mkrConcRef, sConcTypeFNBase);
        ASSERT( ptypConc );
        Str8 sDescr;
        ptypConc->SetDescription(sDescr);
        CMarkerSet* pmkrset = ptypConc->pmkrset();
        pmkrset->SetDefaultLangEnc(plngConc);

        // set appropriate field properties
        Str8 sFieldName;
        CNote* pnot = NULL;
        BOOL bOK;
        CMarker* pmkr_r = ptypConc->pmkrRecord();
        ASSERT( pmkr_r );
        sFieldName = "Reference";
        bOK = pmkr_r->bSetFieldName(sFieldName, &pnot);
        ASSERT( bOK );
        CDatabaseTypeProxy* ptrx = ptypset->ptrxFirst(); // 1.4ywc If there is a text db type, use the lang of its refs for conc refs
        for ( ; ptrx; ptrx = ptypset->ptrxNext(ptrx) )
            {
            CDatabaseType* ptyp = ptrx->ptyp(notlst);
            if ( ptyp->bTextFile() ) // 1.1sb If this type is text, use its ref lang
				{
				CMarker* pmkr = ptyp->pmkrTextRef();
				if ( pmkr )
					plngConcRef = pmkr->plng();
				break; // 1.1sb One text type is all that is necessary
				}
            }
        pmkr_r->SetLanguageEnc(plngConcRef);
        ASSERT( bOK );
        pmkr_r->SetDataType(FALSE, TRUE);

        sFieldName = "Before";
        bOK = pmkrset->bAddMarker(psz_mkrConcBefore, "", sFieldName, plngConc, &pnot) ;
        ASSERT( bOK );

        sFieldName = "Target";
        bOK = pmkrset->bAddMarker(psz_mkrConcTarget, "", sFieldName, plngConc, &pnot) ;
        ASSERT( bOK );

        sFieldName = "After";
        bOK = pmkrset->bAddMarker(psz_mkrConcAfter, "", sFieldName, plngConc, &pnot) ;
        ASSERT( bOK );

        ptypConc->WritePropertiesIfModified(TRUE);  // Write the .typ file now
        }

    return ptypConc;
}

WrdInfo::WrdInfo()
	{
	iCount = 0;
	pwrdNext = NULL;
	}

WrdInfo::WrdInfo( const char* pszWord )
	{
	sWord = pszWord;
	iCount = 1;
	pwrdNext = NULL;
	}

WrdInfoList::WrdInfoList()
	{
	pwrdFirst = NULL;
	}

WrdInfoList::~WrdInfoList()
	{
	while ( pwrdFirst )
		{
		WrdInfo* pwrd = pwrdFirst->pwrdNext;
		delete pwrdFirst;
		pwrdFirst = pwrd;
		}
	}

WrdInfo* WrdInfoList::pwrdAdd( const char* pszWord, WrdInfo* pwrdPrev ) // Add a new word to the list
	{
	WrdInfo* pwrd = new WrdInfo( pszWord );
	if ( pwrdPrev ) // If prev given, add after it
		{
		pwrd->pwrdNext = pwrdPrev->pwrdNext;
		pwrdPrev->pwrdNext = pwrd;
		}
	else // Else, no prev given, add at top of list
		{
		pwrd->pwrdNext = pwrdFirst;
		pwrdFirst = pwrd;
		}
	return pwrd;
	}

WrdInfo* WrdInfoList::pwrdSearch( const char* pszWord, WrdInfo*& pwrdPrev ) // Search for a word
	{
	pwrdPrev = NULL;
	for ( WrdInfo* pwrd = pwrdFirst; pwrd; pwrd = pwrd->pwrdNext )
		{
		if ( !strcmp( pszWord, (const char*) pwrd->sWord ) ) // If word found, return it
			return pwrd;
		pwrdPrev = pwrd; // Remember previous word
		}
	return NULL;
	}

void WrdInfoList::MoveToTop( WrdInfo* pwrd, WrdInfo* pwrdPrev ) // Move a word to top of list for faster search
	{
	if ( pwrdPrev ) // 1.4ted Don't move if no prev given, maybe already at top
		{
		pwrdPrev->pwrdNext = pwrd->pwrdNext; // Point prev to next
		pwrd->pwrdNext = pwrdFirst; // Point word to old first
		pwrdFirst = pwrd; // Point first to word
		}
	}
