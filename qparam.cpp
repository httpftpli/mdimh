#include "qparam.h"
#include <QFileInfo>
#include "qsyslog.h"
#include "globaldata.h"
#include "communicat.h"
#include "pattern.h"
#include "config.h"
#include"constdata.h"


const struct QParam::SpaItemDsp QParam::spaItemDsp[QParam::SpaItemHd_Guide] = {
 /*   ADDR            |runsendid | len |offsetinbag|valrangetop|valrangebottom  */
    {SpaAddr_XiTong,          7,        29,     0,          0,           0      },
#if DUAL_SYSTEM
    {SpaAddr_DanMianDMBC,        8,         8,     0,        -200,         200    },
    {SpaAddr_SiPinDMBC,        9,         8,     0,        -200,         200    },
    {SpaAddr_LingWeiDMBC,     10,        8,     0,        -200,         200    },
#else
    {SpaAddr_DanMianDMBC,        8,         4,     0,        -200,         200    },
    {SpaAddr_SiPinDMBC,        9,         4,     0,        -200,         200    },
    {SpaAddr_LingWeiDMBC,     10,        4,     0,        -200,         200    },
#endif
    {SpaAddr_YaoCuang,        11,        33,    0,        -500,         500    },
    {SpaAddr_FanZenYC,      12,        16,    0,        -500,         500    },
    {SpaAddr_FanZenZYC,    13,        16,    0,        -500,         500    },
    {SpaAddr_FanZenFYC,    14,        16,    0,        -500,         500    },
    {SpaAddr_JiQiGongZuo ,        1,        50,    0,         0,            0,   }
};


QParam::QParam(QComm *send, QObject *parent):
    QObject(parent),spabuf(NULL),pcomm(send)
{
}

QParam::~QParam(){
    if(spafile)
        spafile->flush();
}

int QParam::spaItemValBottom(QParam::SpaItemHd hd)
{
    return  spaItemDsp[hd].valrangebottom;
}

int QParam::spaItemValTop(QParam::SpaItemHd hd)
{
     return  spaItemDsp[hd].valrangetop;
}

bool QParam::setFile(const QString &spafilepath){
    spafile = QSharedPointer<QFile>(new QFile(spafilepath));
    if(!spafile->exists()){
        ERRORLOG("SPA文件不存在");
        qWarning("in data.cpp QParam::setFile spafile not exist");
        spafile.clear();
        return FALSE;
    }
    if(!spafile->open(QIODevice::ReadOnly))
        return FALSE;
    this->spafilepath = spafilepath;
    QFileInfo spafileinfo(*spafile);
    spafilename = spafileinfo.fileName();
    spabuf =  (short *)(spafile->map(0,spafile->size()));
    emit changed();
    return TRUE;
}


short QParam::fechData(SpaItemHd handle,int offset){
    if(spabuf){
        unsigned short addr = spaItemDsp[handle].addr;
        return spabuf[addr+offset];
    }
    return 0;
}

void QParam::setData(SpaItemHd handle,int offset,short data,bool emitdirty){
    if(spabuf){
        unsigned short addr = spaItemDsp[handle].addr;
        spabuf[addr+offset] = data;
        modifyedItem.insert(handle);
        if(emitdirty)
            emit dirty(TRUE);
    }
    return;
}

#if DUAL_SYSTEM
void QParam::setDankouLock(bool lock){
    setData(SpaItemHd_Xtcs,20,lock,TRUE);
}
#endif


int QParam::sendParama()
{
    if(!spabuf)
        return Md::ParamaError;
    if(!patternData.wrkbuf)
        return Md::PatternError;
    return pcomm->SendParama((char *)(patternData.wrkbuf),(char *)spabuf,0xffffffff);
}

unsigned short  QParam::duMuZhiBuGongZuo(int row,Md::POS_LEFTRIGHT kou,Md::POSFLAG_FRONTREAR fr){
#if DUAL_SYSTEM
    unsigned char rfoffset = (fr==Md::POSREAR)?0:4;
    unsigned char kouoffset = (kou==Md::POSLEFT)?0:2;
    unsigned char rowoffset = (row%2)?0:1;
    return fechData(SpaItemHd_Bgzdmlwbc,rfoffset+kouoffset+rowoffset);
#else
    Q_UNUSED(kou);
    unsigned char rfoffset = (rf==Md::POSREAR)?0:2;
    unsigned char rowoffset = (row%2)?0:1;
    return fechData(SpaItemHd_Bgzdmlwbc,rfoffset+rowoffset);
#endif
}

int QParam::updata(SpaItemHd hd){
    const SpaItemDsp dsp = spaItemDsp[hd];
    if(hd==SpaItemHd_Jqgzcs){
        unsigned short buf[dsp.len];
        for(int i=0;i<dsp.len;i++)
            buf[i] = spabuf[dsp.addr+i];
        const QPattern::WrkItemDsp qizendiandsp =QPattern::wrkItemDsp[QPattern::WrkItemHd_QiZenDian] ;
        const QPattern::WrkItemDsp ZanKaiPianSudsp =QPattern::wrkItemDsp[QPattern::WrkItemHd_ZanKaiPianSu] ;
        buf[qizendiandsp.offsetinbag] = patternData.wrkbuf[qizendiandsp.addr];
        buf[ZanKaiPianSudsp.offsetinbag] = patternData.wrkbuf[ZanKaiPianSudsp.addr];
        return pcomm->paramaUpdata(qizendiandsp.runsendid,buf,dsp.len,TRUE);
    }else{
        return pcomm->paramaUpdata(dsp.runsendid,(unsigned short *)(spabuf+dsp.addr),dsp.len,TRUE);
    }
}

int QParam::updataPivotal(){
    unsigned char yiyincunzs =(unsigned char) fechData(SpaItemHd_Xtcs,7);
    unsigned short chijvjiaozeng = fechData(SpaItemHd_Xtcs,11);
    unsigned short hengjizhenshu = fechData(SpaItemHd_Xtcs,12);
    return pcomm->paramaUpdata(yiyincunzs,chijvjiaozeng,hengjizhenshu);
}

int QParam::updataAll(){
    QSet<SpaItemHd>::Iterator iterator = modifyedItem.begin();
    int r=Md::Ok;
    while(iterator!=modifyedItem.end()){
        r = updata(*iterator);
        if(r!=Md::Ok)
            break;
        iterator = modifyedItem.erase(iterator);
    }
    emit dirty(!modifyedItem.isEmpty());
    return Md::Ok ;
}


