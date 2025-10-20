// export_d.cpp  File Export dialog boxes

#include "stdafx.h"
#include "toolbox.h"
#include "export.h"
#include "export_d.h"
#include "mrfset_d.h"  // CSelectFieldsDlg
#include "pgs.h"  // RtfPageSetup
#include "pgs_d.h"  // CRtfPageSetupSheet
#include "ind.h"  // CIndex, CRecLookEl
#include "set_d.h"  // CSetDlg
#include "typ.h"  // CDatabaseType

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


// --------------------------------------------------------------------------

CExportProcessTypeDlg::CExportProcessTypeDlg(BOOL bEnableMDF,
		BOOL* pbMDF, BOOL* pbXML, BOOL* pbRTF) :
    CDialog(CExportProcessTypeDlg::IDD),
    m_bEnableMDF(bEnableMDF),
    m_pbMDF(pbMDF),
    m_pbXML(pbXML),  // 1999-05-21 MRP
    m_pbRTF(pbRTF)
{
    ASSERT( m_pbMDF );
    ASSERT( m_pbXML );  // 1999-05-21 MRP
    ASSERT( m_pbRTF );
    if( *m_pbMDF )
	    m_iFormat = eMDF;
    else if ( *m_pbXML )  // 1999-05-21 MRP
	    m_iFormat = eXML;
    else if ( *m_pbRTF )
        m_iFormat = eRTF;
    else
        m_iFormat = eSF;
}

void CExportProcessTypeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportProcessTypeDlg)
    DDX_Radio(pDX, IDC_Export_optSF, m_iFormat);
    DDX_Control(pDX, IDC_Export_optMDF, m_optMDF);
    //}}AFX_DATA_MAP
}

BOOL CExportProcessTypeDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();
    m_optMDF.EnableWindow( m_bEnableMDF );
    return bRet;
}

BEGIN_MESSAGE_MAP(CExportProcessTypeDlg, CDialog)
    //{{AFX_MSG_MAP(CExportProcessTypeDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportProcessTypeDlg::OnOK()
{
    CDialog::OnOK();
    if ( m_iFormat == eSF )
        {
        *m_pbMDF = FALSE;
        *m_pbXML = FALSE;  // 1999-05-21 MRP
        *m_pbRTF = FALSE;
        }
    else if ( m_iFormat == eRTF )
        {
        *m_pbMDF = FALSE;
        *m_pbXML = FALSE;  // 1999-05-21 MRP
        *m_pbRTF = TRUE;
        }
    else if ( m_iFormat == eXML )  // 1999-05-21 MRP
        {
        *m_pbMDF = FALSE;
        *m_pbXML = TRUE;
        *m_pbRTF = FALSE;
        }
    else if ( m_iFormat == eMDF )
        {
        *m_pbMDF = TRUE;
        *m_pbXML = FALSE;  // 1999-05-21 MRP
        *m_pbRTF = FALSE;
        }
    else
        ASSERT( FALSE );
}


// --------------------------------------------------------------------------

CExportProcess* CExportProcessListBox::pexpSelected()
{
    return (CExportProcess*)pselSelected();
}

void CExportProcessListBox::InitLabels()
{
    m_xExportProcesses = xSubItem_Show(IDC_SET_NAME);
    m_xType = xSubItem_Show(IDC_Export_lblType);
}

void CExportProcessListBox::DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel)
{
    CExportProcess* pexp = (CExportProcess*)pel;
    DrawSubItem(cDC, rcItem, m_xExportProcesses, m_xType, pexp->sName());
    DrawSubItem(cDC, rcItem, m_xType, 0, pexp->pszType());
}

// --------------------------------------------------------------------------

CExportProcessDlg::CExportProcessDlg(CExportProcessSet* pexpset,
        CIndex* pindCur, const CRecLookEl* prelCur,
        const Str8& sPathDatabase) :
    CDialog(CExportProcessDlg::IDD),
    m_pexpset(pexpset),
    m_pexpDefault(m_pexpset->m_pexpDefault),
    m_lboExportProcesses(m_pexpset, &m_pexpDefault)
{
    ASSERT( m_pexpset );
    m_bCurrentRecordEnabled = ( prelCur != NULL );
    if ( m_pexpset->m_bCurrentRecord && m_bCurrentRecordEnabled )
        m_iRecordsToInclude = eCurrentRecord;
    else if ( m_pexpset->m_bCurrentWindow && m_bCurrentRecordEnabled )
        m_iRecordsToInclude = eCurrentWindow;
    else
        m_iRecordsToInclude = eEntireDatabase;

    Str8 sFileNameExt = sGetFileName(sPathDatabase);
	m_sEntireDatabase = _("Entire &file"); // 1.5.0fg // 1.5.1c
	m_sEntireDatabase = m_sEntireDatabase + ": " + sFileNameExt; // 1.5.0fg 
	m_sLexicalDatabase = m_sEntireDatabase; // 1.5.0fg 

    ASSERT( pindCur );
    const CFilter* pfil = pindCur->pfil();
    Filtering(pfil, m_sFiltering);
    const CMarker* pmkrPriKey = pindCur->pmkrPriKey();
	m_sSorting = _("Sorted by"); // 1.5.0fg // 1.5.1c
	m_sSorting = m_sSorting + ": " + pmkrPriKey->sMarker(); // 1.5.0fg 
    m_bShowProperties = m_pexpset->m_bShowProperties;  // 1998-03-10 MRP
    m_bAutoOpen = m_pexpset->m_bAutoOpen;

    // 1999-01-25 MRP: Show the database type name to help the user
    // better understand how the properties relate to the file.
    // This may also help avoid confusion if the user chooses File Export
    // when the active window (and database) isn't what they thought it was.
    const Str8& sName = m_pexpset->m_ptypMyOwner->sName();
	m_sDatabaseType = _("Database Type:"); // 1.5.0fg 
	m_sDatabaseType = m_sDatabaseType + " " + sName; // 1.5.0fg 
}

CExportProcessDlg::~CExportProcessDlg()
{
}

void CExportProcessDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportProcessDlg)
    DDX_Control(pDX, IDC_Export_lboExportProcesses, m_lboExportProcesses);
    DDX_Radio(pDX, IDC_Export_optEntireDatabase, m_iRecordsToInclude);
    DDX_Control(pDX, IDC_Export_optEntireDatabase, m_optEntireDatabase);
    DDX_Control(pDX, IDC_Export_lblEntireDatabase, m_lblEntireDatabase);
    DDX_Control(pDX, IDC_Export_optCurrentWindow, m_optCurrentWindow);
    DDX_Control(pDX, IDC_Export_optCurrentRecord, m_optCurrentRecord);
    DDX_Control(pDX, IDC_Export_lblFiltering, m_lblFiltering);
    DDX_Control(pDX, IDC_Export_lblSorting, m_lblSorting);
    DDX_Control(pDX, IDC_Export_optCurrentRecord, m_optCurrentRecord);
    DDX_Check(pDX, IDC_Export_chkShowProperties, m_bShowProperties);
    DDX_Control(pDX, IDC_Export_chkAutoOpen, m_chkAutoOpen);
    DDX_Check(pDX, IDC_Export_chkAutoOpen, m_bAutoOpen);
//    DDX_Txt(pDX, IDC_Export_lblDatabaseType, m_sDatabaseType);
    DDX_Control(pDX, IDC_ADD, m_btnAdd);
    DDX_Control(pDX, IDC_COPY, m_btnCopy);
    DDX_Control(pDX, IDC_MODIFY, m_btnModify);
    DDX_Control(pDX, IDC_DELETE, m_btnDelete);
    //}}AFX_DATA_MAP
}

BOOL CExportProcessDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();
    SetDlgItemText( IDC_Export_lblDatabaseType, swUTF16( m_sDatabaseType ) ); // 1.4qpv

    m_lboExportProcesses.InitListBox();

    m_optEntireDatabase.SetWindowText(swUTF16( m_sEntireDatabase ) );     // 1.4qpv
    m_lblEntireDatabase.SetWindowText(swUTF16( m_sLexicalDatabase ) ); // 1.4qpv
    m_lblSorting.SetWindowText(  swUTF16( m_sSorting ) ); // 1.4qpy
    m_optCurrentRecord.EnableWindow(m_bCurrentRecordEnabled);
    // Enable or disable controls for initially selected element
    UpdateCopyModifyButtons();
    OnSelchangeExportProcess();

#ifdef _MAC
    m_chkAutoOpen.EnableWindow(FALSE);
#endif
    
    return bRet;
}


BEGIN_MESSAGE_MAP(CExportProcessDlg, CDialog)
    //{{AFX_MSG_MAP(CExportProcessDlg)
    ON_LBN_DBLCLK(IDC_Export_lboExportProcesses, OnDblclkExportProcess)
    ON_LBN_SELCHANGE(IDC_Export_lboExportProcesses, OnSelchangeExportProcess)
    ON_BN_CLICKED(IDC_ADD, OnAdd)
    ON_BN_CLICKED(IDC_COPY, OnCopy)
    ON_BN_CLICKED(IDC_MODIFY, OnModify)
    ON_BN_CLICKED(IDC_DELETE, OnDelete)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportProcessDlg::OnCancel()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data

    m_lboExportProcesses.Close();  // 1.1ca Change cancel button to Close, save all settings
    m_pexpset->m_pexpDefault = m_pexpDefault;

    if ( m_iRecordsToInclude == eCurrentRecord )
        {
        m_pexpset->m_bCurrentRecord = TRUE;
        m_pexpset->m_bCurrentWindow = FALSE;
        }
    else  // Entire database or Current window
        {
        m_pexpset->m_bCurrentRecord = FALSE;
        m_pexpset->m_bCurrentWindow = ( m_iRecordsToInclude == eCurrentWindow );
        }

    m_pexpset->m_bShowProperties = m_bShowProperties;  // 1998-03-10 MRP
    m_pexpset->m_bAutoOpen = m_bAutoOpen;
	CDialog::OnCancel();
}

void CExportProcessDlg::OnOK()
{
    CDialog::OnOK();
//    GetDlgItemText( IDC_Export_lblDatabaseType, (char*)buffer, DLGBUFMAX );
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_Export_lblDatabaseType, sDlgItem ); // 1.4qpw
	m_sDatabaseType = sUTF8( sDlgItem ); // 1.4qpw

    m_lboExportProcesses.Close();  // 1998-03-10 MRP: Update the selected element
    m_pexpset->m_pexpDefault = m_pexpDefault;

    if ( m_iRecordsToInclude == eCurrentRecord )
        {
        m_pexpset->m_bCurrentRecord = TRUE;
        m_pexpset->m_bCurrentWindow = FALSE;
        }
    else  // Entire database or Current window
        {
        m_pexpset->m_bCurrentRecord = FALSE;
        m_pexpset->m_bCurrentWindow = ( m_iRecordsToInclude == eCurrentWindow );
        }

    m_pexpset->m_bShowProperties = m_bShowProperties;  // 1998-03-10 MRP
    m_pexpset->m_bAutoOpen = m_bAutoOpen;
}

void CExportProcessDlg::OnDblclkExportProcess()
{
    OnOK();  // shortcut for the OK button
}

void CExportProcessDlg::OnSelchangeExportProcess()
{
    CFilter* pfil = NULL;
    BOOL bMDF = FALSE;
    BOOL bCanAutoOpen = FALSE;

    CExportProcess* pexp = m_lboExportProcesses.pexpSelected();
    if ( pexp )
        {
        bMDF = pexp->bMDF(&pfil);
        bCanAutoOpen = pexp->bCanAutoOpen();
        }

    if ( bMDF )
        {
        Str8 sFiltering;
        Filtering(pfil, sFiltering);
        m_lblFiltering.SetWindowText( swUTF16( sFiltering) ); // 1.4qpy
        }
    else
        m_lblFiltering.SetWindowText( swUTF16( m_sFiltering) ); // 1.4qpy

    int swNonMDF = (bMDF ? SW_HIDE : SW_SHOW);
    m_optEntireDatabase.ShowWindow(swNonMDF);
    m_optCurrentWindow.ShowWindow(swNonMDF);
    m_lblSorting.ShowWindow(swNonMDF);
    m_optCurrentRecord.ShowWindow(swNonMDF);

    int swMDF = (bMDF ? SW_SHOW : SW_HIDE);
    m_lblEntireDatabase.ShowWindow(swMDF);

#ifndef _MAC
    m_chkAutoOpen.EnableWindow(bCanAutoOpen);
#endif
    UpdateDeleteButton();
}

void CExportProcessDlg::OnAdd()
{
    if ( !m_lboExportProcesses.bAdd() )
        return;

    UpdateCopyModifyButtons();
    OnSelchangeExportProcess();
}

void CExportProcessDlg::OnCopy()
{
    if ( !m_lboExportProcesses.bCopy() )
        return;

    OnSelchangeExportProcess();
}

void CExportProcessDlg::OnModify()
{
    if ( !m_lboExportProcesses.bModify() )
        return;

    OnSelchangeExportProcess();
}

