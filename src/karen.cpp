// Karen.cpp  Karen's Shw debugging/development code  (1995-05-24)

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "project.h"
#include "alan.h"
#include "interlin.h"
#include "shwview.h" 

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

BOOL bDebugKaren = FALSE;

void CShwView::OnDebugKaren() // 1.4bk Allow Ctrl+Alt+Shift+K to keep chained export files
{
	bDebugKaren = TRUE;
	AfxMessageBox( "Keep temporary files turned on" );
}
