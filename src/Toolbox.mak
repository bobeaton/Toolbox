# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Power Macintosh Application" 0x0401

!IF "$(CFG)" == ""
CFG=Toolbox - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Toolbox - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Toolbox - Win32 Release" && "$(CFG)" !=\
 "Toolbox - Win32 Debug" && "$(CFG)" != "Toolbox - Power Macintosh Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "Toolbox.mak" CFG="Toolbox - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Toolbox - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Toolbox - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "Toolbox - Power Macintosh Release" (based on\
 "Power Macintosh Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "Toolbox - Win32 Debug"

!IF  "$(CFG)" == "Toolbox - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\Toolbox.exe"

CLEAN : 
	-@erase ".\Release\Toolbox.exe"
	-@erase ".\Release\crlookel.obj"
	-@erase ".\Release\jmp.obj"
	-@erase ".\Release\mkrset_d.obj"
	-@erase ".\Release\kbd.obj"
	-@erase ".\Release\clook.obj"
	-@erase ".\Release\EditLng.obj"
	-@erase ".\Release\patch.obj"
	-@erase ".\Release\tom.obj"
	-@erase ".\Release\newdoc_d.obj"
	-@erase ".\Release\shw.obj"
	-@erase ".\Release\html.obj"
	-@erase ".\Release\stdiofil.obj"
	-@erase ".\Release\pgs_d.obj"
	-@erase ".\Release\wrdlst_d.obj"
	-@erase ".\Release\cbo.obj"
	-@erase ".\Release\number_d.obj"
	-@erase ".\Release\browse_d.obj"
	-@erase ".\Release\mdf_d.obj"
	-@erase ".\Release\typ.obj"
	-@erase ".\Release\shwdtplt.obj"
	-@erase ".\Release\shwvwrap.obj"
	-@erase ".\Release\obstream.obj"
	-@erase ".\Release\page_d.obj"
	-@erase ".\Release\lng_d.obj"
	-@erase ".\Release\nlstream.obj"
	-@erase ".\Release\subfield.obj"
	-@erase ".\Release\alan.obj"
	-@erase ".\Release\wordlist.obj"
	-@erase ".\Release\browse.obj"
	-@erase ".\Release\corpus_d.obj"
	-@erase ".\Release\hugestr.obj"
	-@erase ".\Release\dlgrcins.obj"
	-@erase ".\Release\mrfset_d.obj"
	-@erase ".\Release\edlngctl.obj"
	-@erase ".\Release\mkrfed_d.obj"
	-@erase ".\Release\spath_d.obj"
	-@erase ".\Release\crecpos.obj"
	-@erase ".\Release\shwvdis.obj"
	-@erase ".\Release\project.obj"
	-@erase ".\Release\tplt_d.obj"
	-@erase ".\Release\ind_t.obj"
	-@erase ".\Release\multmat.obj"
	-@erase ".\Release\shwvdat.obj"
	-@erase ".\Release\dlgimpt.obj"
	-@erase ".\Release\rundos.obj"
	-@erase ".\Release\shwvprnt.obj"
	-@erase ".\Release\cslookel.obj"
	-@erase ".\Release\fil_d.obj"
	-@erase ".\Release\update.obj"
	-@erase ".\Release\export.obj"
	-@erase ".\Release\listentr.obj"
	-@erase ".\Release\prog_d.obj"
	-@erase ".\Release\cfield.obj"
	-@erase ".\Release\test_all.obj"
	-@erase ".\Release\loc.obj"
	-@erase ".\Release\lbo.obj"
	-@erase ".\Release\find_d.obj"
	-@erase ".\Release\nomat_d.obj"
	-@erase ".\Release\clookel.obj"
	-@erase ".\Release\dbref.obj"
	-@erase ".\Release\seldb_d.obj"
	-@erase ".\Release\cdbllist.obj"
	-@erase ".\Release\lookup.obj"
	-@erase ".\Release\number.obj"
	-@erase ".\Release\mkr_d.obj"
	-@erase ".\Release\wdf.obj"
	-@erase ".\Release\VersionDlg.obj"
	-@erase ".\Release\trie.obj"
	-@erase ".\Release\crecord.obj"
	-@erase ".\Release\mainfrm.obj"
	-@erase ".\Release\cdblel.obj"
	-@erase ".\Release\recset.obj"
	-@erase ".\Release\tools.obj"
	-@erase ".\Release\mdf.obj"
	-@erase ".\Release\stdafx.obj"
	-@erase ".\Release\pgs.obj"
	-@erase ".\Release\intprc_d.obj"
	-@erase ".\Release\mark.obj"
	-@erase ".\Release\fil.obj"
	-@erase ".\Release\crngst_d.obj"
	-@erase ".\Release\karen.obj"
	-@erase ".\Release\rod.obj"
	-@erase ".\Release\not.obj"
	-@erase ".\Release\set.obj"
	-@erase ".\Release\progress.obj"
	-@erase ".\Release\srt_d.obj"
	-@erase ".\Release\srtset_d.obj"
	-@erase ".\Release\shwvfind.obj"
	-@erase ".\Release\cck_d.obj"
	-@erase ".\Release\corpus.obj"
	-@erase ".\Release\printinf.obj"
	-@erase ".\Release\ind.obj"
	-@erase ".\Release\export_d.obj"
	-@erase ".\Release\shwview.obj"
	-@erase ".\Release\shwdoc.obj"
	-@erase ".\Release\ambig_d.obj"
	-@erase ".\Release\charset.obj"
	-@erase ".\Release\crngset.obj"
	-@erase ".\Release\shwvbrs.obj"
	-@erase ".\Release\cck.obj"
	-@erase ".\Release\typ_d.obj"
	-@erase ".\Release\lng.obj"
	-@erase ".\Release\cct.obj"
	-@erase ".\Release\kmn.obj"
	-@erase ".\Release\rtf.obj"
	-@erase ".\Release\shwdio.obj"
	-@erase ".\Release\wav.obj"
	-@erase ".\Release\interlin.obj"
	-@erase ".\Release\dlgsort.obj"
	-@erase ".\Release\font.obj"
	-@erase ".\Release\patmch.obj"
	-@erase ".\Release\intsetup.obj"
	-@erase ".\Release\jumpdlg.obj"
	-@erase ".\Release\filsel_d.obj"
	-@erase ".\Release\dirlist.obj"
	-@erase ".\Release\wdf_d.obj"
	-@erase ".\Release\xml.obj"
	-@erase ".\Release\date.obj"
	-@erase ".\Release\ddx.obj"
	-@erase ".\Release\progbar.obj"
	-@erase ".\Release\stringex.obj"
	-@erase ".\Release\status.obj"
	-@erase ".\Release\doclist.obj"
	-@erase ".\Release\mkr.obj"
	-@erase ".\Release\var_d.obj"
	-@erase ".\Release\var.obj"
	-@erase ".\Release\cslook.obj"
	-@erase ".\Release\key_d.obj"
	-@erase ".\Release\sfstream.obj"
	-@erase ".\Release\shwvscrl.obj"
	-@erase ".\Release\brian.obj"
	-@erase ".\Release\set_d.obj"
	-@erase ".\Release\booklist.obj"
	-@erase ".\Release\srt.obj"
	-@erase ".\Release\ref.obj"
	-@erase ".\Release\shwvundo.obj"
	-@erase ".\Release\shwpc.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Toolbox.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/shwpc.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Toolbox.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/Toolbox.pdb" /machine:I386 /out:"$(OUTDIR)/Toolbox.exe" 
LINK32_OBJS= \
	"$(INTDIR)/crlookel.obj" \
	"$(INTDIR)/jmp.obj" \
	"$(INTDIR)/mkrset_d.obj" \
	"$(INTDIR)/kbd.obj" \
	"$(INTDIR)/clook.obj" \
	"$(INTDIR)/EditLng.obj" \
	"$(INTDIR)/patch.obj" \
	"$(INTDIR)/tom.obj" \
	"$(INTDIR)/newdoc_d.obj" \
	"$(INTDIR)/shw.obj" \
	"$(INTDIR)/html.obj" \
	"$(INTDIR)/stdiofil.obj" \
	"$(INTDIR)/pgs_d.obj" \
	"$(INTDIR)/wrdlst_d.obj" \
	"$(INTDIR)/cbo.obj" \
	"$(INTDIR)/number_d.obj" \
	"$(INTDIR)/browse_d.obj" \
	"$(INTDIR)/mdf_d.obj" \
	"$(INTDIR)/typ.obj" \
	"$(INTDIR)/shwdtplt.obj" \
	"$(INTDIR)/shwvwrap.obj" \
	"$(INTDIR)/obstream.obj" \
	"$(INTDIR)/page_d.obj" \
	"$(INTDIR)/lng_d.obj" \
	"$(INTDIR)/nlstream.obj" \
	"$(INTDIR)/subfield.obj" \
	"$(INTDIR)/alan.obj" \
	"$(INTDIR)/wordlist.obj" \
	"$(INTDIR)/browse.obj" \
	"$(INTDIR)/corpus_d.obj" \
	"$(INTDIR)/hugestr.obj" \
	"$(INTDIR)/dlgrcins.obj" \
	"$(INTDIR)/mrfset_d.obj" \
	"$(INTDIR)/edlngctl.obj" \
	"$(INTDIR)/mkrfed_d.obj" \
	"$(INTDIR)/spath_d.obj" \
	"$(INTDIR)/crecpos.obj" \
	"$(INTDIR)/shwvdis.obj" \
	"$(INTDIR)/project.obj" \
	"$(INTDIR)/tplt_d.obj" \
	"$(INTDIR)/ind_t.obj" \
	"$(INTDIR)/multmat.obj" \
	"$(INTDIR)/shwvdat.obj" \
	"$(INTDIR)/dlgimpt.obj" \
	"$(INTDIR)/rundos.obj" \
	"$(INTDIR)/shwvprnt.obj" \
	"$(INTDIR)/cslookel.obj" \
	"$(INTDIR)/fil_d.obj" \
	"$(INTDIR)/update.obj" \
	"$(INTDIR)/export.obj" \
	"$(INTDIR)/listentr.obj" \
	"$(INTDIR)/prog_d.obj" \
	"$(INTDIR)/cfield.obj" \
	"$(INTDIR)/test_all.obj" \
	"$(INTDIR)/loc.obj" \
	"$(INTDIR)/lbo.obj" \
	"$(INTDIR)/find_d.obj" \
	"$(INTDIR)/nomat_d.obj" \
	"$(INTDIR)/clookel.obj" \
	"$(INTDIR)/dbref.obj" \
	"$(INTDIR)/seldb_d.obj" \
	"$(INTDIR)/cdbllist.obj" \
	"$(INTDIR)/lookup.obj" \
	"$(INTDIR)/number.obj" \
	"$(INTDIR)/mkr_d.obj" \
	"$(INTDIR)/wdf.obj" \
	"$(INTDIR)/VersionDlg.obj" \
	"$(INTDIR)/trie.obj" \
	"$(INTDIR)/crecord.obj" \
	"$(INTDIR)/mainfrm.obj" \
	"$(INTDIR)/cdblel.obj" \
	"$(INTDIR)/recset.obj" \
	"$(INTDIR)/tools.obj" \
	"$(INTDIR)/mdf.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/pgs.obj" \
	"$(INTDIR)/intprc_d.obj" \
	"$(INTDIR)/mark.obj" \
	"$(INTDIR)/fil.obj" \
	"$(INTDIR)/crngst_d.obj" \
	"$(INTDIR)/karen.obj" \
	"$(INTDIR)/rod.obj" \
	"$(INTDIR)/not.obj" \
	"$(INTDIR)/set.obj" \
	"$(INTDIR)/progress.obj" \
	"$(INTDIR)/srt_d.obj" \
	"$(INTDIR)/srtset_d.obj" \
	"$(INTDIR)/shwvfind.obj" \
	"$(INTDIR)/cck_d.obj" \
	"$(INTDIR)/corpus.obj" \
	"$(INTDIR)/printinf.obj" \
	"$(INTDIR)/ind.obj" \
	"$(INTDIR)/export_d.obj" \
	"$(INTDIR)/shwview.obj" \
	"$(INTDIR)/shwdoc.obj" \
	"$(INTDIR)/ambig_d.obj" \
	"$(INTDIR)/charset.obj" \
	"$(INTDIR)/crngset.obj" \
	"$(INTDIR)/shwvbrs.obj" \
	"$(INTDIR)/cck.obj" \
	"$(INTDIR)/typ_d.obj" \
	"$(INTDIR)/lng.obj" \
	"$(INTDIR)/cct.obj" \
	"$(INTDIR)/kmn.obj" \
	"$(INTDIR)/rtf.obj" \
	"$(INTDIR)/shwdio.obj" \
	"$(INTDIR)/wav.obj" \
	"$(INTDIR)/interlin.obj" \
	"$(INTDIR)/dlgsort.obj" \
	"$(INTDIR)/font.obj" \
	"$(INTDIR)/patmch.obj" \
	"$(INTDIR)/intsetup.obj" \
	"$(INTDIR)/jumpdlg.obj" \
	"$(INTDIR)/filsel_d.obj" \
	"$(INTDIR)/dirlist.obj" \
	"$(INTDIR)/wdf_d.obj" \
	"$(INTDIR)/xml.obj" \
	"$(INTDIR)/date.obj" \
	"$(INTDIR)/ddx.obj" \
	"$(INTDIR)/progbar.obj" \
	"$(INTDIR)/stringex.obj" \
	"$(INTDIR)/status.obj" \
	"$(INTDIR)/doclist.obj" \
	"$(INTDIR)/mkr.obj" \
	"$(INTDIR)/var_d.obj" \
	"$(INTDIR)/var.obj" \
	"$(INTDIR)/cslook.obj" \
	"$(INTDIR)/key_d.obj" \
	"$(INTDIR)/sfstream.obj" \
	"$(INTDIR)/shwvscrl.obj" \
	"$(INTDIR)/brian.obj" \
	"$(INTDIR)/set_d.obj" \
	"$(INTDIR)/booklist.obj" \
	"$(INTDIR)/srt.obj" \
	"$(INTDIR)/ref.obj" \
	"$(INTDIR)/shwvundo.obj" \
	"$(INTDIR)/shwpc.res"

"$(OUTDIR)\Toolbox.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\Toolbox.exe"

CLEAN : 
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"
	-@erase ".\Debug\Toolbox.exe"
	-@erase ".\Debug\doclist.obj"
	-@erase ".\Debug\test_all.obj"
	-@erase ".\Debug\mainfrm.obj"
	-@erase ".\Debug\clook.obj"
	-@erase ".\Debug\srt.obj"
	-@erase ".\Debug\patch.obj"
	-@erase ".\Debug\recset.obj"
	-@erase ".\Debug\html.obj"
	-@erase ".\Debug\cdbllist.obj"
	-@erase ".\Debug\jmp.obj"
	-@erase ".\Debug\pgs_d.obj"
	-@erase ".\Debug\rod.obj"
	-@erase ".\Debug\mdf_d.obj"
	-@erase ".\Debug\not.obj"
	-@erase ".\Debug\set.obj"
	-@erase ".\Debug\tom.obj"
	-@erase ".\Debug\lng_d.obj"
	-@erase ".\Debug\shw.obj"
	-@erase ".\Debug\alan.obj"
	-@erase ".\Debug\intprc_d.obj"
	-@erase ".\Debug\cbo.obj"
	-@erase ".\Debug\crngst_d.obj"
	-@erase ".\Debug\ind_t.obj"
	-@erase ".\Debug\lng.obj"
	-@erase ".\Debug\kmn.obj"
	-@erase ".\Debug\rtf.obj"
	-@erase ".\Debug\progress.obj"
	-@erase ".\Debug\shwvfind.obj"
	-@erase ".\Debug\fil_d.obj"
	-@erase ".\Debug\dlgsort.obj"
	-@erase ".\Debug\printinf.obj"
	-@erase ".\Debug\dbref.obj"
	-@erase ".\Debug\update.obj"
	-@erase ".\Debug\export.obj"
	-@erase ".\Debug\export_d.obj"
	-@erase ".\Debug\prog_d.obj"
	-@erase ".\Debug\cfield.obj"
	-@erase ".\Debug\patmch.obj"
	-@erase ".\Debug\xml.obj"
	-@erase ".\Debug\ddx.obj"
	-@erase ".\Debug\find_d.obj"
	-@erase ".\Debug\project.obj"
	-@erase ".\Debug\mkr_d.obj"
	-@erase ".\Debug\trie.obj"
	-@erase ".\Debug\var.obj"
	-@erase ".\Debug\tools.obj"
	-@erase ".\Debug\intsetup.obj"
	-@erase ".\Debug\lookup.obj"
	-@erase ".\Debug\number.obj"
	-@erase ".\Debug\ref.obj"
	-@erase ".\Debug\filsel_d.obj"
	-@erase ".\Debug\cdblel.obj"
	-@erase ".\Debug\wdf.obj"
	-@erase ".\Debug\mark.obj"
	-@erase ".\Debug\stringex.obj"
	-@erase ".\Debug\stdafx.obj"
	-@erase ".\Debug\karen.obj"
	-@erase ".\Debug\kbd.obj"
	-@erase ".\Debug\mdf.obj"
	-@erase ".\Debug\crecord.obj"
	-@erase ".\Debug\srt_d.obj"
	-@erase ".\Debug\sfstream.obj"
	-@erase ".\Debug\cck_d.obj"
	-@erase ".\Debug\pgs.obj"
	-@erase ".\Debug\shwvscrl.obj"
	-@erase ".\Debug\booklist.obj"
	-@erase ".\Debug\EditLng.obj"
	-@erase ".\Debug\fil.obj"
	-@erase ".\Debug\shwvundo.obj"
	-@erase ".\Debug\typ.obj"
	-@erase ".\Debug\crlookel.obj"
	-@erase ".\Debug\corpus.obj"
	-@erase ".\Debug\mkrset_d.obj"
	-@erase ".\Debug\typ_d.obj"
	-@erase ".\Debug\font.obj"
	-@erase ".\Debug\ind.obj"
	-@erase ".\Debug\newdoc_d.obj"
	-@erase ".\Debug\shwdoc.obj"
	-@erase ".\Debug\page_d.obj"
	-@erase ".\Debug\shwdio.obj"
	-@erase ".\Debug\stdiofil.obj"
	-@erase ".\Debug\wrdlst_d.obj"
	-@erase ".\Debug\cck.obj"
	-@erase ".\Debug\number_d.obj"
	-@erase ".\Debug\browse_d.obj"
	-@erase ".\Debug\hugestr.obj"
	-@erase ".\Debug\cct.obj"
	-@erase ".\Debug\wav.obj"
	-@erase ".\Debug\shwview.obj"
	-@erase ".\Debug\shwdtplt.obj"
	-@erase ".\Debug\spath_d.obj"
	-@erase ".\Debug\wdf_d.obj"
	-@erase ".\Debug\browse.obj"
	-@erase ".\Debug\crecpos.obj"
	-@erase ".\Debug\shwvdis.obj"
	-@erase ".\Debug\date.obj"
	-@erase ".\Debug\ambig_d.obj"
	-@erase ".\Debug\srtset_d.obj"
	-@erase ".\Debug\charset.obj"
	-@erase ".\Debug\VersionDlg.obj"
	-@erase ".\Debug\crngset.obj"
	-@erase ".\Debug\shwvbrs.obj"
	-@erase ".\Debug\shwvwrap.obj"
	-@erase ".\Debug\loc.obj"
	-@erase ".\Debug\obstream.obj"
	-@erase ".\Debug\lbo.obj"
	-@erase ".\Debug\multmat.obj"
	-@erase ".\Debug\var_d.obj"
	-@erase ".\Debug\shwvdat.obj"
	-@erase ".\Debug\dlgimpt.obj"
	-@erase ".\Debug\nlstream.obj"
	-@erase ".\Debug\tplt_d.obj"
	-@erase ".\Debug\key_d.obj"
	-@erase ".\Debug\subfield.obj"
	-@erase ".\Debug\rundos.obj"
	-@erase ".\Debug\wordlist.obj"
	-@erase ".\Debug\brian.obj"
	-@erase ".\Debug\set_d.obj"
	-@erase ".\Debug\corpus_d.obj"
	-@erase ".\Debug\dlgrcins.obj"
	-@erase ".\Debug\status.obj"
	-@erase ".\Debug\mrfset_d.obj"
	-@erase ".\Debug\edlngctl.obj"
	-@erase ".\Debug\mkrfed_d.obj"
	-@erase ".\Debug\jumpdlg.obj"
	-@erase ".\Debug\nomat_d.obj"
	-@erase ".\Debug\clookel.obj"
	-@erase ".\Debug\cslook.obj"
	-@erase ".\Debug\mkr.obj"
	-@erase ".\Debug\dirlist.obj"
	-@erase ".\Debug\seldb_d.obj"
	-@erase ".\Debug\interlin.obj"
	-@erase ".\Debug\shwvprnt.obj"
	-@erase ".\Debug\cslookel.obj"
	-@erase ".\Debug\progbar.obj"
	-@erase ".\Debug\listentr.obj"
	-@erase ".\Debug\shwpc.res"
	-@erase ".\Debug\Toolbox.ilk"
	-@erase ".\Debug\Toolbox.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/Toolbox.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/shwpc.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Toolbox.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/Toolbox.pdb" /debug /machine:I386 /out:"$(OUTDIR)/Toolbox.exe" 
