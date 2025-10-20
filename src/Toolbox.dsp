# Microsoft Developer Studio Project File - Name="shw32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=shw32 - Win32 MacWine Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Toolbox.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Toolbox.mak" CFG="shw32 - Win32 MacWine Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "shw32 - Win32 Debug Unicode" (based on "Win32 (x86) Application")
!MESSAGE "shw32 - Win32 Release Unicode" (based on "Win32 (x86) Application")
!MESSAGE "shw32 - Win32 MacWine Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CARLABox/Shw", XQEAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "shw32 - Win32 Debug Unicode"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "shw32___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "shw32___Win32_Debug_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_U"
# PROP Intermediate_Dir "Debug_U"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /ML /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NO_DUMP" /D "_MBCS" /D "OBSTREAM_NOTES" /FR /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# ADD CPP /nologo /Zp4 /ML /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NO_DUMP" /D "_MBCS" /D "OBSTREAM_NOTES" /D "_UNICODE" /FR /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WIN32"
# ADD RSC /l 0x409 /fo".\Debug_U/Shw.res" /d "_DEBUG" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o".\Debug_U/Toolbox.bsc"
LINK32=link.exe
# ADD BASE LINK32 cclib32.lib winmm.lib usp10.lib /nologo /version:0.4 /stack:0x2800 /subsystem:windows /map /debug /machine:IX86 /nodefaultlib:"libc"
# SUBTRACT BASE LINK32 /pdb:none /incremental:no /nodefaultlib
# ADD LINK32 Cclib32.lib winmm.lib usp10.lib /nologo /stack:0x2800 /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:".\Debug_U/Toolbox.pdb" /map:".\Debug_U/Toolbox.map" /debug /machine:IX86 /nodefaultlib:"libc" /out:".\Debug_U/Toolbox.exe"
# SUBTRACT LINK32 /pdb:none /incremental:no /nodefaultlib

!ELSEIF  "$(CFG)" == "shw32 - Win32 Release Unicode"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "shw32___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "shw32___Win32_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Release_U"
# PROP Intermediate_Dir ".\Release_U"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /ML /W3 /Gm /Gi /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "NO_DUMP" /D "_MBCS" /D "OBSTREAM_NOTES" /D "_UNICODE" /FR /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# ADD CPP /nologo /Zp4 /ML /W3 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NO_DUMP" /D "_MBCS" /D "OBSTREAM_NOTES" /D "_UNICODE" /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /fo".\Debug_wxU/Shw.res" /d "_DEBUG" /d "WIN32"
# ADD RSC /l 0x409 /d "NDEBUG" /d "WIN32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo /o".\Debug_wxU/Toolbox.bsc"
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 cclib32.lib winmm.lib usp10.lib /nologo /version:0.4 /stack:0x2800 /entry:"wWinMainCRTStartup" /subsystem:windows /pdb:".\Debug_wxU/Toolbox.pdb" /map:".\Debug_wxU/Toolbox.map" /debug /machine:IX86 /nodefaultlib:"libc" /out:".\Debug_wxU/Toolbox.exe"
# SUBTRACT BASE LINK32 /pdb:none /incremental:no /nodefaultlib
# ADD LINK32 Cclib32.lib winmm.lib usp10.lib /nologo /stack:0x2800 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /map /machine:IX86 /nodefaultlib:"libc"
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "shw32 - Win32 MacWine Release"

# PROP BASE Use_MFC 1
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "shw32___Win32_MacWine_Release"
# PROP BASE Intermediate_Dir "shw32___Win32_MacWine_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\MacWineRelease"
# PROP Intermediate_Dir ".\MacWineRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp4 /ML /W3 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NO_DUMP" /D "OBSTREAM_NOTES" /D "_UNICODE" /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /Zp4 /ML /W3 /Gi /GX /O1 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "NO_DUMP" /D "_MBCS" /D "OBSTREAM_NOTES" /D "_UNICODE" /D "MacWine" /Fp".\SHW32.PCH" /YX"stdafx.h" /Fd".\SHW32.PDB" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /mktyplib203 /win32
# ADD MTL /nologo /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "WIN32"
# ADD RSC /l 0x409 /d "NDEBUG" /d "WIN32" /d "MacWine"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Cclib32.lib winmm.lib usp10.lib /nologo /stack:0x2800 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /map /machine:IX86 /nodefaultlib:"libc"
# SUBTRACT BASE LINK32 /pdb:none /debug /nodefaultlib
# ADD LINK32 Cclib32.lib winmm.lib usp10.lib /nologo /stack:0x2800 /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:no /map /machine:IX86 /nodefaultlib:"libc" /out:".\MacWineRelease/ToolboxMac.exe"
# SUBTRACT LINK32 /pdb:none /debug /nodefaultlib

