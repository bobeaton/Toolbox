// mdf_d.h  Multi-Dictionary Formatter property sheet and pages

#ifndef MDF_D_H
#define MDF_D_H

#include "EditLng.h" // 1.4qmb
#include "resource.h"
#include "filsel_d.h"  // CFilterSetComboBox
#include "mkr.h"  // CMarkerSubSet

class CExportProcessMDF;  // mdf.h
class CExportProcessSet;  // export.h
class RtfPageSetup;  // pgs.h
class CIndexSet;  // ind.h
class CFilter;  // fil.h


// **************************************************************************

class CMDFDlg : public CDialog  // Hungarian: dlg
{
private:
    CExportProcessMDF* m_pmdf;
    CExportProcessMDF** m_ppexpNew;
    CExportProcessSet* m_pexpset;
    BOOL m_bEnableName;

    CFilter* m_pfil;

    // 1999-01-25 MRP: Finderlist language is independent of audience
    Str8 m_aasTitle[2][5];
    CLangEnc* m_plngEnglish;
    CLangEnc* m_plngNational;   
    CEditLng m_elcTitle; // 1.4qpn // 1.4qmb Change most CLngEditCtrl to CEditLng

// Dialog Data
    //{{AFX_DATA(CMDFDlg)
    enum { IDD = IDD_MDF };
    CEdit m_edtName;
    Str8 m_sName;
    CButton m_optEnglish;
    int m_iAudience;
    int m_iFormat;
    CFilterSetComboBox m_cboFilter;
    int m_iExportedFileType;  // 1999-02-01 MRP
    BOOL m_bCheckConsistency;
    //}}AFX_DATA

    friend class CMDFOptionsDlg;    
    BOOL m_bSortByCitationForm;
    BOOL m_bIncludeLexemeWithCitationForm;
    BOOL m_bIncludeExamples;
    BOOL m_bIncludeNotes;
    BOOL m_bIncludeRegionalWithNational;
    BOOL m_bIncludeNonMDF;
    BOOL m_bIncludePartOfSpeech;        
    Str8 m_sCopyright;  // 1999-01-25 MRP
    BOOL m_bHeader;
    BOOL m_bDate;
    BOOL m_bTotal;
    CMarkerSubSet m_subsetMarkersToExport;
    RtfPageSetup* m_ppgs;
    
    friend class CMDFAudienceSetupDlg;    
    Str8 m_sEnglishLabelsCct;
    Str8 m_sEnglishDot;
    Str8 m_sNationalLabelsCct;
    Str8 m_sNationalDot;

    void Init();

public:
    CMDFDlg(CExportProcessSet* pexpset,
    		BOOL bWebPages,  // 1998-10-16 MRP
            CExportProcessMDF** ppexpNew);
        // Add button
    CMDFDlg(const CExportProcessMDF* pexpToCopyFrom,
            CExportProcessMDF** ppexpNew);
        // Copy button
    CMDFDlg(CExportProcessMDF* pmdf, BOOL bEnableName);
        // Modify button
    ~CMDFDlg();

    const CExportProcessMDF* pmdf() { return m_pmdf; }

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CMDFDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnAudienceSetup();
    afx_msg void OnOptions();
    afx_msg void OnAudienceOrFormatChanged();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMDFDlg


// --------------------------------------------------------------------------

class CMDFOptionsDlg : public CDialog  // Hungarian: dlg
{
private:
    CMDFDlg* m_pdlg;
    CMarkerSubSet m_subsetMarkersToExport;
    RtfPageSetup* m_ppgs;
    Str8 m_sChainProcess;

// Dialog Data
    //{{AFX_DATA(CMDFOptionsDlg)
    enum { IDD = IDD_MDF_Options };
    BOOL m_bSortByCitationForm;
    BOOL m_bIncludeLexemeWithCitationForm;
    BOOL m_bIncludeExamples;
    BOOL m_bIncludeNotes;
    BOOL m_bIncludeNonMDF;
    BOOL m_bIncludeRegionalWithNational;
    BOOL m_bIncludePartOfSpeech;        
    // 1999-01-25 MRP: Added copyright and moved hierarchy from MDF dialog box.
    Str8 m_sCopyright;
    Str8 m_sMarkerHierarchy;
    BOOL m_bHeader;
    BOOL m_bDate;
    BOOL m_bTotal;
    //}}AFX_DATA

public:
    CMDFOptionsDlg(CMDFDlg* pdlg);
    ~CMDFOptionsDlg();

    virtual void OnOK();            // ok or apply now pressed -- KillActive is called first

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CMDFOptionsDlg)
    afx_msg void OnSelectFields();
    afx_msg void OnPageSetup();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMDFOptionsDlg


// --------------------------------------------------------------------------

class CMDFAudienceSetupDlg : public CDialog  // Hungarian: dlg
{
private:
    CMDFDlg* m_pdlg;

// Dialog Data
    //{{AFX_DATA(CMDFAudienceSetupDlg)
    enum { IDD = IDD_MDF_AudienceSetup };
    Str8 m_sEnglishLabelsCct;
    Str8 m_sEnglishDot;
    Str8 m_sNationalLabelsCct;
    Str8 m_sNationalDot;
    //}}AFX_DATA

public:
    CMDFAudienceSetupDlg(CMDFDlg* pdlg);
    ~CMDFAudienceSetupDlg();

    virtual void OnOK();

protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CMDFAudienceSetupDlg)
    afx_msg void OnBrowseEnglishLabelsCct();
    afx_msg void OnBrowseEnglishDot();
    afx_msg void OnBrowseNationalLabelsCct();
    afx_msg void OnBrowseNationalDot();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMDFAudienceSetupDlg


// --------------------------------------------------------------------------

// 1998-01-21 MRP

class CMDFReminderDlg : public CDialog  // Hungarian: dlg
{
private:
    CExportProcessSet* m_pexpset;

// Dialog Data
    //{{AFX_DATA(CMDFReminderDlg)
    enum { IDD = IDD_MDF_Reminder };
    CStatic m_lblToPrint;
    BOOL m_bNoMDFReminder;
    //}}AFX_DATA

public:
    CMDFReminderDlg(CExportProcessSet* pexpset);
    ~CMDFReminderDlg();

protected:          
    virtual void OnOK();
    virtual void OnCancel();

    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    virtual BOOL OnInitDialog();
};  // class CMDFReminderDlg
                         

// --------------------------------------------------------------------------

// 1999-02-27 MRP
// 1999-03-05 MRP: Use the term "Update" instead of "Convert"

class CMDFUpdateOlderDlg : public CDialog
{
private:
    CExportProcessSet* m_pexpset;
    BOOL m_bShow;  // 1999-03-09 MRP
    
// Dialog Data
    //{{AFX_DATA(CConvertSettingsDlg)
    enum { IDD = IDD_MDF_UpdateOlder };
    Str8 m_sShoeboxVersion;
    Str8 m_sProjectSettings;
    Str8 m_sDefaultSettings;
    Str8 m_sNewerMDF;
    //}}AFX_DATA

public:
    CMDFUpdateOlderDlg(CExportProcessSet* pexpset);

    BOOL bShow() const { return m_bShow; }
    
// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CConvertSettingsDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CMDFUpdateOlderDlg

// **************************************************************************

#endif  // MDF_D_H
