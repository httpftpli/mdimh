#ifndef QPARAM_H
#define QPARAM_H

#include <QObject>
#include <QSharedPointer>
#include <QFile>
#include <QSet>
#include <QStringList>
#include <QVariant>

#define LENOFSPA 0x1000

 class QSend;

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

enum ItemHd {
    ItemHd_Xtcs = 0,
    ItemHd_Dmdmbc,
    ItemHd_Spmdbc,
    ItemHd_Bgzdmlwbc,
    ItemHd_Ycwzxz,
    ItemHd_Fzycwzxz,
    ItemHd_Fzycwzxz_z,
    ItemHd_Fzycwzxz_f,
    ItemHd_Jqgzcs,
    ItemHd_Guide
};

struct ItemDsp{
    ParamAddr addr;
    unsigned short len;
    unsigned char bag;
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


const struct ItemDsp ITEM_DSP[] = {
 /*   ADDR             len |bag|offsetinbag|valrangetop|valrangebottom  */
    {Addr_Xtcs,        24,  4,      0,          0,           0      },
    {Addr_Dmdmbc,       8,  4,      24,        -200,         200    },
    {Addr_Spmdbc,       8,  4,      32,        -200,         200    },
    {Addr_Bgzdmlwbc,    8,  4,      40,        -200,         200    },
    {Addr_Ycwzxz,      33,  4,      48,        -500,         500    },
    {Addr_Fzycwzxz,    16,  4,      81,        -500,         500    },
    {Addr_Fzycwzxz_z,  16,  4,      97,        -500,         500    },
    {Addr_Fzycwzxz_f,  16,  4,      113,       -500,         500    },
    {Addr_Jqgzcs ,     32,  1,      129,        0,            0,    }
};


class QParam : public QObject
{
    Q_OBJECT
    public:
    enum HeadPosDir{BackLeft = 0,BackRight =1,FrontLeft =2,FrontRight =3};

    explicit QParam(QSend *send,QObject *parent = 0);
    ~QParam();
    QSharedPointer<QFile> spafile;
    QString spafilepath;
    QString spafilename;
    void  loadFile();
    void  releaseBuf();
    bool  setFile(const QString &spafilepath);
    short fechData(ItemHd handle,int offset);
    void setData(ItemHd handle,int offset,short data);
    void refreshBuf();
    void save(bool isdownload);
    unsigned short duMu_BuGongZuo(HeadPosDir posdir);

private:
    short *spabuf;
    QSet<ItemHd> modifyedItem;
    unsigned short dumu_bugongzuo[4];
    QSend *qsend;
signals:
    void dirty(bool val);
    void changed();
public slots:

};

#endif // QPARAM_H
