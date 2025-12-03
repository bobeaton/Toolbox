// dlgsort.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "dlgsort.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortDlg dialog


CSortDlg::CSortDlg(CIndex** ppind, CIndex** ppindUnfiltered) :
    CDialog(CSortDlg::IDD)
{
    m_ppind = ppind;
    ASSERT( m_ppind );
    CIndex* pind = *m_ppind;
    ASSERT( pind );
    m_pindset = pind->pindsetMyOwner();
    m_ppindUnfiltered = ppindUnfiltered;
    ASSERT( m_ppindUnfiltered );
    m_iSortKeyLen = pind->maxlenSortKeyComponent();

    //{{AFX_DATA_INIT(CSortDlg)
    m_bSortFromEnd = pind->bSortPriKeyFromEnd();
    //}}AFX_DATA_INIT
    
    m_pmkrPriKey = pind->pmkrPriKey();
    m_pmkrset = m_pmkrPriKey->pmkrsetMyOwner();
    m_psrtPri = pind->psrtPriKey();
    m_pmrflstSecKeys = pind->pmrflstSecKeys();
    m_pfil = pind->pfil();
}

void CSortDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSortDlg)
    DDX_Check(pDX, IDC_SORTINDEX_CHKSORTFROMEND, m_bSortFromEnd);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSortDlg, CDialog)
    //{{AFX_MSG_MAP(CSortDlg)
    ON_BN_CLICKED(IDC_SORTINDEX_MOVEFROM, OnSortindexMovefrom)
    ON_BN_CLICKED(IDC_SORTINDEX_MOVETO, OnSortindexMoveto)
    ON_LBN_DBLCLK(IDC_SORTINDEX_LSTMRKSTOSORT, OnDblclkSortindexLstmrkstosort)
    ON_LBN_DBLCLK(IDC_SORTINDEX_LSTMRKSAVAILABLE, OnDblclkSortindexLstmrksavailable)
    ON_BN_CLICKED(IDC_SORTINDEX_FIRSTTO, OnSortindexFirstto)
    ON_BN_CLICKED(IDC_SORTINDEX_LASTTO, OnSortindexLastto)
    ON_BN_CLICKED(IDC_SORTINDEX_CLEARSORTS, OnSortindexClearsorts)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSortDlg message handlers

BOOL CSortDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    int idx, iTmp;
    Str8 sLBItem;
    
    // Load the fields sorted list.
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    
    // Put the primary sort key as first in the fields sorted list.
    sLBItem = m_pmkrPriKey->sMarker();
    idx = pLBMkrSort->InsertString(-1, swUTF16( sLBItem ) ); // 1.4qre
    iTmp = pLBMkrSort->SetItemDataPtr(idx,(void*)m_pmkrPriKey);
    
    // For each secondary marker.
    for (CMarkerRef* pmrf = m_pmrflstSecKeys->pmrfFirst();
         pmrf != NULL;  pmrf = m_pmrflstSecKeys->pmrfNext(pmrf))
        {
        // Add it to the fields sorted list in that order.
        sLBItem = pmrf->pmkr()->sMarker();
        idx = pLBMkrSort->InsertString(-1, swUTF16( sLBItem ) ); // 1.4qre
        iTmp = pLBMkrSort->SetItemDataPtr(idx,(void*)pmrf->pmkr());
        }
        
    // Set the current selection to the first item (primary key).
    pLBMkrSort->SetCurSel(0);
    
    // Load all available markers into other listbox
    LoadLBMkrsAvailable();

    // Load the sort orders for primary field combo box.
    LoadSortOrdersCbx();
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSortDlg::LoadLBMkrsAvailable( const char* pszMrkSel )
{
    Str8 sLBItem;
    int idx, iTmp;
    
    // Load up the available markers list.
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    
    pLBMkr->ResetContent();

    // first put all markers into markers available listbox
    for (CMarker* pmkr = m_pmkrset->pmkrFirst(); pmkr != NULL; pmkr = m_pmkrset->pmkrNext(pmkr))
        {
        // Add to the markers available listbox
        idx = pLBMkr->InsertString(-1, swUTF16( pmkr->sMarker() ) ); // 1.4qre
        iTmp = pLBMkr->SetItemDataPtr(idx,(void*)pmkr);
        }   

    // remove markers from markers available list that are in sorted listbox
    int iSortItems = pLBMkrSort->GetCount(); // get number of items in sorted listbox
    for (int i=0; i < iSortItems; i++)
        {
//		char buffer[DLGBUFMAX+1]; // 1.4qvp
		CString swText; // 1.4qvp Upgrade GetText for Unicode build
        pLBMkrSort->GetText(i, swText); // retrieve marker text // 1.4ed Change listbox get text to not use CStrg // 1.4qvp
        Str8 sText =  sUTF8( swText ); // 1.4qvp
        idx = pLBMkr->FindStringExact(-1, swUTF16( sText ) ); // 1.4qvr
        if (idx != LB_ERR) // is it in sorted list?
            pLBMkr->DeleteString(idx); // if so, delete it from available list
        }
    idx = 0;
    if (pszMrkSel)
		{
		Str8 s = pszMrkSel; // 1.4qvr
		CString sw = swUTF16( s ); // 1.4qvr
        idx = pLBMkr->FindStringExact(-1, sw); // try to set selection to specified marker // 1.4qvr
		}
    if (idx == LB_ERR)
        idx = 0;
    pLBMkr->SetCurSel(idx);
}

