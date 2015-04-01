# Microsoft Developer Studio Project File - Name="zlIP" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=zlIP - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "zlIP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "zlIP.mak" CFG="zlIP - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zlIP - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "zlIP - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "zlIP - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "zlIP - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "zlIP - Win32 Release"
# Name "zlIP - Win32 Debug"
# Begin Group "TCPIP"

# PROP Default_Filter ""
# Begin Group "TCPIPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\TCPIP\icmp.c
# End Source File
# Begin Source File

SOURCE=..\TCPIP\IP.c
# End Source File
# Begin Source File

SOURCE=..\TCPIP\NetIf.c
# End Source File
# Begin Source File

SOURCE=..\TCPIP\TCP.c
# End Source File
# Begin Source File

SOURCE=..\TCPIP\TCPIPmem.c
# End Source File
# End Group
# Begin Group "TCPIPH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\TCPIP\icmp.h
# End Source File
# Begin Source File

SOURCE=..\TCPIP\IP.h
# End Source File
# Begin Source File

SOURCE=..\TCPIP\NetIf.h
# End Source File
# Begin Source File

SOURCE=..\TCPIP\TCP.h
# End Source File
# Begin Source File

SOURCE=..\TCPIP\TCPIPmem.h
# End Source File
# End Group
# End Group
# Begin Group "Netif"

# PROP Default_Filter ""
# Begin Group "NetifC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Netif\ARP.c
# End Source File
# Begin Source File

SOURCE=..\Netif\etherif.c
# End Source File
# Begin Source File

SOURCE=..\Netif\RTL8019.c
# End Source File
# End Group
# Begin Group "NetifH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Netif\ARP.h
# End Source File
# Begin Source File

SOURCE=..\Netif\etherif.h
# End Source File
# Begin Source File

SOURCE=..\Netif\RTL8019.h
# End Source File
# End Group
# End Group
# Begin Group "GloblDef"

# PROP Default_Filter ""
# Begin Group "GloblDefC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GloblDef\GloblDef.c
# End Source File
# End Group
# Begin Group "GloblDefH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\GloblDef\GloblDef.h
# End Source File
# End Group
# End Group
# Begin Group "App"

# PROP Default_Filter ""
# Begin Group "AppC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\App\main.c
# End Source File
# End Group
# Begin Group "AppH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\App\main.h
# End Source File
# End Group
# End Group
# End Target
# End Project
