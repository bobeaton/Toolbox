// mrfset_d.cpp  Marker reference set list box and Select Fields dialog box

#include "stdafx.h"
#include "toolbox.h"
#include "mrfset_d.h"
#include "tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// --------------------------------------------------------------------------

CMarkerSubSetBox::CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide) :
    m_subset(subset),
    m_bIncludedSide(bIncludedSide),
	m_plng(NULL),
	m_psubsetInvalid(NULL)
{
}

CMarkerSubSetBox::CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide,
								   const CMarkerSubSet* psubsetInvalid) :
    m_subset(subset),
    m_bIncludedSide(bIncludedSide),
	m_plng(NULL),
	m_psubsetInvalid(psubsetInvalid)
{
	ASSERT(m_psubsetInvalid);
}

CMarkerSubSetBox::CMarkerSubSetBox(CMarkerSubSet& subset, BOOL bIncludedSide,
								   const CLangEnc* plng, const CMarkerSubSet* psubsetInvalid) :
    m_subset(subset),
    m_bIncludedSide(bIncludedSide),
	m_plng(plng),
	m_psubsetInvalid(psubsetInvalid)
{
	ASSERT(m_plng);
	ASSERT(m_psubsetInvalid);
}

void CMarkerSubSetBox::Init()
{
    SetTabStops(24);
    // Insert the names of the elements in the list in the list box.
    const CMarkerSet* pmkrset = m_subset.pmkrset();
    const CMarker* pmkr = pmkrset->pmkrFirst();
    for ( ; pmkr; pmkr = pmkrset->pmkrNext(pmkr) )
        {
        if ( bInclude(pmkr) )
            {
            // Insert the item at the end of the list box.
            // Unlike AddString(), InsertString() does not cause
            // a list with the LBS_SORT style to be sorted.
            ItemIndex itmInserted = InsertString(-1, swUTF16( pmkr->sItemTab() ) ); // 1.4qre
            if ( itmInserted != LB_ERR )  // 1999-01-21 MRP: removed ;
                SetItemDataPtr(itmInserted, (void*)pmkr);
            }
        }
    if ( bNonEmpty() )
        SetSel(0, TRUE);
}

BOOL CMarkerSubSetBox::bInclude(const CMarker* pmkr) const
{
	if ( m_plng )
		{
		if ( pmkr->plng() != m_plng )
			return FALSE;
		}
	if ( m_psubsetInvalid )
		{
		if ( m_psubsetInvalid->bIncludes(pmkr) )
			return FALSE;
		}
	BOOL bIncluded = m_subset.bIncludes(pmkr);
	return ( m_bIncludedSide ? bIncluded : !bIncluded );
}

void CMarkerSubSetBox::AddSelectedItemsTo(CMarkerSubSetBox& lboTo)
{
    ASSERT( bNonEmpty() );
    lboTo.SetSel(-1, FALSE);  // Remove selection from all items

    ASSERT( &m_subset == &lboTo.m_subset );
    const CMarkerSet* pmkrset = m_subset.pmkrset();
    ItemIndex itmSelectedTo = 0;
    ItemIndex itmTo = 0;
    const CMarker* pmkrTo = lboTo.pmkrItem(itmTo);
    if ( pmkrTo == pmkrset->pmkrFirst() )
        {
        CMarker* pmkr = pmkrset->pmkrNext(pmkrTo);
        while ( (pmkrTo = lboTo.pmkrItem(++itmTo)) == pmkr && pmkrTo )
            pmkr = pmkrset->pmkrNext(pmkrTo);
        }
    ItemIndex itmSelectedFrom = 0;
    ItemIndex itmFrom = 0;
    while ( itmFrom < GetCount() )
        {
        const CMarker* pmkrFrom = pmkrItem(itmFrom);
        if ( GetSel(itmFrom) > 0 )
            {
            (void) lboTo.InsertString(itmTo, swUTF16( pmkrFrom->sItemTab() ) ); // 1.4qre
            lboTo.SetItemDataPtr(itmTo, (void*)pmkrFrom);
            lboTo.SetSel(itmTo, TRUE);
            itmSelectedTo = itmTo;
            const CMarker* pmkr = pmkrset->pmkrNext(pmkrFrom);
            while ( (pmkrTo = lboTo.pmkrItem(++itmTo)) == pmkr && pmkrTo )
                pmkr = pmkrset->pmkrNext(pmkrTo);
            DeleteString(itmFrom);
            itmSelectedFrom = itmFrom;
            }
        else
            {
            if ( pmkrTo && pmkrTo == pmkrset->pmkrNext(pmkrFrom) )
                {
                const CMarker* pmkr = pmkrset->pmkrNext(pmkrTo);
                while ( (pmkrTo = lboTo.pmkrItem(++itmTo)) == pmkr && pmkrTo )
                    pmkr = pmkrset->pmkrNext(pmkrTo);
                }
            itmFrom++;
            }
        }

    if ( bNonEmpty() )
        {
        if ( itmSelectedFrom == GetCount() )
            itmSelectedFrom--;
        SetSel(itmSelectedFrom, TRUE);
        }
    lboTo.SetCaretIndex(itmSelectedTo, FALSE);  // Make fully visible
    m_sMatched.Empty();
    lboTo.m_sMatched.Empty();
}