void CExportProcessDlg::OnDelete()
{
    if ( !m_lboExportProcesses.bDelete() )
        return;

    UpdateCopyModifyButtons();
    OnSelchangeExportProcess();
}

void CExportProcessDlg::UpdateCopyModifyButtons()
{
    BOOL bListHasItems = ( m_lboExportProcesses.GetCount() > 0 );
    m_btnCopy.EnableWindow(bListHasItems);
    m_btnModify.EnableWindow(bListHasItems);
}

void CExportProcessDlg::UpdateDeleteButton()
{
    BOOL bDeletable = FALSE;
    CSetEl* psel = m_lboExportProcesses.pselSelected();
    if ( psel )
        bDeletable = psel->bDeletable();
    m_btnDelete.EnableWindow(bDeletable);
}

void CExportProcessDlg::Filtering(const CFilter* pfil, Str8& sFiltering)
{
    if ( pfil )
		{
		sFiltering = "Filter:"; // 1.5.0fg 
		sFiltering = sFiltering + " " + pfil->sName(); // 1.5.0fg 
		}
    else
        sFiltering = _("All records [no filter]");
}

extern void AddSettingsPathIfNone( Str8& sPath ); // 1.4bf Add settings dir path if no dir path 

// --------------------------------------------------------------------------

// Add button
CExportProcessRTFDlg::CExportProcessRTFDlg(CExportProcessSet* pexpset,
        CExportProcessRTF** ppexpNew) :
    CDialog(CExportProcessRTFDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpset),
    m_bEnableName(TRUE),
    m_bMarkerFont(FALSE),
    m_subsetMarkersToExport(m_pexpset->pmkrset()),
    m_ppgs(new RtfPageSetup()),
    // 1999-02-01 MRP: Default is tab-aligned in version 4.1 (and later)
    m_bInterlinearTabAligned(TRUE),
    m_twxInterlinearSpacing(CExportProcessRTF::s_twxDefaultInterlinearSpacing),
    m_bCheckConsistency(FALSE),
    m_bOverwriteOutputFile(FALSE),
    m_bAddAlphDiv(FALSE)
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Copy button
CExportProcessRTFDlg::CExportProcessRTFDlg(const CExportProcessRTF* pexpToCopyFrom,
        CExportProcessRTF** ppexpNew) :
    CDialog(CExportProcessRTFDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpToCopyFrom->m_pexpsetMyOwner),
    m_bEnableName(TRUE),
    m_sPathCCT(pexpToCopyFrom->m_sPathCCT),
    m_sPathDOT(pexpToCopyFrom->m_sPathDOT),
    m_bMarkerFont(pexpToCopyFrom->m_bMarkerFont),
    m_subsetMarkersToExport(pexpToCopyFrom->m_subsetMarkersToExport),
    m_ppgs(new RtfPageSetup(*pexpToCopyFrom->m_ppgs)),
    m_bInterlinearTabAligned(pexpToCopyFrom->m_bInterlinearTabAligned),
    m_twxInterlinearSpacing(pexpToCopyFrom->m_twxInterlinearSpacing),
    m_bCheckConsistency(pexpToCopyFrom->m_bCheckConsistency),
    m_bOverwriteOutputFile(pexpToCopyFrom->m_bOverwriteOutputFile), // 1.4ytg Try to fix possible export file overwrite bug
    m_bAddAlphDiv(pexpToCopyFrom->m_bAddAlphDiv)
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Modify button
CExportProcessRTFDlg::CExportProcessRTFDlg(CExportProcessRTF* pexp,
        BOOL bEnableName) :
    CDialog(CExportProcessRTFDlg::IDD),
    m_pexp(pexp),
    m_ppexpNew(NULL),
    m_bEnableName(bEnableName),
    m_pexpset(m_pexp->m_pexpsetMyOwner),
    m_sName(m_pexp->sName()),
    m_sPathCCT(m_pexp->m_sPathCCT),
    m_sPathDOT(m_pexp->m_sPathDOT),
    m_bMarkerFont(m_pexp->m_bMarkerFont),
    m_subsetMarkersToExport(m_pexp->m_subsetMarkersToExport),
    m_ppgs(new RtfPageSetup(*m_pexp->m_ppgs)),
    m_bInterlinearTabAligned(m_pexp->m_bInterlinearTabAligned),
    m_twxInterlinearSpacing(m_pexp->m_twxInterlinearSpacing),
    m_sPathRTF(m_pexp->m_sPathRTF), // 1.1bb Include output file in export process
    m_bCheckConsistency(m_pexp->m_bCheckConsistency),
    m_bOverwriteOutputFile(m_pexp->m_bOverwriteOutputFile),
    m_bAddAlphDiv(m_pexp->m_bAddAlphDiv)
{
    ASSERT( m_pexp );
}

CExportProcessRTFDlg::~CExportProcessRTFDlg()
{
    delete m_ppgs;
}

void CExportProcessRTFDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportProcessRTFDlg)
    DDX_Control(pDX, IDC_Export_edtName, m_edtName);
    DDX_Control(pDX, IDC_Export_chkAllFields, m_chkAllFields);
    DDX_Control(pDX, IDC_Export_btnSelectFields, m_btnSelectFields);
//    DDX_Txt(pDX, IDC_Export_edtCCT, m_sPathCCT);
//    DDX_Txt(pDX, IDC_Export_edtDOT, m_sPathDOT);
//    DDX_Txt(pDX, IDC_Export_edtOutputFile, m_sPathRTF);
    DDX_Radio(pDX, IDC_Export_optDocumentTemplate, m_iStyleFormatting);
    DDX_Control(pDX, IDC_RTF_chkInterlinearTabAligned, m_chkInterlinearTabAligned);
    DDX_Check(pDX, IDC_RTF_chkInterlinearTabAligned, m_bInterlinearTabAligned);
    DDX_Control(pDX, IDC_RTF_lblInterlinearSpacing, m_lblInterlinearSpacing);
    DDX_Control(pDX, IDC_RTF_edtInterlinearSpacing, m_edtInterlinearSpacing);
    DDX_Text(pDX, IDC_RTF_edtInterlinearSpacing, m_twxInterlinearSpacing); // 1.4dw OK because not CStrg
    DDX_Check(pDX, IDC_Export_chkCheckConsistency, m_bCheckConsistency);
    DDX_Check(pDX, IDC_Export_chkOverwrite, m_bOverwriteOutputFile);
    DDX_Check(pDX, IDC_Export_chkAddAlphDiv, m_bAddAlphDiv);
    //}}AFX_DATA_MAP