!ENDIF 

# Begin Target

# Name "shw32 - Win32 Debug Unicode"
# Name "shw32 - Win32 Release Unicode"
# Name "shw32 - Win32 MacWine Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\Alan.cpp
# End Source File
# Begin Source File

SOURCE=.\Ambig_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Brian.cpp
# End Source File
# Begin Source File

SOURCE=.\browse.CPP
# End Source File
# Begin Source File

SOURCE=.\browse_d.CPP
# End Source File
# Begin Source File

SOURCE=.\Cbo.cpp
# End Source File
# Begin Source File

SOURCE=.\cck.cpp
# End Source File
# Begin Source File

SOURCE=.\cck_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Cct.cpp
# End Source File
# Begin Source File

SOURCE=.\Cdblel.cpp
# End Source File
# Begin Source File

SOURCE=.\Cdbllist.cpp
# End Source File
# Begin Source File

SOURCE=.\Cfield.cpp
# End Source File
# Begin Source File

SOURCE=.\CorGuess.cpp
# End Source File
# Begin Source File

SOURCE=.\Corpus.cpp
# End Source File
# Begin Source File

SOURCE=.\Corpus_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Crecord.cpp
# End Source File
# Begin Source File

SOURCE=.\crecpos.cpp
# End Source File
# Begin Source File

SOURCE=.\crlookel.cpp
# End Source File
# Begin Source File

SOURCE=.\Crngset.cpp
# End Source File
# Begin Source File

SOURCE=.\CRngSt_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Date.cpp
# End Source File
# Begin Source File

SOURCE=.\dbref.cpp
# End Source File
# Begin Source File

SOURCE=.\Ddx.cpp
# End Source File
# Begin Source File

SOURCE=.\dirlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgimpt.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgrcins.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlgsort.cpp
# End Source File
# Begin Source File

SOURCE=.\Doclist.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLng.cpp
# End Source File
# Begin Source File

SOURCE=.\Edlngctl.cpp
# End Source File
# Begin Source File

SOURCE=.\Export.cpp
# End Source File
# Begin Source File

SOURCE=.\Export_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Fil.cpp
# End Source File
# Begin Source File

SOURCE=.\Fil_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Filsel_d.cpp
# End Source File
# Begin Source File

SOURCE=.\find_d.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\hide_d.cpp
# End Source File
# Begin Source File

SOURCE=.\html.cpp
# End Source File
# Begin Source File

SOURCE=.\HugeStr.cpp
# End Source File
# Begin Source File

SOURCE=.\Ind.cpp
# End Source File
# Begin Source File

SOURCE=.\Interlin.cpp
# End Source File
# Begin Source File

SOURCE=.\Intprc_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Intsetup.cpp
# End Source File
# Begin Source File

SOURCE=.\jmp.cpp
# End Source File
# Begin Source File

SOURCE=.\Jumpdlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Karen.cpp
# End Source File
# Begin Source File

SOURCE=.\Kbd.cpp
# End Source File
# Begin Source File

SOURCE=.\Key_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Kmn.cpp
# End Source File
# Begin Source File

SOURCE=.\Lbo.cpp
# End Source File
# Begin Source File

SOURCE=.\ListEntr.cpp
# End Source File
# Begin Source File

SOURCE=.\Lng.cpp
# End Source File
# Begin Source File

SOURCE=.\Lng_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Loc.cpp
# End Source File
# Begin Source File

SOURCE=.\Lookup.cpp
# End Source File
# Begin Source File

SOURCE=.\Mac.rc
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Mark.cpp
# End Source File
# Begin Source File

SOURCE=.\Mdf.cpp
# End Source File
# Begin Source File

SOURCE=.\Mdf_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Mkr.cpp
# End Source File
# Begin Source File

SOURCE=.\Mkr_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Mkrfed_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Mkrset_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Mrfset_d.cpp
# End Source File
# Begin Source File

SOURCE=.\multmat.cpp
# End Source File
# Begin Source File

SOURCE=.\Newdoc_d.cpp
# End Source File
# Begin Source File

SOURCE=.\nlstream.cpp
# End Source File
# Begin Source File

SOURCE=.\Nomat_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Not.cpp
# End Source File
# Begin Source File

