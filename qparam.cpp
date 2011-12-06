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



short QParam::fechData(ItemHd handle,int offset){
    if(spabuf){
        unsigned short addr = ITEM_DSP[handle].addr;
        return spabuf[addr+offset];
    }
    return 0;
}

void QParam::setData(ItemHd handle,int offset,short data){
    if(spabuf){
        unsigned short addr = ITEM_DSP[handle].addr;
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

void QParam::save(bool isdownload){
    if(!spabuf||!modifyedItem.isEmpty())
        return ;
    spafile->open(QIODevice::ReadWrite);
    foreach(ItemHd handle,modifyedItem)   {
        unsigned addr  = ITEM_DSP[handle].addr;
        unsigned len = ITEM_DSP[handle].len;
        spafile->seek(addr*2);
        spafile->write((char *)&spabuf[addr],len*2);
    }
    spafile->flush();
    spafile->close();
    emit dirty(FALSE);
    emit changed();
    if(isdownload){
        unsigned short bagflag =0;
        foreach(ItemHd handle,modifyedItem){
            bagflag|=1<<(ITEM_DSP[handle].bag-1);
        }
        qsend->SendParama(*patternData.wrkfile,*spafile,bagflag,NULL);
    }
}


