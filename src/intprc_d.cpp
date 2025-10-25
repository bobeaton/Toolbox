//  intprc_d.cpp Interface to interlinear processes

#include "stdafx.h"
#include "toolbox.h"
#include "intsetup.h"
#include "intprc_d.h"
#include "typ.h"
#include "shw.h"
#include "shwdoc.h"
#include "project.h"
#include "wdf.h"  // 1999-09-06 MRP: CWordFormulaSet
#include "tools.h"
#include "alan.h" // 1.5.8h 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------


CIntProcListBox::CIntProcListBox( CInterlinearProcList* pintprclst ) : CDblListListBox( pintprclst )
{
    m_pintprclst = pintprclst;
}

void CIntProcListBox::InitLabels()
{
    m_xFrom = xSubItem_Show(IDC_INTPRC_FROM);
    m_xTo = xSubItem_Show(IDC_INTPRC_TO);
    m_xProc = xSubItem_Show(IDC_INTPRC_PROC);
    
}

void CIntProcListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CInterlinearProc* pintprc = (CInterlinearProc*)pel;
    DrawSubItem(cDC, rcItem, m_xFrom, m_xTo,
    	pintprc->pmkrFrom()->sName() );
    DrawSubItem(cDC, rcItem, m_xTo, m_xProc,
    	pintprc->pmkrTo()->sName() );
    DrawSubItem(cDC, rcItem, m_xProc, 0,
    	pintprc->sName() );
}

void CIntProcListBox::InitListBox()
{
    InitLabels();
    UpdateElements( m_pintprclst->pintprcFirst() );
}

