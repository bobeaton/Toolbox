// Rod.cpp  Rod's Shw debugging/development code  (1995-05-24)

// 0.4.R.1.0	1996-01-12	Status bar -- use the field dialog font and constant size eDlgFontHeight.
// 0.3.R.2.0	1995-11-06	Major mods to incorporate CFilteredRecordSet functionality into
//							the CIndex class.  Also added new classes: CNumberedRecElPtr, CIndexPtr, etc.
// 0.3.R.1.0	1995-10-19	Modifications to CShwStatusBar -- override of OnUpdateCmdUI.
// 0.2.R.7.3	1995-10-04  Work complete on CShwStatusBar.
// 0.2.R.7.2	1995-10-02	Add CShwStatusBar class: prep for custom status bar.
// 0.2.R.7.1	1995-09-27	Combine the commonality of read and import.
//							Make status bar able to handle do-nothing situations.
// 0.2.R.7.0	1995-09-27	Changes and additions in preparation for Alpha 2.
// 0.2.R.6.1	1995-09-26  Have CFilterSet's "Use" checkbox dis/enabled on (non)empty filter set.
// 0.2.R.6.0	1995-09-21	Change filtering dialog to be two levels instead of three.
// 0.2.R.5.0	1995-09-15	Add CShwView::UpdateStatusBar() to update the status bar
//							Also added CShwApp::OnIdle() override.
// 0.2.R.4.0	1995-09-14  Change CIndex::bUsesMarker to search directly rather than use filter.
// 0.2.R.3.1	1995-09-14	Comprehensive tests of CIndex and CIndexSet.
// 0.2.R.3.0	1996-09-01	Enable general test of index through Ctrl-Alt-Shift-R
//							s_TestMFS_Simple(), s_TestMFS_Similarities(), complex later.
// 0.2.R.2.0	1995-08-21  Fix/change items according to MRP's 8/17/95 email.
// 0.2.R.1.0	1995-08-18  Change variable for writing \_no field to an unsigned short.
//							Change Sort Key &Length to Sort Key Le&ngth in Index sort dialog.
// 0.1.R.5.2	1995-08-07	Write out \_no field on output of Sh2.0 db.
// 0.1.R.5.1	1995-08-04	Bring in the CProgressBar class and dialog.
// 0.1.R.5.0	1995-08-01	Implement CSearchPath(List), put object in CShwDoc.
// 0.1.R.4.1	1995-07-27	CShwDoc update calls CIndex::bHasMarker()
//							CIndex::bHasMarker() filters for the presence of a marker.
// 0.1.R.4.0	1995-07-26  Added CShwApp::SendUpdateToAllShwDocuments()
//							Added CShwDoc::Update()
// 0.1.R.3.1	1995-07-24	Add Clear, First/Last > to Sort Fields, hid "Make Owner"
// 0.1.R.3.0	1995-07-20	Add doubleclick to Sort Fields selection boxes.
// 0.1.R.2.0	1995-06-28  Finished most functionality of and enabled the indices.
// 0.1.R.1.6	1995-06-23	Add CIndexList class, have CIndex track its references.
// 0.1.R.1.5    1995-06-22	Fix GPF bug on file import.  Index was not pre-created.
// 0.1.R.1.4	1995-06-22 	Fold COwningIndex into its base class, CIndex.
//							I've reflected this change thruout the project.
// 0.1.R.1.3 	1995-06-21 	Fix import unrecognized Sh file bug.
// 0.1.R.1.2 	1995-06-20 	Implement File-Output View dialog. 
// 0.1.R.1.1 	19 Jun 95  	Implement sort fields selection dialog class.

// (Modified by Josh to compile for Mac Debug)

#ifdef _DEBUG

#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "shwdoc.h"

#include "shwview.h"

void DebugRod(CFrameWnd* pFrameWnd)
{
#ifndef _MAC
	afxTraceEnabled = 1;
#endif  // _MAC
   	return;
}

#endif  // _DEBUG

