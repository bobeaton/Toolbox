// lng_d.cpp  Implementation of a dialog box representing a language encoding

#include "stdafx.h"
#include "toolbox.h"
#include "afxcmn.h" // for CTabCtrl
#include "lng_d.h"  
#include "project.h"
#include "shw.h"
#include "kmn.h"
#include "tools.h"
#include "ddx.h"
#include "typ.h"
#include "mkr.h"
#include "corpus.h"
#include "alan.h" // 1.5.8h 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CLangEncSetDlg::CLangEncSetDlg(CLangEncSetListBox& lboSet, CLangEncSet* plngset) :
    CSetDlg(lboSet, _("Language Encodings"), Shw_pProject()->sSettingsDirLabel(), IDD_lngset, IDD_lngset),
    m_plngset(plngset)
{
    ASSERT( m_plngset );
    m_bAutoSwitching = TRUE; // 1.1kw Remove AutoSwitching option // m_plngset->pkbdset()->bAutoSwitching();
    m_bAutoSwitchingOriginal = TRUE; // 1.1kw Remove AutoSwitching option // m_bAutoSwitching;
}


void CLangEncSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CSetDlg::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLangEncSetDlg)
    DDX_Control(pDX, IDC_kbd_chkAutoSwitching, m_chkAutoSwitching);
    DDX_Check(pDX, IDC_kbd_chkAutoSwitching, m_bAutoSwitching);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLangEncSetDlg, CSetDlg)
    //{{AFX_MSG_MAP(CLangEncSetDlg)
    ON_BN_CLICKED(IDC_kbd_chkAutoSwitching, OnAutoSwitching)
    ON_BN_CLICKED(IDC_CLOSE, OnClose)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLangEncSetDlg::OnInitDialog()
{
    BOOL b = CSetDlg::OnInitDialog();

#ifdef _MAC
    m_chkAutoSwitching.ShowWindow(SW_HIDE);
#endif

    return b;  // input focus will be set to the dialog's first control
}

void CLangEncSetDlg::OnAutoSwitching()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
    m_plngset->pkbdset()->SetAutoSwitching(m_bAutoSwitching);
}

void CLangEncSetDlg::OnClose()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
    m_plngset->pkbdset()->SetAutoSwitching(m_bAutoSwitching);
    CSetDlg::OnClose();
}

void CLangEncSetDlg::OnCancel()
{
    m_plngset->pkbdset()->SetAutoSwitching(m_bAutoSwitchingOriginal);
    CSetDlg::OnCancel();
}


// --------------------------------------------------------------------------

CLangEncSheet::CLangEncSheet(CLangEnc* plng, BOOL bAddOrCopy, int iActivePage,
        CSortOrder* psrt, CVarInstance* pvin) :
    CPropertySheet(_T(" "), NULL, iActivePage), // 1.4qur Upgrade CPropertySheet for Unicode build
	m_plng(plng),
    m_sName(m_plng->sName()),
    m_sDescription(m_plng->sDescription()),
    m_sCase(m_plng->sCase()),
    m_sPunct(m_plng->sPunct()),
    m_bAddOrCopy(bAddOrCopy),
    m_psrt(psrt),
    m_psrtDefaultOriginal(m_plng->psrtset()->psrtDefault()),
    m_lboSortOrders(m_plng->psrtset(), &m_psrt),
    m_dlgSortOrders(m_lboSortOrders, m_plng->psrtset()),
    m_dlgCase(m_plng, &m_sCase, &m_sPunct ), // 1.4qb
    m_pvin(pvin),
    m_lboVars(m_plng->pvinset(), &m_pvin),
    m_dlgVars(m_lboVars, IDS_VARSET_TITLE), // 1.4gf OK, doesn't use CStrg
    m_pmch(NULL),
    m_dlgOptions(m_plng, m_bAddOrCopy, &m_sName, &m_sDescription)
{
    // 1996-02-28 MRP: Move these to OnCancel eventually
    m_plng->ClearPropertiesDlgFont();
    m_plng->ClearPropertiesDlgKeyboard();
    
    m_dlgSortOrders.SetHelpID(IDD_lng_SortOrdersPage);
    AddPage(&m_dlgSortOrders);
    ASSERT(eSortOrderSetPage == 0);

    m_dlgCase.SetHelpID(IDD_lng_CasePage);
    AddPage(&m_dlgCase);
    ASSERT(eCasePage == eSortOrderSetPage+1);

    m_dlgVars.SetHelpID(IDD_lng_VariablesPage);
    AddPage(&m_dlgVars);
    ASSERT(eVarSetPage == eCasePage+1);
    
    m_dlgOptions.SetHelpID(IDD_lng_OptionsPage);
    AddPage(&m_dlgOptions);
    ASSERT(eOptionsPage == eCharSetPage+1);

	Str8 sTitle = _("Language Encoding Properties"); // 1.4sb
    if ( !m_bAddOrCopy )
		sTitle = sTitle + " - " + m_plng->sName(); // 1.4sb // 1.5.0fd 
	SetTitle( swUTF16( sTitle  ) ); // 1.4qus Upgrade SetTitle for Unicode build
}

