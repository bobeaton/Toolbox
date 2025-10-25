//File: MacPcStr.h

// 08-08-1997
// This stream class provides all, datas from PC and MAC and UNIX
// This changes will happen:
// '/xd''/xa' -> '/n'
// '/xa' -> '/n'
// '/xd' -> '/n'

#ifndef nlstream_H
#define nlstream_H

#include <iostream>
using namespace std;  // classes istream, streambuf

class Newline_istreambuf : public std::streambuf
{
private:
    std::istream& m_iosInput;      // The source of input (Source of buffer)
    BOOL m_bAtEOF;
    char* m_pszbuffer;        // Pointer to buffer
    const int m_ibufferlen;    // Length of buffer
    
public:
    Newline_istreambuf(std::istream& iosInput);
      // Konstructor
    virtual ~Newline_istreambuf();
      // Destructor

      // The virtual overflow, sync, and underflow functions
      // define the characteristics of a streambuf - derived class.
    virtual int overflow(int ch);  //Not intend to be called for input
    virtual int sync();  //Not intend to be called for input
    virtual int underflow();
      // Return the first character from this buffer's get area.
      // In this function the extraction of '\r' occures.
      // Return EOF when the input is exhausted.

private:
    std::streamsize lenReadFromSource(char* pszbuffer, int len);
      // Connect the input stream m_iosInput to the buffer of the
      // derived class
      // pszbuffer is the Destination from len Characters.
      // The function returns the realy extracted characters
};


//-----------------------------------------------------------------------------

class Newline_istream : public std::istream
{
private:  
    Newline_istreambuf m_buffer;

public:
    Newline_istream(std::istream& iosInput);
      // Konstructor
};

//-----------------------------------------------------------------------------

#endif  //nlstream_H
