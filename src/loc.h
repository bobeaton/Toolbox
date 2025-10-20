// loc.h  Interface for marked string locations (1995-04-19)

// A description of descriptive markup appears at the end of this file. 

// Change history:
// 1995-04-19 0.11  MRP: separate from rest of marker classes


#ifndef LOC_H
#define LOC_H


#include "mkr.h"


// **************************************************************************

// A markup element corresponds to an occurrence of a Standard Format marker
// within a marked string (i.e. SF field).

// There are three varieties of markup elements, corresponding to:
// 1. the marker of the entire string, e.g. "\mkr" in the example below
// 2. an embedded opening marker, e.g. "{ew}"
// 3. an embedded closing marker (sometimes left unidentified), e.g. "{-}"
// E.g. "\mkr Contains {ew}embedded{-} text."

// Notice a subtle distinction between _marker_ and _markup element_.
// When the focus of the discussion is the Standard Format text itself,
// we might say, "The example text has three markers".
// When discussing the classes, it is more precise to say that the structure
// of the text is represented by three _markup elements_ (class CMarkupEl);
// and that they refer to two _markers_ (class CMarker), "mkr" and "ew". 
// Also, in the code, a marker would not include the delimiting characters
// (backslash, curly braces, dash), wheras a markup element would.

class CMarkupElOpening;

class CMarkupEl : public CDblListEl  // Abstract base class. Hungarian: mel
{
public:
	virtual ~CMarkupEl() {}

	virtual BOOL bOpening() const = 0;
	// This element's marker, and the marker of text at locations after it
	virtual CMarker* pmkrEl() const = 0;	
	virtual CMarker* pmkrAtLocsAfter() const = 0;
		// E.g. "\mkr Contains {ew}embedded{-}^ text."
		// The marker of element "{-}" is "ew", but the descriptive marker
		// applying to text after it (^ and beyond) is "mkr".
		
	virtual CMarkupElOpening* pmelCurUnclosedOpening() const = 0;
};  // class CMarkupEl


class CMarkupElOpening : public CMarkupEl  // Hungarian: mel
{
private:
	CMarker* m_pmkr;
	CMarkupElOpening* m_pmelPrevUnclosedOpening;	
	
	friend class CMarkupList;
	CMarkupElOpening(CMarker* pmkr);
		// the marker of an entire string (i.e. SF field)
	CMarkupElOpening(CMarker* pmkr, CMarkupElOpening* pmelPrevUnclosedOpening);
		// an embedded opening marker
	
public:
	~CMarkupElOpening() {}

	BOOL bOpening() const { return TRUE; }
	CMarker* pmkrEl() const { return m_pmkr; }	
	CMarker* pmkrAtLocsAfter() const { return m_pmkr; }
	
	CMarkupElOpening* pmelCurUnclosedOpening() const
		{ return (CMarkupElOpening*)this; }  // cast away const-ness
	CMarkupElOpening* pmelPrevUnclosedOpening() const
		{ return m_pmelPrevUnclosedOpening; }
};  // class CMarkupElOpening


class CMarkupElClosing : public CMarkupEl  // Hungarian: mel
{
private:
	CMarkupElOpening* m_pmelOpening;	
	
	friend class CMarkupList;
	CMarkupElClosing(CMarkupElOpening* pmelOpening);
	
public:
	~CMarkupElClosing() {}

	BOOL bOpening() const { return FALSE; }
	CMarker* pmkrEl() const { return m_pmelOpening->pmkrEl(); }	
	CMarker* pmkrAtLocsAfter() const
		{ return pmelCurUnclosedOpening()->pmkrAtLocsAfter(); }
		
	CMarkupElOpening* pmelCurUnclosedOpening() const;
};  // class CMarkupElClosing


// --------------------------------------------------------------------------

// A markup list consists of elements in one-to-one correspondence with
// occurrences of markup elements within a marked string (i.e. SF field).
// It is an auxilliary representation of the substructure of a string
// used by location objects and defined as a by-product of their movement.

