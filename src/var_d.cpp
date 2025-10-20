// var_d.cpp  Implementation of the variable dialog box

#include "stdafx.h"
#include "toolbox.h"
#include "var_d.h"
#include "lng.h"
#include "not.h"
#include "ddx.h"  // Shw_DDX_MultiLineText

#include <strstrea.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


CVarInstanceDlg::CVarInstanceDlg( CVarInstance* pvin ) : CDialog(CVarInstanceDlg::IDD)
{
    m_pvin = pvin;
    ASSERT( m_pvin );
    m_ppvinNew = NULL;
    m_pvinset = m_pvin->pvinsetMyOwner();
    ASSERT( m_pvinset );
    m_sName = m_pvin->sName();
    m_sOriginalName = m_sName;
    m_sChars = m_pvin->sChars();
}

CVarInstanceDlg::CVarInstanceDlg( CVarInstanceSet* pvinset, CVarInstance** ppvinNew ) :
    CDialog(CVarInstanceDlg::IDD)
{
    m_pvin = NULL;
    m_ppvinNew = ppvinNew;
    ASSERT( m_ppvinNew );
    m_pvinset = pvinset;
    ASSERT( m_pvinset );
}

CVarInstanceDlg::CVarInstanceDlg( const CVarInstance* pvinToCopyFrom,
        CVarInstance** ppvinNew ) :
    CDialog(CVarInstanceDlg::IDD)
{
    m_pvin = NULL;
    m_ppvinNew = ppvinNew;
    ASSERT( m_ppvinNew );
    ASSERT( pvinToCopyFrom );
    m_pvinset = pvinToCopyFrom->pvinsetMyOwner();

    m_sName = pvinToCopyFrom->sName();
    m_sChars = pvinToCopyFrom->sChars();
}


void CVarInstanceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CVarInstanceDlg)
    DDX_Control(pDX, IDC_VAR_NAME, m_edtName);
    DDX_Control(pDX, IDC_VAR_CHARS, m_elcChars);
    DDX_Control(pDX, IDC_DLG_NOT, m_edtNote);
    //}}AFX_DATA_MAP
//    m_pvinset->DDX_Name(pDX, IDC_VAR_NAME, m_sName);  // 1.4hdq Remove DDX_Name from variable dlg box
//    Shw_DDX_MultiLineText(pDX, IDC_VAR_CHARS, m_sChars); // 1.4hdy Replace DDX_MultiLineText in variable dlg box
}

BEGIN_MESSAGE_MAP(CVarInstanceDlg, CDialog)
    //{{AFX_MSG_MAP(CVarInstanceDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CVarInstanceDlg message handlers

BOOL CVarInstanceDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_VAR_NAME, swUTF16( m_sName ) ); // 1.4hdq // 1.4qpv
	m_elcChars.SetEditLngText( m_sChars ); // 1.4qpn
    CLangEnc* plng = m_pvinset->plngMyOwner();
    m_elcChars.SetLangEnc(plng);
    if ( m_ppvinNew )
        m_edtName.SetFocus();  // Add
    else
        m_elcChars.SetFocus();  // Modify
    return FALSE;
}

void CVarInstanceDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_VAR_NAME, pszBigBuffer, BIGBUFMAX ); // 1.4hdq
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_VAR_NAME, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdq
	m_sChars = m_elcChars.sGetEditLngText(); // 1.4qpn
    int len = m_sName.GetLength(); // 1.4hdq Change all spaces to underlines
    for ( int i = 0; i != len; i++ )
        if ( m_sName.GetChar(i) == ' ' )
            m_sName.SetAt(i, '_');
    
    if ( !m_ppvinNew && !bEqual(m_sName, m_sOriginalName) )
        {
        }
    
    CNote* pnot = NULL;
    BOOL bOK = ( m_pvin ?
        m_pvin->bModifyProperties(m_sName, m_sChars, &pnot) :
            // User edited an existing variable
        m_pvinset->bAddVarInstance(m_sName, m_sChars, &m_pvin, &pnot)
            // User added a new variable
        );
        
    if ( bOK )
        {
        ASSERT( m_pvin );
        ASSERT( !pnot );
        
        if ( m_ppvinNew )
            *m_ppvinNew = m_pvin;  // Set the marker
        CDialog::OnOK();
        }
    else
        {
        // Report to the user that a problem prevented editing or adding
        // the marker. The marker object itself has not been changed.
        // Set the insertion point, or select text referred to in note
        ASSERT( pnot );
        if ( pnot->bRefersToString(m_sName) )
            {
            m_edtName.SetSel(pnot->dwSelReferent());
            m_edtName.SetFocus();
            }
        else if ( pnot->bRefersToString(m_sChars) )
            {
			m_elcChars.SetSel( LOWORD(pnot->dwMultiLineSelReferent()), HIWORD(pnot->dwMultiLineSelReferent())); // 1.4kya No DWORD in Wnd version of edit ctrl
            m_elcChars.SetFocus();
            }
            
        // Show the note on the dialog itself
        m_edtNote.SetWindowText( swUTF16( pnot->sNote()) ); // 1.4qpy
        m_edtNote.ShowWindow(SW_SHOWNOACTIVATE);
        delete pnot;
        }
}  // CVarInstanceDlg::OnOK()

void CVarInstanceDlg::OnCancel()
{
    CDialog::OnCancel();
}
