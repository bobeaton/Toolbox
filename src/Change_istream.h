#pragma once
#include "stdafx.h"
#include <streambuf>
#include <istream>
#include "ChangeTable.h"

class Change_istreambuf : public std::streambuf
{
public:
    Change_istreambuf(const char* pszChangeTablePath, std::istream& iosInput);
    virtual ~Change_istreambuf();

protected:
    virtual int underflow() override;

private:
    ChangeTable   m_cct;
    std::istream& m_iosInput;
    bool          m_bAtEOF;
    char* m_pszBuffer;
    const int     m_buflen;
};

class Change_istream : public std::istream
{
public:
    Change_istream(const char* pszChangeTablePath, std::istream& iosInput)
        : std::istream(nullptr),
        m_buf(pszChangeTablePath, iosInput)
    {
        init(&m_buf);
    }

    ~Change_istream() override = default;

private:
    Change_istreambuf m_buf;
};