// For fields, we have chosen to separate the marker from the contents,
// and to represent it as an actual item of the object's member data.
// At least so far, are choosing _not_ to separate substrings enclosed
// by embedded markers from the string which logically contains them.
// Instead, we are providing for the CMString class a location "iterator"
// (CMStringLoc, see below), which encapsulates in its state and behavior
// the string's units of textual encoding and its descriptive structure.

// A location instance builds "on demand" a markup list corresponding to the
// contents of the string to which it points. The loc uses it to determine
// what is the current marker when the string can contain embedded markup.
// E.g. "\mkr Con^tains {ew}em^bedded{-} text^."
// The current marker at each of the three ^ locations: "mkr", "ew", "mkr".

// This strategy can be viewed as a pair of tradeoffs-- space vs. time,
// and declarative vs. procedural. Two consequences of these choices:
// 1. Detection of inconsistencies in embedded markup introduced by changing
//    a database outside Shw may be delayed until the SF field is "used".
// 2. The reference count for embedded markers will not be the total number
//    of marked substrings; rather those pointed to by location objects
//    (roughly speaking, those in the "current fields" of record views).

class CMarkupList : public CDblList  // Hungarian: mellst
{
private:
	CRefCount m_ref;  // number of marked string locations referring to this

public:
	CMarkupList(CMarker* pmkrMString);  // i.e. field marker
	~CMarkupList() {}  // destructor
	
	CMarkupEl* pmelFirst() const  // First: of entire string (i.e. SF field)
						{ return (CMarkupEl*)pelFirst(); }  // First mel
	CMarkupEl* pmelLast() const  // Last: seen so far when moving location
						{ return (CMarkupEl*)pelLast(); }
	
	CMarkupEl* pmelNext( CMarkupEl* pmelCur ) const	// Next after current
						{ return (CMarkupEl*)pelNext( pmelCur ); }
	CMarkupEl* pmelPrev( CMarkupEl* pmelCur ) const	// Previous before current
						{ return (CMarkupEl*)pelPrev( pmelCur ); }

	CMarkupEl* pmelNext_AddIfNew( CMarkupEl* pmelCur,
			CMarker* pmkrSubString, BOOL bOpening );
		// When moving location and matching an embedded marker,
		// add it if this is the farthest advance into the marked string
		// of any loc using this markup element list. 

	NumRefs numRefs() const { return m_ref.numRefs(); }
	BOOL bHasRefs() const { return m_ref.bHasRefs(); }	
	void IncrementNumRefs() { m_ref.IncrementNumRefs(); }
	void DecrementNumRefs() { m_ref.DecrementNumRefs(); }
};  // class CMarkupList


// ==========================================================================

// A marked string location is an augmented pointer into an SF field.
// The atomic _units_ of text to which it can point, by which it can move,
// and into which it segments a marked string, are the following:
// 1. multicharacter designator (in the current language encoding); or
// 2. single character (i.e. undefined in the current language encoding); or
// 3. embedded marker (if they are being interpreted).

// A substring can be delimited by a pair of locations [locB,locE),
// where locB is its beginning, and locE is _just after_ its end.
// (Most likely, one of B or E would have been copy constructed from the
// other, and then moved by an interface action or computing operation).

// A location's attributes include the current descriptive marker and
// language encoding in which the text is expressed.
// Whether to interpret embedded markers is an option.

typedef unsigned char BBOOL;  // Hungarian: b


class CMStringLoc  // Hungarian: loc
{
private:
	const char* m_psz;			// pointer into internal buffer of Str8
    const Str8* m_ps;		// the Str8 into which this points
	CMarker* m_pmkr;			// marker of text at this loc
	CLangEnc* m_plng;			// language encoding of text at this loc
	CMarkupList* m_pmellst;		// elements of descriptive markup in string
	CMarkupEl* m_pmelCur;  		// last markup element before this loc
#ifdef _DEBUG
	const char* m_pszBeginning;  // Str8's original internal buffer
#endif  // _DEBUG
	BBOOL m_bEmbeddedMarkup;			// interpreting embedded markers

public:
	CMStringLoc(const CMString* pmks, BOOL bEmbeddedMarkup = FALSE, int iPos = 0);
		// Set this location to the beginning of the marked string.
		// Choose whether to interpret markers embedded within the string.
		// (This instance constructs and owns its markup list.)
	CMStringLoc(const CMStringLoc& loc);  // copy constructor
		// (This instance refers to the same markup list being used by loc.)
	CMStringLoc(const CMStringLoc& loc, const Str8* ps);
		// Set this location to the beginning of an ordinary string ps.
		// and consider its marker to be the current marker of loc,
		// NOTE: Use for text pattern match strings, e.g. [Search|string],
		// in order to avoid constructing lots of temporary marked strings.
	~CMStringLoc();  // destructor
	