//    m_pexpset->DDX_Name(pDX, IDC_Export_edtName, m_sName); // 1.4hdd Replace DDX_Name in export proc dlgs
}

BOOL CExportProcessRTFDlg::OnInitDialog()
{
    m_iStyleFormatting = (m_bMarkerFont ? 1 : 0);

    BOOL b = CDialog::OnInitDialog();
    SetDlgItemText( IDC_Export_edtName, swUTF16( m_sName ) ); // 1.4hdd // 1.4qpv
    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtDOT, swUTF16( m_sPathDOT ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtOutputFile, swUTF16( m_sPathRTF ) ); // 1.4qpv
    
    BOOL bAllFields = m_subsetMarkersToExport.bIncludeAllElements();    
    m_chkAllFields.SetCheck(bAllFields ? 1 : 0);
    m_btnSelectFields.EnableWindow(!bAllFields);

    // 1999-01-30 MRP: Enable the TabAligned check box if
    // the database type has interlinear properties.
    // Show the Spacing label and edit box if the check box
    // is both enabled and checked (note that the default
    // is checked, even when it's not enabled).
    BOOL bInterlinear = m_pexpset->bInterlinear();
    m_chkInterlinearTabAligned.EnableWindow(bInterlinear);
    BOOL bSpacing = (bInterlinear && m_bInterlinearTabAligned);
    m_lblInterlinearSpacing.ShowWindow(bSpacing ? SW_SHOW : SW_HIDE);
    m_edtInterlinearSpacing.ShowWindow(bSpacing ? SW_SHOW : SW_HIDE);
    
    m_edtName.EnableWindow(m_bEnableName);  // 1998-03-10 MRP
    if ( m_pexp )
        {
        // When modifying an existing export process,
        // don't let the Name edit box have focus. 
        m_chkAllFields.SetFocus();
        return FALSE;
        }
    
    return b;
}

BEGIN_MESSAGE_MAP(CExportProcessRTFDlg, CDialog)
    //{{AFX_MSG_MAP(CExportProcessRTFDlg)
    ON_BN_CLICKED(IDC_Export_chkAllFields, OnAllFieldsChanged)
    ON_BN_CLICKED(IDC_Export_btnSelectFields, OnSelectFields)
    ON_BN_CLICKED(IDC_Export_btnBrowseCCT, OnBrowseCCT)
    ON_BN_CLICKED(IDC_Export_btnBrowseDOT, OnBrowseDOT)
    ON_BN_CLICKED(IDC_Export_btnPageSetup, OnPageSetup)
    ON_BN_CLICKED(IDC_RTF_chkInterlinearTabAligned, OnInterlinearTabAligned)
    ON_BN_CLICKED(IDC_Export_btnBrowseOutputFile, OnBrowseOutputFile)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportProcessRTFDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_Export_edtName, (char*)buffer, DLGBUFMAX ); // 1.4hdd
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_Export_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdd
//    GetDlgItemText( IDC_Export_edtCCT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtCCT, sDlgItem ); // 1.4qpw
	m_sPathCCT = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_Export_edtDOT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtDOT, sDlgItem ); // 1.4qpw
	m_sPathDOT = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_Export_edtOutputFile, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtOutputFile, sDlgItem ); // 1.4qpw
	m_sPathRTF = sUTF8( sDlgItem ); // 1.4qpw

    m_bMarkerFont = (m_iStyleFormatting == 1);

    Str8 sMessage;
    if ( !m_pexpset->bValidNameDlg(m_sName, m_pexp, sMessage) )
        {
        MessageBeep(0);
        AfxMessageBox(sMessage);
        m_edtName.SetFocus();
        m_edtName.SetSel(0, -1);  // Select the entire name
        return;
        }

    if ( m_pexp )  // Modify
        {
        // TO DO: Make sure the process name isn't empty or duplicate
        if ( strcmp(m_sName, m_pexp->sName()) != 0 )
            m_pexp->ChangeNameTo(m_sName);
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathDOT );
		AddSettingsPathIfNone( m_sPathRTF );
        m_pexp->m_sPathCCT = m_sPathCCT;
        m_pexp->m_sPathDOT = m_sPathDOT;
        m_pexp->m_sPathRTF = m_sPathRTF; // 1.1bb Include output file in export process
        m_pexp->m_bMarkerFont = m_bMarkerFont;
        m_pexp->m_subsetMarkersToExport = m_subsetMarkersToExport;
        *m_pexp->m_ppgs = *m_ppgs;
        m_pexp->m_bInterlinearTabAligned = m_bInterlinearTabAligned;
        m_pexp->m_twxInterlinearSpacing = m_twxInterlinearSpacing;
        m_pexp->SetValidInterlinearSpacing();  // 1999-02-01 MRP
        m_pexp->m_bCheckConsistency = m_bCheckConsistency;  // 1999-06-14 MRP
        m_pexp->m_bOverwriteOutputFile = m_bOverwriteOutputFile;
        m_pexp->m_bAddAlphDiv = m_bAddAlphDiv;
        }
    else  // Add or Copy
        {
        // TO DO: Construct a new export process and add it to m_pexpset
        *m_ppexpNew = new CExportProcessRTF(m_sName, m_pexpset,
            m_sPathCCT, m_sPathDOT, m_bMarkerFont,
            m_subsetMarkersToExport, m_bCheckConsistency, m_ppgs,
            m_bInterlinearTabAligned, m_twxInterlinearSpacing);
        m_pexpset->Add(*m_ppexpNew);
	    CExportProcessRTF* pexp = *m_ppexpNew;
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathDOT );
		AddSettingsPathIfNone( m_sPathRTF );
		pexp->m_sPathCCT = m_sPathCCT; // 1.4bg Don't lose cc file name on add export process
		pexp->m_sPathDOT = m_sPathDOT; // 1.4bg Don't lose cc file name on add export process
		pexp->m_sPathRTF = m_sPathRTF; // 1.4bg Don't lose cc file name on add export process
        }

    CDialog::OnOK();
}

void CExportProcessRTFDlg::OnAllFieldsChanged()
{
    BOOL bAllFields = (m_chkAllFields.GetCheck() != 0);
    m_subsetMarkersToExport.IncludeAllElements(bAllFields);
    m_btnSelectFields.EnableWindow(!bAllFields);
}

void CExportProcessRTFDlg::OnSelectFields()
{
    CSelectFieldsDlg dlg(m_subsetMarkersToExport, TRUE ); // , IDSx_SelectExportFields); // 1.4gb
    dlg.DoModal();
}


