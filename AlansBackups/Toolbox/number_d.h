// number_d.h  (Re)Numbering dialog boxes

#ifndef NUMBER_D_H
#define NUMBER_D_H

#include "resource.h"
#include "number.h"
#include "cbo.h"  // class CSetComboBox
#include "mrfset_d.h" // class CMarkerSubSetBox
#include "EditLng.h"

// --------------------------------------------------------------------------

class CMarkerUseContentsComboBox : public CSetComboBox
{
private:
public:
    CMarkerUseContentsComboBox(CMarkerSet* pmkrset) : CSetComboBox(pmkrset) {}
    void UpdateElements(CMarker* pmkrToSelect) { CSetComboBox::UpdateElements(pmkrToSelect); }
    CMarker* pmkrSelected() { return (CMarker*)CSetComboBox::pselSelected(); }
    BOOL bIncludeElement(const CSetEl* psel) const;
};

// --------------------------------------------------------------------------

class CMarkerNumberingComboBox : public CSetComboBox
{
private:
	CMarker* m_pmkrDontUse;
public:
    CMarkerNumberingComboBox(CMarkerSet* pmkrset, CMarker* pmkrDontUse = NULL);
    void UpdateElements(CMarker* pmkrToSelect, CMarker* pmkrDontUse = NULL);
    CMarker* pmkrSelected() { return (CMarker*)CSetComboBox::pselSelected(); }
    BOOL bIncludeElement(const CSetEl* psel) const;
};

// --------------------------------------------------------------------------

class CReNumberDlg : public CDialog
{
protected:
	CNumberProcess* m_pnbrproc;
private:
	const CField* m_pfldCurRecordMkr;
	long m_lStartRef;

    // Read-only "Edit" control for displaying the record marker field contents
    CEditLng m_elcRecord; // 1.4qpn

    // Dialog Data
    //{{AFX_DATA(CReNumberDlg)
    enum { IDD = IDD_RENUMBER };
    CButton m_optCurRecord;
    CButton m_optWholeDB;
	CEdit m_edtStartRef;
	Str8 m_sMkrRef;
    Str8 m_sMkrTxt;
    //}}AFX_DATA


public:
    CReNumberDlg(CNumberProcess* pnbrproc, const CField* pfldCurRecordMkr, int iIDD = IDD_RENUMBER);

protected:
	virtual void InitControls();
	virtual BOOL bValidate();
	virtual void UpdateNumberProc();
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CReNumberDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnOK();
    afx_msg void OnSelectFields();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CReNumberDlg

// --------------------------------------------------------------------------

class CNumberDlg : public CReNumberDlg
{
private:
    CMarkerSubSet m_subsetInvalidMarkersForTextBreak;

// Dialog Data
    //{{AFX_DATA(CNumberDlg)
    enum { IDD = IDD_NUMBER };
	Str8 m_sPunctuation;
	CMarkerSubSetBox m_lboTextBreakMkrs;
    CButton m_optUseContents;
	CMarkerUseContentsComboBox m_cboUseContents;
    CButton m_optUseText;
	CEdit m_edtUseText;
    //}}AFX_DATA

	virtual void InitControls();
	virtual BOOL bValidate();
	virtual void UpdateNumberProc();

public:
    CNumberDlg(CNumberProcess* pnbrproc, const CField* pfldCurRecordMkr);

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CNumberDlg)
	afx_msg void OnSelectFields();
    afx_msg void SetState();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CNumberDlg

// --------------------------------------------------------------------------

class CSetupNbrDlg : public CDialog
{
private:

// Dialog Data
    //{{AFX_DATA(CSetupNbrDlg)
    enum { IDD = IDD_Set_Up_Numbering };
    Str8 m_sDBName;
    //}}AFX_DATA

public:
    CSetupNbrDlg(const Str8 sDBName);
    virtual void OnOK();

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    
    // Generated message map functions
    //{{AFX_MSG(CSetupNbrDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};  // class CSetupNbrDlg

// --------------------------------------------------------------------------

class CDbTypNumberingPage : public CPropertyPage
{
private:
    BOOL m_bWasActive; // reflects whether this page was ever active
    CDatabaseType* m_ptyp;

// Construction
public:
    CDbTypNumberingPage( CDatabaseType* ptyp );

private:
// Dialog Data
    //{{AFX_DATA(CDbTypNumberingPage)
    enum { IDD = IDD_DBTYP_NUMBERING_PAGE };
	Str8 m_sDBTypeName;
    Str8 m_sMkrRec;
    CMarkerNumberingComboBox m_cboMkrRef;
    CMarkerNumberingComboBox m_cboMkrTxt;
    BOOL m_bNumberingDisabled;
    //}}AFX_DATA

public:
    virtual void OnOK();
    virtual BOOL OnSetActive(); // called when our page is switched to

private:
	virtual void SetState();

// Implementation
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

    // Generated message map functions
    //{{AFX_MSG(CDbTypNumberingPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnDisableNumberingChanged();
	afx_msg void OnSelchangeMkrRef();
	afx_msg void OnSelchangeMkrTxt();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // not NUMBER_D_H