void CSortDlg::LoadSortOrdersCbx()
{
    // Load the sort orders for primary field combo box.
    
    int iTmp, idx;
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
    CSortOrderSet* psrtset = m_psrtPri->psrtsetMyOwner();  // 2000-09-06 MRP
    ASSERT(psrtset != NULL);    
    CSortOrder* psrt = psrtset->psrtFirst();
    ASSERT( psrt != NULL );
    for ( ; psrt != NULL; psrt = psrtset->psrtNext(psrt) )
        {
        // Add this one to the box.
        idx = pCbx->AddString( swUTF16( psrt->sName()  ) ); // 1.4qvs
        ASSERT(idx != CB_ERR);
        iTmp = pCbx->SetItemDataPtr(idx, psrt);
        }
        
    // Set that to the current primary field.
    idx = pCbx->FindStringExact(-1, swUTF16( m_psrtPri->sName() ) ); // 1.4qvr
    if (idx != CB_ERR)
        iTmp = pCbx->SetCurSel(idx);
}

void CSortDlg::OnSortindexMovefrom()
{
    int iTmp;
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );


    int idxMkrSort = pLBMkrSort->GetCurSel();
    if (idxMkrSort == LB_ERR)
        return;
    
    int idxMkrList = pLBMkr->GetCurSel();
    if (idxMkrList == LB_ERR)
        idxMkrList = pLBMkr->GetCount() - 1;    // Inserts after last element.
        
    // Remove marker from the sort field markers list.
    CMarker* pmkr = (CMarker*)pLBMkrSort->GetItemDataPtr(idxMkrSort);
    iTmp = pLBMkrSort->DeleteString(idxMkrSort);
    idxMkrSort = (idxMkrSort < pLBMkrSort->GetCount()) ? idxMkrSort : idxMkrSort-1;
    // Select next item.
    iTmp = pLBMkrSort->SetCurSel(idxMkrSort);
    
    // If there is a new first item, if there are items
    // change the sort order combo box to its default.
    if ( (idxMkrSort == 0) && (pLBMkrSort->GetCount() > 0))
        {
        CMarker* pMkrFirst = (CMarker*)pLBMkrSort->GetItemDataPtr(idxMkrSort);
        ASSERT(pMkrFirst != NULL);
        // I may have to do a total reload of the Sort Order Cbx.
        pCbx->ResetContent();
        
        // The new primary marker adopts its language encoding's default
        // sort order.
        m_psrtPri = pMkrFirst->plng()->psrtDefault();
        ASSERT(m_psrtPri != NULL);      
        LoadSortOrdersCbx();
        }
    
    // Reload list of available markers
    LoadLBMkrsAvailable(pmkr->sName());

    if (pLBMkrSort->GetCount() <= 0)
        {
        pCbx->SetCurSel(-1);        // Blank the selection.
        pCbx->EnableWindow(FALSE);
        }
    pLBMkrSort->SetFocus(); // set focus back to list box so they can type another marker
}


