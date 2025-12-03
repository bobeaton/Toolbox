// pgs_d.cpp  Page Setup dialog box for export as Rich Text Format

#include "stdafx.h"
#include "toolbox.h"
#include "pgs.h"
#include "pgs_d.h"  // RtfPageSetup
#include "tools.h"
#include "resource.h"

#define CRestrictedEdit CEdit

// ==========================================================================

class CDocumentPage : public CPropertyPage
{
private:
    CRtfPageSetupSheet* m_pdlgMyOwner;
    BOOL m_bWasActive; // reflects whether this page was ever active

    RtfPageSetup* m_ppgs;
    int m_iPaperSize;
    RtfUnits m_unxPaperWidth;
    RtfUnits m_unyPaperHeight;
    RtfUnits m_unyTopMargin;
    RtfUnits m_unyBottomMargin;
    RtfUnits m_unxLeftMargin;  // Inside, if Mirror Margins
    RtfUnits m_unxRightMargin;  // Outside, if Mirror Margins
    RtfUnits m_unxGutter;
    BOOL m_bMirrorMargins;

// Dialog Data
    //{{AFX_DATA(CDocumentPage)
    enum { IDD = IDD_RTF_DocumentPage };
    CComboBox m_cboPaperSize;
    CStatic m_lblMeasurementUnit;
    CRestrictedEdit m_edtPaperWidth;
    CRestrictedEdit m_edtPaperHeight;
    CRestrictedEdit m_edtTopMargin;
    CRestrictedEdit m_edtBottomMargin;
    CStatic m_lblLeftInside;
    CRestrictedEdit m_edtLeftMargin;
    CStatic m_lblRightOutside;
    CRestrictedEdit m_edtRightMargin;
    CRestrictedEdit m_edtGutter;
    CButton m_chkMirrorMargins;
    //}}AFX_DATA

public:
    CDocumentPage(CRtfPageSetupSheet* pdlgMyOwner);

    void OK();
    void ConvertMeasurements(BOOL bToCentimeters);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();

    // Generated message map functions
    //{{AFX_MSG(CDocumentPage)
    afx_msg void OnSelchangePaperSize();
    afx_msg void OnMirrorMargins();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un);
    void SetPaper();
    void SetLeftRightMarginLabels();
};  // class CDocumentPage


// --------------------------------------------------------------------------

static const char s_pszValidDecimal[] = ".0123456789";
static const int s_maxlenDecimal = 6;

CDocumentPage::CDocumentPage(CRtfPageSetupSheet* pdlgMyOwner) :
    CPropertyPage(CDocumentPage::IDD),
    m_pdlgMyOwner(pdlgMyOwner),
    m_bWasActive(FALSE),
    m_ppgs(m_pdlgMyOwner->ppgs()) //,
//    m_edtPaperWidth(s_pszValidDecimal),
//    m_edtPaperHeight(s_pszValidDecimal),
//    m_edtTopMargin(s_pszValidDecimal),
//    m_edtBottomMargin(s_pszValidDecimal),
//    m_edtLeftMargin(s_pszValidDecimal),
//    m_edtRightMargin(s_pszValidDecimal),
//    m_edtGutter(s_pszValidDecimal)
{
    ASSERT( m_ppgs );
    m_iPaperSize =      m_ppgs->m_iPaperSize;
    m_unxPaperWidth =   m_ppgs->m_unxPaperWidth;
    m_unyPaperHeight =  m_ppgs->m_unyPaperHeight;
    m_unyTopMargin =    m_ppgs->m_unyTopMargin;
    m_unyBottomMargin = m_ppgs->m_unyBottomMargin;
    m_unxLeftMargin =   m_ppgs->m_unxLeftMargin;
    m_unxRightMargin =  m_ppgs->m_unxRightMargin;
    m_unxGutter =       m_ppgs->m_unxGutter;
    m_bMirrorMargins =  m_ppgs->m_bMirrorMargins;
}


