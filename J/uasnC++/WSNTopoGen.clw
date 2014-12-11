; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWSNTopoGenDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WSNTopoGen.h"

ClassCount=4
Class1=CWSNTopoGenApp
Class2=CWSNTopoGenDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CPlotCtrl
Resource3=IDD_WSNTOPOGEN_DIALOG

[CLS:CWSNTopoGenApp]
Type=0
HeaderFile=WSNTopoGen.h
ImplementationFile=WSNTopoGen.cpp
Filter=N

[CLS:CWSNTopoGenDlg]
Type=0
HeaderFile=WSNTopoGenDlg.h
ImplementationFile=WSNTopoGenDlg.cpp
Filter=D
LastObject=IDC_BTN_UASN
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=WSNTopoGenDlg.h
ImplementationFile=WSNTopoGenDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WSNTOPOGEN_DIALOG]
Type=1
Class=CWSNTopoGenDlg
ControlCount=25
Control1=IDC_STATIC_PLOT,static,1342177287
Control2=IDC_BTN_GENERATE,button,1342242816
Control3=IDC_STATIC,static,1342308864
Control4=IDC_EDIT_NODE_CNT,edit,1350631552
Control5=IDC_STATIC,static,1073873408
Control6=IDC_EDIT_LISTEN_COST,edit,1082196096
Control7=IDC_STATIC,static,1073873408
Control8=IDC_EDIT_ADJUST_TIME,edit,1082196096
Control9=IDC_STATIC,static,1342308864
Control10=IDC_EDIT_FLOW_VELOCITY,edit,1350631552
Control11=IDC_STATIC,static,1342308864
Control12=IDC_EDIT_ENERGY_CONSUM_MODEL_COEF,edit,1350631552
Control13=IDC_STATIC,static,1342308864
Control14=IDC_EDIT_NODE_ENERGY,edit,1350631552
Control15=IDC_STATIC,static,1073873408
Control16=IDC_EDIT_RADIUS,edit,1082196096
Control17=IDC_STATIC,static,1073873408
Control18=IDC_EDIT_DO_TIME,edit,1082196096
Control19=IDC_PROGRESS,msctls_progress32,1350565889
Control20=IDC_STATIC,static,1073873408
Control21=IDC_STATIC,static,1073873408
Control22=IDC_EDIT_AVG_DURATION,edit,1082198144
Control23=IDC_EDIT_AVG_HDR_CNT,edit,1082198144
Control24=IDC_BTN_LMST,button,1342242816
Control25=IDC_BTN_UASN,button,1342242816

[CLS:CPlotCtrl]
Type=0
HeaderFile=PlotCtrl.h
ImplementationFile=PlotCtrl.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CPlotCtrl