	void Relocate(const CMStringLoc* ploc);
		// Update this location from another copy (analogous to *this = *ploc)
		// NOTE: Actual assignment seemed not to have the proper semantics
		// for the owning/referencing associations with the markup list.


	// Marker and language encoding of the text at the current location.
	CMarker* pmkr() const { return m_pmkr; }  // 1995-04-26 MRP:  pmkrCur??? 
	CLangEnc* plng() const { return m_plng; }  // plngCur???

	// Return whether currently at certain kinds of locations.
	BOOL bAtBeginning() const { return (m_psz == *m_ps); }
	BOOL bNotAtBeginning() const { return (m_psz != *m_ps); }
	BOOL bAtEnd() const { return (*m_psz == '\0'); }
	BOOL bNotAtEnd() const { return (*m_psz != '\0'); }
	BOOL bAtMarkup() const
		{ return (m_bEmbeddedMarkup && (*m_psz == '{')); }
	BOOL bAfterMarkup() const
		{ return (m_bEmbeddedMarkup && (*(m_psz - 1) == '}')); }
		// Caller must first determine that loc is not at beginning of string.
	BOOL bAtValidLoc() const;  // Use after having moved by single chars
	BOOL bAtNewline() const { return (*m_psz == '\n'); }


	// Move this location within the text by one unit.
	// _Next: Return the unit beginning at this, the _current_ location,
	//        then move just after it to the beginning of the _next_ unit
	//        (like ch = *pch++).
	// _Prev: Move to the beginning of the _previous_ unit, and return it
	//        (like ch = *--pch).

	// Return whether the current/previous unit is an embedded markup element
		// (simply returns FALSE if this loc is not set to interpret them).
	BOOL bMarkup_Next();
	BOOL bMarkup_Next(CMarker** ppmkr, Length* plenMarkup);
	BOOL bMarkup_Prev();
	BOOL bMarkup_Prev(CMarker** ppmkr, Length* plenMarkup);
		// NOTE: The identity of the markup element is returned, but that is
		// _not_ always the current marker of the text after having moved.
		// E.g. "\mkr Contains {ew}embedded{-}^ text."
		// Next just returned "ew", but the current marker at ^ is now "mkr".
		// NOTE: Determine _first_ whether a text unit is an embedded marker;
		// if not, it is a multicharacter designator (possibly undefined).

	// Return the current/previous multichar (movement is as described above).	
		// All the following functions return the "undefined" multichar,
		// if no multichar in the current language encoding matches.
	const CMChar* pmch_Next();
	const CMChar* pmch_Prev();
	const CMChar* pmch_Next(Length* plenMChar);
	const CMChar* pmch_Prev(Length* plenMChar);
		// Also return length of multichar
	BOOL bMChar_Next(const CMChar** ppmch, Length* plenMChar);
	BOOL bMChar_Prev(const CMChar** ppmch, Length* plenMChar);
		// Also return whether multichar returned is defined. 
	
	BOOL bMChar_NextIncluded(const CMChar** ppmch, Length* plenMChar,
			MatchSetting matset);
		// Return whether there is a multichar included at the match setting
		// at or after this location. If there is, return the next such
		// and its length; _skipping over_ any (embedded markers or???)
		// non-included multichars previous to it,
	
	void Next();  // Move from the current text unit to the next. 
	void Prev();  // Move from the current text unit to the previous. 


	// Use these relations only on locs within the same marked string as this.	
	BOOL operator==(const CMStringLoc& loc) const;
	BOOL operator!=(const CMStringLoc& loc) const;
	BOOL operator<(const CMStringLoc& loc) const;
	int operator-(const CMStringLoc& loc) const;

