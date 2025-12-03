// cct.cpp  Consistent Changes

// Since this file is intended to be reusable in other projects,
// we don't include stdafx.h, since that contains project-specific stuff.
#include "stdafx.h"
#include <afxwin.h>  // HANDLE, ASSERT

#include "cct.h"
#include <fstream>  // ifstream, ofstream

#ifdef _DEBUG
#endif  // _DEBUG


// --------------------------------------------------------------------------

#define _export

#include "ccdll.h"


// **************************************************************************

#if UseCct
HINSTANCE ChangeTable::s_hClientInstance = NULL;

ChangeTable::ChangeTable()
{
    m_hcct = 0;
}

ChangeTable::~ChangeTable()
{
    if ( bLoaded() )
        UnLoad();
}

BOOL ChangeTable::bLoadFromFile(const char* pszChangeTablePath)
{
    if ( bLoaded() )  // If another table had been loaded
        UnLoad();

    ASSERT( m_hcct == 0 );
    ASSERT( pszChangeTablePath );
    char* pszPath = (char*)pszChangeTablePath;  // Cast away const-ness
    HINSTANCE hClientInstance = ( s_hClientInstance ?
        s_hClientInstance :
        (HINSTANCE)GetCurrentProcess()
        );
    int iResult = CCLoadTable(pszPath, &m_hcct, hClientInstance);
    ASSERT( iResult == CC_SUCCESS ? m_hcct != 0 : m_hcct == 0 );
    
//  if ( m_hcct &&
//          CCSetErrorCallBack(m_hcct, s_OnChangeTableError) != CC_SUCCESS )
//      UnLoad();

    return bLoaded();
}

void ChangeTable::UnLoad()
{
    CCUnloadTable(m_hcct);
    m_hcct = 0;
}


void ChangeTable::Reinitialize()
{
    ASSERT( m_hcct );
    CCReinitializeTable(m_hcct);
}


BOOL ChangeTable::bMakeChanges(const char* pszInputPath,
        const char* pszOutputPath)
{
    ASSERT( pszInputPath );
    std::ifstream iosInput(pszInputPath);
    if ( iosInput.fail() )
        return FALSE;

    ASSERT( pszOutputPath );
    std::ofstream iosOutput(pszOutputPath);
    if ( iosOutput.fail() )
        return FALSE;
        
    return bMakeChanges(iosInput, iosOutput);
}

// This function will be called by CC when CCGetBuffer needs more input.
// Register it and the stream pointer via CCSetUpInputFilter.
//
// The callback function should copy len characters from its input source
// into the pszInput buffer. It is required to fill the buffer except
// [on the last call] when the input is exhausted.

int WINAPI _export s_iInputFromStream(char FAR * pszInputBuffer, int len,
        long* plUserInputData)
{
    ASSERT( pszInputBuffer );
    ASSERT( 0 <= len );
    ASSERT( plUserInputData );
    std::istream* piosInput = (std::istream*)*plUserInputData;
    ASSERT( piosInput );
    (void) piosInput->read(pszInputBuffer, len);
    std::streamsize lenRead = piosInput->gcount();

    return (int)lenRead; 
}

BOOL ChangeTable::bMakeChanges(std::istream& iosInput, std::ostream& iosOutput)
{
    ASSERT( bLoaded() );

    // Make this like a fresh run of CC on these input and output files 
    if ( CCReinitializeTable(m_hcct) != CC_SUCCESS )
        return FALSE;

    // Register the input callback function and the input stream.
    if ( CCSetUpInputFilter(m_hcct, s_iInputFromStream, (long)&iosInput) !=
            CC_SUCCESS )
        return FALSE;

    // Allocate a buffer for the output
    const int maxlenOutput = 512;  // Can change later to tune performance
    char* pszOutputBuffer = new char[maxlenOutput];
    if ( !pszOutputBuffer )
        return FALSE;

    // Keep getting buffers of changed output until CC exhausts its input
    int lenOutput = maxlenOutput;
    int iResult = CCGetBuffer(m_hcct, pszOutputBuffer, &lenOutput);
    while ( iResult == CC_GOT_FULL_BUFFER )  // Not yet at end of input
        {
        (void) iosOutput.write(pszOutputBuffer, lenOutput);
        lenOutput = maxlenOutput;
        iResult = CCGetBuffer(m_hcct, pszOutputBuffer, &lenOutput);
        }
    BOOL bMadeChanges = ( iResult == CC_GOT_END_OF_DATA );
    if ( bMadeChanges )
        (void) iosOutput.write(pszOutputBuffer, lenOutput);  // Last buffer

    delete [] pszOutputBuffer;  // Free the output buffer
    
    return bMadeChanges; 
}

