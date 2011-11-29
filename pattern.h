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
    WrkItemHd_PzkClose,
    WrkItemHd_ZanKaiPianSu,
    WrkItemHd_QiZenDian,
    WrkItemHd_Guide
};

struct WrkItemDsp{
    WrkAddr addr;
    unsigned short len;
    unsigned char bag;
    unsigned short offsetinbag;
    short valrangebottom;
    short valrangetop;
};

const struct WrkItemDsp wrkItemDsp[WrkItemHd_Guide]={
   /*      addr            |  len  |  bag |offsetinbag|bottom |top  */
    { WrkAddr_DuMuZi,          192,    2,      0,         0,    800},
    { WrkAddr_SuDuZi,          24,     1,      0,         1,    100},
    { WrkAddr_ShaZuiTF,        128,    3,      0,         0,    99 },
    { WrkAddr_JuanBu,          24,     1,      24,      -100,   100},
    { WrkAddr_JuanBuFZ,        24,     2,      192,     -100,   100},
    { WrkAddr_LeftSongSa,      24,     5 ,     0,         0,    100},
    { WrkAddr_RightSongSa,     24,     5 ,     12,        0,    100},
    { WrkAddr_YTXTingFang,     16,     6,      0,         0,     30},
    { WrkAddr_YTXXiuZen,       192,    6,      16,        0,     20},
    { WrkAddr_CJP_FanZen,      48,     5,      24,        0,   1000},
    { WrkAddr_CJP_BianZi,      48,     5,      72,        0,   1000},
    { WrkAddr_PzkSaZui,        64,     6,      208,       1,      8},
    { WrkAddr_PzkClose,        1,      6,      272,       1,      8},
    { WrkAddr_ZanKaiPianSu,    1,      1,      0,         1,      8},
    { WrkAddr_QiZenDian,       1,      1,      24,        0,    800}
};


class QPatternData : public QObject{
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
    QPatternData(QSend *s,QObject *parent=0);
    ~QPatternData();
    Result setFile(const QString &cntfilepath,const QString &patfilepath, const QString &wrkfilepath,const QString &sazfilepath,Md::HAVEFILEFLAG openflag=Md::HAVENO);
    Md::HAVEFILEFLAG loadFile(Md::HAVEFILEFLAG flag=Md::HAVEALL);
    void refreshBuf(Md::HAVEFILEFLAG flag);
    void deloadFile(Md::HAVEFILEFLAG flag=Md::HAVEALL);
    void patSetData(unsigned short row,unsigned short column,unsigned char data);
    void cntSetData(unsigned short row,unsigned char index,unsigned short data,unsigned char len);
    Md::HAVEFILEFLAG loadStatus();
    Result loadLoop(const QString &prmfilepath);
    void Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag=Md::HAVENO);

    unsigned char shaZui(unsigned short row);    //返回沙嘴捆绑后当前行的沙嘴
    unsigned char cnt_shaZui1(unsigned short row);//返回CNT文件中当前行1系统的沙嘴
    unsigned char cnt_shaZui2(unsigned short row);//返回CNT文件中当前行2系统的沙嘴
    unsigned char cnt_duMu(unsigned short row,bool &doubleOrSigle);
    unsigned char duMuZhi(bool backorfront,unsigned short row);
    unsigned char cnt_yaJiao(unsigned short row);
    QString  cnt_yaoChuang(unsigned short row);
    QString  cnt_seDaiHao(unsigned short row,unsigned char inc);
    unsigned char cnt_spead(unsigned short row);
    unsigned char cnt_mainLuola(unsigned short row);
    unsigned char cnt_fuzuLuola(unsigned short row);
    unsigned char cnt_songSha(unsigned short row);
    unsigned char cnt_shazuiTf(unsigned short row);
    unsigned char cnt_tingChe(unsigned short row);

    unsigned short cnt_huabanhang_q1(unsigned short row);
    unsigned short cnt_huabanhang_h1(unsigned short row);
    unsigned short cnt_huabanhang_q2(unsigned short row);
    unsigned short cnt_huabanhang_h2(unsigned short row);

    unsigned short wrk_qizhengdian();


    //unsigned char  cnt_fechData(unsigned short row,unsigned char addr);
    unsigned short cnt_fechData(unsigned short row,unsigned char addr,unsigned short leng=1);
    short wrk_fechData(WrkItemHd handle,int offset);
    void  wrk_setData(WrkItemHd handle,int offset,short data);

    QSize patternSize() const;
    bool isValid () const ;

    int _azl(unsigned char data);
    int _hzl(unsigned char data);

    QString patternName;
    QString patternDir;
    QString patFilePath;
    QString cntFilePath;
    QString wrkFilePath;
    QString wrkFileName;
    QString sazFilePath;
    unsigned char *cntbuf;    //用于载入整个CNT文件
    unsigned char *patbuf;    //用于载入整个PAT文件
    unsigned short *wrkbuf;    //用于载入整个wrk文件
    unsigned char shazuiused_r;  //系统1左边花型中用到的沙嘴；
    unsigned char shazuiused_l;  //系统1左边花型中用到的沙嘴；
    QList<SzkbData> sazbuf;   //沙嘴捆绑数据
    unsigned char shaZuiKb;   //捆绑的沙嘴，从sazbuf中提取；

    QSharedPointer<QFile> cntfile;
    QSharedPointer<QFile> patfile;
    QSharedPointer<QFile> wrkfile;
    QSharedPointer<QFile> sazfile;

    unsigned short tatalrow;
    unsigned short tatalcolumn;
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
    unsigned char dumu_history;
    QSend *qsend;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(QPatternData::FileStateFlag)



#endif // PATTERN_H
