// srtset_d.cpp  Dialog tabbed page representing a set of sort orders

#include "stdafx.h"
#include "toolbox.h"
#include "srtset_d.h"

// **************************************************************************

CSortOrderSetPage::CSortOrderSetPage(CSortOrderSetListBox& lbosrtset,
		CSortOrderSet* psrtset) :
	CSetPage(lbosrtset, IDS_SRTSET_TITLE, IDD_SRTSET_PAGE), // 1.4et OK, doesn't use CStrg
	m_psrtset(psrtset),
	m_cbosrtDefault(m_psrtset)
{
	ASSERT( m_psrtset );
}

void CSortOrderSetPage::DoDataExchange(CDataExchange* pDX)
{
	CSetPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortOrderSetPage)
	DDX_Control(pDX, IDC_srtset_cbosrtDefault, m_cbosrtDefault);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSortOrderSetPage, CSetPage)
	//{{AFX_MSG_MAP(CSortOrderSetPage)
	ON_CBN_SELCHANGE(IDC_srtset_cbosrtDefault, OnSelchangeDefaultSortOrderCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSortOrderSetPage::OnInitDialog()
{
	CSetPage::OnInitDialog();
	
	CSortOrder* psrtDefault = m_psrtset->psrtDefault();
	ASSERT( psrtDefault );
	m_cbosrtDefault.UpdateElements(psrtDefault);

	return TRUE;  // input focus will be set to the dialog's first control
}

void CSortOrderSetPage::OnSelchangeDefaultSortOrderCombo()
{
	CSortOrder* psrtDefault = m_cbosrtDefault.psrtSelected();
	ASSERT( psrtDefault );
	m_psrtset->SetDefaultSortOrder(psrtDefault);
	
	CSetEl* psel = m_lboSet.pselSelected();
	m_lboSet.UpdateElements(psel);
	// Must prevent selected default sort order from being deleted
	UpdateDeleteButton();
}

void CSortOrderSetPage::UpdateOtherControls()
{
	// In case the name of the selected element has been modified.
	CSortOrder* psrt = m_cbosrtDefault.psrtSelected();
	m_cbosrtDefault.UpdateElements(psrt);
}

// **************************************************************************