// --------------------------------------------------------------------------
void CIntProcPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CIntProcPage)
	DDX_Control(pDX, IDC_EXPRESS_SETUP, m_btnExpressSetup);
	DDX_Control(pDX, IDC_MODIFY, m_btnModify);
	DDX_Control(pDX, IDC_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_ADD, m_btnAdd);
    DDX_Control(pDX, IDC_INTPRC_LIST, m_lboIntProcs);
	DDX_Text(pDX, IDC_EDIT_INTERLINEAR_SPACING, m_iInterlinearSpacing); // 1.4dw OK because not CStrg
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIntProcPage, CPropertyPage)
    //{{AFX_MSG_MAP(CIntProcPage)
    ON_LBN_DBLCLK(IDC_INTPRC_LIST, OnDblclkIntProcList)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_MODIFY, OnModify)
	ON_BN_CLICKED(IDC_EXPRESS_SETUP, OnExpressSetup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CIntProcPage::CIntProcPage( CInterlinearProcList* pintprclst ) :
    CPropertyPage(IDD), m_lboIntProcs( pintprclst )
{
	m_pintprclst = pintprclst;
	//{{AFX_DATA_INIT(CIntProcPage)
	//}}AFX_DATA_INIT
	m_iInterlinearSpacing = m_pintprclst->ptyp()->iInterlinCharWd();
    m_bWasActive = FALSE;
}

BOOL CIntProcPage::OnInitDialog()
{
    m_bWasActive = TRUE;
    CPropertyPage::OnInitDialog();
             
    m_lboIntProcs.InitListBox();
	UpdateButtons();

    return TRUE;  // input focus will be set to the dialog's first control
}

void CIntProcPage::OnDblclkIntProcList()
{
    OnModify();
}

void CIntProcPage::OnAdd()
{
	CInterlinearProc* pintprc = NULL;
	int iProcID;
	CSelectProcDlg dlg( &iProcID ); // Have user select type of process to add
	if ( dlg.DoModal() == IDOK )
		{
		// Here put any other process besides Lookup
		if ( iProcID == CProcNames::eRearrange || iProcID == CProcNames::eGenerate )
			{
			CRearrangeProc* pintprcRearrange = new CRearrangeProc( m_pintprclst,
					iProcID == CProcNames::eGenerate ); // Make a new process
			if ( pintprcRearrange->bModalView_Properties() ) // If user sets it up and accepts it, keep it to add to list
				pintprc = pintprcRearrange;
			else // Else (user canceled the new process) delete the process
				delete pintprcRearrange;
			}
		else if ( iProcID == CProcNames::eGiven )
			{
			CGivenProc* pintprcGiven = new CGivenProc( m_pintprclst ); // Make a new process
			if ( pintprcGiven->bModalView_Properties() ) // If user sets it up and accepts it, keep it to add to list
				pintprc = pintprcGiven;
			else // Else (user canceled the new process) delete the process
				delete pintprcGiven;
			}
		else
			{
			CLookupProc* pintprcLook = new CLookupProc( m_pintprclst,
					iProcID == CProcNames::eParse ); // Make a new process
			if ( pintprcLook->bModalView_Properties() ) // If user sets it up and accepts it, keep it to add to list
				pintprc = pintprcLook;
			else // Else (user canceled the new process) delete the process
				delete pintprcLook;
				}
		if ( pintprc ) // If they made a process, add it to the list after the current one
			{
			CInterlinearProc* pintprcSel = m_lboIntProcs.pintprcSelected(); // Get selected
			CInterlinearProc* pintprcFirst = m_lboIntProcs.pintprclst()->pintprcFirst(); // Get first
			if( pintprcSel )
				{
				if ( pintprcSel == pintprcFirst && m_lboIntProcs.GetCount() > 1 ) // If first of multiple selected, insert before it
					m_pintprclst->InsertBefore( pintprcSel, pintprc ); 
				else
					m_pintprclst->InsertAfter( pintprcSel, pintprc ); // Else insert after
				}
			else
				m_pintprclst->Add( pintprc );	
			m_pintprclst->ptyp()->SetInterlinearMkrs(); // Reset interlinear settings of markers
			m_lboIntProcs.UpdateElements( pintprc ); // Update displayed list
			UpdateButtons();
			}
		}
}

void CIntProcPage::OnDelete()
{
#ifdef NOSTRINGRES
	Str8 sMessage = _("Delete interlinear process?"); // 1.5.0fd
	if ( AfxMessageBox( sMessage, MB_YESNO | MB_ICONQUESTION ) != IDYES ) // 1.5.0fd
#else
	CString sw;
	sw.LoadString( IDS_DELETE_INTERLIN_PROC ); // 1.5nb Put string into res as test
	if ( AfxMessageBox( sw, MB_YESNO | MB_ICONQUESTION ) != IDYES ) // 1.5.0fd
#endif
		return;

	CInterlinearProc* pintprc = m_lboIntProcs.pintprcSelected();
	ASSERT( pintprc );
	m_pintprclst->Delete( &pintprc );	
	m_pintprclst->ptyp()->SetInterlinearMkrs(); // Reset interlinear settings of markers
	m_lboIntProcs.UpdateElements( pintprc ); // Update displayed list
	UpdateButtons();
}

void CIntProcPage::OnModify()
{
	CInterlinearProc* pintprc = m_lboIntProcs.pintprcSelected();
	ASSERT( pintprc );
	pintprc->bModalView_Properties();
	m_pintprclst->ptyp()->SetInterlinearMkrs(); // Reset interlinear settings of markers
	m_lboIntProcs.Invalidate();
}

void CIntProcPage::UpdateButtons()
{
	BOOL bListHasItems = (!bListEmpty());
	
	m_btnModify.EnableWindow(bListHasItems);
	m_btnDelete.EnableWindow(bListHasItems);
	m_btnExpressSetup.EnableWindow(!bListHasItems);
}

void CIntProcPage::OnOK()
{
	if (!m_bWasActive) // don't do anything if this page was never viewed
		return;
	UpdateData(TRUE);

	m_iInterlinearSpacing = __max( m_iInterlinearSpacing, 1 );
	m_iInterlinearSpacing = __min( m_iInterlinearSpacing, 30 );
    m_pintprclst->ptyp()->SetInterlinCharWd( m_iInterlinearSpacing );

    CPropertyPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CSelectProcDlg dialog


CSelectProcDlg::CSelectProcDlg( int* piProcID )
	: CDialog(CSelectProcDlg::IDD, NULL)
{
	m_piProcID = piProcID;
	//{{AFX_DATA_INIT(CSelectProcDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSelectProcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectProcDlg)
	DDX_Control(pDX, IDC_SELECT_PROC_LIST, m_lboSelectProc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelectProcDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectProcDlg)
	ON_LBN_DBLCLK(IDC_SELECT_PROC_LIST, OnDblclkSelectProcList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelectProcDlg message handlers

BOOL CSelectProcDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Fill list box
	for ( int i = 0; i < CProcNames::eNumProcs; i++ )
		{
		Str8 s = CProcNames::sProcName[ i ]; // 1.4ah Change to not use string resource id in interlinear 
		m_lboSelectProc.AddString( swUTF16( s  ) ); // 1.4qux
		}
	
	m_lboSelectProc.SetCurSel(0); // hilite first process in list
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelectProcDlg::OnDblclkSelectProcList()
{
	OnOK(); // Select double clicked process
}

void CSelectProcDlg::OnOK()
{
	*m_piProcID = m_lboSelectProc.GetCurSel(); // Return via pointer the id of the selected process
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CParseDlg dialog

CParseDlg::CParseDlg( CLookupProc* pintprc )
	: CDialog(CParseDlg::IDD, NULL),
	m_cboToMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboFromMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL )
{
	m_pintprc = pintprc;
    ASSERT( m_pintprc );  // 1999-08-28 MRP
	CInterlinearProcList* pintprclst = pintprc->pintprclst();
	m_sForcedGlossEnd = pintprclst->cForcedGlossEndChar();
	m_sForcedGlossStart = pintprclst->cForcedGlossStartChar(); 
	m_sMorphBound = pintprclst->cParseMorphBoundChar();
	m_sMorphBreaks = pintprclst->sMorphBreakChars();
	if ( m_pintprc->bPreferPrefix() ) // Set radio button based on preference setting
		m_iAffixSetting = 0;
	else if ( m_pintprc->bPreferSuffix() )
		m_iAffixSetting = 1;
	else
		m_iAffixSetting = 2;

    // 1999-08-30 MRP: Change from word formula database to properties
    CWordFormulaSet* pwdfset = m_pintprc->pwdfset();
    if ( pwdfset )
        m_pwdfset = new CWordFormulaSet(*pwdfset);
    else
        m_pwdfset = NULL;
	m_bEnableWordFormulas = (pwdfset && pwdfset->bEnabled());
}

CParseDlg::~CParseDlg()
{
    delete m_pwdfset;
}

void CParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParseDlg)
	DDX_Control(pDX, IDC_CompRoot, m_chkCompRoot);
	DDX_Control(pDX, IDC_Adapt, m_chkAdapt);
	DDX_Control(pDX, IDC_InfixBefore, m_chkInfixBefore);
	DDX_Control(pDX, IDC_InfixMultiple, m_chkInfixMultiple);
	DDX_Control(pDX, IDC_LongerOverride, m_chkLongerOverride);
	DDX_Control(pDX, IDC_OverrideShowAll, m_chkOverrideShowAll);
	DDX_Control(pDX, IDC_InsertFail, m_chkInsertFail);
	DDX_Control(pDX, IDC_SHOW_WORD, m_chkShowWord);
	DDX_Control(pDX, IDC_SHOW_ROOT, m_chkShowRootGuess);
	DDX_Control(pDX, IDC_SHOW_FAIL_MARK, m_chkShowFailMark);
	DDX_Control(pDX, IDC_KEEPPUNC, m_chkKeepPunc);
	DDX_Control(pDX, IDC_KEEPCAP, m_chkKeepCap);
	DDX_Control(pDX, IDC_TO_MARKER, m_cboToMarker);
	DDX_Control(pDX, IDC_FROM_MARKER, m_cboFromMarker);
	DDX_Control(pDX, IDC_SH2_PARSE, m_chkSH2Parse);
	DDX_Control(pDX, IDC_PREFIX, m_btnConjAff);
	DDX_Control(pDX, IDC_PARSE, m_btnParseDB);
	DDX_Control(pDX, IDC_EDIT_MORPH_BREAKS, m_elcMorphBreaks);
	DDX_Control(pDX, IDC_EDIT_MORPH_BOUND, m_elcMorphBound);
	DDX_Control(pDX, IDC_EDIT_FC_GLOSS_ST, m_elcForcedGlossStart);
	DDX_Control(pDX, IDC_EDIT_FC_GLOSS_END, m_elcForcedGlossEnd);
//	DDX_Txt(pDX, IDC_EDIT_FC_GLOSS_ST, m_sForcedGlossStart);
//	DDX_Txt(pDX, IDC_EDIT_FC_GLOSS_END, m_sForcedGlossEnd);
//	DDX_Txt(pDX, IDC_EDIT_MORPH_BOUND, m_sMorphBound);
//	DDX_Txt(pDX, IDC_EDIT_MORPH_BREAKS, m_sMorphBreaks);
//	DDV_MaxChars(pDX, m_sForcedGlossEnd, 1);
//	DDV_MaxChars(pDX, m_sForcedGlossStart, 1);
//	DDV_MaxChars(pDX, m_sMorphBound, 1);
    DDX_Radio(pDX, IDC_PreferPrefix, m_iAffixSetting);
    DDX_Check(pDX, IDC_wdf_chkEnableWordFormulas, m_bEnableWordFormulas);
    DDX_Control(pDX, IDC_wdf_chkEnableWordFormulas, m_chkEnableWordFormulas);
	DDX_Control(pDX, IDC_wdf_btnFormulas, m_btnFormulas);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CParseDlg, CDialog)
	//{{AFX_MSG_MAP(CParseDlg)
	ON_CBN_SELCHANGE(IDC_TO_MARKER, OnSelchangeToMarker)
	ON_BN_CLICKED(IDC_PARSE, OnParse)
	ON_BN_CLICKED(IDC_PREFIX, OnPrefix)
	ON_BN_CLICKED(IDC_ROOT, OnRoot)
	ON_BN_CLICKED(IDC_SH2_PARSE, OnSh2Parse)
	ON_BN_CLICKED(IDC_wdf_chkEnableWordFormulas, OnEnableWordFormulas)
	ON_BN_CLICKED(IDC_wdf_btnFormulas, OnFormulas)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CParseDlg message handlers

BOOL CParseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
//	SetDlgItemTxt( IDC_EDIT_C_GLOSS_ST, m_sForcedGlossStart); // 1.4qpn
	m_elcForcedGlossStart.SetEditLngText( m_sForcedGlossStart ); // 1.4qpn
//	SetDlgItemTxt( IDC_EDIT_FC_GLOSS_END, m_sForcedGlossEnd); // 1.4qpn
	m_elcForcedGlossEnd.SetEditLngText( m_sForcedGlossEnd ); // 1.4qpn
//	SetDlgItemTxt( IDC_EDIT_MORPH_BOUND, m_sMorphBound); // 1.4qpn
	m_elcMorphBound.SetEditLngText( m_sMorphBound ); // 1.4qpn
//	SetDlgItemTxt( IDC_EDIT_MORPH_BREAKS, m_sMorphBreaks); // 1.4qpn
	m_elcMorphBreaks.SetEditLngText( m_sMorphBreaks ); // 1.4qpn

	CMarker* pmkr = m_pintprc->pmkrTo();
    ASSERT( pmkr );
	m_elcMorphBreaks.SetLangEnc( pmkr->plng() );
	m_elcMorphBound.SetLangEnc( pmkr->plng() ); // 1.4vxk 
	m_elcForcedGlossStart.SetLangEnc( pmkr->plng() ); // 1.4vxk 
	m_elcForcedGlossEnd.SetLangEnc( pmkr->plng() ); // 1.4vxk 

	m_cboToMarker.UpdateElements( m_pintprc->pmkrTo() );	
	m_cboFromMarker.UpdateElements( m_pintprc->pmkrFrom() );
	
	if ( m_pintprc->bKeepCap() )
		m_chkKeepCap.SetCheck( 1 );
	if ( m_pintprc->bKeepPunc() )
		m_chkKeepPunc.SetCheck( 1 );
	if ( m_pintprc->bShowFailMark() )
		m_chkShowFailMark.SetCheck( 1 );
	if ( m_pintprc->bInsertFail() )
		m_chkInsertFail.SetCheck( 1 );
	if ( m_pintprc->bShowWord() )
		m_chkShowWord.SetCheck( 1 );
	if ( m_pintprc->bAdapt() )
		m_chkAdapt.SetCheck( 1 );
	if ( m_pintprc->bShowRootGuess() )
		m_chkShowRootGuess.SetCheck( 1 );
	if ( m_pintprc->bInfixBefore() ) // 1.1hr Make longer override option
		m_chkInfixBefore.SetCheck( 1 );
	if ( m_pintprc->bInfixMultiple() )  // 1.5.1mb 
		m_chkInfixMultiple.SetCheck( 1 ); // 1.5.1mb 
	if ( m_pintprc->bLongerOverride() )
		m_chkLongerOverride.SetCheck( 1 );
	if ( m_pintprc->bOverrideShowAll() )
		m_chkOverrideShowAll.SetCheck( 1 );
	if ( m_pintprc->bCompRoot() )
		m_chkCompRoot.SetCheck( 1 );

	if ( m_pintprc->bSH2Parse() ) // If SH2 parse, display SH2 buttons
		{
		m_chkSH2Parse.SetCheck( 1 ); // Set it checked
		m_btnConjAff.ShowWindow( SW_SHOWNOACTIVATE ); // Display conjoined affixe button
		m_btnConjAff.EnableWindow(); // Display conjoined affixe button
		m_btnParseDB.ShowWindow( SW_SHOWNOACTIVATE ); // Display parse database button
		m_btnParseDB.EnableWindow(); // Display parse database button
		}
	else // Else hide SH2 buttons
		{
		m_chkSH2Parse.SetCheck( 0 ); // Set it unchecked
		m_btnConjAff.ShowWindow( SW_HIDE ); // Hide conjoined affixe button
		m_btnConjAff.EnableWindow(FALSE); // Disable so activation is not possible
		m_btnParseDB.ShowWindow( SW_HIDE ); // Hide parse database button
		m_btnParseDB.EnableWindow(FALSE); // Disable so activation is not possible
		}

    // 1999-08-30 MRP: Change from word formula database to properties
    m_btnFormulas.EnableWindow(m_bEnableWordFormulas);  // 1999-09-06 MRP

	return TRUE;  // return TRUE  unless you set the focus to a control
}

static void RemoveSpaces( Str8& s )
{
	int iPos = 0;
	while ( ( iPos = s.Find( ' ' ) ) > 0 )
		s = s.Left( iPos ) + s.Mid( iPos + 1 );
}

void CParseDlg::OnOK()
{
	if ( !UpdateData( TRUE ) )
		return;
	if ( m_cboFromMarker.pmkrSelected() == m_cboToMarker.pmkrSelected() ) // Don't let them select same marker
		{
		AfxMessageBox( _("The From and To markers cannot be the same.") );
		return;
		}
	m_sForcedGlossStart = m_elcForcedGlossStart.sGetEditLngText(); // 1.4qpn
	m_sForcedGlossEnd = m_elcForcedGlossEnd.sGetEditLngText(); // 1.4qpn
	m_sMorphBound = m_elcMorphBound.sGetEditLngText(); // 1.4qpn
	m_sMorphBreaks = m_elcMorphBreaks.sGetEditLngText(); // 1.4qpn

	m_pintprc->SetMarkers( m_cboFromMarker.pmkrSelected(),
		m_cboToMarker.pmkrSelected() );

	m_pintprc->SetPreferPrefix( FALSE );
	m_pintprc->SetPreferSuffix( FALSE );
	if ( m_iAffixSetting == 0 )
		m_pintprc->SetPreferPrefix( TRUE );
	else if ( m_iAffixSetting == 1 )
		m_pintprc->SetPreferSuffix( TRUE );

	m_pintprc->SetSH2Parse( m_chkSH2Parse.GetCheck() ); 
	if ( !m_chkSH2Parse.GetCheck() ) // If not SH2 parse, affixes are in lexicon, copy lexicon settings to affix tries
		{
		m_pintprc->ptri( PREF )->CopySettings( m_pintprc->ptri( ROOT ) ); // Copy settings to the other two tries 
		m_pintprc->ptri( SUFF )->CopySettings( m_pintprc->ptri( ROOT ) ); // Copy settings to the other two tries 
		m_pintprc->ptri( INF )->CopySettings( m_pintprc->ptri( ROOT ) ); // Copy settings to the other two tries 
		m_pintprc->ptri( LOOK )->ClearAll(); // Unload and clear settings of the parse database trie
		}
	
	RemoveSpaces( m_sForcedGlossStart );
	RemoveSpaces( m_sForcedGlossEnd );
	RemoveSpaces( m_sMorphBound );
	RemoveSpaces( m_sMorphBreaks );
	
	if ( !*m_sForcedGlossStart ) // Don't allow empty values
		m_sForcedGlossStart = "{";
	if ( !*m_sForcedGlossEnd )
		m_sForcedGlossEnd = "}";
	if ( !*m_sMorphBound )
		m_sMorphBound = "+";
	if ( !*m_sMorphBreaks )
		m_sMorphBreaks = " "; // Langs with no word break chars use space here

    m_pintprc->pintprclst()->SetForcedGlossStartChar( *m_sForcedGlossStart );
	m_pintprc->pintprclst()->SetForcedGlossEndChar( *m_sForcedGlossEnd );
	m_pintprc->pintprclst()->SetParseMorphBoundChar( *m_sMorphBound );
	m_pintprc->pintprclst()->SetMorphBreakChars( m_sMorphBreaks );

	m_pintprc->SetKeepCap( m_chkKeepCap.GetCheck() ); 
	m_pintprc->SetKeepPunc( m_chkKeepPunc.GetCheck() ); 
	m_pintprc->SetShowFailMark( m_chkShowFailMark.GetCheck() ); 
	m_pintprc->SetInsertFail( m_chkInsertFail.GetCheck() ); 
	m_pintprc->SetShowWord( m_chkShowWord.GetCheck() ); 
//	m_pintprc->SetAdapt( m_chkAdapt.GetCheck() );
	m_pintprc->SetAdapt( FALSE ); // Never an adapt process
	m_pintprc->SetShowRootGuess( m_chkShowRootGuess.GetCheck() );
	m_pintprc->SetInfixBefore( m_chkInfixBefore.GetCheck() ); 
	m_pintprc->SetInfixMultiple( m_chkInfixMultiple.GetCheck() );  // 1.5.1mb 
	m_pintprc->SetLongerOverride( m_chkLongerOverride.GetCheck() ); 
	m_pintprc->SetOverrideShowAll( m_chkOverrideShowAll.GetCheck() ); 
	m_pintprc->SetCompRoot( m_chkCompRoot.GetCheck() ); 
	
	if ( m_pintprc->bShowWord() || m_pintprc->bShowRootGuess() ) // If showing fail mark on word, use a short one
		m_pintprc->SetFailMark( "*" ); // !!! Temp until it is in interface
	else
		m_pintprc->SetFailMark( "***" ); // !!! Temp until it is in interface

    // 1999-08-30 MRP: Change from word formula database to properties
    if ( m_pwdfset )
        m_pwdfset->EnableWordFormulas(m_bEnableWordFormulas);
    m_pintprc->SetWordFormulas(m_pwdfset);

	CDialog::OnOK();
}

void CParseDlg::OnSelchangeToMarker()
{
    CMarker* pmkr = m_cboToMarker.pmkrSelected();
    ASSERT( pmkr );
	m_elcMorphBreaks.SetLangEnc( pmkr->plng() ); // 1.4vxk 
	m_elcMorphBound.SetLangEnc( pmkr->plng() ); // 1.4vxk 
	m_elcForcedGlossStart.SetLangEnc( pmkr->plng() ); // 1.4vxk 
	m_elcForcedGlossEnd.SetLangEnc( pmkr->plng() );	 // 1.4vxk 
}

void CParseDlg::OnRoot()
{
	m_pintprc->ptri( ROOT )->bModalView_Properties();
}

void CParseDlg::OnSh2Parse() // Click on the SH2 style parse check box
{
	if ( m_chkSH2Parse.GetCheck() ) // If button being checked, display SH2 buttons
		{
		m_chkSH2Parse.SetCheck( 1 ); // Set it checked
		m_btnConjAff.ShowWindow( SW_SHOWNOACTIVATE ); // Display conjoined affixe button
		m_btnConjAff.EnableWindow();
		m_btnParseDB.ShowWindow( SW_SHOWNOACTIVATE ); // Display parse database button
		m_btnParseDB.EnableWindow();
		}
	else // Else (button being cleared), hide SH2 buttons
		{
		m_chkSH2Parse.SetCheck( 0 ); // Set it unchecked
		m_btnConjAff.ShowWindow( SW_HIDE ); // Hide conjoined affixe button
		m_btnConjAff.EnableWindow(FALSE); // Disable so activation is not possible
		m_btnParseDB.ShowWindow( SW_HIDE ); // Hide parse database button
		m_btnParseDB.EnableWindow(FALSE); // Disable so activation is not possible
		}
}
 
void CParseDlg::OnPrefix()
{
	m_pintprc->ptri( PREF )->bModalView_Properties();
	m_pintprc->ptri( SUFF )->CopySettings( m_pintprc->ptri( PREF ) ); // Copy settings to the other two tries 
	m_pintprc->ptri( INF )->CopySettings( m_pintprc->ptri( PREF ) ); // Copy settings to the other two tries 
}

void CParseDlg::OnParse()
{
	m_pintprc->ptri( LOOK )->bModalView_Properties();
}

void CParseDlg::OnEnableWordFormulas()
{
	m_bEnableWordFormulas = m_chkEnableWordFormulas.GetCheck();
    m_btnFormulas.EnableWindow(m_bEnableWordFormulas);
    if ( m_bEnableWordFormulas && !m_pwdfset )
        CWordFormulaSet::s_EnableWordFormulas(m_pintprc, &m_pwdfset);
}

void CParseDlg::OnFormulas()
{
    ASSERT( m_pwdfset );
    m_pwdfset->bModify();
}

/////////////////////////////////////////////////////////////////////////////
// CLookupDlg dialog

CLookupDlg::CLookupDlg( CLookupProc* pintprc )
	: CDialog(CLookupDlg::IDD, NULL),
	m_cboToMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboFromMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL )
{
	m_pintprc = pintprc;
	//{{AFX_DATA_INIT(CLookupDlg)
	//}}AFX_DATA_INIT
}

void CLookupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLookupDlg)
	DDX_Control(pDX, IDC_Adapt, m_chkAdapt);
	DDX_Control(pDX, IDC_InsertFail, m_chkInsertFail);
	DDX_Control(pDX, IDC_EDIT_GLOSS_SEPARATOR, m_elcGlossSeparator);
	DDX_Control(pDX, IDC_STOP_AT_SEMICOLON, m_chkStopAtSemicolon);
	DDX_Control(pDX, IDC_KEEPPUNC, m_chkKeepPunc);
	DDX_Control(pDX, IDC_KEEPCAP, m_chkKeepCap);
	DDX_Control(pDX, IDC_APPLYCC, m_chkApplyCC);
	DDX_Control(pDX, IDC_SHOW_WORD, m_chkShowWord);
	DDX_Control(pDX, IDC_SHOW_FAIL_MARK, m_chkShowFailMark);
	DDX_Control(pDX, IDC_LBL_CCTABLE, m_lblCCTable);
	DDX_Control(pDX, IDC_TO_MARKER, m_cboToMarker);
	DDX_Control(pDX, IDC_FROM_MARKER, m_cboFromMarker);
//	DDX_Txt(pDX, IDC_EDIT_GLOSS_SEPARATOR, m_sGlossSeparator);
//	DDV_MaxChars(pDX, m_sGlossSeparator, 1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLookupDlg, CDialog)
	//{{AFX_MSG_MAP(CLookupDlg)
	ON_BN_CLICKED(IDC_LOOKUP, OnLookup)
	ON_BN_CLICKED(IDC_BROWSE_CCTABLE, OnBrowseCctable)
	ON_BN_CLICKED(IDC_APPLYCC, OnApplycc)
	ON_BN_CLICKED(IDC_SHOW_WORD, OnShowWord)
	ON_BN_CLICKED(IDC_SHOW_FAIL_MARK, OnShowFailMark)
	ON_CBN_SELCHANGE(IDC_TO_MARKER, OnSelchangeToMarker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLookupDlg message handlers

void CLookupDlg::SetCCTWindowTextNone() // Set the CC table text to None
{
	Str8 sNone;
	sNone = _("None");
    m_lblCCTable.SetWindowText(  swUTF16( sNone ) ); // 1.4qpy
}

BOOL CLookupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_sGlossSeparator = m_pintprc->pszGlossSeparator(); // 1.4qnj Fix bug of not showing Gloss separator in Lookup dialog
//	SetDlgItemTxt( IDC_EDIT_GLOSS_SEPARATOR, m_sGlossSeparator); // 1.4qpn
	m_elcGlossSeparator.SetEditLngText( m_sGlossSeparator ); // 1.4qpn
	
	m_cboToMarker.UpdateElements( m_pintprc->pmkrTo() );	
	m_cboFromMarker.UpdateElements( m_pintprc->pmkrFrom() );
	m_elcGlossSeparator.SetLangEnc( m_cboToMarker.pmkrSelected()->plng() ); // 1.4vxk 

	if ( m_pintprc->bKeepCap() )
		m_chkKeepCap.SetCheck( 1 );
	if ( m_pintprc->bKeepPunc() )
		m_chkKeepPunc.SetCheck( 1 );
	if ( m_pintprc->bStopAtSemicolon() )
		m_chkStopAtSemicolon.SetCheck( 1 );
	if ( m_pintprc->bShowFailMark() )
		m_chkShowFailMark.SetCheck( 1 );
	if ( m_pintprc->bInsertFail() )
		m_chkInsertFail.SetCheck( 1 );
	if ( m_pintprc->bInsertFail() )
		m_chkInsertFail.SetCheck( 1 );
	if ( m_pintprc->bShowWord() )
		m_chkShowWord.SetCheck( 1 );
	if ( m_pintprc->bApplyCC() )
		m_chkApplyCC.SetCheck( 1 );
	if ( m_pintprc->bAdapt() )
		m_chkAdapt.SetCheck( 1 );

#if UseCct
	m_sCCTable = m_pintprc->pszCCT(); // Get name of CC table
	if ( !bCCTableExists() ) // If table doesn't exist, clear its name, and don't try to use it
		{
		m_sCCTable = ""; // if not valid file, trash it
		}
	if ( !m_sCCTable.IsEmpty() )
		m_lblCCTable.SetWindowText(  swUTF16( sGetFileName(m_sCCTable) ) ); // 1.4qpy
	else
		SetCCTWindowTextNone(); // Set CC Table text to None
#endif

	UpdateData( FALSE ); // Put values into controls
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLookupDlg::OnOK()
{
	if ( !UpdateData( TRUE ) )
		return;
	if ( m_cboFromMarker.pmkrSelected() == m_cboToMarker.pmkrSelected() ) // Don't let them select same marker
		{
		AfxMessageBox( _("The From and To markers cannot be the same.") );
		return;
		}
#if UseCct
	if ( m_sCCTable.GetLength()
			&& !m_pintprc->bLoadCCFromFile( m_sCCTable ) ) // If CC table won't load, refuse
		return;
#endif
	m_sGlossSeparator = m_elcGlossSeparator.sGetEditLngText(); // 1.4qpn

	m_pintprc->SetMarkers( m_cboFromMarker.pmkrSelected(),
		m_cboToMarker.pmkrSelected() );
	m_pintprc->SetKeepCap( m_chkKeepCap.GetCheck() ); 
	m_pintprc->SetKeepPunc( m_chkKeepPunc.GetCheck() ); 
	m_pintprc->SetStopAtSemicolon( m_chkStopAtSemicolon.GetCheck() ); 
	m_pintprc->SetShowFailMark( m_chkShowFailMark.GetCheck() ); 
	m_pintprc->SetInsertFail( m_chkInsertFail.GetCheck() ); 
	m_pintprc->SetShowWord( m_chkShowWord.GetCheck() ); 
	m_pintprc->SetApplyCC( m_chkApplyCC.GetCheck() );
	m_pintprc->SetAdapt( m_chkAdapt.GetCheck() );
	m_pintprc->SetGlossSeparator( m_sGlossSeparator );
	
	if ( m_pintprc->bShowWord() ) // If showing fail mark on word, use a short one
		m_pintprc->SetFailMark( "*" ); // !!! Temp until it is in interface
	else
		m_pintprc->SetFailMark( "***" ); // !!! Temp until it is in interface

	CDialog::OnOK();
}

void CLookupDlg::OnLookup()
{
	m_pintprc->ptri( LOOK )->bModalView_Properties();
}

void CLookupDlg::OnBrowseCctable()
{
	if ( bGetCCTable() ) // Get a CC table
		m_chkApplyCC.SetCheck( 1 ); // If it is OK, turn on use of it
}

void CLookupDlg::OnApplycc()
{
	if ( m_chkApplyCC.GetCheck() ) // If they are setting it, get a table
		{
		if ( !bGetCCTable() ) // Get a CC table
			m_chkApplyCC.SetCheck( 0 ); // If table is not OK, don't turn on check
		}
	else // Else, they are clearing it, clear the table
		{
		m_sCCTable.Empty(); // Clear table name
        SetCCTWindowTextNone();
        }				
}

void CLookupDlg::OnShowWord()
{
	// TODO: Add your control notification handler code here
	
}

void CLookupDlg::OnShowFailMark()
{
	// TODO: Add your control notification handler code here
	
}

BOOL CLookupDlg::bGetCCTable( BOOL bOnlyIfNecessary )
{
	if ( bOnlyIfNecessary && bCCTableExists() )
		return TRUE;

	long lFlags = (OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);
	Str8 sFileFilter = _("Change Table"); // 1.5.0fz 
	sFileFilter += " (*.cct)|*.cct||"; // 1.5.0fz 
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlgCCTable(TRUE, swUTF16( "cct" ), swUTF16( sGetFileName(m_sCCTable) ), lFlags, swUTF16( sFileFilter ) ); // 1.4qrd // 1.5.8h 
		Str8 sInitialDir = sGetDirPath(m_sCCTable);
		dlgCCTable.m_ofn.lpstrInitialDir =  swUTF16( sInitialDir ); // 1.4qra
		dlgCCTable.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlgCCTable ) != IDOK )
			return FALSE;
		sPath = sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sCCTable) ) // 1.5.8h 
			return FALSE; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return FALSE; // 1.4vze 