BEGIN_MESSAGE_MAP(CDocumentPage, CPropertyPage)
    //{{AFX_MSG_MAP(CDocumentPage)
    ON_BN_CLICKED(IDC_RTF_chkMirrorMargins, OnMirrorMargins)
    ON_CBN_SELCHANGE(IDC_RTF_cboPaperSize, OnSelchangePaperSize)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDocumentPage::DoDataExchange(CDataExchange* pDX)
{
    const RtfUnits minun = RtfPageSetup::s_minun();
    const RtfUnits maxun = RtfPageSetup::s_maxun(m_pdlgMyOwner->bCentimeters());

    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDocumentPage)
    DDX_CBIndex(pDX, IDC_RTF_cboPaperSize, m_iPaperSize);
    DDX_Control(pDX, IDC_RTF_cboPaperSize, m_cboPaperSize);
    DDX_Control(pDX, IDC_RTF_lblMeasurementUnit, m_lblMeasurementUnit);

    DDX_Control(pDX, IDC_RTF_edtPaperWidth, m_edtPaperWidth);
//    DDX_RtfUnits(pDX, IDC_RTF_edtPaperWidth, m_unxPaperWidth);
//    DDV_MinMaxDouble(pDX, m_unxPaperWidth, minun, maxun);

    DDX_Control(pDX, IDC_RTF_edtPaperHeight, m_edtPaperHeight);
//    DDX_RtfUnits(pDX, IDC_RTF_edtPaperHeight, m_unyPaperHeight);
//    DDV_MinMaxDouble(pDX, m_unyPaperHeight, minun, maxun);

    DDX_Control(pDX, IDC_RTF_edtTopMargin, m_edtTopMargin);
//    DDX_RtfUnits(pDX, IDC_RTF_edtTopMargin, m_unyTopMargin);
//    DDV_MinMaxDouble(pDX, m_unyTopMargin, minun, m_unyPaperHeight);

    DDX_Control(pDX, IDC_RTF_edtBottomMargin, m_edtBottomMargin);
//    DDX_RtfUnits(pDX, IDC_RTF_edtBottomMargin, m_unyBottomMargin);
//    DDV_MinMaxDouble(pDX, m_unyBottomMargin, minun, m_unyPaperHeight);

    DDX_Control(pDX, IDC_RTF_lblLeftInside, m_lblLeftInside);
    DDX_Control(pDX, IDC_RTF_edtLeftMargin, m_edtLeftMargin);
//    DDX_RtfUnits(pDX, IDC_RTF_edtLeftMargin, m_unxLeftMargin);
//    DDV_MinMaxDouble(pDX, m_unxLeftMargin, minun, m_unxPaperWidth);

    DDX_Control(pDX, IDC_RTF_lblRightOutside, m_lblRightOutside);
    DDX_Control(pDX, IDC_RTF_edtRightMargin, m_edtRightMargin);
//    DDX_RtfUnits(pDX, IDC_RTF_edtRightMargin, m_unxRightMargin);
//    DDV_MinMaxDouble(pDX, m_unxRightMargin, minun, m_unxPaperWidth);

    DDX_Control(pDX, IDC_RTF_edtGutter, m_edtGutter);
//    DDX_RtfUnits(pDX, IDC_RTF_edtGutter, m_unxGutter);
//    DDV_MinMaxDouble(pDX, m_unxGutter, 0.0, m_unxPaperWidth);

    DDX_Control(pDX, IDC_RTF_chkMirrorMargins, m_chkMirrorMargins);
    DDX_Check(pDX, IDC_RTF_chkMirrorMargins, m_bMirrorMargins);
    //}}AFX_DATA_MAP
}

void CDocumentPage::DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un)
{
    m_pdlgMyOwner->DDX_RtfUnits(pDX, idc, un);
}


static const int s_numPaperSizes = 5;  // 1998-10-01 MRP: For French version

static const char* s_apszPaperSize[s_numPaperSizes] = // 1.4ep
	{
	_("Letter  8.5 x 11 in"),
	_("Legal  8.5 x 14 in"),
	_("A4  21 x 29.7 cm"),
	_("Custom size:  in"),
	_("Custom size:  cm")
	};
	
