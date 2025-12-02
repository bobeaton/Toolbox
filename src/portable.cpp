// portable.cpp Portability functions

#ifdef wxTbx
#include "wx/wxprec.h" // For compilers that support precompilation, includes "wx/wx.h".
#else
#include "stdafx.h"
#include <afxpriv.h> // 1.5.0gd For USES_CONVERSION
#include <atlconv.h>
#endif

#include "portable.h"

int TbxMessageBox( const char* psz, UINT nType )
	{
#ifdef wxTbx
	wxString wxsMsg = swUTF16( psz );
	msg wxMessageDialog( NULL, wxMsg );
	int iRet = msg.ShowModal();
#else
	USES_CONVERSION; // 1.5.0gd 
	return AfxMessageBox( A2CW( psz ), nType ); // 1.5.0gd Make message box use code page, for translatability
#endif
	return 1;
	}