//	m_sCCTable =  sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
	m_sCCTable =  sPath; // 1.4qra // 1.5.8h 
	m_lblCCTable.SetWindowText(  swUTF16( sGetFileName(m_sCCTable) ) ); // Clear display of name // 1.4qpy
	return TRUE;
}

BOOL CLookupDlg::bCCTableExists()
{
	if ( m_sCCTable.GetLength() )
		{
		if ( bFileExists( m_sCCTable ) ) // does cc table exist
			return TRUE;
		}
	return FALSE;
}

void CLookupDlg::OnSelchangeToMarker() 
{
	m_elcGlossSeparator.SetLangEnc( m_cboToMarker.pmkrSelected()->plng() ); // 1.4vxk 
}

/////////////////////////////////////////////////////////////////////////////
// CTrieDlg dialog
                                                                                    

CTrieDlg::CTrieDlg( CDbTrie* ptri )
	: CDialog(CTrieDlg::IDD, NULL),
	m_mkrsetEmpty( Shw_plngset() ),
	m_lboMkrsAvailable( &m_mkrsetEmpty, &m_pmkrEmpty ),
	m_cboMkrOutput( &m_mkrsetEmpty, FALSE, NULL ),
	m_lboDbsToSearch( ptri->pdrflst() )
{
	m_ptri = ptri;
	//{{AFX_DATA_INIT(CTrieDlg)
	//}}AFX_DATA_INIT
}

void CTrieDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrieDlg)
	DDX_Control(pDX, IDC_TRIE_SHOWPATH, m_chkShowPath);
	DDX_Control(pDX, IDC_TRIE_MKROUTPUT, m_cboMkrOutput);
	DDX_Control(pDX, IDC_TRIE_LSTMKRSTOFIND, m_lboMkrsToFind);
	DDX_Control(pDX, IDC_TRIE_LSTMKRSAVAILABLE, m_lboMkrsAvailable);
	DDX_Control(pDX, IDC_TRIE_LSTDBSTOSEARCH, m_lboDbsToSearch);
	DDX_Control(pDX, IDC_TRIE_LSTDBSAVAILABLE, m_lboDbsAvailable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrieDlg, CDialog)
	//{{AFX_MSG_MAP(CTrieDlg)
	ON_LBN_DBLCLK(IDC_TRIE_LSTDBSAVAILABLE, OnDblclkTrieLstdbsavailable)
	ON_LBN_DBLCLK(IDC_TRIE_LSTMKRSAVAILABLE, OnDblclkTrieLstmkrsavailable)
	ON_BN_CLICKED(IDC_TRIE_MOVETO, OnTrieMoveto)
	ON_BN_CLICKED(IDC_TRIE_REMOVE, OnTrieRemove)
	ON_BN_CLICKED(IDC_TRIE_MKRMOVETO, OnTrieMkrmoveto)
	ON_BN_CLICKED(IDC_TRIE_MKRREMOVE, OnTrieMkrremove)
	ON_BN_CLICKED(IDC_TRIE_SHOWPATH, OnTrieShowpath)
	ON_LBN_DBLCLK(IDC_TRIE_LSTMKRSTOFIND, OnDblclkTrieLstmkrstofind)
	ON_LBN_DBLCLK(IDC_TRIE_LSTDBSTOSEARCH, OnDblclkTrieLstdbstosearch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTrieDlg message handlers

BOOL CTrieDlg::OnInitDialog()
{
	m_ptri->MakeRefs(); // Set marker pointers from names

	CDialog::OnInitDialog();
			// Clear text of fake label used to hide the unused marker    
    CStatic* pstc = ( CStatic* )GetDlgItem( IDC_SEARCHPATH_FIELDTOSEARCH ); // Clear text from fake empty box
	ASSERT( pstc );
	pstc->SetWindowText(  swUTF16( "" ) ); // 1.4qpy
			// Load controls
	m_lboDbsAvailable.LoadDbsAvailable(); // Load available databases
    m_lboDbsToSearch.InitListBox(); // Load databases to search
    		// Load all the marker boxes
	CDatabaseType* ptyp = m_ptri->ptypDrfType(); // Get database type (getting it this way instead of from the first pdrf avoids loading the database if not loaded)
	CMarkerSet* pmkrset = ptyp? ptyp->pmkrset() : &m_mkrsetEmpty; // Get database marker set or empty marker set
			// Load markers available list box
	CMarker* pmkr = pmkrset->pmkrFirst(); // Temp marker pointer
    m_lboMkrsAvailable.ChangeSet( pmkrset, pmkr ); // (empty marker set and null marker are OK)
			// Load markers to find list box
    CMarkerRefList* pmrflst = m_ptri->pmrflst();
    for ( CMarkerRef* pmrf = pmrflst->pmrfFirst(); pmrf; pmrf = pmrflst->pmrfNext( pmrf ) ) // Load markers to find box
    	{
    	pmkr = pmrf->pmkr();
    	int idx = m_lboMkrsToFind.AddString( swUTF16( pmkr->sName()  ) ); // 1.4qux
    	m_lboMkrsToFind.SetItemDataPtr( idx, (void*)pmkr );
    	}
    m_lboMkrsToFind.SetCurSel( 0 ); // Set select to first (empty list is OK)
		    // Load output marker combo box
    pmkr = m_ptri->pmkrOut(); // Get out marker
    if ( !pmkr ) // If no out marker, take the first of the marker set (NULL if marker set is empty)
    	pmkr = pmkrset->pmkrFirst();
    m_cboMkrOutput.ChangeSet( pmkrset, pmkr, NULL ); // Load combo box and set select to output marker (empty marker set and null marker are OK)

    m_chkShowPath.SetCheck( Shw_pProject()->bShowWholePath() ); // Set whole path check box
    		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTrieDlg::OnDblclkTrieLstdbsavailable()
{
	OnTrieMoveto();
}

void CTrieDlg::OnDblclkTrieLstdbstosearch()
{
	OnTrieRemove();
}

void CTrieDlg::OnDblclkTrieLstmkrsavailable()
{
	OnTrieMkrmoveto();
}

void CTrieDlg::OnDblclkTrieLstmkrstofind()
{
	OnTrieMkrremove();
}

void CTrieDlg::OnTrieMoveto()
{
	int	idxAvailable = m_lboDbsAvailable.GetCurSel();
	if (idxAvailable == LB_ERR)
		return;
			// Get currently selected database to add, and put it in the search path
	CShwDoc* pdocAdd = (CShwDoc*)m_lboDbsAvailable.GetItemDataPtr(idxAvailable);
	if ( m_lboDbsToSearch.GetCount() >= 1 ) // If list not empty, If not same type as those already in, refuse to insert it
	    {
	    CShwDoc* pdocOld = m_lboDbsToSearch.pdrfSelected()->pdoc(); // Get document type of selected doc (may be null if file cannot be opened)
	    if ( pdocOld )
	    	{
			if ( pdocAdd->ptyp() != pdocOld->ptyp() ) 
				{
				AfxMessageBox( _("Cannot insert database because all databases must be of the same type.") );
				return;
				}
			}	
		}	
	int idx = m_lboDbsToSearch.InsertPath( -1,  sUTF8( pdocAdd->GetPathName() ), "" ); // insert item into listbox // 1.4quy Upgrade GetPathName for Unicode build
	m_lboDbsToSearch.SetCurSel(idx);	// Make that the current selection.
    		// If list was empty, get the marker set of the new database and show it in the marker boxes
	if ( m_lboDbsToSearch.GetCount() == 1 ) // If list was empty, intitialize marker boxes
		{
		CMarkerSet* pmkrset = m_lboDbsToSearch.pdrfSelected()->pdoc()->ptyp()->pmkrset(); // pdrf has to exist because we just added it and selected it. pdoc has to exist because these are only open databases
	    m_lboMkrsAvailable.ChangeSet( pmkrset, pmkrset->pmkrFirst() ); // (empty marker set and null marker are OK)
	    m_cboMkrOutput.ChangeSet( pmkrset, pmkrset->pmkrFirst(), NULL ); // Load combo box and set select to output marker (empty marker set and null marker are OK)
	    }

    m_lboDbsAvailable.SetFocus(); // Set focus back to list box so they can select another db
}

void CTrieDlg::OnTrieRemove()
{
	int	idx = m_lboDbsToSearch.GetCurSel();
	if (idx == LB_ERR)
		return;

	m_lboDbsToSearch.DeleteString(idx);
	idx = (idx < m_lboDbsToSearch.GetCount()) ? idx : idx-1;
	m_lboDbsToSearch.SetCurSel(idx);
    
    if ( !m_lboDbsToSearch.GetCount() ) // If list became empty, remove markers from display
    	{
    	m_lboMkrsAvailable.ResetContent();
    	m_lboMkrsToFind.ResetContent();
    	m_cboMkrOutput.ResetContent();
    	}	
    
	m_lboDbsToSearch.SetFocus();
}

void CTrieDlg::OnTrieMkrmoveto()
{
	int	idxAvailable = m_lboMkrsAvailable.GetCurSel();
	if (idxAvailable == LB_ERR)
		return;
	// get currently selected marker to add
	CMarker* pmkr = (CMarker*)m_lboMkrsAvailable.GetItemDataPtr(idxAvailable);
	// Stick it in the search path
	int idx = m_lboMkrsToFind.AddString( swUTF16( pmkr->sName()  ) ); // 1.4qux Upgrade AddString for Unicode build
	m_lboMkrsToFind.SetItemDataPtr(idx, (void*)pmkr);
	m_lboMkrsToFind.SetCurSel(idx);	// Make that the current selection.

    m_lboMkrsAvailable.SetFocus(); // set focus back to list box so they can select another db
}

void CTrieDlg::OnTrieMkrremove()
{
	int	idx = m_lboMkrsToFind.GetCurSel();
	if (idx == LB_ERR)
		return;

	m_lboMkrsToFind.DeleteString(idx);
	idx = (idx < m_lboMkrsToFind.GetCount()) ? idx : idx-1;
	m_lboMkrsToFind.SetCurSel(idx);

	m_lboMkrsToFind.SetFocus();
}

void CTrieDlg::OnTrieShowpath()
{
	Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
	m_lboDbsToSearch.Invalidate(); // force redraw
	m_lboDbsAvailable.Invalidate();
}

void CTrieDlg::OnOK()
{
			// Clear everything from trie
	m_ptri->Unload();
    m_ptri->pdrflst()->DeleteAll();
    m_ptri->pmrflst()->DeleteAll();
	m_ptri->SetMkrOut( "" );

	if ( m_lboDbsToSearch.GetCount() ) // If there are databases, load them into the trie
		{
		CDatabaseRef* pdrf;
		int idx = 0;
		for (; idx < m_lboDbsToSearch.GetCount(); idx++)
			{
			pdrf = (CDatabaseRef*)m_lboDbsToSearch.GetItemDataPtr(idx);
			m_ptri->pdrflst()->Add( pdrf->sDatabasePath(), pdrf->sKey() );
			}     
			
    	m_ptri->SetDrfType( m_ptri->pdrflst()->pdrfFirst()->pdoc()->ptyp()->sName() ); // Set the database type
    	
		CMarker* pmkr;
		for ( idx = 0; idx < m_lboMkrsToFind.GetCount(); idx++) // Update marker list
			{
			pmkr = (CMarker*)m_lboMkrsToFind.GetItemDataPtr(idx);
			m_ptri->pmrflst()->AddMarkerRef( pmkr, pmkr->sName() );
			}

		m_ptri->SetMkrOut( m_cboMkrOutput.pmkrSelected()->sName() ); // Set output marker
		
		m_ptri->MakeRefs(); // Set all marker pointers
		}
	else // If no drfs, clear type
		m_ptri->SetDrfType( "" );

 	CDialog::OnOK();
}

void CIntProcPage::OnExpressSetup()
{
	Str8 sMkrText;
	Str8 sMkrMB;
	Str8 sMkrGloss;
	Str8 sMkrPS;
	Str8 sMkrLexGloss;
	Str8 sMkrLexPS;
	Str8 sMkrAlt;
	Str8 sMkrUnd;
	CDatabaseRefList drflst;	
	
	// Get the interlinear text database type and marker set
	CDatabaseType* ptypText = m_pintprclst->ptyp();
	CMarkerSet* pmkrsetText = ptypText->pmkrset();
	// Ask for the markers
	CExpressTextDlg dlg; // (); // &asMkrs ); // 1.4yg Fix U bug of Quick Setup giving blank markers
	if ( dlg.DoModal() == IDOK )
		{
		sMkrText = dlg.m_sText; // 1.4yg 
		sMkrMB = dlg.m_sMB; // 1.4yg 
		sMkrGloss = dlg.m_sGloss; // 1.4yg 
		sMkrPS = dlg.m_sPS; // 1.4yg 
		CExpressLexiconDlg dlg( &drflst ); // 1.4yg 
		if ( dlg.DoModal() == IDOK )
			{
			sMkrLexGloss = dlg.m_sGloss; // 1.4yg 
			sMkrLexPS = dlg.m_sPS; // 1.4yg 
			sMkrAlt = dlg.m_sAlt; // 1.4yg 
			sMkrUnd = dlg.m_sUnd; // 1.4yg 
			// Do the actual setup
					// Make the text markers
			CMarker* pmkrText = pmkrsetText->pmkrSearch_AddIfNew( sMkrText, pmkrsetText->pmkrRecord(), "Text" );
			CMarker* pmkrMB = pmkrsetText->pmkrSearch_AddIfNew( sMkrMB, pmkrText, "Morpheme Breaks" );
			CMarker* pmkrGloss = pmkrsetText->pmkrSearch_AddIfNew( sMkrGloss, pmkrText, "Gloss" );
			CMarker* pmkrPS = pmkrsetText->pmkrSearch_AddIfNew( sMkrPS, pmkrText, "Part of Speech" );
            		// Make the processes
			CLookupProc* pintprc1 = new CLookupProc( m_pintprclst, TRUE ); // Make a parse process
            pintprc1->SetMarkers( pmkrText, pmkrMB );
			m_pintprclst->Add( pintprc1 );	
			CLookupProc* pintprc2 = new CLookupProc( m_pintprclst, FALSE ); // Make a parse process
            pintprc2->SetMarkers( pmkrMB, pmkrGloss );
			m_pintprclst->Add( pintprc2 );	
			CLookupProc* pintprc3 = new CLookupProc( m_pintprclst, FALSE ); // Make a parse process
            pintprc3->SetMarkers( pmkrMB, pmkrPS );
			m_pintprclst->Add( pintprc3 );	
            		// Load the tries
			if ( !drflst.bIsEmpty() ) // If there are databases, load them into the trie
				{
				for ( CDatabaseRef* pdrf = drflst.pdrfFirst(); pdrf; pdrf = drflst.pdrfNext( pdrf ) )
					{
					pintprc1->ptri( ROOT )->pdrflst()->Add( pdrf->sDatabasePath(), pdrf->sKey() );
					pintprc2->ptri( LOOK )->pdrflst()->Add( pdrf->sDatabasePath(), pdrf->sKey() );
					pintprc3->ptri( LOOK )->pdrflst()->Add( pdrf->sDatabasePath(), pdrf->sKey() );
					}     
				CDatabaseType* ptypLex = pintprc1->ptri( ROOT )->pdrflst()->pdrfFirst()->pdoc()->ptyp();	
		    	pintprc1->ptri( ROOT )->SetDrfType( ptypLex->sName() ); // Set the database type
		    	pintprc2->ptri( LOOK )->SetDrfType( ptypLex->sName() ); // Set the database type
		    	pintprc3->ptri( LOOK )->SetDrfType( ptypLex->sName() ); // Set the database type
						// Make the lexicon markers
				CMarkerSet* pmkrsetLex = ptypLex->pmkrset();
				CMarker* pmkrRec = ptypLex->pmkrRecord();
				CMarker* pmkrLexGloss = pmkrsetLex->pmkrSearch_AddIfNew( sMkrLexGloss );
				CMarker* pmkrLexPS = pmkrsetLex->pmkrSearch_AddIfNew( sMkrLexPS );
				CMarker* pmkrAlt = pmkrsetLex->pmkrSearch_AddIfNew( sMkrAlt );
				CMarker* pmkrUnd = pmkrsetLex->pmkrSearch_AddIfNew( sMkrUnd );
						// Load the search markers into the tries			
		    	pintprc1->ptri( ROOT )->pmrflst()->AddMarkerRef( pmkrRec, pmkrRec->sName() ); // Parse root and affixes with rec mark and alt
		    	pintprc1->ptri( ROOT )->pmrflst()->AddMarkerRef( pmkrAlt, pmkrAlt->sName() );
		    	pintprc2->ptri( LOOK )->pmrflst()->AddMarkerRef( pmkrRec, pmkrRec->sName() ); // Lookups with rec mark
		    	pintprc3->ptri( LOOK )->pmrflst()->AddMarkerRef( pmkrRec, pmkrRec->sName() );
                		// Load the output markers into the tries
		    	pintprc1->ptri( ROOT )->SetMkrOut( pmkrUnd->sName() ); // Parse root and affixes output underlying form
		    	pintprc2->ptri( LOOK )->SetMkrOut( pmkrLexGloss->sName() ); // Gloss line output gloss
		    	pintprc3->ptri( LOOK )->SetMkrOut( pmkrLexPS->sName() ); // PS line output PS
                		// Copy parse settings to affix tries
				pintprc1->ptri( PREF )->CopySettings( pintprc1->ptri( ROOT ) ); // Copy settings to the other two tries 
				pintprc1->ptri( SUFF )->CopySettings( pintprc1->ptri( ROOT ) ); // Copy settings to the other two tries 
				pintprc1->ptri( INF )->CopySettings( pintprc1->ptri( ROOT ) ); // Copy settings to the other two tries 
		                		// Make refs in all tries
		    	pintprc1->ptri( ROOT )->MakeRefs(); // Set all marker pointers
		    	pintprc1->ptri( PREF )->MakeRefs();
		    	pintprc1->ptri( SUFF )->MakeRefs();
		    	pintprc1->ptri( INF )->MakeRefs();
		    	pintprc2->ptri( LOOK )->MakeRefs();
		    	pintprc3->ptri( LOOK )->MakeRefs();
				}

			m_pintprclst->ptyp()->SetInterlinearMkrs(); // Set interlinear settings of markers
			m_lboIntProcs.UpdateElements( pintprc1 ); // Update displayed list
			UpdateButtons();
			}	
		}
}

/////////////////////////////////////////////////////////////////////////////
// CExpressTextDlg dialog


CExpressTextDlg::CExpressTextDlg() 
	: CDialog(CExpressTextDlg::IDD, NULL)
{
//	m_pasMkrs = pasMkrs; // 1.4yg 
	//{{AFX_DATA_INIT(CExpressTextDlg)
	m_sText = "tx";
	m_sMB = "mb";
	m_sGloss = "ge";
	m_sPS = "ps";
	//}}AFX_DATA_INIT
}

void CExpressTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressTextDlg)
//	DDX_Txt(pDX, IDC_GLOSS, m_sGloss);
//	DDX_Txt(pDX, IDC_MB, m_sMB);
//	DDX_Txt(pDX, IDC_PS, m_sPS);
//	DDX_Txt(pDX, IDC_TEXT, m_sText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExpressTextDlg, CDialog)
	//{{AFX_MSG_MAP(CExpressTextDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExpressTextDlg message handlers

BOOL CExpressTextDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_GLOSS, swUTF16( m_sGloss ) ); // 1.4qpv
	SetDlgItemText( IDC_MB, swUTF16( m_sMB ) ); // 1.4qpv
	SetDlgItemText( IDC_PS, swUTF16( m_sPS ) ); // 1.4qpv
	SetDlgItemText( IDC_TEXT, swUTF16( m_sText ) ); // 1.4qpv

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExpressTextDlg::OnOK()
{
//	GetDlgItemText( IDC_GLOSS, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_GLOSS, sDlgItem ); // 1.4qpw
	m_sGloss = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_MB, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_MB, sDlgItem ); // 1.4qpw
	m_sMB = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PS, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PS, sDlgItem ); // 1.4qpw
	m_sPS = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_TEXT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_TEXT, sDlgItem ); // 1.4qpw
	m_sText = sUTF8( sDlgItem ); // 1.4qpw

	if ( m_sText.IsEmpty() ) // Insist that the markers not be empty
		m_sText = "tx"; // 5.99c Change defaults to more common 2 letter codes
	if ( m_sMB.IsEmpty() )
		m_sMB = "mb";
	if ( m_sGloss.IsEmpty() )
		m_sGloss = "ge"; // 1.4yh Change default gloss marker of Quick Setup to ge
	if ( m_sPS.IsEmpty() )
		m_sPS = "ps";
