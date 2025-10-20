// progress.cpp  Progress indicator

#include "stdafx.h"
#include "toolbox.h"
#include "progress.h"

#include "tools.h"

#include "status.h"  // class CShwStatusBar, Shw_pbarStatus


CProgressIndicator::CProgressIndicator(DWORD dwMaxRange, UINT idsMessage,
		const char* pszMessageArg, BOOL bCancelEnabled, BOOL bShowPercent)
{
	Str8 sMessage = ""; // 1.4ab Always use null string for message

	m_pbar = Shw_pbarStatus();
	m_pbar->InitializeProgress(dwMaxRange, sMessage, bShowPercent);
//	pdlgCancel = new CProgressDlg( bCancelEnabled ); // 1.4ac Eliminate cancel button from progress indicator
}

CProgressIndicator::CProgressIndicator(DWORD dwMaxRange, UINT idsMessage,
		const char* pszMessageArg1, const char* pszMessageArg2,
        BOOL bCancelEnabled, BOOL bShowPercent)
{
	Str8 sMessage = ""; // 1.4ab Always use null string for message

	m_pbar = Shw_pbarStatus();
	m_pbar->InitializeProgress(dwMaxRange, sMessage, bShowPercent);
//	pdlgCancel = new CProgressDlg( bCancelEnabled ); // 1.4ac Eliminate cancel button from progress indicator
}

CProgressIndicator::~CProgressIndicator()
{
	m_pbar->EndProgress();
//	pdlgCancel->Destroy(); // kill the cancel button dialog // 1.4ac Eliminate cancel button from progress indicator
//	delete pdlgCancel; // 1.4ac Eliminate cancel button from progress indicator
}

BOOL CProgressIndicator::bUpdateProgress(DWORD dwPos, BOOL bHandleMessages)
{
	m_pbar->UpdateProgress(dwPos);
	return TRUE; // 1.4ac Eliminate cancel button from progress indicator
}

// 1.4ab Eliminate message from progress indicator
