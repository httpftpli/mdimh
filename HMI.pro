#-------------------------------------------------
#
# Project created by QtCreator 2011-05-30T14:08:53
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += testlib
QT       += webkit



TARGET = HMI
TEMPLATE = app
TRANSLATIONS += hmi.ts
CODECFORTR   =  UTF-8

QMAKE_LFLAGS += -Wl,-Map,hmi.map



SOURCES += main.cpp\
    mdPushbutton/qmdpushbutton.cpp \
    qformfile.cpp \
    mainwindow.cpp \
    algorithm.cpp \
    communicat.cpp \
    data.cpp \
    machineexame.cpp \
    globaldata.cpp \
    mdLabel/qmdlabel.cpp \
    qmytablewidget.cpp \
    patfileform.cpp \
    paramform.cpp \
    headtestform.cpp \
    qsyslog.cpp \
    syslogform.cpp \
    updateromform.cpp \
    pattern.cpp \
    mdtableview/qmdtableview.cpp \
    calibrationtestwidget.cpp \
    calibration.cpp \
    qshazuikbform.cpp \
    qhuabanxhform.cpp \
    qshazuiexchform.cpp \
    qcnteditform.cpp \
    qparam.cpp \
    QProgressIndicator.cpp \
    formjqgzcs.cpp \
    formxtcs.cpp \
    csound.cpp \
    qmdstyle.cpp \
    qmdmessagebox.cpp \
    qmdapplication.cpp \
    qmdsplashscreen.cpp \
    qhmidata.cpp \
    dialogsetfinishcount.cpp \
    dialogtingche.cpp \
    alarmform.cpp \
    formxtcs2.cpp \
    formhead.cpp \
    formheadtest.cpp \
    helpform.cpp \
    dialogabout.cpp

HEADERS  += \
    mdPushbutton/qmdpushbutton.h \
    qformfile.h \
    mainwindow.h \
    algorithm.h \
    communicat.h \
    data.h \
    machineexame.h \
    globaldata.h \
    mdLabel/qmdlabel.h \
    qmytablewidget.h \
    patfileform.h \
    paramform.h \
    headtestform.h \
    main.h \
    appconfig.h \
    qsyslog.h \
    syslogform.h \
    updateromform.h \
    pattern.h \
    namespace.h \
    mdtableview/qmdtableview.h \
    calibrationtestwidget.h \
    calibration.h \
    qshazuikbform.h \
    qhuabanxhform.h \
    qshazuiexchform.h \
    qcnteditform.h \
    qparam.h \
    QProgressIndicator.h \
    formjqgzcs.h \
    formxtcs.h \
    csound.h \
    qmdstyle.h \
    qmdmessagebox.h \
    qmdapplication.h \
    qmdsplashscreen.h \
    qhmidata.h \
    dialogsetfinishcount.h \
    dialogtingche.h \
    customerid.h \
    alarmform.h \
    formxtcs2.h \
    formhead.h \
    formheadtest.h \
    helpform.h \
    dialogabout.h \
    config.h \
    constdata.h

FORMS    += \
    qformfile.ui \
    mainwindow.ui \
    machineexame.ui \
    patfileform.ui \
    paramform.ui \
    headtestform.ui \
    syslogform.ui \
    updateromform.ui \
    calibrationtestwidget.ui \
    qshazuikbform.ui \
    qhuabanxhform.ui \
    qshazuiexchform.ui \
    qcnteditform.ui \
    formjqgzcs.ui \
    formxtcs.ui \
    dialogsetfinishcount.ui \
    dialogtingche.ui \
    formxtcs2.ui \
    formhead.ui \
    formheadtest.ui \
    helpform.ui \
    dialogabout.ui

OTHER_FILES +=

RESOURCES += \
    resouce.qrc

#CONFIG+=assistant

linux-arm-g++{
    DEFINES += ARM6410
}
