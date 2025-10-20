// CRecLookel.h CRecLookEl record lookup element class Alan Buseman 8 Feb 95

// This class is the list element for the CRecLookEl class. See CRecLook.h for more information.

#ifndef CRecLookEl_H
#define CRecLookEl_H

#include "cdblel.h" // For derivation
#include "crecord.h" // For CRecord
#include "srt.h" // For UIdMChar

class Field_ostream;  // sfstream.h

class CRecLookEl : public CDblListEl {	// Record lookup element, derived from lookup base
friend class CIndex;
private:
	CRecord* m_prec; // Record
	CField*  m_pfldPriKey; // Field containing the primary key component
    UIdMChar* m_puchSortKey;  // 1999-04-30 MRP: Replace m_sKey

public:
	CRecLookEl( const UIdMChar* puch, CRecord* prec, CField* pfldPriKey );
	~CRecLookEl(); // Destructor                                                                  

	void SetSortKey(const UIdMChar* puchKey);  // 1999-04-30 MRP
	const UIdMChar* puchSortKey() const { return m_puchSortKey; }
	CRecord* prec() const; // Get content
	CField* pfldPriKey() const { return m_pfldPriKey; }

	const CField* pfldFirstInSubRecord(const CMarker* pmkr) const;
		// Return the first occurrence of a field with pmkr in relation to
		// this record element's primary sort key field
		// with regard to the outline hierarchy of markers.
		// Returns NULL if the record contains no such field.
		// Simply returns the very first field with pmkr
		// if the record has no occurrence of the primary sort key field
		// or if the outline hierarchy is the default.

	// Used to preserve order stability of otherwise equal record elements	
	// when sorting an index.
	long lNum() const { return CDblListEl::lNum(); };
	void SetNum(long lNum) { CDblListEl::SetNum(lNum); }
	void ClearNum() { CDblListEl::ClearNum(); }
	
	void DeleteRecord(); // Delete content record
	
#ifdef _DEBUG
	void AssertValid() const;
#endif		
}; // class CRecLookEl

/* Discussion
Hungarian notation used in this file:
	m_ is Microsoft standard for member variable
	i is for integer
	p is for pointer
	el is for CRecLookEl

This class provides the following functions:
	Above:
		prec Returns a record pointer
	For debugging:	
		AssertValid
*/

#endif // CRecLookEl_H
