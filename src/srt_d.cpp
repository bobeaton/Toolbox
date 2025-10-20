// srt_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "lng.h"
#include "srt_d.h"
#include "ddx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// Definitions for helper functions needed just in this module for now.
#if 0
BOOL bSpaceIsFirst(const char* pszOrder)
{
	Str8 sOrder(pszOrder);
	// Look for <<space>>
	int pos = sOrder.Find("«space»");
	if (pos == 0) return TRUE;
	return FALSE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortEditDlg dialog

#if  0
CSortEditDlg::CSortEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSortEditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSortEditDlg)
	m_sIgnores = "";
	m_sPrimaries = "";
	m_sSecondsAfter = "";
	m_sSecondsBefore = "";
	m_sName = "";
	m_sDescribe = "";
	m_bSecondariesAfter = FALSE;
	//}}AFX_DATA_INIT
}
#endif

CSortEditDlg::CSortEditDlg(CSortOrder* psrt) : CDialog(CSortEditDlg::IDD)
{
	ASSERT( psrt != NULL );
	m_psrt = psrt;
	m_ppsrtNew = NULL;
	m_psrtset = m_psrt->psrtsetMyOwner();
	m_sName = m_psrt->sName();
	m_sDescribe = m_psrt->sDescription();
	m_sPrimaries = m_psrt->pszPrimaryOrder();
	m_sSecondsBefore = m_psrt->pszSecondariesPreceding();
	m_sSecondsAfter = m_psrt->pszSecondariesFollowing();
	m_sIgnores = m_psrt->pszIgnore();
	m_bSecondariesAfter = m_psrt->bSecAfterBase();
}
	
CSortEditDlg::CSortEditDlg(CSortOrderSet* psrtset, CSortOrder** ppsrtNew) :
		CDialog(CSortEditDlg::IDD)
{
	ASSERT( psrtset != NULL );
	ASSERT( ppsrtNew != NULL );
	m_ppsrtNew = ppsrtNew;
	m_psrt = NULL;
	m_psrtset = psrtset;
	m_sName = "";
	m_sDescribe = CSortOrder::s_pszDescription;
	m_sPrimaries = CSortOrder::s_pszPrimary;
	m_sSecondsBefore = CSortOrder::s_pszSecondaryBefore;
	m_sSecondsAfter = CSortOrder::s_pszSecondaryAfter;
	m_sIgnores = CSortOrder::s_pszIgnore;
	m_bSecondariesAfter = CSortOrder::s_bSecondariesFollowBase;
}

CSortEditDlg::CSortEditDlg(const CSortOrder* psrtToCopyFrom, CSortOrder** ppsrtNew) :
		CDialog(CSortEditDlg::IDD)
{
	ASSERT( ppsrtNew != NULL );
	m_ppsrtNew = ppsrtNew;
	m_psrt = NULL;
	ASSERT( psrtToCopyFrom );
	m_psrtset = psrtToCopyFrom->psrtsetMyOwner();
	m_sName = psrtToCopyFrom->sName();
	m_sDescribe = psrtToCopyFrom->sDescription();
	m_sPrimaries = psrtToCopyFrom->pszPrimaryOrder();
	m_sSecondsBefore = psrtToCopyFrom->pszSecondariesPreceding();
	m_sSecondsAfter = psrtToCopyFrom->pszSecondariesFollowing();
	m_sIgnores = psrtToCopyFrom->pszIgnore();
	m_bSecondariesAfter = psrtToCopyFrom->bSecAfterBase();
}
	

void CSortEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SORTEDIT_NAME, m_edtName);
	DDX_Control(pDX, IDC_SORTEDIT_PRIMARIES, m_elcPrimaries);
	DDX_Control(pDX, IDC_SORTEDIT_SECONDSBEFORE, m_elcSecondariesBefore);
	DDX_Control(pDX, IDC_SORTEDIT_SECONDSAFTER, m_elcSecondariesAfter);
	DDX_Control(pDX, IDC_SORTEDIT_IGNORES, m_elcIgnores);
	DDX_Control(pDX, IDC_DLG_NOT, m_edtNote);
	
	//{{AFX_DATA_MAP(CSortEditDlg)
	DDX_Control(pDX, IDC_LBL_SortCCTable, m_lblSortCCTable);
//	DDX_Txt(pDX, IDC_SORTEDIT_IGNORES, m_sIgnores);
//	DDX_Txt(pDX, IDC_SORTEDIT_SECONDSAFTER, m_sSecondsAfter);
//	DDX_Txt(pDX, IDC_SORTEDIT_SECONDSBEFORE, m_sSecondsBefore);
	DDX_Check(pDX, IDC_SORTEDIT_CHKSECONDARIESAFTER, m_bSecondariesAfter);
	//}}AFX_DATA_MAP

//	m_psrtset->DDX_Name(pDX, IDC_SORTEDIT_NAME, m_sName); // 1.4hdh Replace DDX_Name in sort order dlg
//	Shw_DDX_MultiLineText(pDX, IDC_SORTEDIT_DESCRIBE, m_sDescribe); // 1.4hdu Replace DDX_MultiLineText in lang enc options pg dlg
//	Shw_DDX_MultiLineText(pDX, IDC_SORTEDIT_PRIMARIES, m_sPrimaries); // 1.4hdx Replace DDX_MultiLineText in sort order primaries
}

BEGIN_MESSAGE_MAP(CSortEditDlg, CDialog)
	//{{AFX_MSG_MAP(CSortEditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSortEditDlg message handlers

