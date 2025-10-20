// set_d.cpp  Implementation of  a dialog box representing a set (1995-06-24)

#include "stdafx.h"
#include "toolbox.h"
#include "set_d.h"
#include "set.h"  // class CSetEl
#include "tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// **************************************************************************

CSetDlg::CSetDlg(CSetListBox& lboSet, const char* pszTitle, const Str8& sLabel, ID idHelp, ID iddDialog) :
    CDialog(iddDialog),
    m_lboSet(lboSet),
    m_sLabel(sLabel)
{
    m_sTitle = pszTitle;
    SetHelpID(idHelp);
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetDlg)
    DDX_Control(pDX, IDC_SET_LIST, m_lboSet);
    DDX_Control(pDX, IDC_ADD, m_btnAdd);
    DDX_Control(pDX, IDC_COPY, m_btnCopy);
    DDX_Control(pDX, IDC_MODIFY, m_btnModify);
    DDX_Control(pDX, IDC_DELETE, m_btnDelete);
//    DDX_Txt(pDX, IDC_SET_LABEL, m_sLabel);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
    //{{AFX_MSG_MAP(CSetDlg)
    ON_BN_CLICKED(IDC_CLOSE, OnClose)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_COPY, OnCopy)
    ON_BN_CLICKED(IDC_MODIFY, OnModify)
    ON_LBN_DBLCLK(IDC_SET_LIST, OnDblclkItem)
    ON_BN_CLICKED(IDC_DELETE, OnDelete)
    ON_LBN_SELCHANGE(IDC_SET_LIST, OnSelchangeList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSetDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_SET_LABEL, swUTF16( m_sLabel ) ); // 1.4qpv

    SetWindowText( swUTF16( m_sTitle) ); // 1.4qpy
    m_lboSet.InitListBox();  // 1995-06-22 MRP
    UpdateButtons();
    OnSelchangeList();

    return TRUE;  // input focus will be set to the dialog's first control
}

void CSetDlg::OnCancel()
{
    m_lboSet.Close();  // 1997-11-15 MRP: Update the selected element
    CDialog::OnCancel();
}

void CSetDlg::OnClose()
{
//    GetDlgItemText( IDC_SET_LABEL, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_SET_LABEL, sDlgItem ); // 1.4qpw
	m_sLabel = sUTF8( sDlgItem ); // 1.4qpw

    m_lboSet.Close();  // 1997-11-15 MRP: Update the selected element
    CDialog::OnOK();
}

void CSetDlg::OnAdd()
{
    if ( m_lboSet.bAdd() )
        UpdateButtons();
}

void CSetDlg::OnCopy()
{
    if ( m_lboSet.bCopy() )
        UpdateButtons();
}

void CSetDlg::OnModify()
{
    (void) m_lboSet.bModify();
}

void CSetDlg::OnDblclkItem()
{
    OnModify();  // shortcut for the Modify button
}

void CSetDlg::OnDelete()
{
    // 1995-06-10 MRP: Test deleting a filter in use: GPF???
    
    if ( m_lboSet.bDelete() )
        {
        UpdateButtons();
        
        // RNE 1996-01-11 Also call OnSelchangeList because deleting currently
        // selected element is a selection change.
        OnSelchangeList();
        }
}

void CSetDlg::OnSelchangeList()
{
    UpdateDeleteButton();
}

void CSetDlg::UpdateButtons()
{
    BOOL bListHasItems = !bListEmpty();
    
    m_btnCopy.EnableWindow(bListHasItems);
    m_btnModify.EnableWindow(bListHasItems);
    UpdateDeleteButton();
}

void CSetDlg::UpdateDeleteButton()
{
    CSetEl* psel = m_lboSet.pselSelected();
    BOOL bDeletable = ( psel != NULL ? psel->bDeletable() : FALSE );
    m_btnDelete.EnableWindow(bDeletable);
}

void DlgNotYetImplemented()
{
    Str8 sMessage;
    sMessage = _("This dialog box is not yet implemented.");
    (void) AfxMessageBox(sMessage, MB_OK|MB_ICONINFORMATION);
}


// **************************************************************************

CSetPage::CSetPage(CSetListBox& lboSet, ID idsTitle, ID iddDialog) :
        CPropertyPage(iddDialog, idsTitle), m_lboSet(lboSet)
{
    m_idsTitle = idsTitle;
}

void CSetPage::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSetPage)
    DDX_Control(pDX, IDC_SET_LIST, m_lboSet);
    DDX_Control(pDX, IDC_ADD, m_btnAdd);
    DDX_Control(pDX, IDC_COPY, m_btnCopy);
    DDX_Control(pDX, IDC_MODIFY, m_btnModify);
    DDX_Control(pDX, IDC_DELETE, m_btnDelete);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetPage, CPropertyPage)
    //{{AFX_MSG_MAP(CSetPage)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_COPY, OnCopy)
    ON_BN_CLICKED(IDC_MODIFY, OnModify)
    ON_LBN_DBLCLK(IDC_SET_LIST, OnDblclkItem)
    ON_BN_CLICKED(IDC_DELETE, OnDelete)
    ON_LBN_SELCHANGE(IDC_SET_LIST, OnSelchangeList)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSetPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();
    m_lboSet.InitListBox();  // 1995-06-22 MRP
    UpdateButtons();
    OnSelchangeList();

    return TRUE;  // input focus will be set to the dialog's first control
}

void CSetPage::OnOK()
{
}

void CSetPage::OnAdd()
{
    if ( m_lboSet.bAdd() )
        UpdateButtons();
}

void CSetPage::OnCopy()
{
    if ( m_lboSet.bCopy() )
        UpdateButtons();
}

void CSetPage::OnModify()
{
    if ( m_lboSet.bModify() )
        UpdateButtons();  // Because other controls may need to be updated
}

void CSetPage::OnDblclkItem()
{
    OnModify();  // shortcut for the Modify button
}

void CSetPage::OnDelete()
{
    // 1995-06-10 MRP: Test deleting a filter in use: GPF???
    
    if ( m_lboSet.bDelete() )
        {
        UpdateButtons();
        OnSelchangeList();
        }
}

void CSetPage::OnSelchangeList()
{
    UpdateDeleteButton();
}

void CSetPage::UpdateButtons()
{
    BOOL bListHasItems = !bListEmpty();
    
    m_btnCopy.EnableWindow(bListHasItems);
    m_btnModify.EnableWindow(bListHasItems);
    UpdateDeleteButton();
    
    UpdateOtherControls();
}

void CSetPage::UpdateDeleteButton()
{
    CSetEl* psel = m_lboSet.pselSelected();
    BOOL bDeletable = ( psel != NULL ? psel->bDeletable() : FALSE );
    m_btnDelete.EnableWindow(bDeletable);
}

void CSetPage::UpdateOtherControls()
{
}
