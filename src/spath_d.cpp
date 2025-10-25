// spath_d.cpp  Implementation of jump path dialog boxes
//
// "Thy word is a lamp to my feet, And a light to my path."
//                       Psalm 119:105

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "project.h"
#include "shwdoc.h"
#include "doclist.h"
#include "spath_d.h"
#include "seldb_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatabaseRefListBox implementation

CDatabaseRefListBox::CDatabaseRefListBox( const CDatabaseRefList* pdrflst) :
    CDblListListBox((CDblList*)pdrflst) // Cast away const-ness rather than redo base class
{
    m_pdrflst = pdrflst;
}

void CDatabaseRefListBox::InitLabels()
{
    m_xDatabase = xSubItem_Show(IDC_SEARCHPATH_LBLINPATH);
    m_xMarker = xSubItem_Show(IDC_SEARCHPATH_FIELDTOSEARCH);
}

void CDatabaseRefListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CDatabaseRef* pdrf = (CDatabaseRef*)pel;
    Str8 sPath;
    if (Shw_pProject()->bShowWholePath())
        sPath = pdrf->sDatabasePath();
    else
        sPath = sGetFileName( pdrf->sDatabasePath() ); // just show filename without path

    DrawSubItem(cDC, rcItem, m_xDatabase, m_xMarker, sPath);
    DrawSubItem(cDC, rcItem, m_xMarker, 0, pdrf->sKey());
}

void CDatabaseRefListBox::InitListBox()
{
    InitLabels();
    ResetContent();  // remove any items currently in the list box

    // Insert the names of the elements in the list in the list box.
    CDatabaseRef* pdrf = m_pdrflst->pdrfFirst();
    for ( ; pdrf != NULL; pdrf = m_pdrflst->pdrfNext(pdrf) )
        InsertPath( -1, pdrf->sDatabasePath(), pdrf->sKey() );
    SetCurSel(0);
}

int CDatabaseRefListBox::InsertPath(int iDest, Str8 sPath, Str8 sKey)
{
    CDatabaseRef* pdrf = new CDatabaseRef( sPath, sKey );
    m_drflstCleanup.Add(pdrf); // for object deletion when we destruct
    int idx = InsertString(iDest, swUTF16( "" ) ); // insert item into listbox // 1.4qre
    SetItemDataPtr(idx, (void*)pdrf);
    return idx;
}


void CDocListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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
        Str8 sText;
        if (Shw_pProject()->bShowWholePath())
            {
            CShwDoc* pdoc = (CShwDoc*)GetItemDataPtr(lpDrawItemStruct->itemID);
            ASSERT((long)pdoc != LB_ERR);
            sText =  sUTF8( pdoc->GetPathName() ); // 1.4qra
            }
        else // just show filename without path
			{
//			char buffer[DLGBUFMAX+1]; // 1.4qvp
			CString swText; // 1.4qvp Upgrade GetText for Unicode build
            GetText(lpDrawItemStruct->itemID, swText); // get filename from listbox // 1.4qvp
			sText =  sUTF8( swText ); // 1.4qvp
			}
        
		CString swText = swUTF16( sText ); // 1.4qta
        cDC.ExtTextOut(rcItem.left, rcItem.top, ETO_CLIPPED, &rcItem, swText, swText.GetLength(), NULL); // 1.4qta
        }
#ifndef _MAC
    if ((lpDrawItemStruct->itemState) & ODS_FOCUS)
        cDC.DrawFocusRect(&(lpDrawItemStruct->rcItem));
#endif      
    cDC.Detach();
}

void CDocListBox::AddDocument(CShwDoc* pdoc, BOOL bCheckForDuplicates)
{
    Str8 sName = sGetFileName( sUTF8( pdoc->GetPathName()) );  // add filename only so keys work to select dbs // 1.4qwt Upgrade GetPathName for Unicode build
    if (bCheckForDuplicates && FindStringExact( -1, swUTF16( sName ) ) != LB_ERR ) // don't add a document more than once // 1.4qvr
        return;
    // Add to the databases available listbox
    int idx = AddString( swUTF16( sName  ) ); // 1.4qvs
    SetItemDataPtr(idx, (void*)pdoc);
}

int CDocListBox::LoadDbsAvailable(char* pszDbSel)
{
    int iCount = 0;

    ResetContent();

    CDocList doclst; // list of currently open docs
    for ( CShwDoc* pdoc = doclst.pdocFirst(); pdoc; pdoc = doclst.pdocNext() )
        if ( pdoc->pindset() ) // don't add uninitialized doc - can happen in File New... or Import
            {
            AddDocument(pdoc); // add to list
            iCount++;
            }

    int idx = 0;
    if (pszDbSel)
		{
		Str8 s = pszDbSel; // 1.4qvr
		CString sw = swUTF16( s ); // 1.4qvr
        idx = FindStringExact(-1, sw); // try to set selection to specified marker // 1.4qvr
		}
    if (idx == LB_ERR)
        idx = 0;
    SetCurSel(idx);
    return iCount;
}

