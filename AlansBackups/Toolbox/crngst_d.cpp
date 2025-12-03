// File : CRngSt_d.cpp
// Implementation of CRngSt_d.h

#include "stdafx.h"
#include "toolbox.h"
#include "crngset.h"  // CRangeSet
#include "CRngSt_d.h"
#include "mkr.h"  // CMarker
#include "mkr_d.h" // CMarkerPropertiesSheet
#include "project.h"  // Shw_pProject()->iLangCtrlHeight()
#include "help_ids.h"

// --------------------------------------------------------------------------
CDataItemSetListBox::CDataItemSetListBox(CDataItemSet* pdatset) :
    CSetListBox(pdatset),
    m_plng(NULL),
    m_pfnt(NULL)
{
}

void CDataItemSetListBox::SetLangEnc(CLangEnc* plng)
{
    m_plng = plng;
}

void CDataItemSetListBox::SetFont(CFont* pfnt)
{
    m_pfnt = pfnt;
}

int CDataItemSetListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CDataItemSetListBox::InitLabels()
{
}

void CDataItemSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CDataItemEl* pdat = (CDataItemEl*)pel;
    
    DrawSubItemJustify(cDC, rcItem, 1, 0, pdat->sName(), m_plng);
//    DrawSubItemJustify(cDC, rcItem, 1, 0, pdat->sName(), m_pfnt, m_plng);
}

// --------------------------------------------------------------------------
CRangeSetListBox::CRangeSetListBox(CRangeSet* prngset, CRangeEl** pprng) :
    CSetListBox(prngset, (CSetEl**)pprng),
    m_pmkr(prngset->pmkr())
{
    ASSERT( m_pmkr );
}

CRangeSetListBox::CRangeSetListBox(CRangeSet* prngset) :
    CSetListBox(prngset),
    m_pmkr(prngset->pmkr())
{
    ASSERT( m_pmkr );
}

int CRangeSetListBox::iItemHeight()
{
    return Shw_pProject()->iLangCtrlHeight();
}

void CRangeSetListBox::InitLabels()
{
}

void CRangeSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CRangeEl* prng = (CRangeEl*)pel;
    
    DrawSubItemJustify(cDC, rcItem, 1, 0, prng->sName(), m_pmkr);
}


//************************************************************************
//************************************************************************
// Main Dialog 'Range Set' (CMarkerRangeSetPage)
//************************************************************************

CMarkerRangeSetPage::CMarkerRangeSetPage(BOOL& bUseRangeSet, CDataItemSet& datset,
        const BOOL& bMultipleItemData, const BOOL& bMultipleWordItems,
        const CMarker* pmkr, BOOL bDisableRangeSetRemove)
    : CPropertyPage(IDD),
    m_bWasActive(FALSE),
    m_datset(datset),
    m_bMultipleItemData(bMultipleItemData),
    m_bMultipleWordItems(bMultipleWordItems),
    m_pmkr(pmkr),
    m_bEnableUseRangeSet(!bDisableRangeSetRemove),
    m_lboRangeSet(&datset),
    m_bUseRangeSet(bUseRangeSet),
	// 1998-02-10 MRP: If this is a newly added marker,
    // m_pmkr is still NULL, so use the default label and
    // disable the Build Range Set button.
	m_bValidToBuild(( m_pmkr ) ? TRUE : FALSE),
	m_bValidToConvert(datset.bCharacterItems())
{
    // Should only be able to turn off this check box if there is already a range set
    // that we are trying to protect from deletion.
    if ( !bUseRangeSet )
        ASSERT(m_bEnableUseRangeSet);
}

//---------------------------------------

void CMarkerRangeSetPage::SetOwner(CMarkerPropertiesSheet* pwndMyOwner)
{
    m_pwndMyOwner = pwndMyOwner;
    ASSERT(m_pwndMyOwner);
}

//---------------------------------------

void CMarkerRangeSetPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_edtElement, m_elcRangeSetEl);
    DDX_Control(pDX, IDC_lboRangeSet, m_lboRangeSet);
	//{{AFX_DATA_MAP(CMarkerRangeSetPage)
    DDX_Control(pDX, IDC_chkUseRangeSet, m_chkUseRangeSet);
	DDX_Check(pDX, IDC_chkUseRangeSet, m_bUseRangeSet);
    DDX_Control(pDX, IDC_btnAdd, m_btnAdd);
    DDX_Control(pDX, IDC_btnReplace, m_btnReplace);
    DDX_Control(pDX, IDC_btnDelete, m_btnDelete);
    DDX_Control(pDX, IDC_btnClear, m_btnClear);
    DDX_Control(pDX, IDC_btnBuildRangeSet, m_btnBuildRangeSet);  // 1998-02-10 MRP
    DDX_Control(pDX, IDC_lblCharRangeSet, m_lblCharRangeSet);
    DDX_Control(pDX, IDC_btnMoreInfo, m_btnMoreInfo);
    DDX_Control(pDX, IDC_btnConvertCharRangeSet, m_btnConvertCharRangeSet);
    //}}AFX_DATA_MAP
}