void CMarkerSubSetBox::UpdateMarkerSubSet() const
{
    ItemIndex nitm = GetCount();
    ItemIndex itm = 0;
    for ( ; itm < nitm; itm++ )
        {
        CMarker* pmkr = (CMarker*)GetItemDataPtr(itm);
        m_subset.Include(pmkr, m_bIncludedSide);
        }
}

CMarker* CMarkerSubSetBox::pmkrItem(ItemIndex itm) const
{
    return itm < GetCount() ? (CMarker*)GetItemDataPtr(itm) : NULL;
}

BEGIN_MESSAGE_MAP(CMarkerSubSetBox, CListBox)
    ON_WM_CHAR()
    ON_WM_KILLFOCUS( )
END_MESSAGE_MAP()

void CMarkerSubSetBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if ( (nChar < 32 || 255 < nChar) && nChar != VK_BACK )
        {
        m_sMatched.Empty();
        CListBox::OnChar(nChar, nRepCnt, nFlags);
        return;
        }

    Str8 s = m_sMatched;     
    if ( nChar == VK_BACK )
        {
        if ( s.IsEmpty() )
            {
            MessageBeep(0);
            return;
            }
        else if ( s.GetLength() == 1 )
            {
            m_sMatched.Empty();
            SetSel(-1, FALSE);  // Remove selection from all items
            SetSel(0, TRUE);  // Select first item
            return;
            }
        s = s.Left(s.GetLength()-1);
        }
    else
        s += (char)nChar;
        
    ItemIndex itm = 0;
    if ( !m_sMatched.IsEmpty() && nChar != VK_BACK )
        itm = GetCaretIndex();
    ASSERT( 0 <= itm );

    itm = FindString(itm-1, swUTF16( s ) ); // 1.4qtf
    if ( 0 <= itm )
        {
        m_sMatched = s;
        SetSel(-1, FALSE);  // Remove selection from all items
        SetSel(itm, TRUE);  // Select this item
        }
    else
        MessageBeep(0);
}

void CMarkerSubSetBox::OnKillFocus(CWnd* pwnd)
{
    m_sMatched.Empty();
    CListBox::OnKillFocus(pwnd);    
}

        
BOOL CMarkerSubSetBox::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam,
    LRESULT* pLResult)
{
    if ( message == WM_COMMAND )
        {
        WORD wNotificationCode = HIWORD(wParam);
        if ( wNotificationCode == LBN_SELCHANGE )
            m_sMatched.Empty();
        }
        
    return FALSE;  // Let the parent handle the message
}


// --------------------------------------------------------------------------

CSelectFieldsDlg::CSelectFieldsDlg(CMarkerSubSet& subset, BOOL bEnableAutoInclude ) : // , UINT idsInstructions) : // 1.4gb
    CDialog(CSelectFieldsDlg::IDD),
    m_subset(subset),
//	m_idsInstructions(idsInstructions), // 1.4gb
    m_bShowAutoInclude(bEnableAutoInclude),
    m_lboExcluded(m_subset, FALSE),
    m_lboIncluded(m_subset, TRUE)
{
    m_subset.IncludeAllElements(FALSE);
    m_bAutoInclude = m_subset.bAutoIncludeNewElements();
}

CSelectFieldsDlg::CSelectFieldsDlg(CMarkerSubSet& subset, BOOL bEnableAutoInclude, CLangEncPtr plng, CMarkerSubSet* psubsetInvalid ) : // , UINT idsInstructions) : // 1.4gb
    CDialog(CSelectFieldsDlg::IDD),
    m_subset(subset),
