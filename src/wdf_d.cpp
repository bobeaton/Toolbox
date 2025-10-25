// wdf_d.cpp  Word formulas for selecting valid parses automatically

#include "stdafx.h"
#include "toolbox.h"
#include "wdf_d.h"
#include "lng.h"  // CLangEnc::iLineHeight
#include "ddx.h"
#include "tools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

// --------------------------------------------------------------------------

CWordFormulaSetListBox::CWordFormulaSetListBox(CWordFormulaSet* pwdfset, CWordFormula** ppwdf) :
    CSetListBox(pwdfset, (CSetEl**)ppwdf),
    m_pwdfset(pwdfset),
    m_plngPatterns(pwdfset->plngPatterns())
{
    m_xSymbol = 0;
    m_xPatterns = 0;
    ASSERT( m_pwdfset );
    ASSERT( m_plngPatterns );

    // 2000-09-19 TLB & MRP : Make the line height the greater of the height required for the
    // default font and the height required for the language font.
    CDC dc;
    dc.CreateCompatibleDC( NULL ); // Create a temporary screen device context for measuring font
    TEXTMETRIC tm;
    dc.GetTextMetrics( &tm ); // get text measurements
    int iDefaultLineHeight = tm.tmHeight;
    
    m_uLineHeight = max(m_plngPatterns->iLineHeight(), iDefaultLineHeight);
    ASSERT( m_uLineHeight != 0 );
    m_sPrimaryMark = "P";
    m_sEtc = ". . .";
    m_bShowExpandedList = FALSE;
}

CWordFormula* CWordFormulaSetListBox::pwdfSelected()
{
    return (CWordFormula*)pselSelected();
}

void CWordFormulaSetListBox::LangEncPatterns(CLangEnc* plng)
{
    m_plngPatterns = plng;
    ASSERT( m_plngPatterns );
    m_uLineHeight = m_plngPatterns->iLineHeight();
    ASSERT( 0 < m_uLineHeight );
}

void CWordFormulaSetListBox::InitLabels()
{
    m_xSymbol = xSubItem_Show(IDC_wdf_lblSymbol);
    m_xPatterns = xSubItem_Show(IDC_wdf_lblPatterns);
    m_xEtc = xSubItem_Show(IDC_wdf_lblEtc);
}

int CWordFormulaSetListBox::iItemHeight()
{
    return m_uLineHeight;
}

int CWordFormulaSetListBox::maxnItemsToShow() const
{
    if ( m_bShowExpandedList )
        return 3;

    return 1;
}

UINT CWordFormulaSetListBox::uItemHeight(CWordFormula* pwdf)
{
    ASSERT( pwdf );
    int maxnItems = maxnItemsToShow();
    int nItems = 0;
    UINT uHeight = 0;
    CWordPattern* pwdp = pwdf->pwdpFirst();
    for ( ; pwdp; pwdp = pwdf->pwdpNext(pwdp) )
        if ( ++nItems <= maxnItems )
            uHeight += m_uLineHeight;
        else
            break;

    if ( uHeight == 0 )
        uHeight = m_uLineHeight;
    else if ( 255 < uHeight )
        uHeight = 255;

    return uHeight;
}

void CWordFormulaSetListBox::MeasureItem(LPMEASUREITEMSTRUCT lpmis)
{
    ASSERT( lpmis );
//    const char* pszName = (const char*)lpmis->itemData; // Needs convert from UTF16 to UTF8
//    ASSERT( pszName );
    CString swName = (const wchar_t*)lpmis->itemData; // 1.4qzpg Fix U bug of crash on Formulas button in Ambig box
	Str8 sName = sUTF8( swName ); // 1.4qzpg
    CWordFormula* pwdf = (CWordFormula*)m_pwdfset->pselSearch(sName);
    ASSERT( pwdf );
    lpmis->itemHeight = uItemHeight(pwdf);
}

void CWordFormulaSetListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CWordFormula* pwdf = (CWordFormula*)pel;
    ASSERT( pwdf );

    RECT rcSymbol = rcItem;
    rcSymbol.bottom = rcSymbol.top + m_uLineHeight;
    if ( pwdf->bPrimary() )
        DrawSubItemJustify(cDC, rcSymbol, 2, m_xSymbol, m_sPrimaryMark);
    DrawSubItemJustify(cDC, rcSymbol, m_xSymbol, m_xPatterns, pwdf->sName());

    int maxnItems = maxnItemsToShow();
    int nItems = 0;
    RECT rcPattern = rcItem;
    const CWordPattern* pwdp = pwdf->pwdpFirst();
    for ( ; pwdp; pwdp = pwdf->pwdpNext(pwdp) )
        if ( ++nItems <= maxnItems )
            {
            rcPattern.bottom = rcPattern.top + m_uLineHeight;
            int xRight = 0;
            // If this is the last line and not the last item, draw ... too.
            if ( nItems == maxnItems && pwdf->pwdpNext(pwdp) )
                {
                DrawSubItemJustify(cDC, rcPattern, m_xEtc, 0, m_sEtc);
                xRight = m_xEtc;
                }
            DrawSubItemJustify(cDC, rcPattern, m_xPatterns, xRight, pwdp->sPattern(), m_plngPatterns);
            rcPattern.top = rcPattern.bottom;
            }
        else
            break;
}