//----------------------------------------

BEGIN_MESSAGE_MAP(CMarkerRangeSetPage, CDialog)
    //{{AFX_MSG_MAP(CMarkerRangeSetPage)
    ON_BN_CLICKED(IDC_chkUseRangeSet, OnUseRangeSet)
    ON_EN_CHANGE(IDC_edtElement, UpdateButtons)
    ON_LBN_SELCHANGE(IDC_lboRangeSet, OnSelchangeRangeSet)
    ON_BN_CLICKED(IDC_btnAdd, OnAdd)
    ON_BN_CLICKED(IDC_btnReplace, OnReplace)
    ON_BN_CLICKED(IDC_btnDelete, OnDelete)
    ON_BN_CLICKED(IDC_btnClear, OnClear)
    ON_BN_CLICKED(IDC_btnBuildRangeSet, OnBuildRangeSet)
    ON_BN_CLICKED(IDC_btnMoreInfo, OnMoreInfo)
    ON_BN_CLICKED(IDC_btnConvertCharRangeSet, OnConvertCharRangeSet)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP() 

//--------------------------------------------
BOOL CMarkerRangeSetPage::OnInitDialog()
{
    BOOL bRet = CPropertyPage::OnInitDialog();

    m_chkUseRangeSet.EnableWindow(m_bEnableUseRangeSet);
    
    m_lblCharRangeSet.ShowWindow(m_bValidToConvert);
    m_btnMoreInfo.ShowWindow(m_bValidToConvert);
    m_btnConvertCharRangeSet.ShowWindow(m_bValidToConvert);

    SetLanguageControlFonts();
    m_lboRangeSet.InitListBox();

    EnableControls();

    m_bWasActive = TRUE;

    return bRet;
}

//--------------------------------------------
BOOL CMarkerRangeSetPage::OnSetActive()
{
    BOOL bRet = CPropertyPage::OnSetActive();

    SetLanguageControlFonts();
    return bRet;
}

//--------------------------------------------
void CMarkerRangeSetPage::SetLanguageControlFonts()
{
    ASSERT(m_pwndMyOwner);

    CLangEnc* plng = (CLangEnc*) m_pwndMyOwner->plng(); // Cast away const-ness
    m_elcRangeSetEl.SetLangEnc(plng);
    m_lboRangeSet.SetLangEnc(plng);
	CFont* pfnt = (CFont*) m_pwndMyOwner->pfnt(); // Cast away const-ness
    if ( !pfnt )
        pfnt = (CFont*) plng->pfntDlg(); // Cast away const-ness
    ASSERT(pfnt);
}

//--------------------------------------------

void CMarkerRangeSetPage::EnableControls()
{
    m_elcRangeSetEl.EnableWindow(m_bUseRangeSet);
    m_lboRangeSet.EnableWindow(m_bUseRangeSet);
    if ( m_bUseRangeSet )
        UpdateButtons();
    else
        {
        m_btnAdd.EnableWindow(FALSE);
        m_btnReplace.EnableWindow(FALSE);
        m_btnDelete.EnableWindow(FALSE);
        m_btnClear.EnableWindow(FALSE);
        }
    m_btnBuildRangeSet.EnableWindow(m_bUseRangeSet && m_bValidToBuild);
    m_lblCharRangeSet.EnableWindow(m_bUseRangeSet && m_bValidToConvert);
    m_btnMoreInfo.EnableWindow(m_bUseRangeSet && m_bValidToConvert);
    m_btnConvertCharRangeSet.EnableWindow(m_bUseRangeSet && m_bValidToConvert);
}

//------------------------------------------

