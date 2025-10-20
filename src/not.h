// not.h  Interface for notes describing problems  (1995-05-08)

// When the program has a problem doing a task for the user, it provides in
// a note information that describes the problem and helps the user solve it.
// Thus mistakes and slips become a minor annoyance instead of a major
// problem for the user. There is rarely an excuse for not providing
// coping information. [Heckel, pp. 57-58; Lewis and Norman, p. 414]
// I am intentionally avoiding the term "error message".

// Notes on detecting and describing problems appear at the end of this file. 

// Change history:
// 1995-05-15 0.15a MRP: Use Windows string resources, add higher level notes
// 1995-05-08 0.15  MRP: Initial design sketch; replace simple sErr arg


#ifndef NOT_H
#define NOT_H

#include "cdbllist.h"


// **************************************************************************

// E.g. IDSNOTE_FIL_CON_EMPTY0 identifies a note describing the problem:
// A filter's condition was empty, or consisted entirely of white space
// The note's text might read: Expecting a filter condition.
//     IDSNOTE: Identifier symbol for a string resource used in a note
//     FIL: Hungarian of entity (e.g. filter) which has a problem
//     CON: Hungarian of sub-component (e.g. condition), if any
//     EMPTY: A very terse statement of the problem
//         Adjectives imply "is" (e.g. this obligatory element is EMPTY)
//         Nouns often imply "expecting a" (e.g. RPAREN);
//             but, if qualified negatively, may sometimes imply
//                 "the problem is a" (e.g. NAME_UNDEFINED)
//     0: Number of (placeholders for) contextual arguments (e.g. none)


// --------------------------------------------------------------------------

// An element of a note describes a problem using information available
// at a certain _level_, either of the part-whole hierarchy of the data,
// or of the procedural processing of the program.
// The first element is at the lowest level (i.e. deepest in the program's
// call stack), where the problem is first detected.
// The last element is at the highest level, closest to the context of the
// user's intended action and his understanding of the program's operation.

class CNoteElement : public CDblListEl  // Hungarian: nel
{
private:
	IDString m_idsNote;  // identifier of Windows string resource
	Str8 m_sNote; // 1.4ek Change CNote to use string instead of ID
	Str8 m_sArgs;  // info specific to this particular problem occurrence
	const char* m_pszReferent;  // part of data to which this note refers
	Length m_lenReferent;  // zero means a location; non-zero, a substring
	const char* m_pszStringContainingReferent;  // enclosing data item

private:	
	friend class CNote;
		// CNoteElement is private implementation for class CNote below.
		// For comments on member functions see the CNote public interface.
		
	CNoteElement(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent);
	CNoteElement(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent);
	CNoteElement(IDString idsNote, const char* pszArgs = "");

	void Initialize(IDString idsNote, const char* pszArgs, const char* pszReferent = NULL, Length lenReferent = 0, const char* pszStringContainingReferent = NULL);
		// Common implementation used by the constructors above.

//	IDString idsNote() const { return m_idsNote; } // 1.4ek
	BOOL bSubstitute(IDString idsIfThat, IDString idsSubstituteThis);
	
	CNoteElement(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent); // 1.4ek
	CNoteElement(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent); // 1.4ek
	CNoteElement(const char* pszNote, const char* pszArgs = ""); // 1.4ek
		
	void Initialize(const char* pszNote, const char* pszArgs, const char* pszReferent = NULL, Length lenReferent = 0, const char* pszStringContainingReferent = NULL); // 1.4ek

	BOOL bSubstitute(const char* pszIfThat, const char* pszSubstituteThis); // 1.4ek

	Str8 sNote() const;

	void SetStringContainingReferent(const char* pszString);
	BOOL bRefersToString(const char* pszString) const;
	
	void Referent(const char** ppszReferent, Length* plenReferent) const;
	DWORD dwSelReferent() const;  // selection in a dialog edit control
	DWORD dwMultiLineSelReferent() const;
		// To set the caret in a [Windows] multiline edit box
		// the offset must be incremented to account for the \r characters
		// that occur in the edit box but NOT in the actual string.

public:
	~CNoteElement() {}  // destructor
};  // class CNoteElement


// --------------------------------------------------------------------------

// A note is a description of one problem which prevented the program from
// doing a task for the user (e.g. updating an edited filter). It can have:
//     arguments specific to the particular problem occurrence;
//     a reference to a specific place in or part of the user's data;
//     elements added to the note at higher levels as the program's functions
//         return back to the user's context and level of intended action.

class CNote : public CDblListEl, private CDblList  // Hungarian: not
{
public:
	// Construct the first element of the note.
	CNote(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent);
		// Note with arguments, referring to a place in a user data string.
	CNote(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent);
		// Beginning of string containing referent not known at lower levels.
		// See SetStringContainingReferent() below.
	CNote(IDString idsNote, const char* pszArgs = "");
		// Note which doesn't need to refer directly to the user's data.

