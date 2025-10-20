// mkrfed_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"
#include "project.h"	// For Shw_papp()
#include "kmn.h"	// For Keyboard functions.
#include "shw.h"
#include "shwview.h"
#include "mkrfed_d.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEdit

static WNDPROC NEAR pfnSuper = NULL;

WNDPROC* CFieldMarkerEdit::GetSuperWndProcAddr()
{
	return &pfnSuper;
}

BEGIN_MESSAGE_MAP(CFieldMarkerEdit, CEdit)
	//{{AFX_MSG_MAP(CFieldMarkerEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEdit message handlers

#define EN_BACKSPACE 0x8000 // message to send to parent dialog box to signal backspace on empty box

void CFieldMarkerEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if ( nChar == ' ' || nChar == '\r' )	// trap space
		{
        GetParent()->GetParent()->SendMessage( WM_COMMAND, IDOK );
		return;
		}
	else if ( nChar == 8  && !GetWindowTextLength() ) // check for backspace on empty edit box
		{
        GetParent()->GetParent()->SendMessage( WM_COMMAND, (WPARAM)MAKELONG(IDC_FIELD_MARKER_COMBO, EN_BACKSPACE), (LPARAM)m_hWnd );
		return;
		}	
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


// RNE Turn off Keyman while in the marker edit area.
void CFieldMarkerEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);
	Shw_pProject()->pkmnManager()->DeActivateKeyboards();
}


/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEditDlg dialog


CFieldMarkerEditDlg::CFieldMarkerEditDlg( CField* pfld, RECT rcWindowPos, CPoint pntCaretLoc, const CMarker* pmkrRecMark )
	: CDialog(CFieldMarkerEditDlg::IDD) // Constructor for changing existing marker
{
	m_sMarker = pfld->sMarker(); // Marker string
	m_pmkrset = pfld->pmkr()->pmkrsetMyOwner(); // Owning marker set
	m_pfld = pfld;
	m_ppfldNew = NULL;
	m_rcWindowPos = rcWindowPos;
	m_pntCaretLoc = pntCaretLoc;
	m_pmkrRecMark = pmkrRecMark;
}

CFieldMarkerEditDlg::CFieldMarkerEditDlg( CMarkerSet* pmkrset, CField** ppfldNew, RECT rcWindowPos, CPoint pntCaretLoc, const CMarker* pmkrRecMark )
	: CDialog(CFieldMarkerEditDlg::IDD) // Constructor for inserting new marker
{
	m_sMarker = ""; // Marker string starts out null
	m_pmkrset = pmkrset; // Owning marker set
	m_pfld = NULL;
	m_ppfldNew = ppfldNew; // Field pointer for returning new field
	m_rcWindowPos = rcWindowPos;
	m_pntCaretLoc = pntCaretLoc;
	m_pmkrRecMark = pmkrRecMark;
}

void CFieldMarkerEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIELD_MARKER_COMBO, *m_combo);
	//{{AFX_DATA_MAP(CFieldMarkerEditDlg)
	//}}AFX_DATA_MAP
//	m_pmkrset->DDX_Name(pDX, IDC_FIELD_MARKER_COMBO, m_sMarker); // 1.4hdm Replace DDX_Name in field marker edit dlg
//	DDV_MaxChars(pDX, m_sMarker, 25);
}

