// printinf.h
// 1996-05-31
// Rod Early

#ifndef _printinf_h
#define _printinf_h

#include "ind.h"
#include "crecpos.h"

class CPrintPosition : public CDblListEl    // Hungarian ptp
{
    friend class CPrintPosList;
private:
    CRecPos m_rps;  // The position in the record as we are printing.
    CNumberedRecElPtr m_prel;

    CPrintPosition( const CNumberedRecElPtr& prel, const CRecPos& rps )
        { m_rps = rps; m_prel = prel; }

public:
    // CPrintPosition( const CPrintPosition& ptp );  // copy ctr

    ~CPrintPosition() {}  // destructor

    CNumberedRecElPtr& prel() { return m_prel; }
    CRecPos& rps()  { return m_rps; }
    
};


class CPrintPosList : public CDblList // fldlst List of fields
{
public:
    CPrintPosList() {}
    ~CPrintPosList() {}
    
    CPrintPosition* pptpFirst() const   // first (key) field
                        { return (CPrintPosition*) pelFirst(); }
    CPrintPosition* pptpLast() const    // last field
                        { return (CPrintPosition*) pelLast(); }
    
    CPrintPosition* pptpNext( const CPrintPosition* pptpCur ) const // field after current
        { return (CPrintPosition*) pelNext( pptpCur ); }
    CPrintPosition* pptpPrev( const CPrintPosition* pptpCur ) const // field before current
        { return (CPrintPosition*) pelPrev( pptpCur ); }

    // Add a new print position to the end of the list, also returning
    // a pointer to the newly created position element.
    void Add(const CNumberedRecElPtr& prel, const CRecPos& rps );   // Insert new at end

#ifdef _DEBUG
    void AssertValid() const {};
#endif  // _DEBUG
};



    enum HeaderDatatype
        {
        eFileName,
        ePageNumber,
        eRecordNumber,
        eRecordsTotal,
        eDate,
        eTime,
        eFilterName,
        eRecordMarker,  // Note -- backslashes NOT INCLUDED.
        eRecordField,
        eKeyMarker,
        eKeyField,
        eText
        };

class CHeaderInfo : public CDblListEl   // Hungarian hdr
{
    friend class CHeaderInfoList;
public:
private:

    HeaderDatatype  m_eType;
    const CFont*            m_pfnt;     // The font for this segment.
    // CSize        m_sizDim;       // The width and height of this segment.
    Str8         m_sTxt;     // The text of this segment

    CHeaderInfo( HeaderDatatype eType, const CFont* pfnt, const char* pszTxt = NULL )
        { m_eType = eType; m_pfnt = pfnt;
          m_sTxt = (pszTxt != NULL) ? pszTxt : ""; }

public:

    ~CHeaderInfo() {}  // destructor

    Str8 sContents() { return m_sTxt; }
    void SetContents(const char* pszTxt)
        { m_sTxt = (pszTxt != NULL) ? pszTxt : ""; }

    CSize sizGetDimensions(CDC* pDC);
    // Get the dimensions for this header.

    void Draw(CDC* pDC, CRect& rect);
    
};


class CPrintProperties; // Forward ref


class CHeaderInfoList : public CDblList // hdrlst List of fields
{

int m_iSpaceBetweenHeaderItems;

public:
    CHeaderInfoList()
        {
        m_iSpaceBetweenHeaderItems= s_SpaceBetweenHeaderItemsDefault;
        }
    ~CHeaderInfoList() {}

public: 
    static const int s_SpaceBetweenHeaderItemsDefault;

    CHeaderInfo* phdrFirst() const  // first (key) field
                        { return (CHeaderInfo*) pelFirst(); }
    CHeaderInfo* phdrLast() const   // last field
                        { return (CHeaderInfo*) pelLast(); }
    
    CHeaderInfo* phdrNext( const CHeaderInfo* phdrCur ) const
        { return (CHeaderInfo*) pelNext( phdrCur ); }
    CHeaderInfo* phdrPrev( const CHeaderInfo* phdrCur ) const
        { return (CHeaderInfo*) pelPrev( phdrCur ); }
        
    void Add( HeaderDatatype eType, const CFont* pfnt, const char* pszTxt = NULL ); // Insert new at end
    BOOL bParse(CPrintProperties* pptp, const CShwView* pView, const char* pszTxt, BOOL bParseOnly = FALSE);

    void LoadData(CPrintInfo* pInfo, const CShwView* pView);

    void Draw(CDC* pDC, CPrintInfo* pInfo, const CShwView* pView, CRect &rectBounding, BOOL bBottomAlign = FALSE);

    int iLineHt(CDC* pDC);

private:
    CSize sizMaxDimensions(CDC* pDC);

public:

#ifdef _DEBUG
    void AssertValid() const {};
#endif  // _DEBUG
};


class CPrintProperties      // Hungarian prtp
{
public:  // default values, in 1/100'ths of an inch
    static const int s_MargLeft_Default;
    static const int s_MargRight_Default;
    static const int s_MargTop_Default;
    static const int s_MargBottom_Default;

