// progress.h  Progress indicator

#ifndef PROGRESS_H
#define PROGRESS_H

class CShwStatusBar;  // status.h

// Use a progress indicator whenever an operation iterates through
// an index and does significant processing at each element, or
// in any other case when there can be a lengthy wait when
// the amount of data is large.

// This class encapsulates access to the status bar, which does
// the real work. It avoids direct reference to the interface classes
// from data classes, in case we ever reuse them in other contexts. 

class CProgressIndicator  // Hungarian: prg
{
private:
//	CProgressDlg* pdlgCancel; // small dialog with Cancel button for cancelling current operation // 1.4ac Eliminate cancel button from progress indicator

public:
	CProgressIndicator(DWORD dwMaxRange, UINT idsMessage, 
			const char* pszMessageArg = NULL,
			BOOL  bCancelEnabled = FALSE, BOOL bShowPercent = TRUE); // 1.0af If no message, use empty string
		// Show the numeric progress guage and message on the status bar.
		// dwMaxRange corresponds to 100%
		// If the idsMessage resource string contains a %1 placeholder,
		// supply the contextual argument via pszMessageArg.
	CProgressIndicator(DWORD dwMaxRange, UINT idsMessage,
			const char* pszMessageArg1, const char* pszMessageArg2,
			BOOL  bCancelEnabled = FALSE, BOOL bShowPercent = TRUE);

	~CProgressIndicator();
		// Restore the status bar to its normal appearance.
	
	BOOL bUpdateProgress(DWORD dwPos, BOOL bHandleMessages=TRUE);
		// Call this function after every unit of processing.
		// Returns FALSE if the user has chosen Cancel [not yet implemented]

private:
	CShwStatusBar* m_pbar;
};  // class CProgressIndicator

#endif  // not PROGRESS_H
