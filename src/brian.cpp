// Brian.cpp  Brian's Shw debugging/development code  (1995-05-24)
// 5.98s ALB Changed to write log of all files active in currently open project
// This is to help bug reporters send only the active files
// Also can help consultants see what path every file is opened from

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "shwdoc.h"
#include "shwview.h"
#include "project.h"
#include "tools.h"
#include "doclist.h"
#include <fstream.h>

extern Str8 g_sVersion;
extern void WriteAllPaths( BOOL bMessage );

void BugReport()
{
	WriteAllPaths( TRUE ); // Write all active paths into ProjectFilesList.txt file
}

void WriteAllPaths( BOOL bMessage )
{
	// 5.98s Output a list of all files active in currently open project
	CProject* pprj = Shw_papp()->pProject();
    Str8 sProjectPath;
    sProjectPath = pprj->pszPath();
    Str8 sProjectFileFolder = sGetDirPath(sProjectPath);
	Str8 sProjectFileName = sGetFileName( sProjectPath, TRUE );
	Str8 sProjectListFilePath = sProjectFileFolder + "ProjectFilesList.txt";
	Str8 sMessage = _("Writing file with list of all files active in current project:"); // 1.5.0fj 
	sMessage = sMessage + " " + sProjectListFilePath; // 1.5.0fj 
	if ( bMessage )
		AfxMessageBox( sMessage );

	ofstream ostr( sProjectListFilePath );
	ostr << "Toolbox" << " " << g_sVersion << "\n";
	ostr << _("List of all files active in project:") << " ";  // 1.5.0ft 
	ostr << sProjectFileName << "\n";
	CTime t = CTime::GetCurrentTime();
	Str8 sOut = sUTF8( t.Format( "%A, %B %d, %Y, %H:%M" ) ); // 1.4qpz Fix time output Format call for Unicode
	ostr << sOut << "\n\n";
	ostr << sProjectPath << "\n";

    CDatabaseTypeSet* ptypset = pprj->ptypset();
    for ( CDatabaseTypeProxy* ptrx = ptypset->ptrxFirst(); ptrx; ptrx = ptypset->ptrxNext(ptrx) )
		{
        if ( ptrx->bHasRefs() )
            {
			ostr << sProjectFileFolder << ptrx->sFileName() << "\n";
            CNoteList notlst;
            CDatabaseType* ptyp = ptrx->ptyp(notlst);
            ptyp->WritePaths( ostr );
			}
		}

    CLangEncSet* plngset = pprj->plngset();
    CLangEncProxy* plrx = plngset->plrxFirst();
    for ( ; plrx; plrx = plngset->plrxNext(plrx) )
        {
		if ( plrx->bHasRefs() )
			{
			ostr << plrx->sFullPathName() << "\n";
            CNoteList notlst;
            CLangEnc* plng = NULL;
			plng = plrx->plng(notlst);
            plng->WritePaths( ostr );

			}
		}
	
	ostr << "\n"; // Put blank line between settings and data files
	CDocList doclst; // get list of currently open documents
    for ( CShwDoc* pdoc = doclst.pdocFirst(); pdoc; pdoc = doclst.pdocNext() )
        ostr << sUTF8( pdoc->GetPathName() ) << "\n"; // 1.4qzpa Fix bug of bad write of file path in ProjectFilesList.txt

//	ostr << "\n" << sProjectListFilePath << "\n"; // 5.98t Include name of this file so they will copy it as well as the rest // 1.5.1ka Don't write ProjectFilesList.txt file name into list of active files

}