BEGIN_MESSAGE_MAP(CLangEncSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CLangEncSheet)
    ON_COMMAND(IDOK, OnOK)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

afx_msg void CLangEncSheet::OnOK()
{
    m_dlgSortOrders.OnOK();
    m_dlgCase.OnOK();
    m_dlgVars.OnOK();
    m_dlgOptions.OnOK();
    
    CLangEncSet* plngset = m_plng->plngsetMyOwner();
    ASSERT( m_bAddOrCopy == !plngset->bIsMember(m_plng) );

    if ( !bEqual( m_sName, m_plng->sName() ) ) // 1.1rd If name is changed to another existing one, offer to change to different language everywhere
		{
		CLangEnc* plngNew = plngset->plngSearch( m_sName );
		if ( plngNew ) // 1.1rd If new name is an already existing language, change it everywhere
			{
			int iResult = AfxMessageBox( _("Change language everywhere?"), MB_YESNO ); // 1.5.0fg 
			if ( iResult == IDYES )
				{
				CDatabaseTypeSet* ptypset = Shw_pProject()->ptypset();
				CDatabaseTypeProxy* ptrx = ptypset->ptrxFirst();
				for ( ; ptrx; ptrx = ptypset->ptrxNext(ptrx) ) // For every database type
					{
					CNoteList notlst;
					CDatabaseType* ptyp = ptrx->ptyp(notlst);
					CMarkerSet* pmkrset = ptyp->pmkrset();
					CMarker* pmkr = pmkrset->pmkrFirst();
					for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) ) // For every marker in database type
						{
						if ( pmkr->plng() == m_plng ) // If marker is assigned to the current language, change it to the new language
							{
							pmkr->SetLanguageEnc( plngNew ); // 1.1rd Set marker to new language encoding
							pmkr->SetFont( NULL ); // Set to use language font (later try to change font but keep style)
							}
						}
					CCorpusSet* pcorset = Shw_pcorset();
					CCorpus* pcor = (CCorpus*)pcorset->pelFirst();
					for ( ; pcor; pcor = (CCorpus*)pcorset->pelNext( pcor ) ) // For every corpus in corpus set
						{
						if ( pcor->plng() == m_plng ) // If corpus is assigned to the current language, change it
							pcor->SetLangEnc( plngNew ); // 1.1rd Set corpus to new language encoding
						}
					}
			    EndDialog(IDOK);
				return;
				}
			}
		}
    
	
	CNote* pnot = NULL;
    BOOL bOK = ( m_bAddOrCopy ?
        plngset->bAddLangEnc(m_sName, m_sDescription, m_sCase, m_sPunct, // 1.4qb
            m_psrtDefaultOriginal, m_plng, &pnot) :
        m_plng->bModifyProperties(m_sName, m_sDescription, m_sCase, m_sPunct, // 1.4qb
            m_psrtDefaultOriginal, &pnot)
        );
        
    if ( !bOK )
        {
        ASSERT( pnot );
        
        // Show the note on the proper page
        BOOL bNoteShown = FALSE;
        if ( m_dlgOptions.bContainsReferentOfNote(pnot, this) )
            bNoteShown = TRUE;
        else if ( m_dlgCase.bContainsReferentOfNote(pnot, this) )
            bNoteShown = TRUE;
//      ASSERT( bNoteShown );
            
        MessageBeep(0);
        delete pnot;
        return;
        }

    EndDialog(IDOK);
}