BOOL ChangeTable::bMakeChanges(const char* pszInput, int lenInput, char* pszOutputBuffer, int* plenOutput)
{
    ASSERT( bLoaded() );
    // CCProcessBuffer now reinitializes the table automatically.

    char* psz = (char*)pszInput;  // Cast away const-ness
	BOOL bSucc = CCProcessBuffer(m_hcct, psz, lenInput, pszOutputBuffer, plenOutput) == CC_SUCCESS;
    return bSucc;
}

BOOL ChangeTable::s_bMakeChanges(const char* pszChangeTablePath,
        const char* pszInputPath, const char* pszOutputPath)
{
    ChangeTable cct;
    if ( !cct.bLoadFromFile(pszChangeTablePath) )
        return FALSE;
        
    return cct.bMakeChanges(pszInputPath, pszOutputPath);
}


BOOL ChangeTable::bSetInputCallback(CCInputProc *pfn, long lClientData)
{
    int iResult = CCSetUpInputFilter(m_hcct, pfn, lClientData);
    return iResult == CC_SUCCESS;
}

BOOL ChangeTable::bPullOut(char* psz, int* plen, BOOL* pbEndOfInput)
{
    if ( !bLoaded() )
        {
        ASSERT( plen );
        *plen = 0;
        return FALSE;
        }

    int iResult = CCGetBuffer(m_hcct, psz, plen);

    BOOL bGotBuffer = FALSE;
    if ( iResult == CC_GOT_FULL_BUFFER )
        bGotBuffer = TRUE;
    else if ( iResult == CC_GOT_END_OF_DATA )
        {
        ASSERT( pbEndOfInput );
        *pbEndOfInput = TRUE;
        bGotBuffer = TRUE;
        }       
    return bGotBuffer;
}


BOOL ChangeTable::bSetOutputCallback(CCOutputProc *pfn, long lClientData)
{
    int iResult = CCSetUpOutputFilter(m_hcct, pfn, lClientData);
    return iResult == CC_SUCCESS;
}

BOOL ChangeTable::bPushIn(char* psz, int len, BOOL bEndOfInput)
{
    if ( !bLoaded() )
        return FALSE;

    int iResult = CCPutBuffer(m_hcct, psz, len, bEndOfInput);
    return iResult == CC_SUCCESS;
}

BOOL ChangeTable::bPushEndOfInput()
{
    if ( !bLoaded() )
        return FALSE;

    int iResult = CCFlush(m_hcct);
    return iResult == CC_SUCCESS;
}
#endif

// --------------------------------------------------------------------------

#ifdef SAMPLE_CALLBACK_FUNCTIONS
// The following careful reading algorithm, which had proved necessary
// for input using the file descriptor _read function, seems no longer
// to be required for the input stream read function. But just in case...

std::streamsize WINAPI _export s_iInputFromStream(char FAR * pszInput, int len)
{
    ASSERT( 0 <= len );
    if ( len == 0 )
        return 0;
        
    char* psz = pszInput;
    ASSERT( psz );
    std::streamsize lenRemaining = len;
    ASSERT( s_piosInput );
    
    // NOTE: The reason we cannot use a simple call to _read is that
    // the file ought to have been opened in text mode so that newline is
    // a single '\n' character that CC expects, rather than a "\r\n" pair.
    // In text mode the length requested represents the binary length
    // in the file but the length returned is the text length in the buffer.
    // The latter will generally be less than the former due to newlines.
    // Since CC assumes end of data if it receives less than it requested,
    // we keep reading until the buffer is full or the input is exhausted.
    int nchRead = 0;
    do
        {
        (void) s_piosInput->read(psz, lenRemaining);
        nchRead = s_piosInput->gcount();
        psz += nchRead;
        lenRemaining -= nchRead;
        } while ( lenRemaining != 0 && nchRead != 0 );

    return len - lenRemaining;  
}

// This function will be called by CC when CCPutBuffer produces more output.
// Register it and the stream pointer via CCSetUpOutputFilter.
//
// The callback function should copy from the pszOutput buffer len characters
// to its ouput destination.