// 1999-11-01 TLB : Remove all the extra boolean params and change function name. Make use of new
// dbref function: bInsertable.
void CDocListBox::LoadDbsInsertable( const CDatabaseRefList* pdrflst )
{
    // load up list based on a drflst (from jump path, or from copy/move record, or from merge db)
    ResetContent();
    CShwDoc* pdocFirst = NULL; // used to select first db in path below
    
    CDatabaseRef* pdrf = pdrflst->pdrfFirst();
    for ( ; pdrf; pdrf = pdrflst->pdrfNext(pdrf) ) // only want docs that are currently open
        {
        if ( pdrf->bInsertable() )
            {
            CShwDoc* pdoc = pdrf->pdoc(FALSE);
            AddDocument(pdoc, TRUE); // add to list, checking for duplicates
            if ( !pdocFirst )
                pdocFirst = pdoc;
            }
        }

    // First db in drflst may not be first one in list box, so go find it.
	int i=GetCount()-1;
    for (; i>=0; i--)
        if ((void*)pdocFirst == GetItemDataPtr(i)) // find the one we want?
            break;

    // NOTE i == -1 means no selection
    SetCurSel(i); // set selection to first db in jump path
}

// **************************************************************************

CJumpPathSetPage::CJumpPathSetPage(CSetListBox& lboSet) :
    CSetPage(lboSet, IDS_JMPSET_TITLE, IDD) // 1.4et OK, doesn't use CStrg
{
    m_bWarnDocClose = Shw_pProject()->bWarnDocClose();
    m_bWasActive = FALSE;
}

BOOL CJumpPathSetPage::OnInitDialog()
{
    m_bWasActive = TRUE;
    
    return CSetPage::OnInitDialog();
}

void CJumpPathSetPage::DoDataExchange(CDataExchange* pDX)
{
    CSetPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CJumpPathSetPage)
    DDX_Check(pDX, IDC_jumppath_chkWarnDocClose, m_bWarnDocClose);
    //}}AFX_DATA_MAP
}

void CJumpPathSetPage::OnOK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;
    UpdateData(TRUE);
    
    Shw_pProject()->SetWarnDocClose(m_bWarnDocClose);
    CSetPage::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CJumpPathDlg dialog


CJumpPathDlg::CJumpPathDlg( CJumpPath* pjmp ) :
    CDialog(CJumpPathDlg::IDD),
    m_pjmpset(pjmp->m_pjmpsetMyOwner),
    m_pjmp(pjmp),
    m_ppjmpNew(NULL),
    m_sName(m_pjmp->sName()),
    m_cboMarkers(NULL, FALSE, NULL),
    m_drflstTemp(pjmp->m_drflstTarget),
    m_lboPaths(&m_drflstTemp),
    m_subsetUsed(m_pjmpset->pmkrset(), FALSE),
    m_subsetTemp(pjmp->m_subsetSourceFields),
    m_lboExcluded(m_subsetTemp, FALSE, &m_subsetUsed),
    m_lboIncluded(m_subsetTemp, TRUE),
    m_bKeySetExplicitlyByUser(pjmp->bKeySetExplicitlyByUser())
{
    ASSERT( m_pjmpset );
    ASSERT( m_pjmp );
    ASSERT( !m_ppjmpNew );
    m_pjmp->SetKeyCriteria(m_key);
}

CJumpPathDlg::CJumpPathDlg(CJumpPathSet* pjmpset, CJumpPath** ppjmpNew) :
    CDialog(CJumpPathDlg::IDD),
    m_pjmpset(pjmpset),
    m_pjmp(NULL),
    m_ppjmpNew(ppjmpNew),
    m_cboMarkers(NULL, FALSE, NULL),
    m_lboPaths(&m_drflstTemp),
    m_subsetUsed(m_pjmpset->pmkrset(), FALSE),
    m_subsetTemp(m_pjmpset->pmkrset(), FALSE),
    m_lboExcluded(m_subsetTemp, FALSE, &m_subsetUsed),
    m_lboIncluded(m_subsetTemp, TRUE),
    m_bKeySetExplicitlyByUser(FALSE),
    m_key("", TRUE, CMCharOrder::matsetSamePrimaryGroup)
{
    ASSERT( m_pjmpset );
    ASSERT( !m_pjmp );
    ASSERT( m_ppjmpNew );
}

