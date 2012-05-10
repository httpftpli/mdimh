#ifndef QHMIDATA_H
#define QHMIDATA_H
#include "namespace.h"
#include <QQueue>
#include"config.h"
#include<QTimer>
#include<QVariant>

class QParam;
class QComm;

const unsigned char  INPUT_MAP[32][2] = {  //显示序号与开关量的映射表(数组下标,位)
    {3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7},
    {2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7},
    {1,0}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7},
    {0,4}, {0,1}, {0,2}, {0,0}, {0,3}, {0,5}, {0,6}, {0,7}
};

const QString CWBJ_ErrorCode[60]={
QObject::trUtf8(" "),             //  communication error                        //[0]
QObject::trUtf8("主电机没有准备信号!"),           // "Moto is not ready!",
QObject::trUtf8("摇床电机没有准备信号!"),          //"Move is not ready!",
QObject::trUtf8("机头左限位报警!"),                 // "Left limite alarm!",
QObject::trUtf8("机头右限位报警!"),             // "Right limite alarm!",
QObject::trUtf8("摇床限位报警!"),              //"Move bed error!",
QObject::trUtf8("左收线报警!"),                //"Left take up error!",
QObject::trUtf8("右收线报警!"),               //"Right take up error!",
QObject::trUtf8("断纱报警!"),                //"Break yarn error!",
QObject::trUtf8("储纱器断纱报警!"),             //"Store yarn break error!",
QObject::trUtf8("大纱结报警!"),               //"Big knot yarn error!",                        //[10]
QObject::trUtf8("卷布报警!"),                    //"Roll error!",
QObject::trUtf8("落布报警!"),                //"Drop cloth error!",
QObject::trUtf8("付罗拉限位报警!"),             //"Secont roll error!",
QObject::trUtf8("前床撞针报警!"),              //"Front break knit error!",
QObject::trUtf8("后备电源异常!"),              //"Power down error!",
QObject::trUtf8("机头12V电源失效!"),           //"Head 12V fuse error!",                    //[16]
QObject::trUtf8("机头主板+24V保险丝F1失效!"),     //"Head +24V fuse F1 error!",        //[17]
QObject::trUtf8("1号选针器板+24V保险丝F3失效!"),//"Select 1 +24V fuse F3 error!",
QObject::trUtf8("探针报警!"),                //"Probe error!",
QObject::trUtf8("7号密度马达故障!"),        //"No.7 density motor error!",                //[20]
QObject::trUtf8("8号密度马达故障!"),        //"No.8 density motor error!",
QObject::trUtf8("3号密度马达故障!"),        //"No.3 density motor error!",
QObject::trUtf8("4号密度马达故障!"),        //"No.4 density motor error!",
QObject::trUtf8("5号密度马达故障!"),        //"No.5 density motor error!",
QObject::trUtf8("6号密度马达故障!"),        //"No.6 density motor error!",
QObject::trUtf8("1号密度马达故障!"),        //"No.1 density motor error!",
QObject::trUtf8("2号密度马达故障!"),        //"No.2 density motor error!",
QObject::trUtf8("后生克电机故障!"),         //"Back sinker error!",
QObject::trUtf8("前生克电机故障!"),         //"Front sinker error!",
QObject::trUtf8("摇床零位异常!"),          //"Bed zero error!",                               //[30]
QObject::trUtf8("紧急制动锁定!"),          //"Stop break locking!",                            // 31
QObject::trUtf8("编织花样超出针板范围!"),      //"Over neddle plate!",
QObject::trUtf8("2号选针器板+24V保险丝F2失效!"),//"Select 2 +24V fuse F2 error!",     // 33
QObject::trUtf8("机头电机板-24V保险丝F6失效!"),//"Motor -24V fuse F6 error!",
QObject::trUtf8("1号选针器板-24V保险丝F4失效!"),//"Select 1 -24V fuse F4 error!",
QObject::trUtf8("2号选针器板-24V保险丝F5失效!"),//"Select 2 -24V fuse F5 error!",
QObject::trUtf8("后床撞针报警!"),              //"Back break knit error!",
QObject::trUtf8("摇床位置错误!"),              //"Move bed place error!",                         //38
QObject::trUtf8("主板12V保险丝失效!"),          //"Main board 12V fuse error!",                 //[39]
QObject::trUtf8("内存无花型!"),               //"No file in mem!",                                 //[40]
QObject::trUtf8("错误3"),                    //"Rev error3!",
QObject::trUtf8("设定件数完成!"),              //"Set pieces done!",
QObject::trUtf8("单件停车!"),                   //"One finish stop!",
QObject::trUtf8("机头背包通讯异常!"),            //"Head communications error!",
QObject::trUtf8("后备电源正在掉电!"),            //"Power downing!",
QObject::trUtf8("移床采集CPLD 输出 异常"),       //"System reservation error!",
QObject::trUtf8("主板写入花型数据异常"),          //"System reservation error!",            //47
"",
"",
"",              // 50
"",
"",
"",
"",
"",
"",
"",
"",
QObject::trUtf8("通讯错误，通讯码 ")             //  communication error                        //[59]
};


class QHMIData: public QObject{
   Q_OBJECT

public:
    enum DATATAG{
                 XTDD= 0 ,          //system power down
                 GLWC,        //系统归零完成
                 ZXHJSQ,


