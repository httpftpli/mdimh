#ifndef QPARAM_H
#define QPARAM_H
#include <QSharedPointer>
#include <QSet>
#include <QVariant>
#include "namespace.h"
#include"config.h"
#include"constdata.h"


 class QComm;
 class QParam;
 class QFile;

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


class QParam : public QObject
{

    friend class QPattern;
    Q_OBJECT
public:
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
    //enum HeadPosDir{BackLeft = 0,BackRight =1,FrontLeft =2,FrontRight =3};

    explicit QParam(QComm *send,QObject *parent = 0);
    ~QParam();
    static int spaItemValBottom(SpaItemHd hd);
    static int spaItemValTop(SpaItemHd hd);
    QSharedPointer<QFile> spafile;
    QString spafilepath;
    QString spafilename;
    bool  setFile(const QString &spafilepath);
    short fechData(SpaItemHd handle,int offset);
    void setData(SpaItemHd handle,int offset,short data,bool emitdirty=TRUE);
    int sendParama();
    int updataAll();
#if DUAL_SYSTEM
    void setDankouLock(bool lock);  //send the dankoulock only set in the file ,
                                    // don't send to mainboard and emit signal
#endif
    unsigned short duMuZhiBuGongZuo(int row,Md::POS_LEFTRIGHT kou,Md::POSFLAG_FRONTREAR fr);
    int updataPivotal();

private:
    static const struct SpaItemDsp spaItemDsp[SpaItemHd_Guide];
    short *spabuf;
    QSet<SpaItemHd> modifyedItem;
    QComm *pcomm;
    int updata(SpaItemHd hd);


signals:
    void dirty(bool val);
    void changed();
public slots:

};

#endif // QPARAM_H