CJumpPathDlg::CJumpPathDlg(const CJumpPath* pjmpToCopyFrom, CJumpPath** ppjmpNew) :
    CDialog(CJumpPathDlg::IDD),
    m_pjmpset(pjmpToCopyFrom->pjmpsetMyOwner()),
    m_pjmp(NULL),
    m_ppjmpNew(ppjmpNew),
    m_cboMarkers(NULL, FALSE, NULL),
    m_drflstTemp(pjmpToCopyFrom->m_drflstTarget),
    m_lboPaths(&m_drflstTemp),
    m_subsetUsed(m_pjmpset->pmkrset(), FALSE),
    m_subsetTemp(m_pjmpset->pmkrset(), FALSE),
    m_lboExcluded(m_subsetTemp, FALSE, &m_subsetUsed),
    m_lboIncluded(m_subsetTemp, TRUE),
    m_bKeySetExplicitlyByUser(pjmpToCopyFrom->bKeySetExplicitlyByUser())
{
    ASSERT( m_pjmpset );
    ASSERT( !m_pjmp );
    ASSERT( m_ppjmpNew );
    ASSERT( pjmpToCopyFrom );
    pjmpToCopyFrom->SetKeyCriteria(m_key);
}

void CJumpPathDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CJumpPathDlg)
    DDX_Control(pDX, IDC_jumppath_chkDefault, m_chkDefault);
    DDX_Control(pDX, IDC_jumppath_lboAvailableFields, m_lboExcluded);
    DDX_Control(pDX, IDC_jumppath_btnAddField, m_btnAddField);
    DDX_Control(pDX, IDC_jumppath_btnRemoveField, m_btnRemoveField);
    DDX_Control(pDX, IDC_jumppath_btnClearFields, m_btnClearFields);
    DDX_Control(pDX, IDC_jumppath_lboSourceFields, m_lboIncluded);
    DDX_Control(pDX, IDC_jumppath_lboPaths, m_lboPaths);
    DDX_Control(pDX, IDC_jumppath_lboDBsAvailable, m_lboDbsAvailable);
    DDX_Control(pDX, IDC_jumppath_cboTargetFields, m_cboMarkers);
    DDX_Control(pDX, IDC_jumppath_chkShowPath, m_chkShowPath);
    DDX_Control(pDX, IDC_jumpath_chkRefIntegrity, m_chkRefIntegrity);
    //}}AFX_DATA_MAP
//    m_pjmpset->DDX_Name(pDX, IDC_jumppath_edtName, m_sName); // 1.4hdg Replace DDX_Name in jump path dlg
}

BEGIN_MESSAGE_MAP(CJumpPathDlg, CDialog)
    //{{AFX_MSG_MAP(CJumpPathDlg)
    ON_BN_CLICKED(IDC_jumppath_chkDefault, OnClkDefault)
    ON_BN_CLICKED(IDC_jumpath_chkRefIntegrity, OnClkDataLink)
    ON_BN_CLICKED(IDC_jumppath_btnAddField, OnAdd)
    ON_BN_CLICKED(IDC_jumppath_btnRemoveField, OnRemove)
    ON_BN_CLICKED(IDC_jumppath_btnClearFields, OnRemoveAll)
    ON_LBN_DBLCLK(IDC_jumppath_lboAvailableFields, OnAdd)
    ON_LBN_DBLCLK(IDC_jumppath_lboSourceFields, OnRemove)

    ON_LBN_DBLCLK(IDC_jumppath_lboDBsAvailable, OnSearchpathInsert)
    ON_LBN_SELCHANGE(IDC_jumppath_lboDBsAvailable, ReloadMkrCombo)
    ON_BN_CLICKED(IDC_jumppath_btnFirst, OnSearchpathFirst)
    ON_BN_CLICKED(IDC_jumppath_btnLast, OnSearchpathLast)
    ON_BN_CLICKED(IDC_jumppath_btnInsert, OnSearchpathInsert)
    ON_BN_CLICKED(IDC_jumppath_btnRemovePath, OnSearchpathRemove)
    ON_BN_CLICKED(IDC_jumppath_btnClearPaths, OnSearchpathClear)
    ON_LBN_DBLCLK(IDC_jumppath_lboPaths, OnSearchpathRemove)
    ON_BN_CLICKED(IDC_jumppath_chkShowPath, OnSearchpathShowpath)
    ON_BN_CLICKED(IDC_jumppath_Matching, OnJumpPathMatchingCriteria)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJumpPathDlg message handlers

