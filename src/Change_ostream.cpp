#include "Change_ostream.h"
#include <cstring>

Change_ostreambuf::Change_ostreambuf(const char* pszChangeTablePath, std::ostream& iosOutput)
    : m_iosOutput(iosOutput),
    m_buflen(512)
{
    m_pszBuffer = new char[m_buflen + 1];
    setp(m_pszBuffer, m_pszBuffer + m_buflen);

    m_cct.bLoadFromFile(pszChangeTablePath);
}

Change_ostreambuf::~Change_ostreambuf()
{
    sync();
    delete[] m_pszBuffer;
}

int Change_ostreambuf::sync()
{
    int len = (int)(pptr() - pbase());
    if (len > 0)
    {
        static char outBuf[1024];
        int outLen = sizeof(outBuf);

        if (m_cct.bLoaded() && m_cct.bMakeChanges(pbase(), len, outBuf, &outLen))
        {
            m_iosOutput.write(outBuf, outLen);
        }
        else
        {
            m_iosOutput.write(pbase(), len);
        }

        pbump(-len);
    }
    m_iosOutput.flush();
    return 0;
}

int Change_ostreambuf::overflow(int ch)
{
    if (ch != std::char_traits<char>::eof())
    {
        *pptr() = (char)ch;
        pbump(1);
    }
    // sync returns 0 on success, EOF (traits::eof()) on failure.
    if (sync() == std::char_traits<char>::eof())
        return std::char_traits<char>::eof();
    return ch;
}
