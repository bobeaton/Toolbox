// cct.h  Consistent Changes

#ifndef CCT_H
#define CCT_H

#include <iostream>
using namespace std;  // classes istream, ostream, streambuf
#include <sstream>  // class strstreambuf
#include <strstream> 

#if UseCct
typedef int WINAPI CCInputProc(char FAR *, int, long* plUserInputData);
typedef int WINAPI CCOutputProc(char FAR *, int, long* plUserOutputData);


// Consistent Changes is described in the DTS manual (Waxhaw: JAARS, 1990)
// and in "Using the Consistent Changes (CC) Processor" (SIL, 1991).

class ChangeTable  // Hungarian: cct
{
private:
	HANDLE m_hcct;  // Handle to this table's state in the change engine
	
	static HINSTANCE s_hClientInstance;
		// The client's instance handle improves robustness when the
		// Consistent Change engine is called as a dynamic link library.
		// If this has not been set, the task handle is used instead.
	
	// Prevent use of the copy constructor and assignment operator
	// by making them private and omitting their definitions.
	ChangeTable(const ChangeTable& cct);
	ChangeTable& operator=(const ChangeTable& cct);
	
public:
	ChangeTable();
		// NOTE: Since external circumstances, such as correctness of the
		// change table or existence of the file containing it, determine
		// success of loading a table, we use "two-phase" construction.
		// You must not only construct a table, but also make sure that
		// it was loaded successfully before using it.
	~ChangeTable();

	BOOL bLoadFromFile(const char* pszChangeTablePath);
		// Load the change table contained in the file pszChangeTablePath.
		// You can use this function again to choose a different table.
		
	BOOL bLoaded() const { return m_hcct != 0; }

	// The following functions make changes as if from one call to CC,
	// by reinitializing the table's state beforehand.
	
	BOOL bMakeChanges(const char* pszInputPath, const char* pszOutputPath);
		// cc -t this -o pszOutputPath pszInputPath
		// The table gets its input from the contents of file pszInputPath
		// and outputs the result to pszOutputPath. The contents of the 
		// output file will be replaced if it already exists; otherwise,
		// it will be created.
	BOOL bMakeChanges(std::istream& iosInput, std::ostream& iosOutput);
		
	BOOL bMakeChanges(const char* pszInput, int lenInput,
			char* pszOutputBuffer, int* plenOutput);
		// The table gets as its input lenInput characters at pszInput
		// and copies the result into pszOutputBuffer.
		// You are responsible to provide an output buffer large enough
		// to store the result of those changes on that input.
		// The initial value of *plenOutput is the length of the buffer;
		// its value upon successful return is the actual amount stored.
		// The input need not be null-terminated, but if the input
		// includes a null [in its length], the output will also.

	// NOTE: If you are using a table only once to change files, instead of
	// constructing a table object at all just use the following function:
	static BOOL s_bMakeChanges(const char* pszChangeTablePath,
			const char* pszInputPath, const char* pszOutputPath);
		// cc -t pszChangeTablePath -o pszOutputPath pszInputPath
		
	static void SetClientInstanceHandle(HINSTANCE hClientInstance)
		{ s_hClientInstance = hClientInstance; }
		// A client program written in C++ using the MFC library could call
		// this with argument m_hInstance from the InitInstance function.
		
private:
	void UnLoad();
	void Reinitialize();
	
	friend class Change_ostreambuf;
	BOOL bSetOutputCallback(CCOutputProc *pfn, long lClientData);
	BOOL bPushIn(char* psz, int len, BOOL bEndOfInput);
	BOOL bPushEndOfInput();

	friend class Change_istreambuf;
	BOOL bSetInputCallback(CCInputProc *pfn, long lClientData);
	BOOL bPullOut(char* psz, int* plen, BOOL* pbEndOfInput);
};  // class ChangeTable
#endif

// --------------------------------------------------------------------------

/*
// The DOS command line:  cc -t table.cct -o output.out input.inp

// Corresponds to the following code fragment:

	ChangeTable cct;
	if ( !cct.bLoadFromFile("table.cct") )
		return FALSE;
		
	if ( !cct.bMakeChanges("input.inp", "output.out") )
		return FALSE;
		
// Or, if you are using the table only once:

	if ( !ChangeTable::s_bMakeChanges("table.cct", "input.inp", "output.out") )
		return FALSE;

// See also the internal tests in cct.cpp called by ChangeTable::s_Test().
*/


// --------------------------------------------------------------------------

class Change_istreambuf : public std::streambuf  // Hungarian: buf
{
private:
	ChangeTable m_cct;
	std::istream& m_iosInput;  // The actual source of input to be changed
	BOOL m_bAtEOF;
	char* m_pszBuffer;
	int m_iBufferLen;

	// Prevent use of the copy constructor and assignment operator
	// by making them private and omitting their definitions.
	Change_istreambuf(const Change_istreambuf& buf);
	Change_istreambuf& operator=(const Change_istreambuf& buf);

public:
	Change_istreambuf(const char* pszChangeTablePath, std::istream& iosInput);
		// Make changes to the text to be got from this buffer
		// according to the table contained in the file pszChangeTablePath.
		// The buffer's and table's source is stream iosInput. 
	virtual ~Change_istreambuf();
	
	// The virtual overflow, sync, and underflow functions
	// define the characteristics of a streambuf-derived class.	
	virtual int overflow(int ch);  // Not intended to be called for input
	virtual int sync();  // Not intended to be called for input
	virtual int underflow();
		// Return the first character from this buffer's get area.
		// Return EOF when the input is exhausted.
		// (When the client stream extracts text from this buffer
		// it refills itself by pulling text out from its change table m_cct
		// whose input is read from the actual stream source m_ios.)
	
