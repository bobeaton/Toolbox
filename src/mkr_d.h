// mkr_d.h  Interface for the marker dialog box

#include "resource.h"
#include "mkr.h"
#include "font.h"
#include "crngset.h"
#include "crngst_d.h"

class CMarkerGeneralPage : public CPropertyPage
{
private:
    CMarkerSet*  m_pmkrset;

    CFontDef&    m_fntdef; // font definition for marker
    CMarker*&    m_pmkrOverThis;
    CMarker*&    m_pmkrFollowingThis;
    CLangEncPtr& m_plng;
    CFont        m_fnt; // Needed for creating font to supply to range set page
	BOOL&        m_bCharStyle;

// Dialog Data
    //{{AFX_DATA(CMarkerGeneralPage)
    enum { IDD = IDD_MKR_General_Page };
    CButton m_btnFont;
    CButton m_chkUseDefaultFont;
    CEdit   m_edtNote;
    CEdit   m_edtFieldName;
    CEdit   m_edtMarker;
    CLangEncSetComboBox m_cbolng;
    CMarkerHierarchyComboBox m_cbomkrOverThis;
    CMarkerSetComboBox m_cbomkrFollowingThis;
    Str8& m_sMarker;
    Str8& m_sFieldName;
    BOOL&    m_bUseDefaultFont;
    int      m_iStyle;
    Str8& m_sDescription;
    //}}AFX_DATA
    
//    BOOL m_bSetFocusToMarkerEditBox;  // Is the marker defined?
    BOOL m_bSetFocusToFieldNameEditBox;  // TRUE if the marker is already defined; FALSE if a new marker
    BOOL m_bMarkerNameReadOnly;

    enum { eCharStyle = 0, eParStyle = 1 };

public:
    CMarkerGeneralPage(CMarker* pmkr, CMarkerSet* pmkrset, CFontDef& fntdef, CMarker*& pmkrOverThis,
					   CMarker*& pmkrFollowingThis, CLangEncPtr& plng, Str8& sMarker, Str8& sFieldName,
                       BOOL& bUseDefaultFont, BOOL& bCharStyle, Str8& sDescription, BOOL bSetFocusToMarkerEditBox,
					   BOOL bMarkerNameReadOnly);
 
    const CFont* pfnt();
	void GetDlgText(); // 1.4gx Fix bug (1.4dw) of losing marker description
	void ReportError(CNote* pnot);

// Implementation
private:
    void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CMarkerGeneralPage)
//	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnChooseFont();
    afx_msg void OnUseDefaultFont();
    afx_msg void OnLanguageProperties();
    afx_msg void OnSelchangeLangEncCombo();
    afx_msg void OnSetFocusMarker();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

//    BOOL OnSetActive(); // called when General page is switched to

}; // class CMarkerGeneralPage

// **************************************************************************

class CDataPropertiesPage : public CPropertyPage
{
private:
    BOOL& m_bMultipleItemData;
    BOOL& m_bMultipleWordItems;
    BOOL& m_bMustHaveData;

// Dialog Data
    //{{AFX_DATA(CDataPropertiesPage)
    enum { IDD = IDD_MKR_DataProperties_Page };
    int   m_iDataType;
    BOOL  m_bAllowEmptyContents;
    BOOL& m_bNoWordWrap;
    //}}AFX_DATA
    
    enum { eSingleWord = 0, eMultipleWords, eMultiwordItem };

public:
    CDataPropertiesPage(BOOL& bMultipleItemData, BOOL& bMultipleWordItems,
                        BOOL& bMustHaveData, BOOL& bNoWordWrap);

// Implementation
private:
    void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

}; // class CDataPropertiesPage

// **************************************************************************

class CMarkerPropertiesSheet : public CPropertySheet
{
private:
    CMarker* m_pmkr;  // the marker being viewed or added new
    CMarker** m_ppmkrNew;  // means by which to return new marker
    CMarkerSet* m_pmkrset;  // marker set which owns the marker
    const Str8 m_sOriginalMarker;  // original name of an existing marker (otherwise empty)
    CFontDef m_fntdef; // font definition for marker
    CMarker* m_pmkrOverThis;
    CMarker* m_pmkrFollowingThis;
    CLangEncPtr m_plng;
    Str8 m_sMarker; // new or potentially changed marker name
    Str8 m_sFieldName;
    BOOL m_bUseDefaultFont;
    BOOL m_bCharStyle;
    // Data for Range Set Page
    BOOL m_bUseRangeSet;
    CDataItemSet m_datset;
    // Data for Data Properties page
    Str8 m_sDescription;
    BOOL m_bMultipleItemData;
    BOOL m_bMultipleWordItems;
    BOOL m_bMustHaveData;
    BOOL m_bNoWordWrap;

	CMarkerGeneralPage m_dlgMarkerProperties;

    CMarkerRangeSetPage m_dlgRangeSet;

    CDataPropertiesPage m_dlgDataProperties;

public:
    enum EPage { pagGeneral=0, pagRangeSet, pagDataProp }; // Marker Data Properties pages

    CMarkerPropertiesSheet( CMarkerSet* pmkrset, CMarker** ppmkrNew, const char* pszMarker = "" );
        // A dialog box for adding a new marker to the set
    CMarkerPropertiesSheet( const CMarker* pmkrToCopyFrom, CMarker** ppmkrNew );
        // A dialog box for copying an existing marker to the set
        // by copying from an existing one
    CMarkerPropertiesSheet( CMarker* pmkr, EPage pagActive = pagGeneral,
                            BOOL bDisableMarkerChange = FALSE, BOOL bDisableRangeSetRemove = FALSE);
        // A dialog box for viewing and possibly modifying an existing marker

    void AddPages();

    virtual BOOL OnInitDialog();

	const CLangEnc* plng() const { return m_plng; }
	const CFont* pfnt() { return m_dlgMarkerProperties.pfnt(); }


protected:
    void SetActivePage(EPage pag);

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMarkerPropertiesSheet)
    afx_msg void OnOK();
    //}}AFX_VIRTUAL

    // Generated message map functions
    //{{AFX_MSG(CMarkerPropertiesSheet)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	void SetButtonLabels();  // 1998-09-30 MRP: For French version                     
};  // class CMarkerPropertiesSheet
