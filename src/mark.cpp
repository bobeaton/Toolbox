// Mark.cpp  Mark's Shw debugging/development code  (1995-05-24)

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "mainfrm.h"  // class CMainFrame
#include "shwdoc.h"  // class CShwDoc
#include "shwview.h"  // class CShwView

#include "project.h"  // Shw_pProject()
#include "kmn.h"  // class CKeyboardManager


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

void CMainFrame::OnDebugMark()
{
	Str8 sMessage;
	Str8 sMessage2 = "\n\n "; // 1.5.0fs Move lock level meaning info to help file
//	sMessage2 = sMessage2 + "Level 0 is no lock." + "\n";
//	sMessage2 = sMessage2 + "Level 1 is the same as turning on Project, Lock Project." + "\n";
//	sMessage2 = sMessage2 + "Level 2 removes the Project menu." + "\n";
//	sMessage2 = sMessage2 + "Level 3 removes the Tools menu." + "\n";
//	sMessage2 = sMessage2 + "Level 4 removes the View menu." + "\n";
//	sMessage2 = sMessage2 + "Level 5 removes the Window menu." + "\n";
//	sMessage2 = sMessage2 + "Level 6 prevents changing size and position of windows." + "\n";
//	sMessage2 = sMessage2 + "Level 7 removes the Checks menu." + "\n";
//	sMessage2 = sMessage2 + "Levels 8 to 9 make no further changes." + "\n";
//	sMessage2 = sMessage2 + "Level 10 removes the File menu and prevents all changes to data or settings." + "\n";
//	sMessage2 = sMessage2 + "   " + "This makes Toolbox act as a file viewer." + "\n\n";
	sMessage2 = sMessage2 + _("Yes increases lock level.") + "\n";
	sMessage2 = sMessage2 + _("No sets level to 0.") + "\n";
	sMessage2 = sMessage2 + _("Cancel keeps current level.") + "\n";
	sMessage2 = sMessage2 + _("Changed lock level will apply the next time the project is opened.");
	int iLockProjectLevel = Shw_papp()->pProject()->iLockProject(); // 1.2bz Make ctrl+alt+shift+M change project lock level
	int iLockProjectOriginal = iLockProjectLevel; // 1.2ks Remember original lock level
	int iAnswer = 0;
	while( TRUE ) 
		{
		char buffer[20];
		_itoa_s(iLockProjectLevel, buffer, (int)sizeof(buffer), 10);
		sMessage = _("Project lock level:"); // 1.5.0fg 
		sMessage = sMessage + " " + buffer; // 1.5.0fg 
		sMessage += sMessage2;
		iAnswer = AfxMessageBox( sMessage, MB_YESNOCANCEL );
		if ( iAnswer == IDYES )
			{
			if ( iLockProjectLevel < 10 ) // 1.2ks Don't allow lock level above 10
				iLockProjectLevel++;
			}
		else if ( iAnswer == IDNO )
			iLockProjectLevel = 0;
		else
			break;
		}
	if ( iLockProjectOriginal || iLockProjectLevel ) // 1.2ks If originally locked or locked now, call OnLock to lock (or unlock) data files
		Shw_papp()->OnLockProject(); // Lock data files
	if ( iLockProjectOriginal && iLockProjectLevel ) // 1.2ks If originally locked and still locked now, call OnLock to lock data files
		Shw_papp()->OnLockProject(); // Lock data files
	Shw_papp()->pProject()->SetLockProject( iLockProjectLevel );
}
