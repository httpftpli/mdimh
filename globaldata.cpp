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
#include <QTest>

QComm qComm;
QPatternData patternData(&qComm);
QParam paramaData(&qComm);
QHMIData hmiData(&paramaData,&qComm);
QSysLog sysLog;
QProgressIndicator *ProgressIndiForm;


Md::Result sysInit(){
    QObject::connect(&hmiData,SIGNAL(hmi_finishCount(int)),&hmiData,
                     SLOT(on_clothFinish()));
    QObject::connect(&paramaData,SIGNAL(changed()),&hmiData,SLOT(onParamChanged()));
    QObject::connect(&patternData,SIGNAL(patternChanged(const QString &,const QString &,const QString &,const QString &,const QString &)),
            &hmiData,SLOT(on_patternChange(const QString &,const QString &,const QString &,const QString &,const QString &)));
    hmiData.loadParam("./sysconfig.conf");
    INFORMLOG(QObject::tr("开机，系统初始化开始"));
    sysLog.setFile(hmiData.sysLogFilePath);
    QMdSplashScreen Splash(QPixmap("resource/image/matlab.png"));
    QMdSplashScreen *splash = &Splash;
    splash->show();
    QMdMessageBox box;
    QPatternData::Result r;
    r = patternData.setFile(hmiData.cntFilePath,hmiData.patFilePath,hmiData.wrkFilePath);
    if(r==QPatternData::Ok){
        patternData.loadFile(Md::HAVEWRK|Md::HAVECNT);
    }else if(r==QPatternData::NoCntFile){
        box.setIcon(QMessageBox::Warning);
        box.setText(QObject::tr("载入花型"));
        box.setInformativeText(QObject::tr("找不到花型，缺少CNT文件"));
        box.setStandardButtons(QMessageBox::Ok);
        box.setDefaultButton(QMessageBox::Ok);
        box.exec();
    }else if(r==QPatternData::NoPatFile){
        box.setIcon(QMessageBox::Warning);
        box.setText(QObject::tr("载入花型"));
        box.setInformativeText(QObject::tr("找不到花型，缺少PAT文件"));
        box.setStandardButtons(QMessageBox::Ok);
        box.setDefaultButton(QMessageBox::Ok);
        box.exec();
    }else{
        box.setIcon(QMessageBox::Warning);
        box.setText(QObject::tr("载入花型"));
        box.setInformativeText(QObject::tr("花型文件错误"));
        box.setStandardButtons(QMessageBox::Ok);
        box.setDefaultButton(QMessageBox::Ok);
        box.exec();
    }
    paramaData.setFile(hmiData.spaFilePath);
    int commResult;
    /////poll valible rom////////////////
    commResult  = qComm.IsInBoot();
    if(commResult & Md::InBootState){
        box.setText(QObject::tr("初始化"));
        box.setIcon(QMessageBox::Question);
        box.setInformativeText(QObject::tr("主控板无固件，是否下载"));
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        if(box.exec()==QMessageBox::Yes){
            QString filename = QFileDialog::getOpenFileName(splash,
                   QObject::tr("打开文件"),QString("./rom"),QObject::tr("固件文件 (*.bin *.min)"));
            if(filename!=""){
                QFile file(filename);
                commResult = qComm.SendBin(file,splash);
                if(commResult == Md::Ok){
                    box.setText(QObject::tr("下载固件"));
                    box.setInformativeText(QObject::tr("下载成功"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.setIcon(QMessageBox::Information);
                    box.exec();
                }
                if(commResult & Md::CommError){
                    box.setInformativeText(QObject::tr("通信出错"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.setIcon(QMessageBox::Warning);
                    box.exec();
                    return Md::MainBoardNoRom;
                }
                if(commResult & Md::EraseError){
                    box.setInformativeText(QObject::tr("擦除错误"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.exec();
                    return Md::MainBoardNoRom;
                }
                if(commResult == Md::BurnError){
                    box.setInformativeText(QObject::tr("烧写错误"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.exec();
                    return Md::MainBoardNoRom;
                }
            }else{
                return Md::MainBoardNoRom;
            }
        }  else{
            return Md::MainBoardNoRom;
        }

    } else if(commResult & Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,花型文件未成功下载"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;
    }

    //////togle system/////////////////
    commResult = qComm.TogSysStat(QHMIData::SysInParam);
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,花型文件未成功下载"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;
    }
    splash->showMessage(QObject::tr("正在下载参数"),Qt::AlignBottom);
    QFile wrkfile(hmiData.wrkFilePath);
    QFile spafile(hmiData.spaFilePath);
    commResult = qComm.SendParama(wrkfile,spafile,0xff);
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,参数未成功下载"),Qt::AlignBottom);
        return Md::CommError;
    }
    ///szkb///////////////////////////////////
    commResult = patternData.sendShazuiKb();
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("下载SAZ文件，通讯错误"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;;
    }
    ///////下载cnt文件///////////////////////////////////
    QObject::connect(&qComm,SIGNAL(commPercent(int)),splash,SLOT(showCntMessage(int)),Qt::QueuedConnection);
    commResult = qComm.sendFile(hmiData.cntFilePath,0,TRUE,NULL);
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("下载CNT文件，通讯错误"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;;
    }
    QObject::disconnect(&qComm,SIGNAL(commPercent(int)),splash,SLOT(showCntMessage(int)));
    QObject::connect(&qComm,SIGNAL(commPercent(int)),splash,SLOT(showPatMessage(int)),Qt::QueuedConnection);
    commResult = qComm.sendFile(hmiData.patFilePath,0,TRUE,NULL);
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("下载PAT文件，通讯错误"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;
    }
    QObject::disconnect(&qComm,SIGNAL(commPercent(int)),splash,SLOT(showPatMessage(int)));

    //等待下位机响应/////////////////////////////////
    int i=0;
    for(i=0;i<3;i++) {
        commResult = qComm.TogSysStat(QHMIData::SysInParam);
        if(commResult==Md::Ok)
            break;
    }
    if(3==i){
        splash->showMessage(QObject::tr("下载花型后主机未成功响应"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;
    }
    //运行时参数设置//////////////////////////////////////////////
    commResult = hmiData.sendParamaInRun();
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,运行时参数设置失败"),Qt::AlignBottom);
        QTest::qWait(2000);
        return Md::CommError;
    }
    if(hmiData.customerId!=0xffff){
        commResult = qComm.checkCustomerId(hmiData.customerId);
        if(commResult==Md::CommError){
            splash->showMessage(QObject::tr("通讯错误,查询厂商ID失败"),Qt::AlignBottom);
            QTest::qWait(2000);
            return Md::CommError;
        }else if(commResult == Md::CustomerIdNotPass){
            splash->showMessage(QObject::tr("厂商ID不匹配"),Qt::AlignBottom);
            QTest::qWait(2000);
            return Md::CustomerIdNotPass;
        }
    }
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