BOOL CJumpPathDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_jumppath_edtName, swUTF16( m_sName ) ); // 1.4hdg // 1.4qpv
    // Is this the default path?
    m_bDefault = ( m_pjmp )? m_pjmp->bDefaultPath(): FALSE;
    m_chkDefault.SetCheck(m_bDefault);
    // Check all other jump paths to determine which source markers are already used.
    // Also, check for a default path.
    BOOL bDefaultExists = m_bDefault;
    CJumpPath* pjmp = m_pjmpset->pjmpFirst();
    for ( ; pjmp; pjmp = m_pjmpset->pjmpNext(pjmp) )
        {
        if ( m_pjmp != pjmp )
            {
            if ( pjmp->bDefaultPath() )
                bDefaultExists = TRUE;
            else 
                m_subsetUsed.IncludeSubsetElements(pjmp->m_subsetSourceFields);
            }
        }
	m_lboExcluded.Init();
	m_lboIncluded.Init();
    if ( m_bDefault ) // If this is the default jump path, disable the source selection controls.
        {
        m_lboExcluded.EnableWindow(FALSE);
        m_btnAddField.EnableWindow(FALSE);
        m_btnRemoveField.EnableWindow(FALSE);
        m_btnClearFields.EnableWindow(FALSE);
        m_lboIncluded.EnableWindow(FALSE);
        // Also, uncheck & disable referential integrity
        m_chkRefIntegrity.SetCheck(FALSE);
        m_chkRefIntegrity.EnableWindow(FALSE);
        }
    else
        {
        UpdateSourceButtons();
        if ( bDefaultExists ) // If there is already a default jump path (not this one), disable the default check box.
            m_chkDefault.EnableWindow(FALSE);
        m_chkRefIntegrity.SetCheck(( m_pjmp )? m_pjmp->bEnforceRefIntegrity(): FALSE);
        }
    m_pdocCurSel = NULL; // force reload of marker combo
    if ( m_lboDbsAvailable.LoadDbsAvailable() )
        {
        ReloadMkrCombo();
        }
    m_lboPaths.InitListBox();
    m_chkShowPath.SetCheck( Shw_pProject()->bShowWholePath() );
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJumpPathDlg::UpdateSourceButtons()
{
    m_btnAddField.EnableWindow(m_lboExcluded.bNonEmpty());
    m_btnRemoveField.EnableWindow(m_lboIncluded.bNonEmpty());
    m_btnClearFields.EnableWindow(m_lboIncluded.bNonEmpty());
}

void CJumpPathDlg::OnClkDefault()
{
    m_bDefault = m_chkDefault.GetCheck();
    if ( m_bDefault )
        {
        m_lboExcluded.ResetContent();
        m_lboIncluded.ResetContent();
        m_subsetTemp.IncludeAllElements(TRUE);
	    m_lboIncluded.Init();
        }
    else
        {
        m_lboIncluded.ResetContent();
        m_subsetTemp.ExcludeAllElements();
	    m_lboExcluded.Init();
        }
    m_lboIncluded.EnableWindow(!m_bDefault);
    m_lboExcluded.EnableWindow(!m_bDefault);
    m_btnAddField.EnableWindow(!m_bDefault);
    m_btnRemoveField.EnableWindow(!m_bDefault);
    m_btnClearFields.EnableWindow(!m_bDefault);
    m_chkRefIntegrity.SetCheck(m_chkRefIntegrity.GetCheck() && !m_bDefault);
    m_chkRefIntegrity.EnableWindow(!m_bDefault);
}

void CJumpPathDlg::OnClkDataLink()
{
    if ( m_bKeySetExplicitlyByUser )
        return;
    BOOL bDataLink = m_chkRefIntegrity.GetCheck();
    m_key.SetKeyCriteria(CJumpPath::s_bDefaultMatchWholeSetting(bDataLink),
                         CJumpPath::s_matsetDefaultSetting(bDataLink));
}

void CJumpPathDlg::OnAdd()
{
    m_lboExcluded.AddSelectedItemsTo(m_lboIncluded);
    UpdateSourceButtons();
    if ( m_lboExcluded.bEmpty() )
        m_lboIncluded.SetFocus();
}

void CJumpPathDlg::OnRemove()
{
    m_lboIncluded.AddSelectedItemsTo(m_lboExcluded);
    UpdateSourceButtons();
    if ( m_lboIncluded.bEmpty() )
        m_lboExcluded.SetFocus();
}