BOOL CSortEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    SetDlgItemText( IDC_SORTEDIT_NAME, swUTF16( m_sName ) ); // 1.4hdh // 1.4qpv
	m_sDescribe.Replace( "\n", "\r\n" ); // 1.4hdu Use Replace to change lf to crlf
    SetDlgItemText( IDC_SORTEDIT_DESCRIBE, swUTF16( m_sDescribe ) ); // 1.4hdu // 1.4qpv
	m_elcPrimaries.SetLangEnc( m_psrtset->plngMyOwner() );
	m_elcSecondariesBefore.SetLangEnc( m_psrtset->plngMyOwner() );	
	m_elcSecondariesAfter.SetLangEnc( m_psrtset->plngMyOwner() );	
	m_elcIgnores.SetLangEnc( m_psrtset->plngMyOwner() );	
	m_elcPrimaries.SetEditLngText( m_sPrimaries ); // 1.4qpn
	m_elcSecondariesBefore .SetEditLngText( m_sSecondsBefore ); // 1.4qpn
	m_elcSecondariesAfter .SetEditLngText( m_sSecondsAfter ); // 1.4qpn
	m_elcIgnores.SetEditLngText( m_sIgnores ); // 1.4qpn
	CFont* pfnt = (CFont*)m_psrtset->plngMyOwner()->pfntPropertiesDlg();
	CKeyboard* pkbd = m_psrtset->plngMyOwner()->pkbdPropertiesDlg();
	Str8 sCCTable;
	if ( m_psrt )
		sCCTable= m_psrt->pszPathCCT();
	if ( sCCTable.GetLength() > 0 )
		{
		Str8 sSortCCLabel = "Sort CC table: "; // 1.5.1ma 
		sCCTable = sSortCCLabel + sCCTable; // 1.5.0ft // 1.5.1ma Fix bug of odd display of sort cct in lng
		}
	m_lblSortCCTable.SetWindowText(  swUTF16( sCCTable ) ); // Show CC table // 1.4qpy
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSortEditDlg::OnCancel()
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CSortEditDlg::OnOK()
{
	UpdateData(TRUE);  // from dialog controls to dialog object member data
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_SORTEDIT_NAME, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdh
	GetDlgItemText( IDC_SORTEDIT_DESCRIBE, sDlgItem ); // 1.4qpw
	m_sDescribe = sUTF8( sDlgItem ); // 1.4qpw
	m_sDescribe.Replace( "\r\n", "\n" ); // 1.4hdu
	m_sPrimaries = m_elcPrimaries.sGetEditLngText(); // 1.4qpn
	m_sPrimaries.Replace( "\r\n", "\n" ); // 1.4vxj Fix U bug of added chars in sort not working till reload
	m_sPrimaries.Replace( "\r", "\n" ); // 1.4vxj
	m_sSecondsBefore = m_elcSecondariesBefore .sGetEditLngText(); // 1.4qpn
	m_sSecondsAfter = m_elcSecondariesAfter .sGetEditLngText(); // 1.4qpn
	m_sIgnores = m_elcIgnores.sGetEditLngText(); // 1.4qpn
	
	CNote* pnot = NULL;
	BOOL bOK = ( m_psrt ?
		m_psrt->bModifyProperties(m_sName, m_sDescribe,
					m_sPrimaries,  m_sSecondsBefore, m_sSecondsAfter,
					m_sIgnores, m_bSecondariesAfter, &pnot) :
			// User edited an existing sort order.
		m_psrtset->bAddSortOrder(m_sName, m_sDescribe,
					m_sPrimaries,  m_sSecondsBefore, m_sSecondsAfter,
					m_sIgnores, m_bSecondariesAfter, &m_psrt, &pnot)
			// User added a new sort order.
		);
		
	if ( bOK )
		{
		ASSERT( m_psrt );
		ASSERT( !pnot );

		if ( m_ppsrtNew )
			*m_ppsrtNew = m_psrt;  // Set the order.
		CDialog::OnOK();
		}
	else
		{
		// Report to the user that a problem prevented editing or adding
		// the sort order. The sort order object itself has not been changed.
		// Set the insertion point, or select text referred to in note
		ASSERT( pnot );
		if ( pnot->bRefersToString(m_sName) )
			{
			m_edtName.SetSel(pnot->dwSelReferent());
			m_edtName.SetFocus();
			}
		else if ( pnot->bRefersToString(m_sPrimaries) )
			{
			m_elcPrimaries.SetSel( LOWORD(pnot->dwMultiLineSelReferent()), HIWORD(pnot->dwMultiLineSelReferent())); // 1.4kya No DWORD in Wnd version of edit ctrl
			m_elcPrimaries.SetFocus();
			}
		else if ( pnot->bRefersToString(m_sSecondsBefore) )
			{
			m_elcPrimaries.SetSel( LOWORD(pnot->dwSelReferent()), HIWORD(pnot->dwSelReferent())); // 1.4kya No DWORD in Wnd version of edit ctrl
			m_elcSecondariesBefore.SetFocus();
			}
		else if ( pnot->bRefersToString(m_sSecondsAfter) )
			{
			m_elcPrimaries.SetSel( LOWORD(pnot->dwSelReferent()), HIWORD(pnot->dwSelReferent())); // 1.4kya No DWORD in Wnd version of edit ctrl
			m_elcSecondariesAfter.SetFocus();
			}
		else if ( pnot->bRefersToString(m_sIgnores) )
			{
			m_elcPrimaries.SetSel( LOWORD(pnot->dwSelReferent()), HIWORD(pnot->dwSelReferent())); // 1.4kya No DWORD in Wnd version of edit ctrl
			m_elcIgnores.SetFocus();
			}
		else
			ASSERT( FALSE );

		// Show the note on the dialog
		m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
		m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
		MessageBeep(0);

		delete pnot;
		}
}
