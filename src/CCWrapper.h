#pragma once
#include "stdafx.h"
#include <windows.h>
#include <string>
#include <stdexcept>

class CCWrapper
{
public:
    using CCLoadTableFunc = int(__stdcall*)(char*, void**, HINSTANCE);
    using CCUnloadTableFunc = int(__stdcall*)(void*);
    using CCProcessBufferFunc = int(__stdcall*)(void*, char*, int, char*, int*);
    using CCReinitFunc = int(__stdcall*)(void*);
    using CCSetErrorProcFunc = int(__stdcall*)(void*, void*);

    CCWrapper(const std::wstring& dllPath = L"CC32.dll")
        : m_hDll(nullptr),
        m_pLoad(nullptr),
        m_pUnload(nullptr),
        m_pProcess(nullptr),
        m_pReinit(nullptr),
        m_hTable(nullptr)
    {
        m_hDll = ::LoadLibraryW(dllPath.c_str());
        if (!m_hDll)
            throw std::runtime_error("Unable to load CC32.dll");

        m_pLoad = (CCLoadTableFunc)GetProcAddress(m_hDll, "CCLoadTable");
        m_pUnload = (CCUnloadTableFunc)GetProcAddress(m_hDll, "CCUnloadTable");
        m_pProcess = (CCProcessBufferFunc)GetProcAddress(m_hDll, "CCProcessBuffer");
        m_pReinit = (CCReinitFunc)GetProcAddress(m_hDll, "CCReinitializeTable");

        if (!m_pLoad || !m_pUnload || !m_pProcess)
            throw std::runtime_error("Missing expected exports in CC32.dll");
    }

    ~CCWrapper()
    {
        if (m_hTable && m_pUnload)
            m_pUnload(m_hTable);

        if (m_hDll)
            ::FreeLibrary(m_hDll);
    }

    bool LoadTable(const char* tablePath)
    {
        if (!m_pLoad) return false;
        void* hTable = nullptr;
        HINSTANCE hInst = (HINSTANCE)GetCurrentProcess();
        int result = m_pLoad((char*)tablePath, &hTable, hInst);
        if (result == 0 && hTable != nullptr)
        {
            m_hTable = hTable;
            return true;
        }
        return false;
    }

    bool ProcessBuffer(const char* input, int lenIn, char* output, int* lenOut)
    {
        if (!m_hTable || !m_pProcess) return false;
        return (m_pProcess(m_hTable, (char*)input, lenIn, output, lenOut) == 0);
    }

    void Unload()
    {
        if (m_hTable && m_pUnload)
        {
            m_pUnload(m_hTable);
            m_hTable = nullptr;
        }
    }

private:
    HMODULE m_hDll;
    void* m_hTable;
    CCLoadTableFunc     m_pLoad;
    CCUnloadTableFunc   m_pUnload;
    CCProcessBufferFunc m_pProcess;
    CCReinitFunc        m_pReinit;
};
