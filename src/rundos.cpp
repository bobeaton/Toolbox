/////////////////////////////////////////////////////////////////////////////
// rundos.cpp
// The CRunDos class
// This class manages a dialog the has the ability to run any dos program.
// Author: Doug Eberman
// copyright 1997 JAARS Inc. SIL
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "toolbox.h"
#include "shw.h"
#include "rundos.h"
#include "tools.h"
#include "obstream.h"  // Object_istream, Object_ostream

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif


static const char* psz_rundos  = "rundos";  
static const char* psz_command = "command";


/////////////////////////////////////////////////////////////////////////////
// CRunDos dialog


CRunDos::CRunDos(CWnd* pParent /*=NULL*/)
	: CDialog(CRunDos::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRunDos)
	//}}AFX_DATA_INIT
	
	m_isRunning = FALSE;

	for (int i=0; i<MAX_COMMAND_MEMORY; i++) {
		m_apStrCommands[i] = new Str8;
	}
}

CRunDos::~CRunDos()
{
	for (int i=0; i<MAX_COMMAND_MEMORY; i++) {
		if (m_apStrCommands[i]) {
			delete m_apStrCommands[i];
		}
	}
}

void CRunDos::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunDos)
	DDX_Control(pDX, IDC_COMBO1, m_CommandList);  
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRunDos, CDialog)
	//{{AFX_MSG_MAP(CRunDos)
	ON_BN_CLICKED(ID_RUNDOS, OnRundos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static BOOL bFirstTime = TRUE;
/*
 *  CRunDos::OnRundos - user clicked run - run a dos command or batch file.
 * Used by Tools, Run Batch File command.
 * Written by Doug Eberman Aug 97
 */
void CRunDos::OnRundos()
{
#ifndef _MAC
	if ( m_isRunning ) // If already running, don't do another (ALB should never happen, since the box is modal)
		return;

    if ( !Shw_app().bSaveAllFiles( FALSE ) ) // 1.2gz Unwrite protect data files when running batch file // Save all files. If any not valid, let user fix.
		return;

    UpdateData();
	Str8 str;
	CString swCommand;  // 1.4qpx
	int idx = m_CommandList.GetCurSel();
	if ( idx == CB_ERR ) // If no selection, look at the edit box text.
		m_CommandList.GetWindowText( swCommand ); // 1.4qpx
	else	// Else look at the list box.
		m_CommandList.GetLBText( idx, swCommand ); // 1.4qpx
	str = sUTF8( swCommand );  // 1.4qpx
	if ( str.IsEmpty() ) // If no command given, don't do anything
		return;

    m_isRunning = TRUE;  
	Str8 sPrev; // Previous command for comparison
	if ( m_CommandList.GetCount() > 0 ) // 1.4whe Fix U bug of crash on run batch file
		{
		m_CommandList.GetLBText( 0, swCommand ); // 1.4qpx
		sPrev = sUTF8( swCommand ); // 1.4qpx
		}
	if ( !( str == sPrev ) ) // If different from previous command, remember it.
		{
		m_CommandList.InsertString( 0, swUTF16( str  ) ); // 1.4qre
		MemorizeCommand( str );
		}
	//	RDE: for Win2000, command.com doesn't work (nicely). For example, it doesn't allow batch files which use long // 1.4sb
	//	filenames. Therefore, determine if this is 2K/NT and if so, use CMD.exe instead.
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx (&osvi);
	Str8 strCmd;
	if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT )
		strCmd = "Cmd.Exe /C ";
	else
		strCmd = "COMMAND.COM /C ";
	strCmd += str;
	bWinExecWait( strCmd, SW_SHOW ); // 1.4qzfs
	m_didCommand = TRUE;
	m_isRunning = FALSE;
#endif  // ifndef _MAC
    CDialog::OnOK();
}

/*
 *  CRunDos::MemorizeCommand - memorize a command for the command list.
 */
void CRunDos::MemorizeCommand( const Str8 & str )
{
	for ( int i=( MAX_COMMAND_MEMORY-1 ); i>0; i-- )
		*m_apStrCommands[ i ] = *m_apStrCommands[ i - 1 ];
	*m_apStrCommands[ 0 ] = str;
}

/*
 *  CRunDos::OnInitDialog - setup the Combobox during dialog initialization.
 */
BOOL CRunDos::OnInitDialog()
{
    CDialog::OnInitDialog();
    
    m_didCommand = FALSE;
    
    for ( int i = 0; i < MAX_COMMAND_MEMORY; i++ )
		{
    	if ( !m_apStrCommands[ i ]->IsEmpty())
			{
			Str8 s = *m_apStrCommands[ i ]; // 1.4quk
			CString sw = swUTF16( s ); // 1.4quk
    		m_CommandList.AddString( sw ); // 1.4quk Upgrade AddString for Unicode build
			}
		}                                                                    
    int len = m_apStrCommands[ 0 ]->GetLength();
    m_CommandList.SetWindowText(  swUTF16( *m_apStrCommands[ 0 ] ) ); // ->GetBuffr( len + 1 ) ) ); // 1.4qpy // 1.4qzfu Eliminate use of GetBuffr
//    m_apStrCommands[ 0 ]->ReleaseBuffer(); // 1.4qzfu
    
	return FALSE;  // return TRUE  unless you set the focus to a control
}

/*
 * CRunDos::bReadProperites - read in all the data we need to give this
 * class a persistant memory, from the project file.
 */
BOOL CRunDos::bReadProperties(Object_istream& obs, const char* pszQualifier)
{       
    if (!obs.bReadBeginMarkerWithQualifier(psz_rundos, pszQualifier))
        return FALSE;

	int i=0;                    
    while ( !obs.bAtEnd() ) {
    	// While more in file
        Str8 strCommand;
    	if ( obs.bReadString(psz_command, strCommand ) ) {
	        if (i < MAX_COMMAND_MEMORY) {
	        	*m_apStrCommands[i++] = strCommand;
	        }
        } else if ( obs.bEndWithQualifier( psz_rundos, pszQualifier ) ) {
        	// If end marker or erroneous begin break
            break;
        }
    }   
        
    return TRUE;
}

/*
 *  CRunDos::WriteProperties - write out all member variables needed to give
 *  this class a persistant memory.  Use the project file.
 */
#ifdef prjWritefstream // 1.6.4aa 
void CRunDos::WriteProperties(Object_ofstream& obs, const char* pszQualifier) const 
#else
void CRunDos::WriteProperties(Object_ostream& obs, const char* pszQualifier) const 
#endif
{
	obs.WriteBeginMarkerWithQualifier(psz_rundos, pszQualifier);
	
	for (int i=0;i < MAX_COMMAND_MEMORY; i++) {
        if (!m_apStrCommands[i]->IsEmpty()) {
        	int len = m_apStrCommands[i]->GetLength();
	        obs.WriteString(psz_command, *m_apStrCommands[i]); // ->GetBuffr(len+1)); // 1.4qzft Eliminate use of GetBuffr
//	    	m_apStrCommands[i]->ReleaseBuffer(); // 1.4qzft
		}
    }
    
	obs.WriteEndMarkerWithQualifier(psz_rundos, pszQualifier);
}