SOURCE=.\number.cpp
# End Source File
# Begin Source File

SOURCE=.\number_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Obstream.cpp
# End Source File
# Begin Source File

SOURCE=.\Page_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Patch.cpp
# End Source File
# Begin Source File

SOURCE=.\Patmch.cpp
# End Source File
# Begin Source File

SOURCE=.\Pgs.cpp
# End Source File
# Begin Source File

SOURCE=.\Pgs_d.cpp
# End Source File
# Begin Source File

SOURCE=.\portable.cpp
# End Source File
# Begin Source File

SOURCE=.\Printinf.cpp
# End Source File
# Begin Source File

SOURCE=.\Progbar.cpp
# End Source File
# Begin Source File

SOURCE=.\Progress.cpp
# End Source File
# Begin Source File

SOURCE=.\project.cpp
# End Source File
# Begin Source File

SOURCE=.\Recset.cpp
# End Source File
# Begin Source File

SOURCE=.\Ref.cpp
# End Source File
# Begin Source File

SOURCE=.\Rod.cpp
# End Source File
# Begin Source File

SOURCE=.\Rtf.cpp
# End Source File
# Begin Source File

SOURCE=.\Rundos.cpp
# End Source File
# Begin Source File

SOURCE=.\Seldb_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Set.cpp
# End Source File
# Begin Source File

SOURCE=.\Set_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Sfstream.cpp
# End Source File
# Begin Source File

SOURCE=.\Shw.cpp
# End Source File
# Begin Source File

SOURCE=.\Shw.rc
# End Source File
# Begin Source File

SOURCE=.\Shw32.def
# End Source File
# Begin Source File

SOURCE=.\Shwdio.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwdoc.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwdtplt.cpp
# End Source File
# Begin Source File

SOURCE=.\shwmac.r
# End Source File
# Begin Source File

SOURCE=.\shwvbrs.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwvdat.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwvdis.cpp
# End Source File
# Begin Source File

SOURCE=.\shwvfind.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwview.cpp
# End Source File
# Begin Source File

SOURCE=.\Shwvprnt.cpp
# End Source File
# Begin Source File

SOURCE=.\shwvscrl.cpp
# End Source File
# Begin Source File

SOURCE=.\shwvundo.cpp
# End Source File
# Begin Source File

SOURCE=.\shwvwrap.cpp
# End Source File
# Begin Source File

SOURCE=.\Spath_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Srt.cpp
# End Source File
# Begin Source File

SOURCE=.\Srt_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Srtset_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Status.cpp
# End Source File
# Begin Source File

SOURCE=.\StdioFil.cpp
# End Source File
# Begin Source File

SOURCE=.\Str8.cpp
# End Source File
# Begin Source File

SOURCE=.\StringEx.CPP
# End Source File
# Begin Source File

SOURCE=.\Subfield.cpp
# End Source File
# Begin Source File

SOURCE=.\tom.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\tplt_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Trie.cpp
# End Source File
# Begin Source File

SOURCE=.\Typ.cpp
# End Source File
# Begin Source File

SOURCE=.\Typ_d.cpp
# End Source File
# Begin Source File

SOURCE=.\Update.cpp
# End Source File
# Begin Source File

SOURCE=.\Var.cpp
# End Source File
# Begin Source File

SOURCE=.\Var_d.cpp
# End Source File
# Begin Source File

SOURCE=.\VersDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Wav.cpp
# End Source File
# Begin Source File

SOURCE=.\wdf.cpp
# End Source File
# Begin Source File

SOURCE=.\wdf_d.cpp
# End Source File
# Begin Source File

SOURCE=.\wrdlst_d.cpp
# End Source File
# Begin Source File

SOURCE=.\xml.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\alan.h
# End Source File
# Begin Source File

SOURCE=.\all.h
# End Source File
# Begin Source File

SOURCE=.\Ambig_d.h
# End Source File
# Begin Source File

SOURCE=.\Booklist.h
# End Source File
# Begin Source File

SOURCE=.\Browse.h
# End Source File
# Begin Source File

SOURCE=.\Browse_d.h
# End Source File
# Begin Source File

SOURCE=.\Cbo.h
# End Source File
# Begin Source File

SOURCE=.\Ccdll.h
# End Source File
# Begin Source File

SOURCE=.\cck.h
# End Source File
# Begin Source File

SOURCE=.\cck_d.h
# End Source File
# Begin Source File

SOURCE=.\Cct.h
# End Source File
# Begin Source File