	BOOL bLoaded() const { return m_cct.bLoaded(); }
	
private:
	void IndicateFailure()
		{ m_iosInput.clear(ios::failbit); }  // Actually *sets* the fail bit
		// Since the client must check the source stream's error flags
		// at the end anyway, we use them to indicate change failures too.
		// See the implementation of Change_istream::s_bDebugFailure.

	// Connect the input stream source m_ios to the change table m_cct.
	std::streamsize lenReadFromSource(char* psz, int len);
	friend std::streamsize WINAPI s_iInputFromChange_istreambuf(char FAR * psz,
			int len, long* plUserInputData);
};  // class Change_istreambuf


// --------------------------------------------------------------------------

class Change_ostreambuf : public std::streambuf  // Hungarian: buf
{
private:
	ChangeTable m_cct;
	std::ostream& m_iosOutput;  // The actual destination for changed output
	
	// Prevent use of the copy constructor and assignment operator
	// by making them private and omitting their definitions.
	Change_ostreambuf(const Change_ostreambuf& buf);
	Change_ostreambuf& operator=(const Change_ostreambuf& buf);

	char* m_pszBuffer;

public:
	Change_ostreambuf(const char* pszChangeTablePath, std::ostream& iosOutput);
		// Make changes to the text put into this buffer
		// according to the table contained in the file pszChangeTablePath.
		// The buffer's and table's destination is stream iosOutput.
	virtual ~Change_ostreambuf();
		// Push any remaining buffered output into change table m_cct and
		// flush it to output stream m_ios.

	// The virtual overflow, sync, and underflow functions
	// define the characteristics of a streambuf-derived class.	
	virtual int overflow(int ch);
		// Put character ch (unless it is EOF) into this buffer's put area.
		// Return EOF to indicate an error.
		// (When the client stream inserts text into this buffer
		// it flushes itself by pushing text into its change table m_cct
		// whose output is written to the actual stream destination m_ios.)
	virtual int sync();
		// Flush the buffer's put area. Return EOF to indicate an error.
	virtual int underflow();  // Not intended to be called for output
	
	BOOL bLoaded() const { return m_cct.bLoaded(); }
	
private:
	void IndicateFailure()
		{ m_iosOutput.clear(ios::failbit); }  // Actually *sets* the fail bit
		// Since the client must check the destination stream's error flags
		// at the end anyway, we use them to indicate change failures too.
		// See the implementation of Change_ostream::s_bDebugFailure.

	// Connect the change table m_cct to the output stream destination m_ios.
	void WriteToDestination(char* psz, int len);
	friend int WINAPI s_iOutputToChange_ostreambuf(char FAR * psz,
		int len, long* plUserOutputData);
};  // class Change_ostreambuf


// --------------------------------------------------------------------------

class Change_istream : public std::istream  // Hungarian: ios
{
private:
	Change_istreambuf m_bufChangeTable;
	
	// Prevent use of the copy constructor and assignment operator
	// by making them private and omitting their definitions.
	Change_istream(const Change_istream& ios);
	Change_istream& operator=(const Change_istream& ios);

public:
	Change_istream(const char* pszChangeTablePath, std::istream& iosInput);
		// Make changes to the text extracted from this stream
		// according to the table contained in the file pszChangeTablePath.
		// The table's input source is stream ios. 
		// Failure to load the change table sets this stream's fail bit.
		// That and all other failures set stream iosInput's fail bit.

#ifdef _DEBUG    
	static BOOL s_bDebugFailure(const char* pszChangeTablePath,
			const char* pszInputPath, const char* pszOutputPath);
		// cc -t pszChangeTablePath -o pszOutputPath pszInputPath
		// Return whether the file was read and changes were made.
		// (If there were any failures, look for TRACE debug output.)
		// Use this function's implementation as an example of using
		// change input streams and properly detecting failures.
#endif  // _DEBUG
};  // class Change_istream


// --------------------------------------------------------------------------

class Change_ostream : public std::ostream  // Hungarian: ios
{
private:
	Change_ostreambuf m_bufChangeTable;
	
	// Prevent use of the copy constructor and assignment operator
	// by making them private and omitting their definitions.
	Change_ostream(const Change_ostream& ios);
	Change_ostream& operator=(const Change_ostream& ios);

public:
	Change_ostream(const char* pszChangeTablePath, std::ostream& iosOutput);
		// Make changes to the text inserted into this stream
		// according to the table contained in the file pszChangeTablePath.
		// The table's output destination is stream ios. 
		// Failure to load the change table sets this stream's fail bit.
		// That and all other failures set stream iosOutput's fail bit.
		
#ifdef _DEBUG    
	static BOOL s_bDebugFailure(const char* pszChangeTablePath,
			const char* pszInputPath, const char* pszOutputPath);
		// cc -t pszChangeTablePath -o pszOutputPath pszInputPath
		// Return whether the changes were made and written successfully.
		// (If there were any failures, look for TRACE debug output.)
		// Use this function's implementation as an example of using
		// change output streams and properly detecting failures.
#endif  // _DEBUG
};  // class Change_ostream


// --------------------------------------------------------------------------

// Use class strstream_d255 as a substitute for strstream to avoid
// the following problem with special characters in MSVC++ 1.52c:
// the strstreambuf::underflow and strstream::get functions
// return character code d255 (xFF) as -1 (end of file),
// consequently the input gets truncated prematurely at that point.

class strstreambuf_d255 : public strstreambuf
{
public:
	virtual int underflow();
};

class strstream_d255 : public std::iostream
{
private:
	strstreambuf_d255 m_buf;
	
public:
	strstream_d255();
};

#endif  // CCT_H