                 RUN_LOOP_TOTAL,
                 RUN_LOOP_LEFT,
                 RUN_LOOP_STARTNO,
                 RUN_LOOP_ENDNO,
                 RUN_CNT_NUMBER,
                 RUN_DERECTION,
                 RUN_JUMPORNOT,
                 RUN_COUNT_FINISH,
                 RUN_DUMU,
                 RUN_YAOCHUANG,
                 RUN_SPEED,
                 RUN_YAJIAO,
                 RUN_TINGCHE,
                 RUN_MAINLUOLA,
                 RUN_FUZULUOLA,
                 RUN_SONGSHA,
                 RUN_SHAZUITF,
                 YXHXCL,
                 JTXDZS,         //机头相对针数
                 JTBMQZ,         //机头编码器值
                 JTJDZS,         //机头绝对针数


                 QHXTZT_ACK,     //切换系统状态应答0X40
                 CWBJXX,        //错误报警信息0X43

                 YXTHCL,      //运行跳行处理OX47

                 CXBJZSZJTH,   //程序编辑中的沙嘴交/替换0x49
                 CXBJZ_CNT_GB,  //程序编辑中的CNT数据块改变0x4a
                 DQZDJZT,       //当前主电机状态
                 GUARDIANS        //枚举类型最后一个，仅用于边界守护

             };
    enum SysStat{SysIdle,SysRun,SysTest,SysReset,SysInParam};

    int dataBuf[GUARDIANS];
    QHMIData(QParam *param,QComm *send,QObject *parent = 0);

    bool patternVailable;
    unsigned short customerId;
    int stopTimeHistory;
    int runTimeHistory;
    int curruntTime;
    int stopTime;
    int runTime;
    QString patternPath;
    QString sys_wrkFilePath;
    QString spaFilePath;
    QString sazFilePath;
    QString loopFilePath;
    QString sysLogFilePath;
    QString udiskDirPath;
    QString sysconfigfilename;
    QString partternName;
    int totalBzTime;
    int currentBzTime;
    int totalStopTime;
    int currentStopTime;
    int commResult;
    QComm *pcomm;
    bool isInBoot;
    bool isRuning();
    void loadParam(const QString &inifilepath);
    void finish();
    int setclothSetCount(unsigned short val,bool send);
    int setclothFinishCount(unsigned short val,bool send);
    int clothFinishCount();
    int clothSetCount();
    int setSpeedLimit(bool limit,bool send);
    int setAlarmLimit(bool limit,bool send);
    int setShazuiUp(bool up,bool send);
    int setStopPerOne(bool stop,bool send);
    int setLineLock(bool lock,bool send);
#if DUAL_SYSTEM
    int setDankouLock(bool lock,bool send);
    bool dankouLock();
    int toggleDankouLock();
#endif
    bool xtGuiling();
    bool xtRun();
    int sendParamaInRun();
    int pollSysVersion();
    QString mainboardVersion();
    QString bagVersion();
    void saveSysCfgFile();
    void startTimer1s();
    void start();
    QString fetchAlarm();
    void clearAlarm();
    Md::Result errorcode;
    int errorCode();
    int TogSysStat(SysStat stat);
    bool mainboardRomAvailable();

public slots:
    void On_DataChanged_FromCtrl(unsigned short index,const QVariant &Val);
    void on_patternChange(const QString &dirpath,const QString &name);
    void on_clothFinish();
public slots:
    void onParamChanged();

signals:
    void DataChanged_ToCtrl(unsigned short index,QVariant val);
    void DataChanged_ToHMI(unsigned short index,QVariant val);
    void hmi_finishCount(int val);
    void hmi_loopend(int val);
    void hmi_loopStart(int val);
    void hmi_direction(Md::DIRECTION);
    void hmi_loopleft(int val);
    void hmi_cntNumber(unsigned short val);
    void hmi_loopTatal(int val);
    void hmi_jitouxiangduizhengshu(int val);
    void xtRunOrGuiling(bool val);
    void powerDown();
    void xtGuilingError();
    void Sig_tatalPatLine(int line);
    void clothSetCountChanged(int val);
    void clothFinishCountChanged(int val);
    void time1sOuted();
    void speedLimit(bool);
    void alarmLimit(bool);
    void shazuiUp(bool);
    void stopPerOne(bool);
    void lineLock(bool);
    void runing(bool);
#if DUAL_SYSTEM
    void sigDankouLock(bool);
#endif
    void alarm(unsigned char code);
////////////////////////////////
protected:
    virtual void timerEvent(QTimerEvent * event);//1s 600ms,
private  slots:
    void on_700mstimeout();
    void on_CommTimerOut(unsigned char);
private:
    int timeid1s;
    bool isinitfinish;
    QTimer timer700ms;
    QParam *pparam;
    bool speedlimit;
    bool stopperone;
    bool alarmlimit;
    bool linelock;

    bool dankoulock;
    bool shazuiup;
    bool isruning;
    bool xtrunorguiling;
    unsigned short clothfinishcount;
    unsigned short clothsetcount;
    QQueue<int > alarmque;
    QQueue<int > commerrorcode;
    QString mainboardversion,bagversion;
};


#endif // QHMIDATA_H
