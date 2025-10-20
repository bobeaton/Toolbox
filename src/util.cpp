// util.cpp

#include "all.h"

void MyAssertFailedLine(char* pszFileName, int nLine)
{
    char pszLine[11];
    _itoa( nLine, pszLine, 10 ); // 1.4qua Upgrade wsprintf for Unicode build
    Str8 sFileName = sGetFileName( pszFileName );
    Str8 sMessage = "Internal error. "; // 1.5.0fg 
//	sMessage = sMessage + "Toolbox" + g_sVersion + " " + "File:" + " " + sFileName + " " + "Line:" + " " + pszLine; // 1.5.0fg 
	sMessage = sMessage + "File: " + sFileName + " " + "Line:" + " " + pszLine; // 1.5.0fg 
    AfxMessageBox(sMessage, MB_ICONEXCLAMATION);
}

Str8 sGetFileName( Str8 sFile )
{
int iSep = ReverseFind( FILESEPARATOR ); // Find rightmost separator
return Mid( iSep ); // If separator not found, returns whole string
}
