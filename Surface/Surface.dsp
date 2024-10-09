# Microsoft Developer Studio Project File - Name="Surface" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Surface - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Surface.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Surface.mak" CFG="Surface - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Surface - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Surface - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/NCM24/Source", YLAAAAAA"
# PROP Scc_LocalPath "..\source"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Surface - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\lib\release"
# PROP Intermediate_Dir "..\Release\Surface"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\common" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\release/Surface.dll"

!ELSEIF  "$(CFG)" == "Surface - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib\Debug"
# PROP Intermediate_Dir "../Debug\Surface"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\common" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../Debug\Surface.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Surface - Win32 Release"
# Name "Surface - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Build.cpp
# End Source File
# Begin Source File

SOURCE=.\NCadrParam.cpp
# End Source File
# Begin Source File

SOURCE=.\NContour.cpp
# End Source File
# Begin Source File

SOURCE=.\NCubicPiece.cpp
# End Source File
# Begin Source File

SOURCE=.\NInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\NLine.cpp
# End Source File
# Begin Source File

SOURCE=.\NPartSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\NPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\NSquarePiece.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMComTool.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMCone.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMContour.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMHorizontal.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMSimTool.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\NSurFMTorahs.cpp
# End Source File
# Begin Source File

SOURCE=.\NTreeFMCadrs.cpp
# End Source File
# Begin Source File

SOURCE=.\NTreeFMCone.cpp
# End Source File
# Begin Source File

SOURCE=.\NTreeFMContour.cpp
# End Source File
# Begin Source File

SOURCE=.\NTreeFMHorizontal.cpp
# End Source File
# Begin Source File

SOURCE=.\NTreeFMOneCadr.cpp
# End Source File
# Begin Source File

SOURCE=.\NTrimPiece.cpp
# End Source File
# Begin Source File

SOURCE=.\NUVPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\PartArray.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Surface.cpp
# End Source File
# Begin Source File

SOURCE=.\Surface.def
# End Source File
# Begin Source File

SOURCE=.\Surface.rc
# End Source File
# Begin Source File

SOURCE=.\ZeroIN.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EssayLine.h
# End Source File
# Begin Source File

SOURCE=.\NCadrParam.h
# End Source File
# Begin Source File

SOURCE=.\NContour.h
# End Source File
# Begin Source File

SOURCE=.\NCubicPiece.h
# End Source File
# Begin Source File

SOURCE=..\Common\NInterface.h
# End Source File
# Begin Source File

SOURCE=.\NLine.h
# End Source File
# Begin Source File

SOURCE=..\Common\NPartSurface.h
# End Source File
# Begin Source File

SOURCE=.\NPlane.h
# End Source File
# Begin Source File

SOURCE=.\NSquarePiece.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMComTool.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMCone.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMContour.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMHorizontal.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMSimTool.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMSphere.h
# End Source File
# Begin Source File

SOURCE=.\NSurFMTorahs.h
# End Source File
# Begin Source File

SOURCE=.\NTreeFMCadrs.h
# End Source File
# Begin Source File

SOURCE=.\NTreeFMCone.h
# End Source File
# Begin Source File

SOURCE=.\NTreeFMContour.h
# End Source File
# Begin Source File

SOURCE=.\NTreeFMHorizontal.h
# End Source File
# Begin Source File

SOURCE=.\NTreeFMOneCadr.h
# End Source File
# Begin Source File

SOURCE=.\NTrimPiece.h
# End Source File
# Begin Source File

SOURCE=..\Common\NUVPoint.h
# End Source File
# Begin Source File

SOURCE=.\NUVPoint.h
# End Source File
# Begin Source File

SOURCE=..\Common\PartArray.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\surEnums.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Surface.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
