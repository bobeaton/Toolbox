// Implementation of nlstream.h
//File: nlstream.cpp

#include <afxwin.h>    //ASSERT
#include"nlstream.h"


Newline_istreambuf::Newline_istreambuf(istream& iosInput) : 
                    m_iosInput(iosInput),m_ibufferlen(512)
{
    m_bAtEOF = FALSE;
  
      // One Byte extra for defined Pointeroperation
    m_pszbuffer = new char[m_ibufferlen+1];
    setbuf(m_pszbuffer, m_ibufferlen+1);  // Reserve area
    ASSERT( base() == m_pszbuffer );
    
      // Fill the buffer
    int ilenread = lenReadFromSource(m_pszbuffer,m_ibufferlen);  
    setg(m_pszbuffer, m_pszbuffer, m_pszbuffer+ilenread);  // Get area    
}


Newline_istreambuf::~Newline_istreambuf()
{
    delete [] base();  // Reserve area
}
    

int Newline_istreambuf::overflow(int ch)
{
    ASSERT( FALSE );  // Not intended to be called for input
    return EOF;
}


int Newline_istreambuf::sync()
{
    ASSERT( FALSE );  // Not intended to be called for input
    return EOF;
}


int Newline_istreambuf::underflow()
{
    int ilenRemaining = in_avail();  
    
    if (ilenRemaining == 0)   // If the get area is empty
        {
        if (m_bAtEOF )       // If the last fill exhausted the input
            return EOF;
    
           // Fill the get area again!
        int ilenread = lenReadFromSource(m_pszbuffer,m_ibufferlen);
           // Adjust Pointer for get arae         
        setg(m_pszbuffer, m_pszbuffer, m_pszbuffer + ilenread);    
        } 
    
    ilenRemaining = in_avail();   // is necessary for bufferlength = 1;
    
    if ( (*gptr() == '\xd') || (*gptr() == '\xa') ) // if hex code for \r or \n detected
        {
        if ( (ilenRemaining == 1) && (*gptr() == '\xd') ) // \r on end of buffer
            {
            if (m_iosInput.peek() == '\xa')   // the next charactar is \n
                m_iosInput.ignore(1);         // ignore it
            *gptr() = '\n';                   // change current charater into \n  
            }
        else                    // not end of buffer
            {
                if ( (*gptr() == '\xd') && (*(gptr() + 1) == '\xa') )
                                             // carriage return followed by \n
                    gbump(1);                // skip carriage

               *gptr() = '\n';        // change it into \n
             }
        }

    unsigned char uch = *gptr();  // Avoid returning character 0xFF as EOF
    return uch;  // Return the current character
}


int Newline_istreambuf::lenReadFromSource(char* psz, int ilen)
{
    ASSERT( 0 <= ilen );
    if ( ilen == 0 )
        return 0;

    (void) m_iosInput.read(psz, ilen);  // read from source into buffer
    
    int ilenread = m_iosInput.gcount();  
             
    if ( (ilenread < ilen) || (m_iosInput.peek() == EOF)) // If EOF is reached
        m_bAtEOF = TRUE;
   
    return ilenread;
}


//-----------------------------------------------------------------------------

Newline_istream::Newline_istream(istream& iosInput) : 
                                 m_buffer(iosInput), istream(&m_buffer)
{}