    static const int s_SpaceBetweenRecords_Default;
    // This is the space between records if there are no page breaks
    // between the records.

private:
    Str8 m_sMargTop;
    Str8 m_sMargBottom;
    Str8 m_sMargLeft;
    Str8 m_sMargRight;

    int   m_iSpaceBetweenRecs;

    BOOL  m_bPrintAllRecords;
    BOOL  m_bBreakBetweenRecords;

    BOOL  m_bEndOfBrowse;
    // Set when we've displayed the last/final record in a browse.

    CRecPos m_rps;  // The position in the record as we are printing.
    CNumberedRecElPtr m_prel; // The corresponding numbered rec el.

    CPrintPosList m_ptplst;
    // This is a list of pointers to the top of pages (CPrintPosition objects.)

    CHeaderInfoList m_hdrlstHeader,
                    m_hdrlstFooter;

    Str8 m_sHeader,
            m_sFooter;

public:
    CPrintProperties();

    ~CPrintProperties() { ClearRecPos(); }

    CRect GetTruePageMargin(CDC* pDC);
    // Gets the true page margins -- that is, it takes into account
    // both the user margin and the unprintable "margin" around the edges
    // of the page.  (This varies among printers.)

    // These return the margins in units of 1/100th inch.
    CRect GetMargins();
    void SetMargins(CRect& rectMargins);
    void SetMargins(int iLeft, int iTop, int iRight, int iBottom)
        { SetMargins( CRect(iLeft, iTop, iRight, iBottom) ); }

    Str8 sTopMargin()    { return m_sMargTop;    }
    Str8 sLeftMargin()   { return m_sMargLeft;   }
    Str8 sBottomMargin() { return m_sMargBottom; }
    Str8 sRightMargin()  { return m_sMargRight;  }

    void SetTopMargin(const char* psz)  { m_sMargTop = psz;    }
    void SetLeftMargin(const char* psz) { m_sMargLeft = psz;   }
    void SetBottomMargin(const char* psz)   { m_sMargBottom = psz; }
    void SetRightMargin(const char* psz)    { m_sMargRight = psz;  }

    BOOL bValidateMeasure(Str8 &sVal);
    // This checks to see that the string has "in" or "cm".
    // If no unit of measure, it assumes "in" and attaches "in" to the end of the string
    // The presence of alphabetic characters elsewhere is invalid and will return FALSE.

    BOOL bMeasureInches(const char* psz);
    // Returns TRUE if this is in inches.

    BOOL bMeasureCm(const char* psz);
    // Returns TRUE if this is in centimeters.

    int iLoEnglish(const char* psz);
    // Return the value as 1/100ths of inch.  (Converts metrics)
    // Returns zero on failure.

    void SetSpaceBetweenRecords(int iVertSpace)
        { m_iSpaceBetweenRecs = iVertSpace; }
    // This is the vertical space between records in the mode where
    // we're printing multiple records on a single page.

    int iSpaceBetweenRecords() { return m_iSpaceBetweenRecs; }
    void SaveCurrentPositionAtEnd();
    // Called to add the present position to the end of the pages list.

    void ClearRecPos();

    void PrintAllRecords()      { m_bPrintAllRecords = TRUE; }
    void PrintCurrentRecord()   { m_bPrintAllRecords = FALSE; }
    BOOL bPrintingAllRecords()  { return m_bPrintAllRecords; }
    // Printing all records in the view/index versus just the current record.

    BOOL bBreakBetweenRecords() { return m_bBreakBetweenRecords; }
    void SetBreakBetweenRecords(BOOL bBreak) { m_bBreakBetweenRecords = bBreak;}
    // For having a page break between each record or having space instead.

    BOOL  bEndOfBrowse()            { return m_bEndOfBrowse; }
    void  SetEndOfBrowse(BOOL bEnd) { m_bEndOfBrowse = bEnd; }

    Str8 sHeader()   { return m_sHeader; }
    void SetHeader(const char* pszHeader)   { m_sHeader = pszHeader; }

    Str8 sFooter()   { return m_sFooter; }
    void SetFooter(const char* pszFooter)   { m_sFooter = pszFooter; }

    CHeaderInfoList* phdrlstHeader()
        { return &m_hdrlstHeader; }

    CHeaderInfoList* phdrlstFooter()
        { return &m_hdrlstFooter; }

    void Parse(const CShwView* pView)
        {
        (void) m_hdrlstHeader.bParse(this, pView, m_sHeader);
        (void) m_hdrlstFooter.bParse(this, pView, m_sFooter);
        }

    void EmptyHeaders()
        {
        m_hdrlstHeader.DeleteAll();
        m_hdrlstFooter.DeleteAll();
        }

    CNumberedRecElPtr& prel() { return m_prel; }
    CRecPos& rps()  { return m_rps; }

    CPrintPosList* pptplst() { return &m_ptplst; }

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);
};

#endif // _printinf_h