int WINAPI _export s_iOutputToStream(char FAR * pszOutput, int len,
        long* plUserOutputData)
{
    ASSERT( 0 <= len );
    if ( len == 0 )
        return 0;
        
    ASSERT( pszOutput );
    ASSERT( plUserOutputData );
    std::ostream* piosOutput = (std::ostream*)*plUserOutputData;
    ASSERT( piosOutput );
    piosOutput->write(pszOutput, len);
    int iResult = piosOutput->fail();  // Success: zero; I/O error: non-zero
    
    return iResult;
}
#endif  // SAMPLE_CALLBACK_FUNCTIONS

// 1.5.0fa Remove cct test code, it had lots of odd strings
// --------------------------------------------------------------------------

Change_istreambuf::Change_istreambuf(const char* pszChangeTablePath,
        std::istream& iosInput) :
    m_iosInput(iosInput),
    m_bAtEOF(FALSE)
{
    const int len = 512;  // 1996-05-11 MRP: Make an optional argument???
	m_iBufferLen = len;
    m_pszBuffer = new char[m_iBufferLen];
    setbuf(m_pszBuffer, m_iBufferLen);  // Reserve area
    setg(m_pszBuffer, m_pszBuffer, m_pszBuffer);  // Get area
    if ( m_cct.bLoadFromFile(pszChangeTablePath) )
        m_cct.bSetInputCallback(s_iInputFromChange_istreambuf, (long)this);
    else
        IndicateFailure();
}

Change_istreambuf::~Change_istreambuf()
{
    delete [] m_pszBuffer;  // Reserve area
}
    
int Change_istreambuf::overflow(int ch)
{
    ASSERT( FALSE );  // Not intended to be called for input
    return std::char_traits<char>::eof();
}

int Change_istreambuf::sync()
{
    ASSERT( FALSE );  // Not intended to be called for input
    return std::char_traits<char>::eof();
}

int Change_istreambuf::underflow()
{
    std::streamsize lenRemaining = in_avail();
    if ( lenRemaining == 0 )  // If the get area is empty
        {
        if ( m_bAtEOF )  // If the last fill exhausted the input
            return std::char_traits<char>::eof();
            
        // Fill the get area with changed output from the table
        char* psz = m_pszBuffer;         
        int maxlen = m_iBufferLen;
        int len = maxlen;
        if ( !m_cct.bPullOut(psz, &len, &m_bAtEOF) )
            IndicateFailure();
        if ( len < maxlen )  // If the table has exhausted its input,
            m_bAtEOF = TRUE;  // Then don't refill the get area again.
        if ( len == 0 )  // If the get area is still empty after filling
            return std::char_traits<char>::eof();

        setg(psz, psz, psz + len);  // Get area
        }

    unsigned char uch = *gptr();  // Avoid returning character 0xFF as EOF
    return uch;  // Return the buffer get area's first character
}

std::streamsize Change_istreambuf::lenReadFromSource(char* psz, int len)
{
    ASSERT( psz );
    ASSERT( 0 <= len );
    if ( len == 0 )
        return 0;

    BOOL bFailBitSetPreviously = ( m_iosInput.rdstate() & ios::failbit );       
    (void) m_iosInput.read(psz, len);  // istream::read returns an istream&
    // NOTE: If the stream had less than len characters remaining,
    // then read will have set the fail bit. We don't want that.
    int iState = m_iosInput.rdstate();
    BOOL bFailBitSet = ( iState & ios::failbit );
    if ( bFailBitSet && !bFailBitSetPreviously )  // If it just got set now,
        m_iosInput.clear(iState & ~ios::failbit);  // Then clear it

    std::streamsize lenRead = m_iosInput.gcount();  
    return lenRead;  // Number of characters actually read
}

// CC will call this function when CCFillBuffer requires more input.
// Register it and the stream buffer pointer via CCSetUpInputFilter.
// The callback function will copy to the psz buffer len characters
// from its input source unless there are fewer characters remaining.

int WINAPI _export s_iInputFromChange_istreambuf(char FAR * psz, int len,
        long* plUserInputData)
{
    ASSERT( plUserInputData );
    Change_istreambuf* pbuf = (Change_istreambuf*)*plUserInputData;
    ASSERT( pbuf );
    return (int)pbuf->lenReadFromSource(psz, len);  // Number of characters read
}


// --------------------------------------------------------------------------