	// Construct the first element of the note.
	CNote(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent); // 1.4ek
		// Note with arguments, referring to a place in a user data string.
	CNote(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent); // 1.4ek
		// Beginning of string containing referent not known at lower levels.
		// See SetStringContainingReferent() below.
	CNote(const char* pszNote, const char* pszArgs = ""); // 1.4ek
		// Note which doesn't need to refer directly to the user's data.

	~CNote() {}  // destructor

	// Add a higher level element to the note.
	void Add(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent);
	void Add(IDString idsNote, const char* pszArgs, const char* pszReferent, Length lenReferent);
	void Add(IDString idsNote, const char* pszArgs = "");

//	IDString idsNote() const;  // unique identifier of _last_ element // 1.4ek
	BOOL bSubstitute(IDString idsIfThat, IDString idsSubstituteThis);
	
	void Add(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent, const char* pszStringContainingReferent); // 1.4ek
	void Add(const char* pszNote, const char* pszArgs, const char* pszReferent, Length lenReferent); // 1.4ek
	void Add(const char* pszNote, const char* pszArgs = "");	 // 1.4ek

	BOOL bSubstitute(const char* pszIfThat, const char* pszSubstituteThis); // 1.4ek
		// Some notes from the lowest level helper functions, which are
		// too generic, or even potentially confusing to the user,
		// are replaced by a more specific note at the next level.
		// E.g. "Expecting a name" is replaced by "Expecting a marker".

	Str8 sNote() const;
		// Return the text of the note with arguments filled in.
		// Concatenates elements from first to last, each separated by \r\n
		// (for the Windows API) and increasing spaces of indentation.
	
	void SetStringContainingReferent(const char* pszString);
		// Use after having noted a problem in a _substring_ at a low level.
		// E.g. Filter condition constructors note problems but don't know
		// where is the beginning of the string. The filter constructor does.
	BOOL bRefersToString(const char* pszString) const;
		// Returns whether this note refers to the user data string psz.
		// I.e. Is psz (the beginning of) the string containing what this
		// note refers to?
		// NOTE NOTE NOTE: You _must_ determine to which string (if any!)
		// this note refers _before_ attempting to use the referent.

	// The place in the data to which this note's _first_ element refers.	
	void Referent(const char** ppszReferent, Length* plenReferent) const;
	DWORD dwSelReferent() const;  // selection in a dialog edit control
	DWORD dwMultiLineSelReferent() const;
		// To set the caret in a [Windows] multiline edit box
		// the offset must be incremented to account for the \r characters
		// that occur in the edit box but NOT in the actual string.

private:
	CNoteElement* pnelFirst() const  // first element
		{ return (CNoteElement*)pelFirst(); }
	CNoteElement* pnelLast() const  // last element
		{ return (CNoteElement*)pelLast(); }
	CNoteElement* pnelNext( CNoteElement* pnelCur ) const  // after current
		{ return (CNoteElement*)pelNext( pnelCur ); }
	CNoteElement* pnelPrev( CNoteElement* pnelCur ) const  // before current
		{ return (CNoteElement*)pelPrev( pnelCur ); }

	void Add( CNoteElement* pnelNew );  // add new element at end	
};  // class CNote

// For an example of using notes to give interactive feedback when entities
// are added and edited, see: dlgfiled.cpp, fil.cpp, mkr.cpp, patmch.cpp
//
// E.g. Updating the filter condition "[p|n] or [|v]" causes the following:
// *ppnot = new CNote("Expecting a name", ...);  // "Expecting a name"
// (*ppnot)->bSubstitute("Expecting a name", "Expecting a marker");
// (*ppnot)->Add("in a marker-string filter condition");
//
// The resulting note and data referent (insertion point denoted by ^):
// "Expecting a marker
//   in a marker-string filter condition"
// Condition: [p|n] or [^|v]


// --------------------------------------------------------------------------

// A list of notes is a report of problems the program had doing a task
// which either involved entity sets (e.g. filters), or sub-tasks which
// the user may not realize were required to accomplish the intended task
// (e.g. reading marker sets and language encodings referred to by databases
// being autoloaded when starting Shoebox).
// The program will have attempted to recover as best as possible from
// problems in "composite" or "batch" operations, and will explain afterward
// in the note list what additional action the user should take to resolve
// the problems completely.
// The list is empty if nothing noteworthy occurred.

class CNoteList : public CDblList  // Hungarian: notlst
{
public:
	const CNote* pnotFirst() const  // first note
		{ return (const CNote*)pelFirst(); }
	const CNote* pnotLast() const  // last note
		{ return (const CNote*)pelLast(); }
	const CNote* pnotNext( const CNote* pnotCur ) const  // note after current
		{ return (const CNote*)pelNext( (CNote*)pnotCur ); }
	const CNote* pnotPrev( const CNote* pnotCur ) const  // note before current
		{ return (const CNote*)pelPrev( (CNote*)pnotCur ); }