LINK32_OBJS= \
	"$(INTDIR)/doclist.obj" \
	"$(INTDIR)/test_all.obj" \
	"$(INTDIR)/mainfrm.obj" \
	"$(INTDIR)/clook.obj" \
	"$(INTDIR)/srt.obj" \
	"$(INTDIR)/patch.obj" \
	"$(INTDIR)/recset.obj" \
	"$(INTDIR)/html.obj" \
	"$(INTDIR)/cdbllist.obj" \
	"$(INTDIR)/jmp.obj" \
	"$(INTDIR)/pgs_d.obj" \
	"$(INTDIR)/rod.obj" \
	"$(INTDIR)/mdf_d.obj" \
	"$(INTDIR)/not.obj" \
	"$(INTDIR)/set.obj" \
	"$(INTDIR)/tom.obj" \
	"$(INTDIR)/lng_d.obj" \
	"$(INTDIR)/shw.obj" \
	"$(INTDIR)/alan.obj" \
	"$(INTDIR)/intprc_d.obj" \
	"$(INTDIR)/cbo.obj" \
	"$(INTDIR)/crngst_d.obj" \
	"$(INTDIR)/ind_t.obj" \
	"$(INTDIR)/lng.obj" \
	"$(INTDIR)/kmn.obj" \
	"$(INTDIR)/rtf.obj" \
	"$(INTDIR)/progress.obj" \
	"$(INTDIR)/shwvfind.obj" \
	"$(INTDIR)/fil_d.obj" \
	"$(INTDIR)/dlgsort.obj" \
	"$(INTDIR)/printinf.obj" \
	"$(INTDIR)/dbref.obj" \
	"$(INTDIR)/update.obj" \
	"$(INTDIR)/export.obj" \
	"$(INTDIR)/export_d.obj" \
	"$(INTDIR)/prog_d.obj" \
	"$(INTDIR)/cfield.obj" \
	"$(INTDIR)/patmch.obj" \
	"$(INTDIR)/xml.obj" \
	"$(INTDIR)/ddx.obj" \
	"$(INTDIR)/find_d.obj" \
	"$(INTDIR)/project.obj" \
	"$(INTDIR)/mkr_d.obj" \
	"$(INTDIR)/trie.obj" \
	"$(INTDIR)/var.obj" \
	"$(INTDIR)/tools.obj" \
	"$(INTDIR)/intsetup.obj" \
	"$(INTDIR)/lookup.obj" \
	"$(INTDIR)/number.obj" \
	"$(INTDIR)/ref.obj" \
	"$(INTDIR)/filsel_d.obj" \
	"$(INTDIR)/cdblel.obj" \
	"$(INTDIR)/wdf.obj" \
	"$(INTDIR)/mark.obj" \
	"$(INTDIR)/stringex.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/karen.obj" \
	"$(INTDIR)/kbd.obj" \
	"$(INTDIR)/mdf.obj" \
	"$(INTDIR)/crecord.obj" \
	"$(INTDIR)/srt_d.obj" \
	"$(INTDIR)/sfstream.obj" \
	"$(INTDIR)/cck_d.obj" \
	"$(INTDIR)/pgs.obj" \
	"$(INTDIR)/shwvscrl.obj" \
	"$(INTDIR)/booklist.obj" \
	"$(INTDIR)/EditLng.obj" \
	"$(INTDIR)/fil.obj" \
	"$(INTDIR)/shwvundo.obj" \
	"$(INTDIR)/typ.obj" \
	"$(INTDIR)/crlookel.obj" \
	"$(INTDIR)/corpus.obj" \
	"$(INTDIR)/mkrset_d.obj" \
	"$(INTDIR)/typ_d.obj" \
	"$(INTDIR)/font.obj" \
	"$(INTDIR)/ind.obj" \
	"$(INTDIR)/newdoc_d.obj" \
	"$(INTDIR)/shwdoc.obj" \
	"$(INTDIR)/page_d.obj" \
	"$(INTDIR)/shwdio.obj" \
	"$(INTDIR)/stdiofil.obj" \
	"$(INTDIR)/wrdlst_d.obj" \
	"$(INTDIR)/cck.obj" \
	"$(INTDIR)/number_d.obj" \
	"$(INTDIR)/browse_d.obj" \
	"$(INTDIR)/hugestr.obj" \
	"$(INTDIR)/cct.obj" \
	"$(INTDIR)/wav.obj" \
	"$(INTDIR)/shwview.obj" \
	"$(INTDIR)/shwdtplt.obj" \
	"$(INTDIR)/spath_d.obj" \
	"$(INTDIR)/wdf_d.obj" \
	"$(INTDIR)/browse.obj" \
	"$(INTDIR)/crecpos.obj" \
	"$(INTDIR)/shwvdis.obj" \
	"$(INTDIR)/date.obj" \
	"$(INTDIR)/ambig_d.obj" \
	"$(INTDIR)/srtset_d.obj" \
	"$(INTDIR)/charset.obj" \
	"$(INTDIR)/VersionDlg.obj" \
	"$(INTDIR)/crngset.obj" \
	"$(INTDIR)/shwvbrs.obj" \
	"$(INTDIR)/shwvwrap.obj" \
	"$(INTDIR)/loc.obj" \
	"$(INTDIR)/obstream.obj" \
	"$(INTDIR)/lbo.obj" \
	"$(INTDIR)/multmat.obj" \
	"$(INTDIR)/var_d.obj" \
	"$(INTDIR)/shwvdat.obj" \
	"$(INTDIR)/dlgimpt.obj" \
	"$(INTDIR)/nlstream.obj" \
	"$(INTDIR)/tplt_d.obj" \
	"$(INTDIR)/key_d.obj" \
	"$(INTDIR)/subfield.obj" \
	"$(INTDIR)/rundos.obj" \
	"$(INTDIR)/wordlist.obj" \
	"$(INTDIR)/brian.obj" \
	"$(INTDIR)/set_d.obj" \
	"$(INTDIR)/corpus_d.obj" \
	"$(INTDIR)/dlgrcins.obj" \
	"$(INTDIR)/status.obj" \
	"$(INTDIR)/mrfset_d.obj" \
	"$(INTDIR)/edlngctl.obj" \
	"$(INTDIR)/mkrfed_d.obj" \
	"$(INTDIR)/jumpdlg.obj" \
	"$(INTDIR)/nomat_d.obj" \
	"$(INTDIR)/clookel.obj" \
	"$(INTDIR)/cslook.obj" \
	"$(INTDIR)/mkr.obj" \
	"$(INTDIR)/dirlist.obj" \
	"$(INTDIR)/seldb_d.obj" \
	"$(INTDIR)/interlin.obj" \
	"$(INTDIR)/shwvprnt.obj" \
	"$(INTDIR)/cslookel.obj" \
	"$(INTDIR)/progbar.obj" \
	"$(INTDIR)/listentr.obj" \
	"$(INTDIR)/shwpc.res"

"$(OUTDIR)\Toolbox.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Toolbox_"
# PROP BASE Intermediate_Dir "Toolbox_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Mac"
# PROP Intermediate_Dir "MacTemp"
# PROP Target_Dir ""
OUTDIR=.\Mac
INTDIR=.\MacTemp

ALL : "$(OUTDIR)\Toolbox.exe" "$(OUTDIR)\Toolbox.bsc" "$(OUTDIR)\Toolbox.trg"

CLEAN : 
	-@erase ".\Mac\Toolbox.bsc"
	-@erase ".\MacTemp\key_d.sbr"
	-@erase ".\MacTemp\xml.sbr"
	-@erase ".\MacTemp\brian.sbr"
	-@erase ".\MacTemp\corpus.sbr"
	-@erase ".\MacTemp\filsel_d.sbr"
	-@erase ".\MacTemp\mkr.sbr"
	-@erase ".\MacTemp\var.sbr"
	-@erase ".\MacTemp\stringex.sbr"
	-@erase ".\MacTemp\cslookel.sbr"
	-@erase ".\MacTemp\mkr_d.sbr"
	-@erase ".\MacTemp\shwdoc.sbr"
	-@erase ".\MacTemp\srt.sbr"
	-@erase ".\MacTemp\test_all.sbr"
	-@erase ".\MacTemp\ref.sbr"
	-@erase ".\MacTemp\tools.sbr"
	-@erase ".\MacTemp\html.sbr"
	-@erase ".\MacTemp\sfstream.sbr"
	-@erase ".\MacTemp\page_d.sbr"
	-@erase ".\MacTemp\EditLng.sbr"
	-@erase ".\MacTemp\clook.sbr"
	-@erase ".\MacTemp\shwdio.sbr"
	-@erase ".\MacTemp\booklist.sbr"
	-@erase ".\MacTemp\patch.sbr"
	-@erase ".\MacTemp\VersionDlg.sbr"
	-@erase ".\MacTemp\jmp.sbr"
	-@erase ".\MacTemp\browse.sbr"
	-@erase ".\MacTemp\kbd.sbr"
	-@erase ".\MacTemp\crlookel.sbr"
	-@erase ".\MacTemp\tom.sbr"
	-@erase ".\MacTemp\font.sbr"
	-@erase ".\MacTemp\tplt_d.sbr"
	-@erase ".\MacTemp\alan.sbr"
	-@erase ".\MacTemp\mkrset_d.sbr"
	-@erase ".\MacTemp\rundos.sbr"
	-@erase ".\MacTemp\shw.sbr"
	-@erase ".\MacTemp\srt_d.sbr"
	-@erase ".\MacTemp\cck_d.sbr"
	-@erase ".\MacTemp\status.sbr"
	-@erase ".\MacTemp\dlgsort.sbr"
	-@erase ".\MacTemp\cbo.sbr"
	-@erase ".\MacTemp\newdoc_d.sbr"
	-@erase ".\MacTemp\cslook.sbr"
	-@erase ".\MacTemp\date.sbr"
	-@erase ".\MacTemp\hugestr.sbr"
	-@erase ".\MacTemp\stdiofil.sbr"
	-@erase ".\MacTemp\wrdlst_d.sbr"
	-@erase ".\MacTemp\number_d.sbr"
	-@erase ".\MacTemp\spath_d.sbr"
	-@erase ".\MacTemp\crecpos.sbr"
	-@erase ".\MacTemp\shwvdis.sbr"
	-@erase ".\MacTemp\project.sbr"
	-@erase ".\MacTemp\shwdtplt.sbr"
	-@erase ".\MacTemp\ind_t.sbr"
	-@erase ".\MacTemp\srtset_d.sbr"
	-@erase ".\MacTemp\multmat.sbr"
	-@erase ".\MacTemp\shwvdat.sbr"
	-@erase ".\MacTemp\dlgimpt.sbr"
	-@erase ".\MacTemp\shwvfind.sbr"
	-@erase ".\MacTemp\obstream.sbr"
	-@erase ".\MacTemp\nlstream.sbr"
	-@erase ".\MacTemp\recset.sbr"
	-@erase ".\MacTemp\subfield.sbr"
	-@erase ".\MacTemp\wordlist.sbr"
	-@erase ".\MacTemp\corpus_d.sbr"
	-@erase ".\MacTemp\dlgrcins.sbr"
	-@erase ".\MacTemp\clookel.sbr"
	-@erase ".\MacTemp\ddx.sbr"
	-@erase ".\MacTemp\mrfset_d.sbr"
	-@erase ".\MacTemp\edlngctl.sbr"
	-@erase ".\MacTemp\mkrfed_d.sbr"
	-@erase ".\MacTemp\dbref.sbr"
	-@erase ".\MacTemp\loc.sbr"
	-@erase ".\MacTemp\seldb_d.sbr"
	-@erase ".\MacTemp\lbo.sbr"
	-@erase ".\MacTemp\interlin.sbr"
	-@erase ".\MacTemp\shwvprnt.sbr"
	-@erase ".\MacTemp\crecord.sbr"
	-@erase ".\MacTemp\mainfrm.sbr"
	-@erase ".\MacTemp\listentr.sbr"
	-@erase ".\MacTemp\var_d.sbr"
	-@erase ".\MacTemp\intsetup.sbr"
	-@erase ".\MacTemp\mark.sbr"
	-@erase ".\MacTemp\set_d.sbr"
	-@erase ".\MacTemp\wdf.sbr"
	-@erase ".\MacTemp\cdbllist.sbr"
	-@erase ".\MacTemp\mdf.sbr"
	-@erase ".\MacTemp\karen.sbr"
	-@erase ".\MacTemp\pgs.sbr"
	-@erase ".\MacTemp\shwvscrl.sbr"
	-@erase ".\MacTemp\update.sbr"
	-@erase ".\MacTemp\export.sbr"
	-@erase ".\MacTemp\fil.sbr"
	-@erase ".\MacTemp\prog_d.sbr"
	-@erase ".\MacTemp\cfield.sbr"
	-@erase ".\MacTemp\patmch.sbr"
	-@erase ".\MacTemp\intprc_d.sbr"
	-@erase ".\MacTemp\find_d.sbr"
	-@erase ".\MacTemp\typ.sbr"
	-@erase ".\MacTemp\rod.sbr"
	-@erase ".\MacTemp\shwvundo.sbr"
	-@erase ".\MacTemp\not.sbr"
	-@erase ".\MacTemp\set.sbr"
	-@erase ".\MacTemp\pgs_d.sbr"
	-@erase ".\MacTemp\crngst_d.sbr"
	-@erase ".\MacTemp\shwview.sbr"
	-@erase ".\MacTemp\mdf_d.sbr"
	-@erase ".\MacTemp\ambig_d.sbr"
	-@erase ".\MacTemp\charset.sbr"
	-@erase ".\MacTemp\crngset.sbr"
	-@erase ".\MacTemp\shwvbrs.sbr"
	-@erase ".\MacTemp\ind.sbr"
	-@erase ".\MacTemp\progress.sbr"
	-@erase ".\MacTemp\lookup.sbr"
	-@erase ".\MacTemp\number.sbr"
	-@erase ".\MacTemp\typ_d.sbr"
	-@erase ".\MacTemp\lng_d.sbr"
	-@erase ".\MacTemp\cdblel.sbr"
	-@erase ".\MacTemp\printinf.sbr"
	-@erase ".\MacTemp\cck.sbr"
	-@erase ".\MacTemp\lng.sbr"
	-@erase ".\MacTemp\stdafx.sbr"
	-@erase ".\MacTemp\jumpdlg.sbr"
	-@erase ".\MacTemp\cct.sbr"
	-@erase ".\MacTemp\kmn.sbr"
	-@erase ".\MacTemp\nomat_d.sbr"
	-@erase ".\MacTemp\rtf.sbr"
	-@erase ".\MacTemp\wav.sbr"
	-@erase ".\MacTemp\trie.sbr"
	-@erase ".\MacTemp\browse_d.sbr"
	-@erase ".\MacTemp\export_d.sbr"
	-@erase ".\MacTemp\dirlist.sbr"
	-@erase ".\MacTemp\shwvwrap.sbr"
	-@erase ".\MacTemp\wdf_d.sbr"
	-@erase ".\MacTemp\progbar.sbr"
	-@erase ".\MacTemp\doclist.sbr"
	-@erase ".\MacTemp\fil_d.sbr"
	-@erase ".\Mac\Toolbox.trg"
	-@erase ".\Mac\Toolbox.exe"
	-@erase ".\MacTemp\mdf_d.obj"
	-@erase ".\MacTemp\ambig_d.obj"
	-@erase ".\MacTemp\charset.obj"
	-@erase ".\MacTemp\crngset.obj"
	-@erase ".\MacTemp\shwvbrs.obj"
	-@erase ".\MacTemp\ind.obj"
	-@erase ".\MacTemp\progress.obj"
	-@erase ".\MacTemp\lookup.obj"
	-@erase ".\MacTemp\number.obj"
	-@erase ".\MacTemp\typ_d.obj"
	-@erase ".\MacTemp\lng_d.obj"
	-@erase ".\MacTemp\cdblel.obj"
	-@erase ".\MacTemp\printinf.obj"
	-@erase ".\MacTemp\cck.obj"
	-@erase ".\MacTemp\lng.obj"
	-@erase ".\MacTemp\stdafx.obj"
	-@erase ".\MacTemp\jumpdlg.obj"
	-@erase ".\MacTemp\cct.obj"
	-@erase ".\MacTemp\kmn.obj"
	-@erase ".\MacTemp\nomat_d.obj"
	-@erase ".\MacTemp\rtf.obj"
	-@erase ".\MacTemp\wav.obj"
	-@erase ".\MacTemp\trie.obj"
	-@erase ".\MacTemp\browse_d.obj"
	-@erase ".\MacTemp\export_d.obj"
	-@erase ".\MacTemp\dirlist.obj"
	-@erase ".\MacTemp\shwvwrap.obj"
	-@erase ".\MacTemp\wdf_d.obj"
	-@erase ".\MacTemp\progbar.obj"
	-@erase ".\MacTemp\doclist.obj"
	-@erase ".\MacTemp\fil_d.obj"
	-@erase ".\MacTemp\key_d.obj"
	-@erase ".\MacTemp\xml.obj"
	-@erase ".\MacTemp\brian.obj"
	-@erase ".\MacTemp\corpus.obj"
	-@erase ".\MacTemp\filsel_d.obj"
	-@erase ".\MacTemp\mkr.obj"
	-@erase ".\MacTemp\var.obj"
	-@erase ".\MacTemp\stringex.obj"
	-@erase ".\MacTemp\cslookel.obj"
	-@erase ".\MacTemp\mkr_d.obj"
	-@erase ".\MacTemp\shwdoc.obj"
	-@erase ".\MacTemp\srt.obj"
	-@erase ".\MacTemp\test_all.obj"
	-@erase ".\MacTemp\ref.obj"
	-@erase ".\MacTemp\tools.obj"
	-@erase ".\MacTemp\html.obj"
	-@erase ".\MacTemp\sfstream.obj"
	-@erase ".\MacTemp\page_d.obj"
	-@erase ".\MacTemp\EditLng.obj"
	-@erase ".\MacTemp\clook.obj"
	-@erase ".\MacTemp\shwdio.obj"
	-@erase ".\MacTemp\booklist.obj"
	-@erase ".\MacTemp\patch.obj"
	-@erase ".\MacTemp\VersionDlg.obj"
	-@erase ".\MacTemp\jmp.obj"
	-@erase ".\MacTemp\browse.obj"
	-@erase ".\MacTemp\kbd.obj"
	-@erase ".\MacTemp\crlookel.obj"
	-@erase ".\MacTemp\tom.obj"
	-@erase ".\MacTemp\font.obj"
	-@erase ".\MacTemp\tplt_d.obj"
	-@erase ".\MacTemp\alan.obj"
	-@erase ".\MacTemp\mkrset_d.obj"
	-@erase ".\MacTemp\rundos.obj"
	-@erase ".\MacTemp\shw.obj"
	-@erase ".\MacTemp\srt_d.obj"
	-@erase ".\MacTemp\cck_d.obj"
	-@erase ".\MacTemp\status.obj"
	-@erase ".\MacTemp\dlgsort.obj"
	-@erase ".\MacTemp\cbo.obj"
	-@erase ".\MacTemp\newdoc_d.obj"
	-@erase ".\MacTemp\cslook.obj"
	-@erase ".\MacTemp\date.obj"
	-@erase ".\MacTemp\hugestr.obj"
	-@erase ".\MacTemp\stdiofil.obj"
	-@erase ".\MacTemp\wrdlst_d.obj"
	-@erase ".\MacTemp\number_d.obj"
	-@erase ".\MacTemp\spath_d.obj"
	-@erase ".\MacTemp\crecpos.obj"
	-@erase ".\MacTemp\shwvdis.obj"
	-@erase ".\MacTemp\project.obj"
	-@erase ".\MacTemp\shwdtplt.obj"
	-@erase ".\MacTemp\ind_t.obj"
	-@erase ".\MacTemp\srtset_d.obj"
	-@erase ".\MacTemp\multmat.obj"
	-@erase ".\MacTemp\shwvdat.obj"
	-@erase ".\MacTemp\dlgimpt.obj"
	-@erase ".\MacTemp\shwvfind.obj"
	-@erase ".\MacTemp\obstream.obj"
	-@erase ".\MacTemp\nlstream.obj"
	-@erase ".\MacTemp\recset.obj"
	-@erase ".\MacTemp\subfield.obj"
	-@erase ".\MacTemp\wordlist.obj"
	-@erase ".\MacTemp\corpus_d.obj"
	-@erase ".\MacTemp\dlgrcins.obj"
	-@erase ".\MacTemp\clookel.obj"
	-@erase ".\MacTemp\ddx.obj"
	-@erase ".\MacTemp\mrfset_d.obj"
	-@erase ".\MacTemp\edlngctl.obj"
	-@erase ".\MacTemp\mkrfed_d.obj"
	-@erase ".\MacTemp\dbref.obj"
	-@erase ".\MacTemp\loc.obj"
	-@erase ".\MacTemp\seldb_d.obj"
	-@erase ".\MacTemp\lbo.obj"
	-@erase ".\MacTemp\interlin.obj"
	-@erase ".\MacTemp\shwvprnt.obj"
	-@erase ".\MacTemp\crecord.obj"
	-@erase ".\MacTemp\mainfrm.obj"
	-@erase ".\MacTemp\listentr.obj"
	-@erase ".\MacTemp\var_d.obj"
	-@erase ".\MacTemp\intsetup.obj"
	-@erase ".\MacTemp\mark.obj"
	-@erase ".\MacTemp\set_d.obj"
	-@erase ".\MacTemp\wdf.obj"
	-@erase ".\MacTemp\cdbllist.obj"
	-@erase ".\MacTemp\mdf.obj"
	-@erase ".\MacTemp\karen.obj"
	-@erase ".\MacTemp\pgs.obj"
	-@erase ".\MacTemp\shwvscrl.obj"
	-@erase ".\MacTemp\update.obj"
	-@erase ".\MacTemp\export.obj"
	-@erase ".\MacTemp\fil.obj"
	-@erase ".\MacTemp\prog_d.obj"
	-@erase ".\MacTemp\cfield.obj"
	-@erase ".\MacTemp\patmch.obj"
	-@erase ".\MacTemp\intprc_d.obj"
	-@erase ".\MacTemp\find_d.obj"
	-@erase ".\MacTemp\typ.obj"
	-@erase ".\MacTemp\rod.obj"
	-@erase ".\MacTemp\shwvundo.obj"
	-@erase ".\MacTemp\not.obj"
	-@erase ".\MacTemp\set.obj"
	-@erase ".\MacTemp\pgs_d.obj"
	-@erase ".\MacTemp\crngst_d.obj"
	-@erase ".\MacTemp\shwview.obj"
	-@erase ".\shwmac.rsc"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