SOURCE=.\Cdblel.h
# End Source File
# Begin Source File

SOURCE=.\Cdbllist.h
# End Source File
# Begin Source File

SOURCE=.\Cfield.h
# End Source File
# Begin Source File

SOURCE=.\Charset.h
# End Source File
# Begin Source File

SOURCE=.\Corpus.h
# End Source File
# Begin Source File

SOURCE=.\Corpus_d.h
# End Source File
# Begin Source File

SOURCE=.\CorrespGuess.h
# End Source File
# Begin Source File

SOURCE=.\Crecord.h
# End Source File
# Begin Source File

SOURCE=.\Crecpos.h
# End Source File
# Begin Source File

SOURCE=.\Crlookel.h
# End Source File
# Begin Source File

SOURCE=.\Crngset.h
# End Source File
# Begin Source File

SOURCE=.\Crngst_d.h
# End Source File
# Begin Source File

SOURCE=.\Date.h
# End Source File
# Begin Source File

SOURCE=.\dbmac.h
# End Source File
# Begin Source File

SOURCE=.\Dbref.h
# End Source File
# Begin Source File

SOURCE=.\dbtypmac.h
# End Source File
# Begin Source File

SOURCE=.\Ddx.h
# End Source File
# Begin Source File

SOURCE=.\Dirlist.h
# End Source File
# Begin Source File

SOURCE=.\Dlgimpt.h
# End Source File
# Begin Source File

SOURCE=.\Dlgrcins.h
# End Source File
# Begin Source File

SOURCE=.\Dlgsort.h
# End Source File
# Begin Source File

SOURCE=.\Doclist.h
# End Source File
# Begin Source File

SOURCE=.\EditLng.h
# End Source File
# Begin Source File

SOURCE=.\Edlngctl.h
# End Source File
# Begin Source File

SOURCE=.\Export.h
# End Source File
# Begin Source File

SOURCE=.\Export_d.h
# End Source File
# Begin Source File

SOURCE=.\Fil.h
# End Source File
# Begin Source File

SOURCE=.\Fil_d.h
# End Source File
# Begin Source File

SOURCE=.\Filsel_d.h
# End Source File
# Begin Source File

SOURCE=.\Find_d.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\generic.h
# End Source File
# Begin Source File

SOURCE=.\Hint.h
# End Source File
# Begin Source File

SOURCE=.\Hints.h
# End Source File
# Begin Source File

SOURCE=.\Hugestr.h
# End Source File
# Begin Source File

SOURCE=.\Ind.h
# End Source File
# Begin Source File

SOURCE=.\Interlin.h
# End Source File
# Begin Source File

SOURCE=.\Intprc_d.h
# End Source File
# Begin Source File

SOURCE=.\Intsetup.h
# End Source File
# Begin Source File

SOURCE=.\jmp.h
# End Source File
# Begin Source File

SOURCE=.\Jumpdlg.h
# End Source File
# Begin Source File

SOURCE=.\Kbd.h
# End Source File
# Begin Source File

SOURCE=.\Key_d.h
# End Source File
# Begin Source File

SOURCE=.\Keyman5.h
# End Source File
# Begin Source File

SOURCE=.\Kmn.h
# End Source File
# Begin Source File

SOURCE=.\LangEdit.h
# End Source File
# Begin Source File

SOURCE=.\Lbo.h
# End Source File
# Begin Source File

SOURCE=.\Listentr.h
# End Source File
# Begin Source File

SOURCE=.\Lng.h
# End Source File
# Begin Source File

SOURCE=.\Lng_d.h
# End Source File
# Begin Source File

SOURCE=.\LngEdCtl.h
# End Source File
# Begin Source File

SOURCE=.\Loc.h
# End Source File
# Begin Source File

SOURCE=.\Lookup.h
# End Source File
# Begin Source File

SOURCE=.\Mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\Mdf.h
# End Source File
# Begin Source File

SOURCE=.\Mdf_d.h
# End Source File
# Begin Source File

SOURCE=.\Mkr.h
# End Source File
# Begin Source File

SOURCE=.\Mkr_d.h
# End Source File
# Begin Source File

SOURCE=.\Mkrfed_d.h
# End Source File
# Begin Source File

SOURCE=.\Mkrset_d.h
# End Source File
# Begin Source File

SOURCE=.\Mrfset_d.h
# End Source File
# Begin Source File

SOURCE=.\Multmat.h
# End Source File
# Begin Source File

