#include "globaldata.h"
#include "main.h"
#include "qhmidata.h"
#include "communicat.h"
#include "qparam.h"
#include "pattern.h"
#include "qmdmessagebox.h"
#include "qmdsplashscreen.h"
#include "QDir"
#include "QCoreApplication"
#include <QFileDialog>
#include<QTextStream>


QComm qComm;
QPattern patternData(&qComm);
QParam paramaData(&qComm);
QHMIData hmiData(&paramaData,&qComm);
QSysLog sysLog;
QProgressIndicator *ProgressIndiForm;


Md::Result sysInit(){
    QObject::connect(&hmiData,SIGNAL(hmi_finishCount(int)),&hmiData,
                     SLOT(on_clothFinish()));
    QObject::connect(&paramaData,SIGNAL(changed()),&hmiData,SLOT(onParamChanged()));
    QObject::connect(&patternData,SIGNAL(patternChanged(QString,QString)),
            &hmiData,SLOT(on_patternChange(QString,QString)));
    int commResult;
    QMdSplashScreen Splash(QPixmap("resource/image/matlab.png"));
    QMdSplashScreen *splash = &Splash;
    splash->show();
    hmiData.loadParam("./sysconfig.conf");
    INFORMLOG(QObject::tr("开机，系统初始化开始"));
    sysLog.setFile(hmiData.sysLogFilePath);
    QPattern::Result r;
    r = patternData.setFile(hmiData.patternPath);
    if(QPattern::Ok!=r){
        if(r==QPattern::NoCntFile){
            splash->showMessage(QObject::tr("载入花型:------------缺少CNT文件"),Qt::AlignBottom);
        }else if(r==QPattern::NoPatFile){
            splash->showMessage(QObject::tr("载入花型:------------缺少PAT文件"),Qt::AlignBottom);
        }else{
            splash->showMessage(QObject::tr("载入花型:------------花型文件错误"),Qt::AlignBottom);
        }
        sleep(3);
        return Md::NotPatCntSaz;
    }
    paramaData.setFile(hmiData.spaFilePath);

    ////start communication//////////////
    qComm.start();
    /////poll valible rom////////////////
    if(!hmiData.mainboardRomAvailable()){
        splash->showMessage(QObject::tr("检查主板固件:----------主控板无固件"),Qt::AlignBottom);
        sleep(3);
        return Md::MainBoardNoRom;
    }
    /////test comm
    if(!qComm.isAvailable()){
        splash->showMessage(QObject::tr("测试通信:------------通信异常"),Qt::AlignBottom);
        sleep(3);
        return Md::CommError;
    }

    //////togle system/////////////////
    commResult = qComm.TogSysStat(QHMIData::SysInParam);
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,花型未成功下载"),Qt::AlignBottom);
        sleep(3);
        return Md::CommError;
    }
    if(patternData.isPatternAvailable()){
        ////download wrk spa
        splash->showMessage(QObject::tr("正在下载参数"),Qt::AlignBottom);
        commResult = paramaData.sendParama();
        if(commResult == Md::CommError){
            splash->showMessage(QObject::tr("通讯错误,参数未成功下载"),Qt::AlignBottom);
            return Md::CommError;
        }
        ///download szkb///////////////////////////////////
        commResult = patternData.sendShazuiKb();
        if(commResult == Md::CommError){
            splash->showMessage(QObject::tr("下载SAZ文件，通讯错误"),Qt::AlignBottom);
            sleep(3);
            return Md::CommError;
        }


        ///////下载cnt文件///////////////////////////////////
        QObject::connect(&patternData,SIGNAL(patternSendPercent(int)),splash,SLOT(showPatternSendMessage(int)),Qt::QueuedConnection);
        commResult = patternData.sendPattern();
        if(commResult == Md::CommError){
            splash->showMessage(QObject::tr("下载CNT文件，通讯错误"),Qt::AlignBottom);
            sleep(3);
            return Md::CommError;
        }
        QObject::disconnect(&patternData,SIGNAL(patternSendPercent(int)),splash,SLOT(showPatternSendMessage(int)));
        //等待下位机响应/////////////////////////////////
        int i=0;
        for(i=0;i<3;i++) {
            commResult = qComm.TogSysStat(QHMIData::SysInParam);
            if(commResult==Md::Ok)
                break;
        }
        if(3==i){
            splash->showMessage(QObject::tr("下载花型后主机未成功响应"),Qt::AlignBottom);
            sleep(3);
            return Md::CommError;
        }
    }
    //运行时参数设置//////////////////////////////////////////////
    commResult = hmiData.sendParamaInRun();
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,运行时参数设置失败"),Qt::AlignBottom);
        sleep(3);
        return Md::CommError;
    }
    //确认厂商ID///////////////////////////////////////////////
    if(hmiData.customerId!=0xffff){
        commResult = qComm.checkCustomerId(hmiData.customerId);
        if(commResult==Md::CommError){
            splash->showMessage(QObject::tr("通讯错误,查询厂商ID失败"),Qt::AlignBottom);
            sleep(3);
            return Md::CommError;
        }else if(commResult == Md::CustomerIdNotPass){
            splash->showMessage(QObject::tr("厂商ID不匹配"),Qt::AlignBottom);
            sleep(3);
            return Md::CustomerIdNotPass;
        }
    }
    ///查询版本/////////////////////////////////////////////////
    hmiData.pollSysVersion();
    INFORMLOG(QObject::tr("开机初始化成功"));
    return Md::Ok;
}

QString secondToString(int sec){
    unsigned  char second,minute,hour;
    second = sec%60;
    minute = sec/60%60;
    hour = sec/60/60;
    return QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
}



void MessageOutput(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("HMI_Debug: %1").arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("HMI_Warning: %1").arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("HMI_Critical: %1").arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("HMI_Fatal: %1").arg(msg);
    }
    QFile outFile("hmi_debuglog.txt");
    if(outFile.size()>(1024*1024*5))
        return;
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}


