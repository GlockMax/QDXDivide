; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=1
ResourceCount=6
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_UNIT_COMP
Resource2=IDD_UNIT_NAME
Resource3=IDD_UNIT_SPEED
Resource4=IDD_UNIT_POSITION
Resource5=IDD_UNIT_UCS
LastClass=NUnitNameDlg
LastTemplate=CDialog
Class1=NUnitNameDlg
Resource6=IDD_UNIT_MODE

[DLG:IDD_UNIT_COMP]
Type=1
Class=?
ControlCount=4
Control1=IDC_LIST1,SysListView32,1350631425
Control2=IDC_COMP_KEY,edit,1350633602
Control3=IDC_COMP_VAL,edit,1350631554
Control4=IDC_COMP_NULL,button,1342242816

[DLG:IDD_UNIT_UCS]
Type=1
Class=?
ControlCount=7
Control1=IDC_UCS_LIST,SysListView32,1350631953
Control2=IDC_UCS_X,edit,1350631552
Control3=IDC_UCS_Y,edit,1350631552
Control4=IDC_UCS_Z,edit,1350631552
Control5=IDC_STATIC,static,1342308865
Control6=IDC_STATIC,static,1342308865
Control7=IDC_STATIC,static,1342308865

[DLG:IDD_UNIT_SPEED]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_FEED_MEAS_MMpMIN,button,1342177289
Control3=IDC_FEED_MEAS_MMpREV,button,1342177289
Control4=IDC_STATIC,static,1342308352
Control5=IDC_FEED,combobox,1344339971
Control6=IDC_STATIC,static,1342308352
Control7=IDC_FEED_SCALE,combobox,1344339971

[DLG:IDD_UNIT_POSITION]
Type=1
Class=?
ControlCount=10
Control1=IDC_STATIC,static,1342308352
Control2=IDC_START_X,edit,1350631552
Control3=IDC_START_Y,edit,1350631552
Control4=IDC_START_Z,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_TOOL_POS,edit,1350631552
Control10=IDC_DEBUG,button,1342242819

[DLG:IDD_UNIT_NAME]
Type=1
Class=NUnitNameDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_UNIT_NAME,listbox,1353777411

[DLG:IDD_UNIT_MODE]
Type=1
Class=?
ControlCount=7
Control1=IDC_STATIC,static,1342308352
Control2=IDC_ABSOLUTE_COORD_PROGR,button,1342177289
Control3=IDC_RELATIVE_COORD_PROGR,button,1342177289
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_MEAS_MM,button,1342177289
Control7=IDC_MEAS_INCH,button,1342177289

[CLS:NUnitNameDlg]
Type=0
HeaderFile=NUnitNameDlg.h
ImplementationFile=NUnitNameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_UNIT_NAME
VirtualFilter=dWC

