// pgs_d.h  Page Setup dialog box for export as Rich Text Format

#ifndef PGS_D_H
#define PGS_D_H


class RtfPageSetup;  // pgs.h

class CRtfPageSetupSheet : public CPropertySheet  // Hungarian: dlg
{
private:
    RtfPageSetup* m_ppgs;
    BOOL m_bCentimeters;
    CDocumentPage* m_pdlgDocument;
    CSectionPage* m_pdlgSection;

public:
    CRtfPageSetupSheet(RtfPageSetup* ppgs);
    ~CRtfPageSetupSheet();

    RtfPageSetup* ppgs() { return m_ppgs; }
    void OK();

    BOOL bCentimeters() const { return m_bCentimeters; }
    void ChangePaperSize(int iPaperSize);
    void DDX_RtfUnits(CDataExchange* pDX, int idc, RtfUnits& un);
    static void s_ConvertMeasurement(RtfUnits& un, BOOL bToCentimeters);

protected:
    virtual BOOL OnInitDialog();

    // Generated message map functions
    //{{AFX_MSG(CRtfPageSetupSheet)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
	void SetButtonLabels();  // 1999-02-01 BW: For French version

};  // class CRtfPageSetupSheet

#endif  // not PGS_D_H
