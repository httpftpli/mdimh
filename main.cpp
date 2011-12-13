
#include <QtGui/QApplication>
#include <QtGui/QWSServer>
#include "mainwindow.h"
#include "algorithm.h"
#include "communicat.h"
#include "data.h"
#include "globaldata.h"
#include "QFont"
#include "qmdinputcontext.h"
#include "qmdmessagebox.h"
#include "qmdapplication.h"
#include "qhmidata.h"
#include "alarmform.h"
#include <QTest>



int main(int argc, char *argv[])
{
    QMdApplication a(argc, argv);
#ifdef  Q_WS_X11
    QApplication::setStyle("plastique");
#endif
#ifdef  Q_WS_QWS
    QMdInputContext inputcontext;
    a.setInputContext(&inputcontext);
    QWSServer::setCursorVisible(FALSE);
#endif
    QTextCodec *textcode = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForCStrings(textcode);
    QTextCodec::setCodecForTr(textcode);
    QTextCodec::setCodecForLocale(textcode);
    QTextCodec::setCodecForCStrings(textcode);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    a.setMouseClickSound("resource/sound/click.wav");
    MainWindow w;
    qRcv.start();
    Md::Result r= sysInit();
    if(r!=Md::Ok){
        QMdMessageBox box;
        box.exec(QObject::tr("控制板初始化"),QObject::tr("初始化错误"),QMessageBox::Critical,
                 QMessageBox::Cancel,QMessageBox::Cancel);
    }
    hmiData.startTimer1s();
    w.show();
    QTest::qWaitForWindowShown(&w);
    AlarmForm alarmform(&hmiData);
    hmiData.start();
    a.setscreenprodelay(20);
    a.setscreenprodelay(20);
    return a.exec();
}



