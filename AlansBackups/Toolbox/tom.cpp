// Tom.cpp  Tom's Shw debugging/development code  (1999-01-27)
// 5.99f ALB Changed to set or clear bExerciseNoSave in project file

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "mainfrm.h"  // class CMainFrame
#include "shwdoc.h"  // class CShwDoc
#include "shwview.h"  // class CShwView
#include "project.h"

void CMainFrame::OnExerciseToggle()
{
	CProject* pprj = Shw_papp()->pProject();
	if ( pprj->bExerciseNoSave() ) // 5.99f Toggle no save setting
		{
		pprj->SetExerciseNoSave( FALSE );
		AfxMessageBox( "Exercise off" );
		}
	else
		{
		pprj->SetExerciseNoSave( TRUE );
		AfxMessageBox( _("Exercise on. No files will be saved.") );
		}
	pprj->SaveSettings( TRUE ); // 5.99f Save project file with new exercise setting
}

void CShwView::OnExerciseToggle()
{
	CProject* pprj = Shw_papp()->pProject();
	if ( pprj->bExerciseNoSave() ) // 5.99f Toggle no save setting
		{
		pprj->SetExerciseNoSave( FALSE );
		pprj->SaveSettings( TRUE ); // 1.4ta Fix bug of not saving paths in db on moved exercise
		AfxMessageBox( _("Exercise off") );
		}
	else
		{
		Shw_app().bSaveAllFiles( FALSE ); // 1.2mt Turn off write protect of data files when exercise setting is turned on // 5.99w Save all before turning on exercise setting
		pprj->SetExerciseNoSave( TRUE );
		AfxMessageBox( _("Exercise on. No files will be saved.") );
		}
	pprj->SaveSettings( TRUE ); // 5.99f Save project file with new exercise setting
}

void CShwView::OnExerciseUpdate() // 1.2mu Add shortcut Ctrl+ALT+E to update exercise project // 1.3as Change update exercise to Ctrl+Shift+S
	{
	CProject* pprj = Shw_papp()->pProject();
	if ( pprj->bExerciseNoSave() ) // 1.3at Update exercise project, don't turn on exercise if not already on
		{
		pprj->SetExerciseNoSave( FALSE ); // 1.2mu Turn off exercise so files can be saved
		Shw_app().bSaveAllFiles( FALSE ); // 1.2mu Save all files to update exercise projectg
		pprj->SaveSettings( TRUE ); // 1.4ta Fix bug of not saving paths in db on moved exercise
		pprj->SetExerciseNoSave( TRUE ); // 1.2mu Turn exercise back on
		AfxMessageBox( _("Exercise updated and on.") ); // 1.2mu Tell user exercise has been updated
		pprj->SaveSettings( TRUE ); // 1.2mv Make exercise update clear write protect of project file
		}
	}