BOOL CWordFormulaSetListBox::bDelete(CSetEl* psel)
{
    CWordFormula* pwdf = (CWordFormula*)psel;
    ASSERT( pwdf );
    ASSERT( !pwdf->bPrimary() );
    if ( pwdf->bUsedInPatterns() )
        {
        Str8 sMessage = _("Delete word formula?"); // 1.5.0fg 
        if ( AfxMessageBox(sMessage, MB_YESNO|MB_ICONQUESTION) != IDYES )
            return FALSE;
        }
    
    return CSetListBox::bDelete(psel);
}

BOOL CWordFormulaSetListBox::bIsBold(CSetEl* psel)
{
    CWordFormula* pwdf = (CWordFormula*)psel;
    ASSERT( pwdf );
    return (pwdf->bUsedInPatterns() || pwdf->bPrimary());
}

// --------------------------------------------------------------------------

// Add... button
CWordFormulaDlg::CWordFormulaDlg(CWordFormulaSet* pwdfset, CWordFormula** ppwdfNew) :
    CDialog(CWordFormulaDlg::IDD),
    m_pwdf(NULL),
    m_ppwdfNew(ppwdfNew),
    m_pwdfset(pwdfset)
{
    ASSERT( m_ppwdfNew );
    ASSERT( m_pwdfset );
}

// Copy... button
CWordFormulaDlg::CWordFormulaDlg(const CWordFormula* pwdfToCopyFrom, CWordFormula** ppwdfNew ) :
    CDialog(CWordFormulaDlg::IDD),
    m_pwdf(NULL),
    m_ppwdfNew(ppwdfNew),
    m_pwdfset(pwdfToCopyFrom->pwdfsetMyOwner()),
    m_sPatterns(pwdfToCopyFrom->sPatterns())
{
    ASSERT( m_pwdfset );
    ASSERT( m_ppwdfNew );
}

// Modify... button
CWordFormulaDlg::CWordFormulaDlg(CWordFormula* pwdf) :
    CDialog(CWordFormulaDlg::IDD),
    m_pwdf(pwdf),
    m_ppwdfNew(NULL),
    m_pwdfset(m_pwdf->pwdfsetMyOwner()),
    m_sSymbol(m_pwdf->sName()),
    m_sPatterns(m_pwdf->sPatterns())
{
    ASSERT( m_pwdf );
    ASSERT( m_pwdfset );
}

void CWordFormulaDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CWordFormulaDlg)
    DDX_Control(pDX, IDC_wdf_edtPatterns, m_elcPatterns);
    //}}AFX_DATA_MAP

//    Shw_DDX_TrimmedTxt(pDX, IDC_wdf_edtSymbol, m_sSymbol); // 1.4hdb Replace DDX_Trimmed in word formula symbol dlg
//	Shw_DDX_MultiLineText(pDX, IDC_wdf_edtPatterns, m_sPatterns); // 1.4hdw Replace DDX_MultiLineText in word formula pattern dlg
}

BEGIN_MESSAGE_MAP(CWordFormulaDlg, CDialog)
    //{{AFX_MSG_MAP(CWordFormulaDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWordFormulaDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
	m_sPatterns.Replace( "\n", "\r\n" ); // 1.4vxg Fix problem with word formula edit control line ends
	m_elcPatterns.SetWindowText( swUTF16( m_sPatterns ) ); // 1.4qpn // 1.4vxf 
    SetDlgItemText( IDC_wdf_edtSymbol, swUTF16( m_sSymbol ) ); // 1.4hdb // 1.4qpv
    BOOL bSetFocus = ( m_pwdf != NULL );
    if ( bSetFocus )
        {
        m_elcPatterns.SetFocus();
        m_elcPatterns.SetSel(0, 0);
        }
    return !bSetFocus;  // return FALSE if we set the focus
}

void CWordFormulaDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//	m_sPatterns = m_elcPatterns.sGetEditLngText(); // 1.4qpn // 1.4vxf 
	CString swPatterns; // 1.4vxf 
	m_elcPatterns.GetWindowText( swPatterns ); // 1.4vxf 
	m_sPatterns = sUTF8( swPatterns ); // 1.4qpn // 1.4vxf 
	m_sPatterns.Replace( "\r\n", "\n" ); // 1.4vxg Fix problem with word formula edit control line ends
