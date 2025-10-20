// multmat.cpp  Implementation of multiple match listbox and dialog

#include "stdafx.h"
#include "toolbox.h"
#include "project.h"
#include "multmat.h"
#include "typ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



int CMultipleMatchList::iModalView_Elements(CMultipleMatch** ppmmt,
        BOOL bSingleIndex, BOOL bEnableInsert)
{
    CMultipleMatchListBox lboMatches(this, ppmmt, bSingleIndex);
    CMultipleMatchDlg dlg(lboMatches, bEnableInsert);
    return dlg.DoModal();  // show a modal view of the multiple match list
}


// --------------------------------------------------------------------------


CMultipleMatchListBox::CMultipleMatchListBox(CMultipleMatchList* pmmtlst,
        CMultipleMatch** ppmmt, BOOL bSingleIndex) : CDblListListBox(pmmtlst)
{
    m_bSingleIndex = bSingleIndex;
    m_pmmtlst = pmmtlst;
    m_ppmmt = ppmmt;
    if (!*m_ppmmt)
        *m_ppmmt = pmmtlst->pmmtFirst(); // select first in list if none specified
}

void CMultipleMatchListBox::InitLabels()
{
    m_xWord = xSubItem_Show(IDC_MULTMATCH_WORD);
    if (!m_bSingleIndex)
        {
        m_xAdditionalField = xSubItem_Show(IDC_MULTMATCH_ADDITIONAL_FIELD1);
        m_xDatabase = xSubItem_Show(IDC_MULTMATCH_DATABASE);
        }
    else
        m_xAdditionalField = xSubItem_Show(IDC_MULTMATCH_ADDITIONAL_FIELD2);
}

void CMultipleMatchListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CMultipleMatch* pmmt = (CMultipleMatch*)pel;
    const CField* pfld = pmmt->prel()->pfldPriKey();
    const CMarker* pmkr = pmmt->pdbref()->pind()->pindsetMyOwner()->ptyp()->pmkrMultipleMatchShow(); // get marker of additional field to show
    int xDatabase = m_bSingleIndex ? 0 : m_xDatabase; // database not displayed if viewing matches from single index search
    int xAdditionalField = pmkr ? m_xAdditionalField : xDatabase; // determine area used to display match text

    // CFont* pfnt = (CFont*)pfld->pmkr()->pfntDlg();
    DrawSubItemJustify(cDC, rcItem, m_xWord, xAdditionalField,
        pfld->sContents(), pfld->plng());  // 1998-02-13 MRP
        // pfld->sContents(), pfnt);

    if ( pmkr ) // may be NULL
        {
        pfld = pmmt->prel()->pfldFirstInSubRecord( pmkr );
        if ( pfld ) // if field exists in record, display it in additional field column
            {
            // pfnt = (CFont*)pfld->pmkr()->pfntDlg();
            DrawSubItemJustify(cDC, rcItem, m_xAdditionalField, xDatabase,
                pfld->sContents(), pfld->plng());  // 1998-02-13 MRP
                // pfld->sContents(), pfnt);
            }
        }

    if (!m_bSingleIndex)
        {
        Str8 sPath;
        if (Shw_pProject()->bShowWholePath())
            sPath = pmmt->pdbref()->sDatabasePath();
        else
            sPath = sGetFileName( pmmt->pdbref()->sDatabasePath() ); // just show filename without path

        DrawSubItemJustify(cDC, rcItem, m_xDatabase, 0, sPath);
        }
}

void CMultipleMatchListBox::InitListBox()
{
    InitLabels();
    SetItemHeight(iItemHeight());
    UpdateElements(*m_ppmmt);
}

int CMultipleMatchListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CMultipleMatchListBox::Close()
{
    // Return the final selection in the list box
    *m_ppmmt = (CMultipleMatch*)pelSelected();
}


// --------------------------------------------------------------------------


void CMultipleMatchDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    int id;
    if ( m_lboMatches.bSingleIndex() )
        id = IDC_MULTMATCH_ADDITIONAL_FIELD2;
    else
        id = IDC_MULTMATCH_ADDITIONAL_FIELD1;
    //{{AFX_DATA_MAP(CMultipleMatchDlg)
    DDX_Control(pDX, IDC_INSERT, m_btnInsert);
    DDX_Control(pDX, id, m_lblAdditionalField);
    DDX_Control(pDX, IDC_MULTMATCH_SHOWPATH, m_chkShowPath);
    DDX_Control(pDX, IDC_MULTMATCH_MKRS, m_cboAdditionalField);
    DDX_Control(pDX, IDC_MULTMATCH_LIST, m_lboMatches);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultipleMatchDlg, CDialog)
    //{{AFX_MSG_MAP(CMultipleMatchDlg)
    ON_LBN_DBLCLK(IDC_MULTMATCH_LIST, OnDblclkMultmatchList)
    ON_CBN_SELCHANGE(IDC_MULTMATCH_MKRS, OnSelchangeMultmatchMkrs)
    ON_LBN_SELCHANGE(IDC_MULTMATCH_LIST, OnSelchangeMultmatchList)
    ON_BN_CLICKED(IDC_MULTMATCH_SHOWPATH, OnMultmatchShowpath)
    ON_WM_MEASUREITEM()
    ON_BN_CLICKED(IDC_TRYAGAIN, OnTryagain)
    ON_BN_CLICKED(IDC_INSERT, OnInsert)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


CMultipleMatchDlg::CMultipleMatchDlg(CMultipleMatchListBox& lboMatches,
        BOOL bEnableInsert) :
    CDialog(IDD),
    m_lboMatches(lboMatches),
    m_bEnableInsert(bEnableInsert),
    m_cboAdditionalField( lboMatches.pmmtlst()->pmmtFirst()->pdbref()->pind()->pindsetMyOwner()->pmkrset(),
                   TRUE, NULL ) //lboMatches.pmmtlst()->pmmtFirst()->pdbref()->pind()->pmkrPriKey())
{
}

BOOL CMultipleMatchDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_btnInsert.EnableWindow(m_bEnableInsert);  // 1996-10-16 MRP    
    if (!m_lboMatches.bSingleIndex())
        {
        m_chkShowPath.ShowWindow(SW_SHOWNOACTIVATE);
        m_chkShowPath.SetCheck(Shw_pProject()->bShowWholePath());
        }

    m_lboMatches.InitListBox();

    ReloadMkrCombo(TRUE);

    return TRUE;  // input focus will be set to the dialog's first control
}

void CMultipleMatchDlg::ReloadMkrCombo( BOOL bForce )
{
    if ( m_lboMatches.bSingleIndex() && !bForce )
        return; // don't bother if matches are all from same index
    
    CMarker* pmkr = m_lboMatches.pindSelected()->pindsetMyOwner()->ptyp()->pmkrMultipleMatchShow();
    // reload marker set - selection change may have been to match from different db
    m_cboAdditionalField.ChangeSet( m_lboMatches.pindSelected()->pindsetMyOwner()->pmkrset(),
            pmkr, NULL ); // m_lboMatches.pindSelected()->pmkrPriKey() );
    if ( pmkr )
        m_lblAdditionalField.SetWindowText(  swUTF16( "\\" + pmkr->sMarker() ) ); // 1.4qpy
    else
        m_lblAdditionalField.SetWindowText(  swUTF16( "" ) ); // 1.4qpy
}

void CMultipleMatchDlg::OnOK()
{
    m_lboMatches.Close();
    CDialog::OnOK();
}

void CMultipleMatchDlg::OnDblclkMultmatchList()
{
    OnOK();
}

void CMultipleMatchDlg::OnSelchangeMultmatchMkrs()
{
    CMarker* pmkr = m_cboAdditionalField.pmkrSelected();
    m_lboMatches.pindSelected()->pindsetMyOwner()->ptyp()->SetMultipleMatchMkr( pmkr );
    if ( pmkr )
        m_lblAdditionalField.SetWindowText(  swUTF16( "\\" + m_cboAdditionalField.pmkrSelected()->sMarker() ) ); // 1.4qpy
    else
        m_lblAdditionalField.SetWindowText(  swUTF16( "" ) ); // 1.4qpy

    m_lboMatches.Invalidate(); // force redraw
}

void CMultipleMatchDlg::OnSelchangeMultmatchList()
{
    ReloadMkrCombo(FALSE);
}

void CMultipleMatchDlg::OnMultmatchShowpath()
{
    Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
    m_lboMatches.Invalidate(); // force redraw
}


void CMultipleMatchDlg::OnTryagain()
{
    EndDialog( IDC_TRYAGAIN );
}

void CMultipleMatchDlg::OnInsert()
{
    EndDialog( IDC_INSERT );
}

