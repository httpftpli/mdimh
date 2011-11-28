#-------------------------------------------------
#
# Project created by QtCreator 2011-05-30T14:08:53
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += testlib


TARGET = HMI
TEMPLATE = app


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
    QMdPlainTextEdit/qmdplaintextedit.cpp \
    paramform.cpp \
    headtestform.cpp \
    qsyslog.cpp \
    syslogform.cpp \
    updateromform.cpp \
    frame.cpp \
    systest.cpp \
    pattern.cpp \
    inputpanel.cpp \
    qmdinputcontext.cpp \
    mdtableview/qmdtableview.cpp \
    calibrationtestwidget.cpp \
    calibration.cpp \
    qshazuikbform.cpp \
    qhuabanxhform.cpp \
    qshazuiexchform.cpp \
    qcnteditform.cpp \
    qparam.cpp \
    formmovie.cpp \
    QProgressIndicator.cpp \
    formjqgzcs.cpp \
    formxtcs.cpp \
    csound.cpp \
    qmdstyle.cpp \
    qmdmessagebox.cpp \
    qmdapplication.cpp \
    qmdsplashscreen.cpp \
    qhmidata.cpp \
    dialogsetfinishcount.cpp

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
    QMdPlainTextEdit/qmdplaintextedit.h \
    paramform.h \
    headtestform.h \
    main.h \
    appconfig.h \
    qsyslog.h \
    syslogform.h \
    updateromform.h \
    frame.h \
    systest.h \
    pattern.h \
    namespace.h \
    inputpanel.h \
    qmdinputcontext.h \
    mdtableview/qmdtableview.h \
    calibrationtestwidget.h \
    calibration.h \
    qshazuikbform.h \
    qhuabanxhform.h \
    qshazuiexchform.h \
    qcnteditform.h \
    qparam.h \
    formmovie.h \
    QProgressIndicator.h \
    formjqgzcs.h \
    formxtcs.h \
    csound.h \
    qmdstyle.h \
    qmdmessagebox.h \
    qmdapplication.h \
    qmdsplashscreen.h \
    qhmidata.h \
    dialogsetfinishcount.h

FORMS    += \
    qformfile.ui \
    mainwindow.ui \
    machineexame.ui \
    patfileform.ui \
    paramform.ui \
    headtestform.ui \
    syslogform.ui \
    updateromform.ui \
    frame.ui \
    inputpanel.ui \
    calibrationtestwidget.ui \
    qshazuikbform.ui \
    qhuabanxhform.ui \
    qshazuiexchform.ui \
    qcnteditform.ui \
    formmovie.ui \
    formjqgzcs.ui \
    formxtcs.ui \
    dialogsetfinishcount.ui

OTHER_FILES +=

RESOURCES += \
    resouce.qrc

#CONFIG+=assistant


linux-arm-g++{
    DEFINES += ARM6410
}
