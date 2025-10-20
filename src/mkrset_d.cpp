// mkrset_d.cpp  Dialog tabbed page representing a set of markers

#include "stdafx.h"
#include "toolbox.h"
#include "mkrset_d.h"


// **************************************************************************

// 1997-12-15 MRP: Emergency correction for problem with merging markers
// via Modify from the Markers tab in Database Type Properties.
// See lbo.cpp, mkr_d.cpp, mkrset_d.h, mkrset_d.cpp

extern CSetListBox* g_plboMarkers;

CMarkerSetPage::CMarkerSetPage(CMarkerSetListBox& lbomkrset,
        CMarkerSet* pmkrset) :
    CSetPage(lbomkrset, IDS_MKRSET_TITLE, IDD_mkrset_Page), // 1.4et OK, doesn't use CStrg
    m_pmkrset(pmkrset),
    m_cbolngDefault(m_pmkrset->plngDefault()->plngsetMyOwner())
{
    ASSERT( m_pmkrset );
    ASSERT( !g_plboMarkers );
    g_plboMarkers = &lbomkrset;
}

CMarkerSetPage::~CMarkerSetPage()
{
    g_plboMarkers = NULL;
}

void CMarkerSetPage::DoDataExchange(CDataExchange* pDX)
{
    CSetPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMarkerSetPage)
    DDX_Control(pDX, IDC_mkrset_cbolngDefault, m_cbolngDefault);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarkerSetPage, CSetPage)
    //{{AFX_MSG_MAP(CMarkerSetPage)
    ON_CBN_SELCHANGE(IDC_mkrset_cbolngDefault, OnSelchangeDefaultLangEncCombo)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CMarkerSetPage::OnInitDialog()
{
    CSetPage::OnInitDialog();
    
    CLangEnc* plngDefault = m_pmkrset->plngDefault();
    ASSERT( plngDefault );
    m_cbolngDefault.UpdateElements(plngDefault);

    return TRUE;  // input focus will be set to the dialog's first control
}

void CMarkerSetPage::OnSelchangeDefaultLangEncCombo()
{
    CLangEnc* plngDefault = m_cbolngDefault.plngSelected();
    ASSERT( plngDefault );
    m_pmkrset->SetDefaultLangEnc(plngDefault);
}

// **************************************************************************
