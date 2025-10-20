// key_d.cpp  Implementation of record key dialog box  (1995-05-24)

#include "stdafx.h"
#include "toolbox.h"
#include "key_d.h"
#include "recset.h"  // CKey
#include "mkr.h"  // CMarker, CMString
#include "ddx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyDlg dialog


CKeyDlg::CKeyDlg(CKey* pkey, int iCommand, CMarker* pmkr) :
		CDialog(CKeyDlg::IDD)
{
	m_pkey = pkey;
	ASSERT( m_pkey );
	m_sKey = m_pkey->sKey();
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
		
	m_iCommand = iCommand;
	m_pmkr = pmkr;	
	ASSERT( m_pmkr );
}


void CKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEY_KEY, m_elcKey);
//	Shw_DDX_TrimmedTxt(pDX, IDC_KEY_KEY, m_sKey); // 1.4gv Replace DDX_Trimmed in Search and Insert boxes
	//{{AFX_DATA_MAP(CKeyDlg)
//	DDX_Txt(pDX, IDC_KEY_LABEL, m_sLabel);
	DDX_Check(pDX, IDC_KEY_WHOLE_KEY, m_bMatchWhole);
	DDX_Radio(pDX, IDC_KEY_MATCH_SAME_PRIMARY, m_iMatchSetting);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CKeyDlg message handlers

BOOL CKeyDlg::OnInitDialog()
{
	// Set the dialog box title and key label according to the user's command.
	Str8 sTitle;
	switch ( m_iCommand )
		{
		case eInsert:
			sTitle = _("Insert Record"); // 1.5mr 
			m_sLabel = _("&Record to insert"); // 1.5mr 
			break;
		case eSearch:
			sTitle = _("Search"); // 1.5mr 
			m_sLabel = _("&Search"); // 1.5.0fd // 1.5mr 
			m_sLabel = m_sLabel + " \\" + m_pmkr->sMarker(); // 1.5.0fd
			break;
		default:
			ASSERT( FALSE );
		}  // must precede OnInitDialog
	SetWindowText( swUTF16( sTitle ) ); // 1.4qpm Fix for Unicode // 1.4sb
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_KEY_LABEL, swUTF16( m_sLabel ) ); // 1.4qpm Fix for Unicode // 1.4sb
	m_elcKey.SetLangEnc( m_pmkr->plng() ); // 1.4vxk Use SetLangEnc for CEditLng instead of SetMarker
	m_elcKey.SetEditLngText( m_sKey ); // 1.4qpk Sample of changes needed for SetDlgItemTxt of lng edit ctrl
	m_elcKey.SetSel( 0, m_sKey.GetLength() ); // 1.4qph Fix bug (1.4qna) of not selecting Search string
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CKeyDlg::OnOK()
{
	UpdateData(TRUE);  // from dialog controls to member variables

	m_sKey = m_elcKey.sGetEditLngText(); // 1.4qpk
	TrimBothEnds( m_sKey ); // 1.4hdc
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
		
	m_pkey->SetKey(m_sKey, m_bMatchWhole, m_matset);
	
	CDialog::OnOK();                  
}

void CKeyDlg::OnCancel()
{
	CDialog::OnCancel();
}