	void Add( CNote* pnotNew );  // add new note to end
};  // class CNoteList


// **************************************************************************
/*
                      OUR COUNTERPRODUCTIVE INSTINCTS
                *Our programs evaluate our user's actions.*

Our natural inclination is to have our programs view input from the
perspective of whether it is right or wrong according to what the
program expects. ...The user finds this judgmental and becomes defensive.
The effective communicator structures his communications to be
descriptive of the problem involved.

_A user expects that programs will not judge his use of them,
but will help him overcome his problems in using them._

[Paul Heckel, _The Elements of Friendly Software Design_. (2nd ed.)
SYBEX. 1991. Chapter 2, "Our Counterproductive Instincts", p. 15.]

*/
/* --------------------------------------------------------------------------

                             DESIGNING FOR ERROR

The user has done something that the system cannot respond to. Why are
these situations called "errors"? What is really meant is that the system
can't interpret the information given to it. Interaction with the system
is to be thought of as a cooperative endeavor, not to find fault and
to assess blame; but, rather, to get the task done.

We divide errors into two major categories: mistakes and slips.
The division occurs at the level of intention:
If the intention is not appropriate, this is a _mistake_.
If the action is not what was intended, this is a _slip_.

Detecting an error is the first step toward recovery: early detection is
extremely important. Slips are easier to detect than mistakes.

How should the system respond when it cannot interpret the information
given to it by the user?

a. Gag: prevent him from continuing, from expressing impossible intentions.
   E.g. A car will not drive unless the key is in the ignition.
   
b. Warn: tell him of a potentially dangerous situation, but let him decide
   how to respond. In some sense, warn might be considered error-preventing.
   E.g. A buzzer warns when the seatbelts are not fastened, but the car
   can be driven even if the warning is ignored.
   
c. Do nothing: if he attempts an illegal action, it just doesn't work.
   Relies on visibility of the effects of operations to convey the gap
   between intentions and outcomes. Used in direct manipulation interfaces.
   
d. Self correct: guess a legal action that might be what he would like to do.
   DWIM [Do What I Mean] is acceptable only when there is a good "undo"
   facility, so if DWIM does something inappropriate, it is easy to undo it.
   
e. Let's talk about it: initiate a dialog with him. Here we have shared
   responsibility between the user and system to explore the problem and
   come up with a solution.
   
f. Teach me: query him to find out just what a particular phrase or command
   might have meant. Used in natural language interfaces.


                             THE PROBLEM OF LEVEL

The system detects an error, the user recognizes that an error has been made,
but just where is the problem? What information is most useful for the user?
One approach to the levels problem is to know the user's intention.

[Clayton Lewis and Donald A. Norman, "Designing for Error". Chapter 20 in
_User-Centered System Design_, Donald A. Norman and Stephen W. Draper (eds.)
Lawrence Erlbaum Assoc. 1986. pp. 411-432.]
 

Most computer users are not familiar with how their systems work beyond a
certain level of operations. Consequently, there is often a problem with
error-message generation in interactive software systems. Such problems
often occur at a program execution level unknown to the user.

Is it possible for lower level operations to generate messages that
make sense at the user's level of intention? The answer is almost always no
unless the lower level operation "knows" the user's intention.

When an error is detected in the lowest level of program execution,
the context stack constitutes a path in the invocation tree, from the root
to the terminal node where the error has occurred. This path is called
the _error path_. The names of errors are inserted into an error list
every time they are signaled to higher levels. The first error inserted
into the list corresponds to the lowest level operation, and the last
corresponds to the user command. The last determines the initial message.

[Kemal Efe, "A Proposed Solution to the Problem of Levels in Error-Message
Generation". _Communications of the ACM_. November, 1987. pp. 948-955.]

*/
/* --------------------------------------------------------------------------

                           WINDOWS STRING RESOURCES

In programming for Windows, strings can be stored in a resource type
called a string table. An application can have only one string table.

In a string table, strings are grouped into segments, or blocks,
of 16 strings each. The segment a string belongs to is determined by
the value of its identifier; for example, strings with identifiers of
0 to 15 are in one string segment, strings with values of 16 to 31 are in
a second segment, and so on. Thus, to move a string from one segment
to another you need to change [the value of] its identifier.

Individual string segments are loaded on demand in order to conserve
memory. For this reason, programmers usually try to group strings into
logical groupings of sixteen or less and then to use each group or segment
only when it's needed.

[_App Studio User's Guide_. Chaper 6, "Using the String Editor", p. 75.]

The notes are stored as string resources in resource script "shwnotes.rc",
which is included in the main resource script "shw.rc". Their symbols
are defined in "shwnotes.h". Include it in .cpp files which construct notes.
The numeric values of note identifiers begin at 48000.

*/
// **************************************************************************

#endif  // NOT_H