void CMarkerRangeSetPage::UpdateButtons()
{
    ASSERT(m_bUseRangeSet);

	Str8 sEditElement = m_elcRangeSetEl.sGetEditLngText(); // 1.4qpn
    sEditElement.TrimLeft();
    sEditElement.TrimRight();
    BOOL bValidEditElement = TRUE; // (!sEditElement.IsEmpty() && !m_datset.bMatch(sEditElement)); // 1.4sp Fix U bug of range set buttons not enabled
    m_btnAdd.EnableWindow(bValidEditElement);
    int iSel = m_lboRangeSet.GetCurSel();
    if ( iSel != LB_ERR )
        {
        m_btnReplace.EnableWindow(bValidEditElement);
        m_btnDelete.EnableWindow(TRUE);
        m_btnClear.EnableWindow(TRUE);
        }
    else
        {
        m_btnReplace.EnableWindow(FALSE);
        m_btnDelete.EnableWindow(FALSE);
        m_btnClear.EnableWindow(FALSE);
        }
    m_btnAdd.EnableWindow(TRUE); // 1.4sp Fix U bug of range set buttons not enabled
}

//----------------------------------------

void CMarkerRangeSetPage::OnUseRangeSet()
{
    UpdateData(TRUE);
    EnableControls();
}

//------------------------------------------

void CMarkerRangeSetPage::OnSelchangeRangeSet()
{
    ASSERT(m_bUseRangeSet);

    int iSel = m_lboRangeSet.GetCurSel();
    if ( iSel != LB_ERR )
        {
		CString swText; // 1.4qvh Upgrade GetText for Unicode build
        m_lboRangeSet.GetText( iSel, swText ); // 1.4ed Change listbox get text to not use CStrg // 1.4qvn
        Str8 sSelElement =  sUTF8( swText ); // 1.4qvh
        m_elcRangeSetEl.SetEditLngText( sSelElement ); // 1.4qpn
        }
    UpdateButtons();
}

//------------------------------------------

void CMarkerRangeSetPage::OnApplyEdit(BOOL bReplaceExisting)
{
    ASSERT(m_bUseRangeSet);

	Str8 sEditElement = m_elcRangeSetEl.sGetEditLngText(); // 1.4qpn
    sEditElement.TrimLeft();
    sEditElement.TrimRight();
    BOOL bValidEditElement = (!sEditElement.IsEmpty() && !m_datset.bMatch(sEditElement)); // 1.4sp
    if ( !bValidEditElement ) // 1.4sp
		return; // 1.4sp
    if ( m_datset.bCharacterItems() && sEditElement.GetLength() != 1 )
        {
        AfxMessageBox( _("Character range set element must be single character."), MB_ICONINFORMATION);
        m_elcRangeSetEl.SetSel(0, -1, TRUE);
        }
    else
        {
        CDataItemEl* pdat;
        ASSERT(m_datset.bAdd(sEditElement, &pdat));
        if ( bReplaceExisting )
            {
            CSetEl* psel = m_lboRangeSet.pselSelected();
            m_datset.Delete(&psel);
            }
        m_lboRangeSet.UpdateElements(pdat);
        m_elcRangeSetEl.SetEditLngText( "" ); // 1.4qpn
        UpdateButtons();
        }
    m_elcRangeSetEl.SetFocus();
}

//------------------------------------------

void CMarkerRangeSetPage::OnAdd()
{
    OnApplyEdit(FALSE);
}

//------------------------------------------

void CMarkerRangeSetPage::OnReplace()
{
    OnApplyEdit(TRUE);
}

//------------------------------------------

void CMarkerRangeSetPage::OnDelete()
{
    ASSERT(m_bUseRangeSet);

    CSetEl* psel = m_lboRangeSet.pselSelected();
//    m_elcRangeSetEl.SetWindowText(psel->sKey()); // 1.4qpn
    m_elcRangeSetEl.SetEditLngText( psel->sKey() ); // 1.4qpn
    m_datset.Delete(&psel);
    m_lboRangeSet.UpdateElements(psel);
    UpdateButtons();
}

//------------------------------------------

void CMarkerRangeSetPage::OnClear()
{
    ASSERT(m_bUseRangeSet);

    m_datset.DeleteAll();
	m_lboRangeSet.ResetContent();
    UpdateButtons();
//  m_elcRangeSetEl.SetWindowText(""); // 1.4qpn
    m_elcRangeSetEl.SetEditLngText( "" ); // 1.4qpn
    m_elcRangeSetEl.SetFocus();
}