void CJumpPathDlg::OnRemoveAll()
{
    // Select all items in the Included list box and call Remove.
    ItemIndex itmLast = m_lboIncluded.GetCount() - 1;
    ASSERT( itmLast >= 0 );  // The button should have been disabled
	if ( itmLast == 0 )
		m_lboIncluded.SetSel(0, TRUE);
	else
		m_lboIncluded.SelItemRange(TRUE, 0, itmLast);
    OnRemove();
}

void CJumpPathDlg::ReloadMkrCombo()
{
    CShwDoc* pdoc = m_lboDbsAvailable.pdocSelected();
    ASSERT( pdoc );
    if (m_pdocCurSel != pdoc) // don't reload if selection not actually changed
        {
        // reload marker set - selection change may have been to match from different db
        m_cboMarkers.ChangeSet( pdoc->pmkrset(), pdoc->pmkrRecord(), NULL );
        m_pdocCurSel = pdoc;
        }
}

void CJumpPathDlg::OnSearchpathClear()
{
    m_lboPaths.ResetContent(); // clear list box
}

void CJumpPathDlg::OnSearchpathFirst()
{
    AddToSearchPathLB(0); // insert into jump path list box
}

void CJumpPathDlg::OnSearchpathLast()
{
    int idx = m_lboPaths.GetCount();
    AddToSearchPathLB(idx);
}

void CJumpPathDlg::OnSearchpathInsert()
{
    int idxSearchPath = m_lboPaths.GetCurSel();
    if (idxSearchPath == LB_ERR)
        idxSearchPath = m_lboPaths.GetCount(); // Insert at beginning.
    else
        idxSearchPath++;    // Forces insertion after highlight.

    AddToSearchPathLB(idxSearchPath); // insert into jump path list box
}

void CJumpPathDlg::AddToSearchPathLB(int idxDest)
{
    int idxAvailable = m_lboDbsAvailable.GetCurSel();
    if (idxAvailable == LB_ERR)
        return;
        
    // get currently selected database to add
    CShwDoc* pdoc = (CShwDoc*)m_lboDbsAvailable.GetItemDataPtr(idxAvailable);
    // get marker of field to search in selected database
    CMarker* pmkr = m_cboMarkers.pmkrSelected();

    // Stick it in the search path
	Str8 sPathName =  sUTF8( pdoc->GetPathName() ); // 1.4qws Upgrade GetPathName for Unicode build
    int idx = m_lboPaths.InsertPath(idxDest, sPathName, pmkr->sName() );
    m_lboPaths.SetCurSel(idx); // Make that the current selection.

    m_lboDbsAvailable.SetFocus(); // set focus back to list box so they can select another db
}

// delete selected path
void CJumpPathDlg::OnSearchpathRemove()
{
    int idx = m_lboPaths.GetCurSel();
    if (idx == LB_ERR)
        return;
        
    m_lboPaths.DeleteString(idx);
    idx = (idx < m_lboPaths.GetCount()) ? idx : idx-1;
    m_lboPaths.SetCurSel(idx);

    m_lboPaths.SetFocus();
}

