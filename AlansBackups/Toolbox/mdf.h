// mdf.h  Multi-Dictionary Formatter

#ifndef MDF_H
#define MDF_H

#include "export.h"  // CExportProcess

class CIndexSet;  // ind.h
class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h
class CLangEnc;  // lng.h
class CMarker;  // mkr.h
class CFilter;  // fil.h
class CFilterUpdate;  // fil.h
class RtfPageSetup;  // rtf.h
class SF_ostream;  // sfstream.h


// --------------------------------------------------------------------------

// 1998-12-02 MRP: Web page format isn't implemented in the 16-bit version,
// mainly because there would be problems with long file names and
// the four-letter .html file name extension.
// However, all versions must read and write the properties correctly.

#define EXPORT_WEB_PAGES

class CExportProcessMDF : public CExportProcess  // Hungarian: exp
{
private:
    BOOL m_bWebPages;  // 1998-09-16 MRP
    BOOL m_bHTML_XML;  // 1998-12-05 MRP
    BOOL m_bUTF8;  // 1999-02-16 MRP: UCS Transformation Form, 8-bit form
    Str8 m_sCopyright;  // 1998-12-05 MRP
public:
    Str8 m_sChainProcess; // 1.4kk Process to chain to
private:

	Str8 m_sUnrecognizedSettingsInfo; // 1.0cp Don't lose settings info from later versions
    
    int m_iAudience;  // English = 0, National = 1
    // 1999-01-25 MRP: Finderlist language is independent of audience
    // EnglishGlossIndex = 2, NationalGlossIndex = 3, RegionalGlossIndex = 4
    int m_iFormat;  // Diglot = 0, Triglot = 1, Finderlist = 2
    Str8 m_aasTitle[2][5];
    Str8 m_aasExportedRtf[2][5];
    Str8 m_asExportedHtm[2];  // 1998-10-16 MRP

    Str8 m_sEnglishLabelsCct;
    Str8 m_sEnglishDot;
    Str8 m_sNationalLabelsCct;
    Str8 m_sNationalDot;

    CFilter* m_pfil;

    BOOL m_bSortByCitationForm;  // Dictionary
    BOOL m_bIncludeLexemeWithCitationForm;
    BOOL m_bIncludeExamples;
    BOOL m_bIncludeNotes;
    BOOL m_bIncludeRegionalWithNational;
    BOOL m_bIncludeNonMDF;
    BOOL m_bIncludePartOfSpeech;  // Reversed index (finderlist)
    BOOL m_bDoubleColumn;  // Page format
    BOOL m_bHeader;
    BOOL m_bDate;
    BOOL m_bTotal;
    
    Str8 m_sDatabaseTypeRTF;
    Str8 m_sDatabaseTypeHTML;  // 1998-10-16 MRP
    
    RtfPageSetup* m_ppgs;

    // 1999-03-10 MRP: Let customized versions of the change tables
    // have different file names, so that Setup won't overwrite them
    // when the user upgrades to a new version of Shoebox and MDF.
    Str8 m_sFileNameDict1;
    Str8 m_sFileNameDict2;
    Str8 m_sFileNameDictH;
    Str8 m_sFileNameLinkH;
    Str8 m_sFileNameList1;
    Str8 m_sFileNameList2;
    Str8 m_sFileNameListH;

    // 1998-12-03 MRP: User-defined index
    BOOL m_bUserDefined;
    CMarkerPtr m_pmkrIndexSelect;
    Str8 m_sOption;
    Str8 m_sFileNameIndex1;
    CMarkerPtr m_pmkrIndexPriKey;
    CMarkerRefList m_mrflstIndexSecKeys;
    BOOL m_bIndexingCategories;
    Str8 m_sFileNameIndex2;
    Str8 m_sFileNameIndexH;  // 1999-03-10 MRP
        
public:
    CExportProcessMDF(const char* pszName, CExportProcessSet* pexpsetMyOwner,
            BOOL bInitDefaultProperties,
            BOOL bWebPages = FALSE);  // 1998-10-16 MRP
    CExportProcessMDF(const CExportProcessMDF& exp);
    ~CExportProcessMDF();

