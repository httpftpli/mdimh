#ifndef PATTERN_H
#define PATTERN_H


#include<QSharedPointer>
#include<namespace.h>
#include<QSet>
#include"config.h"
#include"constdata.h"
#include <QDir>


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

class QFile;
class QSize;
class QComm;

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

static const unsigned char CNT_ZHILINGVAL[9]={0x00,0x01,0x07,0x08,0xa,0x0b,0x0d,0x0e,0x0f};


class QPattern : public QObject{
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
             FaultPatternDir = 0x107,
             Ok          = 0
         };
    enum YCPOSITION{
        ZHENDUICHI = 0x00,
        ZHENDUIZHEN = 0x01,
        FANZHENZHE =  0x02,
        FANZHENFU =   0x03
    };
    enum ORDER{
        Descending_Order,
        Ascending_Order,
        No_Order
    };

    struct SzkbData{
         unsigned short ZuSa;
         unsigned short FuSa; //纱嘴号1~8
         unsigned short Start;
         unsigned short End;
     };

    typedef struct _CntLoopType{
        int startline;
        int endline;
        int numofloop;
    }CntLoopType;


    Q_DECLARE_FLAGS(FileStateFlag, FileState)

    static QMap<unsigned char ,QStringList>CntZhilingMap;
    static QMap<unsigned char ,unsigned char> CntZhilingIndexMap;

    QPattern(QComm *s,QObject *parent=0);
    ~QPattern();
    template<typename T>inline static  T string2bitarray(const QString &str){
        T r = 0;
        for(int i=0;i<str.size();i++){
            char ch = str.at(i).toAscii();
            if(('0'<=ch)&&('9'>=ch))
                ch = ch-'0';
            else if(('A'<=ch)&&('F'>=ch))
                ch = ch-'A'+10;
            else if(('a'<=ch)&&('f'>=ch))
                ch = ch-'a'+10;
            else
                continue;
            r |= 1<<ch;
        }
        return r;
    }

    template<typename T>inline static  QString bitarray2string(T val,ORDER order){
        int numofbit = sizeof(val)*8;
        QString str;
        for(int i = 0;i<numofbit;i++){
            if((val&(1<<i))!=0)
                (order==Ascending_Order)?str.append(QString::number(i,16)):
                                         str.prepend(QString::number(i,16));
        }
        return str;
    }

    Result         setFile(const QString &pattern);
    int               sendPattern();
    bool            isWrkfileSys() const;
    bool            isPatternAvailable() const;
   //void            refreshBuf(Md::HAVEFILEFLAG flag);

    Result          loadLoop(const QString &prmfilepath);
    int             Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag=Md::HAVENO);
    unsigned char   shaZui(int row,unsigned char system);    //返回沙嘴捆绑后当前行的沙嘴
    unsigned char   shaZui(int row,Md::POS_LEFTRIGHT kou);    //返回沙嘴捆绑后当前行的沙嘴
    unsigned char   duMuZhiWork(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR frontrear ,unsigned char dumuindex);
    int             sendShazuiKb();
    QSize           patternSize() const;

    inline unsigned short cnt_FechData(int row,unsigned char addr,unsigned char leng=1) const;
    int cnt_SetData(int row,unsigned char index,unsigned short data,unsigned char len, bool download);
    unsigned char  cnt_yaJiao(int row)const;
    void        cnt_setYajiao(int row,unsigned char yajiao);
    void           cnt_yaoChuang(int row,YCPOSITION &pos,signed char &val);
    void        cnt_setYaoChang(int row,YCPOSITION pos,char val);

    unsigned char  cnt_duMuZu(int row, unsigned char sys,bool &doubleOrSigle, bool usehistorydumu);
    unsigned char  cnt_duMuZu(int row, Md::POS_LEFTRIGHT kou,bool &doubleOrSigle,bool usehistorydumu);
    void        cnt_setDuMuZu(int row, Md::POS_LEFTRIGHT kou,bool doubleorsignle,unsigned char dumu);
    unsigned int   cnt_dumuUsed();
    unsigned char  cnt_spead(int row);
    void        cnt_setSpeed(int row, unsigned char speed);
    unsigned char  cnt_mainLuola(int row);
    void        cnt_setMainLuola(int row, unsigned char mainluola);
    unsigned char  cnt_fuzuLuola(int row);
    void        cnt_setFuzuLuola(int row, unsigned char fluola);
    unsigned char  cnt_songSha(int row);
    void        cnt_setSongsha(int row, unsigned char songsha);
    unsigned char  cnt_shazuiTf(int row);
    void        cnt_setShazuiTf(int row, unsigned char shazuitf);
    unsigned char  cnt_tingChe(int row);
    void        cnt_setTingChe(int row,bool tingche);

    unsigned char   cnt_Zhiling(int row,unsigned char system,Md::POS_FRONTREAR);
    unsigned char   cnt_Zhiling(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR);
    void          cnt_setZhiling(int row,unsigned char system,Md::POS_FRONTREAR fr,unsigned char val);
    void          cnt_setZhiling(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR fr,unsigned char val);

    QString        cnt_seDaiHaoA(int row,unsigned char system,Md::POS_FRONTREAR frontorrear);
    void        cnt_setSeDaiHaoA(int row,unsigned char system,Md::POS_FRONTREAR fr,unsigned short val);
    QString        cnt_seDaiHaoA(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR frontorrear);
    void        cnt_setSeDaiHaoA(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR fr,unsigned short val);

    QString        cnt_seDaiHaoH(int row,unsigned char system,Md::POS_FRONTREAR frontorrear);
    void        cnt_setSeDaiHaoH(int row,unsigned char system,Md::POS_FRONTREAR fr,unsigned short val);
    QString        cnt_seDaiHaoH(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR frontorrear);
    void        cnt_setSeDaiHaoH(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR fr,unsigned short val);

    unsigned char  cnt_shaZui(int row,unsigned char system);
    unsigned char  cnt_shaZui(int row,Md::POS_LEFTRIGHT kou);
    void        cnt_setShaZui(int row,Md::POS_LEFTRIGHT kou,unsigned char shazuibitmap);
    unsigned short cnt_huabanhang(int row,Md::POS_LEFTRIGHT sys_kou,Md::POS_FRONTREAR frontorrear);
    unsigned short cnt_huabanhang(int row,unsigned char sys,Md::POS_FRONTREAR pos);
    void        cnt_setHuabanhang(int row,Md::POS_LEFTRIGHT kou ,Md::POS_FRONTREAR fr,int huabanrow);
    bool cnt_shaZuiUsed(unsigned char &left,unsigned char &right ); //outparam :left right ,is shazui map from bit0 ;
    QMap<int,CntLoopType> cnt_LoopTable() const;
    int cnt_setLoopTable(QMap<int,CntLoopType> loop);

    char           pat_FechData(int row,unsigned short column) const;
    void           pat_SetData(int row,unsigned short column,unsigned char data);

    unsigned short wrk_qizhengdian();
    short          wrk_fechData(WrkItemHd handle,int offset);
    void           wrk_setData(WrkItemHd handle,int offset,short data);
    int             wrk_updataAll();
    int            wrkSpeedZhi(unsigned int index);
    int            wrkMainLuolaZhi(unsigned int index);
    int            wrkFuzhuLuolaZhi(unsigned int index);
    unsigned char saz_shaZuiUsed();


    QString patternName;
    QString patFileName;
    QString patFilePath;
    QString cntFileName;
    QString cntFilePath;
    QString wrkFilePath;
    QString wrkFileName;
    QString sazFilePath;
    QString sazFileName;
    unsigned char shazuiused_r;  //TODO: maybe will del右口用到的沙嘴；
    unsigned char shazuiused_l;  ////TODO:maybe will del左口用到的沙嘴；
    QList<SzkbData> sazbuf;      //沙嘴捆绑数据

    int tatalcntrow;
    int tatalpatrow;
    int tatalcolumn;
    int patbyteperrow;
    QSet<unsigned short> patModifiedRow;//用于保存修改的pat行
    QSet<unsigned short> cntModifiedRow;//用于保存修改的cnt行
    QSet<WrkItemHd> wrkModifiedHandle; //用于保存修改的wrk包号