void CExportProcessRTFDlg::OnBrowseCCT()
{
    UpdateData(TRUE);  // from dialog controls to object members
	Str8 sFileFilter = _("Change Table"); // 1.5.0fz 
	sFileFilter += " (*.cct)|*.cct||"; // 1.5.0fz 
    Str8 sDirPath;
//    Str8 sDirPath = m_pexpsetMyOwner->
//        ptypMyOwner()->ptypsetMyOwner()->sSettingsDirPath();

    if ( CExportProcess::s_bBrowseForFile(_("Browse for a Consistent Changes Table"), NULL, sFileFilter, sDirPath, m_sPathCCT, IDD) )
	    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4gj Fix bug (1.4dw) of browse button not updating file path // 1.4qpv
}

void CExportProcessRTFDlg::OnBrowseDOT()
{
    UpdateData(TRUE);  // from dialog controls to object members
    const char pszDefaultExt[] = "dot";
    Str8 sFileFilter = _("Document Templates"); // 1.5.0fz 
	sFileFilter = sFileFilter + " (*.dot)|*.dot||";
    Str8 sDirPath;
//    Str8 sDirPath = sGetDirPath(m_sPathDOT);

    if ( CExportProcess::s_bBrowseForFile(_("Browse for a Document Template"), pszDefaultExt, sFileFilter, sDirPath, m_sPathDOT, IDD) )
	    SetDlgItemText( IDC_Export_edtDOT, swUTF16( m_sPathDOT ) ); // 1.4gj // 1.4qpv
}

void CExportProcessRTFDlg::OnPageSetup()
{
    CRtfPageSetupSheet dlg(m_ppgs);
    dlg.DoModal();
}

void CExportProcessRTFDlg::OnInterlinearTabAligned()
{
    // Show or hide the Spacing label and edit box whenever
    // whenever the setting of the TabAligned check box changes.
    BOOL bSpacing = (m_chkInterlinearTabAligned.GetCheck() != 0);
    int iShow = (bSpacing ? SW_SHOW : SW_HIDE);
    m_lblInterlinearSpacing.ShowWindow(iShow);
    m_edtInterlinearSpacing.ShowWindow(iShow);
}

void CExportProcessRTFDlg::OnBrowseOutputFile()
{
    UpdateData(TRUE);  // from dialog controls to object members
    int iFilterIndex = 1;
	Str8 sFileFilter = _("All files"); // 1.5.0fz 
	sFileFilter += " (*.*)|*.*||"; // 1.5.0fz 
    if ( CExportProcess::s_bExportedFileDlg(NULL, FALSE, sFileFilter, &iFilterIndex, m_sPathRTF) )
	    SetDlgItemText( IDC_Export_edtOutputFile, swUTF16( m_sPathRTF ) ); // 1.4gj // 1.4qpv
}


// --------------------------------------------------------------------------

// Add button
CExportProcessSFDlg::CExportProcessSFDlg(CExportProcessSet* pexpset,
        CExportProcessSF** ppexpNew) :
    CDialog(CExportProcessSFDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpset),
    m_bEnableName(TRUE),
    m_bCheckConsistency(FALSE),
    m_bOverwriteOutputFile(FALSE),
    m_bAddAlphDiv(FALSE),
    m_bOpenExportedFile(FALSE),
    m_subsetMarkersToExport(m_pexpset->pmkrset())
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Copy button
CExportProcessSFDlg::CExportProcessSFDlg(const CExportProcessSF* pexpToCopyFrom,
        CExportProcessSF** ppexpNew) :
    CDialog(CExportProcessSFDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpToCopyFrom->m_pexpsetMyOwner),
    m_bEnableName(TRUE),
    m_sPathCCT(pexpToCopyFrom->m_sPathCCT),
    m_bCheckConsistency(pexpToCopyFrom->m_bCheckConsistency),
    m_bOverwriteOutputFile(pexpToCopyFrom->m_bOverwriteOutputFile), // 1.4ytg 
    m_bAddAlphDiv(pexpToCopyFrom->m_bAddAlphDiv),
    m_bOpenExportedFile(pexpToCopyFrom->m_bOpenExportedFile),
    m_subsetMarkersToExport(pexpToCopyFrom->m_subsetMarkersToExport)
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Modify button
CExportProcessSFDlg::CExportProcessSFDlg(CExportProcessSF* pexp,
        BOOL bEnableName) :
    CDialog(CExportProcessSFDlg::IDD),
    m_pexp(pexp),
    m_ppexpNew(NULL),
    m_pexpset(m_pexp->m_pexpsetMyOwner),
    m_bEnableName(bEnableName),
    m_sName(m_pexp->sName()),
    m_sPathCCT(m_pexp->m_sPathCCT),
    m_sPathSF(m_pexp->m_sPathSF), // 1.1ba Include output file in export process
    m_sChainProcess(m_pexp->m_sChainProcess), // 1.1br Process to chain to
    m_bCheckConsistency(m_pexp->m_bCheckConsistency),
    m_subsetMarkersToExport(m_pexp->m_subsetMarkersToExport),
    m_bOverwriteOutputFile(m_pexp->m_bOverwriteOutputFile),
    m_bAddAlphDiv(m_pexp->m_bAddAlphDiv),
    m_bOpenExportedFile(m_pexp->m_bOpenExportedFile)
{
    ASSERT( m_pexp );
}

CExportProcessSFDlg::~CExportProcessSFDlg()
{
}

void CExportProcessSFDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportProcessSFDlg)
    DDX_Control(pDX, IDC_Export_edtName, m_edtName);
    DDX_Control(pDX, IDC_Export_chkAllFields, m_chkAllFields);
    DDX_Control(pDX, IDC_Export_btnSelectFields, m_btnSelectFields);
//    DDX_Txt(pDX, IDC_Export_edtCCT, m_sPathCCT);
//    DDX_Txt(pDX, IDC_Export_edtOutputFile, m_sPathSF);
//    DDX_Txt(pDX, IDC_Export_edtChainProcess, m_sChainProcess);
    DDX_Check(pDX, IDC_Export_chkCheckConsistency, m_bCheckConsistency);
    DDX_Check(pDX, IDC_Export_chkOverwrite, m_bOverwriteOutputFile);
    DDX_Check(pDX, IDC_Export_chkAddAlphDiv, m_bAddAlphDiv);
    DDX_Check(pDX, IDC_Export_chkAutoOpen, m_bOpenExportedFile);
    //}}AFX_DATA_MAP
//    m_pexpset->DDX_Name(pDX, IDC_Export_edtName, m_sName); // 1.4hdd
}