//	int i = m_sPatterns.Find( "\r" ); // 1.4vxg None found, so there may never be any
//	m_sPatterns.Replace( "\r", "\n" ); // 1.4vxg Maybe good for safety, but appears unnecessary
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_wdf_edtSymbol, sDlgItem ); // 1.4qpw
	m_sSymbol = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sSymbol ); // 1.4hdc
    if ( m_pwdf )
        {
        if ( !m_pwdf->bModifyProperties(m_sSymbol, m_sPatterns) )
            return;
        }
    else
        {
        if ( !m_pwdfset->bAddWordFormula(m_sSymbol, m_sPatterns, m_ppwdfNew) )
            return;
        }
    CDialog::OnOK();
}

// --------------------------------------------------------------------------

BOOL CWordFormulaSetDlg::s_bDoModal(CWordFormulaSet* pwdfset)
{
    ASSERT( pwdfset );
    CWordFormulaSet wdfset(*pwdfset);
    CWordFormula* pwdf = wdfset.pwdfSelected();
    CWordFormulaSetListBox lboSet(&wdfset, &pwdf);
    Str8 sLabel("");
    CWordFormulaSetDlg dlg(&wdfset, lboSet, sLabel);
    int i = dlg.DoModal();

    if ( i != IDOK )
        return FALSE;

    *pwdfset = wdfset;
    return TRUE;
}

CWordFormulaSetDlg::CWordFormulaSetDlg(CWordFormulaSet* pwdfset,
        CWordFormulaSetListBox& lboSet, const Str8& sLabel) :
    CSetDlg(lboSet, 0, sLabel, IDD_WordFormulas, IDD_WordFormulas),
    m_pwdfset(pwdfset),
    m_lboFormulas(lboSet),
    m_cboLangEncPatterns(m_pwdfset->plngPatterns()->plngsetMyOwner())
{
    ASSERT( m_pwdfset );
    m_bShowExpandedList = m_pwdfset->bShowExpandedList();
}

void CWordFormulaSetDlg::DoDataExchange(CDataExchange* pDX)
{
    CSetDlg::DoDataExchange(pDX);
    
    //{{AFX_DATA_MAP(CWordFormulaSetDlg)
    DDX_Control(pDX, IDC_wdf_cboLangEncPatterns, m_cboLangEncPatterns);
    DDX_Check(pDX, IDC_wdf_chkShowExpandedList, m_bShowExpandedList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWordFormulaSetDlg, CSetDlg)
    //{{AFX_MSG_MAP(CWordFormulaSetDlg)
    ON_CBN_SELCHANGE(IDC_wdf_cboLangEncPatterns, OnSelchangeLangEncPatterns)
    ON_BN_CLICKED(IDC_wdf_chkShowExpandedList, OnShowExpandedList)
    ON_BN_CLICKED(IDC_wdf_btnPrimary, OnPrimary)
    ON_BN_CLICKED(IDC_CLEAR, OnClear)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWordFormulaSetDlg::OnInitDialog()
{
    m_lboFormulas.ShowExpandedList(m_bShowExpandedList);

    BOOL b = CSetDlg::OnInitDialog();

    m_cboLangEncPatterns.UpdateElements(m_pwdfset->plngPatterns());

    return b;  // return FALSE if we set the focus
}

void CWordFormulaSetDlg::OnOK()
{
    CSetDlg::OnOK();
    CWordFormula* pwdf = m_lboFormulas.pwdfSelected();
    m_pwdfset->SelectedWordFormula(pwdf);
    CLangEnc* plng = m_cboLangEncPatterns.plngSelected();
    m_pwdfset->LangEncPatterns(plng);
    m_pwdfset->ShowExpandedList(m_bShowExpandedList);
}

void CWordFormulaSetDlg::OnPrimary()
{
    CWordFormula* pwdf = m_lboFormulas.pwdfSelected();
    m_pwdfset->SetPrimaryWordFormula(pwdf);
    UpdateDeleteButton();  // 2000-09-20 MRP: Needs to be disabled
    m_lboFormulas.UpdateElements(pwdf);
}

void CWordFormulaSetDlg::OnClear()
{
    m_pwdfset->Clear();
    m_lboFormulas.UpdateElements(m_pwdfset->pwdfPrimary());
}

void CWordFormulaSetDlg::OnSelchangeLangEncPatterns()
{
    CLangEnc* plng = m_cboLangEncPatterns.plngSelected();
    m_lboFormulas.LangEncPatterns(plng);
    m_lboFormulas.UpdateElements(m_lboFormulas.pwdfSelected());
    m_pwdfset->LangEncPatterns(plng);
}

void CWordFormulaSetDlg::OnShowExpandedList()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
    m_lboFormulas.ShowExpandedList(m_bShowExpandedList);
    m_lboFormulas.UpdateElements(m_lboFormulas.pwdfSelected());
}
