// hide_d.cpp : implementation file

#include "stdafx.h"
#include "toolbox.h"
#include "hide_d.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHideFieldsDlg dialog


CHideFieldsDlg::CHideFieldsDlg( CMarkerSet* pmkrset ) : CDialog(CHideFieldsDlg::IDD)
{
	m_pmkrset = pmkrset;
	ASSERT( m_pmkrset );

	//{{AFX_DATA_INIT(CHideFieldsDlg)
        //09-23-1997
    //}}AFX_DATA_INIT
}

void CHideFieldsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHideFieldsDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHideFieldsDlg, CDialog)
	//{{AFX_MSG_MAP(CHideFieldsDlg)
	ON_BN_CLICKED(IDC_HIDEFIELDS_MOVEFROM, OnHideFieldMovefrom)
	ON_BN_CLICKED(IDC_HIDEFIELDS_MOVETO, OnHideFieldMoveto)
	ON_LBN_DBLCLK(IDC_HIDEFIELDS_LSTFLDSTOHIDE, OnDblclkHideFieldLstfldstoHide)
	ON_LBN_DBLCLK(IDC_HIDEFIELDS_LSTFLDSAVAILABLE, OnDblclkHideFieldLstfldsavailable)
	ON_BN_CLICKED(IDC_HIDEFIELDS_FIRSTTO, OnHideFieldFirstto)
	ON_BN_CLICKED(IDC_HIDEFIELDS_LASTTO, OnHideFieldLastto)
	ON_BN_CLICKED(IDC_HIDEFIELDS_CLEARHIDE, OnHideFieldClearHide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CHideFieldsDlg message handlers

BOOL CHideFieldsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int	idx, iTmp;
	Str8	sLBItem;
	
  	// Load the fields Hided list.
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	
	
	// Load up the hide fields listbox
	for ( CMarker* pmkr = m_pmkrset->pmkrFirst(); pmkr; pmkr = m_pmkrset->pmkrNext( pmkr ) )
		{
		if ( pmkr->bHidden() ) // If hidden field, add it to hidden fields list
			{
			sLBItem = pmkr->sMarker();
			idx = pLBMkrHide->InsertString(-1, swUTF16( sLBItem ) ); // 1.4qre
			iTmp = pLBMkrHide->SetItemDataPtr(idx, (void*)pmkr);
			}
		}
/*	
	for ( CHideField* pbrf = m_pbrflst->pbrfFirst();
			pbrf != NULL;	pbrf = m_pbrflst->pbrfNext( pbrf ) )
		{
		const CMarker* pmkr = pbrf->pmkr();
		// Add it to the fields hided list in that order.
		sLBItem = pmkr->sMarker();
		idx = pLBMkrHide->InsertString(-1, sLBItem);
		iTmp = pLBMkrHide->SetItemDataPtr(idx, (void*)pmkr);
		}
*/		
	// Set the current selection to the first item (primary key).
	pLBMkrHide->SetCurSel(0);
	
	// Load all available markers into other listbox
	LoadLBMkrsAvailable();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHideFieldsDlg::LoadLBMkrsAvailable( const char* pszMrkSel )
{
    Str8 sLBItem;
    int idx, iTmp;
    
    // Load up the available markers list.
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
    ASSERT(pLBMkrHide != NULL); 
    
	pLBMkr->ResetContent();

	// first put all markers into markers available listbox
    for (CMarker* pmkr = m_pmkrset->pmkrFirst(); pmkr != NULL; pmkr = m_pmkrset->pmkrNext(pmkr))
    	{
	    // Add to the markers available listbox
	    idx = pLBMkr->InsertString(-1, swUTF16( pmkr->sMarker() ) ); // 1.4qre
	    iTmp = pLBMkr->SetItemDataPtr(idx,(void*)pmkr);
    	}   

	// remove markers from markers available list that are in sorted listbox
	int iSortItems = pLBMkrHide->GetCount(); // get number of items in sorted listbox
	for (int i=0; i < iSortItems; i++)
		{
//		char buffer[DLGBUFMAX+1]; // 1.4qvd
		CString swText; // 1.4qvd Upgrade GetText for Unicode build
	    pLBMkrHide->GetText( i, swText ); // retrieve marker text // 1.4ed Change listbox get text to not use CStrg // 1.4qvn
        idx = pLBMkr->FindStringExact(-1, swText); // 1.4qve Upgrade FindStringExact for Unicode build
        if (idx != LB_ERR) // is it in sorted list?
			pLBMkr->DeleteString(idx); // if so, delete it from available list
		}
	idx = 0;
	if (pszMrkSel)
		{
		Str8 s = pszMrkSel; // 1.4qve
		CString sw = swUTF16( s ); // 1.4qve
		idx = pLBMkr->FindStringExact(-1, sw); // try to set selection to specified marker // 1.4qve
		}
	if (idx == LB_ERR)
    	idx = 0;
    pLBMkr->SetCurSel(idx);
}

void CHideFieldsDlg::OnHideFieldMovefrom()
{
	int iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	

	int	idxMkrHide = pLBMkrHide->GetCurSel();
	if (idxMkrHide == LB_ERR)
		return;
	
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		idxMkrList = pLBMkr->GetCount() - 1;	// Inserts after last element.
		
	// Remove marker from the Hide field markers list.
	CMarker* pmkr = (CMarker*)pLBMkrHide->GetItemDataPtr(idxMkrHide);
	iTmp = pLBMkrHide->DeleteString(idxMkrHide);
	idxMkrHide = (idxMkrHide < pLBMkrHide->GetCount()) ? idxMkrHide : idxMkrHide-1;
	// Select next item.
	iTmp = pLBMkrHide->SetCurSel(idxMkrHide);
	
    // Reload list of available markers
	LoadLBMkrsAvailable(pmkr->sName());

    pLBMkrHide->SetFocus(); // set focus back to list box so they can type another marker
}


// Add the currently selected item from the Hide fields list to the
// marker list.
void CHideFieldsDlg::OnHideFieldMoveto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	int	idxMkrHide = pLBMkrHide->GetCurSel();
	if (idxMkrHide == LB_ERR)
		idxMkrHide = pLBMkrHide->GetCount();	// Insert at beginning.
	else
		idxMkrHide++;	// Forces insertion after highlight.
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Hide.
	idx = pLBMkrHide->InsertString(idxMkrHide, swUTF16( pmkr->sName() ) ); // 1.4qre
	iTmp = pLBMkrHide->SetItemDataPtr(idx, (void*)pmkr);
	pLBMkrHide->SetCurSel(idx);	// Make that the current selection.

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CHideFieldsDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CHideFieldsDlg::OnOK()
{
	UpdateData(TRUE);  // from dialog controls to dialog object member data
	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	

	CMarker* pmkr = m_pmkrset->pmkrFirst();
	for ( ; pmkr; pmkr = m_pmkrset->pmkrNext( pmkr ) )
		pmkr->SetHidden( FALSE ); // Clear hidden on all markers
	
	for (int idxThisMkr = 0; idxThisMkr < pLBMkrHide->GetCount(); idxThisMkr++)
		{
		pmkr = (CMarker*)pLBMkrHide->GetItemDataPtr(idxThisMkr);
		CMarker* pmkrHide = m_pmkrset->pmkrSearch( pmkr->sName() );
		if ( pmkrHide )
			pmkrHide->SetHidden( TRUE );
		}

	CDialog::OnOK();
}

void CHideFieldsDlg::OnDblclkHideFieldLstfldstoHide()
{
	// TODO: Add your control notification handler code here
	OnHideFieldMovefrom();
}

void CHideFieldsDlg::OnDblclkHideFieldLstfldsavailable()
{
	// TODO: Add your control notification handler code here
	OnHideFieldMoveto();
}

void CHideFieldsDlg::OnHideFieldFirstto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Hide.
	idx = pLBMkrHide->InsertString(0, swUTF16( pmkr->sName() ) ); // 1.4qre
	if (idx == LB_ERR)
		return;
	iTmp = pLBMkrHide->SetItemDataPtr(idx, (void*)pmkr);
	pLBMkrHide->SetCurSel(idx);	// Make that the current selection.

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CHideFieldsDlg::OnHideFieldLastto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Hide.
	idx = pLBMkrHide->InsertString(pLBMkrHide->GetCount(), swUTF16( pmkr->sName() ) ); // 1.4qre
	if (idx != LB_ERR)
		{
		iTmp = pLBMkrHide->SetItemDataPtr(idx, (void*)pmkr);
		pLBMkrHide->SetCurSel(idx);	// Make that the current selection.
		}

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CHideFieldsDlg::OnHideFieldClearHide()
{
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrHide = (CListBox*)GetDlgItem(IDC_HIDEFIELDS_LSTFLDSTOHIDE);
	ASSERT(pLBMkrHide != NULL);	
	
	// Clear out the Hide fields list.
	pLBMkrHide->ResetContent();
	ASSERT(pLBMkrHide->GetCount() == 0);
	
	// Also clear the available markers.
	pLBMkr->ResetContent();
	
	LoadLBMkrsAvailable(); // reload available markers listbox

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}