BOOL CExportProcessSFDlg::OnInitDialog()
{
    BOOL b = CDialog::OnInitDialog();
    SetDlgItemText( IDC_Export_edtName, swUTF16( m_sName ) ); // 1.4hdd // 1.4qpv
    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtOutputFile, swUTF16( m_sPathSF ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtChainProcess, swUTF16( m_sChainProcess ) ); // 1.4qpv

    BOOL bAllFields = m_subsetMarkersToExport.bIncludeAllElements();    
    m_chkAllFields.SetCheck(bAllFields ? 1 : 0);
    m_btnSelectFields.EnableWindow(!bAllFields);
    
    m_edtName.EnableWindow(m_bEnableName);  // 1998-03-10 MRP
    if ( m_pexp )
        {
        // When modifying an existing export process,
        // don't let the Name edit box have focus. 
        m_chkAllFields.SetFocus();
        return FALSE;
        }
    
    return b;
}


BEGIN_MESSAGE_MAP(CExportProcessSFDlg, CDialog)
    //{{AFX_MSG_MAP(CExportProcessSFDlg)
    ON_BN_CLICKED(IDC_Export_chkAllFields, OnAllFieldsChanged)
    ON_BN_CLICKED(IDC_Export_btnSelectFields, OnSelectFields)
    ON_BN_CLICKED(IDC_Export_btnBrowseCCT, OnBrowseCCT)
    ON_BN_CLICKED(IDC_Export_btnBrowseOutputFile, OnBrowseOutputFile)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportProcessSFDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_Export_edtName, (char*)buffer, DLGBUFMAX ); // 1.4hdd
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_Export_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdd
//    GetDlgItemText( IDC_Export_edtCCT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtCCT, sDlgItem ); // 1.4qpw
	m_sPathCCT = sUTF8( sDlgItem ); // 1.4qpw
	if ( m_sPathCCT.GetLength() > 126 ) // 1.5.4e Check for CCT path too long
        {
        MessageBeep(0); // 1.5.4e 
        AfxMessageBox("Path to Consistent Changes Table too long, maximum 126 characters"); // 1.5.4e 
        return; // 1.5.4e 
        }
//    GetDlgItemText( IDC_Export_edtOutputFile, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtOutputFile, sDlgItem ); // 1.4qpw
	m_sPathSF = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_Export_edtChainProcess, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtChainProcess, sDlgItem ); // 1.4qpw
	m_sChainProcess = sUTF8( sDlgItem ); // 1.4qpw

    Str8 sMessage;
    if ( !m_pexpset->bValidNameDlg(m_sName, m_pexp, sMessage) )
        {
        MessageBeep(0);
        AfxMessageBox(sMessage);
        m_edtName.SetFocus();
        m_edtName.SetSel(0, -1);  // Select the entire name
        return;
        }

    if ( m_pexp )  // Modify
        {
        // TO DO: Make sure the process name isn't empty or duplicate
        if ( strcmp(m_sName, m_pexp->sName()) != 0 )
            m_pexp->ChangeNameTo(m_sName);
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathSF );
        m_pexp->m_sPathCCT = m_sPathCCT;
        m_pexp->m_sPathSF = m_sPathSF; // 1.1ba Include output file in export process
        m_pexp->m_subsetMarkersToExport = m_subsetMarkersToExport;
        m_pexp->m_bCheckConsistency = m_bCheckConsistency;  // 1999-06-14 MRP
        m_pexp->m_sChainProcess = m_sChainProcess;
        m_pexp->m_bOverwriteOutputFile = m_bOverwriteOutputFile;
        m_pexp->m_bAddAlphDiv = m_bAddAlphDiv;
        m_pexp->m_bOpenExportedFile = m_bOpenExportedFile;
        }
    else  // Add or Copy
        {
        *m_ppexpNew = new CExportProcessSF(m_sName, m_pexpset,
            m_sPathCCT, m_subsetMarkersToExport, m_bCheckConsistency);
        m_pexpset->Add(*m_ppexpNew);
	    CExportProcessSF* pexp = *m_ppexpNew;
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathSF );
		pexp->m_sPathCCT = m_sPathCCT; // 1.4bg Don't lose cc file name on add export process
		pexp->m_sPathSF = m_sPathSF; // 1.1bj Don't lose file name on add process
		pexp->m_sChainProcess = m_sChainProcess;
        pexp->m_bOverwriteOutputFile = m_bOverwriteOutputFile;
        pexp->m_bAddAlphDiv = m_bAddAlphDiv;
        pexp->m_bOpenExportedFile = m_bOpenExportedFile;
        }

    CDialog::OnOK();
}

void CExportProcessSFDlg::OnAllFieldsChanged()
{
    BOOL bAllFields = (m_chkAllFields.GetCheck() != 0);
    m_subsetMarkersToExport.IncludeAllElements(bAllFields);
    m_btnSelectFields.EnableWindow(!bAllFields);
}

void CExportProcessSFDlg::OnSelectFields()
{
    CSelectFieldsDlg dlg(m_subsetMarkersToExport, TRUE);
    dlg.DoModal();
}

void CExportProcessSFDlg::OnBrowseCCT()
{
    UpdateData(TRUE);  // from dialog controls to object members
	Str8 sFileFilter = _("Change Table"); // 1.5.0fz 
	sFileFilter += " (*.cct)|*.cct||"; // 1.5.0fz 
    Str8 sDirPath;
//    Str8 sDirPath = m_pexpsetMyOwner->
//        ptypMyOwner()->ptypsetMyOwner()->sSettingsDirPath();

    if ( CExportProcess::s_bBrowseForFile(_("Browse for a Consistent Changes Table"), NULL, sFileFilter, sDirPath, m_sPathCCT, IDD) )
	    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4gj // 1.4qpv
}

void CExportProcessSFDlg::OnBrowseOutputFile()
{
    UpdateData(TRUE);  // from dialog controls to object members
    int iFilterIndex = 1;
	Str8 sFileFilter = _("All files"); // 1.5.0fz 
	sFileFilter += " (*.*)|*.*||"; // 1.5.0fz 
    if ( CExportProcess::s_bExportedFileDlg(NULL, FALSE, sFileFilter, &iFilterIndex, m_sPathSF) )
	    SetDlgItemText( IDC_Export_edtOutputFile, swUTF16( m_sPathSF ) ); // 1.4gj // 1.4qpv
}

// --------------------------------------------------------------------------

