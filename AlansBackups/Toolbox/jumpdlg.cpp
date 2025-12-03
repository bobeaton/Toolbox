// jumpdlg.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "recset.h"  // CKey
#include "jumpdlg.h"
#include "jmp.h" // CJumpPath
#include "ddx.h"
#include "typ.h"
#include "typ_d.h" // Needed for page ID for opening DB Type properties sheet.

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJumpDlg dialog

CJumpDlg::CJumpDlg( CKey* pkey, CDatabaseType* ptyp, CLangEnc** pplng, CJumpPath** ppjmp, BOOL bIsJumpInsert )
	: CDialog(CJumpDlg::IDD, NULL),
	m_cboLangEncSet(ptyp->ptrxMyProxy()->ptypsetMyOwner()->plngset()),
	m_cboPriJumpPath(ptyp->pjmpset()),
    m_bLngEncChangedByUser(FALSE),
	m_bIsJumpInsert(bIsJumpInsert),
	m_pkey(pkey),
  	m_ptyp(ptyp),
	m_pplng(pplng),
	m_ppjmp(ppjmp)
{
	ASSERT( m_pkey );
	m_sKey = m_pkey->sKey();
	if (m_bIsJumpInsert)
		SetHelpID(IDD_JUMPINSERT);
	ASSERT( m_pplng );
    m_plng = *m_pplng;
	ASSERT( m_plng );
    ASSERT( m_ppjmp );
    m_pjmp = *m_ppjmp;

	m_bMatchWhole = m_pkey->bMatchWhole();
    SetMatchSetting(m_pkey->matset());

	//{{AFX_DATA_INIT(CJumpDlg)
	//}}AFX_DATA_INIT
}

void CJumpDlg::SetMatchSetting(MatchSetting matset)
{
	m_matset = matset;
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

void CJumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGSET_COMBO, m_cboLangEncSet);
	DDX_Control(pDX, IDC_jmp_cboPriJumpPath, m_cboPriJumpPath);
	DDX_Control(pDX, IDC_JUMP_KEY, m_elcJumpKey);
//	Shw_DDX_TrimmedTxt(pDX, IDC_JUMP_KEY, m_sKey); // 1.4gu Replace DDX_Trimmed in jump dialog
	//{{AFX_DATA_MAP(CJumpDlg)
	DDX_Check(pDX, IDC_JUMP_WHOLE_KEY, m_bMatchWhole);
	DDX_Radio(pDX, IDC_JUMP_MATCH_SAME_PRIMARY, m_iMatchSetting);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CJumpDlg, CDialog)
	//{{AFX_MSG_MAP(CJumpDlg)
	ON_BN_CLICKED(IDC_JUMPPATH, OnJumppath)
	ON_CBN_SELCHANGE(IDC_LANGSET_COMBO, OnSelchangeLangsetCombo)
	ON_CBN_SELCHANGE(IDC_jmp_cboPriJumpPath, OnSelchangePriJumpPathCombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CJumpDlg message handlers

BOOL CJumpDlg::OnInitDialog()
{
	Str8 sTitle;
	if ( m_bIsJumpInsert )
		{
		sTitle = _("Jump Insert");
		SetWindowText(  swUTF16( sTitle ) ); // 1.4qpy
		sTitle = _("&Record to Insert:");
		CStatic* plbl = (CStatic*)GetDlgItem( IDC_JUMPKEY_LABEL );
		ASSERT(plbl);
		plbl->SetWindowText(  swUTF16( sTitle ) ); // 1.4qpy
        }
	CDialog::OnInitDialog();
	m_elcJumpKey.SetLangEnc(m_plng);
	m_elcJumpKey.SetEditLngText( m_sKey ); // 1.4qpn
	m_cboLangEncSet.UpdateElements(m_plng);
    m_cboPriJumpPath.UpdateElements(m_pjmp);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CJumpDlg::OnJumppath()
{
    m_ptyp->bViewProperties(CDatabaseTypeSheet::eJumpPathPage);
    m_cboPriJumpPath.UpdateElements(m_pjmp);
}

void CJumpDlg::OnOK()
{
	UpdateData(TRUE);
	m_sKey = m_elcJumpKey.sGetEditLngText(); // 1.4qpn
	m_sKey.TrimLeft(); // 1.4gu
	m_sKey.TrimRight(); // 1.4gu
	*m_pplng = m_plng;
    *m_ppjmp = m_pjmp;
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
	m_pkey->SetKey(m_sKey, m_bMatchWhole, m_matset);
    if ( m_pjmp && !m_pjmp->bEnforceRefIntegrity())
        {
        if ( m_pjmp->bMatchWhole() != m_bMatchWhole || m_pjmp->matset() != m_matset )
            {
            if ( AfxMessageBox(_("You have selected matching criteria different from the defaults for the selected jump path. Do you want to save these settings in the jump path to be used for future jumps?"), MB_YESNO | MB_ICONQUESTION) == IDYES )
                m_pjmp->SetMatchCriteriaFromKey(m_pkey);
            }
        }	
	CDialog::OnOK();
}


void CJumpDlg::OnSelchangeLangsetCombo()
{
	m_plng = m_cboLangEncSet.plngSelected();
	ASSERT( m_plng );
	
	m_elcJumpKey.SetLangEnc(m_plng);
    m_bLngEncChangedByUser = TRUE;
}

void CJumpDlg::OnSelchangePriJumpPathCombo()
{
    UpdateData(TRUE);
	m_pjmp = m_cboPriJumpPath.pjmpSelected();
	
    if ( m_pjmp )
        {
        if (!m_bLngEncChangedByUser )
            {
            if ( m_pjmp == *m_ppjmp ) // if user reverts to original jump path, revert to original language
                {
                m_plng = *m_pplng;
                m_cboLangEncSet.SelectElement(m_plng);
        	    m_elcJumpKey.SetLangEnc(m_plng);
                }
            else if ( !m_pjmp->bDefaultPath() )
                {
                m_plng = m_pjmp->plngDefault();
                ASSERT( m_plng );
                m_cboLangEncSet.SelectElement(m_plng);
        	    m_elcJumpKey.SetLangEnc(m_plng);
                }
            }
        m_bMatchWhole = m_pjmp->bMatchWhole();
        SetMatchSetting(m_pjmp->matset());
        UpdateData(FALSE); // 1.4kub May be needed
        }
}