    static const char* s_pszVersion;  // 1999-03-02 MRP

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
            CExportProcessSet* pexpsetMyOwner, CExportProcess** ppexp);
    static BOOL s_bReadOldProperties(Object_istream& obs,
            CExportProcessSet* pexpsetMyOwner, CExportProcessMDF** ppexp);

    const char* pszType() const;
    BOOL bMDF(CFilter** ppfil) const;
    BOOL bCanAutoOpen() const;

    BOOL bExport(CIndex* pindCur, const CRecLookEl* prelCur,
            const Str8& sPathDatabase, Str8& sMessage);

	BOOL bWebPages() const { return m_bWebPages; }

    void FilterUpdated(CFilterUpdate& fup);

    friend class CMDFDlg;
    BOOL bCopy(CSetEl** ppselNew);
    BOOL bModify();

    Str8 sHierarchyMarkers() const;
        // 1998-04-16 MRP: Generalize the "alternate" hierarchy.

    BOOL bCustomizedChangeTables() const;
        // 1999-03-10 MRP: Has the user customized any of the basic
        // change tables? Doesn't include the change tables for labels.

    Str8 sSettingsFolder() const;  // 1999-03-10 MRP

private:
    void InitDefaultProperties();
    void InitCopyright();  // 1999-01-25 MRP
    void InitOldTitles();  // 1999-01-26 MRP
    void InitNewTitles();  // 1999-01-26 MRP
    void InitExcludedFields();
    void ReadProperties(Object_istream& obs);
    void ReadOldProperties(Object_istream& obs);
    void ConvertOlderProperties();  // 1999-01-25 MRP

    const char* pszFormat(int iFormat) const;
    
    BOOL bNational() const { return m_iAudience == 1; }
    BOOL bTriglot() const { return m_iFormat == 1; }
    // 1999-01-25 MRP: Gloss index (finderlist) language is
    // independent of audience language.
    BOOL bFinderlist() const { return 2 <= m_iFormat; }
    BOOL bEnglishGlossIndex() const { return m_iFormat == 2; }
    BOOL bNationalGlossIndex() const { return m_iFormat == 3; }
    BOOL bRegionalGlossIndex() const { return m_iFormat == 4; }

    void WriteHierarchy(SF_ostream& sfs) const;
        // 1998-04-15 MRP: Let change tables generalize their hierarchy logic.

    // 1999-03-10 MRP: Customized versions of the change tables.
    Str8 sChangeTableFolder() const;
    Str8 sChangeTablePath(const Str8& sFileName) const;
    Str8 sFileNameChangeTable1() const;
    Str8 sFileNameChangeTable2() const;
    Str8 sFileNameChangeTableLinkH() const;
    Str8 sFileNameChangeTableLabels() const;

    BOOL bExportedFileDlg(const Str8& sDatabasePath);
    Str8 sExportedFilePath() const;
    Str8 sDocumentPath(const Str8& sDatabasePath) const;
    void SetDocumentPath(const Str8& sDocumentPath);

    BOOL bSelectSortFormat(CIndexSet* pindset,
            const Str8& sPathIn, Str8& sMessage);
    
    BOOL bSelect(CIndexSet* pindset, const Str8& sPathIn,
            const Str8& sTempPath,
            const Str8& sProjectPath, Str8& sMessage) const;
    BOOL bSortAndFormatRTF(const Str8& sTempPath, CDatabaseType* ptyp,
            const Str8& sOutputPath,
            const Str8& sProjectPath, Str8& sMessage) const;
#ifdef EXPORT_WEB_PAGES
    BOOL bSortAndFormatHTML(const Str8& sTempPath, CDatabaseType* ptyp,
            const Str8& sOutputPath,
            const Str8& sProjectPath, Str8& sMessage) const;
    BOOL bFormatHTML(const Str8& sTempPath, CDatabaseType* ptyp,
            const Str8& sOutputPath,
            const Str8& sProjectPath, Str8& sMessage) const;
        // 1999-03-02 MRP: Use MDFLinkH.cct to convert |fvlink{...}
        // added by MDFDictH.cct to the |{A ...}...|{/A} notation
        // expected by the SF-to-HTML conversion step.
#endif
    BOOL bFormat(const Str8& sTempPath, CDatabaseType* ptyp,
            const Str8& sOutputPath,
            const Str8& sProjectPath, Str8& sMessage) const;
};  // class CExportProcessMDF

#endif  // MDF_H
