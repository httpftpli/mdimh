#include "qhmidata.h"
#include "pattern.h"
#include "qmdmessagebox.h"
#include "globaldata.h"
#include "data.h"
#include <QTimerEvent>
#include <QProcess>


QHMIData::QHMIData(QSend *send,QObject *parent):QObject(parent),psend(send){

}


/*void QHMIData::setPatternFile(const QString &cntfilename,const QString &patfilename,const QString &sazfilename){
    patterndata.SetFile(cntfilename,patfilename);
    patterndata.LoadFile(TRUE,FALSE);
}
*/

void QHMIData::On_DataChanged_FromHMI(unsigned short index,QVariant Val){
    unsigned char temp_uchar;
    unsigned short temp_short;
    unsigned long temp_long;
    switch(index){
    case ZSD :
        temp_uchar = Val.toUInt();
        qSend.LedTest(temp_uchar);
        break;
    case YSS:
        temp_uchar = Val.toUInt();
        qSend.RightMuslin(temp_uchar);
        break;
    case ZSS:
        temp_uchar = Val.toUInt();
        qSend.LeftMuslin( temp_uchar);
        break;
    case FZLL:
        temp_short =Val.toUInt();
        qSend.AssistRollTest((unsigned char)(temp_short>>8),(unsigned char)temp_short);
        break;
    case BJLL:
        temp_long = Val.toUInt();
        qSend.StepRollTest((unsigned char)(temp_long>>24),(unsigned char)(temp_long>>16),(unsigned char)(temp_long>>8));
        break;
    case JTDJ:
        temp_long = Val.toUInt();
        qSend.MainMotorTest((unsigned char)(temp_long>>8),(unsigned char)temp_long);
        break;
    }
}

void QHMIData::On_DataChanged_FromCtrl(unsigned short index,QVariant Val){
    switch(index){
    case QHMIData::YXHXCL:{     //运行时换向
        QByteArray bytearray = Val.toByteArray();
        QDataStream stream(bytearray);
        stream.setByteOrder(QDataStream::BigEndian);
        unsigned short tatalloop,leftloop,loopstart,loopend,finishcount,cntnumber;
        unsigned char derection,stat;

        stream >> tatalloop;
        if(tatalloop!=dataBuf[RUN_LOOP_TOTAL].Val){
            dataBuf[RUN_LOOP_TOTAL].Val = tatalloop;
            emit hmi_loopTatal(tatalloop);
        }
        stream >> cntnumber;
        cntnumber--;                            //上发的行号以一为起点，所以减去1
        dataBuf[RUN_CNT_NUMBER].Val = cntnumber;
        emit hmi_cntNumber(cntnumber);
        stream >> leftloop;
        if(leftloop!=dataBuf[RUN_LOOP_LEFT].Val){
           dataBuf[RUN_LOOP_LEFT].Val = leftloop;
           emit hmi_loopleft(leftloop);
        }
         stream >> derection;
        if(derection!=dataBuf[RUN_DERECTION].Val){
           dataBuf[RUN_DERECTION].Val = derection;
           emit hmi_direction(derection?Md::LEFT:Md::RIGHT);
        }
        stream >> loopstart;
        if(loopstart!=dataBuf[RUN_LOOP_STARTNO].Val){
           dataBuf[RUN_LOOP_STARTNO].Val = loopstart;
           emit hmi_loopStart(loopstart);
        }
        stream >> loopend;
        if(loopend!=dataBuf[RUN_LOOP_ENDNO].Val){
           dataBuf[RUN_LOOP_ENDNO].Val = loopend;
           emit hmi_loopend(loopend);
        }
        stream >> stat;
        if(stat!=dataBuf[RUN_JUMPORNOT].Val){
           dataBuf[RUN_JUMPORNOT].Val = stat; //0:正常运行，1:跳行
        }
        stream >> finishcount;
        if(finishcount!=dataBuf[RUN_COUNT_FINISH].Val){
           dataBuf[RUN_COUNT_FINISH].Val = finishcount;
           emit hmi_finishCount(finishcount);
        }

     break;
    }

    case QHMIData::DQZDJZT:  //当前主电机状态
        //收到数据01表示停止，停车检查定时器(600ms)重新启动
        //运行定时器关，停车定时器开
        //JTZTtimer.start();
        dataBuf[JTDZZT].Val=0; //标记为停车
        break;
    case QHMIData::CWBJXX:{//报警信息
        unsigned int i = Val.toInt();
        if(i>79)
            i = 79;
        emit DataChanged_ToHMI(index,CWBJ_ErrorCode[i]);
        break;
    }
    case QHMIData::JTXDZS:{//机头相对针数，显示在主界面上
        dataBuf[JTXDZS].Val= Val.toInt();
        emit hmi_jitouxiangduizhengshu(dataBuf[JTXDZS].Val);
        break;
    }
    case QHMIData::GLWC:{
            emit hmi_xtguilingwc(Val.toInt());
        }
    default:
        emit DataChanged_ToHMI(index,Val);
        break;

    }
}