// Add button
CExportProcessXMLDlg::CExportProcessXMLDlg(CExportProcessSet* pexpset,
        CExportProcessXML** ppexpNew) :
    CDialog(CExportProcessXMLDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpset),
    m_bEnableName(TRUE),
    m_bCheckConsistency(TRUE),
    m_bOverwriteOutputFile(FALSE),
    m_bAddAlphDiv(FALSE),
    m_subsetMarkersToExport(m_pexpset->pmkrset())
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Copy button
CExportProcessXMLDlg::CExportProcessXMLDlg(const CExportProcessXML* pexpToCopyFrom,
        CExportProcessXML** ppexpNew) :
    CDialog(CExportProcessXMLDlg::IDD),
    m_pexp(NULL),
    m_ppexpNew(ppexpNew),
    m_pexpset(pexpToCopyFrom->m_pexpsetMyOwner),
    m_bEnableName(TRUE),
    m_sPathCCT(pexpToCopyFrom->m_sPathCCT),
    m_sPathStylesheet(pexpToCopyFrom->m_sPathStylesheet),
    m_bCheckConsistency(pexpToCopyFrom->m_bCheckConsistency),
    m_bOverwriteOutputFile(pexpToCopyFrom->m_bOverwriteOutputFile), // 1.4ytg 
    m_bAddAlphDiv(pexpToCopyFrom->m_bAddAlphDiv),
    m_subsetMarkersToExport(pexpToCopyFrom->m_subsetMarkersToExport)
{
    ASSERT( m_ppexpNew );
    ASSERT( m_pexpset );
}

// Modify button
CExportProcessXMLDlg::CExportProcessXMLDlg(CExportProcessXML* pexp,
        BOOL bEnableName) :
    CDialog(CExportProcessXMLDlg::IDD),
    m_pexp(pexp),
    m_ppexpNew(NULL),
    m_pexpset(m_pexp->m_pexpsetMyOwner),
    m_bEnableName(bEnableName),
    m_sName(m_pexp->sName()),
    m_sPathCCT(m_pexp->m_sPathCCT),
    m_sPathStylesheet(m_pexp->m_sPathStylesheet),
    m_sPathXML(m_pexp->m_sPathXML), // 1.1bb Include output file in export process
    m_bCheckConsistency(m_pexp->m_bCheckConsistency),
    m_bOverwriteOutputFile(m_pexp->m_bOverwriteOutputFile),
    m_bAddAlphDiv(m_pexp->m_bAddAlphDiv),
    m_subsetMarkersToExport(m_pexp->m_subsetMarkersToExport)
{
    ASSERT( m_pexp );
}

CExportProcessXMLDlg::~CExportProcessXMLDlg()
{
}

void CExportProcessXMLDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportProcessXMLDlg)
    DDX_Control(pDX, IDC_Export_edtName, m_edtName);
    DDX_Control(pDX, IDC_Export_chkAllFields, m_chkAllFields);
    DDX_Control(pDX, IDC_Export_btnSelectFields, m_btnSelectFields);
//    DDX_Txt(pDX, IDC_Export_edtCCT, m_sPathCCT);
//    DDX_Txt(pDX, IDC_Export_edtStylesheet, m_sPathStylesheet);
//    DDX_Txt(pDX, IDC_Export_edtOutputFile2, m_sPathXML);
    DDX_Check(pDX, IDC_Export_chkCheckConsistency, m_bCheckConsistency);
    DDX_Check(pDX, IDC_Export_chkOverwrite, m_bOverwriteOutputFile);
    DDX_Check(pDX, IDC_Export_chkAddAlphDiv, m_bAddAlphDiv);
    //}}AFX_DATA_MAP
//    m_pexpset->DDX_Name(pDX, IDC_Export_edtName, m_sName); // 1.4hdd
}

BOOL CExportProcessXMLDlg::OnInitDialog()
{
    BOOL b = CDialog::OnInitDialog();
    SetDlgItemText( IDC_Export_edtName, swUTF16( m_sName ) ); // 1.4hdd // 1.4qpv
    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtStylesheet, swUTF16( m_sPathStylesheet ) ); // 1.4qpv
    SetDlgItemText( IDC_Export_edtOutputFile2, swUTF16( m_sPathXML ) ); // 1.4qpv

    BOOL bAllFields = m_subsetMarkersToExport.bIncludeAllElements();    
    m_chkAllFields.SetCheck(bAllFields ? 1 : 0);
    m_btnSelectFields.EnableWindow(!bAllFields);
    
    m_edtName.EnableWindow(m_bEnableName);  // 1998-03-10 MRP
    if ( m_pexp )
        {
        // When modifying an existing export process,
        // don't let the Name edit box have focus. 
        m_chkAllFields.SetFocus();
        return FALSE;
        }
    
    return b;
}


BEGIN_MESSAGE_MAP(CExportProcessXMLDlg, CDialog)
    //{{AFX_MSG_MAP(CExportProcessXMLDlg)
    ON_BN_CLICKED(IDC_Export_chkAllFields, OnAllFieldsChanged)
    ON_BN_CLICKED(IDC_Export_btnSelectFields, OnSelectFields)
    ON_BN_CLICKED(IDC_Export_btnBrowseCCT, OnBrowseCCT)
    ON_BN_CLICKED(IDC_Export_btnBrowseStylesheet, OnBrowseStylesheet)
    ON_BN_CLICKED(IDC_Export_btnBrowseOutputFile2, OnBrowseOutputFile)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportProcessXMLDlg::OnOK()
{
    UpdateData(TRUE);  // from dialog controls to dialog object member data
//    GetDlgItemText( IDC_Export_edtName, (char*)buffer, DLGBUFMAX ); // 1.4hdd
	CString sDlgItem; // 1.4qpw
	GetDlgItemText( IDC_Export_edtName, sDlgItem ); // 1.4qpw
	m_sName = sUTF8( sDlgItem ); // 1.4qpw
	TrimBothEnds( m_sName ); // 1.4hdd
//    GetDlgItemText( IDC_Export_edtCCT, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtCCT, sDlgItem ); // 1.4qpw
	m_sPathCCT = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_Export_edtStylesheet, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtStylesheet, sDlgItem ); // 1.4qpw
	m_sPathStylesheet = sUTF8( sDlgItem ); // 1.4qpw