BOOL CDocumentPage::OnInitDialog()
{
    m_bWasActive = TRUE;
    BOOL b = CPropertyPage::OnInitDialog();

	// 1998-10-01 MRP: For French version
    int itm = 0;
    for ( ; itm != s_numPaperSizes; itm++ )
    	{
	 	Str8 sItem;
    	sItem = s_apszPaperSize[itm]; // 1.4ep
        m_cboPaperSize.InsertString(itm, swUTF16( sItem ) ); // 1.4qre
        }
	m_cboPaperSize.SetCurSel(m_iPaperSize);

//    m_edtPaperWidth.LimitText(s_maxlenDecimal);
//    m_edtPaperHeight.LimitText(s_maxlenDecimal);
//    m_edtTopMargin.LimitText(s_maxlenDecimal);
//    m_edtBottomMargin.LimitText(s_maxlenDecimal);
//    m_edtLeftMargin.LimitText(s_maxlenDecimal);
//    m_edtRightMargin.LimitText(s_maxlenDecimal);
//    m_edtGutter.LimitText(s_maxlenDecimal);

	Str8 sUnits;
    RtfPageSetup::s_RtfUnitsToString(m_unxPaperWidth, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtPaperWidth, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unyPaperHeight, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtPaperHeight, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unyTopMargin, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtTopMargin, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unyBottomMargin, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtBottomMargin, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unxLeftMargin, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtLeftMargin, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unxRightMargin, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtRightMargin, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unxGutter, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtGutter, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    
	SetLeftRightMarginLabels();
    SetPaper();

    return b;
}

void CDocumentPage::OnOK()
{
    CPropertyPage::OnOK();  // Calls CDocumentPage::DoDataExchange
    m_pdlgMyOwner->OK();  // Calls CDocumentPage::OK and CSectionPage::OK
}

void CDocumentPage::OK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;

//	GetDlgItemText( IDC_RTF_edtPaperWidth, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_RTF_edtPaperWidth, sDlgItem ); // 1.4qpw
	m_unxPaperWidth = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtPaperHeight, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtPaperHeight, sDlgItem ); // 1.4qpw
	m_unyPaperHeight = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtTopMargin, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtTopMargin, sDlgItem ); // 1.4qpw
	m_unyTopMargin = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtBottomMargin, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtBottomMargin, sDlgItem ); // 1.4qpw
	m_unyBottomMargin = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtLeftMargin, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtLeftMargin, sDlgItem ); // 1.4qpw
	m_unxLeftMargin = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtRightMargin, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtRightMargin, sDlgItem ); // 1.4qpw
	m_unxRightMargin = atof( sUTF8( sDlgItem ) ); // 1.4qpw
//	GetDlgItemText( IDC_RTF_edtGutter, (char*)buffer, DLGBUFMAX ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved
	GetDlgItemText( IDC_RTF_edtGutter, sDlgItem ); // 1.4qpw
	m_unxGutter = atof( sUTF8( sDlgItem ) ); // 1.4qpw

    // Note: UpdateData has already been called
    m_ppgs->m_iPaperSize =      m_iPaperSize;
    m_ppgs->m_unxPaperWidth =   m_unxPaperWidth;
    m_ppgs->m_unyPaperHeight =  m_unyPaperHeight;
    m_ppgs->m_unyTopMargin =    m_unyTopMargin;
    m_ppgs->m_unyBottomMargin = m_unyBottomMargin;
    m_ppgs->m_unxLeftMargin =   m_unxLeftMargin;
    m_ppgs->m_unxRightMargin =  m_unxRightMargin;
    m_ppgs->m_unxGutter =       m_unxGutter;
    m_ppgs->m_bMirrorMargins =  m_bMirrorMargins;
}

void CDocumentPage::OnSelchangePaperSize()
{
    UpdateData(TRUE);  // dialog box controls to object members
    m_unxPaperWidth = RtfPageSetup::s_unxPaperWidth(m_iPaperSize);
    m_unyPaperHeight = RtfPageSetup::s_unyPaperHeight(m_iPaperSize);

	Str8 sUnits;
    RtfPageSetup::s_RtfUnitsToString(m_unxPaperWidth, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtPaperWidth, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unyPaperHeight, RtfPageSetup::s_bCentimeters(m_iPaperSize), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtPaperHeight, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    m_pdlgMyOwner->ChangePaperSize(m_iPaperSize);
    SetPaper();
}


void CDocumentPage::ConvertMeasurements(BOOL bToCentimeters)
{
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unyTopMargin, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unyBottomMargin, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unxLeftMargin, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unxRightMargin, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unxGutter, bToCentimeters);
}

void CDocumentPage::SetPaper()
{
    Str8 s;
    s = m_pdlgMyOwner->bCentimeters() ? // 1.4eq
        _("Measurement unit: centimeters") :
        _("Measurement unit: inches");
    m_lblMeasurementUnit.SetWindowText( swUTF16( s) ); // 1.4qpy

    BOOL bStandardSize = RtfPageSetup::s_bStandardSize(m_iPaperSize);
    m_edtPaperWidth.SetReadOnly(bStandardSize);
    m_edtPaperHeight.SetReadOnly(bStandardSize);
}


