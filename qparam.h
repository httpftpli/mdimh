#ifndef QPARAM_H
#define QPARAM_H

#include <QObject>
#include <QSharedPointer>
#include <QFile>
#include <QSet>
#include <QStringList>
#include <QVariant>
#include "globaldata.h"

#define LENOFSPA 0x1000

 class QComm;
 class QParam;

const QVariant::Type XTCS_DATATYPE[]={
  QVariant::Int,    QVariant::Int,    QVariant::Int,
  QVariant::Int,    QVariant::Int,    QVariant::Int,       QVariant::Int,
  QVariant::Int,    QVariant::Int,    QVariant::Int,       QVariant::Int,
  QVariant::Int,    QVariant::Int,    QVariant::Bool,      QVariant::Int,
  QVariant::Int,    QVariant::Int,    QVariant::Bool,      QVariant::Bool,
  QVariant::Bool,   QVariant::Int
};

const QVariant::Type JQGZCS_DATATYPE[]={
  QVariant::Int,       QVariant::Int,        QVariant::Int,
  QVariant::Int,       QVariant::Int,        QVariant::Int,       QVariant::Int,
  QVariant::Bool,      QVariant::Int,        QVariant::Int,       QVariant::Int,
  QVariant::Int,       QVariant::Int,        QVariant::Int,       QVariant::Int,
  QVariant::Int,       QVariant::Bool,       QVariant::Int,       QVariant::Int,
  QVariant::Int,       QVariant::Bool,       QVariant::Int,       QVariant::Bool,
  QVariant::Int,       QVariant::Int,        QVariant::Bool,      QVariant::Int,
  QVariant::Int,       QVariant::Int,        QVariant::Int
};

enum ParamAddr{
    Addr_Base = 0x0000, //铁电存储器中参数的起始地址
    Addr_Xtcs = Addr_Base+0x0, //(未用)
    Addr_Dmdmbc = Addr_Base+0x50, //Bag2 //共192个  :度目值24档
    Addr_Spmdbc = Addr_Base+0x160, //Bag1 //共24个  :速度24档
    Addr_Bgzdmlwbc = Addr_Base+0x170, //Bag3 //共128个:纱嘴停放位置
    Addr_Ycwzxz = Addr_Base+0x180, //Bag1 //共24个  :步进卷布24档
    Addr_Fzycwzxz = Addr_Base+0x200 ,//Bag2 //共24个  :辅助卷布24档
    Addr_Fzycwzxz_z = Addr_Base+0x220, //Bag5 //共24个  :左送纱
    Addr_Fzycwzxz_f = Addr_Base+0x240, //Bag5 //共24个  :右送纱
    Addr_Jqgzcs = Addr_Base+0x260, //Bag6 //共16个  :引塔夏纱嘴停
};

enum SpaItemHd {
    SpaItemHd_Xtcs = 0,
    SpaItemHd_Dmdmbc,
    SpaItemHd_Spmdbc,
    SpaItemHd_Bgzdmlwbc,
    SpaItemHd_Ycwzxz,
    SpaItemHd_Fzycwzxz,
    SpaItemHd_Fzycwzxz_z,
    SpaItemHd_Fzycwzxz_f,
    SpaItemHd_Jqgzcs,
    SpaItemHd_Guide
};

struct SpaItemDsp{
    unsigned short addr;
    unsigned char runsendid;
    unsigned short len;
    unsigned short offsetinbag;
    short valrangebottom;
    short valrangetop;
    //short **valrange;
};

const unsigned short JQGZCS_RANGE[][2]={
    {10,100}, {20,60}, {5,30},  {4,20},  {6,12},
    {0,1000}, {0,127},  {0,1},   {0,99},  {0,99},  {0,99},
    {0,99},   {5,50},   {0,1000},{0,1000},{1,5},   {0,1},
    {0,100},  {0,100},  {0,100}, {0,1},   {4,8},   {0,1},
    {0,399},  {1,200}, {0,1},   {0,2000},{0,2000},
    {0,30},   {0,20}
};
const unsigned short XTCS_RANGE[][2]={
    {0,2000}, {0,2000}, {0,2000}, {0,2000}, {0,2000}, {1,160},
    {0,79},   {0,79},   {0,19},    {1,5},    {1,10},  {0,65535},
    {10,1000}, {0,1},   {0,128},  {0,128},  {0,100},   {0,1},
    {0,1},     {0,1},   {0,100}
};


const struct SpaItemDsp spaItemDsp[SpaItemHd_Guide] = {
 /*   ADDR            |runsendid | len |offsetinbag|valrangetop|valrangebottom  */
    {Addr_Xtcs,          7,        29,     0,          0,           0      },
#if DUAL_SYSTEM
    {Addr_Dmdmbc,        8,         8,     0,        -200,         200    },
    {Addr_Spmdbc,        9,         8,     0,        -200,         200    },
    {Addr_Bgzdmlwbc,     10,        8,     0,        -200,         200    },
#else
    {Addr_Dmdmbc,        8,         4,     0,        -200,         200    },
    {Addr_Spmdbc,        9,         4,     0,        -200,         200    },
    {Addr_Bgzdmlwbc,     10,        4,     0,        -200,         200    },
#endif
    {Addr_Ycwzxz,        11,        33,    0,        -500,         500    },
    {Addr_Fzycwzxz,      12,        16,    0,        -500,         500    },
    {Addr_Fzycwzxz_z,    13,        16,    0,        -500,         500    },
    {Addr_Fzycwzxz_f,    14,        16,    0,        -500,         500    },
    {Addr_Jqgzcs ,        1,        50,    0,         0,            0,   }
};


class QParam : public QObject
{
    friend class QPatternData;
    Q_OBJECT
    public:
    //enum HeadPosDir{BackLeft = 0,BackRight =1,FrontLeft =2,FrontRight =3};

    explicit QParam(QComm *send,QObject *parent = 0);
    ~QParam();
    QSharedPointer<QFile> spafile;
    QString spafilepath;
    QString spafilename;
    void  loadFile();
    void  releaseBuf();
    bool  setFile(const QString &spafilepath);
    short fechData(SpaItemHd handle,int offset);
    void setData(SpaItemHd handle,int offset,short data,bool emitdirty=TRUE);
#if DUAL_SYSTEM
    void setDankouLock(bool lock);  //send the dankoulock only set in the file ,
                                    // don't send to mainboard and emit signal
#endif
    void refreshBuf();
    int save(bool isdownload,bool isall=TRUE);
    unsigned short duMuZhiBuGongZuo(int row,Md::POS_LFETRIGHT kou,Md::POSFLAG_FRONTREAR fr);
    int updataPivotal();

private:
    short *spabuf;
    QSet<SpaItemHd> modifyedItem;
#if DUAL_SYSTEM
    unsigned short dumu_bugongzuo[8];
#else
    unsigned short dumu_bugongzuo[4];
#endif
    QComm *pcomm;
    int updata(SpaItemHd hd);


signals:
    void dirty(bool val);
    void changed();
public slots:

};

#endif // QPARAM_H
