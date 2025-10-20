#include    "resource.h"

/////////////////////////////////////////////////////////////////////////
// WLM resources

#include    "ftab.r"
#include	"mrc\types.r"
#include	"mrc\balloons.r"
#include        "systypes.r"

/////////////////////////////////////////////////////////////////////////
// MFC resources

#include    "afxaete.r"


/////////////////////////////////////////////////////////////////////////
// Code Fragment resources

#include    "CodeFrag.r"


/////////////////////////////////////////////////////////////////////////
// Shm resources

resource 'SIZE' (-1)
{
	reserved,
	acceptSuspendResumeEvents,
	reserved,
	canBackground,
	doesActivateOnFGSwitch,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreAppDiedEvents,
	is32BitCompatible,
	isHighLevelEventAware,
	localAndRemoteHLEvents,
	isStationeryAware,
	dontUseTextEditServices,
	reserved,
	reserved,
	reserved,
#ifdef _MPPC_
	10000 * 1024,  // Preferred memory size
	 7500 * 1024   // Minimum memory size
#else	// 68K Mac
#ifdef _DEBUG
	10000 * 1024,
	 7500 * 1024
#else
	 8000 * 1024,  // Preferred memory size
	 6000 * 1024   // Minimum memory size
#endif
#endif
};

resource 'vers' (1)
{
	0x01,
	0x00,
	final,
	0x00,
	verUS,
	"6.0",
#ifdef _MPPC_
	"Toolbox 6.0 for Power Macintosh, Copyright \251 1996-2001 SIL"
#else
	"Toolbox 6.0 for Macintosh, Copyright \251 1996-2001 SIL"
#endif
};

resource 'vers' (2)
{
	0x04,
	0x00,
	final,
	0x00,
	verUS,
	"6.0",
	"Field Linguist's Toolbox 6.0"
};

resource 'kind' (128)
{
	'SHBX',							// app signature
	0,          // verUS
	{
		'SHBX', "Shoebox Project",	// file type & name
		'TEXT', "Shoebox Database",
	}
};

resource 'open' (128)
{
	'SHBX',							// app signature
	{
		'SHBX'						// file types that app can open
	}
};

resource 'BNDL' (128)
{
	'SHBX',
	0,
	{
		'FREF',
		{
			0, 128,
			1, 129,
		},
		'ICN#',
		{
			0, IDR_MAINFRAME,
			1, IDR_SHWTYPE,
		}
	}
};

type 'SHBX' as 'STR ';
resource 'SHBX' (0)
{
	"Field Linguist's Toolbox 6.0, Copyright \251 1996-2001 SIL"
};

#ifdef _MPPC_
resource 'STR ' (500)
{
	"This is Field Linguist's Toolbox for Power Macintosh "
	"ported to the Macintosh using Microsoft VC++ Edition "
	"for the Apple Macintosh"
};
#else	// 68K Mac
resource 'STR ' (500)
{
	"This is Field Linguist's Toolbox for Macintosh "
	"ported to the Macintosh using Microsoft VC++ Edition "
	"for the Apple Macintosh"
};
#endif

resource 'FREF' (128)
{
	'APPL',
	0,
	""
};

resource 'FREF' (129)
{
	'SHBX',
	1,
	""
};

#ifdef _MPPC_
resource 'cfrg' (0) {
  {
    kPowerPC,
    kFullLib,
    kNoVersionNum,kNoVersionNum,
    0, 0,
    kIsApp,kOnDiskFlat,kZeroOffset,kWholeFork,
    ""
  }
};
#endif



