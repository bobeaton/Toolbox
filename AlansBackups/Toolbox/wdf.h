// wdf.h  Word formulas for selecting valid parses automatically

#ifndef WDF_H
#define WDF_H

#include "set.h"  // CDblListEl, CDblList, CSetEl, CSet

class Object_istream;  // obstream.h
class Object_ostream;  // obstream.h

// --------------------------------------------------------------------------

class CWordFormulaSet;

class CWordPattern : public CDblListEl  // Hungarian: wdp
{
private:
    Str8 m_sPattern;
    int m_nDataItems;

    friend class CWordPatternList;
    CWordPattern(const Str8& sPattern);
    CWordPattern(const CWordPattern& wdp);  // copy constructor

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CWordPattern** ppwdp);

public:
    const Str8& sPattern() const { return m_sPattern; }

    BOOL bMatchParse(CWordFormulaSet* pwdfset,
            const char* pszParse, int nItems, const char** ppszUnmatched);

    BOOL bNextItem(const char** ppsz,
            const char** ppszItem, int* plenItem, BOOL* pbOptional);

private:
    void CountDataItems(CWordFormulaSet* pwdfset);
};  // class CWordPattern

// --------------------------------------------------------------------------

class CWordPatternList : public CDblList  // Hungarian: wdplst
{
public:
    CWordPatternList();
    CWordPatternList(const CWordPatternList& wdplst);  // copy constructor

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    BOOL bReadProperties(Object_istream& obs);

    CWordPattern* pwdpFirst() const
        { return (CWordPattern*)pelFirst(); }
    CWordPattern* pwdpNext(const CWordPattern* pwdpCur) const
        { return (CWordPattern*)pelNext(pwdpCur); }

    Str8 sPatterns() const;
    void AddWordPatterns(const Str8& sPatterns);
    void ReplaceWordPatterns(const Str8& sPatterns);

    void CountDataItems(CWordFormulaSet* pwdfset);
};  // class CWordPatternList

// --------------------------------------------------------------------------

class CWordFormula : public CSetEl  // Hungarian: wdf
{
private:
    CWordPatternList m_wdplst;

    BOOL m_bUsedInPatterns;

    BOOL m_bReplaced;
    int m_nItemsWhenReplaced;
    BOOL m_bEndlessRecursion;

    friend class CWordFormulaSet;
    CWordFormulaSet* m_pwdfsetMyOwner;

    CWordFormula(const Str8& sSymbol, CWordFormulaSet* pwdfsetMyOwner);
    CWordFormula(const CWordFormula& wdf, CWordFormulaSet* pwdfsetMyOwner);
    void Initialize();

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
        CWordFormulaSet* pwdfsetMyOwner, CWordFormula** ppwdf);

public:
    CWordFormulaSet* pwdfsetMyOwner() const { return m_pwdfsetMyOwner; }
    virtual CSet* psetMyOwner();

    BOOL bMatchParse(const char* pszParse, int nItems, const char** ppszUnmatched);

    BOOL bPrimary() const;
    CWordPattern* pwdpFirst() { return m_wdplst.pwdpFirst(); }
    CWordPattern* pwdpNext(const CWordPattern* pwdpCur) { return m_wdplst.pwdpNext(pwdpCur); }
    Str8 sPatterns() const { return m_wdplst.sPatterns(); }

    BOOL bUsedInPatterns() const { return m_bUsedInPatterns; }
    void SetUsedInPatterns() { m_bUsedInPatterns = TRUE; }

    virtual BOOL bCopy(CSetEl** ppselNew);
    virtual BOOL bModify();
    BOOL bModifyProperties(const Str8& sSymbol, const Str8& sPatterns);
    virtual BOOL bDeletable();

private:
    void AddWordPatterns(const Str8& sPatterns);
    void CountDataItems();
};  // class CWordFormula

// --------------------------------------------------------------------------

class CLookupProc;  // interlin.h
class CIndexSet;  // ind.h
class CNote;  // not.h
class CLangEnc;  // lng.h
class CFieldList;  // rec.h

class CWordFormulaSet : public CSet  // Hungarian: wdfset
{
private:
    BOOL m_bEnabled;
    CWordFormula* m_pwdfPrimary;
    CWordFormula* m_pwdfSelected;
    Str8 m_sDefaultPrimarySymbol;
    BOOL m_bShowExpandedList;
    Str8 m_sLangEncPatterns;
    CLookupProc* m_pintprcMyOwner;

    CWordFormulaSet(CLookupProc* pintprcMyOwner);

public:
    CWordFormulaSet(const CWordFormulaSet& wdfset);  // copy constructor
    void operator=(const CWordFormulaSet& wdfset);

#ifdef typWritefstream // 1.6.4ac
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs,
            CLookupProc* pintprc, CWordFormulaSet** ppwdfset);
    static BOOL s_bConvertOlderWordFormulas(const CIndexSet* pindset,
            CLookupProc* pintprc, CWordFormulaSet** ppwdfset);

    void MakeValidSymbol(Str8& sSymbol);

    const Str8& sPrimarySymbol() const;
    CWordFormula* pwdfPrimary();
    void SetPrimaryWordFormula(CWordFormula* pwdf);

    CWordFormula* pwdfSelected();
    void SelectedWordFormula(CWordFormula* pwdf);

    CLangEnc* plngPatterns() const;
    void LangEncPatterns(CLangEnc* plng);

    BOOL bShowExpandedList() const { return m_bShowExpandedList; }
    void ShowExpandedList(BOOL b) { m_bShowExpandedList = b; }

    BOOL bEnabled() const { return m_bEnabled; }
    void MatchParses(CFieldList* pfldlst, CFieldList* pfldlstExcluded,
            BOOL bIncludeInvalidParses);
    BOOL bMatchParse(const Str8& sParse);
    BOOL bMatchParseItem(const char* pszItem, int lenItem, BOOL bOptional,
            const char* pszParse, int nItems, const char** ppszUnmatched);

    CWordFormula* pwdfFirst() const
        { return (CWordFormula*)pelFirst(); }
    CWordFormula* pwdfNext(const CWordFormula* pwdfCur) const
        { return (CWordFormula*)pelNext(pwdfCur); }
    CWordFormula* pwdfSearch(const char* pszSymbol, int lenSymbol) const;

    void CountDataItems();
    virtual void Delete(CSetEl** ppsel);

    static void s_EnableWordFormulas(CLookupProc* pintprc,
            CWordFormulaSet** ppwdfset);
    void EnableWordFormulas(BOOL bEnabled) { m_bEnabled = bEnabled; }
    BOOL bModify();
    virtual BOOL bAdd(CSetEl** ppselNew);
    BOOL bAddWordFormula(const Str8& sSymbol, const Str8& sPatterns,
            CWordFormula** ppwdfNew);
    void Clear();
    BOOL bValidSymbol(const Str8& sSymbol, Str8& sMessage);

private:
    void CopyFrom(const CWordFormulaSet& wdfset);
    void ReadProperties(Object_istream& obs);
    void AddDefaultPrimaryWordFormula();
    BOOL bConvertOlderWordFormulas(const CIndexSet* pindset);
    BOOL bConvertOlderWordFormula(const Str8& sFormula, CWordFormula** ppwdf);
};  // class CWordFormulaSet

#endif  // WDF_H