void CDocumentPage::OnMirrorMargins()
{
    m_bMirrorMargins = ( m_chkMirrorMargins.GetCheck() == 1 );
    SetLeftRightMarginLabels();
}

void CDocumentPage::SetLeftRightMarginLabels()
{
    Str8 sLabel;
    sLabel = m_bMirrorMargins ? _("&Inside:") : _("&Left:"); // 1.4er
    m_lblLeftInside.SetWindowText( swUTF16( sLabel) ); // 1.4qpy
    sLabel = m_bMirrorMargins ? _("&Outside:") : _("&Right:"); // 1.4er
    m_lblRightOutside.SetWindowText( swUTF16( sLabel) ); // 1.4qpy
}


// ==========================================================================

class CSectionPage : public CPropertyPage
{
private:
    CRtfPageSetupSheet* m_pdlgMyOwner;
    BOOL m_bWasActive; // reflects whether this page was ever active

    RtfPageSetup* m_ppgs;
    RtfUnits m_unyHeaderToEdge;
    RtfUnits m_unyFooterToEdge;
    BOOL m_bFirstPage;  // 1998-10-14 MRP
    BOOL m_bOddAndEven;
    int m_numColumns;
    RtfUnits m_unxColumnSpacing;
    BOOL m_bLineBetweenColumns;

// Dialog Data
    //{{AFX_DATA(CSectionPage)
    enum { IDD = IDD_RTF_SectionPage };
    CRestrictedEdit m_edtHeaderToEdge;
    CRestrictedEdit m_edtFooterToEdge;
    CRestrictedEdit m_edtNumberOfColumns;
    CRestrictedEdit m_edtColumnSpacing;
    //}}AFX_DATA

public:
    CSectionPage(CRtfPageSetupSheet* pdlgMyOwner);

    void OK();
    void ConvertMeasurements(BOOL bToCentimeters);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    void DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un);
};  // class CSectionPage


// --------------------------------------------------------------------------

static const char s_pszValidNumberOfColumns[] = "1234";
static const char s_pszOnlyOneOrTwoColumns[] = "12";
static const int s_maxlenNumberOfColumns = 2;

CSectionPage::CSectionPage(CRtfPageSetupSheet* pdlgMyOwner) :
    CPropertyPage(CSectionPage::IDD),
    m_pdlgMyOwner(pdlgMyOwner),
    m_bWasActive(FALSE),
    m_ppgs(m_pdlgMyOwner->ppgs()) //,
//    m_edtHeaderToEdge(s_pszValidDecimal),
//    m_edtFooterToEdge(s_pszValidDecimal),
//    m_edtNumberOfColumns(m_ppgs->m_bOnlyOneOrTwoColumns ?
//        s_pszOnlyOneOrTwoColumns :
//        s_pszValidNumberOfColumns),
//    m_edtColumnSpacing(s_pszValidDecimal)
{
    ASSERT( m_ppgs );
    m_unyHeaderToEdge =     m_ppgs->m_unyHeaderToEdge;
    m_unyFooterToEdge =     m_ppgs->m_unyFooterToEdge;
    m_bFirstPage =          m_ppgs->m_bFirstPage;  // 1998-10-14 MRP
    m_bOddAndEven =         m_ppgs->m_bOddAndEven;
    m_numColumns =          m_ppgs->m_numColumns;
    m_unxColumnSpacing =    m_ppgs->m_unxColumnSpacing;
    m_bLineBetweenColumns = m_ppgs->m_bLineBetweenColumns;
}

void CSectionPage::DoDataExchange(CDataExchange* pDX)
{
    const RtfUnits minun = RtfPageSetup::s_minun();
    const RtfUnits maxun = RtfPageSetup::s_maxun(m_pdlgMyOwner->bCentimeters());

    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSectionPage)
    DDX_Control(pDX, IDC_RTF_edtHeaderToEdge, m_edtHeaderToEdge);
//    DDX_RtfUnits(pDX, IDC_RTF_edtHeaderToEdge, m_unyHeaderToEdge);
//    DDV_MinMaxDouble(pDX, m_unyHeaderToEdge, minun, maxun);

    DDX_Control(pDX, IDC_RTF_edtFooterToEdge, m_edtFooterToEdge);
