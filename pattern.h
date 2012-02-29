#ifndef PATTERN_H
#define PATTERN_H

#include<QFile>
#include<QSize>
#include<QFlags>
#include<QSharedPointer>
#include<QVector>
#include<namespace.h>
#include<QSet>
#include "communicat.h"


#define LEN_OF_WRK 0x6E1
#define LEN_0F_SPA 0x294


#define CNT_S1Q_Pat             0	// 1系统前花板行号
#define CNT_S1Q_AZiLing		2
#define CNT_S1Q_AColor		3	// 1系统前花板A位色彩(2B)
#define CNT_S1Q_HZiLing		6	// 1系统前A,H位动作(1B):0x0=空,空;0x8=编织, 空;0xd=编织,吊目;0x1=翻针,空
#define CNT_S1Q_HColor		7
#define CNT_S1H_Pat		10
#define CNT_S1H_AZiLing		12
#define CNT_S1H_AColor		13
#define CNT_S1H_HZiLing		16
#define CNT_S1H_HColor		17
#define CNT_S2Q_Pat		20	// 2系统前花板行号
#define CNT_S2Q_AZiLing		22
#define CNT_S2Q_AColor		23	// 2系统前花板A位色彩(2B)
#define CNT_S2Q_HZiLing		26	// 2系统前A,H位动作(1B):0x0=空,空;0x8=编织, 空;0xd=编织,吊目;0x1=翻针,空
#define CNT_S2Q_HColor		27
#define CNT_S2H_Pat		30
#define CNT_S2H_AZiLing		32
#define CNT_S2H_AColor		33
#define CNT_S2H_HZiLing		36
#define CNT_S2H_HColor		37
#define CNT_S1_SaZui		80
#define CNT_Qdb_SaCu		82
#define CNT_S2_SaZui		83
#define CNT_Qdb_JiaZi		85
#define CNT_DuMu                92
#define CNT_SuDu		93
#define CNT_JuanBu		94
#define CNT_FuJuanBu		95
#define CNT_FuJBKaiHe		96
#define CNT_TingCe		99
#define CNT_YaoCuangDir		100
#define CNT_YaoCuangZi		101
#define CNT_SaZuoTingFang       107
#define CNT_HuiZuanJu		108
#define CNT_YinTaXia		113
#define CNT_SaRu		114	//纱入标志,0x55表示本行有纱入(自定义)
#define CNT_SaRuYarn		115	//纱入的纱嘴位变量(自定义)
#define CNT_End			116
#define CNT_LoopStart		117
#define CNT_LoopNum		119
#define CNT_DuMu_ExS2		125	//两个度目中的系统2度目档
#define CNT_CRCL		125	//自定义
#define CNT_CRCH                126	//自定义
#define CNT_YaJiaoDuan		127


enum WrkAddr{
    WrkAddr_Base = 0x0000, //铁电存储器中参数的起始地址
    WrkAddr_GongZuo = WrkAddr_Base+0x0, //(未用)
    WrkAddr_DuMuZi = WrkAddr_Base+0x000, //Bag2 //共192个  :度目值24档
    WrkAddr_SuDuZi = WrkAddr_Base+0x180, //Bag1 //共24个  :速度24档
    WrkAddr_ShaZuiTF = WrkAddr_Base+0x200, //Bag3 //共128个:纱嘴停放位置
    WrkAddr_JuanBu = WrkAddr_Base+0x300, //Bag1 //共24个  :步进卷布24档
    WrkAddr_JuanBuFZ = WrkAddr_Base+0x350 ,//Bag2 //共24个  :辅助卷布24档
    WrkAddr_LeftSongSa = WrkAddr_Base+0x400, //Bag5 //共24个  :左送纱
    WrkAddr_RightSongSa = WrkAddr_Base+0x420, //Bag5 //共24个  :右送纱
    WrkAddr_YTXTingFang = WrkAddr_Base+0x450, //Bag6 //共16个  :引塔夏纱嘴停放
    WrkAddr_YTXXiuZen = WrkAddr_Base+0x500, //Bag6 //共192个:引塔夏纱嘴修正
    WrkAddr_CJP_FanZen = WrkAddr_Base+0x600, //Bag5 //共48个  :后,前沉降片翻针位段数
    WrkAddr_CJP_BianZi = WrkAddr_Base+0x640, //Bag5 //共48个  :后,前沉降片编织位段数
    WrkAddr_PzkSaZui = WrkAddr_Base+0x680, //Bag6 //共64个  :片展开纱嘴对应表
    WrkAddr_PzkClose = WrkAddr_Base+0x6E0, //Bag6 //共1个    :片展开关闭状态(1表示关闭)
    WrkAddr_ZanKaiPianSu =WrkAddr_Base+0x6F0, //Bag1 // 1个         :展开片数
    WrkAddr_QiZenDian = WrkAddr_Base+0x6F5//Bag1 //机器工作参数中的起针点
};
enum WrkItemHd {
    WrkItemHd_DuMuZi = 0,
    WrkItemHd_SuDuZi,
    WrkItemHd_ShaZuiTF,
    WrkItemHd_JuanBu,
    WrkItemHd_JuanBuFZ,
    WrkItemHd_LeftSongSa,
    WrkItemHd_RightSongSa,
    WrkItemHd_YTXTingFang,
    WrkItemHd_YTXXiuZen,
    WrkItemHd_CJP_FanZen,
    WrkItemHd_CJP_BianZi,
    WrkItemHd_PzkSaZui,
    WrkItemHd_ZanKaiPianSu,
    WrkItemHd_QiZenDian,
    WrkItemHd_Guide
};