BOOL CLangEncSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    CButton *pBtn = (CButton*)GetDlgItem(ID_APPLY_NOW);
    ASSERT(pBtn);
    pBtn->ShowWindow(SW_HIDE); // make Apply button do the disappearing act

	SetButtonLabels();  // 1998-09-30 MRP: For French version

    return TRUE;
}
int CLangEncSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CLangEncSheet::SetButtonLabels()
{
	// 1998-09-30 MRP: For French version
	CButton *pbtn = NULL;
	Str8 sLabel;
    
    pbtn = (CButton*)GetDlgItem(IDOK);
    ASSERT( pbtn );
    sLabel = _("OK");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
    
    pbtn = (CButton*)GetDlgItem(IDCANCEL);
    ASSERT( pbtn );
    sLabel = _("Cancel");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy

    pbtn = (CButton*)GetDlgItem(IDHELP);
    ASSERT( pbtn );
    sLabel = _("Help");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
}

void CLangEncSheet::SetActivePage(int ePage)
{
    CPropertySheet::SetActivePage(ePage); // switch to page
}


// --------------------------------------------------------------------------
                    
CCasePage::CCasePage( CLangEnc* plng, Str8* psCase, Str8* psPunct ) // 1.4qb
    : CPropertyPage(CCasePage::IDD),
    m_bWasActive(FALSE),
    m_plng(plng),
    m_sCase(*psCase),
    m_sPunct(*psPunct) // 1.4qb
{
    ASSERT( m_plng );
}

void CCasePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_lng_edtCase, m_elcCase);
    DDX_Control(pDX, IDC_lng_edtPunct, m_elcPunct); // 1.4qb
    //{{AFX_DATA_MAP(CCasePage)
    DDX_Control(pDX, IDC_DLG_NOT, m_edtNote);   
    //}}AFX_DATA_MAP
//    Shw_DDX_MultiLineText(pDX, IDC_lng_edtCase, m_sCase); // 1.4hdz Replace DDX_MultiLineText in lang case dlg
}

BEGIN_MESSAGE_MAP(CCasePage, CPropertyPage)
    //{{AFX_MSG_MAP(CCasePage)
//    ON_EN_SETFOCUS(IDC_lng_edtCase, OnSetfocusCase) // 1.4qb
//2000-07-06 TLB - Use of OnSetFocus (which never got called by the framework) replaced by OnSetActive
//    ON_WM_SETFOCUS()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CCasePage::OnInitDialog()
{
    m_bWasActive = TRUE;
    CPropertyPage::OnInitDialog();
//	m_sPrimaries.Replace( "\n", "\r\n" ); // 1.4hdz Replace not needed for edit language control
//    SetDlgItemTxt( IDC_lng_edtCase, m_sCase); // 1.4hdz // 1.4qpn
	m_elcCase.SetEditLngText( m_sCase ); // 1.4qpn
    m_elcCase.SetLangEnc(m_plng);
//    SetDlgItemTxt( IDC_lng_edtPunct, m_sPunct); // 1.4qb // 1.4qpn
	m_elcPunct.SetEditLngText( m_sPunct ); // 1.4qpn
    m_elcPunct.SetLangEnc(m_plng); // 1.4qb
    return TRUE;
}

BOOL CCasePage::OnSetActive()
{
    m_elcCase.Invalidate();
    m_elcPunct.Invalidate();
    return CPropertyPage::OnSetActive();
}

void CCasePage::OnOK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;
    UpdateData(TRUE);  // from dialog controls to dialog object member data
	m_sCase = m_elcCase.sGetEditLngText(); // 1.4qpn
	m_sPunct = m_elcPunct.sGetEditLngText(); // 1.4qpn
    CPropertyPage::OnOK();
}


BOOL CCasePage::bContainsReferentOfNote(CNote* pnot, CLangEncSheet* pdlg)
{
    ASSERT( pnot );
    if ( !pnot->bRefersToString(m_sCase) )
        return FALSE;  // Note did not refer to any of this page's controls
    ASSERT( pdlg );
    pdlg->SetActivePage(CLangEncSheet::eCasePage);
    m_elcCase.SetFocus();               
    // Show the note on the dialog itself
    m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
    m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
    return TRUE;
}