SOURCE=.\Newdoc_d.h
# End Source File
# Begin Source File

SOURCE=.\Nlstream.h
# End Source File
# Begin Source File

SOURCE=.\Nomat_d.h
# End Source File
# Begin Source File

SOURCE=.\Not.h
# End Source File
# Begin Source File

SOURCE=.\number.h
# End Source File
# Begin Source File

SOURCE=.\number_d.h
# End Source File
# Begin Source File

SOURCE=.\Obstream.h
# End Source File
# Begin Source File

SOURCE=.\Page_d.h
# End Source File
# Begin Source File

SOURCE=.\Patch.h
# End Source File
# Begin Source File

SOURCE=.\Patmch.h
# End Source File
# Begin Source File

SOURCE=.\Pgs.h
# End Source File
# Begin Source File

SOURCE=.\Pgs_d.h
# End Source File
# Begin Source File

SOURCE=.\portable.h
# End Source File
# Begin Source File

SOURCE=.\Printinf.h
# End Source File
# Begin Source File

SOURCE=.\Prog_d.h
# End Source File
# Begin Source File

SOURCE=.\Progbar.h
# End Source File
# Begin Source File

SOURCE=.\Progbar.inl
# End Source File
# Begin Source File

SOURCE=.\Progress.h
# End Source File
# Begin Source File

SOURCE=.\Project.h
# End Source File
# Begin Source File

SOURCE=.\Ptr.h
# End Source File
# Begin Source File

SOURCE=.\Recset.h
# End Source File
# Begin Source File

SOURCE=.\Ref.h
# End Source File
# Begin Source File

SOURCE=.\Rtf.h
# End Source File
# Begin Source File

SOURCE=.\Rundos.h
# End Source File
# Begin Source File

SOURCE=.\Seldb_d.h
# End Source File
# Begin Source File

SOURCE=.\Set.h
# End Source File
# Begin Source File

SOURCE=.\Set_d.h
# End Source File
# Begin Source File

SOURCE=.\Sfstream.h
# End Source File
# Begin Source File

SOURCE=.\Shw.h
# End Source File
# Begin Source File

SOURCE=.\Shwdefs.h
# End Source File
# Begin Source File

SOURCE=.\Shwdoc.h
# End Source File
# Begin Source File

SOURCE=.\Shwdtplt.h
# End Source File
# Begin Source File

SOURCE=.\Shwview.h
# End Source File
# Begin Source File

SOURCE=.\Shwvscrl.h
# End Source File
# Begin Source File

SOURCE=.\Spath_d.h
# End Source File
# Begin Source File

SOURCE=.\Srt.h
# End Source File
# Begin Source File

SOURCE=.\Srt_d.h
# End Source File
# Begin Source File

SOURCE=.\Srtset_d.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\Stdafx.h
# End Source File
# Begin Source File

SOURCE=.\Stdiofil.h
# End Source File
# Begin Source File

SOURCE=.\Str8.h
# End Source File
# Begin Source File

SOURCE=.\Stringex.h
# End Source File
# Begin Source File

SOURCE=.\Subfield.h
# End Source File
# Begin Source File

SOURCE=.\toolbox.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Tplt_d.h
# End Source File
# Begin Source File

SOURCE=.\Trie.h
# End Source File
# Begin Source File

SOURCE=.\Typ.h
# End Source File
# Begin Source File

SOURCE=.\Typ_d.h
# End Source File
# Begin Source File

SOURCE=.\Undo.h
# End Source File
# Begin Source File

SOURCE=.\Update.h
# End Source File
# Begin Source File

SOURCE=.\Var.h
# End Source File
# Begin Source File

SOURCE=.\Var_d.h
# End Source File
# Begin Source File

SOURCE=.\Wav.h
# End Source File
# Begin Source File

SOURCE=.\wdf.h
# End Source File
# Begin Source File

SOURCE=.\wdf_d.h
# End Source File
# Begin Source File

SOURCE=.\Wordlist.h
# End Source File
# Begin Source File

SOURCE=.\Wrdlst_d.h
# End Source File
# Begin Source File

SOURCE=.\xml.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Exclamat.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Idr_shwt.bmp
# End Source File
# Begin Source File

SOURCE=.\Res\Rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Rightmac.cur
# End Source File
# Begin Source File

SOURCE=.\Res\Shw.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Shw.rc2
# End Source File
# Begin Source File

SOURCE=.\Res\Shwdoc.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Shwnotes.rc
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Target
# End Project