// Add the currently selected item from the sort fields list to the
// marker list.
void CSortDlg::OnSortindexMoveto()
{
    int idx, iTmp;
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
        
    int idxMkrList = pLBMkr->GetCurSel();
    if (idxMkrList == LB_ERR)
        return;
        
    int idxMkrSort = pLBMkrSort->GetCurSel();
    if (idxMkrSort == LB_ERR)
        idxMkrSort = pLBMkrSort->GetCount();    // Insert at beginning.
    else
        idxMkrSort++;   // Forces insertion after highlight.
        
    // Remove marker from the markers list.
    CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
    iTmp = pLBMkr->DeleteString(idxMkrList);
    // Select next item.
    iTmp = pLBMkr->SetCurSel( 
            (idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1); 
    
    // Stick it in the list of fields on which to sort.
    idx = pLBMkrSort->InsertString(idxMkrSort, swUTF16( pmkr->sName() ) ); // 1.4qre
    iTmp = pLBMkrSort->SetItemDataPtr(idx, (void*)pmkr);
    pLBMkrSort->SetCurSel(idx); // Make that the current selection.

    // If the sort list just received its first element, update the sort orders cbx.
    if (pLBMkrSort->GetCount() == 1)
        {
        pCbx->ResetContent();
        // The new primary marker adopts its language encoding's default
        // sort order.
        m_psrtPri = pmkr->plng()->psrtDefault();
        ASSERT(m_psrtPri != NULL);      
        LoadSortOrdersCbx();
        }

    if (pCbx->GetCount() > 0)
        pCbx->EnableWindow(TRUE);
    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CSortDlg::OnCancel()
{
    // TODO: Add extra cleanup here
    
    CDialog::OnCancel();
}

void CSortDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
    
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
        
    // The first field in the sorted fields list is the primary field always.
    if (pLBMkrSort->GetCount() <= 0)
        {
        CIndex* pind = m_pindset->pindRecordOwner();
        CMarker* pmkr = pind->pmkrPriKey();
        *m_ppind = *m_ppindUnfiltered = pind;
        }
    else
        {
        m_pmkrPriKey = (CMarker*)pLBMkrSort->GetItemDataPtr(0);
     
        // The sort order for the primary sort field comes from the combo box.
        int idxSortOrd = pCbx->GetCurSel();
        if (idxSortOrd != CB_ERR)
            m_psrtPri = (CSortOrder*)pCbx->GetItemDataPtr(idxSortOrd);
        ASSERT( m_pmkrPriKey->plng()->psrtset()->bIsMember(m_psrtPri) );

        CMarker* pmkr;
        for (int idxThisMkr = 1; idxThisMkr < pLBMkrSort->GetCount(); idxThisMkr++)
            {
            pmkr = (CMarker*)pLBMkrSort->GetItemDataPtr(idxThisMkr);
            m_mrflstUsersChoice.AddMarkerRef(pmkr);
            }
        
        m_pindset->GetIndex(m_pmkrPriKey, m_psrtPri,
            &m_mrflstUsersChoice, (Length)m_iSortKeyLen, m_bSortFromEnd,
            m_pfil, m_ppind, m_ppindUnfiltered);
        }   
    
    CDialog::OnOK();
}

void CSortDlg::OnDblclkSortindexLstmrkstosort()
{
    // TODO: Add your control notification handler code here
    OnSortindexMovefrom();
}

void CSortDlg::OnDblclkSortindexLstmrksavailable()
{
    // TODO: Add your control notification handler code here
    OnSortindexMoveto();
}

void CSortDlg::OnSortindexFirstto()
{
    int idx, iTmp;
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
        
    int idxMkrList = pLBMkr->GetCurSel();
    if (idxMkrList == LB_ERR)
        return;
        
    // Remove marker from the markers list.
    CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
    iTmp = pLBMkr->DeleteString(idxMkrList);
    // Select next item.
    iTmp = pLBMkr->SetCurSel( 
            (idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1); 
    
    // Stick it in the list of fields on which to sort.
    idx = pLBMkrSort->InsertString(0, swUTF16( pmkr->sName() ) ); // 1.4qre
    if (idx == LB_ERR)
        return;
    iTmp = pLBMkrSort->SetItemDataPtr(idx, (void*)pmkr);
    pLBMkrSort->SetCurSel(idx); // Make that the current selection.
    
    // Since there is a new first item
    // change the sort order combo box to its default.
    pCbx->ResetContent();
    // The new primary marker adopts its language encoding's default
    // sort order.
    m_psrtPri = pmkr->plng()->psrtDefault();
    ASSERT(m_psrtPri != NULL);      
    LoadSortOrdersCbx();

    if (pCbx->GetCount() > 0)
        pCbx->EnableWindow(TRUE);
    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CSortDlg::OnSortindexLastto()
{
    int idx, iTmp;
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
        
    int idxMkrList = pLBMkr->GetCurSel();
    if (idxMkrList == LB_ERR)
        return;
        
    // Remove marker from the markers list.
    CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
    iTmp = pLBMkr->DeleteString(idxMkrList);
    // Select next item.
    iTmp = pLBMkr->SetCurSel( 
            (idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1); 
    
    // Stick it in the list of fields on which to sort.
    idx = pLBMkrSort->InsertString(pLBMkrSort->GetCount(), swUTF16( pmkr->sName() ) ); // 1.4qre
    if (idx != LB_ERR)
        {
        iTmp = pLBMkrSort->SetItemDataPtr(idx, (void*)pmkr);
        pLBMkrSort->SetCurSel(idx); // Make that the current selection.
        }
        
    // If the sort list just received its first element, update the sort orders cbx.
    if (pLBMkrSort->GetCount() == 1)
        {
        pCbx->ResetContent();
        // The new primary marker adopts its language encoding's default
        // sort order.
        m_psrtPri = pmkr->plng()->psrtDefault();
        ASSERT(m_psrtPri != NULL);      
        LoadSortOrdersCbx();
        }
        
    if (pCbx->GetCount() > 0)
        pCbx->EnableWindow(TRUE);
    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CSortDlg::OnSortindexClearsorts()
{
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrSort = (CListBox*)GetDlgItem(IDC_SORTINDEX_LSTMRKSTOSORT);
    ASSERT(pLBMkrSort != NULL); 
    CComboBox* pCbx = (CComboBox*)GetDlgItem(IDC_SORTINDEX_CBXFIRSTFLD_SO);
    ASSERT( pCbx != NULL );
    
    // Clear out the sort fields list.
    pLBMkrSort->ResetContent();
    ASSERT(pLBMkrSort->GetCount() == 0);
    
    // Also disable combo box while no primary sort field available.
    pCbx->SetCurSel(-1);        // Blank the selection.
    pCbx->EnableWindow(FALSE);
    
    LoadLBMkrsAvailable(); // reload available markers listbox

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}