void CJumpPathDlg::OnOK()
{
    UpdateData( TRUE ); // get data from controls to member variables
//    GetDlgItemText( IDC_jumppath_edtName, (char*)buffer, DLGBUFMAX ); // 1.4hdg
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_jumppath_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdg

    if ( m_sName.IsEmpty() )
        {
        AfxMessageBox(_("Please give a name."));
        return;
        }

    if ( 0 == m_lboPaths.GetCount() )
        {
        AfxMessageBox(_("At least one jump path destination is required."));
        return;
        }

    if ( !m_pjmp )
        {
        if ( m_pjmpset->pselSearch(m_sName) )
            {
            Str8 sMsg;
			sMsg = sMsg + _("Name is already used:") + " " + m_sName; // 1.5.0fd 
            AfxMessageBox(sMsg);
            return;
            }

        m_pjmp = new CJumpPath(m_sName, m_pjmpset);
        if (!m_pjmp)
            {
            ASSERT(FALSE);
            return;
            }

        CDatabaseRef* pdrf;
        for (int idx = 0; idx < m_lboPaths.GetCount(); idx++)
            {
            pdrf = (CDatabaseRef*)m_lboPaths.GetItemDataPtr(idx);
            m_pjmp->m_drflstTarget.Add( pdrf->sDatabasePath(), pdrf->sKey() );
            }
         }
    else
        {
        if ( m_sName != m_pjmp->sName() )
            if ( m_pjmpset->pselSearch(m_sName) )
                {
                Str8 sMsg;
				sMsg = sMsg + _("Name is already used:") + " " + m_sName; // 1.5.0fd 
                AfxMessageBox(sMsg);
                return;
                }

        CDatabaseRef* pdrf = m_pjmp->m_drflstTarget.pdrfFirst();
        while ( pdrf )
            m_drflstTemp.Add( m_pjmp->m_drflstTarget.pdrfRemove( &pdrf ) ); // transfer old path to temp list

        for (int idx = 0; idx < m_lboPaths.GetCount(); idx++)
            {
            pdrf = (CDatabaseRef*)m_lboPaths.GetItemDataPtr(idx);
            CDatabaseRef* pdrfT = m_drflstTemp.pdrfFirst();
            for ( ; pdrfT; pdrfT = m_drflstTemp.pdrfNext( pdrfT ) ) // search old jump path for match
                {
                if ( pdrf->sDatabasePath() == pdrfT->sDatabasePath() && // matching?
                        pdrf->sKey() == pdrfT->sKey() )
                    break;
                }
            if ( pdrfT ) // NULL if matching path not found in old jump path
                m_pjmp->m_drflstTarget.Add( m_drflstTemp.pdrfRemove( &pdrfT ) );
            else
                m_pjmp->m_drflstTarget.Add( pdrf->sDatabasePath(), pdrf->sKey() );
            }
        }

    ASSERT( m_pjmp != NULL );
    m_pjmp->ChangeNameTo(m_sName);

    if ( m_bDefault )
        {
        m_pjmp->m_subsetSourceFields.IncludeAllElements(TRUE);
        }
    else
        {
        ASSERT(!m_subsetTemp.bIncludeAllElements());
        m_lboExcluded.UpdateMarkerSubSet();
        if ( m_lboIncluded.bEmpty() )
            AfxMessageBox(_("This jump path has no source fields."), MB_OK | MB_ICONINFORMATION);
        m_lboIncluded.UpdateMarkerSubSet();
        m_pjmp->m_subsetSourceFields = m_subsetTemp;
        }

    m_pjmp->m_bEnforceRefIntegrity = m_chkRefIntegrity.GetCheck();
    m_pjmp->SetMatchCriteriaFromKey(&m_key, m_bKeySetExplicitlyByUser);

    if ( m_ppjmpNew != NULL )
    {
        m_pjmpset->Add(m_pjmp);
        *m_ppjmpNew = m_pjmp;  // return the new jump path
    }

    CDialog::OnOK();
}

void CJumpPathDlg::OnSearchpathShowpath()
{
    Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
    m_lboPaths.Invalidate(); // force redraw
    m_lboDbsAvailable.Invalidate();
}

void CJumpPathDlg::OnJumpPathMatchingCriteria()
{
    CJumpPathMatchingCriteriaDlg dlg(&m_key);
    if ( dlg.DoModal() == IDOK )
        m_bKeySetExplicitlyByUser = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CJumpPathMatchingCriteriaDlg dialog

CJumpPathMatchingCriteriaDlg::CJumpPathMatchingCriteriaDlg(CKey* pkey)
	: CDialog(CJumpPathMatchingCriteriaDlg::IDD),
	m_pkey(pkey)
{
	ASSERT( m_pkey );

	m_bMatchWhole = m_pkey->bMatchWhole();
	m_matset = m_pkey->matset();

	// 1995-05-24 MRP: need to encapsulate this better!!!	
	if ( m_matset == CMCharOrder::matsetSamePrimaryGroup )
		m_iMatchSetting = 0;  // index of radio button in group
	else if ( m_matset == CMCharOrder::matsetExactDisregardCase )
		m_iMatchSetting = 1;
	else if ( m_matset == CMCharOrder::matsetExact )
		m_iMatchSetting = 2;
	else if ( m_matset == CMCharOrder::matsetEvenIgnore )
		m_iMatchSetting = 3;
	else
		ASSERT( FALSE );
}

void CJumpPathMatchingCriteriaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJumpPathMatchingCriteriaDlg)
	DDX_Check(pDX, IDC_JUMP_WHOLE_KEY, m_bMatchWhole);
	DDX_Radio(pDX, IDC_JUMP_MATCH_SAME_PRIMARY, m_iMatchSetting);
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CJumpPathMatchingCriteriaDlg message handlers

BOOL CJumpPathMatchingCriteriaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJumpPathMatchingCriteriaDlg::OnOK()
{
	UpdateData(TRUE);

	// 1995-05-24 MRP: need to encapsulate this better!!!	
	if ( m_iMatchSetting == 0 )  // index of radio button in group
		m_matset = CMCharOrder::matsetSamePrimaryGroup;
	else if ( m_iMatchSetting == 1 )
		m_matset = CMCharOrder::matsetExactDisregardCase;
	else if ( m_iMatchSetting == 2 )
		m_matset = CMCharOrder::matsetExact;
	else if ( m_iMatchSetting == 3 )
		m_matset = CMCharOrder::matsetEvenIgnore;
	else
		ASSERT( FALSE );
		
	m_pkey->SetKeyCriteria(m_bMatchWhole, m_matset);
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CDocCloseWarnDlg dialog

CDocCloseWarnDlg::CDocCloseWarnDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CDocCloseWarnDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CDocCloseWarnDlg)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

void CDocCloseWarnDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDocCloseWarnDlg)
    DDX_Control(pDX, IDC_WARN_DOC_CLOSE, m_chkWarnDocClose);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDocCloseWarnDlg, CDialog)
    //{{AFX_MSG_MAP(CDocCloseWarnDlg)
    ON_BN_CLICKED(IDNO, OnNo)
    ON_BN_CLICKED(IDYES, OnYes)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CDocCloseWarnDlg message handlers

BOOL CDocCloseWarnDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    m_chkWarnDocClose.SetCheck(TRUE); // must be checked if we got here

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDocCloseWarnDlg::OnNo()
{
    Shw_pProject()->SetWarnDocClose( m_chkWarnDocClose.GetCheck() );
    EndDialog(IDNO);
}

void CDocCloseWarnDlg::OnYes()
{
    Shw_pProject()->SetWarnDocClose( m_chkWarnDocClose.GetCheck() );
    EndDialog(IDYES);
}

/////////////////////////////////////////////////////////////////////////////
// CDBNotOpenDlg dialog

CDBNotOpenDlg::CDBNotOpenDlg(const Str8& sClosedDB, const Str8& sJumpPath, CWnd* pParent) :
    CDialog(CDBNotOpenDlg::IDD, pParent),
    m_sJumpPath(sJumpPath),
    m_sClosedDB(sClosedDB)
{
}

void CDBNotOpenDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDBNotOpenDlg)
//    DDX_Txt(pDX, IDC_lblJumpPath, m_sJumpPath);
//    DDX_Txt(pDX, IDC_lblClosedDB, m_sClosedDB);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBNotOpenDlg, CDialog)
    //{{AFX_MSG_MAP(CDBNotOpenDlg)
    ON_BN_CLICKED(ID_SKIP, OnSkip)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDBNotOpenDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_lblJumpPath, swUTF16( m_sJumpPath ) ); // 1.4qpv
    SetDlgItemText( IDC_lblClosedDB, swUTF16( m_sClosedDB ) ); // 1.4qpv
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBNotOpenDlg::OnSkip()
{
    EndDialog(ID_SKIP);
}

/////////////////////////////////////////////////////////////////////////////
// CDBDiffPathDlg dialog

CDBDiffPathDlg::CDBDiffPathDlg(const Str8& sClosedDB, const Str8& sOpenDB,
                               const Str8& sJumpPath, CWnd* pParent) :
    CDialog(CDBDiffPathDlg::IDD, pParent),
    m_sJumpPath(sJumpPath),
    m_sClosedDB(sClosedDB),
    m_sOpenDB(sOpenDB)
{
}

void CDBDiffPathDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDBDiffPathDlg)
//    DDX_Txt(pDX, IDC_lblJumpPath, m_sJumpPath);
//    DDX_Txt(pDX, IDC_lblClosedDB, m_sClosedDB);
//    DDX_Txt(pDX, IDC_lblOpenDB, m_sOpenDB);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDBDiffPathDlg, CDialog)
    //{{AFX_MSG_MAP(CDBDiffPathDlg)
    ON_BN_CLICKED(ID_FIX_IT, OnFixIt)
    ON_BN_CLICKED(ID_SKIP, OnSkip)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDBDiffPathDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_lblJumpPath, swUTF16( m_sJumpPath ) ); // 1.4qpv
    SetDlgItemText( IDC_lblClosedDB, swUTF16( m_sClosedDB ) ); // 1.4qpv
    SetDlgItemText( IDC_lblOpenDB, swUTF16( m_sOpenDB ) ); // 1.4qpv
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDBDiffPathDlg::OnFixIt()
{
    EndDialog(ID_FIX_IT);
}

void CDBDiffPathDlg::OnSkip()
{
    EndDialog(ID_SKIP);
}

/////////////////////////////////////////////////////////////////////////////
// CFieldNotDefinedDlg dialog

CFieldNotDefinedDlg::CFieldNotDefinedDlg(const Str8& sDB, const Str8& sMkr,
                                         const Str8& sJumpPath, CWnd* pParent) :
    CDialog(CFieldNotDefinedDlg::IDD, pParent),
    m_sJumpPath(sJumpPath),
    m_sMkr(sMkr),
    m_sDB(sDB)
{
}

void CFieldNotDefinedDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFieldNotDefinedDlg)
//    DDX_Txt(pDX, IDC_lblJumpPath, m_sJumpPath);
//    DDX_Txt(pDX, IDC_lblMkr, m_sMkr);
//    DDX_Txt(pDX, IDC_lblDB, m_sDB);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFieldNotDefinedDlg, CDialog)
    //{{AFX_MSG_MAP(CFieldNotDefinedDlg)
    ON_BN_CLICKED(ID_SKIP, OnSkip)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFieldNotDefinedDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_lblJumpPath, swUTF16( m_sJumpPath ) ); // 1.4qpv
    SetDlgItemText( IDC_lblMkr, swUTF16( m_sMkr ) ); // 1.4qpv
    SetDlgItemText( IDC_lblDB, swUTF16( m_sDB ) ); // 1.4qpv
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFieldNotDefinedDlg::OnSkip()
{
    EndDialog(ID_SKIP);
}

/////////////////////////////////////////////////////////////////////////////
// CJumpPathSetListBox implementation

BOOL CJumpPathSetListBox::bDelete(CSetEl* psel)
{
    Str8 sMessage = _("Delete jump path?"); // 1.5.0fg 
    if ( AfxMessageBox(sMessage, MB_YESNO|MB_ICONQUESTION) == IDYES )
        return FALSE;  // not deleted
    return CSetListBox::bDelete(psel);
}

// --------------------------------------------------------------------------

CReferentialInconsistencyDlg::CReferentialInconsistencyDlg(CShwView* pview,
            CRecPos& rpsStart, CRecPos& rpsEnd, CCheckingState *pcks,
            BOOL& bNeedToRecheck, BOOL bEnableInsert, CJumpPath* pjmp,
            CShwDoc** ppdoc) :
    CInconsistencyDlg(CReferentialInconsistencyDlg::IDD, pview, rpsStart,
                      rpsEnd, pcks, bNeedToRecheck,
					  _("Data referenced by jump path not found"), // 1.4heq // 1.4vc // 1.4ve  // 1.5.0fg 
					  FALSE,
					  pjmp->sName()),
    m_bEnableInsert(bEnableInsert),
    m_pjmp(pjmp),
    m_ppdoc(ppdoc)
{
    ASSERT(m_pjmp);
}

CReferentialInconsistencyDlg::~CReferentialInconsistencyDlg()
{
}

BEGIN_MESSAGE_MAP(CReferentialInconsistencyDlg, CInconsistencyDlg)
    //{{AFX_MSG_MAP(CReferentialInconsistencyDlg)
    ON_BN_CLICKED(IDC_cck_btnInsert, OnInsert)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CReferentialInconsistencyDlg::DoDataExchange(CDataExchange* pDX)
{
    CInconsistencyDlg::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CReferentialInconsistencyDlg)
    DDX_Control(pDX, IDC_cck_btnInsert, m_btnInsert);
    //}}AFX_DATA_MAP
}

BOOL CReferentialInconsistencyDlg::OnInitDialog()
{
    BOOL bRet = CInconsistencyDlg::OnInitDialog();

    // TLB 11/17/99 - AT this time, we don't really need "!bReplacementModified()", but
    // it's consistent and it may help keep it working if things change in the future.
    m_btnInsert.EnableWindow(!bReplacementModified() && m_bEnableInsert);

    return bRet;
}

void CReferentialInconsistencyDlg::OnChangeReplacement()
{
    CInconsistencyDlg::OnChangeReplacement();
    // Insert is a valid choice if the replacement string matches the original
    // inconsistency and if one or more potential target DBs are writeable.
    if ( !bReplacementModified() && m_bEnableInsert )
    {
        m_btnInsert.EnableWindow(TRUE);
        SetDefID(IDC_cck_btnInsert);
    }
    else
        m_btnInsert.EnableWindow(FALSE);
}

void CReferentialInconsistencyDlg::OnInsert()
{
    CShwDoc* pdoc;
    // Have user select database to insert new record into (if there's more than one).
    CSelectDbDlg dlg(m_pjmp->pdrflstTarget(), &pdoc, CSelectDbDlg::actInsert, TRUE);
    if (dlg.DoModal() != IDOK || !pdoc )
        return;
    ASSERT(m_ppdoc);
    *m_ppdoc = pdoc;
    EndDialog(IDC_INSERT);
}

void CReferentialInconsistencyDlg::OnModify()
{
    AfxMessageBox("Not yet implemented", MB_ICONINFORMATION);
}
