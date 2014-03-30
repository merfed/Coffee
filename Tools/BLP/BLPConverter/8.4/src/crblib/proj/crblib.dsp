# Microsoft Developer Studio Project File - Name="crblib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=crblib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "crblib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "crblib.mak" CFG="crblib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "crblib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "crblib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""Perforce Project""
# PROP Scc_LocalPath ".."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "crblib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /O2 /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\ms_crb.lib"

!ELSEIF  "$(CFG)" == "crblib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\mscrblibd.lib"

!ENDIF 

# Begin Target

# Name "crblib - Win32 Release"
# Name "crblib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\arithc.c
# End Source File
# Begin Source File

SOURCE=..\arithc.h
# End Source File
# Begin Source File

SOURCE=..\bbitio.c
# End Source File
# Begin Source File

SOURCE=..\bbitio.h
# End Source File
# Begin Source File

SOURCE=..\bmpio.c
# End Source File
# Begin Source File

SOURCE=..\bmpio.h
# End Source File
# Begin Source File

SOURCE=..\cacmari.c
# End Source File
# Begin Source File

SOURCE=..\cacmari.h
# End Source File
# Begin Source File

SOURCE=..\chshutil.c
# End Source File
# Begin Source File

SOURCE=..\chshutil.h
# End Source File
# Begin Source File

SOURCE=..\cindcatr.c
# End Source File
# Begin Source File

SOURCE=..\cindcatr.h
# End Source File
# Begin Source File

SOURCE=..\codeutil.c
# End Source File
# Begin Source File

SOURCE=..\codeutil.h
# End Source File
# Begin Source File

SOURCE=..\context.c
# End Source File
# Begin Source File

SOURCE=..\context.h
# End Source File
# Begin Source File

SOURCE=..\crbconv.c
# End Source File
# Begin Source File

SOURCE=..\crbconv.h
# End Source File
# Begin Source File

SOURCE=..\crbeqlib.c
# End Source File
# Begin Source File

SOURCE=..\crbeqlib.h
# End Source File
# Begin Source File

SOURCE=..\crbequtil.c
# End Source File
# Begin Source File

SOURCE=..\crc32.c
# End Source File
# Begin Source File

SOURCE=..\crc32.h
# End Source File
# Begin Source File

SOURCE=..\dbf.c
# End Source File
# Begin Source File

SOURCE=..\equtil.c
# End Source File
# Begin Source File

SOURCE=..\equtil.h
# End Source File
# Begin Source File

SOURCE=..\farithc.c
# End Source File
# Begin Source File

SOURCE=..\farithc.h
# End Source File
# Begin Source File

SOURCE=..\fbitio.c
# End Source File
# Begin Source File

SOURCE=..\fbitio.h
# End Source File
# Begin Source File

SOURCE=..\fileutil.c
# End Source File
# Begin Source File

SOURCE=..\fileutil.h
# End Source File
# Begin Source File

SOURCE=..\fileutio.c
# End Source File
# Begin Source File

SOURCE=..\floatutil.c
# End Source File
# Begin Source File

SOURCE=..\floatutil.h
# End Source File
# Begin Source File

SOURCE=..\huffa.c
# End Source File
# Begin Source File

SOURCE=..\huffa.h
# End Source File
# Begin Source File

SOURCE=..\huffman2.c
# End Source File
# Begin Source File

SOURCE=..\huffman2.h
# End Source File
# Begin Source File

SOURCE=..\imutil.c
# End Source File
# Begin Source File

SOURCE=..\imutil.h
# End Source File
# Begin Source File

SOURCE=..\inc.h
# End Source File
# Begin Source File

SOURCE=..\intmath.c
# End Source File
# Begin Source File

SOURCE=..\intmath.h
# End Source File
# Begin Source File

SOURCE=..\ladder.c
# End Source File
# Begin Source File

SOURCE=..\ladder.h
# End Source File
# Begin Source File

SOURCE=..\lbitio.c
# End Source File
# Begin Source File

SOURCE=..\lbitio.h
# End Source File
# Begin Source File

SOURCE=..\list.c
# End Source File
# Begin Source File

SOURCE=..\list.h
# End Source File
# Begin Source File

SOURCE=..\log.c
# End Source File
# Begin Source File

SOURCE=..\log.h
# End Source File
# Begin Source File

SOURCE=..\matchpat.c
# End Source File
# Begin Source File

SOURCE=..\matchpat.h
# End Source File
# Begin Source File

SOURCE=..\Matrix.c
# End Source File
# Begin Source File

SOURCE=..\Matrix.h
# End Source File
# Begin Source File

SOURCE=..\mempool.c
# End Source File
# Begin Source File

SOURCE=..\mempool.h
# End Source File
# Begin Source File

SOURCE=..\memutil.c
# End Source File
# Begin Source File

SOURCE=..\memutil.h
# End Source File
# Begin Source File

SOURCE=..\mtf.c
# End Source File
# Begin Source File

SOURCE=..\mtf.h
# End Source File
# Begin Source File

SOURCE=..\myassert.c
# End Source File
# Begin Source File

SOURCE=..\myassert.h
# End Source File
# Begin Source File

SOURCE=..\o0coder.c
# End Source File
# Begin Source File

SOURCE=..\o0coder.h
# End Source File
# Begin Source File

SOURCE=..\o1coder.c
# End Source File
# Begin Source File

SOURCE=..\o1coder.h
# End Source File
# Begin Source File

SOURCE=..\report.c
# End Source File
# Begin Source File

SOURCE=..\report.h
# End Source File
# Begin Source File

SOURCE=..\rungae.c
# End Source File
# Begin Source File

SOURCE=..\rungae.h
# End Source File
# Begin Source File

SOURCE=..\rungo1.c
# End Source File
# Begin Source File

SOURCE=..\rungo1.h
# End Source File
# Begin Source File

SOURCE=..\runtrans.c
# End Source File
# Begin Source File

SOURCE=..\runtrans.h
# End Source File
# Begin Source File

SOURCE=..\scontext.c
# End Source File
# Begin Source File

SOURCE=..\scontext.h
# End Source File
# Begin Source File

SOURCE=..\sortnods.c
# End Source File
# Begin Source File

SOURCE=..\sortnods.h
# End Source File
# Begin Source File

SOURCE=..\soz.c
# End Source File
# Begin Source File

SOURCE=..\soz.h
# End Source File
# Begin Source File

SOURCE=..\spawnmutil.c
# End Source File
# Begin Source File

SOURCE=..\spawnmutil.h
# End Source File
# Begin Source File

SOURCE=..\strutil.c
# End Source File
# Begin Source File

SOURCE=..\strutil.h
# End Source File
# Begin Source File

SOURCE=..\timer.c
# End Source File
# Begin Source File

SOURCE=..\timer.h
# End Source File
# Begin Source File

SOURCE=..\tsc.c
# End Source File
# Begin Source File

SOURCE=..\tsc.h
# End Source File
# Begin Source File

SOURCE=..\Vector.c
# End Source File
# Begin Source File

SOURCE=..\Vector.h
# End Source File
# Begin Source File

SOURCE=..\vqarith.c
# End Source File
# Begin Source File

SOURCE=..\vqarith.h
# End Source File
# Begin Source File

SOURCE=..\walker.c
# End Source File
# Begin Source File

SOURCE=..\walker.h
# End Source File
# End Group
# End Target
# End Project