//	UpdateData(TRUE);
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CExpressLexiconDlg dialog

CExpressLexiconDlg::CExpressLexiconDlg( CDatabaseRefList* pdrflst )
	: CDialog(CExpressLexiconDlg::IDD, NULL),
	m_lboDbsToSearch( pdrflst )
{
//	m_pasMkrs = pasMkrs; // 1.4yg 
	m_pdrflst = pdrflst;
	//{{AFX_DATA_INIT(CExpressLexiconDlg)
	m_sGloss = "ge";
	m_sPS = "ps";
	m_sAlt = "a";
	m_sUnd = "u";
	//}}AFX_DATA_INIT
}

void CExpressLexiconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressLexiconDlg)
	DDX_Control(pDX, IDC_TRIE_SHOWPATH, m_chkShowPath);
	DDX_Control(pDX, IDC_LSTDBSTOSEARCH, m_lboDbsToSearch);
	DDX_Control(pDX, IDC_LSTDBSAVAILABLE, m_lboDbsAvailable);
//	DDX_Txt(pDX, IDC_ALT, m_sAlt);
//	DDX_Txt(pDX, IDC_GLOSS, m_sGloss);
//	DDX_Txt(pDX, IDC_PS, m_sPS);
//	DDX_Txt(pDX, IDC_UND, m_sUnd);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExpressLexiconDlg, CDialog)
	//{{AFX_MSG_MAP(CExpressLexiconDlg)
	ON_BN_CLICKED(IDC_MOVETO, OnMoveto)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_DBLCLK(IDC_LSTDBSAVAILABLE, OnDblclkLstdbsavailable)
	ON_LBN_DBLCLK(IDC_LSTDBSTOSEARCH, OnDblclkLstdbstosearch)
	ON_BN_CLICKED(IDC_TRIE_SHOWPATH, OnShowpath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CExpressLexiconDlg message handlers

BOOL CExpressLexiconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText( IDC_ALT, swUTF16( m_sAlt ) ); // 1.4qpv
	SetDlgItemText( IDC_GLOSS, swUTF16( m_sGloss ) ); // 1.4qpv
	SetDlgItemText( IDC_PS, swUTF16( m_sPS ) ); // 1.4qpv
	SetDlgItemText( IDC_UND, swUTF16( m_sUnd ) ); // 1.4qpv

	// Clear text of fake label used to hide the unused marker    
    CStatic* pstc = ( CStatic* )GetDlgItem( IDC_SEARCHPATH_FIELDTOSEARCH ); // Clear text from fake empty box
	ASSERT( pstc );
	pstc->SetWindowText(  swUTF16( "" ) ); // 1.4qpy
			// Load controls
	m_lboDbsAvailable.LoadDbsAvailable(); // Load available databases
    m_lboDbsToSearch.InitListBox(); // Init databases to search
    m_chkShowPath.SetCheck( Shw_pProject()->bShowWholePath() ); // Set whole path check box
    		
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CExpressLexiconDlg::OnMoveto()
{
	int	idxAvailable = m_lboDbsAvailable.GetCurSel();
	if (idxAvailable == LB_ERR)
		return;
			// Get currently selected database to add, and put it in the search path
	CShwDoc* pdocAdd = (CShwDoc*)m_lboDbsAvailable.GetItemDataPtr(idxAvailable);
	if ( m_lboDbsToSearch.GetCount() >= 1 ) // If list not empty, If not same type as those already in, refuse to insert it
	    {
	    CShwDoc* pdocOld = m_lboDbsToSearch.pdrfSelected()->pdoc(); // Get document type of selected doc (may be null if file cannot be opened)
	    if ( pdocOld )
	    	{
			if ( pdocAdd->ptyp() != pdocOld->ptyp() ) 
				{
				AfxMessageBox( _("Cannot insert database because all databases must be of the same type.") );
				return;
				}
			}	
		}	
	int idx = m_lboDbsToSearch.InsertPath( -1,  sUTF8( pdocAdd->GetPathName() ), "" ); // insert item into listbox // 1.4quy
	m_lboDbsToSearch.SetCurSel(idx);	// Make that the current selection.
    m_lboDbsAvailable.SetFocus(); // Set focus back to list box so they can select another db
}

void CExpressLexiconDlg::OnRemove()
{
	int	idx = m_lboDbsToSearch.GetCurSel();
	if (idx == LB_ERR)
		return;
	m_lboDbsToSearch.DeleteString(idx);
	idx = (idx < m_lboDbsToSearch.GetCount()) ? idx : idx-1;
	m_lboDbsToSearch.SetCurSel(idx);
	m_lboDbsToSearch.SetFocus();
}


void CExpressLexiconDlg::OnDblclkLstdbsavailable()
{
	OnMoveto();
}

void CExpressLexiconDlg::OnDblclkLstdbstosearch()
{
	OnRemove();
}

void CExpressLexiconDlg::OnShowpath()
{
	Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
	m_lboDbsToSearch.Invalidate(); // force redraw
	m_lboDbsAvailable.Invalidate();
}

void CExpressLexiconDlg::OnOK()
{
//	GetDlgItemText( IDC_ALT, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_ALT, sDlgItem ); // 1.4qpw
	m_sAlt = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_GLOSS, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_GLOSS, sDlgItem ); // 1.4qpw
	m_sGloss = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_PS, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_PS, sDlgItem ); // 1.4qpw
	m_sPS = sUTF8( sDlgItem ); // 1.4qpw