struct WrkItemDsp{
    WrkAddr addr;
    unsigned char runsendid;
    unsigned short len;
    unsigned short offsetinbag;
    short valrangebottom;
    short valrangetop;
};

const struct WrkItemDsp wrkItemDsp[WrkItemHd_Guide]={
   /*      addr             |runsendid |  len   |offsetinbag|bottom |top  */
#if DUAL_SYSTEM
    { WrkAddr_DuMuZi,             4,      192,      0,         0,     800},
#else
    { WrkAddr_DuMuZi,             4,       96,      0,         0,     800},
#endif
    { WrkAddr_SuDuZi,             2,       24,      0,         1,     100},
    { WrkAddr_ShaZuiTF,           6,      128,      0,         0,      99},
    { WrkAddr_JuanBu,             3,       24,      0,      -100,     100},
    { WrkAddr_JuanBuFZ,           5,       24,      0,      -100,     100},
    { WrkAddr_LeftSongSa,        15,       24,      0,         0,     100},
    { WrkAddr_RightSongSa,       16,       24,      0,         0,     100},
    { WrkAddr_YTXTingFang,       19,       16,      0,         0,      30},
    { WrkAddr_YTXXiuZen,         20,      192,      0,         0,      20},
    { WrkAddr_CJP_FanZen,        17,       48,      0,         0,    1000},
    { WrkAddr_CJP_BianZi,        18,       48,      0,         0,    1000},
    { WrkAddr_PzkSaZui,          21,       65,      0,         1,       8},
    { WrkAddr_ZanKaiPianSu,       1,        1,      24,        1,       8},
    { WrkAddr_QiZenDian,          1,        1,      0,         0,     800}
};


class QPatternData : public QObject{
    friend class QParam;
    Q_OBJECT
public:
    enum FileState {
             CNTFILESET = 0x01,
             PATFILESET = 0x02,
             WRKFILESET = 0x04,
             SAZFILESET = 0X08,
             CNTFILELOAD = 0x100,
             PATFILELOAD = 0x200,
             WRKFILELOAD = 0x400,
             SAZFILELOAD = 0x800,

         };

    enum Result {
             CntFileError = 0x100,
             PatFileError = 0X101,
             CntPatNotMatch = 0x102,
             ShaZuiKbError =  0x103,
             NoCntFile = 0x104,
             NoPatFile = 0x105,
             NoWrkFile = 0x106,
             Ok          = 0
         };
    struct SzkbData{
         unsigned short ZuSa;
         unsigned short FuSa; //纱嘴号1~8
         unsigned short Start;
         unsigned short End;
     };


    Q_DECLARE_FLAGS(FileStateFlag, FileState)
    QPatternData(QComm *s,QObject *parent=0);
    ~QPatternData();
    Result setFile(const QString &cntfilepath,const QString &patfilepath);
    Md::HAVEFILEFLAG loadFile(Md::HAVEFILEFLAG flag=Md::HAVEALL);
    bool isWrkfileSys();
    bool isPatternAvailable();
    void refreshBuf(Md::HAVEFILEFLAG flag);
    void deloadFile(Md::HAVEFILEFLAG flag=Md::HAVEALL);

