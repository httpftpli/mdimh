#include "qparam.h"
#include <QFileInfo>
#include "qsyslog.h"
#include "globaldata.h"
#include "communicat.h"
#include "pattern.h"




QParam::QParam(QComm *send, QObject *parent):
    QObject(parent),spabuf(NULL),pcomm(send)
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
    if(!spabuf)
        spabuf = new short[LENOFSPA];
    spafile->open(QIODevice::ReadOnly);
    int num = spafile->read((char *)spabuf,LENOFSPA);
    if((unsigned int)num<sizeof LENOFSPA)
        qWarning("len of spafile shorter than LENOFSPA");
    spafile->close();
    modifyedItem.clear();
}


void  QParam::releaseBuf(){
    delete [] spabuf;
    modifyedItem.clear();
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

unsigned short  QParam::duMuZhiBuGongZuo(int row,Md::POS_LFETRIGHT kou,Md::POSFLAG_FRONTREAR fr){
#if DUAL_SYSTEM
    unsigned char rfoffset = (fr==Md::POSREAR)?0:4;
    unsigned char kouoffset = (kou==Md::POSLEFT)?0:2;
    unsigned char rowoffset = (row%2)?0:1;
    return dumu_bugongzuo[rfoffset+kouoffset+rowoffset];
#else
    Q_UNUSED(kou);
    unsigned char rfoffset = (rf==Md::POSREAR)?0:2;
    unsigned char rowoffset = (row%2)?0:1;
    return dumu_bugongzuo[rfoffset+rowoffset];
#endif
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
    if(hd==SpaItemHd_Jqgzcs){
        unsigned short buf[dsp.len];
        for(int i=0;i<dsp.len;i++)
            buf[i] = spabuf[dsp.addr+i];
        const WrkItemDsp qizendiandsp =wrkItemDsp[WrkItemHd_QiZenDian] ;
        const WrkItemDsp ZanKaiPianSudsp =wrkItemDsp[WrkItemHd_ZanKaiPianSu] ;
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

int QParam::save(bool isdownload,bool isall){
    if(!spabuf||modifyedItem.isEmpty())
        return Md::Ok ;
    spafile->open(QIODevice::ReadWrite);
    foreach(SpaItemHd handle,modifyedItem){
        if(!isall){
            if((handle!=SpaItemHd_Fzycwzxz_z)&&(handle!=SpaItemHd_Fzycwzxz_f)&&
                    (handle!=SpaItemHd_Fzycwzxz)&&(handle!=SpaItemHd_Ycwzxz)&&
                    (handle!=SpaItemHd_Xtcs)) //Fzycwz,Ycwzxz,Xtcs is not need to click save ,
                                              //it will be saved auto when cancle parama seting form
                continue;
        }
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
        if(modifyedItem.contains(SpaItemHd_Xtcs)){
            if(updata(SpaItemHd_Xtcs)==Md::Ok)
                modifyedItem.remove(SpaItemHd_Xtcs);
            else
                return Md::CommError;
        }
        if(modifyedItem.contains(SpaItemHd_Fzycwzxz_z))
            modifyedItem.remove(SpaItemHd_Fzycwzxz_z);
        if(modifyedItem.contains(SpaItemHd_Fzycwzxz_f))
            modifyedItem.remove(SpaItemHd_Fzycwzxz_f);
        if(modifyedItem.contains(SpaItemHd_Fzycwzxz))
            modifyedItem.remove(SpaItemHd_Fzycwzxz);
        if(modifyedItem.contains(SpaItemHd_Ycwzxz))
            modifyedItem.remove(SpaItemHd_Ycwzxz);
        if(isall){
            foreach(SpaItemHd hd,modifyedItem){
                int r = updata(hd);
                if(r!=Md::Ok)
                    return r;
            }
        }
    }
    modifyedItem.clear();
    return Md::Ok;
}


