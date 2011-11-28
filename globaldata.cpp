#include "globaldata.h"
#include "main.h"
#include "qhmidata.h"
#include "communicat.h"
#include <QMutex>
#include "qparam.h"
#include "pattern.h"
#include "qmdmessagebox.h"
#include "qmdsplashscreen.h"
#include "QDir"
#include "QCoreApplication"
#include <QFileDialog>

QSend qSend;
QRcv  qRcv;
QMutex mutex;
QPatternData patternData(&qSend);
QParam paramaData(&qSend);
QHMIData hmiData(&qSend);
QSysLog sysLog;
QProgressIndicator *ProgressIndiForm;


Md::InitResult sysInit(const QString &filename){
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QObject::connect(&qRcv,SIGNAL(DataChanged(unsigned short,QVariant)),&hmiData,
                     SLOT(On_DataChanged_FromCtrl(unsigned short,QVariant)));
    QObject::connect(&qSend,SIGNAL(NotifyRcver(unsigned char,bool)),&qRcv,
                     SLOT(On_NotifyRcver(unsigned char,bool)));
    QObject::connect(&hmiData,SIGNAL(hmi_finishCount(int)),&hmiData,
                     SLOT(on_clothFinish()));
    INFORMLOG(QObject::tr("开机，系统初始化开始"));
    QObject::connect(&patternData,SIGNAL(patternChanged(const QString &,const QString &,const QString &,const QString &,const QString &)),
            &hmiData,SLOT(on_patternChange(const QString &,const QString &,const QString &,const QString &,const QString &)));
    hmiData.loadParam(filename);
    QPatternData::Result r;
    r = patternData.setFile(hmiData.cntFilePath,hmiData.patFilePath,hmiData.wrkFilePath,hmiData.sazFilePath);
    if(r!=QPatternData::OK){
        ERRORLOG(QObject::tr("花型载入错误"));
        return Md::PatternError;
    }
    patternData.loadFile(Md::HAVEWRK|Md::HAVECNT);
    paramaData.setFile(hmiData.spaFilePath);
    QMdSplashScreen Splash(QPixmap("resource/image/matlab.png"));
    QMdSplashScreen *splash = &Splash;
    splash->show();
    QSend::SendResult commResult;
    /////poll valible rom////////////////
    commResult  = qSend.IsInBoot(NULL);
    QMdMessageBox box;
    if(commResult == QSend::InBootState){
        box.setText(QObject::tr("初始化"));
        box.setInformativeText(QObject::tr("主控板无固件，是否下载"));
        box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        box.setDefaultButton(QMessageBox::Yes);
        if(box.exec()==QMessageBox::Yes){
            QString filename = QFileDialog::getOpenFileName(splash,
                   QObject::tr("打开文件"),QString("./rom"),QObject::tr("固件文件 (*.bin *.min)"));
            if(filename!=""){
                QFile file(filename);
                commResult = qSend.SendBin(file,splash);
                if(commResult == QSend::BurnOK){
                    box.setParent(splash);
                    box.setText(QObject::tr("下载固件"));
                    box.setInformativeText(QObject::tr("下载成功"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.setIcon(QMessageBox::Information);
                    box.exec();
                }
                if(commResult == QSend::CommucationError){
                    box.setInformativeText(QObject::tr("通信出错"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.setIcon(QMessageBox::Warning);
                    box.exec();
                    return Md::MainBoardNoRom;
                }
                if(commResult == QSend::EraseError){
                    box.setInformativeText(QObject::tr("擦除错误"));
                    box.setStandardButtons(QMessageBox::Ok);
                    box.exec();
                    return Md::MainBoardNoRom;
                }
                if(commResult == QSend::BurnError){
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

    } else if(commResult==QSend::CommucationError){
        splash->showMessage(QObject::tr("通讯错误,花型文件未成功下载"),Qt::AlignBottom);
        return Md::CommError;
    }

    //////togle system/////////////////
    commResult = qSend.TogSysStat(0x04);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("通讯错误,花型文件未成功下载"),Qt::AlignBottom);
        return Md::CommError;
    }

    splash->showMessage(QObject::tr("正在下载参数"),Qt::AlignBottom);
    QFile wrkfile(hmiData.wrkFilePath);
    QFile spafile("./param/SYST_000.SPA");
    commResult = qSend.SendParama(wrkfile,spafile,0xff);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("通讯错误,参数未成功下载"),Qt::AlignBottom);
        return Md::CommError;
    }
    ///szkb///////////////////////////////////
    QFile sazfile(hmiData.sazFilePath);
    commResult = qSend.SendFile(sazfile,0,FALSE,NULL);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("下载SAZ文件，通讯错误"),Qt::AlignBottom);
        return Md::CommError;;
    }
    ///////下载cnt文件///////////////////////////////////
    QObject::connect(&qSend,SIGNAL(commPercent(int)),splash,SLOT(showCntMessage(int)));
    QFile cntfile(hmiData.cntFilePath);
    commResult = qSend.SendFile(cntfile,0,FALSE,NULL);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("下载CNT文件，通讯错误"),Qt::AlignBottom);
        return Md::CommError;;
    }
    QObject::disconnect(&qSend,SIGNAL(commPercent(int)),splash,SLOT(showCntMessage(int)));
    QObject::connect(&qSend,SIGNAL(commPercent(int)),splash,SLOT(showPatMessage(int)));
    QFile patfile(hmiData.patFilePath);
    commResult = qSend.SendFile(patfile,0,FALSE,NULL);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("下载PAT文件，通讯错误"),Qt::AlignBottom);
        return Md::CommError;
    }
    QObject::disconnect(&qSend,SIGNAL(commPercent(int)),splash,SLOT(showPatMessage(int)));

    //等待下位机响应/////////////////////////////////
    int i=0;
    for(i=0;i<3;i++) {
        commResult = qSend.TogSysStat(0x04);
        if(commResult==QSend::SendOk)
            break;
    }
    if(3==i){
        splash->showMessage(QObject::tr("下载花型后主机未成功响应"),Qt::AlignBottom);
         return Md::CommError;
    }

    //运行时参数设置//////////////////////////////////////////////
    commResult = qSend.sendParamaInRun(hmiData.clothSetCount,hmiData.clothFinishCount,
                                   hmiData.speedLimit,hmiData.stopPerOne,
                                   hmiData.alarmLimit,hmiData.dankouLock);
    if(commResult == QSend::CommucationError){
        splash->showMessage(QObject::tr("通讯错误,运行时参数设置失败"),Qt::AlignBottom);
        return Md::CommError;
    }
    if(hmiData.customerId!=0xffff){
        commResult = qSend.checkCustomerId(hmiData.customerId);
        if(commResult==QSend::CommucationError){
            splash->showMessage(QObject::tr("通讯错误,查询厂商ID失败"),Qt::AlignBottom);
            return Md::CommError;
        }else if(commResult == QSend::CustomerIdNotPass){
            splash->showMessage(QObject::tr("厂商ID不匹配"),Qt::AlignBottom);
           return Md::CustomerIdError;
        }
    }
    INFORMLOG(QObject::tr("开机初始化成功"));
    return Md::OK;
}

QString secondToString(int sec){
    unsigned  char second,minute,hour;
    second = sec%60;
    minute = sec/60%60;
    hour = sec/60/60;
    return QString("%1:%2:%3").arg(hour,2,10,QChar('0')).arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0'));
}


