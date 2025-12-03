#pragma once
#include "stdafx.h"
#include <streambuf>
#include <ostream>
#include "ChangeTable.h"

class Change_ostreambuf : public std::streambuf
{
public:
    Change_ostreambuf(const char* pszChangeTablePath, std::ostream& iosOutput);
    virtual ~Change_ostreambuf();

protected:
    virtual int sync() override;
    virtual int overflow(int ch = std::char_traits<char>::eof()) override;

private:
    ChangeTable   m_cct;
    std::ostream& m_iosOutput;
    char* m_pszBuffer;
    const int     m_buflen;
};

class Change_ostream : public std::ostream
{
public:
    Change_ostream(const char* pszChangeTablePath, std::ostream& iosOutput)
        : std::ostream(nullptr),
        m_buf(pszChangeTablePath, iosOutput)
    {
        init(&m_buf);
    }

    ~Change_ostream() override = default;

private:
    Change_ostreambuf m_buf;
};
