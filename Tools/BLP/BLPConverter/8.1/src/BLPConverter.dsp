# Microsoft Developer Studio Project File - Name="BLPConverter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=BLPConverter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BLPConverter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BLPConverter.mak" CFG="BLPConverter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BLPConverter - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "BLPConverter - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BLPConverter - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "c:\devel\lpng128" /I "c:\devel\zlib123" /I ".." /I "..\lpng128" /I "..\zlib123" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 ati_compress.lib mscrblibd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"c:\Lua/BLPConverter.exe" /libpath:"..\crblib" /libpath:"..\ATI_COMPRESS"

!ELSEIF  "$(CFG)" == "BLPConverter - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I ".." /I "..\lpng128" /I "..\zlib123" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ati_compress.lib mscrblibd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"F:\WoW\Data\Item\BLPConverter.exe" /pdbtype:sept /libpath:"..\crblib" /libpath:"..\ATI_COMPRESS"

!ENDIF 

# Begin Target

# Name "BLPConverter - Win32 Release"
# Name "BLPConverter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BLPConverter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "PNGLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\lpng128\png.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\png.h
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngconf.h
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngerror.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngget.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngmem.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngpread.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngread.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngrio.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngrtran.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngrutil.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngset.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngtrans.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngwio.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngwrite.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngwtran.c
# End Source File
# Begin Source File

SOURCE=..\lpng128\pngwutil.c
# End Source File
# End Group
# Begin Group "ZLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Zlib123\adler32.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\compress.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\crc32.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\crc32.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\deflate.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\deflate.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\gzio.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\infback.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inffast.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inffast.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inffixed.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inflate.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inflate.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\inftrees.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\trees.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\trees.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\zconf.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\zconf.in.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\zlib.h
# End Source File
# Begin Source File

SOURCE=..\Zlib123\zutil.c
# End Source File
# Begin Source File

SOURCE=..\Zlib123\zutil.h
# End Source File
# End Group
# Begin Group "palbmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\palbmp\colorconv.c
# End Source File
# Begin Source File

SOURCE=..\palbmp\colorconv.h
# End Source File
# Begin Source File

SOURCE=..\palbmp\palcreate.c
# End Source File
# Begin Source File

SOURCE=..\palbmp\palcreate.h
# End Source File
# Begin Source File

SOURCE=..\palbmp\palettize.c
# End Source File
# Begin Source File

SOURCE=..\palbmp\palettize.h
# End Source File
# Begin Source File

SOURCE=..\palbmp\paloptimize.c
# End Source File
# Begin Source File

SOURCE=..\palbmp\paloptimize.h
# End Source File
# End Group
# End Target
# End Project
