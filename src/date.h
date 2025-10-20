// date.h - Shoebox marker-date filter condition and datestamp

// 1998-04-09 MRP: Date sorting isn't fully integrated into Shoebox,
// but year-2000-compatible date.lng (along with date.cct)
// is provided for the user to associate with date field markers.

#ifndef DATE_H
#define DATE_H

class Object_ostream;  // obstream.h
class Object_istream;  // obstream.h

class CNote;  // not.h

// **************************************************************************

class CDateCon  // Hungarian: dat
{
private:
    int m_iRelation;  // e.g. Before; see enum below
    const char* m_pszRelation;  // For writing properties and displaying
    Str8 m_sSurface;  // e.g. 09/Apr/1998
    Str8 m_sUnderlying;  // e.g. 19980409
    BOOL m_bValidUnderlying;
        // The visual user interface for marker-date filter conditions
        // gurarantees valid format, but direct editing of the .typ file
        // could make it invalid when the properties are read.

    CDateCon(int iRelation, const Str8& sSurface, const char* pszUnderlying);
        // Used by s_bReadProperties and s_bConstruct.

    // Prevent use of the assignment operator by making it private and
    // omitting its definition.
    CDateCon& operator=(const CDateCon& dat);

public:
    CDateCon(const CDateCon& dat);
        // Used to copy filters that contain marker-date elements.

#ifdef prjWritefstream // 1.6.4aa 
    void WriteProperties(Object_ofstream& obs) const;
#else
    void WriteProperties(Object_ostream& obs) const;
#endif
    static BOOL s_bReadProperties(Object_istream& obs, CDateCon** ppdat);

    enum { eBefore = 0, eEqual = 1, eAfter = 2 };  // iRelation
        // NOTE: These enum values correspond to the radio buttons
        // in the Marker Date dialog box.

    int iRelation() const { return m_iRelation; }
    const char* pszRelation() const { return m_pszRelation; }
    const Str8& sDate() const { return m_sSurface; }

    BOOL bMatches(const Str8& sDateField) const;
        // Returns TRUE if sDateField is in the correct relation
        // to this date. Returns FALSE if this date wasn't valid or
        // if sDateField isn't. E.g. 01/Jan/2000 matches the date
        // condition "After 31/Dec/1999" but 09/Apr/1998 doesn't.

    static BOOL s_bConstruct(int iRelation, const Str8& sSurface,
            CDateCon** ppdat, CNote** ppnot);
        // Used by First, Insert, and Last in Filter Properties.
    BOOL bModifyProperties(int iRelation, const Str8& sSurface,
            CNote** ppnot);
        // Used by Modify in Filter Properties.

//  -------------------------------------------------------------------------
    
    static void s_DateStamp(Str8& sDateStamp);
        // Returns the contents of a datestamp field via sDateStamp:
        // the surface form of today's date, e.g. 09/Apr/1998.
        // NOTE: At the year 2000, datestamps in Sh2 will roll-over:
        // 01/Jan/00; in Sh3 they'll be mangled: 01/Jan/100@#$...
        // Sh4 extends the existing format with a four-digit year.

    static BOOL s_bConvertDateField(const Str8& sDateField,
            Str8& sConvertedDateField, BOOL* pbNotDate);
        // Returns FALSE if sDateField is already in Shoebox 4 format;
        // also if it doesn't look like a date at all (in that case,
        // *pbNotDate is set to TRUE). If it needs to be converted,
        // the result is returned via sConvertedDateField.
        // Preserves white space after the datestamp.

    enum { maxlenUnderlying = 8 };  // yyyymmdd, e.g. 19980409
    static BOOL s_bSurfaceToUnderlying(const char* pszSurface,
            size_t lenSurface, BOOL bCanBePartial, BOOL bCanBeMangled,
            char* pszUnderlying, BOOL* pbMinorVariation);
        // Returns whether or not [pszSurface, pszSurface+lenSurface)
        // is a valid surface form of a datestamp. If it is, four to
        // eight digits are returned as a null-terminated string
        // via pszUnderlying, whict the caller should define as
        // char pszUnderlying[CDateCon::maxlenUnderlying + 1];
        //
        // Examples of surface forms:
        // 09/Apr/98           Shoebox 1, 2, and 3
        //  9/apr/98           Minor variations are accepted
        // 09/Apr/1998         Shoebox 4
        // 01/Jan/00           Shoebox 2
        // 01/Jan/100@#$...    Shoebox 3 mangles it. Yuck!
        // 01/Jan/2000         Shoebox 4
        //    Jun/2001         Partial: month-year
        //        2001         Partial: year-alone

    static void s_UnderlyingToSurface(const char* pszUnderlying,
            Str8& sSurface);
        // Used to upgrade data fields that have two-digit years
        // (or minor variations like a one-digit day) to standard
        // Shoebox 4 format, which is returned via sSurface.
};  // class CDateCon

// **************************************************************************

#endif  // DATE_H