	// Specialized operations, such as for implementing the <<...>> pattern;		
	// possibly also for movement of the editor's insertion point.
	void RelocateToEnd();  // i.e. at the string's terminating null char
	// NOTE: The following operations can move to an "invalid" location
		// (i.e. not at the beginning of a text unit; e.g. "{m^kr}"),
		// or return only a part of a text unit. See bAtValidLoc() above.
	void Char_Next() { (void) ch_Next(); }
	void Char_Prev() { (void) ch_Prev(); }
	char ch_Next();
	char ch_Prev();
	const char* psz() const { return m_psz; }
		// Pointer into the internal character buffer of the marked string,
		// corresponding to the current location.
	int iPos() const { return m_psz - (const char*)*m_ps; } // return current position within string
	
private:
	// Return whether an embedded marker is being pointed to.
	static BOOL s_bMarkup_Next(const char** ppsz,
			const char** ppszMarker, Length* plenMarker, BOOL *pbOpening);
	static BOOL s_bMarkup_Prev(const char* pszAtBeginning, const char** ppsz,
			const char** ppszMarker, Length* plenMarker, BOOL *pbOpening);

	// Maintain the correspondence between the markup list and the text.
	BOOL bMarkup(const char* pszMarker, Length lenMarker, BOOL bOpening,
			BOOL bMovingToNext, CMarker** ppmkr);
	CMarker* pmkrSearch_AddIfNew(const char* pszMarker, Length lenMarker);
	void RelocateToBeginning();  // used by MChar_Prev

	CMStringLoc& operator=(CMStringLoc& loc)  // assignment operator
		{ return loc; }  // prevent its use-- use Relocate() instead
	
public:
#ifdef _DEBUG
	void AssertValid() const;
#endif  // _DEBUG
};  // class CMStringLoc

// For examples of using marked string locations, see:
// 1. CMStringLoc::s_Test (loc.cpp);
// 2. CPatElMChars::bMatchAt (patmch.cpp).

// NOTE: Because units of text are variable length, operations on locs
// are relative instead of absolute (pmch_Next() is analogous to *pch++,
// but CMStringLoc do not have anything like pch[17]).

// NOTE: It has proved better not to have locations automatically skip over
// embedded markers, or multichars not included at a pattern match setting.


