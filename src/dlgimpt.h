// dlgimpt.h : header file
//

#include "lng.h"
#include "typ_d.h"



/////////////////////////////////////////////////////////////////////////////
// CImportDlg dialog

class CImportDlg : public CDialog
{
private:
    Str8& m_sFileText;
    const int m_nMode;      // import mode
    const Str8& m_sMissingDBType;
    CDatabaseTypeProxy** m_pptrx;
    BOOL* m_pbMakeOri;
    Str8 m_sCCTable;
    
// Dialog Data
    //{{AFX_DATA(CImportDlg)
    enum { IDD = IDD_FILE_IMPORT };
    CStatic m_lblWhy;       // label telling user why import is needed
    Str8 m_sSourceFile;
    CDatabaseTypeSetComboBox m_cboDatabaseType;
    CButton m_grpCCStuff;  // 1998-03-04 MRP
    CButton m_chkUseCCTable;
    CStatic m_lblCCTable;
    CButton m_bnBrowseCCTable;
    BOOL m_bRemoveSpaces;
    CButton m_chkRemoveSpaces;
    BOOL m_bMakeOri;
    CButton m_chkMakeOri;
    //}}AFX_DATA

    
// Construction
public:
    CImportDlg( const char* pszPathName, Str8& sFileText,
                    const int nMode, const Str8& sMissingDBType,
                    CDatabaseTypeProxy** pptrx, BOOL* pbMakeOri);

// Implementation
private:
    BOOL bGetCCTable( BOOL bOnlyIfNecessary = FALSE ); // use file open commdlg to select CC table
    BOOL bCCTableExists(); // check for existance of filename in m_sCCTable

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CImportDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnAddNewType();
    afx_msg void OnBrowseCctable();
    afx_msg void OnUseCctable();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CDBTypeNFDlg dialog

class CDBTypeNFDlg : public CDialog  // Hungarian: dlg
{
private:
    const char* m_pszPathName;      // file path to display
    const Str8& m_sMissingDBType;    // missing DB Type name to display

// Dialog Data
    //{{AFX_DATA(CDBTypeNFDlg)
    enum { IDD = IDD_DBTypeNotFound };
    CStatic m_lblToPrint;
    //}}AFX_DATA

// Construction
public:
    CDBTypeNFDlg(const char* pszPathName, const Str8 &sMissingDBType);
    ~CDBTypeNFDlg();

// Implementation
protected:          
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

};  // end of class CDBTypeNFDlg


