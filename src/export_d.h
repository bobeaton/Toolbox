// export_d.h  File Export dialog boxes

#ifndef EXPORT_D_H
#define EXPORT_D_H

#include "resource.h"
#include "export.h"
#include "lbo.h"  // CSetListBox
#include "set_d.h"  // CSetDlg

class CIndex;  // ind.h
class CRecLookEl;  // crlookel.h
class RtfPageSetup;  // pgs.h


// --------------------------------------------------------------------------

class CExportProcessTypeDlg : public CDialog
{
private:
    BOOL m_bEnableMDF;
    BOOL* m_pbMDF;
    BOOL* m_pbXML;  // 1999-05-21 MRP
    BOOL* m_pbRTF;

// Dialog Data
    //{{AFX_DATA(CExportProcessTypeDlg)
    enum { IDD = IDD_Export };
    int m_iFormat;
    CButton m_optMDF;
    //}}AFX_DATA

public:
    CExportProcessTypeDlg(BOOL bEnableMDF, BOOL* pbMDF, BOOL* pbXML,
    		BOOL* pbRTF);
    
    enum { eSF = 0, eRTF, eXML, eMDF };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportProcessTypeDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CExportProcessTypeDlg


// --------------------------------------------------------------------------

class CExportProcessListBox : public CSetListBox
{
public:
    CExportProcessListBox(CExportProcessSet* pexpset, CExportProcess** ppexp) :
        CSetListBox(pexpset, (CSetEl**)ppexp) {}

    CExportProcess* pexpSelected();
    
protected:
    int m_xExportProcesses;
    int m_xType;
    
    virtual void InitLabels();
    virtual void DrawElement(CDC& cDC, const RECT& rcItem,
            const CDblListEl *pel);
    virtual BOOL bIsBold(CSetEl* psel) { return FALSE; }
};  // class CExportProcessListBox

// --------------------------------------------------------------------------

class CExportProcessDlg : public CDialog
{
private:
    CExportProcessSet* m_pexpset;
    CExportProcess* m_pexpDefault;
    Str8 m_sEntireDatabase;
    Str8 m_sLexicalDatabase;
    Str8 m_sFiltering;
    Str8 m_sSorting;
    BOOL m_bCurrentRecordEnabled;
    
    enum { eEntireDatabase = 0, eCurrentWindow, eCurrentRecord };

// Dialog Data
    CExportProcessListBox m_lboExportProcesses;
    //{{AFX_DATA(CExportProcessDlg)
    enum { IDD = IDD_ExportProcess };
    int m_iRecordsToInclude;
    CButton m_optEntireDatabase;
    CButton m_optCurrentWindow;
    CStatic m_lblEntireDatabase;
    CStatic m_lblFiltering;
    CStatic m_lblSorting;
    CButton m_optCurrentRecord;
    BOOL m_bShowProperties;
    CButton m_chkAutoOpen;
    BOOL m_bAutoOpen;
    Str8 m_sDatabaseType;  // 1999-01-25 MRP
    CButton m_btnAdd;
    CButton m_btnCopy;
    CButton m_btnModify;
    CButton m_btnDelete;
    //}}AFX_DATA

    // 1998-03-10 MRP
    // Show Properties dialog box on Export check box
    // Add, Copy, Modify, and Delete buttons

public:
    CExportProcessDlg(CExportProcessSet* pexpset,
            CIndex* pindCur, const CRecLookEl* prelCur,
            const Str8& sPathDatabase);
    ~CExportProcessDlg();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportProcessDlg)
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    virtual void OnOK();
    afx_msg void OnDblclkExportProcess();
    afx_msg void OnSelchangeExportProcess();
    afx_msg void OnAdd();
    afx_msg void OnCopy();
    afx_msg void OnModify();
    afx_msg void OnDelete();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    void UpdateCopyModifyButtons();
        // Whenever the number of items in the list box changes
    void UpdateDeleteButton();
        // Whenever the current selection changes

    void Filtering(const CFilter* pfil, Str8& sFiltering);
};  // class CExportProcessDlg


// --------------------------------------------------------------------------

class CExportProcessRTFDlg : public CDialog
{
private:
    CExportProcessRTF* m_pexp;
    CExportProcessRTF** m_ppexpNew;
    CExportProcessSet* m_pexpset;
    BOOL m_bEnableName;  // 1998-03-10 MRP

    Str8 m_sName;
    Str8 m_sPathCCT;
    Str8 m_sPathDOT;
    Str8 m_sPathRTF;
    BOOL m_bMarkerFont;
    CMarkerSubSet m_subsetMarkersToExport;
    RtfPageSetup* m_ppgs;

