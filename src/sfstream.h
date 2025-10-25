// sfstream.h  SIL Standard Format iostream classes

// Read and write SIL Standard Format records and fields from an iostream,
// which can represent a Shoebox database or other Standard Format file.

#ifndef SFSTREAM_H
#define SFSTREAM_H

#include <iostream>
using namespace std;  // classes istream, ostream

class CMarkerSet;  // mkr.h
class CMarker;  // mkr.h
class CRecord;  // crecord.h
class CField;  // cfield.h
class CDatabaseType; 


// **************************************************************************

// Input Standard Format stream hides from the data destination such details
// as marker references for fields, and adding new markers to the marker set;
// character encoding changes; and other conversions required by File, Open
// of files other than native Windows Shoebox databases.

// One field can be "unread" and then read again.
// Also keeps note of the record marker for the file.


// 3/5/98 BDW
//idea discussed: Split SF_stream into two cooperating classes - 
// SF_Reader and SFDB_Reader:SF:Reader.
//SFDB_Reader would be similar to the existing SF_istream and is DB oriented.
//SF_Reader would have an interface such as 
// bReadSimpleField(const char** ppszMarker, Length* plenMarker,const char** ppszContents))
//which would use the same line buffers, but be useable for reading the
//SH header and first marked field (before the SFDB_Reader is created).
// it would also need to detect the lack of the \sh_ marker


class SF_istream  // Hungarian: sfs
{
private:
    std::istream& m_ios;  // Input source
    CMarkerSet* m_pmkrset;  // From the Database Type
    CMarker* m_pmkrRecord;
    const Str8& m_sRecordMarker;
    Length m_lenRecordMarker;
    BOOL m_bRemoveSpaces;  // Import option (esp. for Sh2 hanging indent)

    CField* m_pfldNext;  // First field of the next record
    char* m_pszFieldBuf;
    char* m_pszFieldEnd;  // Place to append the field's next line
    Length m_lenFieldRemaining;

    // Invariant: the stream has always read one line ahead until EOF   
    const Length m_lenInsertableMarker;  // "??"
    const Length m_maxlenInsertion;  // For inserting \?? and/or space
    char* m_pszNextLineBuf;
    char* m_pszNextLine;  // Beginning of the next line
    BOOL m_bNextLine;  // Have read the next input line into the buffer

    BOOL m_bSh1;  // Importing a Shoebox 1.x database
    BOOL m_bSkippedLines;  // Flag: Plain text lines or internal \_ fields skipped
    long m_lFieldsSkipped;  // Fields skipped Excluding internal \_ fields
    long m_lUsefulLinesSkipped; // Possibly useful lines skipped - plain text or marked, 
                                //    not part of a useable record or field  3/6/98

    BOOL m_bFieldTooLong;  // 07-26-1997 TRUE if a Field is longer as 32KBytes
	Str8 m_sSkippedFields; // 1.4pcg Add member var to store head info

public:
    SF_istream(std::istream& ios, CMarkerSet* pmkrset, CMarker* pmkrRecord,
            BOOL bSkipToFirstRecordMarker = TRUE,
            BOOL bRemoveSpaces = FALSE);
    ~SF_istream();
    
    BOOL bAtEnd() { return !m_pfldNext && !m_bNextLine; }
        // Clients are responsible not to attempt to read data
        // when the stream has no more input left.
    BOOL bReadRecord(CRecord** pprec);
    
    BOOL bReadField(CField** ppfld);
         // bReadField returns FALSE if a Field is longer as 32 KBytes;
    
    BOOL bSkipped() { return m_bSkippedLines || m_lFieldsSkipped != 0; }
        // Returns whether any file content (incl \_ fields) was skipped.
        // Skipping content is expected when importing,
        // but is significant when opening an Sh3 database file.
    long lFieldsSkipped() { return m_lFieldsSkipped; }
        // Returns how many fields were skipped (not including internal \_).
        // This is useful feedback when importing ordinary Standard Format:
        // if more than one (e.g. an \id field) it strongly suggests
        // that the user chose an inappropriate Database Type.
    long lUsefulLinesSkipped() { return m_lUsefulLinesSkipped; }
        // Returns how many "possibly useful" lines were skipped.
        // This is useful feedback when trying to import text that's 
        // not properly marked.
	Str8 sSkippedFields() { return m_sSkippedFields; } // 1.4pcg Add member var to store head info
    
private:
    void UnReadField(CField** ppfld);
        // Hold the first field of the next record until it's needed.
    
    BOOL ReadField(const char** ppszMarker, Length* plenMarker,
            const char** ppszContents);
        // ReadField returns FALSE if a Field is longer as 32 KBytes;
    void AppendNextLineToField();
    BOOL bReadNextLine();
    void GetNextLine();
    void SkipToNextRecordMarker();
    void SkipToNextMarker();
    // 07-30-1997
    // If a fieldline is typed with no wrapping, and it exceeds 4000
    // characters, a bug occures. CheckLineLength will wrap the line,
    // if this case happens.
    void CheckLineLength(const char* pszCont);
};  // class SF_istream


// ==========================================================================

// Output Standard Format stream hides from the data source such details
// as selection of fields to output; character encoding changes; and other
// file conversions required by File, Save As.

class Field_ostream
{
protected:
    std::ostream& m_ios;
    BOOL m_bTrimSurroundingWS;  // 1998-12-07 MRP
    
    Field_ostream(std::ostream& ios, BOOL bTrimSurroundingWS);
    
public:
    virtual ~Field_ostream();

    virtual void WriteField(const CField* pfld, BOOL bFirstField) = 0;
    virtual void WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst) {}; // 1.1cm Add alphabetic dividers
	virtual void WriteMultipleColumnEnd() {}; // 1.2du On rtf export double column, even up column ends on last page

    static void s_TrimmedFieldContents(const CField* pfld, const char** ppchBegin, const char** ppchEnd);
	virtual BOOL bXML() { return FALSE; } // 1.4qnk Add func to test whether this is XML export
	virtual void WriteString(const char* pszContents) // 1.4pcf Write string in XML, added for head extra info string
		{
		m_ios << pszContents; // 1.4kmb Write string with no extra markers or nl
		};

};  // class Field_ostream


// --------------------------------------------------------------------------

class SF_ostream : public Field_ostream
{
private:
    BOOL m_bHaveWrittenField;
    
public:
    SF_ostream(std::ostream& ios, BOOL bTrimSurroundingWS);
    virtual ~SF_ostream();

    virtual void WriteField(const CField* pfld, BOOL bFirstField);
    void WriteField(const char* pszMarker, const char* pszContents);
    void WriteField(const char* pszMarker, const char* pszBegin,
            const char* pszEnd);
    void WriteInternalField(const char* pszMarker, const char* pszContents);
//	void WriteString(const char* pszContents); // 1.4kmb Write string, added for head extra info string

    void WriteAlphDiv(const char* sAlphLet, CDatabaseType* ptyp, BOOL bFirst); // 1.1cm Add alphabetic dividers

#ifdef _DEBUG   
    void AssertValid() const {}
#endif  // _DEBUG
};

#endif  // SFSTREAM_H
