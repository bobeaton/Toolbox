#pragma once
#include "stdafx.h"
#include <string>
#include "CCWrapper.h"

//
// Replacement for legacy ChangeTable that dynamically uses CC32.dll
//
class ChangeTable
{
public:
    ChangeTable();
    ~ChangeTable();

    bool bLoadFromFile(const char* pszChangeTablePath);
    bool bMakeChanges(const char* pszInput, int lenIn, char* pszOutput, int* lenOut);
    bool bMakeChanges(const char* pszInPath, const char* pszOutPath);

    bool bLoaded() const { return m_bLoaded; }

private:
    CCWrapper   m_cc;
    bool        m_bLoaded;
};