signals:
    void patternChanged(const QString &dirpath , const QString &name);
    void patDirty(bool val);
    void cntDirty(bool val);
    void wrkDirty(bool val);
    void patternSendPercent(int val);
private:
    QString patterndirpath;
    QSharedPointer<QFile> cntfile;
    QSharedPointer<QFile> patfile;
    QSharedPointer<QFile> wrkfile;
    unsigned char dumu_history_sys1,dumu_history_sys2;
    QComm *pcomm;
    unsigned char *cntbuf,*patbuf; //花型缓冲区
    unsigned short *wrkbuf;
#if DUAL_SYSTEM
    bool isdualdumuzu;
#endif
    static int _azlfordisplay(unsigned char data);
    static int _hzlfordisplay(unsigned char data);
    static unsigned char _azlfromdisplay(int index);
    static unsigned char _hzlfromdisplay(int index);

    static inline unsigned char kou2sys(Md::POS_LEFTRIGHT kou,int cntnumber);
    //inline unsigned char sys2kou(unsigned char sys,int cntnumber);

    bool iswrkfilesys;
    bool ispatternavalible;
    int wrk_updata(WrkItemHd hd);
    static QMap<unsigned char ,QStringList> initcntzhilingmap();
    static QMap<unsigned char ,unsigned char> initcntzhilingindexmap();
    void reset();
    bool parsepatternpath(const QString &patternpath);
    Result probecnt();
    Result probepat();
    void probewrk();
    void probesaz();
    void probeprm();
    inline unsigned short __cntfetchshortdata(int row,unsigned char offset )const;
    inline unsigned short __cntfetchchardata(int row,unsigned char offset)const;
    inline unsigned char __cnt_shazui(int row,unsigned char system)const;
    inline unsigned char __cnt_shazui(int row, Md::POS_LEFTRIGHT kou)const;


};

Q_DECLARE_OPERATORS_FOR_FLAGS(QPattern::FileStateFlag)

#endif // PATTERN_H