    Md::HAVEFILEFLAG loadStatus();
    Result loadLoop(const QString &prmfilepath);
    int Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag=Md::HAVENO);
    int sendShazuiKb();



    unsigned char  cnt_yaJiao(unsigned short row);
    QString        cnt_yaoChuang(unsigned short row);
    unsigned char  cnt_duMuZu(unsigned short row, unsigned char sys,bool &doubleOrSigle);
    unsigned char  cnt_duMuZu(unsigned short row, Md::POS_LFETRIGHT system_kou,bool &doubleOrSigle);
    unsigned int   cnt_dumuUsed();
    unsigned char  cnt_spead(unsigned short row);
    unsigned char  cnt_mainLuola(unsigned short row);
    unsigned char  cnt_fuzuLuola(unsigned short row);
    unsigned char  cnt_songSha(unsigned short row);
    unsigned char  cnt_shazuiTf(unsigned short row);
    unsigned char  cnt_tingChe(unsigned short row);
    int            cnt_Azhiling(unsigned short row,unsigned char system,Md::POS_FRONTREAR);
    int            cnt_Azhiling(unsigned short row,Md::POS_LFETRIGHT kou,Md::POS_FRONTREAR);
    int            cnt_Hzhiling(unsigned short row,unsigned char system,Md::POS_FRONTREAR);
    int            cnt_Hzhiling(unsigned short row,Md::POS_LFETRIGHT kou,Md::POS_FRONTREAR);
    QString        cnt_seDaiHaoA(unsigned short row,unsigned char system,Md::POS_FRONTREAR frontorrear);
    QString        cnt_seDaiHaoA(unsigned short row,Md::POS_LFETRIGHT kou,Md::POS_FRONTREAR frontorrear);
    QString        cnt_seDaiHaoH(unsigned short row,unsigned char system,Md::POS_FRONTREAR frontorrear);
    QString        cnt_seDaiHaoH(unsigned short row,Md::POS_LFETRIGHT kou,Md::POS_FRONTREAR frontorrear);
    unsigned char  cnt_shaZui(unsigned short row,unsigned char system);
    unsigned char  cnt_shaZui(unsigned char row,Md::POS_LFETRIGHT kou);
    unsigned short cnt_huabanhang(unsigned short row,Md::POS_LFETRIGHT sys_kou,Md::POS_FRONTREAR frontorrear);
    unsigned short cnt_huabanhang(unsigned short row,unsigned char sys,Md::POS_FRONTREAR pos);

    unsigned char  shaZui(unsigned short row,unsigned char system);    //返回沙嘴捆绑后当前行的沙嘴
    unsigned char  shaZui(unsigned short row,Md::POS_LFETRIGHT kou);    //返回沙嘴捆绑后当前行的沙嘴
    unsigned char  duMuZhi(unsigned short row,bool backorfront, unsigned char dumuindex);
    unsigned short wrk_qizhengdian();


    unsigned short cnt_FechData(unsigned short row,unsigned char addr,unsigned short leng=1);
    void cnt_SetData(unsigned short row,unsigned char index,unsigned short data,unsigned char len);
    inline char pat_FechData(unsigned short row,unsigned short column);
    void pat_SetData(unsigned short row,unsigned short column,unsigned char data, bool download = TRUE);

    short wrk_fechData(WrkItemHd handle,int offset);
    void  wrk_setData(WrkItemHd handle,int offset,short data);

    int  wrkSpeedZhi(unsigned int index);
    int  wrkMainLuolaZhi(unsigned int index);
    int  wrkFuzhuLuolaZhi(unsigned int index);

    QSize patternSize() const;
    bool isValid () const ;

    QString patternName;
    QString patternDirPath;
    QString patFilePath;
    QString cntFilePath;
    QString wrkFilePath;
    QString wrkFileName;
    QString sazFilePath;
    unsigned char shazuiused_r;  //右口用到的沙嘴；
    unsigned char shazuiused_l;  //左口用到的沙嘴；
    QList<SzkbData> sazbuf;   //沙嘴捆绑数据
    unsigned char shaZuiKb;   //捆绑的沙嘴，从sazbuf中提取；

    QSharedPointer<QFile> cntfile;
    QSharedPointer<QFile> patfile;
    QSharedPointer<QFile> wrkfile;

    int tatalcntrow;
    int tatalpatrow;
    int tatalcolumn;
    int patbyteperrow;
    QSet<unsigned short> patModifiedRow;//用于保存修改的pat行
    QSet<unsigned short> cntModifiedRow;//用于保存修改的cnt行
    QSet<WrkItemHd> wrkModifiedHandle; //用于保存修改的wrk包号

signals:
    void patternChanged(const QString &patternName,const QString &cntfilepath,const QString &patfilepath,
                        const QString &wrkfilepath,const QString &sazfilepath);
    void patDirty(bool val);
    void cntDirty(bool val);
    void wrkDirty(bool val);
private:
    unsigned char dumu_history_sys1,dumu_history_sys2;
    QComm *pcomm;
    unsigned char *cntbuf,*patbuf; //花型缓冲区
    unsigned short *wrkbuf;
    bool isdualdumuzu;
    unsigned int dumuzu_used;

    int _azl(unsigned char data);
    int _hzl(unsigned char data);

    bool iswrkfilesys;
    bool ispatternavalible;

    int wrk_updata(WrkItemHd hd);
    unsigned char kou2sys(Md::POS_LFETRIGHT kou,unsigned short cntnumber);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QPatternData::FileStateFlag)



#endif // PATTERN_H
