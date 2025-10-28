#include "ChangeTable.h"
#include <fstream>
#include <vector>

ChangeTable::ChangeTable()
    : m_bLoaded(false)
{
}

ChangeTable::~ChangeTable()
{
    m_cc.Unload();
}

bool ChangeTable::bLoadFromFile(const char* pszChangeTablePath)
{
    m_bLoaded = m_cc.LoadTable(pszChangeTablePath);
    return m_bLoaded;
}

bool ChangeTable::bMakeChanges(const char* pszInput, int lenIn, char* pszOutput, int* lenOut)
{
    if (!m_bLoaded)
        return false;
    return m_cc.ProcessBuffer(pszInput, lenIn, pszOutput, lenOut);
}

bool ChangeTable::bMakeChanges(const char* pszInPath, const char* pszOutPath)
{
    if (!m_bLoaded)
        return false;

    std::ifstream ifs(pszInPath, std::ios::binary);
    if (!ifs) return false;

    std::vector<char> inBuf((std::istreambuf_iterator<char>(ifs)), {});
    inBuf.push_back('\0');

    char outBuf[65536];
    int outLen = sizeof(outBuf);

    if (!bMakeChanges(inBuf.data(), (int)inBuf.size(), outBuf, &outLen))
        return false;

    std::ofstream ofs(pszOutPath, std::ios::binary);
    if (!ofs) return false;

    ofs.write(outBuf, outLen);
    return true;
}
