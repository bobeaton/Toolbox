// seldb_d.cpp : implementation file
//

#include "stdafx.h"
#include "toolbox.h"

#include "dbref.h"
#include "seldb_d.h"
#include "project.h"
#include "help_ids.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectDbDlg dialog


CSelectDbDlg::CSelectDbDlg( const CDatabaseRefList* pdrflst, CShwDoc** ppdoc, EAction act, BOOL bExitOnOne)
    : CDialog((act == actMerge)? IDD_MERGE : IDD),
    m_pdrflst(pdrflst),
    m_ppdoc(ppdoc),
	m_bExitOnOne(bExitOnOne),
    m_bOverrideLabel(FALSE)
{
    switch(act)
        {
        case actCopy:
            m_bOverrideLabel = TRUE;
            m_sSelectDB = _("&Database to copy record into:");
            SetHelpID(IDD_SELECTDB_COPY);
            break;
        case actMove:
            m_bOverrideLabel = TRUE;
            m_sSelectDB = _("&Database to move record into:");
            SetHelpID(IDD_SELECTDB_MOVE);
            break;
        case actInsert:
        case actMerge:
            break;
        default:
            ASSERT(FALSE);
        }
}

void CSelectDbDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectDbDlg)
//    if ( m_bOverrideLabel ) // 1.4dw
//        DDX_Txt(pDX, IDC_lblSelectDB, m_sSelectDB);
    DDX_Control(pDX, IDC_SELECTDB_SHOWPATH, m_chkShowPath);
    DDX_Control(pDX, IDC_SELECTDB_LISTBOX, m_lboDbsAvailable);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelectDbDlg, CDialog)
    //{{AFX_MSG_MAP(CSelectDbDlg)
    ON_BN_CLICKED(IDC_SELECTDB_SHOWPATH, OnSelectdbShowpath)
    ON_LBN_DBLCLK(IDC_SELECTDB_LISTBOX, OnDblclkSelectdbListbox)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelectDbDlg message handlers

BOOL CSelectDbDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    if ( m_bOverrideLabel )
        SetDlgItemText( IDC_lblSelectDB, swUTF16( m_sSelectDB ) ); // 1.4qpv
    
    m_lboDbsAvailable.LoadDbsInsertable( m_pdrflst );  // 1996-10-16 MRP
    if ( m_bExitOnOne && (m_lboDbsAvailable.GetCount() == 1) ) // only one db, don't bother displaying dialog?
        OnOK();

    m_chkShowPath.SetCheck( Shw_pProject()->bShowWholePath() );

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSelectDbDlg::OnSelectdbShowpath()
{
    Shw_pProject()->ShowWholePath( m_chkShowPath.GetCheck() );
    m_lboDbsAvailable.Invalidate(); // force redraw
}

void CSelectDbDlg::OnOK()
{
//    GetDlgItemText( IDC_lblSelectDB, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_lblSelectDB, sDlgItem ); // 1.4qpw
	m_sSelectDB = sUTF8( sDlgItem ); // 1.4qpw

    *m_ppdoc = m_lboDbsAvailable.pdocSelected(); // return selected doc to dialog caller
    
    CDialog::OnOK();
}


void CSelectDbDlg::OnDblclkSelectdbListbox()
{
    OnOK();
}