//-------------------------------------------
void CMarkerRangeSetPage::OnBuildRangeSet()
{
    ASSERT(m_bUseRangeSet);

    ASSERT( m_pmkr );  // 1998-02-10 MRP

    const CSetEl* psel = m_lboRangeSet.pselSelected();
    int iItemsAdded;
    int iItemsTotal;
    int iItemsNotFound;
    m_datset.BuildDataItemSet(m_pmkr, m_bMultipleItemData,
                              m_bMultipleWordItems, iItemsAdded,
                              iItemsTotal, iItemsNotFound);
    
    Str8 sInformation;
    sInformation = _("Summary of range set elements."); // 1.5.0fg 
    
    Str8 sPartOfMessage;                
    char cTemp[33];
    ltoa ( (long)iItemsTotal, cTemp, 10 );
	sInformation = sInformation + "\n" + _("Total number:") + " " + (const char*)cTemp; // 1.5.0fg 
     
    ltoa( (long)iItemsAdded, cTemp, 10 );
	sInformation = sInformation + "\n" + _("Newly added:") + " " + (const char*)cTemp; // 1.5.0fg 

    ltoa( (long)iItemsNotFound, cTemp, 10 );
	sInformation = sInformation + "\n" + _("Not found in the data:") + " " + (const char*)cTemp; // 1.5.0fg 
    AfxMessageBox(sInformation, MB_ICONINFORMATION);
    
    if ( !psel )
    {
        psel = m_datset.pdatFirst();
		if ( psel ) // 1.4sv Fix bug of crash on build range set from empty fields
		    m_elcRangeSetEl.SetEditLngText( psel->sItem() ); // 1.4qpn
    }
    m_lboRangeSet.UpdateElements(psel);
    UpdateButtons();
}

//------------------------------------------

void CMarkerRangeSetPage::OnMoreInfo()
{
    ASSERT(m_bUseRangeSet);

    AfxGetApp()->WinHelp(HT_CONVERT_CHAR_RANGE_SET);
}

//------------------------------------------

void CMarkerRangeSetPage::OnConvertCharRangeSet()
{
    ASSERT(m_bUseRangeSet);
    m_datset.ConvertFromCharacterBasedSet();
    m_btnMoreInfo.SetFocus();
    m_bValidToConvert = FALSE;
    m_lblCharRangeSet.ShowWindow(FALSE);
    m_btnConvertCharRangeSet.EnableWindow(m_bValidToConvert);
}

//************************************************************************
//************************************************************************
// Dialog 'Insert from Range Set' (CRangeSetDlg)
//************************************************************************
//  implemented by Brian Yoder  14-Jul-95

CRangeSetDlg::CRangeSetDlg(CRangeSet* prngset, Str8& s)
    : CDialog(CRangeSetDlg::IDD),
    m_prngset(prngset),
    m_s(s),
    m_lboRangeSet(m_prngset)
{
    ASSERT(m_prngset);
    //09-17-1997 Construct the Title of the Editfield
	m_sTitle = _("&Range Set"); // 1.5.0fd 
    //{{AFX_DATA_INIT(CRangeSetDlg)
    //}}AFX_DATA_INIT
}

//-------------------------------------

void CRangeSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRangeSetDlg)
//    DDX_Txt(pDX, IDC_RANGESET, m_sTitle);
    DDX_Control(pDX, IDC_RNGSET_LIST, m_lboRangeSet);
    //}}AFX_DATA_MAP
}

//--------------------------------------

BEGIN_MESSAGE_MAP(CRangeSetDlg, CDialog)
    //{{AFX_MSG_MAP(CRangeSetDlg)
    ON_LBN_DBLCLK(IDC_RNGSET_LIST, OnDblclkRngsetList)
    ON_BN_CLICKED(ID_MODIFY, OnModify)   //09-17-1997
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//--------------------------------------

BOOL CRangeSetDlg::OnInitDialog()
{ 
    CDialog::OnInitDialog();
    SetDlgItemText( IDC_RANGESET, swUTF16( m_sTitle ) ); // 1.4qpv
    
    m_lboRangeSet.InitListBox();
   
    return TRUE;  // return TRUE  unless you set the focus to a control
}

//----------------------------------------

void CRangeSetDlg::OnOK()
{
//    GetDlgItemText( IDC_RANGESET, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_RANGESET, sDlgItem ); // 1.4qpw
	m_sTitle = sUTF8( sDlgItem ); // 1.4qpw

    // 2000-06-29 MRP: Wait to get the selected range set item
    // until the user clicks OK or double-clicks an item in the list.
    // The original code was based on changing the selected item,
    // but this fails if the user simply clicks OK,
    // because the initially selected item is the desired one.
    // m_s = m_sText;
    CRangeEl* prng = m_lboRangeSet.prngSelected();
    if ( prng )
        m_s = prng->sName();

    CDialog::OnOK();
}

