#ifndef QHMIDATA_H
#define QHMIDATA_H
#include <QObject>
#include "namespace.h"
#include "communicat.h"


const unsigned char  INPUT_MAP[32][2] = {  //显示序号与开关量的映射表(数组下标,位)
    {3,0}, {3,1}, {3,2}, {3,3}, {3,4}, {3,5}, {3,6}, {3,7},
    {2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7},
    {1,0}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {1,6}, {1,7},
    {0,4}, {0,1}, {0,2}, {0,0}, {0,3}, {0,5}, {0,6}, {0,7}
};

const QString CWBJ_ErrorCode[80]={
QObject::tr(" "),                          //[0]
QObject::tr("主电机没有准备信号!"),           // "Moto is not ready!",
QObject::tr("摇床电机没有准备信号!"),          //"Move is not ready!",
QObject::tr("机头左限位报警!"),                 // "Left limite alarm!",
QObject::tr("机头右限位报警!"),             // "Right limite alarm!",
QObject::tr("摇床限位报警!"),              //"Move bed error!",
QObject::tr("左收线报警!"),                //"Left take up error!",
QObject::tr("右收线报警!"),               //"Right take up error!",
QObject::tr("断纱报警!"),                //"Break yarn error!",
QObject::tr("储纱器断纱报警!"),             //"Store yarn break error!",
QObject::tr("大纱结报警!"),               //"Big knot yarn error!",                        //[10]
QObject::tr("卷布报警!"),                    //"Roll error!",
QObject::tr("落布报警!"),                //"Drop cloth error!",
QObject::tr("付罗拉限位报警!"),             //"Secont roll error!",
QObject::tr("前床撞针报警!"),              //"Front break knit error!",
QObject::tr("后备电源异常!"),              //"Power down error!",
QObject::tr("机头12V电源失效!"),           //"Head 12V fuse error!",                    //[16]
QObject::tr("机头主板+24V保险丝F1失效!"),     //"Head +24V fuse F1 error!",        //[17]
QObject::tr("1号选针器板+24V保险丝F3失效!"),//"Select 1 +24V fuse F3 error!",
QObject::tr("探针报警!"),                //"Probe error!",
QObject::tr("7号密度马达故障!"),        //"No.7 density motor error!",                //[20]
QObject::tr("8号密度马达故障!"),        //"No.8 density motor error!",
QObject::tr("3号密度马达故障!"),        //"No.3 density motor error!",
QObject::tr("4号密度马达故障!"),        //"No.4 density motor error!",
QObject::tr("5号密度马达故障!"),        //"No.5 density motor error!",
QObject::tr("6号密度马达故障!"),        //"No.6 density motor error!",
QObject::tr("1号密度马达故障!"),        //"No.1 density motor error!",
QObject::tr("2号密度马达故障!"),        //"No.2 density motor error!",
QObject::tr("后生克电机故障!"),         //"Back sinker error!",
QObject::tr("前生克电机故障!"),         //"Front sinker error!",
QObject::tr("摇床零位异常!"),          //"Bed zero error!",                                //[30]
QObject::tr("紧急制动锁定!"),          //"Stop break locking!",                            // 31
QObject::tr("编织花样超出针板范围!"),      //"Over neddle plate!",
QObject::tr("2号选针器板+24V保险丝F2失效!"),//"Select 2 +24V fuse F2 error!",     // 33
QObject::tr("机头电机板-24V保险丝F6失效!"),//"Motor -24V fuse F6 error!",
QObject::tr("1号选针器板-24V保险丝F4失效!"),//"Select 1 -24V fuse F4 error!",
QObject::tr("2号选针器板-24V保险丝F5失效!"),//"Select 2 -24V fuse F5 error!",
QObject::tr("后床撞针报警!"),              //"Back break knit error!",
QObject::tr("摇床位置错误!"),              //"Move bed place error!",                         //38
QObject::tr("主板12V保险丝失效!"),          //"Main board 12V fuse error!",                 //[39]
QObject::tr("内存无花型!"),               //"No file in mem!",                                 //[40]
QObject::tr("错误3"),                    //"Rev error3!",
QObject::tr("设定件数完成!"),              //"Set pieces done!",
QObject::tr("单件停车!"),                   //"One finish stop!",
QObject::tr("机头背包通讯异常!"),            //"Head communications error!",
QObject::tr("后备电源正在掉电!"),            //"Power downing!",
QObject::tr("系统保留的错误"),              //"System reservation error!",
};