/****************************************************************************

                              DESCRIPTIVE MARKUP
                                                    
Markup is the means by which an author signals the structure of a text.
The descriptive markup approach does not specify format; rather it
describes the _function_ of each document element. The structure of the
document is coded in such a way that it is part of the content of the
document. A descriptively tagged document is, in fact, a database in which
queries on structure can be answered along with the more usual queries on
text content. Descriptive markup is a must for the field linguist.
Users of SIL software will recognize the SIL Standard Format convention
as a system of descriptive markup.

Gary F. Simons. Section 1.4.1, pp. 6-10, Laptop Publishing for the Field
Linguist. Priscilla M. Kew and Gary F. Simons (eds.). Dallas, Texas: Summer
Institute of Linguistics. 1989.

-----------------------------------------------------------------------------

              A DETAILED DESCRIPTION OF STANDARD FORMAT MARKERS

There are two types of SIL standard format markers:
(1) those which mark _text elements_ such as verses, paragraphs, or
    section heads; and
(2) those which mark _word elements_ within text elements.

                             Text element markers

A text element marker consists of backslash \ followed by identifying
characters, typically just one or two alphabetic characters, although
most SIL programs are designed to work with at least four characters.
The characters identify the nature or function of the text element
they mark.

Incidentally, if your text is a list of records (such as a dictionary,
address list, etc.), there are a few other things to note.
A _record_ is a collection of text elements (called 'fields', if you are
familiar with database terminology) that all belong together. Each record
has a particular text element by which it is distinguished from the other
records or by which it is 'looked up'. The marker for this text element
is not only a _text element marker_, but it does double duty and acts as
a _record marker_ as well.
For such texts, it is _extremely_ important that there be *at least one
and only one* record marker for each record.

Every text element in a document should be marked. The marker should be
typed as the first item in the element of text and separated from the
actual text by a space. Each marker should be preceded by a carriage
return, i.e., each text marker appears at the left margin.

As the text is processed, the computer recognizes the backslash as a
boundary marker, or delimiter, for the text elements. That is, every
backslash marks the beginning of another text element.

In each case, the characters associated with the backslash identify the
text element with others of the same kind and differentiate is from
those which are different. The characters are usually chosen to help
you remember the type of element marked, e.g., \p for paragraph or \v
for verse.

                             Word element markers

_Word element markers_ are used to mark contrasting or emphasized words
within text elements. The initial marker consists of a vertical bar |,
one or two characters to identify the type of element, and the opening
curly brace, e.g., |xx{. The final marker consists of the vertical bar
and the closing curly brace, e.g., |}. Note that the identifying
characters are not repeated in the final marker.

Section 4, pp. 3-5, "Standard Format Markers User's Guide", Direct
Translator Support. Waxhaw, NC: JAARS, Inc. 1990.

-----------------------------------------------------------------------------

                        STANDARD FORMAT MARKERS (SFM)

Standard Format is a marking system used within SIL. By placing certain
markers in the data, the same data can be used for different purposes and by
different programs. Probably the two most important uses of Standard Format
are with Scripture and Dictionaries.
                        
The principle behind SFM is that there is a difference between format and
content. Content is the text itself. Format is the way it is displayed. In
Standard Format, we mark according to content and not how it will be
formatted.

A standard format marker is usually the backslash (\) followed by one or two
characters. Most programs can handle up to four characters. A space always
follows the marker. Standard format markers are generally at the left margin.

                                  Scripture

Standard format is required for Scripture. In standard format, all the
Scripture text elements are marked. Because they are marked, and the text
is ASCII, we can also do a lot more with it.

One more advantage is that it is a standard way of archiving texts. Who knows
what program will be used five years from now for the revision of the New
Testament. Special character standards are not defined, except that the
method of choice for archiving is to use 7-bit ASCII and multiple characters
for designating special characters.

                                 Dictionaries

Dictionaries are one of the major projects that a linguist does during a
language project. The linguist builds a dictionary over a period of many
years, and includes in the dictionary file a lot of data about the language.
The data is stored with standard format markers for each field of data.
Because each field is marked, certain fields in the vernacular language can
be printed in a different script if desired.

                                   Key file
                                   
It is very important to include with a file you give to someone else a KEY
file that lists the markers you are using and the special character
designators and their meaning.

Since the markers generally used are in abbreviated form, it is necessary to
put a brief but complete dscription of *all* format markers and *all* special
characters used in your data in a small file on each diskette. This is called
a Key file.

All files should contain the \id marker. The "id" stands for "identification".
It is to help those processing your data to maintain file management of the
diskette. It should contain an abbreviation of the book name, the name of the
file, your name, the date of the latest edit, the name of the language, and
the name of the diskette it is on --whatever is necessary for identification.

For Scripture:
\id a 3-letter book abbreviation, file name, date, owner, language and dialect

For dictionaries:
\id the file name, date, owner, language and dialect

*Most SIL Branches have set standards for coding Scripture.*

                                BE CONSISTENT

The single most important rule for standard format is to _be consistent._
You can use about any codes you want as long as you are consistent. Scripture
codes are required, but other codes are optional and may well vary in
different branches and among different individuals.

The marker does not indicate what will be done with the data, only that the
data is available to receive special treatment. The markers are to be
flexible for their needs, but should be *unique, consistent, well-defined,
and sufficient for the data.*

*Unique* means that there should be no ambiguity throughout a particular
type of text. Always use the same marker to mean the same thing throughout
a particular type of text. A marker may mean one thing in Scripture and
something else in a dictionary without ambiguity. *Consistent* means that
the same marker whould always be used for the same thing. *Well-defined*
means that the meaning is clear. This is facilitated by a KEY file.

Lesson 4, pp. 25-31. Learning to Use ED, Manuscripter, and Standard Format
Markers.

****************************************************************************/

#endif  // LOC_H