//----------------------------------------

void CRangeSetDlg::OnDblclkRngsetList()
{
    OnOK();
}

//----------------------------------------

 //09-17-1997
void CRangeSetDlg::OnModify()
{
    // 2000-06-29 MRP: The item string doesn't need to be a class member.
    Str8 sItem;
    CRangeEl* prng = m_lboRangeSet.prngSelected();
    if ( prng )
        sItem = prng->sName();

    CMarkerPropertiesSheet dlg( m_prngset->pmkr(), CMarkerPropertiesSheet::pagRangeSet, TRUE, TRUE );
    dlg.DoModal();

    ASSERT(m_prngset == m_prngset->pmkr()->prngset());
    // Attempt to reset at or near previously selected element
    prng = m_prngset->prngFindClosestMatch(sItem);
      // Update View
    m_lboRangeSet.UpdateElements(prng);
}


// --------------------------------------------------------------------------

CRangeSetInconsistencyDlg::CRangeSetInconsistencyDlg(CShwView* pview, CRecPos& rpsStart,
                                                     CRecPos& rpsEnd, CCheckingState *pcks,
                                                     BOOL& bNeedToRecheck, 
													 Str8 sMessage,  // 1.4heq Add message arg to inconsistency dlg func
													 BOOL bExplicitNewlines,  // 1.4heq Add bool arg to inconsistency dlg func
													 Str8 s) :
    CInconsistencyDlg(CRangeSetInconsistencyDlg::IDD, pview, rpsStart, rpsEnd, pcks, bNeedToRecheck, sMessage, bExplicitNewlines, s), // 1.4heq
    m_prngset(rpsStart.pfld->pmkr()->prngset()),
    // 01/07/2000 - TLB: Have to initialize to NULL here because older versions of the compiler
    // can't accept a call to a base class method in the initializations. Real initialization happens in
    // InitDialog function
    m_prng(NULL),
    m_lboRangeSet(m_prngset, &m_prng)
{
}

CRangeSetInconsistencyDlg::~CRangeSetInconsistencyDlg()
{
}

BEGIN_MESSAGE_MAP(CRangeSetInconsistencyDlg, CInconsistencyDlg)
    //{{AFX_MSG_MAP(CRangeSetInconsistencyDlg)
    ON_LBN_SELCHANGE(IDC_cck_lboRangeSet, OnSelchangeRangeSet)
    ON_LBN_DBLCLK(IDC_cck_lboRangeSet, OnDblclkRangeSet)
    ON_BN_CLICKED(IDC_cck_btnAdd, OnAddToRangeSet)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CRangeSetInconsistencyDlg::DoDataExchange(CDataExchange* pDX)
{
    CInconsistencyDlg::DoDataExchange(pDX);

    //{{AFX_DATA_MAP(CRangeSetInconsistencyDlg)
    DDX_Control(pDX, IDC_cck_lboRangeSet, m_lboRangeSet);
    DDX_Control(pDX, IDC_cck_btnAdd, m_btnAdd);
    //}}AFX_DATA_MAP
}

BOOL CRangeSetInconsistencyDlg::OnInitDialog()
{
    BOOL bRet = CInconsistencyDlg::OnInitDialog();

    m_lboRangeSet.InitListBox();

    // 01/07/2000 - TLB: Had to do this initialization here because older versions of the compiler
    // can't accept a call to a base class method in the initializations.
    m_prng = m_prngset->prngFindClosestMatch(sFailedData());
    m_lboRangeSet.UpdateElements(m_prng);
//    m_btnAdd.EnableWindow(!bReplacementModified() && !sFailedData().IsEmpty()); // 1.5.5a 
    m_btnAdd.EnableWindow(!sFailedData().IsEmpty()); // 1.5.5a Fix bug of not allowing add to range set

    return bRet;
}

void CRangeSetInconsistencyDlg::OnChangeReplacement()
{
    CInconsistencyDlg::OnChangeReplacement();
    // Add to Range Set is a valid choice if the replacement string matches the original
    // inconsistency and it isn't empty (which means we have a data property inconsistency).
//    if ( !bReplacementModified() ) // 1.5.5a Fix bug of not allowing add to range set
    {
        m_btnAdd.EnableWindow(!sFailedData().IsEmpty());
        SetDefID(IDC_cck_btnAdd); // Set Add to range set as default button
    }
//    else // 1.5.5a 
//        m_btnAdd.EnableWindow(FALSE); // 1.5.5a 

    // *** Connect to scrolling mechanism in CSetListBox.
    // But don't cycle between events in edit and list box!
    const char* psz = sReplacement();
    CSetEl* psel = m_prngset->pselSearchLongest(&psz);
    if ( psel )
        m_lboRangeSet.UpdateElements(psel);
}

