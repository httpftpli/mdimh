#include "qhmidata.h"
#include "pattern.h"
#include "qmdmessagebox.h"
#include "data.h"
#include <QTimerEvent>
#include <QProcess>
#include"customerid.h"
#include "qparam.h"


QHMIData::QHMIData(QParam *param,QSend *send,QRcv *rcv,QObject *parent):
        QObject(parent),pparam(param),psend(send),prcv(rcv),speedlimit(0),
        alarmlimit(0),shazuiup(1),isruning(FALSE){
    connect(prcv,SIGNAL(DataChanged(unsigned short,QVariant)),
                     SLOT(On_DataChanged_FromCtrl(unsigned short,QVariant)));
    connect(&timer700ms,SIGNAL(timeout()),SLOT(on_700mstimeout()));
    connect(psend,SIGNAL(commTimerOut(unsigned char)),SLOT(on_CommTimerOut(unsigned char)));
    connect(pparam,SIGNAL(changed()),SLOT(onParamChanged()));
    timer700ms.setInterval(700);
    timer700ms.setSingleShot(FALSE);  
}


/*void QHMIData::setPatternFile(const QString &cntfilename,const QString &patfilename,const QString &sazfilename){
    patterndata.SetFile(cntfilename,patfilename);
    patterndata.LoadFile(TRUE,FALSE);
}
*/


void QHMIData::onParamChanged(){
#if DUAL_SYSTEM
    bool dankoulock = pparam->fechData(ItemHd_Xtcs,20);
    if(this->dankoulock!=dankoulock)
        emit sigDankouLock(dankoulock);
#endif

}

void QHMIData::on_CommTimerOut(unsigned char code){
    if((alarmque.isEmpty())||alarmque.last()!=0){
        alarmque.enqueue(0);
        if(alarmque.size()>20)
            alarmque.dequeue();
        if(commerrorcode.size()<20)
            commerrorcode.enqueue(code);
        emit alarm(0);
    }
}

int QHMIData::pollSysVersion(){
    return psend->pollSysVersion(mainboardversion,bagversion);
}

QString QHMIData::mainboardVersion(){
    return mainboardversion;
}

QString QHMIData::bagVersion(){
    return bagversion;
}

QString QHMIData::fetchAlarm(){
    int code=0;
    if(!alarmque.empty()){
        code = alarmque.dequeue();
        qDebug()<<code;
    }
    if(code!=0){
        qDebug()<<CWBJ_ErrorCode[code];
        return CWBJ_ErrorCode[code];
    }
    else{
        QString str = CWBJ_ErrorCode[0];
        return str.append(QString::number(commerrorcode.dequeue(),16));
    }
}

void QHMIData::clearAlarm(){
    psend->ClearError();
}

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
        //收到数据01表示停止，停车检查定时器(700ms)重新启动
        //运行定时器关，停车定时器开
        timer700ms.start();//reset timer;
        isruning = FALSE;
        break;
    case QHMIData::CWBJXX:{//报警信息
        int i = Val.toInt();
        if(i>79 )
            i = 79;
        if((alarmque.isEmpty())||(alarmque.last()!=i)){
            alarmque.enqueue(i);
            emit alarm(i);
            qDebug()<<"emit alarm";
        }
        if(alarmque.size()>20)
            alarmque.dequeue();
        break;
    }
    case QHMIData::JTXDZS:{//机头相对针数，显示在主界面上
        dataBuf[JTXDZS]= Val.toInt();
        emit hmi_jitouxiangduizhengshu(dataBuf[JTXDZS]);
        break;
    }
    case QHMIData::GLWC:{
            bool r;
            int val = Val.toInt();
            if(0xaa==val)
                r = TRUE;
            else if(0xbb==val)
                r = FALSE;
            emit xtGuilingFinish(r);
        }
    default:
        emit DataChanged_ToHMI(index,Val);
        break;

    }
}

void QHMIData::on_700mstimeout(){
    isruning = TRUE;
}