// --------------------------------------------------------------------------

void CCharSetPage::UpdateButtons()
{
    // Leave the buttons invisible and disabled
}

void CCharSetPage::UpdateDeleteButton()
{
    // Leave the buttons invisible and disabled
}


// --------------------------------------------------------------------------

CLangEncOptionsPage::CLangEncOptionsPage( CLangEnc* plng, BOOL bAddOrCopy,
        Str8* psName, Str8* psDescription )
    : CPropertyPage(CLangEncOptionsPage::IDD),
    m_plng(plng),
    m_plngset(m_plng->plngsetMyOwner()),
    m_bAddOrCopy(bAddOrCopy),
    m_fntdef( m_plng->pfnt(), m_plng->rgbColor() ),
    m_sName(*psName),
    m_sDescription(*psDescription),
    m_cboKeyboard(Shw_plngset()->pkbdset())
{
    m_bWasActive = FALSE;
    ASSERT( m_plng );
    ASSERT( m_plngset );
}

void CLangEncOptionsPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLangEncOptionsPage)
	DDX_Control(pDX, IDC_LBL_KeymanFile, m_lblKeymanFile);
	DDX_Control(pDX, IDC_UseInternalKeyboard, m_chkUseInternalKeyboard);
    DDX_Control(pDX, IDC_lng_edtName, m_edtName);   
    DDX_Control(pDX, IDC_lng_edtDescription, m_edtDescription); 
    DDX_Control(pDX, IDC_kbd_lblKeyboard, m_lblKeyboard);
    DDX_Control(pDX, IDC_kbd_cboKeyboard, m_cboKeyboard);   
    DDX_Control(pDX, IDC_DLG_NOT, m_edtNote);   
	//}}AFX_DATA_MAP
//    m_plngset->DDX_Name(pDX, IDC_lng_edtName, m_sName); // 1.4hde Replace DDX_Name in lang enc options pg dlg
//    Shw_DDX_MultiLineText(pDX, IDC_lng_edtDescription, m_sDescription); // 1.4hdt Replace DDX_MultiLineText in lang enc options pg dlg
}

BEGIN_MESSAGE_MAP(CLangEncOptionsPage, CPropertyPage)
    //{{AFX_MSG_MAP(CLangEncOptionsPage)
    ON_CBN_SELCHANGE(IDC_kbd_cboKeyboard, OnSelchangeKeyboard)
    ON_BN_CLICKED(IDC_LANGENC_FONT, OnFont)
    ON_BN_CLICKED(IDC_BUTTON_LNG_ADV, OnAdvancedOptions)
	ON_BN_CLICKED(IDC_Browse_KeymanFile, OnBrowseKeymanFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CLangEncOptionsPage message handlers

BOOL CLangEncOptionsPage::OnInitDialog()
{
    m_bWasActive = TRUE;
    CPropertyPage::OnInitDialog();
	m_sDescription.Replace( "\n", "\r\n" ); // 1.4hdt Use Replace to change lf to crlf
    SetDlgItemText( IDC_lng_edtDescription, swUTF16( m_sDescription ) ); // 1.4hdt // 1.4qpv
    SetDlgItemText( IDC_lng_edtName, swUTF16( m_sName ) ); // 1.4hdd // 1.4qpv
#ifdef _MAC // Macintosh: Hide the Keyboard label and drop-down list.
    m_lblKeyboard.ShowWindow(SW_HIDE);
    m_cboKeyboard.ShowWindow(SW_HIDE);
#else // Windows: Show the selected keyboard and enable the drop-down list
	Str8 sKeyboardName = m_plng->pkbd() ? m_plng->pkbd()->sName() : "";
	if ( sKeyboardName.Find( ".kmx" ) > 0 ) // If it is a keyman file, show it as that
		{
		m_sKeymanFile = sKeyboardName;
		m_lblKeymanFile.SetWindowText(  swUTF16( sGetFileName(m_sKeymanFile) ) ); // Set display to file name // 1.4qpy
		m_cboKeyboard.UpdateElements(NULL);
		}
	else
		m_cboKeyboard.UpdateElements(m_plng->pkbd());
	m_cboKeyboard.EnableWindow(m_plngset->pkbdset()->bAutoSwitching());
#endif
	if ( m_plng->bUseInternalKeyboard() ) // 1.5.1mc 
		m_chkUseInternalKeyboard.SetCheck( 1 ); // 1.5.1mc 
    BOOL bLetWindowsSetFocusToName = m_bAddOrCopy;
    if ( !bLetWindowsSetFocusToName )
        m_edtDescription.SetFocus();
    return bLetWindowsSetFocusToName;
}

void CLangEncOptionsPage::OnOK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_lng_edtDescription, pszBigBuffer, BIGBUFMAX ); // 1.4hdt // 1.4kh
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_lng_edtDescription, sDlgItem ); // 1.4qpw
	m_sDescription = sUTF8( sDlgItem ); // 1.4qpw
	m_sDescription.Replace( "\r\n", "\n" ); // 1.4hdt Use Replace to change crlf to lf