//	m_idsInstructions(idsInstructions), // 1.4gb
    m_bShowAutoInclude(bEnableAutoInclude),
    m_lboExcluded(m_subset, FALSE, plng, psubsetInvalid),
    m_lboIncluded(m_subset, TRUE, plng, psubsetInvalid)
{
    m_subset.IncludeAllElements(FALSE);
    m_bAutoInclude = m_subset.bAutoIncludeNewElements();
}

void CSelectFieldsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CSelectFieldsDlg)
    DDX_Control(pDX, IDC_mrfset_lblInstructions, m_lblInstructions);
    DDX_Control(pDX, IDC_mrfset_lboExcluded, m_lboExcluded);
    DDX_Control(pDX, IDC_mrfset_btnAdd, m_btnAdd);
    DDX_Control(pDX, IDC_mrfset_btnRemove, m_btnRemove);
    DDX_Control(pDX, IDC_mrfset_btnRemoveAll, m_btnRemoveAll);  // 1999-02-04 MRP
    DDX_Control(pDX, IDC_mrfset_lboIncluded, m_lboIncluded);
    DDX_Control(pDX, IDC_mrfset_chkAutoInclude, m_chkAutoInclude);
    DDX_Check(pDX, IDC_mrfset_chkAutoInclude, m_bAutoInclude);
    //}}AFX_DATA_MAP
}

BOOL CSelectFieldsDlg::OnInitDialog()
{
    BOOL bLetWindowsSetFocus = CDialog::OnInitDialog();

	m_lboExcluded.Init();
	m_lboIncluded.Init();
    UpdateButtons();
    if ( m_lboExcluded.bEmpty() )
        {
        m_lboIncluded.SetFocus();
        bLetWindowsSetFocus = FALSE;
        }
//	if ( m_idsInstructions )
//		{
//		Str8 s;
//		LoadStrng(s, m_idsInstructions); // 1.4gb Eliminate string resource in Select Fields dialog box
//		m_lblInstructions.SetWindowText(s);
//		}
	if ( !m_bShowAutoInclude )
		m_chkAutoInclude.ShowWindow(SW_HIDE);

    return bLetWindowsSetFocus;
}

void CSelectFieldsDlg::UpdateButtons()
{
    m_btnAdd.EnableWindow(m_lboExcluded.bNonEmpty());
    m_btnRemove.EnableWindow(m_lboIncluded.bNonEmpty());
    m_btnRemoveAll.EnableWindow(m_lboIncluded.bNonEmpty());  // 1999-02-04 MRP
}

BEGIN_MESSAGE_MAP(CSelectFieldsDlg, CDialog)
    //{{AFX_MSG_MAP(CSelectFieldsDlg)
    ON_BN_CLICKED(IDC_mrfset_btnAdd, OnAdd)
    ON_BN_CLICKED(IDC_mrfset_btnRemove, OnRemove)
    ON_BN_CLICKED(IDC_mrfset_btnRemoveAll, OnRemoveAll)  // 1999-02-04 MRP
    ON_LBN_DBLCLK(IDC_mrfset_lboExcluded, OnAdd)
    ON_LBN_DBLCLK(IDC_mrfset_lboIncluded, OnRemove)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSelectFieldsDlg::OnOK()
{
    CDialog::OnOK();  // Updates member data from dialog controls
    m_lboExcluded.UpdateMarkerSubSet();
    m_lboIncluded.UpdateMarkerSubSet();
    m_subset.AutoIncludeNewElements(m_bAutoInclude);
}

void CSelectFieldsDlg::OnAdd()
{
    m_lboExcluded.AddSelectedItemsTo(m_lboIncluded);
    UpdateButtons();
    if ( m_lboExcluded.bEmpty() )
        m_lboIncluded.SetFocus();
}

void CSelectFieldsDlg::OnRemove()
{
    m_lboIncluded.AddSelectedItemsTo(m_lboExcluded);
    UpdateButtons();
    if ( m_lboIncluded.bEmpty() )
        m_lboExcluded.SetFocus();
}

void CSelectFieldsDlg::OnRemoveAll()
{
    // Select all items in the Included list box and call Remove.
    ItemIndex itmLast = m_lboIncluded.GetCount() - 1;
    ASSERT( itmLast >= 0 );  // The button should have been disabled
	if ( itmLast == 0 )
		m_lboIncluded.SetSel(0, TRUE);
	else
		m_lboIncluded.SelItemRange(TRUE, 0, itmLast);
    OnRemove();
}
