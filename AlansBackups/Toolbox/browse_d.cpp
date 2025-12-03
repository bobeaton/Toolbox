// browse_d.cpp : implementation file
// Brian Yoder  10-2-95

#include "stdafx.h"
#include "toolbox.h"
#include "browse_d.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowseFieldsDlg dialog


CBrowseFieldsDlg::CBrowseFieldsDlg( CMarkerSet* pmkrset, CBrowseFieldList* pbrflst ) : CDialog(CBrowseFieldsDlg::IDD)
{
	m_pmkrset = pmkrset;
	ASSERT( m_pmkrset );

	m_pbrflst = pbrflst;
	ASSERT( m_pbrflst );

	//{{AFX_DATA_INIT(CBrowseFieldsDlg)
        //09-23-1997
    m_sNoField = pbrflst->GetNoFieldEntry();
    m_sEmptyField = pbrflst->GetFieldEmptyEntry();
    //}}AFX_DATA_INIT
}

void CBrowseFieldsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrowseFieldsDlg)
//    DDX_Txt(pDX, IDC_BrowseEmptyField, m_sEmptyField);  // 1.4dw
//    DDX_Txt(pDX, IDC_BrowseNoField, m_sNoField);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBrowseFieldsDlg, CDialog)
	//{{AFX_MSG_MAP(CBrowseFieldsDlg)
	ON_BN_CLICKED(IDC_BROWSEFIELDS_MOVEFROM, OnBrowseFieldMovefrom)
	ON_BN_CLICKED(IDC_BROWSEFIELDS_MOVETO, OnBrowseFieldMoveto)
	ON_LBN_DBLCLK(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE, OnDblclkBrowseFieldLstfldstoBrowse)
	ON_LBN_DBLCLK(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE, OnDblclkBrowseFieldLstfldsavailable)
	ON_BN_CLICKED(IDC_BROWSEFIELDS_FIRSTTO, OnBrowseFieldFirstto)
	ON_BN_CLICKED(IDC_BROWSEFIELDS_LASTTO, OnBrowseFieldLastto)
	ON_BN_CLICKED(IDC_BROWSEFIELDS_CLEARBROWSE, OnBrowseFieldClearBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBrowseFieldsDlg message handlers

BOOL CBrowseFieldsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int	idx, iTmp;
	Str8	sLBItem;
	
  	// Load the fields Browsed list.
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
	
	// Load up the browse fields listbox
	for ( CBrowseField* pbrf = m_pbrflst->pbrfFirst();
			pbrf != NULL;	pbrf = m_pbrflst->pbrfNext( pbrf ) )
		{
		const CMarker* pmkr = pbrf->pmkr();
		// Add it to the fields browsed list in that order.
		sLBItem = pmkr->sMarker();
		idx = pLBMkrBrowse->InsertString(-1, swUTF16( sLBItem ) ); // 1.4qre
		iTmp = pLBMkrBrowse->SetItemDataPtr(idx, (void*)pmkr);
		}
		
	// Set the current selection to the first item (primary key).
	pLBMkrBrowse->SetCurSel(0);
	
	// Load all available markers into other listbox
	LoadLBMkrsAvailable();

    SetDlgItemText( IDC_BrowseEmptyField, swUTF16( m_sEmptyField ) );  // 1.4dw // 1.4qpv
    SetDlgItemText( IDC_BrowseNoField, swUTF16( m_sNoField ) );  //09-23-1997 // 1.4qpv

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBrowseFieldsDlg::LoadLBMkrsAvailable( const char* pszMrkSel )
{
    Str8 sLBItem;
    int idx, iTmp;
    
    // Load up the available markers list.
    CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
    ASSERT(pLBMkr != NULL); 
    CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
    ASSERT(pLBMkrBrowse != NULL); 
    
	pLBMkr->ResetContent();

	// first put all markers into markers available listbox
    for (CMarker* pmkr = m_pmkrset->pmkrFirst(); pmkr != NULL; pmkr = m_pmkrset->pmkrNext(pmkr))
    	{
	    // Add to the markers available listbox
	    idx = pLBMkr->InsertString(-1, swUTF16( pmkr->sMarker() ) ); // 1.4qre
	    iTmp = pLBMkr->SetItemDataPtr(idx,(void*)pmkr);
    	}   

	// remove markers from markers available list that are in sorted listbox
	int iSortItems = pLBMkrBrowse->GetCount(); // get number of items in sorted listbox
	for (int i=0; i < iSortItems; i++)
		{
//		char buffer[DLGBUFMAX+1]; // 1.4qvk
		CString swText; // 1.4qvk Upgrade GetText and FindStringExact for Unicode build
	    pLBMkrBrowse->GetText( i, swText ); // retrieve marker text // 1.4ed Change listbox get text to not use CStrg // 1.4qvn Upgrade GetText for Unicode build
        idx = pLBMkr->FindStringExact(-1, swText); // 1.4qvk
        if (idx != LB_ERR) // is it in sorted list?
			pLBMkr->DeleteString(idx); // if so, delete it from available list
		}
	idx = 0;
	if (pszMrkSel)
		{
		Str8 s = pszMrkSel; // 1.4qvk
		CString sw = swUTF16( s ); // 1.4qvk
		idx = pLBMkr->FindStringExact(-1, sw); // try to set selection to specified marker // 1.4qvk
		}
	if (idx == LB_ERR)
    	idx = 0;
    pLBMkr->SetCurSel(idx);
}

void CBrowseFieldsDlg::OnBrowseFieldMovefrom()
{
	int iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	

	int	idxMkrBrowse = pLBMkrBrowse->GetCurSel();
	if (idxMkrBrowse == LB_ERR)
		return;
	
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		idxMkrList = pLBMkr->GetCount() - 1;	// Inserts after last element.
		
	// Remove marker from the Browse field markers list.
	CMarker* pmkr = (CMarker*)pLBMkrBrowse->GetItemDataPtr(idxMkrBrowse);
	iTmp = pLBMkrBrowse->DeleteString(idxMkrBrowse);
	idxMkrBrowse = (idxMkrBrowse < pLBMkrBrowse->GetCount()) ? idxMkrBrowse : idxMkrBrowse-1;
	// Select next item.
	iTmp = pLBMkrBrowse->SetCurSel(idxMkrBrowse);
	
    // Reload list of available markers
	LoadLBMkrsAvailable(pmkr->sName());

    pLBMkrBrowse->SetFocus(); // set focus back to list box so they can type another marker
}


// Add the currently selected item from the Browse fields list to the
// marker list.
void CBrowseFieldsDlg::OnBrowseFieldMoveto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	int	idxMkrBrowse = pLBMkrBrowse->GetCurSel();
	if (idxMkrBrowse == LB_ERR)
		idxMkrBrowse = pLBMkrBrowse->GetCount();	// Insert at beginning.
	else
		idxMkrBrowse++;	// Forces insertion after highlight.
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Browse.
	idx = pLBMkrBrowse->InsertString(idxMkrBrowse, swUTF16( pmkr->sName() ) ); // 1.4qre
	iTmp = pLBMkrBrowse->SetItemDataPtr(idx, (void*)pmkr);
	pLBMkrBrowse->SetCurSel(idx);	// Make that the current selection.

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CBrowseFieldsDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CBrowseFieldsDlg::OnOK()
{
	UpdateData(TRUE);  // from dialog controls to dialog object member data
	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
		
	m_pbrflst->DeleteAll(); // trash old field list

	CMarker* pmkr;
	for (int idxThisMkr = 0; idxThisMkr < pLBMkrBrowse->GetCount(); idxThisMkr++)
		{
		pmkr = (CMarker*)pLBMkrBrowse->GetItemDataPtr(idxThisMkr);
		m_pbrflst->Add( pmkr );
		}

//    GetDlgItemText( IDC_BrowseEmptyField, (char*)buffer, DLGBUFMAX ); // 1.4dw
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_BrowseEmptyField, sDlgItem ); // 1.4qpw
	m_sEmptyField = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_BrowseNoField, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_BrowseNoField, sDlgItem ); // 1.4qpw
	m_sNoField = sUTF8( sDlgItem ); // 1.4qpw

	//09-23-1997
    m_pbrflst->SetFieldEmptyEntry(m_sEmptyField);
    m_pbrflst->SetNoFieldEntry(m_sNoField);

	CDialog::OnOK();
}

