; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=NInfoFileDialog
LastTemplate=CFileDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NCMFileInfo.h"
ODLFile=NCMFileInfo.odl

ClassCount=5
Class1=CNCMFileInfoApp
Class2=CNCMFileInfoDlg
Class3=CAboutDlg
Class4=CNCMFileInfoDlgAutoProxy

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX (English (U.S.))
Class5=NInfoFileDialog
Resource4=IDD_NCMFILEINFO_DIALOG

[CLS:CNCMFileInfoApp]
Type=0
HeaderFile=NCMFileInfo.h
ImplementationFile=NCMFileInfo.cpp
Filter=N

[CLS:CNCMFileInfoDlg]
Type=0
HeaderFile=NCMFileInfoDlg.h
ImplementationFile=NCMFileInfoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_PROGRAMS2

[CLS:CAboutDlg]
Type=0
HeaderFile=NCMFileInfoDlg.h
ImplementationFile=NCMFileInfoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[CLS:CNCMFileInfoDlgAutoProxy]
Type=0
HeaderFile=DlgProxy.h
ImplementationFile=DlgProxy.cpp
BaseClass=CCmdTarget
Filter=N

[DLG:IDD_NCMFILEINFO_DIALOG]
Type=1
Class=CNCMFileInfoDlg
ControlCount=10
Control1=IDC_PROGRAMS,SysListView32,1350664321
Control2=IDC_EDIT1,edit,1342244993
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT2,edit,1342244993
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT3,edit,1350631552
Control8=IDC_EDIT4,edit,1350631552
Control9=IDC_TOOLS,SysListView32,1350664321
Control10=IDC_EDIT5,edit,1342244865

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[CLS:NInfoFileDialog]
Type=0
HeaderFile=NInfoFileDialog.h
ImplementationFile=NInfoFileDialog.cpp
BaseClass=CFileDialog
Filter=D
VirtualFilter=dWC