BEGIN_MESSAGE_MAP(CFieldMarkerEditDlg, CDialog)
	//{{AFX_MSG_MAP(CFieldMarkerEditDlg)
	ON_CONTROL(EN_BACKSPACE, IDC_FIELD_MARKER_COMBO, OnFieldMarkerBackspace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFieldMarkerEditDlg message handlers

BOOL CFieldMarkerEditDlg::OnInitDialog() // Override to turn off selection and place caret at end
{
	m_combo = (CComboBox*)this->GetDlgItem( IDC_FIELD_MARKER_COMBO );
	CDialog::OnInitDialog();                              
    SetDlgItemText( IDC_FIELD_MARKER_COMBO, swUTF16( m_sMarker ) ); // 1.4hdm // 1.4qpv
	CWnd* pEditWnd = m_combo->GetWindow( GW_CHILD );
	m_edtMarker.SubclassWindow( pEditWnd->GetSafeHwnd() );
	// fill the combo box list with all markers in this marker set
	CShwView* pview = Shw_papp()->pviewActive(); // 1.0by Show all markers if not hiding fields
	for ( CMarker* pmkr = m_pmkrset->pmkrFirst(); pmkr; pmkr = m_pmkrset->pmkrNext( pmkr ) )
		{
		if ( !( pview->ptyp()->bHideFields() && pmkr->bHidden() ) ) // 1.0bt Don't allow insert of hidden field, 1.0by Show all markers if not hiding fields
			m_combo->InsertString( -1, swUTF16( pmkr->sKey()  ) ); // 1.4qre
		}
	SetWindowPos( NULL, m_rcWindowPos.left, m_rcWindowPos.top + m_pntCaretLoc.y - 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE );
	int iLen = m_sMarker.GetLength();
	m_edtMarker.SetSel( iLen, iLen ); // Set caret to end of marker
	m_edtMarker.SetFocus(); // Set focus so the default won't select entire marker
#ifndef _MAC
	m_combo->ShowDropDown();  // 1996-02-26 MRP
#endif
	return FALSE; // Don't do defaults 
}

void CFieldMarkerEditDlg::OnOK()
{
	UpdateData( TRUE );
//    GetDlgItemText( IDC_FIELD_MARKER_COMBO, (char*)buffer, DLGBUFMAX ); // 1.4hdm
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_FIELD_MARKER_COMBO, sDlgItem ); // 1.4qpw
	m_sMarker = sUTF8( sDlgItem ); // 1.4qpw
	RemoveBackslashAtFront( m_sMarker ); // 1.4hdm
	CNote* pnot = NULL;
	if ( !CMarker::s_bValidName( m_sMarker, &pnot ) ) // If not valid syntactically, give message and make them fix it
		{
		pnot->SetStringContainingReferent( m_sMarker ); // Inform note of beginning of string
		m_edtMarker.SetSel( pnot->dwSelReferent() ); // Select problem character
		int iAnswer = AfxMessageBox( pnot->sNote(), MB_OKCANCEL ); // Give message
		delete pnot; // If failed, delete dynamically created note
		if ( iAnswer == IDCANCEL )
			CDialog::OnCancel();
		return;
		}
	CMarker* pmkr = m_pmkrset->pmkrSearch( m_sMarker );
	if ( !pmkr ) // If not already in list of markers, ask if they want to add it
		{
		MessageBeep(0);  // 1996-02-27 MRP
		if ( Shw_pProject()->bLockProject() ) // 1.2by If project locked, don't offer to add new marker
			{
			int iAnswer = AfxMessageBox( _("Marker not found.") ); //1.2by If project locked, don't offer to add marker
			return;
			}
		int iAnswer = AfxMessageBox( _("Marker not in marker list. Add it?"), MB_YESNO | MB_DEFBUTTON2 ); //1.2bg Change add marker box to default to no
		if ( iAnswer == IDNO ) 
			return;
		if ( !m_pmkrset->bModalView_NewMarker( &pmkr, m_sMarker ) ) // Add new marker to list
			return; // If canceled, go back to editing the marker
		}
	if ( pmkr == m_pmkrRecMark ) // If new marker is record marker, refuse to insert it
		{
		AfxMessageBox(_("Insertion of record marker not allowed. Use Database Insert Record."));
		return; // Send them back to editing the marker
		}
	if ( m_ppfldNew != NULL ) // If inserting new marker, instantiate field and set it 
		*m_ppfldNew = new CField( pmkr );			
	else
		m_pfld->SetMarker( pmkr ); // Else set existing marker in existing field
	CDialog::OnOK(); // Close dialog box
}

// executes when backspace is pressed on an empty field marker editbox
void CFieldMarkerEditDlg::OnFieldMarkerBackspace()
{
	EndDialog( IDNO ); // IDNO stands for NO more marker!!! 
}

