#include "qhmidata.h"
#include "pattern.h"
#include "qmdmessagebox.h"
#include "data.h"
#include <QTimerEvent>
#include <QProcess>
#include "qparam.h"
#include"config.h"
#include<QSettings>
#include"communicat.h"


QHMIData::QHMIData(QParam *param,QComm *qcomm,QObject *parent):
        QObject(parent),pparam(param),pcomm(qcomm),speedlimit(0),
        alarmlimit(0),shazuiup(1),isruning(FALSE),isinitfinish(FALSE){
    connect(pcomm,SIGNAL(DataChangedFromCtrl(unsigned short,QVariant)),
                     SLOT(On_DataChanged_FromCtrl(unsigned short,QVariant)));
    connect(&timer700ms,SIGNAL(timeout()),SLOT(on_700mstimeout()));

    connect(pcomm,SIGNAL(commTimerOut(unsigned char)),SLOT(on_CommTimerOut(unsigned char)));
    timer700ms.setInterval(1000);
    timer700ms.setSingleShot(FALSE);
    sys_wrkFilePath = "defalt.wrk";
}


/*void QHMIData::setPatternFile(const QString &cntfilename,const QString &patfilename,const QString &sazfilename){
    patterndata.SetFile(cntfilename,patfilename);
    patterndata.LoadFile(TRUE,FALSE);
}
*/

void QHMIData::onParamChanged(){
#if DUAL_SYSTEM
    bool dankoulock = pparam->fechData(QParam::SpaItemHd_Xtcs,20);
    if(this->dankoulock!=dankoulock)
        emit sigDankouLock(dankoulock);
#endif

}

void QHMIData::on_CommTimerOut(unsigned char code){
    if((alarmque.isEmpty())||alarmque.last()!=59){
        alarmque.enqueue(59);
        if(alarmque.size()>20)
            alarmque.dequeue();
        if(commerrorcode.size()<20)
            commerrorcode.enqueue(code);
        emit alarm(59);
    }
}

int QHMIData::pollSysVersion(){
    return pcomm->pollSysVersion(mainboardversion,bagversion);
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
    }
    if(code!=59){
        return CWBJ_ErrorCode[code];
    }
    else{
        QString str = CWBJ_ErrorCode[59];
        return str.append(QString::number(commerrorcode.dequeue(),16));
    }
}

void QHMIData::clearAlarm(){
    pcomm->ClearError();
}


void QHMIData::On_DataChanged_FromCtrl(unsigned short index,const QVariant &Val){
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
        stream >> cntnumber;  //cntnumber 以1开始
        if(cntnumber==0){
            cntnumber=0x01;
        }
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
           runTimeHistory = runTime;
           stopTimeHistory = stopTime;
           runTime = 0;
           stopTime = 0;
           emit clothFinishCountChanged(finishcount);
        }

     break;
    }

    case QHMIData::DQZDJZT:  //当前主电机状态
        //收到数据01表示停止，停车检查定时器(700ms)重新启动
        //运行定时器关，停车定时器开
        timer700ms.start();//reset timer;
        if(isruning != FALSE){
            isruning = FALSE;
            emit runing(FALSE);
        }
        break;
    case QHMIData::CWBJXX:{//报警信息
        int i = Val.toInt();
        if(i==0)
            break;
        if(i>79 )
            i = 79;
        if((alarmque.isEmpty())||(alarmque.last()!=i)){
            alarmque.enqueue(i);
            emit alarm(i);
        }
        if(alarmque.size()>20)
            alarmque.dequeue();
        break;
    }
    case QHMIData::JTXDZS:{//机头相对针数，显示在主界面上
        dataBuf[JTXDZS]=(short)(Val.toInt());
        emit hmi_jitouxiangduizhengshu(dataBuf[JTXDZS]);
        break;
    }
    case QHMIData::GLWC:{
        int val = Val.toInt();
        if(0xaa==val){
            emit xtRunOrGuiling(TRUE);
            xtrunorguiling = TRUE;
        }
        else if(0xbb==val)
            emit xtGuilingError();
        break;
    }
    case QHMIData::XTDD:{
        int val = Val.toInt();
        if(0x55==val)
            emit powerDown();
        break;
    }
    default:
        emit DataChanged_ToHMI(index,Val);
        break;
    }
}

void QHMIData::on_700mstimeout(){
    isruning = TRUE;
    emit runing(TRUE);
}

bool QHMIData::isRuning(){
    return isruning;
}


int QHMIData::TogSysStat(SysStat stat){
    unsigned char val=0;
    switch(stat){
    case SysIdle:
        val =0x00;
        break;
    case SysRun:
        val = 72;//0x01;    72 73 下面的机器  0b 0a;
        break;
    case SysTest:
        val = 0x02;
        break;
    case SysReset:
        val =73; // 0x03;
        break;
    case SysInParam:
        val = 0x04;
        break;
    default:
        break;
    }
    return pcomm->TogSysStat(val);
}

bool QHMIData::mainboardRomAvailable()
{
    int commResult  = pcomm->IsInBoot();
    if(commResult & Md::InBootState)
        return FALSE;
    else
        return TRUE;
}