"$(INTDIR)" :
    if not exist "$(INTDIR)/$(NULL)" mkdir "$(INTDIR)"

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "_MAC" /D "_MPPC_" /D "_WINDOWS" /D "WIN32" /D "NDEBUG" /D "_WLMDLL" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "_MAC" /D "_MPPC_" /D "_WINDOWS" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FR /YX /J /c
CPP_PROJ=/nologo /W3 /GX /O2 /D "_MAC" /D "_MPPC_" /D "_WINDOWS" /D "WIN32" /D\
 "NDEBUG" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/Toolbox.pch" /YX\
 /Fo"$(INTDIR)/" /J /c 
CPP_OBJS=.\MacTemp/
CPP_SBRS=.\MacTemp/

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC=rc.exe
# ADD BASE RSC /l 0x409 /r /d "_MAC" /d "_MPPC_" /d "NDEBUG"
# ADD RSC /l 0x409 /r /fo"shwmac.rsc" /d "_MAC" /d "_MPPC_" /d "NDEBUG"
RSC_PROJ=/l 0x409 /r /m /fo"shwmac.rsc" /d "_MAC" /d "_MPPC_" /d "NDEBUG" 
MRC=mrc.exe
# ADD BASE MRC /D "_MPPC_" /D "_MAC" /D "NDEBUG" /NOLOGO
# ADD MRC /D "_MPPC_" /D "_MAC" /D "NDEBUG" /NOLOGO
MRC_PROJ=/D "_MPPC_" /D "_MAC" /D "NDEBUG" /l 0x409 /NOLOGO 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/Toolbox.bsc" 
BSC32_SBRS= \
	"$(INTDIR)/key_d.sbr" \
	"$(INTDIR)/xml.sbr" \
	"$(INTDIR)/brian.sbr" \
	"$(INTDIR)/corpus.sbr" \
	"$(INTDIR)/filsel_d.sbr" \
	"$(INTDIR)/mkr.sbr" \
	"$(INTDIR)/var.sbr" \
	"$(INTDIR)/stringex.sbr" \
	"$(INTDIR)/cslookel.sbr" \
	"$(INTDIR)/mkr_d.sbr" \
	"$(INTDIR)/shwdoc.sbr" \
	"$(INTDIR)/srt.sbr" \
	"$(INTDIR)/test_all.sbr" \
	"$(INTDIR)/ref.sbr" \
	"$(INTDIR)/tools.sbr" \
	"$(INTDIR)/html.sbr" \
	"$(INTDIR)/sfstream.sbr" \
	"$(INTDIR)/page_d.sbr" \
	"$(INTDIR)/EditLng.sbr" \
	"$(INTDIR)/clook.sbr" \
	"$(INTDIR)/shwdio.sbr" \
	"$(INTDIR)/booklist.sbr" \
	"$(INTDIR)/patch.sbr" \
	"$(INTDIR)/VersionDlg.sbr" \
	"$(INTDIR)/jmp.sbr" \
	"$(INTDIR)/browse.sbr" \
	"$(INTDIR)/kbd.sbr" \
	"$(INTDIR)/crlookel.sbr" \
	"$(INTDIR)/tom.sbr" \
	"$(INTDIR)/font.sbr" \
	"$(INTDIR)/tplt_d.sbr" \
	"$(INTDIR)/alan.sbr" \
	"$(INTDIR)/mkrset_d.sbr" \
	"$(INTDIR)/rundos.sbr" \
	"$(INTDIR)/shw.sbr" \
	"$(INTDIR)/srt_d.sbr" \
	"$(INTDIR)/cck_d.sbr" \
	"$(INTDIR)/status.sbr" \
	"$(INTDIR)/dlgsort.sbr" \
	"$(INTDIR)/cbo.sbr" \
	"$(INTDIR)/newdoc_d.sbr" \
	"$(INTDIR)/cslook.sbr" \
	"$(INTDIR)/date.sbr" \
	"$(INTDIR)/hugestr.sbr" \
	"$(INTDIR)/stdiofil.sbr" \
	"$(INTDIR)/wrdlst_d.sbr" \
	"$(INTDIR)/number_d.sbr" \
	"$(INTDIR)/spath_d.sbr" \
	"$(INTDIR)/crecpos.sbr" \
	"$(INTDIR)/shwvdis.sbr" \
	"$(INTDIR)/project.sbr" \
	"$(INTDIR)/shwdtplt.sbr" \
	"$(INTDIR)/ind_t.sbr" \
	"$(INTDIR)/srtset_d.sbr" \
	"$(INTDIR)/multmat.sbr" \
	"$(INTDIR)/shwvdat.sbr" \
	"$(INTDIR)/dlgimpt.sbr" \
	"$(INTDIR)/shwvfind.sbr" \
	"$(INTDIR)/obstream.sbr" \
	"$(INTDIR)/nlstream.sbr" \
	"$(INTDIR)/recset.sbr" \
	"$(INTDIR)/subfield.sbr" \
	"$(INTDIR)/wordlist.sbr" \
	"$(INTDIR)/corpus_d.sbr" \
	"$(INTDIR)/dlgrcins.sbr" \
	"$(INTDIR)/clookel.sbr" \
	"$(INTDIR)/ddx.sbr" \
	"$(INTDIR)/mrfset_d.sbr" \
	"$(INTDIR)/edlngctl.sbr" \
	"$(INTDIR)/mkrfed_d.sbr" \
	"$(INTDIR)/dbref.sbr" \
	"$(INTDIR)/loc.sbr" \
	"$(INTDIR)/seldb_d.sbr" \
	"$(INTDIR)/lbo.sbr" \
	"$(INTDIR)/interlin.sbr" \
	"$(INTDIR)/shwvprnt.sbr" \
	"$(INTDIR)/crecord.sbr" \
	"$(INTDIR)/mainfrm.sbr" \
	"$(INTDIR)/listentr.sbr" \
	"$(INTDIR)/var_d.sbr" \
	"$(INTDIR)/intsetup.sbr" \
	"$(INTDIR)/mark.sbr" \
	"$(INTDIR)/set_d.sbr" \
	"$(INTDIR)/wdf.sbr" \
	"$(INTDIR)/cdbllist.sbr" \
	"$(INTDIR)/mdf.sbr" \
	"$(INTDIR)/karen.sbr" \
	"$(INTDIR)/pgs.sbr" \
	"$(INTDIR)/shwvscrl.sbr" \
	"$(INTDIR)/update.sbr" \
	"$(INTDIR)/export.sbr" \
	"$(INTDIR)/fil.sbr" \
	"$(INTDIR)/prog_d.sbr" \
	"$(INTDIR)/cfield.sbr" \
	"$(INTDIR)/patmch.sbr" \
	"$(INTDIR)/intprc_d.sbr" \
	"$(INTDIR)/find_d.sbr" \
	"$(INTDIR)/typ.sbr" \
	"$(INTDIR)/rod.sbr" \
	"$(INTDIR)/shwvundo.sbr" \
	"$(INTDIR)/not.sbr" \
	"$(INTDIR)/set.sbr" \
	"$(INTDIR)/pgs_d.sbr" \
	"$(INTDIR)/crngst_d.sbr" \
	"$(INTDIR)/shwview.sbr" \
	"$(INTDIR)/mdf_d.sbr" \
	"$(INTDIR)/ambig_d.sbr" \
	"$(INTDIR)/charset.sbr" \
	"$(INTDIR)/crngset.sbr" \
	"$(INTDIR)/shwvbrs.sbr" \
	"$(INTDIR)/ind.sbr" \
	"$(INTDIR)/progress.sbr" \
	"$(INTDIR)/lookup.sbr" \
	"$(INTDIR)/number.sbr" \
	"$(INTDIR)/typ_d.sbr" \
	"$(INTDIR)/lng_d.sbr" \
	"$(INTDIR)/cdblel.sbr" \
	"$(INTDIR)/printinf.sbr" \
	"$(INTDIR)/cck.sbr" \
	"$(INTDIR)/lng.sbr" \
	"$(INTDIR)/stdafx.sbr" \
	"$(INTDIR)/jumpdlg.sbr" \
	"$(INTDIR)/cct.sbr" \
	"$(INTDIR)/kmn.sbr" \
	"$(INTDIR)/nomat_d.sbr" \
	"$(INTDIR)/rtf.sbr" \
	"$(INTDIR)/wav.sbr" \
	"$(INTDIR)/trie.sbr" \
	"$(INTDIR)/browse_d.sbr" \
	"$(INTDIR)/export_d.sbr" \
	"$(INTDIR)/dirlist.sbr" \
	"$(INTDIR)/shwvwrap.sbr" \
	"$(INTDIR)/wdf_d.sbr" \
	"$(INTDIR)/progbar.sbr" \
	"$(INTDIR)/doclist.sbr" \
	"$(INTDIR)/fil_d.sbr"

"$(OUTDIR)\Toolbox.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib cfrg.rsc /nologo /mac:bundle /machine:MPPC
# ADD LINK32 cclibpmac.lib uuid.lib /nologo /mac:bundle /machine:MPPC
LINK32_FLAGS=cclibpmac.lib uuid.lib /nologo /mac:bundle /mac:type="APPL"\
 /mac:creator="????" /incremental:no /pdb:"$(OUTDIR)/Toolbox.pdb" /machine:MPPC\
 /out:"$(OUTDIR)/Toolbox.exe" 
LINK32_OBJS= \
	"$(INTDIR)/mdf_d.obj" \
	"$(INTDIR)/ambig_d.obj" \
	"$(INTDIR)/charset.obj" \
	"$(INTDIR)/crngset.obj" \
	"$(INTDIR)/shwvbrs.obj" \
	"$(INTDIR)/ind.obj" \
	"$(INTDIR)/progress.obj" \
	"$(INTDIR)/lookup.obj" \
	"$(INTDIR)/number.obj" \
	"$(INTDIR)/typ_d.obj" \
	"$(INTDIR)/lng_d.obj" \
	"$(INTDIR)/cdblel.obj" \
	"$(INTDIR)/printinf.obj" \
	"$(INTDIR)/cck.obj" \
	"$(INTDIR)/lng.obj" \
	"$(INTDIR)/stdafx.obj" \
	"$(INTDIR)/jumpdlg.obj" \
	"$(INTDIR)/cct.obj" \
	"$(INTDIR)/kmn.obj" \
	"$(INTDIR)/nomat_d.obj" \
	"$(INTDIR)/rtf.obj" \
	"$(INTDIR)/wav.obj" \
	"$(INTDIR)/trie.obj" \
	"$(INTDIR)/browse_d.obj" \
	"$(INTDIR)/export_d.obj" \
	"$(INTDIR)/dirlist.obj" \
	"$(INTDIR)/shwvwrap.obj" \
	"$(INTDIR)/wdf_d.obj" \
	"$(INTDIR)/progbar.obj" \
	"$(INTDIR)/doclist.obj" \
	"$(INTDIR)/fil_d.obj" \
	"$(INTDIR)/key_d.obj" \
	"$(INTDIR)/xml.obj" \
	"$(INTDIR)/brian.obj" \
	"$(INTDIR)/corpus.obj" \
	"$(INTDIR)/filsel_d.obj" \
	"$(INTDIR)/mkr.obj" \
	"$(INTDIR)/var.obj" \
	"$(INTDIR)/stringex.obj" \
	"$(INTDIR)/cslookel.obj" \
	"$(INTDIR)/mkr_d.obj" \
	"$(INTDIR)/shwdoc.obj" \
	"$(INTDIR)/srt.obj" \
	"$(INTDIR)/test_all.obj" \
	"$(INTDIR)/ref.obj" \
	"$(INTDIR)/tools.obj" \
	"$(INTDIR)/html.obj" \
	"$(INTDIR)/sfstream.obj" \
	"$(INTDIR)/page_d.obj" \
	"$(INTDIR)/EditLng.obj" \
	"$(INTDIR)/clook.obj" \
	"$(INTDIR)/shwdio.obj" \
	"$(INTDIR)/booklist.obj" \
	"$(INTDIR)/patch.obj" \
	"$(INTDIR)/VersionDlg.obj" \
	"$(INTDIR)/jmp.obj" \
	"$(INTDIR)/browse.obj" \
	"$(INTDIR)/kbd.obj" \
	"$(INTDIR)/crlookel.obj" \
	"$(INTDIR)/tom.obj" \
	"$(INTDIR)/font.obj" \
	"$(INTDIR)/tplt_d.obj" \
	"$(INTDIR)/alan.obj" \
	"$(INTDIR)/mkrset_d.obj" \
	"$(INTDIR)/rundos.obj" \
	"$(INTDIR)/shw.obj" \
	"$(INTDIR)/srt_d.obj" \
	"$(INTDIR)/cck_d.obj" \
	"$(INTDIR)/status.obj" \
	"$(INTDIR)/dlgsort.obj" \
	"$(INTDIR)/cbo.obj" \
	"$(INTDIR)/newdoc_d.obj" \
	"$(INTDIR)/cslook.obj" \
	"$(INTDIR)/date.obj" \
	"$(INTDIR)/hugestr.obj" \
	"$(INTDIR)/stdiofil.obj" \
	"$(INTDIR)/wrdlst_d.obj" \
	"$(INTDIR)/number_d.obj" \
	"$(INTDIR)/spath_d.obj" \
	"$(INTDIR)/crecpos.obj" \
	"$(INTDIR)/shwvdis.obj" \
	"$(INTDIR)/project.obj" \
	"$(INTDIR)/shwdtplt.obj" \
	"$(INTDIR)/ind_t.obj" \
	"$(INTDIR)/srtset_d.obj" \
	"$(INTDIR)/multmat.obj" \
	"$(INTDIR)/shwvdat.obj" \
	"$(INTDIR)/dlgimpt.obj" \
	"$(INTDIR)/shwvfind.obj" \
	"$(INTDIR)/obstream.obj" \
	"$(INTDIR)/nlstream.obj" \
	"$(INTDIR)/recset.obj" \
	"$(INTDIR)/subfield.obj" \
	"$(INTDIR)/wordlist.obj" \
	"$(INTDIR)/corpus_d.obj" \
	"$(INTDIR)/dlgrcins.obj" \
	"$(INTDIR)/clookel.obj" \
	"$(INTDIR)/ddx.obj" \
	"$(INTDIR)/mrfset_d.obj" \
	"$(INTDIR)/edlngctl.obj" \
	"$(INTDIR)/mkrfed_d.obj" \
	"$(INTDIR)/dbref.obj" \
	"$(INTDIR)/loc.obj" \
	"$(INTDIR)/seldb_d.obj" \
	"$(INTDIR)/lbo.obj" \
	"$(INTDIR)/interlin.obj" \
	"$(INTDIR)/shwvprnt.obj" \
	"$(INTDIR)/crecord.obj" \
	"$(INTDIR)/mainfrm.obj" \
	"$(INTDIR)/listentr.obj" \
	"$(INTDIR)/var_d.obj" \
	"$(INTDIR)/intsetup.obj" \
	"$(INTDIR)/mark.obj" \
	"$(INTDIR)/set_d.obj" \
	"$(INTDIR)/wdf.obj" \
	"$(INTDIR)/cdbllist.obj" \
	"$(INTDIR)/mdf.obj" \
	"$(INTDIR)/karen.obj" \
	"$(INTDIR)/pgs.obj" \
	"$(INTDIR)/shwvscrl.obj" \
	"$(INTDIR)/update.obj" \
	"$(INTDIR)/export.obj" \
	"$(INTDIR)/fil.obj" \
	"$(INTDIR)/prog_d.obj" \
	"$(INTDIR)/cfield.obj" \
	"$(INTDIR)/patmch.obj" \
	"$(INTDIR)/intprc_d.obj" \
	"$(INTDIR)/find_d.obj" \
	"$(INTDIR)/typ.obj" \
	"$(INTDIR)/rod.obj" \
	"$(INTDIR)/shwvundo.obj" \
	"$(INTDIR)/not.obj" \
	"$(INTDIR)/set.obj" \
	"$(INTDIR)/pgs_d.obj" \
	"$(INTDIR)/crngst_d.obj" \
	"$(INTDIR)/shwview.obj" \
	".\shwmac.rsc"

"$(OUTDIR)\Toolbox.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

MFILE32=mfile.exe
# ADD BASE MFILE32 COPY /NOLOGO
# ADD MFILE32 COPY /NOLOGO
MFILE32_FLAGS=COPY /NOLOGO 
MFILE32_FILES= \
	"$(OUTDIR)/Toolbox.exe"

"$(OUTDIR)\Toolbox.trg" : "$(OUTDIR)" $(MFILE32_FILES)
    $(MFILE32) $(MFILE32_FLAGS) .\Mac\Toolbox.exe\
 "$(MFILE32_DEST):Toolbox.exe">"$(OUTDIR)\Toolbox.trg"

DOWNLOAD : "$(OUTDIR)" $(MFILE32_FILES)
    $(MFILE32) $(MFILE32_FLAGS) .\Mac\Toolbox.exe\
 "$(MFILE32_DEST):Toolbox.exe">"$(OUTDIR)\Toolbox.trg"

!ENDIF 

################################################################################
# Begin Target