void CBrowseFieldsDlg::OnDblclkBrowseFieldLstfldstoBrowse()
{
	// TODO: Add your control notification handler code here
	OnBrowseFieldMovefrom();
}

void CBrowseFieldsDlg::OnDblclkBrowseFieldLstfldsavailable()
{
	// TODO: Add your control notification handler code here
	OnBrowseFieldMoveto();
}

void CBrowseFieldsDlg::OnBrowseFieldFirstto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Browse.
	idx = pLBMkrBrowse->InsertString(0, swUTF16( pmkr->sName() ) ); // 1.4qre
	if (idx == LB_ERR)
		return;
	iTmp = pLBMkrBrowse->SetItemDataPtr(idx, (void*)pmkr);
	pLBMkrBrowse->SetCurSel(idx);	// Make that the current selection.

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CBrowseFieldsDlg::OnBrowseFieldLastto()
{
	int idx, iTmp;
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
		
	int	idxMkrList = pLBMkr->GetCurSel();
	if (idxMkrList == LB_ERR)
		return;
		
	// Remove marker from the markers list.
	CMarker* pmkr = (CMarker*)pLBMkr->GetItemDataPtr(idxMkrList);
	iTmp = pLBMkr->DeleteString(idxMkrList);
	// Select next item.
	iTmp = pLBMkr->SetCurSel( 
			(idxMkrList < pLBMkr->GetCount()) ? idxMkrList : idxMkrList-1);	
	
	// Stick it in the list of fields on which to Browse.
	idx = pLBMkrBrowse->InsertString(pLBMkrBrowse->GetCount(), swUTF16( pmkr->sName() ) ); // 1.4qre
	if (idx != LB_ERR)
		{
		iTmp = pLBMkrBrowse->SetItemDataPtr(idx, (void*)pmkr);
		pLBMkrBrowse->SetCurSel(idx);	// Make that the current selection.
		}

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}

void CBrowseFieldsDlg::OnBrowseFieldClearBrowse()
{
	CListBox* pLBMkr = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSAVAILABLE);
	ASSERT(pLBMkr != NULL);	
	CListBox* pLBMkrBrowse = (CListBox*)GetDlgItem(IDC_BROWSEFIELDS_LSTFLDSTOBROWSE);
	ASSERT(pLBMkrBrowse != NULL);	
	
	// Clear out the Browse fields list.
	pLBMkrBrowse->ResetContent();
	ASSERT(pLBMkrBrowse->GetCount() == 0);
	
	// Also clear the available markers.
	pLBMkr->ResetContent();
	
	LoadLBMkrsAvailable(); // reload available markers listbox

    pLBMkr->SetFocus(); // set focus back to list box so they can type another marker
}


