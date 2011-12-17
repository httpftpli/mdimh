#include "qparam.h"
#include <QFileInfo>
#include "qsyslog.h"
#include "globaldata.h"
#include "communicat.h"
#include "pattern.h"




QParam::QParam(QSend *send, QObject *parent):
    QObject(parent),spabuf(NULL),qsend(send)
{

}


QParam::~QParam(){
    if(spabuf){
        delete [] spabuf;
        spabuf = NULL;
    }
}

bool QParam::setFile(const QString &spafilepath){
    spafile = QSharedPointer<QFile>(new QFile(spafilepath));
    if(spabuf){
        delete [] spabuf;
        spabuf = NULL;
    }
    if(!spafile->exists()){
        ERRORLOG("SPA文件不存在");
        qWarning("in data.cpp QParam::setFile spafile not exist");
        spafile.clear();
        return FALSE;
    }
    spafile->open(QIODevice::ReadOnly);
    this->spafilepath = spafilepath;
    QFileInfo spafileinfo(*spafile);
    spafilename = spafileinfo.fileName();
    spafile->seek(Addr_Bgzdmlwbc*2);
    spafile->read((char *)dumu_bugongzuo,sizeof dumu_bugongzuo);
    spafile->close();
    emit changed();
    return TRUE;
}

void  QParam::loadFile(){
    if(!spafile)
        return;
    spafile->open(QIODevice::ReadOnly);
    spabuf = new short[LENOFSPA];
    int num = spafile->read((char *)spabuf,LENOFSPA);
    if((unsigned int)num<sizeof LENOFSPA)
        qWarning("len of spafile shorter than LENOFSPA");
    spafile->close();
}


void  QParam::releaseBuf(){
    delete [] spabuf;
    spabuf = NULL;
}



short QParam::fechData(SpaItemHd handle,int offset){
    if(spabuf){
        unsigned short addr = spaItemDsp[handle].addr;
        return spabuf[addr+offset];
    }
    return 0;
}

#if DUAL_SYSTEM
void QParam::setDankouLock(bool lock){
   Q_ASSERT(spafile);
   if(spabuf)
       spabuf[Addr_Xtcs+20] = lock;
   bool open = spafile->isOpen();
   if(!open)
       spafile->open(QIODevice::ReadWrite);
   spafile->seek((Addr_Xtcs+20)*2);
   unsigned short temp = lock;
   spafile->write((char *)&temp,2);
   if(!open)
       spafile->close();
}
#endif

void QParam::setData(SpaItemHd handle,int offset,short data){
    if(spabuf){
        unsigned short addr = spaItemDsp[handle].addr;
        spabuf[addr+offset] = data;
        modifyedItem.insert(handle);
        emit dirty(TRUE);
    }
    return;
}

unsigned short  QParam::duMu_BuGongZuo(HeadPosDir posdir){
    return dumu_bugongzuo[posdir];
}

void QParam::refreshBuf(){
    if(spabuf){
        spafile->open(QIODevice::ReadOnly);
        spafile->read((char *)spabuf,LENOFSPA);
        spafile->close();
        emit dirty(FALSE);
    }
    return;
}

int QParam::updata(SpaItemHd hd){
    const SpaItemDsp dsp = spaItemDsp[hd];
    if(hd==SpaItemHd_Xtcs){
        unsigned short buf[dsp.len];
        for(int i=0;i<dsp.len;i++)
            buf[i] = spabuf[dsp.addr+i];
        const WrkItemDsp qizendiandsp =wrkItemDsp[WrkItemHd_QiZenDian] ;
        const WrkItemDsp ZanKaiPianSudsp =wrkItemDsp[WrkItemHd_ZanKaiPianSu] ;
        buf[qizendiandsp.offsetinbag] = patternData.wrkbuf[qizendiandsp.addr];
        buf[ZanKaiPianSudsp.offsetinbag] = patternData.wrkbuf[ZanKaiPianSudsp.addr];
        return qsend->paramaUpdata(qizendiandsp.runsendid,buf,dsp.len,TRUE);
    }else{
        return qsend->paramaUpdata(dsp.runsendid,(unsigned short *)(spabuf+dsp.addr),dsp.len,TRUE);
    }
}

int QParam::save(bool isdownload){
    if(!spabuf||!modifyedItem.isEmpty())
        return Md::Ok ;
    spafile->open(QIODevice::ReadWrite);
    foreach(SpaItemHd handle,modifyedItem)   {
        unsigned addr  = spaItemDsp[handle].addr;
        unsigned len = spaItemDsp[handle].len;
        spafile->seek(addr*2);
        spafile->write((char *)&spabuf[addr],len*2);
    }
    spafile->flush();
    spafile->close();
    emit dirty(FALSE);
    emit changed();
    if(isdownload){
        foreach(SpaItemHd hd,modifyedItem){
            int r = updata(hd);
            if(r!=Md::Ok)
                return r;
        }
    }
    modifyedItem.clear();
    return Md::Ok;
}