//    GetDlgItemText( IDC_lng_edtName, pszBigBuffer, BIGBUFMAX  ); // 1.4hdd // 1.4kh
	GetDlgItemText( IDC_lng_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdd
	if ( m_sKeymanFile != "" ) // If there is a keyman file, use it
		{
		CKeyboardSet* pkbdSet = m_plngset->pkbdset();
		CKeyboard* pkbd = pkbdSet->pkbdSearch_AddIfNew( m_sKeymanFile );
		m_plng->SetKeyboard( pkbd );
		}
	else
		m_plng->SetKeyboard(m_cboKeyboard.pkbdSelected());
	m_plng->SetUseInternalKeyboard( m_chkUseInternalKeyboard.GetCheck() );  // 1.5.1mb 
    CPropertyPage::OnOK();
}

void CLangEncOptionsPage::OnSelchangeKeyboard()
{
    CKeyboard* pkbd = m_cboKeyboard.pkbdSelected();
	Str8 sKeyboardName = pkbd ? pkbd->sName() : "";
	if ( sKeyboardName.Find( ".kmx" ) > 0 ) // If it is a keyman file, show it as that
	{
		m_sKeymanFile = sKeyboardName;
		m_lblKeymanFile.SetWindowText(  swUTF16( sGetFileName(m_sKeymanFile) ) ); // Set display to file name // 1.4qpy
		m_cboKeyboard.UpdateElements(NULL);
	}
	else // Else (not keyman file), use keyboard and clear keyman file display
	{
		m_plng->SetPropertiesDlgKeyboard( pkbd );
		m_sKeymanFile = "";
		m_lblKeymanFile.SetWindowText( swUTF16( "None" ) ); // Clear display of name // 1.4qpy
	}
}

void CLangEncOptionsPage::OnBrowseKeymanFile() 
{
	long lFlags = (OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);
	Str8 sFilter;
	sFilter = "Keyman File (*.kmx)|*.kmx||";
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlgKeymanFile(TRUE, swUTF16( "kmx" ), swUTF16( sGetFileName( m_sKeymanFile ) ), lFlags, swUTF16( sFilter ) ); // 1.4qrd // 1.5.8h 
		Str8 sInitialDir = sGetDirPath(m_sKeymanFile);
		dlgKeymanFile.m_ofn.lpstrInitialDir =  swUTF16( sInitialDir ); // 1.4qra
		dlgKeymanFile.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlgKeymanFile ) == IDOK )
			{
			sPath = sUTF8( dlgKeymanFile.GetPathName() ); // 1.4qra
			if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
				return; // 1.4vze 
			m_sKeymanFile =  sUTF8( dlgKeymanFile.GetPathName() ); // 1.4qra
			m_lblKeymanFile.SetWindowText(  swUTF16( sGetFileName(m_sKeymanFile) ) ); // Set display to file name // 1.4qpy
			m_plng->SetKeyboard(NULL);
			m_cboKeyboard.UpdateElements(m_plng->pkbd());
			}
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sKeymanFile) ) // 1.5.8h 
			return; // 1.5.8h 	
}