//    DDX_RtfUnits(pDX, IDC_RTF_edtFooterToEdge, m_unyFooterToEdge);
//    DDV_MinMaxDouble(pDX, m_unyFooterToEdge, minun, maxun);

    DDX_Check(pDX, IDC_RTF_chkFirstPage, m_bFirstPage);
    DDX_Check(pDX, IDC_RTF_chkOddAndEven, m_bOddAndEven);

    DDX_Control(pDX, IDC_RTF_edtNumberOfColumns, m_edtNumberOfColumns);
    DDX_Text(pDX, IDC_RTF_edtNumberOfColumns, m_numColumns); // 1.4dw OK because not CStrg

    DDX_Control(pDX, IDC_RTF_edtColumnSpacing, m_edtColumnSpacing);
//    DDX_RtfUnits(pDX, IDC_RTF_edtColumnSpacing, m_unxColumnSpacing);
//    DDV_MinMaxDouble(pDX, m_unxColumnSpacing, minun, maxun);

    DDX_Check(pDX, IDC_RTF_chkLineBetweenColumns, m_bLineBetweenColumns);
    //}}AFX_DATA_MAP
}

void CSectionPage::DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un)
{
    m_pdlgMyOwner->DDX_RtfUnits(pDX, idc, un);
}

BOOL CSectionPage::OnInitDialog()
{
    m_bWasActive = TRUE;
    BOOL b = CPropertyPage::OnInitDialog();

//    m_edtHeaderToEdge.LimitText(s_maxlenDecimal);
//    m_edtFooterToEdge.LimitText(s_maxlenDecimal);
//    m_edtNumberOfColumns.LimitText(s_maxlenNumberOfColumns);
//    m_edtColumnSpacing.LimitText(s_maxlenDecimal);

	Str8 sUnits;
    RtfPageSetup::s_RtfUnitsToString(m_unyHeaderToEdge, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtHeaderToEdge, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unyFooterToEdge, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtFooterToEdge, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
    RtfPageSetup::s_RtfUnitsToString(m_unxColumnSpacing, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
    SetDlgItemText( IDC_RTF_edtColumnSpacing, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv

    return b;
}

void CSectionPage::OnOK()
{
    CPropertyPage::OnOK();  // Calls CSectionPage::DoDataExchange
    m_pdlgMyOwner->OK();  // Calls CDocumentPage::OK and CSectionPage::OK
}

void CSectionPage::OK()
{
    if (!m_bWasActive) // don't do anything if this page was never viewed
        return;

//	char buffer[DLGBUFMAX+1]; // 1.4qxp
	CString swBuffer; // 1.4qxp Upgrade GetDlgItemText for Unicode build
	GetDlgItemText( IDC_RTF_edtHeaderToEdge, swBuffer ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved // 1.4qxp
	m_unyHeaderToEdge = atof( sUTF8( swBuffer) ); // 1.4qxp
	GetDlgItemText( IDC_RTF_edtFooterToEdge, swBuffer ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved // 1.4qxp
	m_unyFooterToEdge = atof( sUTF8( swBuffer) ); // 1.4qxp
	GetDlgItemText( IDC_RTF_edtColumnSpacing, swBuffer ); // 1.4dz Fix bug (1.4dw) RTF page setup values not saved // 1.4qxp
	m_unxColumnSpacing = atof( sUTF8( swBuffer) ); // 1.4qxp

    // Note: UpdateData has already been called
    m_ppgs->m_unyHeaderToEdge =     m_unyHeaderToEdge;
    m_ppgs->m_unyFooterToEdge =     m_unyFooterToEdge;
    m_ppgs->m_bFirstPage =          m_bFirstPage;  // 1998-10-14 MRP
    m_ppgs->m_bOddAndEven =         m_bOddAndEven;
    m_ppgs->m_numColumns =          m_numColumns;
    m_ppgs->m_unxColumnSpacing =    m_unxColumnSpacing;
    m_ppgs->m_bLineBetweenColumns = m_bLineBetweenColumns;
}

void CSectionPage::ConvertMeasurements(BOOL bToCentimeters)
{
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unyHeaderToEdge, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unyFooterToEdge, bToCentimeters);
    CRtfPageSetupSheet::s_ConvertMeasurement(m_unxColumnSpacing, bToCentimeters);
    if ( m_bWasActive )
		{
		Str8 sUnits; // 1.4kua Fix bug of page setup dialog not updating correctly
		RtfPageSetup::s_RtfUnitsToString(m_unyHeaderToEdge, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
		SetDlgItemText( IDC_RTF_edtHeaderToEdge, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
		RtfPageSetup::s_RtfUnitsToString(m_unyFooterToEdge, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
		SetDlgItemText( IDC_RTF_edtFooterToEdge, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
		RtfPageSetup::s_RtfUnitsToString(m_unxColumnSpacing, m_pdlgMyOwner->bCentimeters(), sUnits); // 1.4dy Fix bug (1.4dw) RTF page setup values not correct
		SetDlgItemText( IDC_RTF_edtColumnSpacing, swUTF16( sUnits ) ); // 1.4dy // 1.4qpv
		}
}


// ==========================================================================

CRtfPageSetupSheet::CRtfPageSetupSheet(RtfPageSetup* ppgs) :
    CPropertySheet( swUTF16( "Export Page Setup" ), NULL, 0), // 1.4qxr Upgrade CPropertySheet for Unicode build
    m_ppgs(ppgs)
{
    ASSERT( m_ppgs );
    m_bCentimeters = m_ppgs->bCentimeters();

    m_pdlgDocument = new CDocumentPage(this);
    AddPage(m_pdlgDocument);
    m_pdlgDocument->SetHelpID(IDD_RTF_DocumentPage);

    m_pdlgSection = new CSectionPage(this);
    AddPage(m_pdlgSection);
    m_pdlgSection->SetHelpID(IDD_RTF_SectionPage);
}

CRtfPageSetupSheet::~CRtfPageSetupSheet()
{
    delete m_pdlgDocument;
    delete m_pdlgSection;
}


BOOL CRtfPageSetupSheet::OnInitDialog()
{
    CPropertySheet::OnInitDialog();

    CButton *pBtn = (CButton*)GetDlgItem(ID_APPLY_NOW);
    ASSERT(pBtn);
    pBtn->ShowWindow(SW_HIDE); // make Apply button do the disappearing act

	SetButtonLabels();  // 1999-02-01 BW: For French version

    return TRUE;
}


BEGIN_MESSAGE_MAP(CRtfPageSetupSheet, CPropertySheet)
    //{{AFX_MSG_MAP(CRtfPageSetupSheet)
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CRtfPageSetupSheet::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
        return -1;
    return 0;
}

void CRtfPageSetupSheet::SetButtonLabels()
{
	// 1999-02-01 BW: For French version
	CButton *pbtn = NULL;
	Str8 sLabel;
    
    pbtn = (CButton*)GetDlgItem(IDOK);
    ASSERT( pbtn );
    sLabel = _("OK");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
    
    pbtn = (CButton*)GetDlgItem(IDCANCEL);
    ASSERT( pbtn );
    sLabel = _("Cancel");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy

    pbtn = (CButton*)GetDlgItem(IDHELP);
    ASSERT( pbtn );
    sLabel = _("Help");
    pbtn->SetWindowText( swUTF16( sLabel) ); // 1.4qpy
}


void CRtfPageSetupSheet::OK()
{
    // Called by either CDocumentPage::OnOK or CSectionPage::OnOK
    m_pdlgDocument->OK();
    m_pdlgSection->OK();
}


void CRtfPageSetupSheet::ChangePaperSize(int iPaperSize)
{
    ASSERT( 0 <= iPaperSize );
    BOOL bCentimeters = RtfPageSetup::s_bCentimeters(iPaperSize);
    if ( m_bCentimeters != bCentimeters )
        {
        m_bCentimeters = bCentimeters;
        m_pdlgDocument->ConvertMeasurements(m_bCentimeters);
        m_pdlgSection->ConvertMeasurements(m_bCentimeters);
        }
}

void CRtfPageSetupSheet::DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un)
{
    Str8 sUnits;
    if ( pDX->m_bSaveAndValidate )
        {
//		DDX_Text(pDX, idc, sUnits);
//		char buffer[DLGBUFMAX+1]; // 1.4qxp
		CString swBuffer; // 1.4qxp
		GetDlgItemText( idc, swBuffer ); // 1.4qxp
		sUnits =  sUTF8( swBuffer ); // 1.4qxp
        
		un = atof(sUnits);
        }
    else
        {
        RtfPageSetup::s_RtfUnitsToString(un, m_bCentimeters, sUnits);
//		DDX_Txt(pDX, idc, sUnits);
        SetDlgItemText( idc, swUTF16( sUnits ) ); // 1.4qpv
        }
}

void CRtfPageSetupSheet::s_ConvertMeasurement(RtfUnits& un, BOOL bToCentimeters)
{
    un = ( bToCentimeters ? un * 2.54 : un / 2.54 );
}