void QHMIData::loadParam(const QString &inifilepath){
    QSettings sysset(inifilepath,QSettings::IniFormat,this);
    sysconfigfilename = inifilepath;
    sysset.beginGroup("run");
    int clothsetcount = sysset.value("clothSetCount").toInt();
    int clothfinishcount = sysset.value("clothFinishCount").toInt();
    setclothSetCount(clothsetcount,FALSE);
    setclothFinishCount(clothfinishcount,FALSE);
    int stopperone = sysset.value("stopperone").toBool();
    setStopPerOne(stopperone,FALSE);
    int linelock = sysset.value("linelock").toBool();
    setLineLock(linelock,FALSE);
    xtrunorguiling = sysset.value("resetrun").toBool();
    stopTime = sysset.value("stoptime").toLongLong();
    runTime = sysset.value("runtime").toLongLong();
    sysset.endGroup();

    sysset.beginGroup("history");
    stopTimeHistory = sysset.value("stoptime").toLongLong();
    runTimeHistory = sysset.value("runtime").toLongLong();
    sysset.endGroup();

    sysset.beginGroup("system");
    udiskDirPath = sysset.value("udiskFilePath").toString();
    sysLogFilePath = sysset.value("syslogfile").toString();
    customerId = sysset.value("cd").toString().toInt(0);
    sysset.endGroup();

    patternPath = sysset.value("pattern/path").toString();

    spaFilePath = sysset.value("param/spafile").toString();

}

int  QHMIData::errorCode(){
    return errorcode;
}

void QHMIData::finish(){
     saveSysCfgFile();
}

void QHMIData::startTimer1s(){
     timeid1s = startTimer(1000);
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

    sysset.beginGroup("run");
    sysset.setValue("clothFinishCount",QString::number(clothfinishcount));
    sysset.setValue("clothSetCount",QString::number(clothsetcount));
    sysset.setValue("linelock",QString::number(linelock));
    sysset.setValue("resetrun",QString::number(xtrunorguiling));
    sysset.setValue("stopperone",QString::number(stopperone));
    sysset.setValue("runtime",QString::number(runTime));
    sysset.setValue("stoptime",QString::number(stopTime));
    sysset.endGroup();

    sysset.setValue("pattern/path",patternPath);

    sysset.beginGroup("history");
    sysset.setValue("runtime",QString::number(runTimeHistory));
    sysset.setValue("stoptime",QString::number(stopTimeHistory));
    sysset.endGroup();

    sysset.sync();
    system("cat sysconfig.conf >> /dev/null && sync");  //NOTICE: qsetting will lost filedata ,so call shell command
}

int QHMIData::sendParamaInRun(){//dankoulock在机器系统参数中
    return  pcomm->sendParamaInRun(clothsetcount,clothfinishcount,
                               speedlimit,stopperone,
                               alarmlimit,dankoulock);
}

int QHMIData::setSpeedLimit(bool limit,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(clothsetcount,clothfinishcount,limit,
                                 stopperone,alarmlimit,dankoulock);
    if(r==Md::Ok)
        speedlimit = limit;
    emit speedLimit(speedlimit);
    return r;
}

int QHMIData::setAlarmLimit(bool limit,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(clothsetcount,clothfinishcount,
                                   speedlimit,stopperone,limit,dankoulock);
    if(Md::Ok==r)
        alarmlimit = limit;
    emit alarmLimit(alarmlimit);
    return r;
}

int QHMIData::setShazuiUp(bool up,bool send){
    int r=0;
    if(send)
        r = pcomm->sazuiDownUp(up);
    if(Md::Ok==r)
        shazuiup = up;
    emit shazuiUp(shazuiup);
    return r;
}

int QHMIData::setStopPerOne(bool stop,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(clothsetcount,clothfinishcount,
                                   speedlimit,stop,alarmlimit,dankoulock);
    if(Md::Ok==r)
        stopperone = stop;
    emit stopPerOne(stopperone);
    return r;
}

int QHMIData::setLineLock(bool lock,bool send){
    int r=0;
    if(send)
        r = pcomm->lineLock(lock);
    if(Md::Ok==r)
        linelock = lock;
    emit lineLock(linelock);
    return r;
}

#if DUAL_SYSTEM
int QHMIData::setDankouLock(bool lock,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(clothsetcount,clothfinishcount,
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

bool QHMIData::xtGuiling(){
    if(TogSysStat(SysReset)==Md::Ok){
        emit xtRunOrGuiling(FALSE);
        xtrunorguiling = FALSE;
        return TRUE;
    }
    return FALSE;
}

bool QHMIData::xtRun()
{
    if(TogSysStat(SysRun)==Md::Ok){
        emit xtRunOrGuiling(TRUE);
        xtrunorguiling = TRUE;
        return TRUE;
    }
    return FALSE;
}

void QHMIData::start(){
    timer700ms.start();
    isinitfinish = TRUE;
    if(xtrunorguiling)
        xtRun();
    else
        xtGuiling();
}

int QHMIData::setclothSetCount(unsigned short val,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(val,clothfinishcount,
                                   speedlimit,stopperone,alarmlimit,dankoulock);
    if(Md::Ok==r)
        clothsetcount = val;
    emit clothSetCountChanged(clothsetcount);
    return r;
}

int QHMIData::setclothFinishCount(unsigned short val,bool send){
    int r=0;
    if(send)
        r = pcomm->sendParamaInRun(clothsetcount,val,
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

void QHMIData::on_patternChange(const QString &dirpath,const QString &name){
    partternName = name;
    QString path = dirpath+"/"+name;
    if(patternPath!=path){
        patternPath = path;
        setclothFinishCount(0,TRUE);
    }
}