bool QHMIData::isRuning(){
    return isruning;
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
    setclothSetCount(clothsetcount,FALSE);
    setclothFinishCount(clothfinishcount,FALSE);
    int stopperone = sysset.value("run/stopperone").toBool();
    setStopPerOne(stopperone,FALSE);
    int linelock = sysset.value("run/linelock").toBool();
    setLineLock(linelock,FALSE);
    xtguilingorrun = sysset.value("run/resetrun").toBool();
    customerId = sysset.value("system/cd").toString().toInt(0);
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

int  QHMIData::errorCode(){
    return errorcode;
}

void QHMIData::finish(){
     saveSysCfgFile();
}

void QHMIData::start(){
     timeid1s = startTimer(1000);
}

void QHMIData::run(){
     timer700ms.start();
}

void QHMIData::timerEvent(QTimerEvent * event){
    int id = event->timerId();
    if(id==timeid1s){
        QDateTime datatime = QDateTime::currentDateTime();
        curruntTime = datatime.toTime_t();
        if(isruning)
            runTime++;
        else
            stopTime++;
        emit time1sOuted();
        if(!alarmque.isEmpty())
            emit alarm(alarmque.last());
    }
}

void QHMIData::saveSysCfgFile(){
    QSettings sysset(sysconfigfilename,QSettings::IniFormat,this);
    sysset.setValue("run/clothSetCount",QString::number(clothsetcount));
    sysset.setValue("run/clothFinishCount",QString::number(clothfinishcount));
    sysset.setValue("run/stopperone",QString::number(stopperone));
    sysset.setValue("run/linelock",QString::number(linelock));
    sysset.setValue("run/resetrun",QString::number(xtguilingorrun));
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

int QHMIData::sendParamaInRun(){
    return  psend->sendParamaInRun(clothsetcount,clothfinishcount,
                               speedlimit,stopperone,
                               alarmlimit,dankoulock);
}

int QHMIData::setSpeedLimit(bool limit,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(clothsetcount,clothfinishcount,limit,
                                 stopperone,alarmlimit,dankoulock);
    if(r==Md::Ok)
        speedlimit = limit;
    emit speedLimit(speedlimit);
    return r;
}

int QHMIData::setAlarmLimit(bool limit,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(clothsetcount,clothfinishcount,
                                   speedlimit,stopperone,limit,dankoulock);
    if(Md::Ok==r)
        alarmlimit = limit;
    emit alarmLimit(alarmlimit);
    return r;
}

int QHMIData::setShazuiUp(bool up,bool send){
    int r=0;
    if(send)
        r = psend->sazuiDownUp(up);
    if(Md::Ok==r)
        shazuiup = up;
    emit shazuiUp(shazuiup);
    return r;
}

int QHMIData::setStopPerOne(bool stop,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(clothsetcount,clothfinishcount,
                                   speedlimit,stop,alarmlimit,dankoulock);
    if(Md::Ok==r)
        stopperone = stop;
    emit stopPerOne(stopperone);
    return r;
}

int QHMIData::setLineLock(bool lock,bool send){
    int r=0;
    if(send)
        r = psend->lineLock(lock);
    if(Md::Ok==r)
        linelock = lock;
    emit lineLock(linelock);
    return r;
}

#if DUAL_SYSTEM
int QHMIData::setDankouLock(bool lock,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(clothsetcount,clothfinishcount,
                                   speedlimit,stopperone,alarmlimit,lock);    
    if(Md::Ok==r)
        dankoulock = lock;
    pparam->setDankouLock(dankoulock);
    emit sigDankouLock(dankoulock);
    return r;
}

int QHMIData::toggleDankouLock(){
    return setDankouLock(!dankoulock,TRUE);
}

bool QHMIData::dankouLock(){
    return this->dankoulock;
}

#endif

int QHMIData::setRunOrGuiling(bool run){
    unsigned code;    
    switch(customerId){
    case 31:
        code = run?0x0b:0x0a;
        break;
    default:
        code = run?0x01:0x03;
        break;
    }
   if(run)
       emit xtGuilingFinish(TRUE);
   return psend->TogSysStat(code);
}

int QHMIData::setRunOrGuiling(){
    return setRunOrGuiling(xtguilingorrun);
}

int QHMIData::setclothSetCount(unsigned short val,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(val,clothfinishcount,
                                   speedlimit,stopperone,alarmlimit,dankoulock);
    if(Md::Ok==r)
        clothsetcount = val;
    emit clothSetCountChanged(clothsetcount);
    return r;
}

int QHMIData::setclothFinishCount(unsigned short val,bool send){
    int r=0;
    if(send)
        r = psend->sendParamaInRun(clothsetcount,val,
                                   speedlimit,stopperone,alarmlimit,dankoulock);
    if(Md::Ok==r)
        clothfinishcount = val;
    emit clothFinishCountChanged(clothfinishcount);
    return r;
}

int QHMIData::clothFinishCount(){
    return clothfinishcount;
}

int QHMIData::clothSetCount(){
    return clothsetcount;
}

void QHMIData::on_patternChange(const QString &patternname,const QString &cntfilepath, const QString &patfilepath,
                              const QString &wrkfilepath , const QString &sazfilepath){
    partternName = patternname;
    cntFilePath = cntfilepath;
    patFilePath = patfilepath;
    wrkFilePath = wrkfilepath;
    sazFilePath = sazfilepath;
    setclothFinishCount(0,TRUE);
    sendParamaInRun();
}

void QHMIData::on_clothFinish( ){
    runTimeHistory = runTime;
    stopTimeHistory = stopTime;
    runTime = 0;
    stopTime = 0;
}
