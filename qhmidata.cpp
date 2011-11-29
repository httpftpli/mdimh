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
        if(tatalloop!=dataBuf[RUN_LOOP_TOTAL]){
            dataBuf[RUN_LOOP_TOTAL] = tatalloop;
            emit hmi_loopTatal(tatalloop);
        }
        stream >> cntnumber;
        cntnumber--;                            //上发的行号以一为起点，所以减去1
        dataBuf[RUN_CNT_NUMBER] = cntnumber;
        emit hmi_cntNumber(cntnumber);
        stream >> leftloop;
        if(leftloop!=dataBuf[RUN_LOOP_LEFT]){
           dataBuf[RUN_LOOP_LEFT] = leftloop;
           emit hmi_loopleft(leftloop);
        }
         stream >> derection;
        if(derection!=dataBuf[RUN_DERECTION]){
           dataBuf[RUN_DERECTION] = derection;
           emit hmi_direction(derection?Md::LEFT:Md::RIGHT);
        }
        stream >> loopstart;
        if(loopstart!=dataBuf[RUN_LOOP_STARTNO]){
           dataBuf[RUN_LOOP_STARTNO] = loopstart;
           emit hmi_loopStart(loopstart);
        }
        stream >> loopend;
        if(loopend!=dataBuf[RUN_LOOP_ENDNO]){
           dataBuf[RUN_LOOP_ENDNO] = loopend;
           emit hmi_loopend(loopend);
        }
        stream >> stat;
        if(stat!=dataBuf[RUN_JUMPORNOT]){
           dataBuf[RUN_JUMPORNOT] = stat; //0:正常运行，1:跳行
        }
        stream >> finishcount;
        if(finishcount!=dataBuf[RUN_COUNT_FINISH]){
           dataBuf[RUN_COUNT_FINISH] = finishcount;
           emit hmi_finishCount(finishcount);
        }

     break;
    }

    case QHMIData::DQZDJZT:  //当前主电机状态
        //收到数据01表示停止，停车检查定时器(600ms)重新启动
        //运行定时器关，停车定时器开
        //JTZTtimer.start();
        dataBuf[JTDZZT]=0; //标记为停车
        break;
    case QHMIData::CWBJXX:{//报警信息
        unsigned int i = Val.toInt();
        if(i>79)
            i = 79;
        emit DataChanged_ToHMI(index,CWBJ_ErrorCode[i]);
        break;
    }
    case QHMIData::JTXDZS:{//机头相对针数，显示在主界面上
        dataBuf[JTXDZS]= Val.toInt();
        emit hmi_jitouxiangduizhengshu(dataBuf[JTXDZS]);
        break;
    }
    case QHMIData::GLWC:{
            emit xtGuiling(Val.toInt());
        }
    default:
        emit DataChanged_ToHMI(index,Val);
        break;

    }
}

void QHMIData::setJitouTingChi(){  //停车检查定时器溢出
    dataBuf[JTDZZT]=1; //标记为运行
}

bool QHMIData::isRun(){
    return (bool)dataBuf[JTDZZT];
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

Md::Result  QHMIData::errorCode(){
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

int QHMIData::setSpeedLimit(bool limit){
    return psend->sendParamaInRun(clothSetCount,clothFinishCount,limit,
                           stopPerOne,alarmLimit,dankouLock);
}

void QHMIData::setclothSetCount(unsigned short val){
    if(val!=clothSetCount){
        clothSetCount = val;
        emit clothSetCountChanged(val);
    }
}

void QHMIData::setclothFinishCount(unsigned short val){
        clothFinishCount = val;
        emit clothFinishCountChanged(val);
}

void QHMIData::downloadParam(){
    psend->sendParamaInRun(clothSetCount,clothFinishCount,
                           speedLimit,stopPerOne,
                           alarmLimit,dankouLock);
}

void QHMIData::on_patternChange(const QString &patternname,const QString &cntfilepath, const QString &patfilepath,
                              const QString &wrkfilepath , const QString &sazfilepath){
    partternName = patternname;
    cntFilePath = cntfilepath;
    patFilePath = patfilepath;
    wrkFilePath = wrkfilepath;
    sazFilePath = sazfilepath;
    setclothFinishCount(0);
    psend->sendParamaInRun(clothSetCount,clothFinishCount,
                           speedLimit,stopPerOne,alarmLimit,
                           dankouLock);
}

void QHMIData::on_clothFinish( ){
    runTimeHistory = runTime;
    stopTimeHistory = stopTime;
    runTime = 0;
    stopTime = 0;
}