void QHMIData::setJitouTingChi(){  //停车检查定时器溢出
    dataBuf[JTDZZT].Val=1; //标记为运行
}

bool QHMIData::isRun(){
    return (bool)dataBuf[JTDZZT].Val;
}

void QHMIData::RequireData(unsigned short index){
    switch(index){
    case QHMIData::WBSR:
        psend->readDI();
    }
}

void QHMIData::loadParam(const QString &inifilepath){
    QSettings sysset(inifilepath,QSettings::IniFormat,this);
    sysconfigfilename = inifilepath;
    int clothsetcount = sysset.value("run/clothSetCount").toInt();
    int clothfinishcount = sysset.value("run/clothFinishCount").toInt();
    setclothSetCount(clothsetcount);
    setclothFinishCount(clothfinishcount);
    speedLimit = sysset.value("run/speedLimit").toBool();
    stopPerOne = sysset.value("run/stopPerOne").toBool();
    alarmLimit = sysset.value("run/alarmLimit").toBool();
    dankouLock = sysset.value("run/dankouLock").toBool();
    sysStat = sysset.value("run/sysStat").toInt();
    customerId = sysset.value("system/CD").toString().toInt(0);
    patternVailable = sysset.value("pattern/vailable").toBool();
    patFilePath = sysset.value("pattern/patFileName").toString();
    cntFilePath = sysset.value("pattern/cntFileName").toString();
    wrkFilePath = sysset.value("pattern/wrkFileName").toString();
    sys_wrkFilePath = sysset.value("param/wrkFileName").toString();
    sazFilePath = sysset.value("pattern/sazFileName").toString();
    loopFilePath = sysset.value("pattern/loopFileName").toString();
    spaFilePath = sysset.value("param/spaFileName").toString();
    udiskDirPath = sysset.value("system/udiskFilePath").toString();
    sysLogFilePath = sysset.value("system/syslogfile").toString();
    stopTimeHistory = sysset.value("history/stoptime").toLongLong();
    runTimeHistory = sysset.value("history/runtime").toLongLong();
    stopTime = sysset.value("run/stoptime").toLongLong();
    runTime = sysset.value("run/runtime").toLongLong();
}

Md::InitResult  QHMIData::errorCode(){
    return errorcode;
}

void QHMIData::finish(){
     saveSysCfgFile();
}

void QHMIData::start(){
    //clothSetCountChange(this->clothSetCount);
    timeid1s = startTimer(1000);
}

void QHMIData::timerEvent(QTimerEvent * event){
    int id = event->timerId();
    if(id==timeid1s){
        QDateTime datatime = QDateTime::currentDateTime();
        curruntTime = datatime.toTime_t();
        if(hmiData.isRun())
            runTime++;
        else
            stopTime++;
        emit time1sOuted();
    }
}

void QHMIData::saveSysCfgFile(){
    QSettings sysset(sysconfigfilename,QSettings::IniFormat,this);
    sysset.setValue("run/clothSetCount",QString::number(clothSetCount));
    sysset.setValue("run/clothFinishCount",QString::number(clothFinishCount));
    sysset.setValue("run/speedLimit",QString::number(speedLimit));
    sysset.setValue("run/stopPerOne",QString::number(stopPerOne));
    sysset.setValue("run/alarmLimit",QString::number(alarmLimit));
    sysset.setValue("run/dankouLock",QString::number(dankouLock));
    sysset.setValue("run/sysStat",QString::number(sysStat));
    sysset.setValue("pattern/patFileName",patFilePath);
    sysset.setValue("pattern/cntFileName",cntFilePath);
    sysset.setValue("pattern/wrkFileName",wrkFilePath);
    sysset.setValue("pattern/sazFileName",sazFilePath);
    sysset.setValue("history/stoptime",stopTimeHistory);
    sysset.setValue("history/runtime",runTimeHistory);
    sysset.sync();
    int r = QProcess::execute("sh quit.sh");
    qDebug()<<"QProcess::executer"<<r;
}


void QHMIData::setclothSetCount(unsigned short val){
    if(val!=clothSetCount){
        clothSetCount = val;
        emit clothSetCountChanged(val);
    }
}

void QHMIData::setclothFinishCount(unsigned short val){
    if(val!=clothFinishCount){
        clothFinishCount = val;
        emit clothFinishCountChanged(val);
    }
}

void QHMIData::downloadParam(){
    psend->sendParamaInRun(clothSetCount,clothFinishCount,
                           speedLimit,stopPerOne,
                           alarmLimit,dankouLock);
}

void QHMIData::on_patternChange(const QString &patternname,const QString &cntfilepath, const QString &patfilepath,
                              const QString &wrkfilepath , const QString &sazfilepath){
    Q_UNUSED(patternname);
    cntFilePath = cntfilepath;
    patFilePath = patfilepath;
    wrkFilePath = wrkfilepath;
    sazFilePath = sazfilepath;
}

void QHMIData::on_clothFinish( ){
    runTimeHistory = runTime;
    stopTimeHistory = stopTime;
    runTime = 0;
    stopTime = 0;
}