void CLangEncOptionsPage::OnFont()
{
    if ( !m_fntdef.bModalView_Properties( TRUE ) )
        return;

    // 1998-02-13 MRP: After choosing a font on the Options page
    // of Language Encoding Properties, cancelling the main
    // tabbed dialog doesn't undo the choice any longer.
    // This change in behavior became necessary to support
    // context-sensitive rendering and right-to-left scripts
    // in list boxes on the Variables and Characters pages.
    m_plng->SetFont( &m_fntdef );
    // 2000-07-06 TLB - Added the following so that if user re-enters Font dialog box, the changes from first time are
    // already reflected.
    m_fntdef.Set( m_plng->pfnt(), m_plng->rgbColor() );
    // 2000-07-06 TLB - Added the following so that if other pages refer to the language to get the dialog font, it will
    // be re-created based on the new font settings chosen by the user.
    // 2000-07-24 TLB - SetFont just did this? We don't have to do it again. What was I thinking?
    // m_plng->ClearPropertiesDlgFont();
    Shw_UpdateAllViews(NULL); // make all views redraw
}


BOOL CLangEncOptionsPage::bContainsReferentOfNote(CNote* pnot, CLangEncSheet* pdlg)
{
    ASSERT( pnot );
    if ( !pnot->bRefersToString(m_sName) )
        return FALSE;  // Note did not refer to any of this page's controls
        
    ASSERT( pdlg );
    pdlg->SetActivePage(CLangEncSheet::eOptionsPage);
            
    // Set the insertion point, or select text referred to in note
    m_edtName.SetSel(pnot->dwSelReferent());
    m_edtName.SetFocus();
                
    // Show the note on the dialog itself
    m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
    m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
    
    return TRUE;
}

void CLangEncOptionsPage::OnAdvancedOptions()
{
    CLangEncAdvancedOptionsDlg dlg(m_plng);  // 1998-02-12 MRP
    int i = dlg.DoModal();
}


// --------------------------------------------------------------------------

CLangEncAdvancedOptionsDlg::CLangEncAdvancedOptionsDlg(CLangEnc* plng) :
    CDialog(CLangEncAdvancedOptionsDlg::IDD)
{
    m_plng = plng;
    ASSERT( m_plng );
    m_bRightToLeft = m_plng->bRightToLeft();
    m_bUnicodeLang = m_plng->bUnicodeLang();
    m_sRenDLL = m_plng->sRenDLL();
    m_sRenTable = m_plng->sRenTable();
}

void CLangEncAdvancedOptionsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CLangEncAdvancedOptionsDlg)
    DDX_Check(pDX, IDC_LNG_RIGHTTOLEFT, m_bRightToLeft);
	DDX_Check(pDX, IDC_LNG_UNICODE, m_bUnicodeLang);
//    DDX_Txt(pDX, IDC_LNG_DLLNAME, m_sRenDLL);
//    DDX_Txt(pDX, IDC_LNG_DLLTABLE, m_sRenTable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLangEncAdvancedOptionsDlg, CDialog)
    //{{AFX_MSG_MAP(CLangEncAdvancedOptionsDlg)
        // NOTE: the ClassWizard will add message map macros here
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLangEncAdvancedOptionsDlg::OnInitDialog()
{
    BOOL b = CDialog::OnInitDialog();
    SetDlgItemText( IDC_LNG_DLLNAME, swUTF16( m_sRenDLL ) ); // 1.4qpv
    SetDlgItemText( IDC_LNG_DLLTABLE, swUTF16( m_sRenTable ) ); // 1.4qpv
    
    return b;  // return FALSE if we set the focus
}

void CLangEncAdvancedOptionsDlg::OnOK()
{
    // 1998-02-12 MRP: FEEDBACK SHOULD BE GIVEN HERE!!!
    // if right-to-left is checked but no rendering
    // if the rendering DLL isn't found
    // if the table isn't found

    CDialog::OnOK();
//    GetDlgItemText( IDC_LNG_DLLNAME, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_LNG_DLLNAME, sDlgItem ); // 1.4qpw
	m_sRenDLL = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_LNG_DLLTABLE, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_LNG_DLLTABLE, sDlgItem ); // 1.4qpw
	m_sRenTable = sUTF8( sDlgItem ); // 1.4qpw

    m_plng->SetRightToLeft( m_bRightToLeft );
    m_plng->SetUnicodeLang( m_bUnicodeLang );
    m_plng->SetRenDLL( m_sRenDLL );
    m_plng->SetRenTable( m_sRenTable );
}

