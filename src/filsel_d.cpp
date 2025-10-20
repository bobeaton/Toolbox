// filsel_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "fil.h"
#include "filsel_d.h"

#ifdef _MAC
#include "resmac.h" // for IDC_FILSEL_FILTERCOMBO
#else
#include "respc.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CFilterSetComboBox, CComboBox)
	//{{AFX_MSG_MAP(CComboBox)
    ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
#ifdef _MAC
	ON_WM_LBUTTONDOWN()
#endif
END_MESSAGE_MAP()

CFilterSetComboBox::CFilterSetComboBox(CFilterSet* pfilset) :
CSetComboBox(pfilset, TRUE, NULL, _("[no filter]")) {} // 1.4gh

void CFilterSetComboBox::UpdateElements(CFilter* pfilToSelect)
{
	CSetComboBox::UpdateElements(pfilToSelect);
}

void CFilterSetComboBox::ChangeSet(CFilterSet* pfilset, CFilter* pfilToSelect, BOOL bForce)
{
	if (bHasChanged(pfilset) || pfilToSelect != pfilSelected())
		CSetComboBox::ChangeSet( pfilset, pfilToSelect);
}

BOOL CFilterSetComboBox::bHasChanged(CFilterSet* pfilset) // see if filter set has changed
{
	if (pfilset->lGetCount() + 1 != GetCount()) // different number of items in list?
		return TRUE;
	// Verify all filter set members are in combo list
	for (int i=1; i<GetCount(); i++)
		{
		CFilter* pfil = (CFilter*)GetItemData(i);
		if (!pfilset->bIsMember(pfil))
			return TRUE;
		// Also see if the name only has changed.  Name changes are not
		// reflected in the list box of the combo box because we the filter
		// set combo box is not owner draw.
		// A better solution may be to owner-draw the filter name.
//		char buffer[DLGBUFMAX+1]; // 1.4qvg
		CString swText; // 1.4qvg Upgrade GetLBText for Unicode build
		GetLBText( i, swText ); // 1.4ed Change listbox get text to not use CStrg // 1.4qvn
		Str8 sText =  sUTF8( swText ); // 1.4qvg
		if ( sText != pfil->sName() ) // 1.4hem Change Compare to Cmp for Str8 // 1.4qzgc
			return TRUE;
		}
	return FALSE; // set hasn't changed
}

// combo box doesn't send double-click to parent unless it is a CBS_SIMPLE combo
// so we have to force it to!
// BJY 6/21/96 Change to use right click instead of double click
afx_msg void CFilterSetComboBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage( WM_COMMAND, (WPARAM)MAKELONG(IDC_FILSEL_FILTERCOMBO, CBN_DBLCLK),
								(LPARAM)m_hWnd );
}

#ifdef _MAC
afx_msg void CFilterSetComboBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetKeyState(VK_OPTION) & 0x8000) // Ctrl-LButtonClk means modify current filter
		OnRButtonDown(nFlags, point);
	else
		CSetComboBox::OnLButtonDown(nFlags, point);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CFilterSelectionDlg dialog


CFilterSelectionDlg::CFilterSelectionDlg(CFilterSet* pfilset, CFilter** ppfil)
	: CDialog(CFilterSelectionDlg::IDD, NULL),
	m_cboFilterSet(pfilset)
{
	m_pfilset = pfilset;
	m_ppfil = ppfil;
	ASSERT( m_pfilset );
	ASSERT( m_ppfil );

	//{{AFX_DATA_INIT(CFilterSelectionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFilterSelectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterSelectionDlg)
	DDX_Control(pDX, IDC_FILTER_LIST, m_cboFilterSet);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterSelectionDlg, CDialog)
	//{{AFX_MSG_MAP(CFilterSelectionDlg)
	ON_BN_CLICKED(IDC_FILSEL_FILTERS, OnFilselFilters)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFilterSelectionDlg message handlers

BOOL CFilterSelectionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_cboFilterSet.UpdateElements(*m_ppfil);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFilterSelectionDlg::OnFilselFilters()
{
	CFilter* pfil = m_cboFilterSet.pfilSelected();
	if ( m_pfilset->bModalView_Elements(pfil) ) // go to filter set page
		{
		if ( pfil && !m_pfilset->bIsMember(pfil) ) // make sure not deleted
			pfil = NULL;
		m_cboFilterSet.UpdateElements(pfil);
		}
}

void CFilterSelectionDlg::OnOK()
{
	*m_ppfil = m_cboFilterSet.pfilSelected(); // return selected filter to caller
	
	CDialog::OnOK();
}