void CRangeSetInconsistencyDlg::OnSelchangeRangeSet()
{
    if ( bReadOnly() )
        return;

    CRangeEl* prng = m_lboRangeSet.prngSelected();
    if ( !prng )
        return;

    const Str8& sSelection = prng->sName();
    SetReplacementText(sSelection);
}

void CRangeSetInconsistencyDlg::OnDblclkRangeSet()
{
    // OnSelChangeRangeSet and OnChangeReplacement have already been called.
    if ( !bReadOnly() )
        OnReplace();
}

void CRangeSetInconsistencyDlg::OnAddToRangeSet()
{
    // Because the dialog box closes and the check continues,
    // it isn't necessary to reset the contents of the list box.

    // If the inconsistency consists of multiple words and the data field is only
    // supposed to take one word, bAdd will return FALSE and we don't close the dialog.
    ASSERT (!sFailedData().IsEmpty());
    ASSERT (!m_prngset->bMatch(sFailedData()) );
    if ( !m_prngset->bAdd(sFailedData()) )
        AfxMessageBox(_("This data field does not allow range set elements consisting of multiple words."), MB_ICONINFORMATION);
    else
        OnOK();
}

void CRangeSetInconsistencyDlg::OnModify()
{
    AfxMessageBox("Not yet implemented", MB_ICONINFORMATION);

//    CMarkerPropertiesSheet dlg( m_prngset->pmkr(), CMarkerPropertiesSheet::pagRangeSet );
//    dlg.DoModal();

    // Update View
    // If this is ever implemented, use m_prngset->prngFindClosestMatch to attempt to
    // reset selection at or near the failed data.
    m_lboRangeSet.UpdateElements(NULL);
}

// --------------------------------------------------------------------------

CCharRangeSetInconsistencyDlg::CCharRangeSetInconsistencyDlg(CShwView* pview, CRecPos& rpsStartItem,
                                                             CRecPos& rpsEndItem, CCheckingState *pcks,
                                                             BOOL& bNeedToRecheck, 
															 int iInconsistentChar) :
    CRangeSetInconsistencyDlg(pview, rpsStartItem, rpsEndItem, pcks, bNeedToRecheck,
		_("Character not found in range set: %s"), // 1.4vc  // 1.5.0fg 
		FALSE,
		Str8(rpsStartItem.psz()+iInconsistentChar, 1)),
    m_iInconsistentChar(iInconsistentChar)
{
    ASSERT(iInconsistentChar >= 0);
    ASSERT(rpsStartItem.iChar + iInconsistentChar <= rpsEndItem.iChar);
}

CCharRangeSetInconsistencyDlg::~CCharRangeSetInconsistencyDlg()
{
}

BOOL CCharRangeSetInconsistencyDlg::OnInitDialog()
{
    CRangeSetInconsistencyDlg::OnInitDialog();

    // In order to select only the white space in the replacement
    // it is necessary to first set focus explicitly, then select the text,
    // and finally return FALSE at the end of this function.
    EditReplacementSetFocus();
    EditReplacementSetSel(m_iInconsistentChar, m_iInconsistentChar + 1);

    return FALSE;
}

void CCharRangeSetInconsistencyDlg::OnSelchangeRangeSet()
{
    if ( sFailedData().GetLength() == 1 )
        CRangeSetInconsistencyDlg::OnSelchangeRangeSet();
    // Otherwise, do nothing
}

void CCharRangeSetInconsistencyDlg::OnDblclkRangeSet()
{
    if ( sFailedData().GetLength() == 1 )
        CRangeSetInconsistencyDlg::OnDblclkRangeSet();
    // Otherwise, do nothing
}

void CCharRangeSetInconsistencyDlg::OnAddToRangeSet()
{
    // Because the dialog box closes and the check continues,
    // it isn't necessary to reset the contents of the list box.

    ASSERT( !m_prngset->bMatch(sInconsistentChar()) );
    ASSERT( m_prngset->bAdd(sInconsistentChar()) );
    // Since there may be more invalid characters, tell caller to recheck this item
    m_bNeedToRecheck = TRUE;
    OnOK();
}