//    GetDlgItemText( IDC_Export_edtOutputFile2, (char*)buffer, DLGBUFMAX );
	GetDlgItemText( IDC_Export_edtOutputFile2, sDlgItem ); // 1.4qpw
	m_sPathXML = sUTF8( sDlgItem ); // 1.4qpw

    Str8 sMessage;
    if ( !m_pexpset->bValidNameDlg(m_sName, m_pexp, sMessage) )
        {
        MessageBeep(0);
        AfxMessageBox(sMessage);
        m_edtName.SetFocus();
        m_edtName.SetSel(0, -1);  // Select the entire name
        return;
        }

    if ( m_pexp )  // Modify
        {
        // TO DO: Make sure the process name isn't empty or duplicate
        if ( strcmp(m_sName, m_pexp->sName()) != 0 )
            m_pexp->ChangeNameTo(m_sName);
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathStylesheet );
		AddSettingsPathIfNone( m_sPathXML );
        m_pexp->m_sPathCCT = m_sPathCCT;
        m_pexp->m_sPathStylesheet = m_sPathStylesheet;
        m_pexp->m_sPathXML = m_sPathXML; // 1.1bb Include output file in export process
        m_pexp->m_subsetMarkersToExport = m_subsetMarkersToExport;
        m_pexp->m_bCheckConsistency = m_bCheckConsistency;
        m_pexp->m_bOverwriteOutputFile = m_bOverwriteOutputFile;
        m_pexp->m_bAddAlphDiv = m_bAddAlphDiv;
        }
    else  // Add or Copy
        {
        *m_ppexpNew = new CExportProcessXML(m_sName, m_pexpset,
            m_sPathCCT, m_sPathStylesheet, m_subsetMarkersToExport,
            m_bCheckConsistency);
        m_pexpset->Add(*m_ppexpNew);
	    CExportProcessXML* pexp = *m_ppexpNew;
		AddSettingsPathIfNone( m_sPathCCT );
		AddSettingsPathIfNone( m_sPathStylesheet );
		AddSettingsPathIfNone( m_sPathXML );
		pexp->m_sPathCCT = m_sPathCCT; // 1.4bg Don't lose cc file name on add export process
		pexp->m_sPathStylesheet = m_sPathStylesheet; // 1.4bg Don't lose cc file name on add export process
		pexp->m_sPathXML = m_sPathXML; // 1.4bg Don't lose cc file name on add export process
        }

    CDialog::OnOK();
}

void CExportProcessXMLDlg::OnAllFieldsChanged()
{
    BOOL bAllFields = (m_chkAllFields.GetCheck() != 0);
    m_subsetMarkersToExport.IncludeAllElements(bAllFields);
    m_btnSelectFields.EnableWindow(!bAllFields);
}

void CExportProcessXMLDlg::OnSelectFields()
{
    CSelectFieldsDlg dlg(m_subsetMarkersToExport, TRUE);
    dlg.DoModal();
}

void CExportProcessXMLDlg::OnBrowseCCT()
{
    UpdateData(TRUE);  // from dialog controls to object members
	Str8 sFileFilter = _("Change Table"); // 1.5.0fz 
	sFileFilter += " (*.cct)|*.cct||"; // 1.5.0fz 
    Str8 sDirPath;
//    Str8 sDirPath = m_pexpsetMyOwner->
//        ptypMyOwner()->ptypsetMyOwner()->sSettingsDirPath();

    if ( CExportProcess::s_bBrowseForFile(_("Browse for a Consistent Changes Table"), NULL, sFileFilter, sDirPath, m_sPathCCT, IDD) )
	    SetDlgItemText( IDC_Export_edtCCT, swUTF16( m_sPathCCT ) ); // 1.4gj // 1.4qpv
}

void CExportProcessXMLDlg::OnBrowseStylesheet()
{
    UpdateData(TRUE);  // from dialog controls to object members
    const char pszDefaultExt[] = "xsl";
    Str8 sFileFilter = _("Hypertext Stylesheet");
	sFileFilter = sFileFilter + " (*.xsl)|*.xsl||";
    Str8 sDirPath;
//    Str8 sDirPath = sGetDirPath(m_sPathDOT);

    if ( CExportProcess::s_bBrowseForFile(_("Browse for a Hypertext Stylesheet"), pszDefaultExt, sFileFilter, sDirPath, m_sPathStylesheet, IDD) )
	    SetDlgItemText( IDC_Export_edtStylesheet, swUTF16( m_sPathStylesheet ) ); // 1.4gj // 1.4qpv
}

void CExportProcessXMLDlg::OnBrowseOutputFile()
{
    UpdateData(TRUE);  // from dialog controls to object members
    int iFilterIndex = 1;
	Str8 sFileFilter = _("All files"); // 1.5.0fz 
	sFileFilter += " (*.*)|*.*||"; // 1.5.0fz 
    if ( CExportProcess::s_bExportedFileDlg(NULL, FALSE, sFileFilter, &iFilterIndex, m_sPathXML) )
	    SetDlgItemText( IDC_Export_edtOutputFile2, swUTF16( m_sPathXML ) ); // 1.4gj // 1.4qpv
}


// --------------------------------------------------------------------------

CExportCurrentWindowDlg::CExportCurrentWindowDlg(const CMarker* pmkrPriKey,
            BOOL* pbPriKeyFieldFirst) :
    CDialog(CExportCurrentWindowDlg::IDD),
    m_pmkrPriKey(pmkrPriKey),
    m_pbPriKeyFieldFirst(pbPriKeyFieldFirst)
{
    ASSERT( m_pmkrPriKey );
    ASSERT( m_pbPriKeyFieldFirst );
    if( *m_pbPriKeyFieldFirst )
        m_iFieldOrder = 1;
    else
        m_iFieldOrder = 0;
}

void CExportCurrentWindowDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CExportCurrentWindowDlg)
    DDX_Radio(pDX, IDC_Export_optWithoutReordering, m_iFieldOrder);
    DDX_Control(pDX, IDC_Export_lblPriKeyMarker, m_lblPriKeyMarker);
    DDX_Control(pDX, IDC_Export_lblOtherFields, m_lblOtherFields);
    //}}AFX_DATA_MAP
}

BOOL CExportCurrentWindowDlg::OnInitDialog()
{
    BOOL bRet = CDialog::OnInitDialog();
    
    Str8 sLabel = "\\"; // 1.5.0fg 
	sLabel = sLabel + m_pmkrPriKey->sMarker() + " " + m_pmkrPriKey->sFieldName(); // 1.5.0fg 
    m_lblPriKeyMarker.SetWindowText( swUTF16( sLabel) ); // 1.4qpy
	sLabel = _("Others of same field in same record are omitted"); // 1.5.0fg 
    m_lblOtherFields.SetWindowText( swUTF16( sLabel) ); // 1.4qpy
    
    return bRet;
}

BEGIN_MESSAGE_MAP(CExportCurrentWindowDlg, CDialog)
    //{{AFX_MSG_MAP(CExportCurrentWindowDlg)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExportCurrentWindowDlg::OnOK()
{
    CDialog::OnOK();
    *m_pbPriKeyFieldFirst = ( m_iFieldOrder == 1 );
}