    // 1999-01-29 MRP
    CButton m_chkInterlinearTabAligned;
    BOOL m_bInterlinearTabAligned;
    CStatic m_lblInterlinearSpacing;
    CEdit m_edtInterlinearSpacing;
    RtfTwips m_twxInterlinearSpacing;

// Dialog Data
    //{{AFX_DATA(CExportProcessRTFDlg)
    enum { IDD = IDD_ExportRTF };
    CEdit m_edtName;
    CButton m_chkAllFields;
    CButton m_btnSelectFields;
    int m_iStyleFormatting;
    BOOL m_bCheckConsistency;
    BOOL m_bOverwriteOutputFile;
    BOOL m_bAddAlphDiv;
    //}}AFX_DATA

public:
    CExportProcessRTFDlg(CExportProcessSet* pexpset,
            CExportProcessRTF** ppexpNew);
        // Add button
    CExportProcessRTFDlg(const CExportProcessRTF* pexpToCopyFrom,
            CExportProcessRTF** ppexpNew);
        // Copy button
    CExportProcessRTFDlg(CExportProcessRTF* pexp, BOOL bEnableName);
        // Modify button
    ~CExportProcessRTFDlg();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportProcessRTFDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnAllFieldsChanged();
    afx_msg void OnSelectFields();
    afx_msg void OnBrowseCCT();
    afx_msg void OnBrowseDOT();
    afx_msg void OnPageSetup();
    afx_msg void OnInterlinearTabAligned();
    afx_msg void OnBrowseOutputFile();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CExportProcessRTFDlg


// --------------------------------------------------------------------------

class CExportProcessSFDlg : public CDialog
{
private:
    CExportProcessSF* m_pexp;
    CExportProcessSF** m_ppexpNew;
    CExportProcessSet* m_pexpset;
    BOOL m_bEnableName;  // 1998-03-10 MRP

    Str8 m_sName;
    Str8 m_sPathCCT;
    Str8 m_sPathSF;
    Str8 m_sChainProcess;
    CMarkerSubSet m_subsetMarkersToExport;

// Dialog Data
    //{{AFX_DATA(CExportProcessSFDlg)
    enum { IDD = IDD_ExportSF };
    CEdit m_edtName;
    CButton m_chkAllFields;
    CButton m_btnSelectFields;
    BOOL m_bCheckConsistency;
    BOOL m_bOverwriteOutputFile;
    BOOL m_bAddAlphDiv;
    BOOL m_bOpenExportedFile;
    //}}AFX_DATA

public:
    CExportProcessSFDlg(CExportProcessSet* pexpset,
            CExportProcessSF** ppexpNew);
        // Add button
    CExportProcessSFDlg(const CExportProcessSF* pexpToCopyFrom,
            CExportProcessSF** ppexpNew);
        // Copy button
    CExportProcessSFDlg(CExportProcessSF* pexp, BOOL bEnableName);
        // Modify button
    ~CExportProcessSFDlg();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportProcessSFDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnAllFieldsChanged();
    afx_msg void OnSelectFields();
    afx_msg void OnBrowseCCT();
    afx_msg void OnBrowseOutputFile();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CExportProcessSFDlg


// --------------------------------------------------------------------------

class CExportProcessXMLDlg : public CDialog
{
private:
    CExportProcessXML* m_pexp;
    CExportProcessXML** m_ppexpNew;
    CExportProcessSet* m_pexpset;
    BOOL m_bEnableName;  // 1998-03-10 MRP

    Str8 m_sName;
    Str8 m_sPathCCT;
    Str8 m_sPathStylesheet;
    Str8 m_sPathXML;
    CMarkerSubSet m_subsetMarkersToExport;

// Dialog Data
    //{{AFX_DATA(CExportProcessXMLDlg)
    enum { IDD = IDD_ExportXML };
    CEdit m_edtName;
    CButton m_chkAllFields;
    CButton m_btnSelectFields;
    BOOL m_bCheckConsistency;
    BOOL m_bOverwriteOutputFile;
    BOOL m_bAddAlphDiv;
    //}}AFX_DATA

public:
    CExportProcessXMLDlg(CExportProcessSet* pexpset,
            CExportProcessXML** ppexpNew);
        // Add button
    CExportProcessXMLDlg(const CExportProcessXML* pexpToCopyFrom,
            CExportProcessXML** ppexpNew);
        // Copy button
    CExportProcessXMLDlg(CExportProcessXML* pexp, BOOL bEnableName);
        // Modify button
    ~CExportProcessXMLDlg();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportProcessXMLDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnAllFieldsChanged();
    afx_msg void OnSelectFields();
    afx_msg void OnBrowseCCT();
    afx_msg void OnBrowseStylesheet();
    afx_msg void OnBrowseOutputFile();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CExportProcessXMLDlg


// --------------------------------------------------------------------------

class CExportCurrentWindowDlg : public CDialog
{
private:
    const CMarker* m_pmkrPriKey;
    BOOL* m_pbPriKeyFieldFirst;

// Dialog Data
    //{{AFX_DATA(CExportCurrentWindowDlg)
    enum { IDD = IDD_ExportCurrentWindow };
    int m_iFieldOrder;
    CStatic m_lblPriKeyMarker;
    CStatic m_lblOtherFields;
    //}}AFX_DATA

public:
    CExportCurrentWindowDlg(const CMarker* pmkrPriKey,
            BOOL* pbPriKeyFieldFirst);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CExportCurrentWindowDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CExportCurrentWindowDlg

#endif // not EXPORT_D_H