Change_ostreambuf::Change_ostreambuf(const char* pszChangeTablePath,
        std::ostream& iosOutput) :
    m_iosOutput(iosOutput)
{
    const int len = 512;  // 1996-05-11 MRP: Make an optional argument???
    m_pszBuffer = new char[len];
    setbuf(m_pszBuffer, len);  // Reserve area
    setp(m_pszBuffer, m_pszBuffer + len);  // Put area
    if ( m_cct.bLoadFromFile(pszChangeTablePath) )
        m_cct.bSetOutputCallback(s_iOutputToChange_ostreambuf, (long)this);
    else
        IndicateFailure();
}

Change_ostreambuf::~Change_ostreambuf()
{
    // 1a. Flush anything remaining in the put area into the table
    if ( sync() == std::char_traits<char>::eof())
        IndicateFailure();
    // 1b. Indicate to the table that this is the end of input
    if ( !m_cct.bPushEndOfInput() )
        IndicateFailure();
    // 2. Flush the actual output stream destination
    m_iosOutput.flush();
    delete [] m_pszBuffer;  // Reserve area
}
    
int Change_ostreambuf::overflow(int ch)
{
    int iResult = sync();  // Flush the contents of the buffer's put area
    
    if ( ch != std::char_traits<char>::eof())
        {
        *pptr() = ch;  // Put character ch into the buffer's put area
        pbump(1);         
        }
        
    return iResult;
}

int Change_ostreambuf::sync()
{
    int iResult = 0;  // Success
    // int len = out_waiting();
	int len = static_cast<int>(pptr() - pbase());
    if ( len != 0 )
        {
        char* psz = pbase();
        if ( !m_cct.bPushIn(psz, len, FALSE) )
            iResult = std::char_traits<char>::eof();  // Error
        pbump(-len);
        }

    if ( iResult == std::char_traits<char>::eof())
        IndicateFailure();
        
    return iResult;
}

int Change_ostreambuf::underflow()
{
    ASSERT( FALSE );  // Not intended to be called for output
    return std::char_traits<char>::eof();
}

void Change_ostreambuf::WriteToDestination(char* psz, int len)
{
    ASSERT( psz );
    ASSERT( 0 <= len );
    if ( len == 0 )
        return;
        
    m_iosOutput.write(psz, len);
}

// CC will call this function when CCTakeBuffer must flush some output.
// Register it and the stream buffer pointer via CCSetUpOutputFilter.
// The callback function will copy from the psz buffer len characters
// to its ouput destination.

int WINAPI _export s_iOutputToChange_ostreambuf(char FAR * psz, int len,
        long* plUserOutputData)
{
    ASSERT( plUserOutputData );
    Change_ostreambuf* pbuf = (Change_ostreambuf*)*plUserOutputData;
    ASSERT( pbuf );
    pbuf->WriteToDestination(psz, len);

    // Note: Once the pbuf's destination stream fails to write the buffer,
    // it simply ignores subsequent attempts to write. Since it will have
    // set its fail bit, which needs to be checked at the end anyway,
    // there is no additional benefit in returning such a failure to CC.
    return CC_SUCCESS;
}

// --------------------------------------------------------------------------

Change_istream::Change_istream(const char* pszChangeTablePath,
        std::istream& iosInput) :
	std::istream(nullptr),
    m_bufChangeTable(pszChangeTablePath, iosInput)
{
    if ( m_bufChangeTable.bLoaded() )
        init(&m_bufChangeTable);
    else
        clear(ios::failbit);  // This actually *sets* the fail bit
}

// --------------------------------------------------------------------------

Change_ostream::Change_ostream(const char* pszChangeTablePath,
        std::ostream& iosOutput) :
	std::ostream(nullptr),
    m_bufChangeTable(pszChangeTablePath, iosOutput)
{
    if ( m_bufChangeTable.bLoaded() )
        init(&m_bufChangeTable);
    else
        clear(ios::failbit);  // This actually *sets* the fail bit
        // Note: iosOutput's fail bit will have already been set
}

// --------------------------------------------------------------------------

int strstreambuf_d255::underflow()
{
    int ch = strstreambuf::underflow();  // Call the base class function
    if ( ch == std::char_traits<char>::eof() && in_avail() != 0 )  // If the EOF might be incorrect
        {
        int chNext = *(unsigned char*)gptr();  // Double-check it
        if ( chNext == 255 )  // If it should be a d255 character instead
            ch = chNext;  // Correct the return value
        }
        
    return ch;
}

strstream_d255::strstream_d255()
	: std::iostream(nullptr)
{
    init(&m_buf);  // Use the derived strstreambuf_d255 class buffer
}