//	GetDlgItemText( IDC_UND, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_UND, sDlgItem ); // 1.4qpw
	m_sUnd = sUTF8( sDlgItem ); // 1.4qpw

	if ( m_lboDbsToSearch.GetCount() == 0 ) // Insist on at least one database
		{
		AfxMessageBox( _("At least one database must be entered.") );
		return;
		}

	if ( m_sGloss.IsEmpty() ) // Insist that the markers not be empty
		m_sGloss = "ge"; // 5.99c Change defualt to more common 2 letter codes
	if ( m_sPS.IsEmpty() )
		m_sPS = "ps";
	if ( m_sAlt.IsEmpty() )
		m_sAlt = "a";
	if ( m_sUnd.IsEmpty() )
		m_sUnd = "u";
	CDatabaseRef* pdrf;
	for (int idx = 0; idx < m_lboDbsToSearch.GetCount(); idx++)
		{
		pdrf = (CDatabaseRef*)m_lboDbsToSearch.GetItemDataPtr(idx);
		m_pdrflst->Add( pdrf->sDatabasePath(), pdrf->sKey() );
		}     

	UpdateData(TRUE);
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CRearrangeDlg dialog

CRearrangeDlg::CRearrangeDlg( CRearrangeProc* pintprc )
	: CDialog(CRearrangeDlg::IDD, NULL),
	m_cboToMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboFromMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboPSMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboPuncMarker( pintprc->pintprclst()->ptyp()->pmkrset(), TRUE, NULL )
{
	m_pintprc = pintprc;
	//{{AFX_DATA_INIT(CRearrangeDlg)
	//}}AFX_DATA_INIT
}

void CRearrangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRearrangeDlg)
	DDX_Control(pDX, IDC_LBL_RULEFILE, m_lblRuleFile);
	DDX_Control(pDX, IDC_TO_MARKER, m_cboToMarker);
	DDX_Control(pDX, IDC_FROM_MARKER, m_cboFromMarker);
	DDX_Control(pDX, IDC_PUNC_MARKER, m_cboPuncMarker);
	DDX_Control(pDX, IDC_PS_MARKER, m_cboPSMarker);
	DDX_Control(pDX, IDC_Adapt, m_chkAdapt);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRearrangeDlg, CDialog)
	//{{AFX_MSG_MAP(CRearrangeDlg)
	ON_BN_CLICKED(IDC_BROWSE_RULEFILE, OnBrowseRulefile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRearrangeDlg message handlers

void CRearrangeDlg::SetRuleFileWindowTextNone() // Set the CC table text to None
{
	Str8 sNone;
	sNone = _("None");
    m_lblRuleFile.SetWindowText(  swUTF16( sNone ) ); // 1.4qpy
}

BOOL CRearrangeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_cboToMarker.UpdateElements( m_pintprc->pmkrTo() );	
	m_cboFromMarker.UpdateElements( m_pintprc->pmkrFrom() );
	m_cboPSMarker.UpdateElements( m_pintprc->pmkrPSFrom() );	
	m_cboPuncMarker.UpdateElements( m_pintprc->pmkrPunc() );	

	m_sRuleFile = m_pintprc->pszRuleFile(); // Get name of rule file
	if ( !m_sRuleFile.IsEmpty() )
		m_lblRuleFile.SetWindowText(  swUTF16( sGetFileName(m_sRuleFile) ) ); // 1.4qpy
	else
		SetRuleFileWindowTextNone(); // Set rule file text to None

	if ( m_pintprc->bAdapt() )
		m_chkAdapt.SetCheck( 1 );

	UpdateData( FALSE ); // Put values into controls
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRearrangeDlg::OnOK() 
{
	if ( !UpdateData( TRUE ) )
		return;
	if ( m_cboFromMarker.pmkrSelected() == m_cboToMarker.pmkrSelected() ) // Don't let them select same marker
		{
		AfxMessageBox( _("The From and To markers cannot be the same.") );
		return;
		}
	m_pintprc->SetMarkers( m_cboFromMarker.pmkrSelected(),
		m_cboToMarker.pmkrSelected() );
	m_pintprc->SetPSMarker( m_cboPSMarker.pmkrSelected() ); // Set the part of speech marker
	m_pintprc->SetPuncMarker( m_cboPuncMarker.pmkrSelected() ); // Set the punc marker
	m_pintprc->SetRuleFile( m_sRuleFile ); // Set the rule file name
//	m_pintprc->SetAdapt( m_chkAdapt.GetCheck() );      
	m_pintprc->SetAdapt( TRUE ); // Always an adapt process
	
	CDialog::OnOK();
}

void CRearrangeDlg::OnBrowseRulefile()
{
	long lFlags = (OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);
	Str8 sFilter;
	Str8 sFileFilter = _("All files"); // 1.5.0fz 
	sFileFilter += " (*.*)|*.*||"; // 1.5.0fz 
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlgCCTable(TRUE, swUTF16( "rul" ), swUTF16( sGetFileName(m_sRuleFile ) ), lFlags, swUTF16( sFileFilter ) ); // 1.4qrd // 1.5.8h 
		Str8 sInitialDir = sGetDirPath(m_sRuleFile);
		dlgCCTable.m_ofn.lpstrInitialDir =  swUTF16( sInitialDir ); // 1.4qra
		dlgCCTable.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlgCCTable ) != IDOK )
			return;
		sPath = sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sRuleFile) ) // 1.5.8h 
			return; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return; // 1.4vze 
	m_sRuleFile =  sPath; // 1.4qra // 1.5.8h 
//	m_sRuleFile =  sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
	m_lblRuleFile.SetWindowText(  swUTF16( sGetFileName(m_sRuleFile) ) ); // Set display of name // 1.4qpy
}

/////////////////////////////////////////////////////////////////////////////
// CGivenDlg dialog

CGivenDlg::CGivenDlg( CGivenProc* pintprc )
	: CDialog(CGivenDlg::IDD, NULL),
	m_cboFromMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL )
{
	m_pintprc = pintprc;
	//{{AFX_DATA_INIT(CGivenDlg)
	//}}AFX_DATA_INIT
}

void CGivenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGivenDlg)
	DDX_Control(pDX, IDC_FROM_MARKER, m_cboFromMarker);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGivenDlg, CDialog)
	//{{AFX_MSG_MAP(CGivenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGivenDlg message handlers

BOOL CGivenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_cboFromMarker.UpdateElements( m_pintprc->pmkrFrom() );

	UpdateData( FALSE ); // Put values into controls
	return TRUE;  // return TRUE  unless you set the focus to a control
}
	

void CGivenDlg::OnOK() 
{
	if ( !UpdateData( TRUE ) )
		return;
	m_pintprc->SetMarkers( m_cboFromMarker.pmkrSelected(),
		m_cboFromMarker.pmkrSelected() ); // Set both from and to markers to the same thing
	
	CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
// CGenerateDlg dialog


CGenerateDlg::CGenerateDlg( CRearrangeProc* pintprc )
	: CDialog(CGenerateDlg::IDD, NULL),
	m_cboToMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboFromMarker( pintprc->pintprclst()->ptyp()->pmkrset(), FALSE, NULL ),
	m_cboPuncMarker( pintprc->pintprclst()->ptyp()->pmkrset(), TRUE, NULL )
{
	m_pintprc = pintprc;
	//{{AFX_DATA_INIT(CGenerateDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGenerateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerateDlg)
	DDX_Control(pDX, IDC_LBL_RULEFILE, m_lblRuleFile);
	DDX_Control(pDX, IDC_TO_MARKER, m_cboToMarker);
	DDX_Control(pDX, IDC_FROM_MARKER, m_cboFromMarker);
	DDX_Control(pDX, IDC_PUNC_MARKER, m_cboPuncMarker);
	DDX_Control(pDX, IDC_KeepMBrks, m_chkKeepMBrks);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGenerateDlg, CDialog)
	//{{AFX_MSG_MAP(CGenerateDlg)
	ON_BN_CLICKED(IDC_BROWSE_RULEFILE, OnBrowseRulefile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGenerateDlg message handlers

void CGenerateDlg::SetRuleFileWindowTextNone() // Set the CC table text to None
{
	Str8 sNone;
	sNone = _("None");
    m_lblRuleFile.SetWindowText(  swUTF16( sNone ) ); // 1.4qpy
}

BOOL CGenerateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_cboToMarker.UpdateElements( m_pintprc->pmkrTo() );	
	m_cboFromMarker.UpdateElements( m_pintprc->pmkrFrom() );
	m_cboPuncMarker.UpdateElements( m_pintprc->pmkrPunc() );	

	m_sRuleFile = m_pintprc->pszRuleFile(); // Get name of rule file
	if ( !m_sRuleFile.IsEmpty() )
		m_lblRuleFile.SetWindowText(  swUTF16( sGetFileName(m_sRuleFile) ) ); // 1.4qpy
	else
		SetRuleFileWindowTextNone(); // Set rule file text to None

	if ( m_pintprc->bKeepMorphemeBreaks() )
		m_chkKeepMBrks.SetCheck( 1 );

	UpdateData( FALSE ); // Put values into controls
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGenerateDlg::OnOK()
{
	if ( !UpdateData( TRUE ) )
		return;
	if ( m_cboFromMarker.pmkrSelected() == m_cboToMarker.pmkrSelected() ) // Don't let them select same marker
		{
		AfxMessageBox( _("The From and To markers cannot be the same.") );
		return;
		}
	m_pintprc->SetMarkers( m_cboFromMarker.pmkrSelected(),
		m_cboToMarker.pmkrSelected() );
	m_pintprc->SetPuncMarker( m_cboPuncMarker.pmkrSelected() ); // Set the punc marker
	m_pintprc->SetRuleFile( m_sRuleFile ); // Set the rule file name
	m_pintprc->SetKeepMorphemeBreaks( m_chkKeepMBrks.GetCheck() );      
	m_pintprc->SetAdapt( TRUE ); // Always an adapt process
	
	CDialog::OnOK();
}

void CGenerateDlg::OnBrowseRulefile()
{
	long lFlags = (OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR);
	Str8 sFilter;
	Str8 sFileFilter = _("All files"); // 1.5.0fz 
	sFileFilter += " (*.*)|*.*||"; // 1.5.0fz 
	Str8 sPath; // 1.5.8h 
	if ( !Shw_papp()->bUseCrashFixFileEditDlg() ) // 1.5.8h 
		{
		CFileDialog dlgCCTable(TRUE, swUTF16( "rul" ), swUTF16( sGetFileName(m_sRuleFile ) ), lFlags, swUTF16( sFileFilter ) ); // 1.4qrd // 1.5.8h 
		Str8 sInitialDir = sGetDirPath(m_sRuleFile);
		dlgCCTable.m_ofn.lpstrInitialDir =  swUTF16( sInitialDir ); // 1.4qra
		dlgCCTable.m_ofn.lpstrTitle = _T(" "); // 1.4qzhs Fix bad letters in file browse box title
		if ( iDlgDoModalSetReg( &dlgCCTable ) != IDOK )
			return;
		sPath = sUTF8( dlgCCTable.GetPathName() ); // 1.4qra
		}
	else // 1.5.8h 
		if ( !bCrashFixBrowseForFileName(sPath, m_sRuleFile) ) // 1.5.8h 
			return; // 1.5.8h 
	if ( !bAllASCIIComplain( sPath ) ) // 1.4vze 
		return; // 1.4vze 
	m_sRuleFile =  sPath; // 1.4qra // 1.5.8h 
	m_lblRuleFile.SetWindowText(  swUTF16( sGetFileName(m_sRuleFile) ) ); // Set display of name // 1.4qpy
}
