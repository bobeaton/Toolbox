#include "Change_istream.h"
#include <vector>
#include <cstring>

Change_istreambuf::Change_istreambuf(const char* pszChangeTablePath, std::istream& iosInput)
    : m_iosInput(iosInput),
    m_bAtEOF(false),
    m_buflen(512)
{
    m_pszBuffer = new char[m_buflen + 1];
    setg(m_pszBuffer, m_pszBuffer, m_pszBuffer);

    if (m_cct.bLoadFromFile(pszChangeTablePath))
        ; // successfully loaded
    else
        m_bAtEOF = true;
}

Change_istreambuf::~Change_istreambuf()
{
    delete[] m_pszBuffer;
}

int Change_istreambuf::underflow()
{
    if (m_bAtEOF)
        return EOF;

    // Read from source
    m_iosInput.read(m_pszBuffer, m_buflen);
    std::streamsize lenRead = m_iosInput.gcount();

    if (lenRead <= 0)
    {
        m_bAtEOF = true;
        return EOF;
    }

    // Apply change table transformation
    static char outBuf[1024];
    int outLen = sizeof(outBuf);

    if (!m_cct.bMakeChanges(m_pszBuffer, (int)lenRead, outBuf, &outLen))
    {
        m_bAtEOF = true;
        return EOF;
    }

    // Reset buffer pointers
    setg(outBuf, outBuf, outBuf + outLen);
    return (unsigned char)*gptr();
}