# Name "Toolbox - Win32 Release"
# Name "Toolbox - Win32 Debug"
# Name "Toolbox - Power Macintosh Release"

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\xml.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\xml.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_XML_C=\
	".\stdafx.h"\
	".\xml.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\xml.obj" : $(SOURCE) $(DEP_CPP_XML_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_XML_C=\
	".\stdafx.h"\
	".\xml.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\xml.obj" : $(SOURCE) $(DEP_CPP_XML_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_XML_C=\
	".\stdafx.h"\
	".\xml.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\xml.obj" : $(SOURCE) $(DEP_CPP_XML_C) "$(INTDIR)"

"$(INTDIR)\xml.sbr" : $(SOURCE) $(DEP_CPP_XML_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wrdlst_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wrdlst_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_WRDLS=\
	".\stdafx.h"\
	".\wrdlst_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\corpus.h"\
	".\project.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\wordlist.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\prog_d.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	

"$(INTDIR)\wrdlst_d.obj" : $(SOURCE) $(DEP_CPP_WRDLS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_WRDLS=\
	".\stdafx.h"\
	".\wrdlst_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\corpus.h"\
	".\project.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\wordlist.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\prog_d.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	

"$(INTDIR)\wrdlst_d.obj" : $(SOURCE) $(DEP_CPP_WRDLS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_WRDLS=\
	".\stdafx.h"\
	".\wrdlst_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\corpus.h"\
	".\project.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\wordlist.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\prog_d.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	

"$(INTDIR)\wrdlst_d.obj" : $(SOURCE) $(DEP_CPP_WRDLS) "$(INTDIR)"

"$(INTDIR)\wrdlst_d.sbr" : $(SOURCE) $(DEP_CPP_WRDLS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wordlist.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wordlist.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_WORDL=\
	".\stdafx.h"\
	".\wordlist.h"\
	".\stdiofil.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\charset.h"\
	".\stringex.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\wordlist.obj" : $(SOURCE) $(DEP_CPP_WORDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_WORDL=\
	".\stdafx.h"\
	".\wordlist.h"\
	".\stdiofil.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\charset.h"\
	".\stringex.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\wordlist.obj" : $(SOURCE) $(DEP_CPP_WORDL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_WORDL=\
	".\stdafx.h"\
	".\wordlist.h"\
	".\stdiofil.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\charset.h"\
	".\stringex.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\wordlist.obj" : $(SOURCE) $(DEP_CPP_WORDL) "$(INTDIR)"

"$(INTDIR)\wordlist.sbr" : $(SOURCE) $(DEP_CPP_WORDL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wdf_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wdf_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_WDF_D=\
	".\stdafx.h"\
	".\wdf_d.h"\
	".\lng.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\wdf.h"\
	".\lbo.h"\
	".\edlngctl.h"\
	".\set_d.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\wdf_d.obj" : $(SOURCE) $(DEP_CPP_WDF_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_WDF_D=\
	".\stdafx.h"\
	".\wdf_d.h"\
	".\lng.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\wdf.h"\
	".\lbo.h"\
	".\edlngctl.h"\
	".\set_d.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\wdf_d.obj" : $(SOURCE) $(DEP_CPP_WDF_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_WDF_D=\
	".\stdafx.h"\
	".\wdf_d.h"\
	".\lng.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\wdf.h"\
	".\lbo.h"\
	".\edlngctl.h"\
	".\set_d.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\wdf_d.obj" : $(SOURCE) $(DEP_CPP_WDF_D) "$(INTDIR)"

"$(INTDIR)\wdf_d.sbr" : $(SOURCE) $(DEP_CPP_WDF_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wdf.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wdf.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_WDF_C=\
	".\stdafx.h"\
	".\wdf.h"\
	".\typ.h"\
	".\interlin.h"\
	".\ind.h"\
	".\obstream.h"\
	".\set_d.h"\
	".\wdf_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\project.h"\
	".\trie.h"\
	".\crlookel.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\wdf.obj" : $(SOURCE) $(DEP_CPP_WDF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_WDF_C=\
	".\stdafx.h"\
	".\wdf.h"\
	".\typ.h"\
	".\interlin.h"\
	".\ind.h"\
	".\obstream.h"\
	".\set_d.h"\
	".\wdf_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\project.h"\
	".\trie.h"\
	".\crlookel.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\wdf.obj" : $(SOURCE) $(DEP_CPP_WDF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_WDF_C=\
	".\stdafx.h"\
	".\wdf.h"\
	".\typ.h"\
	".\interlin.h"\
	".\ind.h"\
	".\obstream.h"\
	".\set_d.h"\
	".\wdf_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\project.h"\
	".\trie.h"\
	".\crlookel.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\wdf.obj" : $(SOURCE) $(DEP_CPP_WDF_C) "$(INTDIR)"

"$(INTDIR)\wdf.sbr" : $(SOURCE) $(DEP_CPP_WDF_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wav.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\wav.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_WAV_C=\
	".\stdafx.h"\
	".\wav.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\wav.obj" : $(SOURCE) $(DEP_CPP_WAV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_WAV_C=\
	".\stdafx.h"\
	".\wav.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\wav.obj" : $(SOURCE) $(DEP_CPP_WAV_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_WAV_C=\
	".\stdafx.h"\
	".\wav.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\wav.obj" : $(SOURCE) $(DEP_CPP_WAV_C) "$(INTDIR)"

"$(INTDIR)\wav.sbr" : $(SOURCE) $(DEP_CPP_WAV_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\VersionDlg.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\VersionDlg.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_VERSI=\
	".\stdafx.h"\
	".\shw.h"\
	".\VersionDlg.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\VersionDlg.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_VERSI=\
	".\stdafx.h"\
	".\shw.h"\
	".\VersionDlg.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\VersionDlg.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_VERSI=\
	".\stdafx.h"\
	".\shw.h"\
	".\VersionDlg.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\VersionDlg.obj" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"

"$(INTDIR)\VersionDlg.sbr" : $(SOURCE) $(DEP_CPP_VERSI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\var_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\var_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_VAR_D=\
	".\stdafx.h"\
	".\var_d.h"\
	".\lng.h"\
	".\not.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\var.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\var_d.obj" : $(SOURCE) $(DEP_CPP_VAR_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_VAR_D=\
	".\stdafx.h"\
	".\var_d.h"\
	".\lng.h"\
	".\not.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\var.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\var_d.obj" : $(SOURCE) $(DEP_CPP_VAR_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_VAR_D=\
	".\stdafx.h"\
	".\var_d.h"\
	".\lng.h"\
	".\not.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\var.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\var_d.obj" : $(SOURCE) $(DEP_CPP_VAR_D) "$(INTDIR)"

"$(INTDIR)\var_d.sbr" : $(SOURCE) $(DEP_CPP_VAR_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\var.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\var.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_VAR_C=\
	".\stdafx.h"\
	".\var.h"\
	".\patch.h"\
	".\lng.h"\
	".\obstream.h"\
	".\not.h"\
	".\project.h"\
	".\var_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\var.obj" : $(SOURCE) $(DEP_CPP_VAR_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_VAR_C=\
	".\stdafx.h"\
	".\var.h"\
	".\patch.h"\
	".\lng.h"\
	".\obstream.h"\
	".\not.h"\
	".\project.h"\
	".\var_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\var.obj" : $(SOURCE) $(DEP_CPP_VAR_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_VAR_C=\
	".\stdafx.h"\
	".\var.h"\
	".\patch.h"\
	".\lng.h"\
	".\obstream.h"\
	".\not.h"\
	".\project.h"\
	".\var_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\srt.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\var.obj" : $(SOURCE) $(DEP_CPP_VAR_C) "$(INTDIR)"

"$(INTDIR)\var.sbr" : $(SOURCE) $(DEP_CPP_VAR_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\usp10.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\update.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\update.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_UPDAT=\
	".\stdafx.h"\
	".\update.h"\
	".\mkr.h"\
	".\fil.h"\
	".\lng.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\patmch.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_UPDAT=\
	".\stdafx.h"\
	".\update.h"\
	".\mkr.h"\
	".\fil.h"\
	".\lng.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\patmch.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_UPDAT=\
	".\stdafx.h"\
	".\update.h"\
	".\mkr.h"\
	".\fil.h"\
	".\lng.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\patmch.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\update.obj" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"

"$(INTDIR)\update.sbr" : $(SOURCE) $(DEP_CPP_UPDAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\undo.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\typ_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\typ_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TYP_D=\
	".\stdafx.h"\
	".\typ_d.h"\
	".\ddx.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ_d.obj" : $(SOURCE) $(DEP_CPP_TYP_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TYP_D=\
	".\stdafx.h"\
	".\typ_d.h"\
	".\ddx.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ_d.obj" : $(SOURCE) $(DEP_CPP_TYP_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TYP_D=\
	".\stdafx.h"\
	".\typ_d.h"\
	".\ddx.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ_d.obj" : $(SOURCE) $(DEP_CPP_TYP_D) "$(INTDIR)"

"$(INTDIR)\typ_d.sbr" : $(SOURCE) $(DEP_CPP_TYP_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\typ.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\typ.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TYP_C=\
	".\stdafx.h"\
	".\typ.h"\
	".\tools.h"\
	".\spath_d.h"\
	".\project.h"\
	".\cct.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\export.h"\
	".\typ_d.h"\
	".\lbo.h"\
	".\nlstream.h"\
	".\dirlist.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ.obj" : $(SOURCE) $(DEP_CPP_TYP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TYP_C=\
	".\stdafx.h"\
	".\typ.h"\
	".\tools.h"\
	".\spath_d.h"\
	".\project.h"\
	".\cct.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\export.h"\
	".\typ_d.h"\
	".\lbo.h"\
	".\nlstream.h"\
	".\dirlist.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ.obj" : $(SOURCE) $(DEP_CPP_TYP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TYP_C=\
	".\stdafx.h"\
	".\typ.h"\
	".\tools.h"\
	".\spath_d.h"\
	".\project.h"\
	".\cct.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\export.h"\
	".\typ_d.h"\
	".\lbo.h"\
	".\nlstream.h"\
	".\dirlist.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\typ.obj" : $(SOURCE) $(DEP_CPP_TYP_C) "$(INTDIR)"

"$(INTDIR)\typ.sbr" : $(SOURCE) $(DEP_CPP_TYP_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\trie.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\trie.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TRIE_=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\intprc_d.h"\
	".\trie.h"\
	".\progress.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\dbref.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\prog_d.h"\
	

"$(INTDIR)\trie.obj" : $(SOURCE) $(DEP_CPP_TRIE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TRIE_=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\intprc_d.h"\
	".\trie.h"\
	".\progress.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\dbref.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\prog_d.h"\
	

"$(INTDIR)\trie.obj" : $(SOURCE) $(DEP_CPP_TRIE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TRIE_=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\intprc_d.h"\
	".\trie.h"\
	".\progress.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\dbref.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\prog_d.h"\
	

"$(INTDIR)\trie.obj" : $(SOURCE) $(DEP_CPP_TRIE_) "$(INTDIR)"

"$(INTDIR)\trie.sbr" : $(SOURCE) $(DEP_CPP_TRIE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tplt_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tplt_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TPLT_=\
	".\stdafx.h"\
	".\tplt_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tplt_d.obj" : $(SOURCE) $(DEP_CPP_TPLT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TPLT_=\
	".\stdafx.h"\
	".\tplt_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tplt_d.obj" : $(SOURCE) $(DEP_CPP_TPLT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TPLT_=\
	".\stdafx.h"\
	".\tplt_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tplt_d.obj" : $(SOURCE) $(DEP_CPP_TPLT_) "$(INTDIR)"

"$(INTDIR)\tplt_d.sbr" : $(SOURCE) $(DEP_CPP_TPLT_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tools.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tools.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TOOLS=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tools.obj" : $(SOURCE) $(DEP_CPP_TOOLS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TOOLS=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tools.obj" : $(SOURCE) $(DEP_CPP_TOOLS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TOOLS=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\tools.obj" : $(SOURCE) $(DEP_CPP_TOOLS) "$(INTDIR)"

"$(INTDIR)\tools.sbr" : $(SOURCE) $(DEP_CPP_TOOLS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\tom.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TOM_C=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\tom.obj" : $(SOURCE) $(DEP_CPP_TOM_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TOM_C=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\tom.obj" : $(SOURCE) $(DEP_CPP_TOM_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TOM_C=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\tom.obj" : $(SOURCE) $(DEP_CPP_TOM_C) "$(INTDIR)"

"$(INTDIR)\tom.sbr" : $(SOURCE) $(DEP_CPP_TOM_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\test_all.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_TEST_=\
	".\stdafx.h"\
	".\fil.h"\
	".\ind.h"\
	".\lng.h"\
	".\loc.h"\
	".\mkr.h"\
	".\srt.h"\
	".\var.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\crlookel.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\km95api.h"\
	".\cct.h"\
	

"$(INTDIR)\test_all.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_TEST_=\
	".\stdafx.h"\
	".\fil.h"\
	".\ind.h"\
	".\lng.h"\
	".\loc.h"\
	".\mkr.h"\
	".\srt.h"\
	".\var.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\crlookel.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\km95api.h"\
	".\cct.h"\
	

"$(INTDIR)\test_all.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_TEST_=\
	".\stdafx.h"\
	".\fil.h"\
	".\ind.h"\
	".\lng.h"\
	".\loc.h"\
	".\mkr.h"\
	".\srt.h"\
	".\var.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\crlookel.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\km95api.h"\
	".\cct.h"\
	

"$(INTDIR)\test_all.obj" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"

"$(INTDIR)\test_all.sbr" : $(SOURCE) $(DEP_CPP_TEST_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\subfield.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\subfield.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SUBFI=\
	".\stdafx.h"\
	".\subfield.h"\
	".\shwdefs.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\subfield.obj" : $(SOURCE) $(DEP_CPP_SUBFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SUBFI=\
	".\stdafx.h"\
	".\subfield.h"\
	".\shwdefs.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\subfield.obj" : $(SOURCE) $(DEP_CPP_SUBFI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SUBFI=\
	".\stdafx.h"\
	".\subfield.h"\
	".\shwdefs.h"\
	".\crecpos.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\subfield.obj" : $(SOURCE) $(DEP_CPP_SUBFI) "$(INTDIR)"

"$(INTDIR)\subfield.sbr" : $(SOURCE) $(DEP_CPP_SUBFI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stringex.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stringex.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_STRIN=\
	".\stdafx.h"\
	".\stringex.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stringex.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_STRIN=\
	".\stdafx.h"\
	".\stringex.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stringex.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_STRIN=\
	".\stdafx.h"\
	".\stringex.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stringex.obj" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"

"$(INTDIR)\stringex.sbr" : $(SOURCE) $(DEP_CPP_STRIN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdiofil.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdiofil.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_STDIO=\
	".\stdafx.h"\
	".\stdiofil.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdiofil.obj" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_STDIO=\
	".\stdafx.h"\
	".\stdiofil.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdiofil.obj" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_STDIO=\
	".\stdafx.h"\
	".\stdiofil.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdiofil.obj" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"

"$(INTDIR)\stdiofil.sbr" : $(SOURCE) $(DEP_CPP_STDIO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdafx.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\stdafx.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_STDAF=\
	".\stdafx.h"\
	".\tools.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\stdafx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

"$(INTDIR)\stdafx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\status.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\status.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_STATU=\
	".\stdafx.h"\
	".\status.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\status.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_STATU=\
	".\stdafx.h"\
	".\status.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\status.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_STATU=\
	".\stdafx.h"\
	".\status.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\status.obj" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"

"$(INTDIR)\status.sbr" : $(SOURCE) $(DEP_CPP_STATU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srtset_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srtset_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SRTSE=\
	".\stdafx.h"\
	".\srtset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\srt.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	

"$(INTDIR)\srtset_d.obj" : $(SOURCE) $(DEP_CPP_SRTSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SRTSE=\
	".\stdafx.h"\
	".\srtset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\srt.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	

"$(INTDIR)\srtset_d.obj" : $(SOURCE) $(DEP_CPP_SRTSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SRTSE=\
	".\stdafx.h"\
	".\srtset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\srt.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	

"$(INTDIR)\srtset_d.obj" : $(SOURCE) $(DEP_CPP_SRTSE) "$(INTDIR)"

"$(INTDIR)\srtset_d.sbr" : $(SOURCE) $(DEP_CPP_SRTSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srt_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srt_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SRT_D=\
	".\stdafx.h"\
	".\lng.h"\
	".\srt_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt_d.obj" : $(SOURCE) $(DEP_CPP_SRT_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SRT_D=\
	".\stdafx.h"\
	".\lng.h"\
	".\srt_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt_d.obj" : $(SOURCE) $(DEP_CPP_SRT_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SRT_D=\
	".\stdafx.h"\
	".\lng.h"\
	".\srt_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt_d.obj" : $(SOURCE) $(DEP_CPP_SRT_D) "$(INTDIR)"

"$(INTDIR)\srt_d.sbr" : $(SOURCE) $(DEP_CPP_SRT_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srt.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\srt.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SRT_C=\
	".\stdafx.h"\
	".\srt.h"\
	".\lng.h"\
	".\obstream.h"\
	".\patch.h"\
	".\not.h"\
	".\tools.h"\
	".\srt_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt.obj" : $(SOURCE) $(DEP_CPP_SRT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SRT_C=\
	".\stdafx.h"\
	".\srt.h"\
	".\lng.h"\
	".\obstream.h"\
	".\patch.h"\
	".\not.h"\
	".\tools.h"\
	".\srt_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt.obj" : $(SOURCE) $(DEP_CPP_SRT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SRT_C=\
	".\stdafx.h"\
	".\srt.h"\
	".\lng.h"\
	".\obstream.h"\
	".\patch.h"\
	".\not.h"\
	".\tools.h"\
	".\srt_d.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\edlngctl.h"\
	".\EditLng.h"\
	

"$(INTDIR)\srt.obj" : $(SOURCE) $(DEP_CPP_SRT_C) "$(INTDIR)"

"$(INTDIR)\srt.sbr" : $(SOURCE) $(DEP_CPP_SRT_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\spath_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\spath_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SPATH=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\doclist.h"\
	".\spath_d.h"\
	".\seldb_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\spath_d.obj" : $(SOURCE) $(DEP_CPP_SPATH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SPATH=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\doclist.h"\
	".\spath_d.h"\
	".\seldb_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\spath_d.obj" : $(SOURCE) $(DEP_CPP_SPATH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SPATH=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\doclist.h"\
	".\spath_d.h"\
	".\seldb_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\spath_d.obj" : $(SOURCE) $(DEP_CPP_SPATH) "$(INTDIR)"

"$(INTDIR)\spath_d.sbr" : $(SOURCE) $(DEP_CPP_SPATH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvwrap.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVW=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwvwrap.obj" : $(SOURCE) $(DEP_CPP_SHWVW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVW=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwvwrap.obj" : $(SOURCE) $(DEP_CPP_SHWVW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVW=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwvwrap.obj" : $(SOURCE) $(DEP_CPP_SHWVW) "$(INTDIR)"

"$(INTDIR)\shwvwrap.sbr" : $(SOURCE) $(DEP_CPP_SHWVW) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvundo.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVU=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvundo.obj" : $(SOURCE) $(DEP_CPP_SHWVU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVU=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvundo.obj" : $(SOURCE) $(DEP_CPP_SHWVU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVU=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvundo.obj" : $(SOURCE) $(DEP_CPP_SHWVU) "$(INTDIR)"

"$(INTDIR)\shwvundo.sbr" : $(SOURCE) $(DEP_CPP_SHWVU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvscrl.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvscrl.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVS=\
	".\stdafx.h"\
	".\shwvscrl.h"\
	".\mainfrm.h"\
	".\shwdtplt.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\shwvscrl.obj" : $(SOURCE) $(DEP_CPP_SHWVS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVS=\
	".\stdafx.h"\
	".\shwvscrl.h"\
	".\mainfrm.h"\
	".\shwdtplt.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\shwvscrl.obj" : $(SOURCE) $(DEP_CPP_SHWVS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVS=\
	".\stdafx.h"\
	".\shwvscrl.h"\
	".\mainfrm.h"\
	".\shwdtplt.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\shwvscrl.obj" : $(SOURCE) $(DEP_CPP_SHWVS) "$(INTDIR)"

"$(INTDIR)\shwvscrl.sbr" : $(SOURCE) $(DEP_CPP_SHWVS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvprnt.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVP=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\page_d.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvprnt.obj" : $(SOURCE) $(DEP_CPP_SHWVP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVP=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\page_d.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvprnt.obj" : $(SOURCE) $(DEP_CPP_SHWVP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVP=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\page_d.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwvprnt.obj" : $(SOURCE) $(DEP_CPP_SHWVP) "$(INTDIR)"

"$(INTDIR)\shwvprnt.sbr" : $(SOURCE) $(DEP_CPP_SHWVP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwview.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwview.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVI=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\find_d.h"\
	".\obstream.h"\
	".\kmn.h"\
	".\mkr_d.h"\
	".\wav.h"\
	".\doclist.h"\
	".\cck.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	

"$(INTDIR)\shwview.obj" : $(SOURCE) $(DEP_CPP_SHWVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVI=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\find_d.h"\
	".\obstream.h"\
	".\kmn.h"\
	".\mkr_d.h"\
	".\wav.h"\
	".\doclist.h"\
	".\cck.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	

"$(INTDIR)\shwview.obj" : $(SOURCE) $(DEP_CPP_SHWVI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVI=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\find_d.h"\
	".\obstream.h"\
	".\kmn.h"\
	".\mkr_d.h"\
	".\wav.h"\
	".\doclist.h"\
	".\cck.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	

"$(INTDIR)\shwview.obj" : $(SOURCE) $(DEP_CPP_SHWVI) "$(INTDIR)"

"$(INTDIR)\shwview.sbr" : $(SOURCE) $(DEP_CPP_SHWVI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvfind.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVF=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\progress.h"\
	".\find_d.h"\
	".\kmn.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\shwvfind.obj" : $(SOURCE) $(DEP_CPP_SHWVF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVF=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\progress.h"\
	".\find_d.h"\
	".\kmn.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\shwvfind.obj" : $(SOURCE) $(DEP_CPP_SHWVF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVF=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\progress.h"\
	".\find_d.h"\
	".\kmn.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\shwvfind.obj" : $(SOURCE) $(DEP_CPP_SHWVF) "$(INTDIR)"

"$(INTDIR)\shwvfind.sbr" : $(SOURCE) $(DEP_CPP_SHWVF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvdis.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVD=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\mkrfed_d.h"\
	".\jumpdlg.h"\
	".\tools.h"\
	".\typ.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\tplt_d.h"\
	".\kmn.h"\
	".\crngset.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\cct.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\crecpos.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	
NODEP_CPP_SHWVD=\
	".\script.h"\
	

"$(INTDIR)\shwvdis.obj" : $(SOURCE) $(DEP_CPP_SHWVD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVD=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\mkrfed_d.h"\
	".\jumpdlg.h"\
	".\tools.h"\
	".\typ.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\tplt_d.h"\
	".\kmn.h"\
	".\crngset.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\cct.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\crecpos.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\shwvdis.obj" : $(SOURCE) $(DEP_CPP_SHWVD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVD=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\status.h"\
	".\mkrfed_d.h"\
	".\jumpdlg.h"\
	".\tools.h"\
	".\typ.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\tplt_d.h"\
	".\kmn.h"\
	".\crngset.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\cct.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\crecpos.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\shwvdis.obj" : $(SOURCE) $(DEP_CPP_SHWVD) "$(INTDIR)"

"$(INTDIR)\shwvdis.sbr" : $(SOURCE) $(DEP_CPP_SHWVD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvdat.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVDA=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\seldb_d.h"\
	".\ind.h"\
	".\progress.h"\
	".\crngset.h"\
	".\cck.h"\
	".\browse.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\crlookel.h"\
	".\prog_d.h"\
	".\obstream.h"\
	

"$(INTDIR)\shwvdat.obj" : $(SOURCE) $(DEP_CPP_SHWVDA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVDA=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\seldb_d.h"\
	".\ind.h"\
	".\progress.h"\
	".\crngset.h"\
	".\cck.h"\
	".\browse.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\crlookel.h"\
	".\prog_d.h"\
	".\obstream.h"\
	

"$(INTDIR)\shwvdat.obj" : $(SOURCE) $(DEP_CPP_SHWVDA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVDA=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\seldb_d.h"\
	".\ind.h"\
	".\progress.h"\
	".\crngset.h"\
	".\cck.h"\
	".\browse.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\crlookel.h"\
	".\prog_d.h"\
	".\obstream.h"\
	

"$(INTDIR)\shwvdat.obj" : $(SOURCE) $(DEP_CPP_SHWVDA) "$(INTDIR)"

"$(INTDIR)\shwvdat.sbr" : $(SOURCE) $(DEP_CPP_SHWVDA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwvbrs.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWVB=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\mkr_d.h"\
	".\browse.h"\
	".\mainfrm.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	

"$(INTDIR)\shwvbrs.obj" : $(SOURCE) $(DEP_CPP_SHWVB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWVB=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\mkr_d.h"\
	".\browse.h"\
	".\mainfrm.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	

"$(INTDIR)\shwvbrs.obj" : $(SOURCE) $(DEP_CPP_SHWVB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWVB=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\mkr_d.h"\
	".\browse.h"\
	".\mainfrm.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	

"$(INTDIR)\shwvbrs.obj" : $(SOURCE) $(DEP_CPP_SHWVB) "$(INTDIR)"

"$(INTDIR)\shwvbrs.sbr" : $(SOURCE) $(DEP_CPP_SHWVB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwpc.rc

!IF  "$(CFG)" == "Toolbox - Win32 Release"


"$(INTDIR)\shwpc.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"


"$(INTDIR)\shwpc.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"


"$(INTDIR)\shwmac.rsc" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwnotes.rc

!IF  "$(CFG)" == "Toolbox - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwnotes.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwmac.rc

!IF  "$(CFG)" == "Toolbox - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdtplt.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdtplt.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWDT=\
	".\stdafx.h"\
	".\shwdtplt.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwdtplt.obj" : $(SOURCE) $(DEP_CPP_SHWDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWDT=\
	".\stdafx.h"\
	".\shwdtplt.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwdtplt.obj" : $(SOURCE) $(DEP_CPP_SHWDT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWDT=\
	".\stdafx.h"\
	".\shwdtplt.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\shwdtplt.obj" : $(SOURCE) $(DEP_CPP_SHWDT) "$(INTDIR)"

"$(INTDIR)\shwdtplt.sbr" : $(SOURCE) $(DEP_CPP_SHWDT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdoc.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdoc.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\newdoc_d.h"\
	".\obstream.h"\
	".\progress.h"\
	".\status.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdoc.obj" : $(SOURCE) $(DEP_CPP_SHWDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\newdoc_d.h"\
	".\obstream.h"\
	".\progress.h"\
	".\status.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdoc.obj" : $(SOURCE) $(DEP_CPP_SHWDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\newdoc_d.h"\
	".\obstream.h"\
	".\progress.h"\
	".\status.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdoc.obj" : $(SOURCE) $(DEP_CPP_SHWDO) "$(INTDIR)"

"$(INTDIR)\shwdoc.sbr" : $(SOURCE) $(DEP_CPP_SHWDO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdio.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHWDI=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\dlgimpt.h"\
	".\tools.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdio.obj" : $(SOURCE) $(DEP_CPP_SHWDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHWDI=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\dlgimpt.h"\
	".\tools.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdio.obj" : $(SOURCE) $(DEP_CPP_SHWDI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHWDI=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\dlgimpt.h"\
	".\tools.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shwdio.obj" : $(SOURCE) $(DEP_CPP_SHWDI) "$(INTDIR)"

"$(INTDIR)\shwdio.sbr" : $(SOURCE) $(DEP_CPP_SHWDI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shwdefs.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Shw.rc

!IF  "$(CFG)" == "Toolbox - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shw.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\shw.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SHW_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdtplt.h"\
	".\tools.h"\
	".\font.h"\
	".\find_d.h"\
	".\status.h"\
	".\project.h"\
	".\cct.h"\
	".\doclist.h"\
	".\obstream.h"\
	".\wrdlst_d.h"\
	".\corpus.h"\
	".\progress.h"\
	".\rundos.h"\
	".\help_ids.h"\
	".\VersionDlg.h"\
	".\corpus_d.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shw.obj" : $(SOURCE) $(DEP_CPP_SHW_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SHW_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdtplt.h"\
	".\tools.h"\
	".\font.h"\
	".\find_d.h"\
	".\status.h"\
	".\project.h"\
	".\cct.h"\
	".\doclist.h"\
	".\obstream.h"\
	".\wrdlst_d.h"\
	".\corpus.h"\
	".\progress.h"\
	".\rundos.h"\
	".\help_ids.h"\
	".\VersionDlg.h"\
	".\corpus_d.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shw.obj" : $(SOURCE) $(DEP_CPP_SHW_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SHW_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdtplt.h"\
	".\tools.h"\
	".\font.h"\
	".\find_d.h"\
	".\status.h"\
	".\project.h"\
	".\cct.h"\
	".\doclist.h"\
	".\obstream.h"\
	".\wrdlst_d.h"\
	".\corpus.h"\
	".\progress.h"\
	".\rundos.h"\
	".\help_ids.h"\
	".\VersionDlg.h"\
	".\corpus_d.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\var.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\shw.obj" : $(SOURCE) $(DEP_CPP_SHW_C) "$(INTDIR)"

"$(INTDIR)\shw.sbr" : $(SOURCE) $(DEP_CPP_SHW_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\SHConv.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sfstream.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sfstream.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SFSTR=\
	".\stdafx.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\crecord.h"\
	".\patch.h"\
	".\not.h"\
	".\crecpos.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\cfield.h"\
	".\tools.h"\
	

"$(INTDIR)\sfstream.obj" : $(SOURCE) $(DEP_CPP_SFSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SFSTR=\
	".\stdafx.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\crecord.h"\
	".\patch.h"\
	".\not.h"\
	".\crecpos.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\cfield.h"\
	".\tools.h"\
	

"$(INTDIR)\sfstream.obj" : $(SOURCE) $(DEP_CPP_SFSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SFSTR=\
	".\stdafx.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\crecord.h"\
	".\patch.h"\
	".\not.h"\
	".\crecpos.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\cfield.h"\
	".\tools.h"\
	

"$(INTDIR)\sfstream.obj" : $(SOURCE) $(DEP_CPP_SFSTR) "$(INTDIR)"

"$(INTDIR)\sfstream.sbr" : $(SOURCE) $(DEP_CPP_SFSTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\setsel.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\set_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\set_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SET_D=\
	".\stdafx.h"\
	".\set_d.h"\
	".\set.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\set_d.obj" : $(SOURCE) $(DEP_CPP_SET_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SET_D=\
	".\stdafx.h"\
	".\set_d.h"\
	".\set.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\set_d.obj" : $(SOURCE) $(DEP_CPP_SET_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SET_D=\
	".\stdafx.h"\
	".\set_d.h"\
	".\set.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\set_d.obj" : $(SOURCE) $(DEP_CPP_SET_D) "$(INTDIR)"

"$(INTDIR)\set_d.sbr" : $(SOURCE) $(DEP_CPP_SET_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\set.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\set.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SET_C=\
	".\stdafx.h"\
	".\set.h"\
	".\not.h"\
	".\set_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lbo.h"\
	

"$(INTDIR)\set.obj" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SET_C=\
	".\stdafx.h"\
	".\set.h"\
	".\not.h"\
	".\set_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lbo.h"\
	

"$(INTDIR)\set.obj" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SET_C=\
	".\stdafx.h"\
	".\set.h"\
	".\not.h"\
	".\set_d.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lbo.h"\
	

"$(INTDIR)\set.obj" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"

"$(INTDIR)\set.sbr" : $(SOURCE) $(DEP_CPP_SET_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\seldb_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\seldb_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_SELDB=\
	".\stdafx.h"\
	".\dbref.h"\
	".\seldb_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crecpos.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\seldb_d.obj" : $(SOURCE) $(DEP_CPP_SELDB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_SELDB=\
	".\stdafx.h"\
	".\dbref.h"\
	".\seldb_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crecpos.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\seldb_d.obj" : $(SOURCE) $(DEP_CPP_SELDB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_SELDB=\
	".\stdafx.h"\
	".\dbref.h"\
	".\seldb_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crecpos.h"\
	".\obstream.h"\
	".\crngset.h"\
	

"$(INTDIR)\seldb_d.obj" : $(SOURCE) $(DEP_CPP_SELDB) "$(INTDIR)"

"$(INTDIR)\seldb_d.sbr" : $(SOURCE) $(DEP_CPP_SELDB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rundos.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rundos.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_RUNDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\rundos.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\rundos.obj" : $(SOURCE) $(DEP_CPP_RUNDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_RUNDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\rundos.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\rundos.obj" : $(SOURCE) $(DEP_CPP_RUNDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_RUNDO=\
	".\stdafx.h"\
	".\shw.h"\
	".\rundos.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\rundos.obj" : $(SOURCE) $(DEP_CPP_RUNDO) "$(INTDIR)"

"$(INTDIR)\rundos.sbr" : $(SOURCE) $(DEP_CPP_RUNDO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rtf.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rtf.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_RTF_C=\
	".\stdafx.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\set.h"\
	".\cfield.h"\
	".\font.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\tools.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\rtf.obj" : $(SOURCE) $(DEP_CPP_RTF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_RTF_C=\
	".\stdafx.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\set.h"\
	".\cfield.h"\
	".\font.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\tools.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\rtf.obj" : $(SOURCE) $(DEP_CPP_RTF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_RTF_C=\
	".\stdafx.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\set.h"\
	".\cfield.h"\
	".\font.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\mkr.h"\
	".\tools.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\rtf.obj" : $(SOURCE) $(DEP_CPP_RTF_C) "$(INTDIR)"

"$(INTDIR)\rtf.sbr" : $(SOURCE) $(DEP_CPP_RTF_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rod.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_ROD_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\rod.obj" : $(SOURCE) $(DEP_CPP_ROD_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_ROD_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\rod.obj" : $(SOURCE) $(DEP_CPP_ROD_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_ROD_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\rod.obj" : $(SOURCE) $(DEP_CPP_ROD_C) "$(INTDIR)"

"$(INTDIR)\rod.sbr" : $(SOURCE) $(DEP_CPP_ROD_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\respc.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\resource.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\resmac.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ref.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ref.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_REF_C=\
	".\stdafx.h"\
	".\ref.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\ref.obj" : $(SOURCE) $(DEP_CPP_REF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_REF_C=\
	".\stdafx.h"\
	".\ref.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\ref.obj" : $(SOURCE) $(DEP_CPP_REF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_REF_C=\
	".\stdafx.h"\
	".\ref.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\ref.obj" : $(SOURCE) $(DEP_CPP_REF_C) "$(INTDIR)"

"$(INTDIR)\ref.sbr" : $(SOURCE) $(DEP_CPP_REF_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\recset.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\recset.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_RECSE=\
	".\stdafx.h"\
	".\recset.h"\
	".\obstream.h"\
	".\key_d.h"\
	".\multmat.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	

"$(INTDIR)\recset.obj" : $(SOURCE) $(DEP_CPP_RECSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_RECSE=\
	".\stdafx.h"\
	".\recset.h"\
	".\obstream.h"\
	".\key_d.h"\
	".\multmat.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	

"$(INTDIR)\recset.obj" : $(SOURCE) $(DEP_CPP_RECSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_RECSE=\
	".\stdafx.h"\
	".\recset.h"\
	".\obstream.h"\
	".\key_d.h"\
	".\multmat.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	

"$(INTDIR)\recset.obj" : $(SOURCE) $(DEP_CPP_RECSE) "$(INTDIR)"

"$(INTDIR)\recset.sbr" : $(SOURCE) $(DEP_CPP_RECSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ptr.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\project.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\project.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PROJE=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\lng.h"\
	".\typ.h"\
	".\kmn.h"\
	".\find_d.h"\
	".\tools.h"\
	".\font.h"\
	".\obstream.h"\
	".\corpus.h"\
	".\rundos.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\cct.h"\
	".\not.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\patch.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\project.obj" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PROJE=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\lng.h"\
	".\typ.h"\
	".\kmn.h"\
	".\find_d.h"\
	".\tools.h"\
	".\font.h"\
	".\obstream.h"\
	".\corpus.h"\
	".\rundos.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\cct.h"\
	".\not.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\patch.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\project.obj" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PROJE=\
	".\stdafx.h"\
	".\project.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\lng.h"\
	".\typ.h"\
	".\kmn.h"\
	".\find_d.h"\
	".\tools.h"\
	".\font.h"\
	".\obstream.h"\
	".\corpus.h"\
	".\rundos.h"\
	".\nlstream.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\cct.h"\
	".\not.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\patch.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	

"$(INTDIR)\project.obj" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"

"$(INTDIR)\project.sbr" : $(SOURCE) $(DEP_CPP_PROJE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\progress.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\progress.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PROGR=\
	".\stdafx.h"\
	".\progress.h"\
	".\tools.h"\
	".\status.h"\
	".\shwdefs.h"\
	".\prog_d.h"\
	

"$(INTDIR)\progress.obj" : $(SOURCE) $(DEP_CPP_PROGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PROGR=\
	".\stdafx.h"\
	".\progress.h"\
	".\tools.h"\
	".\status.h"\
	".\shwdefs.h"\
	".\prog_d.h"\
	

"$(INTDIR)\progress.obj" : $(SOURCE) $(DEP_CPP_PROGR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PROGR=\
	".\stdafx.h"\
	".\progress.h"\
	".\tools.h"\
	".\status.h"\
	".\shwdefs.h"\
	".\prog_d.h"\
	

"$(INTDIR)\progress.obj" : $(SOURCE) $(DEP_CPP_PROGR) "$(INTDIR)"

"$(INTDIR)\progress.sbr" : $(SOURCE) $(DEP_CPP_PROGR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\progbar.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\progbar.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PROGB=\
	".\stdafx.h"\
	".\progbar.h"\
	".\shwdefs.h"\
	".\progbar.inl"\
	

"$(INTDIR)\progbar.obj" : $(SOURCE) $(DEP_CPP_PROGB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PROGB=\
	".\stdafx.h"\
	".\progbar.h"\
	".\shwdefs.h"\
	".\progbar.inl"\
	

"$(INTDIR)\progbar.obj" : $(SOURCE) $(DEP_CPP_PROGB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PROGB=\
	".\stdafx.h"\
	".\progbar.h"\
	".\shwdefs.h"\
	".\progbar.inl"\
	

"$(INTDIR)\progbar.obj" : $(SOURCE) $(DEP_CPP_PROGB) "$(INTDIR)"

"$(INTDIR)\progbar.sbr" : $(SOURCE) $(DEP_CPP_PROGB) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\prog_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\prog_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PROG_=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\prog_d.h"\
	".\status.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\prog_d.obj" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PROG_=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\prog_d.h"\
	".\status.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\prog_d.obj" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PROG_=\
	".\stdafx.h"\
	".\shw.h"\
	".\mainfrm.h"\
	".\prog_d.h"\
	".\status.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	

"$(INTDIR)\prog_d.obj" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"

"$(INTDIR)\prog_d.sbr" : $(SOURCE) $(DEP_CPP_PROG_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\profile.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printinf.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printinf.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PRINT=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\printinf.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\printinf.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PRINT=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\printinf.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\printinf.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PRINT=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\printinf.h"\
	".\tools.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\printinf.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

"$(INTDIR)\printinf.sbr" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pgs_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pgs_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PGS_D=\
	".\stdafx.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs_d.obj" : $(SOURCE) $(DEP_CPP_PGS_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PGS_D=\
	".\stdafx.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs_d.obj" : $(SOURCE) $(DEP_CPP_PGS_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PGS_D=\
	".\stdafx.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs_d.obj" : $(SOURCE) $(DEP_CPP_PGS_D) "$(INTDIR)"

"$(INTDIR)\pgs_d.sbr" : $(SOURCE) $(DEP_CPP_PGS_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pgs.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\pgs.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PGS_C=\
	".\stdafx.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs.obj" : $(SOURCE) $(DEP_CPP_PGS_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PGS_C=\
	".\stdafx.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs.obj" : $(SOURCE) $(DEP_CPP_PGS_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PGS_C=\
	".\stdafx.h"\
	".\pgs.h"\
	".\obstream.h"\
	".\pgs_d.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\pgs.obj" : $(SOURCE) $(DEP_CPP_PGS_C) "$(INTDIR)"

"$(INTDIR)\pgs.sbr" : $(SOURCE) $(DEP_CPP_PGS_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\patmch.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\patmch.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PATMC=\
	".\stdafx.h"\
	".\patmch.h"\
	".\patch.h"\
	".\loc.h"\
	".\not.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\lng.h"\
	".\cdblel.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mkr.h"\
	

"$(INTDIR)\patmch.obj" : $(SOURCE) $(DEP_CPP_PATMC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PATMC=\
	".\stdafx.h"\
	".\patmch.h"\
	".\patch.h"\
	".\loc.h"\
	".\not.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\lng.h"\
	".\cdblel.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mkr.h"\
	

"$(INTDIR)\patmch.obj" : $(SOURCE) $(DEP_CPP_PATMC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PATMC=\
	".\stdafx.h"\
	".\patmch.h"\
	".\patch.h"\
	".\loc.h"\
	".\not.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\lng.h"\
	".\cdblel.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mkr.h"\
	

"$(INTDIR)\patmch.obj" : $(SOURCE) $(DEP_CPP_PATMC) "$(INTDIR)"

"$(INTDIR)\patmch.sbr" : $(SOURCE) $(DEP_CPP_PATMC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\patch.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\patch.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PATCH=\
	".\stdafx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\patch.obj" : $(SOURCE) $(DEP_CPP_PATCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PATCH=\
	".\stdafx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\patch.obj" : $(SOURCE) $(DEP_CPP_PATCH) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PATCH=\
	".\stdafx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\patch.obj" : $(SOURCE) $(DEP_CPP_PATCH) "$(INTDIR)"

"$(INTDIR)\patch.sbr" : $(SOURCE) $(DEP_CPP_PATCH) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\page_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\page_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_PAGE_=\
	".\stdafx.h"\
	".\shw.h"\
	".\printinf.h"\
	".\page_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crecpos.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\page_d.obj" : $(SOURCE) $(DEP_CPP_PAGE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_PAGE_=\
	".\stdafx.h"\
	".\shw.h"\
	".\printinf.h"\
	".\page_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crecpos.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\page_d.obj" : $(SOURCE) $(DEP_CPP_PAGE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_PAGE_=\
	".\stdafx.h"\
	".\shw.h"\
	".\printinf.h"\
	".\page_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crecpos.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\page_d.obj" : $(SOURCE) $(DEP_CPP_PAGE_) "$(INTDIR)"

"$(INTDIR)\page_d.sbr" : $(SOURCE) $(DEP_CPP_PAGE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\obstream.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\obstream.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_OBSTR=\
	".\stdafx.h"\
	".\obstream.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\obstream.obj" : $(SOURCE) $(DEP_CPP_OBSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_OBSTR=\
	".\stdafx.h"\
	".\obstream.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\obstream.obj" : $(SOURCE) $(DEP_CPP_OBSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_OBSTR=\
	".\stdafx.h"\
	".\obstream.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\obstream.obj" : $(SOURCE) $(DEP_CPP_OBSTR) "$(INTDIR)"

"$(INTDIR)\obstream.sbr" : $(SOURCE) $(DEP_CPP_OBSTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\number_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\number_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NUMBE=\
	".\stdafx.h"\
	".\number.h"\
	".\number_d.h"\
	".\typ.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\number_d.obj" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NUMBE=\
	".\stdafx.h"\
	".\number.h"\
	".\number_d.h"\
	".\typ.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\number_d.obj" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NUMBE=\
	".\stdafx.h"\
	".\number.h"\
	".\number_d.h"\
	".\typ.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\number_d.obj" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"

"$(INTDIR)\number_d.sbr" : $(SOURCE) $(DEP_CPP_NUMBE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\number.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\number.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NUMBER=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\number.h"\
	".\progress.h"\
	".\ind.h"\
	".\obstream.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\shwview.h"\
	".\number_d.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\not.h"\
	".\km95api.h"\
	".\prog_d.h"\
	".\tools.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\number.obj" : $(SOURCE) $(DEP_CPP_NUMBER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NUMBER=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\number.h"\
	".\progress.h"\
	".\ind.h"\
	".\obstream.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\shwview.h"\
	".\number_d.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\not.h"\
	".\km95api.h"\
	".\prog_d.h"\
	".\tools.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\number.obj" : $(SOURCE) $(DEP_CPP_NUMBER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NUMBER=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\number.h"\
	".\progress.h"\
	".\ind.h"\
	".\obstream.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\shwview.h"\
	".\number_d.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\typ.h"\
	".\set.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lbo.h"\
	".\update.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cbo.h"\
	".\not.h"\
	".\km95api.h"\
	".\prog_d.h"\
	".\tools.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\mrfset_d.h"\
	".\EditLng.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\number.obj" : $(SOURCE) $(DEP_CPP_NUMBER) "$(INTDIR)"

"$(INTDIR)\number.sbr" : $(SOURCE) $(DEP_CPP_NUMBER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\not.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\not.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NOT_C=\
	".\stdafx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\not.obj" : $(SOURCE) $(DEP_CPP_NOT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NOT_C=\
	".\stdafx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\not.obj" : $(SOURCE) $(DEP_CPP_NOT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NOT_C=\
	".\stdafx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\not.obj" : $(SOURCE) $(DEP_CPP_NOT_C) "$(INTDIR)"

"$(INTDIR)\not.sbr" : $(SOURCE) $(DEP_CPP_NOT_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nomat_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nomat_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NOMAT=\
	".\stdafx.h"\
	".\nomat_d.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	

"$(INTDIR)\nomat_d.obj" : $(SOURCE) $(DEP_CPP_NOMAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NOMAT=\
	".\stdafx.h"\
	".\nomat_d.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	

"$(INTDIR)\nomat_d.obj" : $(SOURCE) $(DEP_CPP_NOMAT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NOMAT=\
	".\stdafx.h"\
	".\nomat_d.h"\
	".\shwdefs.h"\
	".\EditLng.h"\
	

"$(INTDIR)\nomat_d.obj" : $(SOURCE) $(DEP_CPP_NOMAT) "$(INTDIR)"

"$(INTDIR)\nomat_d.sbr" : $(SOURCE) $(DEP_CPP_NOMAT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nlstream.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\nlstream.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NLSTR=\
	".\nlstream.h"\
	

"$(INTDIR)\nlstream.obj" : $(SOURCE) $(DEP_CPP_NLSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NLSTR=\
	".\nlstream.h"\
	

"$(INTDIR)\nlstream.obj" : $(SOURCE) $(DEP_CPP_NLSTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NLSTR=\
	".\nlstream.h"\
	

"$(INTDIR)\nlstream.obj" : $(SOURCE) $(DEP_CPP_NLSTR) "$(INTDIR)"

"$(INTDIR)\nlstream.sbr" : $(SOURCE) $(DEP_CPP_NLSTR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\newdoc_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\newdoc_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_NEWDO=\
	".\stdafx.h"\
	".\project.h"\
	".\newdoc_d.h"\
	".\shwdefs.h"\
	".\typ_d.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\newdoc_d.obj" : $(SOURCE) $(DEP_CPP_NEWDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_NEWDO=\
	".\stdafx.h"\
	".\project.h"\
	".\newdoc_d.h"\
	".\shwdefs.h"\
	".\typ_d.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\newdoc_d.obj" : $(SOURCE) $(DEP_CPP_NEWDO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_NEWDO=\
	".\stdafx.h"\
	".\project.h"\
	".\newdoc_d.h"\
	".\shwdefs.h"\
	".\typ_d.h"\
	".\typ.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\ptr.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\newdoc_d.obj" : $(SOURCE) $(DEP_CPP_NEWDO) "$(INTDIR)"

"$(INTDIR)\newdoc_d.sbr" : $(SOURCE) $(DEP_CPP_NEWDO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MyMLang.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\multmat.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\multmat.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MULTM=\
	".\stdafx.h"\
	".\project.h"\
	".\multmat.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\dbref.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	

"$(INTDIR)\multmat.obj" : $(SOURCE) $(DEP_CPP_MULTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MULTM=\
	".\stdafx.h"\
	".\project.h"\
	".\multmat.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\dbref.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	

"$(INTDIR)\multmat.obj" : $(SOURCE) $(DEP_CPP_MULTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MULTM=\
	".\stdafx.h"\
	".\project.h"\
	".\multmat.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\dbref.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	

"$(INTDIR)\multmat.obj" : $(SOURCE) $(DEP_CPP_MULTM) "$(INTDIR)"

"$(INTDIR)\multmat.sbr" : $(SOURCE) $(DEP_CPP_MULTM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mrfset_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mrfset_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MRFSE=\
	".\stdafx.h"\
	".\mrfset_d.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\mkr.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mrfset_d.obj" : $(SOURCE) $(DEP_CPP_MRFSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MRFSE=\
	".\stdafx.h"\
	".\mrfset_d.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\mkr.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mrfset_d.obj" : $(SOURCE) $(DEP_CPP_MRFSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MRFSE=\
	".\stdafx.h"\
	".\mrfset_d.h"\
	".\shwdefs.h"\
	".\lbo.h"\
	".\mkr.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mrfset_d.obj" : $(SOURCE) $(DEP_CPP_MRFSE) "$(INTDIR)"

"$(INTDIR)\mrfset_d.sbr" : $(SOURCE) $(DEP_CPP_MRFSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkrset_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkrset_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MKRSE=\
	".\stdafx.h"\
	".\mkrset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrset_d.obj" : $(SOURCE) $(DEP_CPP_MKRSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MKRSE=\
	".\stdafx.h"\
	".\mkrset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrset_d.obj" : $(SOURCE) $(DEP_CPP_MKRSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MKRSE=\
	".\stdafx.h"\
	".\mkrset_d.h"\
	".\shwdefs.h"\
	".\set_d.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrset_d.obj" : $(SOURCE) $(DEP_CPP_MKRSE) "$(INTDIR)"

"$(INTDIR)\mkrset_d.sbr" : $(SOURCE) $(DEP_CPP_MKRSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkrfed_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkrfed_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MKRFE=\
	".\stdafx.h"\
	".\project.h"\
	".\kmn.h"\
	".\mkrfed_d.h"\
	".\shwdefs.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrfed_d.obj" : $(SOURCE) $(DEP_CPP_MKRFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MKRFE=\
	".\stdafx.h"\
	".\project.h"\
	".\kmn.h"\
	".\mkrfed_d.h"\
	".\shwdefs.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrfed_d.obj" : $(SOURCE) $(DEP_CPP_MKRFE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MKRFE=\
	".\stdafx.h"\
	".\project.h"\
	".\kmn.h"\
	".\mkrfed_d.h"\
	".\shwdefs.h"\
	".\cfield.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkrfed_d.obj" : $(SOURCE) $(DEP_CPP_MKRFE) "$(INTDIR)"

"$(INTDIR)\mkrfed_d.sbr" : $(SOURCE) $(DEP_CPP_MKRFE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkr_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkr_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MKR_D=\
	".\stdafx.h"\
	".\mkr_d.h"\
	".\not.h"\
	".\ddx.h"\
	".\typ.h"\
	".\project.h"\
	".\crngst_d.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\font.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	

"$(INTDIR)\mkr_d.obj" : $(SOURCE) $(DEP_CPP_MKR_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MKR_D=\
	".\stdafx.h"\
	".\mkr_d.h"\
	".\not.h"\
	".\ddx.h"\
	".\typ.h"\
	".\project.h"\
	".\crngst_d.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\font.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	

"$(INTDIR)\mkr_d.obj" : $(SOURCE) $(DEP_CPP_MKR_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MKR_D=\
	".\stdafx.h"\
	".\mkr_d.h"\
	".\not.h"\
	".\ddx.h"\
	".\typ.h"\
	".\project.h"\
	".\crngst_d.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\font.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\dbref.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	

"$(INTDIR)\mkr_d.obj" : $(SOURCE) $(DEP_CPP_MKR_D) "$(INTDIR)"

"$(INTDIR)\mkr_d.sbr" : $(SOURCE) $(DEP_CPP_MKR_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkr.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkr.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MKR_C=\
	".\stdafx.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\tools.h"\
	".\font.h"\
	".\interlin.h"\
	".\project.h"\
	".\obstream.h"\
	".\typ.h"\
	".\jmp.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\lng.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\patmch.h"\
	".\trie.h"\
	".\dbref.h"\
	".\printinf.h"\
	".\number.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkr.obj" : $(SOURCE) $(DEP_CPP_MKR_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MKR_C=\
	".\stdafx.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\tools.h"\
	".\font.h"\
	".\interlin.h"\
	".\project.h"\
	".\obstream.h"\
	".\typ.h"\
	".\jmp.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\lng.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\patmch.h"\
	".\trie.h"\
	".\dbref.h"\
	".\printinf.h"\
	".\number.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkr.obj" : $(SOURCE) $(DEP_CPP_MKR_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MKR_C=\
	".\stdafx.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\tools.h"\
	".\font.h"\
	".\interlin.h"\
	".\project.h"\
	".\obstream.h"\
	".\typ.h"\
	".\jmp.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\lng.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\patmch.h"\
	".\trie.h"\
	".\dbref.h"\
	".\printinf.h"\
	".\number.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	

"$(INTDIR)\mkr.obj" : $(SOURCE) $(DEP_CPP_MKR_C) "$(INTDIR)"

"$(INTDIR)\mkr.sbr" : $(SOURCE) $(DEP_CPP_MKR_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdf_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdf_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MDF_D=\
	".\stdafx.h"\
	".\mdf_d.h"\
	".\mrfset_d.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\typ.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\mkr.h"\
	".\cbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\export.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mdf_d.obj" : $(SOURCE) $(DEP_CPP_MDF_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MDF_D=\
	".\stdafx.h"\
	".\mdf_d.h"\
	".\mrfset_d.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\typ.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\mkr.h"\
	".\cbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\export.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mdf_d.obj" : $(SOURCE) $(DEP_CPP_MDF_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MDF_D=\
	".\stdafx.h"\
	".\mdf_d.h"\
	".\mrfset_d.h"\
	".\mdf.h"\
	".\pgs.h"\
	".\typ.h"\
	".\shw.h"\
	".\shwdefs.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\mkr.h"\
	".\cbo.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\export.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mdf_d.obj" : $(SOURCE) $(DEP_CPP_MDF_D) "$(INTDIR)"

"$(INTDIR)\mdf_d.sbr" : $(SOURCE) $(DEP_CPP_MDF_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdf.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mdf.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MDF_C=\
	".\stdafx.h"\
	".\mdf.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\html.h"\
	".\progress.h"\
	".\date.h"\
	".\mdf_d.h"\
	".\shwdefs.h"\
	".\export.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	

"$(INTDIR)\mdf.obj" : $(SOURCE) $(DEP_CPP_MDF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MDF_C=\
	".\stdafx.h"\
	".\mdf.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\html.h"\
	".\progress.h"\
	".\date.h"\
	".\mdf_d.h"\
	".\shwdefs.h"\
	".\export.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	

"$(INTDIR)\mdf.obj" : $(SOURCE) $(DEP_CPP_MDF_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MDF_C=\
	".\stdafx.h"\
	".\mdf.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\cct.h"\
	".\sfstream.h"\
	".\rtf.h"\
	".\pgs.h"\
	".\html.h"\
	".\progress.h"\
	".\date.h"\
	".\mdf_d.h"\
	".\shwdefs.h"\
	".\export.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	

"$(INTDIR)\mdf.obj" : $(SOURCE) $(DEP_CPP_MDF_C) "$(INTDIR)"

"$(INTDIR)\mdf.sbr" : $(SOURCE) $(DEP_CPP_MDF_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mark.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MARK_=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\mark.obj" : $(SOURCE) $(DEP_CPP_MARK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MARK_=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\mark.obj" : $(SOURCE) $(DEP_CPP_MARK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MARK_=\
	".\stdafx.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\mark.obj" : $(SOURCE) $(DEP_CPP_MARK_) "$(INTDIR)"

"$(INTDIR)\mark.sbr" : $(SOURCE) $(DEP_CPP_MARK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mainfrm.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mainfrm.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_MAINF=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\ind.h"\
	".\fil.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_MAINF=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\ind.h"\
	".\fil.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_MAINF=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\mainfrm.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\kmn.h"\
	".\ind.h"\
	".\fil.h"\
	".\shwdefs.h"\
	".\kbd.h"\
	".\status.h"\
	".\filsel_d.h"\
	".\find_d.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\mkr.h"\
	".\EditLng.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\lbo.h"\
	".\not.h"\
	".\cct.h"\
	".\patch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\mainfrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"

"$(INTDIR)\mainfrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mac.rc

!IF  "$(CFG)" == "Toolbox - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lookup.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lookup.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LOOKU=\
	".\stdafx.h"\
	".\lookup.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\stdiofil.h"\
	".\prog_d.h"\
	

"$(INTDIR)\lookup.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LOOKU=\
	".\stdafx.h"\
	".\lookup.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\stdiofil.h"\
	".\prog_d.h"\
	

"$(INTDIR)\lookup.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LOOKU=\
	".\stdafx.h"\
	".\lookup.h"\
	".\progress.h"\
	".\shwdefs.h"\
	".\stdiofil.h"\
	".\prog_d.h"\
	

"$(INTDIR)\lookup.obj" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"

"$(INTDIR)\lookup.sbr" : $(SOURCE) $(DEP_CPP_LOOKU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\loc.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\loc.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LOC_C=\
	".\stdafx.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\loc.obj" : $(SOURCE) $(DEP_CPP_LOC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LOC_C=\
	".\stdafx.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\loc.obj" : $(SOURCE) $(DEP_CPP_LOC_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LOC_C=\
	".\stdafx.h"\
	".\loc.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\loc.obj" : $(SOURCE) $(DEP_CPP_LOC_C) "$(INTDIR)"

"$(INTDIR)\loc.sbr" : $(SOURCE) $(DEP_CPP_LOC_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lng_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lng_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LNG_D=\
	".\stdafx.h"\
	".\lng_d.h"\
	".\project.h"\
	".\shw.h"\
	".\kmn.h"\
	".\tools.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\kbd.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\font.h"\
	".\EditLng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\lng_d.obj" : $(SOURCE) $(DEP_CPP_LNG_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LNG_D=\
	".\stdafx.h"\
	".\lng_d.h"\
	".\project.h"\
	".\shw.h"\
	".\kmn.h"\
	".\tools.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\kbd.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\font.h"\
	".\EditLng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\lng_d.obj" : $(SOURCE) $(DEP_CPP_LNG_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LNG_D=\
	".\stdafx.h"\
	".\lng_d.h"\
	".\project.h"\
	".\shw.h"\
	".\kmn.h"\
	".\tools.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\kbd.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\font.h"\
	".\EditLng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\lng_d.obj" : $(SOURCE) $(DEP_CPP_LNG_D) "$(INTDIR)"

"$(INTDIR)\lng_d.sbr" : $(SOURCE) $(DEP_CPP_LNG_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lng.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lng.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LNG_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\lng.h"\
	".\font.h"\
	".\obstream.h"\
	".\cct.h"\
	".\tools.h"\
	".\usp10.h"\
	".\nlstream.h"\
	".\patch.h"\
	".\lng_d.h"\
	".\find_d.h"\
	".\dirlist.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\EditLng.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\lng.obj" : $(SOURCE) $(DEP_CPP_LNG_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LNG_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\lng.h"\
	".\font.h"\
	".\obstream.h"\
	".\cct.h"\
	".\tools.h"\
	".\usp10.h"\
	".\nlstream.h"\
	".\patch.h"\
	".\lng_d.h"\
	".\find_d.h"\
	".\dirlist.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\EditLng.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\lng.obj" : $(SOURCE) $(DEP_CPP_LNG_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LNG_C=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\lng.h"\
	".\font.h"\
	".\obstream.h"\
	".\cct.h"\
	".\tools.h"\
	".\usp10.h"\
	".\nlstream.h"\
	".\patch.h"\
	".\lng_d.h"\
	".\find_d.h"\
	".\dirlist.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\not.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\srtset_d.h"\
	".\EditLng.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	

"$(INTDIR)\lng.obj" : $(SOURCE) $(DEP_CPP_LNG_C) "$(INTDIR)"

"$(INTDIR)\lng.sbr" : $(SOURCE) $(DEP_CPP_LNG_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\listentr.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\listentr.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LISTE=\
	".\stdafx.h"\
	".\listentr.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\listentr.obj" : $(SOURCE) $(DEP_CPP_LISTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LISTE=\
	".\stdafx.h"\
	".\listentr.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\listentr.obj" : $(SOURCE) $(DEP_CPP_LISTE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LISTE=\
	".\stdafx.h"\
	".\listentr.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\listentr.obj" : $(SOURCE) $(DEP_CPP_LISTE) "$(INTDIR)"

"$(INTDIR)\listentr.sbr" : $(SOURCE) $(DEP_CPP_LISTE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lbo.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\lbo.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_LBO_C=\
	".\stdafx.h"\
	".\lbo.h"\
	".\lng.h"\
	".\mkr.h"\
	".\tools.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\lbo.obj" : $(SOURCE) $(DEP_CPP_LBO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_LBO_C=\
	".\stdafx.h"\
	".\lbo.h"\
	".\lng.h"\
	".\mkr.h"\
	".\tools.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\lbo.obj" : $(SOURCE) $(DEP_CPP_LBO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_LBO_C=\
	".\stdafx.h"\
	".\lbo.h"\
	".\lng.h"\
	".\mkr.h"\
	".\tools.h"\
	".\font.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\lbo.obj" : $(SOURCE) $(DEP_CPP_LBO_C) "$(INTDIR)"

"$(INTDIR)\lbo.sbr" : $(SOURCE) $(DEP_CPP_LBO_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\kmn.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\kmn.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_KMN_C=\
	".\stdafx.h"\
	".\kmn.h"\
	".\kbd.h"\
	".\mkr.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\cct.h"\
	
NODEP_CPP_KMN_C=\
	".\script.h"\
	".\fonts.h"\
	

"$(INTDIR)\kmn.obj" : $(SOURCE) $(DEP_CPP_KMN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_KMN_C=\
	".\stdafx.h"\
	".\kmn.h"\
	".\kbd.h"\
	".\mkr.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\cct.h"\
	
NODEP_CPP_KMN_C=\
	".\script.h"\
	".\fonts.h"\
	

"$(INTDIR)\kmn.obj" : $(SOURCE) $(DEP_CPP_KMN_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_KMN_C=\
	".\stdafx.h"\
	".\kmn.h"\
	".\kbd.h"\
	".\mkr.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\ptr.h"\
	".\cct.h"\
	

"$(INTDIR)\kmn.obj" : $(SOURCE) $(DEP_CPP_KMN_C) "$(INTDIR)"

"$(INTDIR)\kmn.sbr" : $(SOURCE) $(DEP_CPP_KMN_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\km95api.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\key_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\key_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_KEY_D=\
	".\stdafx.h"\
	".\key_d.h"\
	".\recset.h"\
	".\mkr.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\srt.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\key_d.obj" : $(SOURCE) $(DEP_CPP_KEY_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_KEY_D=\
	".\stdafx.h"\
	".\key_d.h"\
	".\recset.h"\
	".\mkr.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\srt.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\key_d.obj" : $(SOURCE) $(DEP_CPP_KEY_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_KEY_D=\
	".\stdafx.h"\
	".\key_d.h"\
	".\recset.h"\
	".\mkr.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\srt.h"\
	".\EditLng.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\key_d.obj" : $(SOURCE) $(DEP_CPP_KEY_D) "$(INTDIR)"

"$(INTDIR)\key_d.sbr" : $(SOURCE) $(DEP_CPP_KEY_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\kbd.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\kbd.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_KBD_C=\
	".\stdafx.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\cct.h"\
	

"$(INTDIR)\kbd.obj" : $(SOURCE) $(DEP_CPP_KBD_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_KBD_C=\
	".\stdafx.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\cct.h"\
	

"$(INTDIR)\kbd.obj" : $(SOURCE) $(DEP_CPP_KBD_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_KBD_C=\
	".\stdafx.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\km95api.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\srt.h"\
	".\var.h"\
	".\cct.h"\
	

"$(INTDIR)\kbd.obj" : $(SOURCE) $(DEP_CPP_KBD_C) "$(INTDIR)"

"$(INTDIR)\kbd.sbr" : $(SOURCE) $(DEP_CPP_KBD_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\karen.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_KAREN=\
	".\stdafx.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\karen.obj" : $(SOURCE) $(DEP_CPP_KAREN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_KAREN=\
	".\stdafx.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\karen.obj" : $(SOURCE) $(DEP_CPP_KAREN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_KAREN=\
	".\stdafx.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\karen.obj" : $(SOURCE) $(DEP_CPP_KAREN) "$(INTDIR)"

"$(INTDIR)\karen.sbr" : $(SOURCE) $(DEP_CPP_KAREN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jumpdlg.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jumpdlg.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_JUMPD=\
	".\stdafx.h"\
	".\recset.h"\
	".\jumpdlg.h"\
	".\jmp.h"\
	".\ddx.h"\
	".\typ.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\jumpdlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_JUMPD=\
	".\stdafx.h"\
	".\recset.h"\
	".\jumpdlg.h"\
	".\jmp.h"\
	".\ddx.h"\
	".\typ.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\jumpdlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_JUMPD=\
	".\stdafx.h"\
	".\recset.h"\
	".\jumpdlg.h"\
	".\jmp.h"\
	".\ddx.h"\
	".\typ.h"\
	".\typ_d.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\EditLng.h"\
	".\dbref.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\jumpdlg.obj" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"

"$(INTDIR)\jumpdlg.sbr" : $(SOURCE) $(DEP_CPP_JUMPD) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmp.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\jmp.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_JMP_C=\
	".\stdafx.h"\
	".\jmp.h"\
	".\spath_d.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\jumpdlg.h"\
	".\multmat.h"\
	".\seldb_d.h"\
	".\nomat_d.h"\
	".\obstream.h"\
	".\crecpos.h"\
	".\shwview.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\dbref.h"\
	".\srt.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\cbo.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\patmch.h"\
	".\cct.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngst_d.h"\
	

"$(INTDIR)\jmp.obj" : $(SOURCE) $(DEP_CPP_JMP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_JMP_C=\
	".\stdafx.h"\
	".\jmp.h"\
	".\spath_d.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\jumpdlg.h"\
	".\multmat.h"\
	".\seldb_d.h"\
	".\nomat_d.h"\
	".\obstream.h"\
	".\crecpos.h"\
	".\shwview.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\dbref.h"\
	".\srt.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\cbo.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\patmch.h"\
	".\cct.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngst_d.h"\
	

"$(INTDIR)\jmp.obj" : $(SOURCE) $(DEP_CPP_JMP_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_JMP_C=\
	".\stdafx.h"\
	".\jmp.h"\
	".\spath_d.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\doclist.h"\
	".\shwdefs.h"\
	".\jumpdlg.h"\
	".\multmat.h"\
	".\seldb_d.h"\
	".\nomat_d.h"\
	".\obstream.h"\
	".\crecpos.h"\
	".\shwview.h"\
	".\mkr_d.h"\
	".\set_d.h"\
	".\set.h"\
	".\mkr.h"\
	".\dbref.h"\
	".\srt.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\cbo.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\patmch.h"\
	".\cct.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\trie.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\font.h"\
	".\crngst_d.h"\
	

"$(INTDIR)\jmp.obj" : $(SOURCE) $(DEP_CPP_JMP_C) "$(INTDIR)"

"$(INTDIR)\jmp.sbr" : $(SOURCE) $(DEP_CPP_JMP_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intsetup.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intsetup.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_INTSE=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	

"$(INTDIR)\intsetup.obj" : $(SOURCE) $(DEP_CPP_INTSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_INTSE=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	

"$(INTDIR)\intsetup.obj" : $(SOURCE) $(DEP_CPP_INTSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_INTSE=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	

"$(INTDIR)\intsetup.obj" : $(SOURCE) $(DEP_CPP_INTSE) "$(INTDIR)"

"$(INTDIR)\intsetup.sbr" : $(SOURCE) $(DEP_CPP_INTSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intprc_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\intprc_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_INTPR=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\intprc_d.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\intprc_d.obj" : $(SOURCE) $(DEP_CPP_INTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_INTPR=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\intprc_d.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\intprc_d.obj" : $(SOURCE) $(DEP_CPP_INTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_INTPR=\
	".\stdafx.h"\
	".\intsetup.h"\
	".\intprc_d.h"\
	".\typ.h"\
	".\shwdoc.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\intprc_d.obj" : $(SOURCE) $(DEP_CPP_INTPR) "$(INTDIR)"

"$(INTDIR)\intprc_d.sbr" : $(SOURCE) $(DEP_CPP_INTPR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\interlin.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\interlin.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_INTER=\
	".\stdafx.h"\
	".\interlin.h"\
	".\recset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\doclist.h"\
	".\intprc_d.h"\
	".\ambig_d.h"\
	".\typ_d.h"\
	".\obstream.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\number_d.h"\
	

"$(INTDIR)\interlin.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_INTER=\
	".\stdafx.h"\
	".\interlin.h"\
	".\recset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\doclist.h"\
	".\intprc_d.h"\
	".\ambig_d.h"\
	".\typ_d.h"\
	".\obstream.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\number_d.h"\
	
NODEP_CPP_INTER=\
	".\mksAnal"\
	

"$(INTDIR)\interlin.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_INTER=\
	".\stdafx.h"\
	".\interlin.h"\
	".\recset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\doclist.h"\
	".\intprc_d.h"\
	".\ambig_d.h"\
	".\typ_d.h"\
	".\obstream.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\edlngctl.h"\
	".\spath_d.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	".\mkrset_d.h"\
	".\number_d.h"\
	

"$(INTDIR)\interlin.obj" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"

"$(INTDIR)\interlin.sbr" : $(SOURCE) $(DEP_CPP_INTER) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ind_t.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_IND_T=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\ind_t.obj" : $(SOURCE) $(DEP_CPP_IND_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_IND_T=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\ind_t.obj" : $(SOURCE) $(DEP_CPP_IND_T) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_IND_T=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\ind_t.obj" : $(SOURCE) $(DEP_CPP_IND_T) "$(INTDIR)"

"$(INTDIR)\ind_t.sbr" : $(SOURCE) $(DEP_CPP_IND_T) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ind.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ind.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_IND_C=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\recset.h"\
	".\srt.h"\
	".\project.h"\
	".\dlgsort.h"\
	".\dlgrcins.h"\
	".\filsel_d.h"\
	".\multmat.h"\
	".\nomat_d.h"\
	".\key_d.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ind.obj" : $(SOURCE) $(DEP_CPP_IND_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_IND_C=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\recset.h"\
	".\srt.h"\
	".\project.h"\
	".\dlgsort.h"\
	".\dlgrcins.h"\
	".\filsel_d.h"\
	".\multmat.h"\
	".\nomat_d.h"\
	".\key_d.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ind.obj" : $(SOURCE) $(DEP_CPP_IND_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_IND_C=\
	".\stdafx.h"\
	".\ind.h"\
	".\typ.h"\
	".\obstream.h"\
	".\sfstream.h"\
	".\progress.h"\
	".\recset.h"\
	".\srt.h"\
	".\project.h"\
	".\dlgsort.h"\
	".\dlgrcins.h"\
	".\filsel_d.h"\
	".\multmat.h"\
	".\nomat_d.h"\
	".\key_d.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ind.obj" : $(SOURCE) $(DEP_CPP_IND_C) "$(INTDIR)"

"$(INTDIR)\ind.sbr" : $(SOURCE) $(DEP_CPP_IND_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\hugestr.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\hugestr.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_HUGES=\
	".\stdafx.h"\
	".\hugestr.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\hugestr.obj" : $(SOURCE) $(DEP_CPP_HUGES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_HUGES=\
	".\stdafx.h"\
	".\hugestr.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\hugestr.obj" : $(SOURCE) $(DEP_CPP_HUGES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_HUGES=\
	".\stdafx.h"\
	".\hugestr.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\hugestr.obj" : $(SOURCE) $(DEP_CPP_HUGES) "$(INTDIR)"

"$(INTDIR)\hugestr.sbr" : $(SOURCE) $(DEP_CPP_HUGES) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\html.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\html.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_HTML_=\
	".\stdafx.h"\
	".\html.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\html.obj" : $(SOURCE) $(DEP_CPP_HTML_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_HTML_=\
	".\stdafx.h"\
	".\html.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\html.obj" : $(SOURCE) $(DEP_CPP_HTML_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_HTML_=\
	".\stdafx.h"\
	".\html.h"\
	".\cfield.h"\
	".\ddx.h"\
	".\shwdefs.h"\
	".\sfstream.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\html.obj" : $(SOURCE) $(DEP_CPP_HTML_) "$(INTDIR)"

"$(INTDIR)\html.sbr" : $(SOURCE) $(DEP_CPP_HTML_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\hints.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\hint.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\help_ids.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\font.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\font.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_FONT_=\
	".\stdafx.h"\
	".\font.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\font.obj" : $(SOURCE) $(DEP_CPP_FONT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_FONT_=\
	".\stdafx.h"\
	".\font.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\font.obj" : $(SOURCE) $(DEP_CPP_FONT_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_FONT_=\
	".\stdafx.h"\
	".\font.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\font.obj" : $(SOURCE) $(DEP_CPP_FONT_) "$(INTDIR)"

"$(INTDIR)\font.sbr" : $(SOURCE) $(DEP_CPP_FONT_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\find_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\find_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_FIND_=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\find_d.h"\
	".\mkr.h"\
	".\font.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\cfield.h"\
	".\tools.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	

"$(INTDIR)\find_d.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_FIND_=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\find_d.h"\
	".\mkr.h"\
	".\font.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\cfield.h"\
	".\tools.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	

"$(INTDIR)\find_d.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_FIND_=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\find_d.h"\
	".\mkr.h"\
	".\font.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\cfield.h"\
	".\tools.h"\
	".\set.h"\
	".\ref.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\EditLng.h"\
	

"$(INTDIR)\find_d.obj" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"

"$(INTDIR)\find_d.sbr" : $(SOURCE) $(DEP_CPP_FIND_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\filsel_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\filsel_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_FILSE=\
	".\stdafx.h"\
	".\fil.h"\
	".\filsel_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\filsel_d.obj" : $(SOURCE) $(DEP_CPP_FILSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_FILSE=\
	".\stdafx.h"\
	".\fil.h"\
	".\filsel_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\filsel_d.obj" : $(SOURCE) $(DEP_CPP_FILSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_FILSE=\
	".\stdafx.h"\
	".\fil.h"\
	".\filsel_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	

"$(INTDIR)\filsel_d.obj" : $(SOURCE) $(DEP_CPP_FILSE) "$(INTDIR)"

"$(INTDIR)\filsel_d.sbr" : $(SOURCE) $(DEP_CPP_FILSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fil_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fil_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_FIL_D=\
	".\stdafx.h"\
	".\fil_d.h"\
	".\date.h"\
	".\not.h"\
	".\typ.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\fil.h"\
	".\EditLng.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	

"$(INTDIR)\fil_d.obj" : $(SOURCE) $(DEP_CPP_FIL_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_FIL_D=\
	".\stdafx.h"\
	".\fil_d.h"\
	".\date.h"\
	".\not.h"\
	".\typ.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\fil.h"\
	".\EditLng.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	

"$(INTDIR)\fil_d.obj" : $(SOURCE) $(DEP_CPP_FIL_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_FIL_D=\
	".\stdafx.h"\
	".\fil_d.h"\
	".\date.h"\
	".\not.h"\
	".\typ.h"\
	".\project.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\fil.h"\
	".\EditLng.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	

"$(INTDIR)\fil_d.obj" : $(SOURCE) $(DEP_CPP_FIL_D) "$(INTDIR)"

"$(INTDIR)\fil_d.sbr" : $(SOURCE) $(DEP_CPP_FIL_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fil.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\fil.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_FIL_C=\
	".\stdafx.h"\
	".\fil.h"\
	".\loc.h"\
	".\ind.h"\
	".\date.h"\
	".\not.h"\
	".\obstream.h"\
	".\fil_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\typ_d.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\fil.obj" : $(SOURCE) $(DEP_CPP_FIL_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_FIL_C=\
	".\stdafx.h"\
	".\fil.h"\
	".\loc.h"\
	".\ind.h"\
	".\date.h"\
	".\not.h"\
	".\obstream.h"\
	".\fil_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\typ_d.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\fil.obj" : $(SOURCE) $(DEP_CPP_FIL_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_FIL_C=\
	".\stdafx.h"\
	".\fil.h"\
	".\loc.h"\
	".\ind.h"\
	".\date.h"\
	".\not.h"\
	".\obstream.h"\
	".\fil_d.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\typ_d.h"\
	".\project.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\patmch.h"\
	".\patch.h"\
	".\mkr.h"\
	".\lbo.h"\
	".\update.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\fil.obj" : $(SOURCE) $(DEP_CPP_FIL_C) "$(INTDIR)"

"$(INTDIR)\fil.sbr" : $(SOURCE) $(DEP_CPP_FIL_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\export_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\export_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_EXPOR=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mrfset_d.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\ind.h"\
	".\set_d.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\export_d.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_EXPOR=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mrfset_d.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\ind.h"\
	".\set_d.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\export_d.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_EXPOR=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mrfset_d.h"\
	".\pgs.h"\
	".\pgs_d.h"\
	".\ind.h"\
	".\set_d.h"\
	".\typ.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\export_d.obj" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"

"$(INTDIR)\export_d.sbr" : $(SOURCE) $(DEP_CPP_EXPOR) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\export.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\export.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_EXPORT=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mdf_d.h"\
	".\obstream.h"\
	".\pgs.h"\
	".\typ.h"\
	".\rtf.h"\
	".\xml.h"\
	".\mdf.h"\
	".\tools.h"\
	".\progress.h"\
	".\shw.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\sfstream.h"\
	".\prog_d.h"\
	".\crngset.h"\
	

"$(INTDIR)\export.obj" : $(SOURCE) $(DEP_CPP_EXPORT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_EXPORT=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mdf_d.h"\
	".\obstream.h"\
	".\pgs.h"\
	".\typ.h"\
	".\rtf.h"\
	".\xml.h"\
	".\mdf.h"\
	".\tools.h"\
	".\progress.h"\
	".\shw.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\sfstream.h"\
	".\prog_d.h"\
	".\crngset.h"\
	

"$(INTDIR)\export.obj" : $(SOURCE) $(DEP_CPP_EXPORT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_EXPORT=\
	".\stdafx.h"\
	".\export.h"\
	".\export_d.h"\
	".\mdf_d.h"\
	".\obstream.h"\
	".\pgs.h"\
	".\typ.h"\
	".\rtf.h"\
	".\xml.h"\
	".\mdf.h"\
	".\tools.h"\
	".\progress.h"\
	".\shw.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\set_d.h"\
	".\edlngctl.h"\
	".\filsel_d.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\sfstream.h"\
	".\prog_d.h"\
	".\crngset.h"\
	

"$(INTDIR)\export.obj" : $(SOURCE) $(DEP_CPP_EXPORT) "$(INTDIR)"

"$(INTDIR)\export.sbr" : $(SOURCE) $(DEP_CPP_EXPORT) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\edlngctl.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\edlngctl.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_EDLNG=\
	".\stdafx.h"\
	".\edlngctl.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\edlngctl.obj" : $(SOURCE) $(DEP_CPP_EDLNG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_EDLNG=\
	".\stdafx.h"\
	".\edlngctl.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\edlngctl.obj" : $(SOURCE) $(DEP_CPP_EDLNG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_EDLNG=\
	".\stdafx.h"\
	".\edlngctl.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\edlngctl.obj" : $(SOURCE) $(DEP_CPP_EDLNG) "$(INTDIR)"

"$(INTDIR)\edlngctl.sbr" : $(SOURCE) $(DEP_CPP_EDLNG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditLng.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditLng.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_EDITL=\
	".\stdafx.h"\
	".\shw.h"\
	".\EditLng.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\EditLng.obj" : $(SOURCE) $(DEP_CPP_EDITL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_EDITL=\
	".\stdafx.h"\
	".\shw.h"\
	".\EditLng.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	
NODEP_CPP_EDITL=\
	".\CEdit"\
	

"$(INTDIR)\EditLng.obj" : $(SOURCE) $(DEP_CPP_EDITL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_EDITL=\
	".\stdafx.h"\
	".\shw.h"\
	".\EditLng.h"\
	".\lng.h"\
	".\mkr.h"\
	".\project.h"\
	".\kmn.h"\
	".\SHConv.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\not.h"\
	".\km95api.h"\
	".\patch.h"\
	

"$(INTDIR)\EditLng.obj" : $(SOURCE) $(DEP_CPP_EDITL) "$(INTDIR)"

"$(INTDIR)\EditLng.sbr" : $(SOURCE) $(DEP_CPP_EDITL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\doclist.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\doclist.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DOCLI=\
	".\stdafx.h"\
	".\doclist.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\doclist.obj" : $(SOURCE) $(DEP_CPP_DOCLI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DOCLI=\
	".\stdafx.h"\
	".\doclist.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\doclist.obj" : $(SOURCE) $(DEP_CPP_DOCLI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DOCLI=\
	".\stdafx.h"\
	".\doclist.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\doclist.obj" : $(SOURCE) $(DEP_CPP_DOCLI) "$(INTDIR)"

"$(INTDIR)\doclist.sbr" : $(SOURCE) $(DEP_CPP_DOCLI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgsort.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgsort.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DLGSO=\
	".\stdafx.h"\
	".\dlgsort.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\dlgsort.obj" : $(SOURCE) $(DEP_CPP_DLGSO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DLGSO=\
	".\stdafx.h"\
	".\dlgsort.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\dlgsort.obj" : $(SOURCE) $(DEP_CPP_DLGSO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DLGSO=\
	".\stdafx.h"\
	".\dlgsort.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	

"$(INTDIR)\dlgsort.obj" : $(SOURCE) $(DEP_CPP_DLGSO) "$(INTDIR)"

"$(INTDIR)\dlgsort.sbr" : $(SOURCE) $(DEP_CPP_DLGSO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgrcins.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgrcins.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DLGRC=\
	".\stdafx.h"\
	".\mkr.h"\
	".\dlgrcins.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\dlgrcins.obj" : $(SOURCE) $(DEP_CPP_DLGRC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DLGRC=\
	".\stdafx.h"\
	".\mkr.h"\
	".\dlgrcins.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\dlgrcins.obj" : $(SOURCE) $(DEP_CPP_DLGRC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DLGRC=\
	".\stdafx.h"\
	".\mkr.h"\
	".\dlgrcins.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\dlgrcins.obj" : $(SOURCE) $(DEP_CPP_DLGRC) "$(INTDIR)"

"$(INTDIR)\dlgrcins.sbr" : $(SOURCE) $(DEP_CPP_DLGRC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgimpt.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dlgimpt.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DLGIM=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\dlgimpt.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\dlgimpt.obj" : $(SOURCE) $(DEP_CPP_DLGIM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DLGIM=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\dlgimpt.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\dlgimpt.obj" : $(SOURCE) $(DEP_CPP_DLGIM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DLGIM=\
	".\stdafx.h"\
	".\project.h"\
	".\shwdoc.h"\
	".\dlgimpt.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\typ_d.h"\
	".\set_d.h"\
	".\mkrset_d.h"\
	".\spath_d.h"\
	".\intprc_d.h"\
	".\number_d.h"\
	".\mrfset_d.h"\
	".\cck_d.h"\
	".\recset.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\edlngctl.h"\
	

"$(INTDIR)\dlgimpt.obj" : $(SOURCE) $(DEP_CPP_DLGIM) "$(INTDIR)"

"$(INTDIR)\dlgimpt.sbr" : $(SOURCE) $(DEP_CPP_DLGIM) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dirlist.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dirlist.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DIRLI=\
	".\stdafx.h"\
	".\dirlist.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\dirlist.obj" : $(SOURCE) $(DEP_CPP_DIRLI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DIRLI=\
	".\stdafx.h"\
	".\dirlist.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\dirlist.obj" : $(SOURCE) $(DEP_CPP_DIRLI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DIRLI=\
	".\stdafx.h"\
	".\dirlist.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\dirlist.obj" : $(SOURCE) $(DEP_CPP_DIRLI) "$(INTDIR)"

"$(INTDIR)\dirlist.sbr" : $(SOURCE) $(DEP_CPP_DIRLI) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddx.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ddx.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DDX_C=\
	".\stdafx.h"\
	".\ddx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\ddx.obj" : $(SOURCE) $(DEP_CPP_DDX_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DDX_C=\
	".\stdafx.h"\
	".\ddx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\ddx.obj" : $(SOURCE) $(DEP_CPP_DDX_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DDX_C=\
	".\stdafx.h"\
	".\ddx.h"\
	".\patch.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	

"$(INTDIR)\ddx.obj" : $(SOURCE) $(DEP_CPP_DDX_C) "$(INTDIR)"

"$(INTDIR)\ddx.sbr" : $(SOURCE) $(DEP_CPP_DDX_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dbref.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dbref.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DBREF=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\spath_d.h"\
	".\multmat.h"\
	".\project.h"\
	".\obstream.h"\
	".\doclist.h"\
	".\dbref.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	

"$(INTDIR)\dbref.obj" : $(SOURCE) $(DEP_CPP_DBREF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DBREF=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\spath_d.h"\
	".\multmat.h"\
	".\project.h"\
	".\obstream.h"\
	".\doclist.h"\
	".\dbref.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	

"$(INTDIR)\dbref.obj" : $(SOURCE) $(DEP_CPP_DBREF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DBREF=\
	".\stdafx.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\recset.h"\
	".\spath_d.h"\
	".\multmat.h"\
	".\project.h"\
	".\obstream.h"\
	".\doclist.h"\
	".\dbref.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\crngset.h"\
	

"$(INTDIR)\dbref.obj" : $(SOURCE) $(DEP_CPP_DBREF) "$(INTDIR)"

"$(INTDIR)\dbref.sbr" : $(SOURCE) $(DEP_CPP_DBREF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\date.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\date.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_DATE_=\
	".\stdafx.h"\
	".\date.h"\
	".\obstream.h"\
	".\ddx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_DATE_=\
	".\stdafx.h"\
	".\date.h"\
	".\obstream.h"\
	".\ddx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_DATE_=\
	".\stdafx.h"\
	".\date.h"\
	".\obstream.h"\
	".\ddx.h"\
	".\not.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	

"$(INTDIR)\date.obj" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"

"$(INTDIR)\date.sbr" : $(SOURCE) $(DEP_CPP_DATE_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cslookel.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cslookel.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CSLOO=\
	".\stdafx.h"\
	".\cslookel.h"\
	".\shwdefs.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslookel.obj" : $(SOURCE) $(DEP_CPP_CSLOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CSLOO=\
	".\stdafx.h"\
	".\cslookel.h"\
	".\shwdefs.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslookel.obj" : $(SOURCE) $(DEP_CPP_CSLOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CSLOO=\
	".\stdafx.h"\
	".\cslookel.h"\
	".\shwdefs.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslookel.obj" : $(SOURCE) $(DEP_CPP_CSLOO) "$(INTDIR)"

"$(INTDIR)\cslookel.sbr" : $(SOURCE) $(DEP_CPP_CSLOO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cslook.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cslook.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CSLOOK=\
	".\stdafx.h"\
	".\cslook.h"\
	".\shwdefs.h"\
	".\clook.h"\
	".\cslookel.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslook.obj" : $(SOURCE) $(DEP_CPP_CSLOOK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CSLOOK=\
	".\stdafx.h"\
	".\cslook.h"\
	".\shwdefs.h"\
	".\clook.h"\
	".\cslookel.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslook.obj" : $(SOURCE) $(DEP_CPP_CSLOOK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CSLOOK=\
	".\stdafx.h"\
	".\cslook.h"\
	".\shwdefs.h"\
	".\clook.h"\
	".\cslookel.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cslook.obj" : $(SOURCE) $(DEP_CPP_CSLOOK) "$(INTDIR)"

"$(INTDIR)\cslook.sbr" : $(SOURCE) $(DEP_CPP_CSLOOK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crngst_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crngst_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CRNGS=\
	".\stdafx.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\mkr.h"\
	".\mkr_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\font.h"\
	

"$(INTDIR)\crngst_d.obj" : $(SOURCE) $(DEP_CPP_CRNGS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CRNGS=\
	".\stdafx.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\mkr.h"\
	".\mkr_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\font.h"\
	

"$(INTDIR)\crngst_d.obj" : $(SOURCE) $(DEP_CPP_CRNGS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CRNGS=\
	".\stdafx.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\mkr.h"\
	".\mkr_d.h"\
	".\project.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\ptr.h"\
	".\srt.h"\
	".\cct.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\patmch.h"\
	".\patch.h"\
	".\lng.h"\
	".\var.h"\
	".\kbd.h"\
	".\not.h"\
	".\km95api.h"\
	".\cck_d.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\font.h"\
	

"$(INTDIR)\crngst_d.obj" : $(SOURCE) $(DEP_CPP_CRNGS) "$(INTDIR)"

"$(INTDIR)\crngst_d.sbr" : $(SOURCE) $(DEP_CPP_CRNGS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crngset.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crngset.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CRNGSE=\
	".\stdafx.h"\
	".\patch.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\shwdoc.h"\
	".\cck.h"\
	".\lng.h"\
	".\shwview.h"\
	".\progress.h"\
	".\doclist.h"\
	".\ind.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ref.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\tools.h"\
	".\cck_d.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\srt.h"\
	".\obstream.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\crlookel.h"\
	

"$(INTDIR)\crngset.obj" : $(SOURCE) $(DEP_CPP_CRNGSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CRNGSE=\
	".\stdafx.h"\
	".\patch.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\shwdoc.h"\
	".\cck.h"\
	".\lng.h"\
	".\shwview.h"\
	".\progress.h"\
	".\doclist.h"\
	".\ind.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ref.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\tools.h"\
	".\cck_d.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\srt.h"\
	".\obstream.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\crlookel.h"\
	

"$(INTDIR)\crngset.obj" : $(SOURCE) $(DEP_CPP_CRNGSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CRNGSE=\
	".\stdafx.h"\
	".\patch.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\shwdoc.h"\
	".\cck.h"\
	".\lng.h"\
	".\shwview.h"\
	".\progress.h"\
	".\doclist.h"\
	".\ind.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\not.h"\
	".\cdblel.h"\
	".\set.h"\
	".\crecpos.h"\
	".\ref.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\tools.h"\
	".\cck_d.h"\
	".\EditLng.h"\
	".\typ.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\cct.h"\
	".\dbref.h"\
	".\ptr.h"\
	".\patmch.h"\
	".\srt.h"\
	".\obstream.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\crlookel.h"\
	

"$(INTDIR)\crngset.obj" : $(SOURCE) $(DEP_CPP_CRNGSE) "$(INTDIR)"

"$(INTDIR)\crngset.sbr" : $(SOURCE) $(DEP_CPP_CRNGSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crlookel.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crlookel.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CRLOO=\
	".\stdafx.h"\
	".\crlookel.h"\
	".\sfstream.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	

"$(INTDIR)\crlookel.obj" : $(SOURCE) $(DEP_CPP_CRLOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CRLOO=\
	".\stdafx.h"\
	".\crlookel.h"\
	".\sfstream.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	

"$(INTDIR)\crlookel.obj" : $(SOURCE) $(DEP_CPP_CRLOO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CRLOO=\
	".\stdafx.h"\
	".\crlookel.h"\
	".\sfstream.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	

"$(INTDIR)\crlookel.obj" : $(SOURCE) $(DEP_CPP_CRLOO) "$(INTDIR)"

"$(INTDIR)\crlookel.sbr" : $(SOURCE) $(DEP_CPP_CRLOO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crecpos.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crecpos.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CRECP=\
	".\stdafx.h"\
	".\crecpos.h"\
	".\shwdefs.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\crecpos.obj" : $(SOURCE) $(DEP_CPP_CRECP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CRECP=\
	".\stdafx.h"\
	".\crecpos.h"\
	".\shwdefs.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\crecpos.obj" : $(SOURCE) $(DEP_CPP_CRECP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CRECP=\
	".\stdafx.h"\
	".\crecpos.h"\
	".\shwdefs.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	

"$(INTDIR)\crecpos.obj" : $(SOURCE) $(DEP_CPP_CRECP) "$(INTDIR)"

"$(INTDIR)\crecpos.sbr" : $(SOURCE) $(DEP_CPP_CRECP) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crecord.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crecord.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CRECO=\
	".\stdafx.h"\
	".\crecord.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\date.h"\
	".\cck_d.h"\
	".\crngst_d.h"\
	".\spath_d.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\recset.h"\
	".\dbref.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	

"$(INTDIR)\crecord.obj" : $(SOURCE) $(DEP_CPP_CRECO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CRECO=\
	".\stdafx.h"\
	".\crecord.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\date.h"\
	".\cck_d.h"\
	".\crngst_d.h"\
	".\spath_d.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\recset.h"\
	".\dbref.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	

"$(INTDIR)\crecord.obj" : $(SOURCE) $(DEP_CPP_CRECO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CRECO=\
	".\stdafx.h"\
	".\crecord.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\date.h"\
	".\cck_d.h"\
	".\crngst_d.h"\
	".\spath_d.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\cck.h"\
	".\EditLng.h"\
	".\obstream.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\mrfset_d.h"\
	".\set_d.h"\
	".\recset.h"\
	".\dbref.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\interlin.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	

"$(INTDIR)\crecord.obj" : $(SOURCE) $(DEP_CPP_CRECO) "$(INTDIR)"

"$(INTDIR)\crecord.sbr" : $(SOURCE) $(DEP_CPP_CRECO) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\corpus_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\corpus_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CORPU=\
	".\stdafx.h"\
	".\corpus_d.h"\
	".\corpus.h"\
	".\not.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\dirlist.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\corpus_d.obj" : $(SOURCE) $(DEP_CPP_CORPU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CORPU=\
	".\stdafx.h"\
	".\corpus_d.h"\
	".\corpus.h"\
	".\not.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\dirlist.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\corpus_d.obj" : $(SOURCE) $(DEP_CPP_CORPU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CORPU=\
	".\stdafx.h"\
	".\corpus_d.h"\
	".\corpus.h"\
	".\not.h"\
	".\shw.h"\
	".\project.h"\
	".\tools.h"\
	".\dirlist.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\lng.h"\
	".\set.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\lbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\cct.h"\
	".\km95api.h"\
	".\obstream.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\charset.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\corpus_d.obj" : $(SOURCE) $(DEP_CPP_CORPU) "$(INTDIR)"

"$(INTDIR)\corpus_d.sbr" : $(SOURCE) $(DEP_CPP_CORPU) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\corpus.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\corpus.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CORPUS=\
	".\stdafx.h"\
	".\corpus.h"\
	".\tools.h"\
	".\corpus_d.h"\
	".\lbo.h"\
	".\project.h"\
	".\not.h"\
	".\charset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\obstream.h"\
	".\lng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\corpus.obj" : $(SOURCE) $(DEP_CPP_CORPUS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CORPUS=\
	".\stdafx.h"\
	".\corpus.h"\
	".\tools.h"\
	".\corpus_d.h"\
	".\lbo.h"\
	".\project.h"\
	".\not.h"\
	".\charset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\obstream.h"\
	".\lng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\corpus.obj" : $(SOURCE) $(DEP_CPP_CORPUS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CORPUS=\
	".\stdafx.h"\
	".\corpus.h"\
	".\tools.h"\
	".\corpus_d.h"\
	".\lbo.h"\
	".\project.h"\
	".\not.h"\
	".\charset.h"\
	".\shw.h"\
	".\shwdoc.h"\
	".\set_d.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\obstream.h"\
	".\lng.h"\
	".\stringex.h"\
	".\wordlist.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\update.h"\
	".\cbo.h"\
	".\cct.h"\
	".\km95api.h"\
	".\lookup.h"\
	".\booklist.h"\
	".\listentr.h"\
	".\progress.h"\
	".\stdiofil.h"\
	".\hugestr.h"\
	".\prog_d.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\patch.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	

"$(INTDIR)\corpus.obj" : $(SOURCE) $(DEP_CPP_CORPUS) "$(INTDIR)"

"$(INTDIR)\corpus.sbr" : $(SOURCE) $(DEP_CPP_CORPUS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clookel.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clookel.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CLOOK=\
	".\stdafx.h"\
	".\clookel.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clookel.obj" : $(SOURCE) $(DEP_CPP_CLOOK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CLOOK=\
	".\stdafx.h"\
	".\clookel.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clookel.obj" : $(SOURCE) $(DEP_CPP_CLOOK) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CLOOK=\
	".\stdafx.h"\
	".\clookel.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clookel.obj" : $(SOURCE) $(DEP_CPP_CLOOK) "$(INTDIR)"

"$(INTDIR)\clookel.sbr" : $(SOURCE) $(DEP_CPP_CLOOK) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clook.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\clook.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CLOOK_=\
	".\stdafx.h"\
	".\clook.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clook.obj" : $(SOURCE) $(DEP_CPP_CLOOK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CLOOK_=\
	".\stdafx.h"\
	".\clook.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clook.obj" : $(SOURCE) $(DEP_CPP_CLOOK_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CLOOK_=\
	".\stdafx.h"\
	".\clook.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\clookel.h"\
	".\cdblel.h"\
	

"$(INTDIR)\clook.obj" : $(SOURCE) $(DEP_CPP_CLOOK_) "$(INTDIR)"

"$(INTDIR)\clook.sbr" : $(SOURCE) $(DEP_CPP_CLOOK_) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\charset.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\charset.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CHARS=\
	".\stdafx.h"\
	".\charset.h"\
	".\stdiofil.h"\
	".\shwdefs.h"\
	".\stringex.h"\
	

"$(INTDIR)\charset.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CHARS=\
	".\stdafx.h"\
	".\charset.h"\
	".\stdiofil.h"\
	".\shwdefs.h"\
	".\stringex.h"\
	

"$(INTDIR)\charset.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CHARS=\
	".\stdafx.h"\
	".\charset.h"\
	".\stdiofil.h"\
	".\shwdefs.h"\
	".\stringex.h"\
	

"$(INTDIR)\charset.obj" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"

"$(INTDIR)\charset.sbr" : $(SOURCE) $(DEP_CPP_CHARS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cfield.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cfield.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CFIEL=\
	".\stdafx.h"\
	".\cfield.h"\
	".\crecpos.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecord.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\obstream.h"\
	

"$(INTDIR)\cfield.obj" : $(SOURCE) $(DEP_CPP_CFIEL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CFIEL=\
	".\stdafx.h"\
	".\cfield.h"\
	".\crecpos.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecord.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\obstream.h"\
	

"$(INTDIR)\cfield.obj" : $(SOURCE) $(DEP_CPP_CFIEL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CFIEL=\
	".\stdafx.h"\
	".\cfield.h"\
	".\crecpos.h"\
	".\crngset.h"\
	".\jmp.h"\
	".\cck.h"\
	".\shwdefs.h"\
	".\mkr.h"\
	".\cdbllist.h"\
	".\tools.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecord.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\dbref.h"\
	".\obstream.h"\
	

"$(INTDIR)\cfield.obj" : $(SOURCE) $(DEP_CPP_CFIEL) "$(INTDIR)"

"$(INTDIR)\cfield.sbr" : $(SOURCE) $(DEP_CPP_CFIEL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cdbllist.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cdbllist.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CDBLL=\
	".\stdafx.h"\
	".\cdbllist.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cdbllist.obj" : $(SOURCE) $(DEP_CPP_CDBLL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CDBLL=\
	".\stdafx.h"\
	".\cdbllist.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cdbllist.obj" : $(SOURCE) $(DEP_CPP_CDBLL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CDBLL=\
	".\stdafx.h"\
	".\cdbllist.h"\
	".\shwdefs.h"\
	".\cdblel.h"\
	

"$(INTDIR)\cdbllist.obj" : $(SOURCE) $(DEP_CPP_CDBLL) "$(INTDIR)"

"$(INTDIR)\cdbllist.sbr" : $(SOURCE) $(DEP_CPP_CDBLL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cdblel.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cdblel.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CDBLE=\
	".\stdafx.h"\
	".\cdblel.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\cdblel.obj" : $(SOURCE) $(DEP_CPP_CDBLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CDBLE=\
	".\stdafx.h"\
	".\cdblel.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\cdblel.obj" : $(SOURCE) $(DEP_CPP_CDBLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CDBLE=\
	".\stdafx.h"\
	".\cdblel.h"\
	".\shwdefs.h"\
	

"$(INTDIR)\cdblel.obj" : $(SOURCE) $(DEP_CPP_CDBLE) "$(INTDIR)"

"$(INTDIR)\cdblel.sbr" : $(SOURCE) $(DEP_CPP_CDBLE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cct.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cct.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CCT_C=\
	".\cct.h"\
	".\ccdll.h"\
	

"$(INTDIR)\cct.obj" : $(SOURCE) $(DEP_CPP_CCT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CCT_C=\
	".\cct.h"\
	".\ccdll.h"\
	

"$(INTDIR)\cct.obj" : $(SOURCE) $(DEP_CPP_CCT_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CCT_C=\
	".\cct.h"\
	".\ccdll.h"\
	

"$(INTDIR)\cct.obj" : $(SOURCE) $(DEP_CPP_CCT_C) "$(INTDIR)"

"$(INTDIR)\cct.sbr" : $(SOURCE) $(DEP_CPP_CCT_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cck_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cck_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CCK_D=\
	".\stdafx.h"\
	".\ind.h"\
	".\cck.h"\
	".\ddx.h"\
	".\mrfset_d.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\mkr_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\obstream.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	".\font.h"\
	

"$(INTDIR)\cck_d.obj" : $(SOURCE) $(DEP_CPP_CCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CCK_D=\
	".\stdafx.h"\
	".\ind.h"\
	".\cck.h"\
	".\ddx.h"\
	".\mrfset_d.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\mkr_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\obstream.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	".\font.h"\
	

"$(INTDIR)\cck_d.obj" : $(SOURCE) $(DEP_CPP_CCK_D) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CCK_D=\
	".\stdafx.h"\
	".\ind.h"\
	".\cck.h"\
	".\ddx.h"\
	".\mrfset_d.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\crngset.h"\
	".\crngst_d.h"\
	".\cck_d.h"\
	".\mkr_d.h"\
	".\help_ids.h"\
	".\shwdefs.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\obstream.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	".\font.h"\
	

"$(INTDIR)\cck_d.obj" : $(SOURCE) $(DEP_CPP_CCK_D) "$(INTDIR)"

"$(INTDIR)\cck_d.sbr" : $(SOURCE) $(DEP_CPP_CCK_D) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cck.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cck.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CCK_C=\
	".\stdafx.h"\
	".\cck.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\cck_d.h"\
	".\shwdefs.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\cck.obj" : $(SOURCE) $(DEP_CPP_CCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CCK_C=\
	".\stdafx.h"\
	".\cck.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\cck_d.h"\
	".\shwdefs.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\cck.obj" : $(SOURCE) $(DEP_CPP_CCK_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CCK_C=\
	".\stdafx.h"\
	".\cck.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\progress.h"\
	".\cck_d.h"\
	".\shwdefs.h"\
	".\obstream.h"\
	".\mkr.h"\
	".\crngset.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\ind.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\typ.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\prog_d.h"\
	".\EditLng.h"\
	

"$(INTDIR)\cck.obj" : $(SOURCE) $(DEP_CPP_CCK_C) "$(INTDIR)"

"$(INTDIR)\cck.sbr" : $(SOURCE) $(DEP_CPP_CCK_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ccdll.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cbo.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cbo.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_CBO_C=\
	".\stdafx.h"\
	".\cbo.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\cbo.obj" : $(SOURCE) $(DEP_CPP_CBO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_CBO_C=\
	".\stdafx.h"\
	".\cbo.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\cbo.obj" : $(SOURCE) $(DEP_CPP_CBO_C) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_CBO_C=\
	".\stdafx.h"\
	".\cbo.h"\
	".\tools.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	

"$(INTDIR)\cbo.obj" : $(SOURCE) $(DEP_CPP_CBO_C) "$(INTDIR)"

"$(INTDIR)\cbo.sbr" : $(SOURCE) $(DEP_CPP_CBO_C) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\browse_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\browse_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_BROWS=\
	".\stdafx.h"\
	".\browse_d.h"\
	".\shwdefs.h"\
	".\browse.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse_d.obj" : $(SOURCE) $(DEP_CPP_BROWS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_BROWS=\
	".\stdafx.h"\
	".\browse_d.h"\
	".\shwdefs.h"\
	".\browse.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse_d.obj" : $(SOURCE) $(DEP_CPP_BROWS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_BROWS=\
	".\stdafx.h"\
	".\browse_d.h"\
	".\shwdefs.h"\
	".\browse.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse_d.obj" : $(SOURCE) $(DEP_CPP_BROWS) "$(INTDIR)"

"$(INTDIR)\browse_d.sbr" : $(SOURCE) $(DEP_CPP_BROWS) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\browse.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\browse.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_BROWSE=\
	".\stdafx.h"\
	".\browse.h"\
	".\browse_d.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse.obj" : $(SOURCE) $(DEP_CPP_BROWSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_BROWSE=\
	".\stdafx.h"\
	".\browse.h"\
	".\browse_d.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse.obj" : $(SOURCE) $(DEP_CPP_BROWSE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_BROWSE=\
	".\stdafx.h"\
	".\browse.h"\
	".\browse_d.h"\
	".\obstream.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\mkr.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	

"$(INTDIR)\browse.obj" : $(SOURCE) $(DEP_CPP_BROWSE) "$(INTDIR)"

"$(INTDIR)\browse.sbr" : $(SOURCE) $(DEP_CPP_BROWSE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\brian.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_BRIAN=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\brian.obj" : $(SOURCE) $(DEP_CPP_BRIAN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_BRIAN=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\brian.obj" : $(SOURCE) $(DEP_CPP_BRIAN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_BRIAN=\
	".\stdafx.h"\
	".\shwdoc.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\ind.h"\
	".\typ.h"\
	".\crlookel.h"\
	".\cdbllist.h"\
	".\fil.h"\
	".\ptr.h"\
	".\cdblel.h"\
	".\crecord.h"\
	".\srt.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\var.h"\
	".\kbd.h"\
	".\km95api.h"\
	".\tools.h"\
	".\cct.h"\
	".\patmch.h"\
	".\interlin.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\project.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\dbref.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\MyMLang.h"\
	".\hint.h"\
	

"$(INTDIR)\brian.obj" : $(SOURCE) $(DEP_CPP_BRIAN) "$(INTDIR)"

"$(INTDIR)\brian.sbr" : $(SOURCE) $(DEP_CPP_BRIAN) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\booklist.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\booklist.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_BOOKL=\
	".\stdafx.h"\
	".\booklist.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\booklist.obj" : $(SOURCE) $(DEP_CPP_BOOKL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_BOOKL=\
	".\stdafx.h"\
	".\booklist.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\booklist.obj" : $(SOURCE) $(DEP_CPP_BOOKL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_BOOKL=\
	".\stdafx.h"\
	".\booklist.h"\
	".\shwdefs.h"\
	".\hugestr.h"\
	

"$(INTDIR)\booklist.obj" : $(SOURCE) $(DEP_CPP_BOOKL) "$(INTDIR)"

"$(INTDIR)\booklist.sbr" : $(SOURCE) $(DEP_CPP_BOOKL) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ambig_d.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ambig_d.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_AMBIG=\
	".\stdafx.h"\
	".\typ.h"\
	".\ambig_d.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ambig_d.obj" : $(SOURCE) $(DEP_CPP_AMBIG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_AMBIG=\
	".\stdafx.h"\
	".\typ.h"\
	".\ambig_d.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ambig_d.obj" : $(SOURCE) $(DEP_CPP_AMBIG) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_AMBIG=\
	".\stdafx.h"\
	".\typ.h"\
	".\ambig_d.h"\
	".\project.h"\
	".\wdf.h"\
	".\shwdefs.h"\
	".\set.h"\
	".\mkr.h"\
	".\interlin.h"\
	".\fil.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\ref.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\cct.h"\
	".\km95api.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\crecord.h"\
	".\cfield.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\patmch.h"\
	".\EditLng.h"\
	

"$(INTDIR)\ambig_d.obj" : $(SOURCE) $(DEP_CPP_AMBIG) "$(INTDIR)"

"$(INTDIR)\ambig_d.sbr" : $(SOURCE) $(DEP_CPP_AMBIG) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\alan.h

!IF  "$(CFG)" == "Toolbox - Win32 Release"

!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\alan.cpp

!IF  "$(CFG)" == "Toolbox - Win32 Release"

DEP_CPP_ALAN_=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\alan.h"\
	".\interlin.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\alan.obj" : $(SOURCE) $(DEP_CPP_ALAN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Win32 Debug"

DEP_CPP_ALAN_=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\alan.h"\
	".\interlin.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\alan.obj" : $(SOURCE) $(DEP_CPP_ALAN_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Toolbox - Power Macintosh Release"

DEP_CPP_ALAN_=\
	".\stdafx.h"\
	".\shw.h"\
	".\project.h"\
	".\alan.h"\
	".\interlin.h"\
	".\shwview.h"\
	".\shwdefs.h"\
	".\cdbllist.h"\
	".\cdblel.h"\
	".\crecpos.h"\
	".\trie.h"\
	".\cct.h"\
	".\crecord.h"\
	".\mkr.h"\
	".\cfield.h"\
	".\set.h"\
	".\lng.h"\
	".\patch.h"\
	".\not.h"\
	".\update.h"\
	".\lbo.h"\
	".\cbo.h"\
	".\ref.h"\
	".\srt.h"\
	".\var.h"\
	".\kbd.h"\
	".\ptr.h"\
	".\km95api.h"\
	".\tools.h"\
	".\dbref.h"\
	".\ind.h"\
	".\crlookel.h"\
	".\fil.h"\
	".\patmch.h"\
	".\recset.h"\
	".\hints.h"\
	".\shwvscrl.h"\
	".\browse.h"\
	".\undo.h"\
	".\shwdoc.h"\
	".\MyMLang.h"\
	".\hint.h"\
	".\typ.h"\
	".\jmp.h"\
	".\printinf.h"\
	".\number.h"\
	

"$(INTDIR)\alan.obj" : $(SOURCE) $(DEP_CPP_ALAN_) "$(INTDIR)"

"$(INTDIR)\alan.sbr" : $(SOURCE) $(DEP_CPP_ALAN_) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