class QHMIData: public QObject{
   Q_OBJECT

public:
    enum DATATAG{
                 GLWC,          //系统归零完成
                 WBSR= 0,       //外部输入信号
                 ZXHJSQ,
                 ZSD,           //指示灯（输出）
                 BJLL,          //步进罗拉（输出） unsigned char 1:ispersent,unsigned char 0:clockwise unsigned char percent
                 FZLL,          //辅助罗拉（输出）
                 ZSS,           //左送纱（输出）
                 YSS,           //右送纱（输出）
                 JTDJ,           //机头电机（输出）
                 YC,            //摇床（输出）
                 QZDUDJ,        //前左度目电机（输出）
                 QYDUDJ,        //前右度目电机（输出）
                 HZDUDJ,        //后左度目电机（输出）
                 HYDUDJ,        //后右度目电机（输出）
                 XZQ,           //选针器（输出）
                 SJDCT,         //三角电磁铁（输出）

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
                 JTDZZT,         //机头动作状态,标记是否停车 0：停止 1：运行


                 QHXTZT_ACK,     //切换系统状态应答0X40
                 CWBJXX,        //错误报警信息0X43

                 YXTHCL,      //运行跳行处理OX47

                 CXBJZSZJTH,   //程序编辑中的沙嘴交/替换0x49
                 CXBJZ_CNT_GB,  //程序编辑中的CNT数据块改变0x4a
                 DQZDJZT,       //当前主电机状态

                 GUARDIANS        //枚举类型最后一个，仅用于边界守护

             };


    typedef struct __HMIData {
        int Val;
        unsigned char  InputOrOutPut;  //0:output, 1:input, 3:biderection
    }HMIDataType;

    HMIDataType dataBuf[GUARDIANS];

    QHMIData(QSend *send,QObject *parent = 0);

    bool speedLimit;
    bool stopPerOne;
    bool alarmLimit;
    bool dankouLock;
    bool patternVailable;
    char sysStat;
    unsigned short customerId;
    int stopTimeHistory;
    int runTimeHistory;
    int curruntTime;
    int stopTime;
    int runTime;
    QString patFilePath;
    QString cntFilePath;
    QString wrkFilePath;
    QString sys_wrkFilePath;
    QString spaFilePath;
    QString sazFilePath;
    QString loopFilePath;
    QString sysLogFilePath;
    QString udiskDirPath;
    QString sysconfigfilename;
    QString parttenfilename;
    int totalBzTime;
    int currentBzTime;
    int totalStopTime;
    int currentStopTime;
    QSend::SendResult commResult;
    QSend *psend;
    bool isInBoot;
    bool isRun();
    void loadParam(const QString &inifilepath);
    void downloadParam();
    void finish();
    void setclothSetCount(unsigned short val);
    void setclothFinishCount(unsigned short val);
    void saveSysCfgFile();
    void start();
    unsigned short clothSetCount;
    unsigned short clothFinishCount;
    Md::InitResult errorcode;
    Md::InitResult errorCode();

public slots:
    void RequireData(unsigned short index);
    void On_DataChanged_FromHMI(unsigned short index,QVariant Val);
    void On_DataChanged_FromCtrl(unsigned short index,QVariant Val);
    void on_patternChange( const QString &patternname, const  QString &cntfilepath, const QString &patfilepath,
                          const  QString &wrkfilepath , const QString &sazfilepath);
    void on_clothFinish();

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
    void hmi_xtguilingwc(unsigned char val);  //0x55归零完成，0xbb归零错误
    void Sig_tatalPatLine(int line);
    void clothSetCountChanged(int val);
    void clothFinishCountChanged(int val);
    void time1sOuted();
////////////////////////////////
protected:
    virtual void timerEvent(QTimerEvent * event);//1s 600ms,
private  slots:
    void setJitouTingChi();//置停车标记
private:
    int timeid1s;
    int timeid700ms;
};


#endif // QHMIDATA_H