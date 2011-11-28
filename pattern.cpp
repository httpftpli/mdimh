#include "pattern.h"
#include <QDataStream>
#include <QFileInfo>
#include "globaldata.h"
#include "qhmidata.h"
#include "qparam.h"

QPatternData::QPatternData(QSend *s,QObject *parent):QObject(parent),
                            shaZuiKb(0),cntbuf(NULL),
                            patfile(NULL), wrkfile(NULL),sazfile(NULL),
                            cntfile(NULL),patbuf(NULL),dumu_history(24),
                            shazuiused_r(0),shazuiused_l(0),qsend(s){

}



QPatternData::~QPatternData(){
    if(cntbuf!=NULL){
        delete [] cntbuf;
        cntbuf =NULL;
    }
    if(patbuf!=NULL){
        delete [] patbuf;
        patbuf = NULL;
    }
    return;
}



QPatternData::Result QPatternData::setFile(const QString &cntfilepath,const QString &patfilepath,
                                           const QString &wrkfilepath,
                                           const QString &sazfilepath,
                                           Md::HAVEFILEFLAG openflag){
    INFORMLOG(tr("扫描花型文件"));
    cntfile.clear();
    patfile.clear();
    wrkfile.clear();
    sazfile.clear();
    sazbuf.clear();
    this->shazuiused_r =0;
    this->shazuiused_l =0;

    this->tatalcolumn = 0;
    this->tatalrow = 0;

    //检查cntfile patfile wrkfile文件是否可用//////////////
    QSharedPointer<QFile> cntfile,patfile,wrkfile;
    cntfile = QSharedPointer<QFile>(new QFile(cntfilepath));
    if(!cntfile->exists()){
       WARNLOG(tr("无花型文件，找不到cnt文件"));
       return NoCntFile;
    }
    patfile = QSharedPointer<QFile>(new QFile(patfilepath));
    if(!patfile->exists()){
       WARNLOG(tr("无花型文件，找不到pat文件"));
       return NoPatFile;
    }
    wrkfile = QSharedPointer<QFile>(new QFile(wrkfilepath));
    if(!wrkfile->exists()){
        INFORMLOG(tr("花型WRK文件不存在，使用系统的WRK文件"));
        wrkfile->setFileName(hmiData.wrkFilePath);
    }
    QFileInfo wrkfileinfo(*wrkfile);
    wrkFileName = wrkfileinfo.fileName();
    unsigned char shazuir=0,shazuil=0,shazui=0,shazuinew=0;
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);

    //检查花型格式,读取花型尺寸，沙嘴使用
    unsigned short wight,hight;
    patfile->open(QIODevice::ReadOnly);
    stream.setDevice(patfile.data());
    stream>>wight>>hight;     //读取花型尺寸
    patfile->close();
    if((wight==0)||(hight==0)){
        ERRORLOG(tr("花型文件格式错误"));
        return  PatFileError;
    }
    if(patfile->size()<(wight/2+wight%2)*(hight+1)){
        ERRORLOG(tr("花型文件格式错误，文件长度不匹配"));
        return  PatFileError;
    }
    if(cntfile->size()<128*hight){
        ERRORLOG(tr("pat文件和cnt文件不匹配"));
        return  CntPatNotMatch;
    }
    cntfile->open(QIODevice::ReadOnly);
    stream.setDevice(cntfile.data());
    for(int i=1;i<=hight;i++){  //读取原始沙嘴
        unsigned short _s1,_s2;
        unsigned char s1,s2,s,snew;
            cntfile->seek(i*128+80);
            stream>>_s1;
            s1 = (unsigned char)(_s1>>8);
            cntfile->seek(i*128+83);
            stream>>_s2;
            s2 = (unsigned char)(_s1>>8);
            s = s2|s1;//所在行的沙嘴（包系统1和系统2）
            shazuinew = shazui|s;
            snew = shazuinew^shazui;
            shazui = shazuinew;
            i%2?(shazuil|=snew):(shazuir|=snew);
    }
    cntfile->close();

    this->cntfile = cntfile;
    this->patfile = patfile;
    this->wrkfile = wrkfile;
    this->cntFilePath = cntfilepath;
    this->patFilePath = patfilepath;
    //根据传入的最后一个参数，决定文件是处于关闭状态还是打开状态


    /////////////////////////////////////

    QFileInfo cntfileinfo(*cntfile);
    QString name = cntfileinfo.fileName();
    patternDir = cntfileinfo.absolutePath();
    this->patternName = name.left(name.size()-4);
    this->shazuiused_l = shazuil;
    this->shazuiused_r = shazuir;
    this->tatalrow = hight;
    this->tatalcolumn = wight;

    //检查沙嘴捆绑
    sazfile = QSharedPointer<QFile>(new QFile(sazfilepath));
    if(!sazfile->exists()){
        sazfile->setFileName(patternDir+'/'+patternName+".saz");
    }else{
        if(sazfile->size()==512){
            sazfile->open(QIODevice::ReadOnly);
            stream.setDevice(sazfile.data());
            SzkbData temp;
            while(!stream.atEnd()){
                stream>>temp.ZuSa>>temp.FuSa>>temp.Start>>temp.End;
                if(temp.ZuSa==0)
                    break;
                shaZuiKb|=1<<(temp.FuSa-1);
                sazbuf<<temp;
            }
            sazfile->close();
        }
        else
            WARNLOG(tr("沙嘴捆绑文件格式错误，文件大小不等于512字节,没有加载沙嘴捆绑数据"));
    }
    emit patternChanged(patternName,cntfilepath,patfilepath,wrkfilepath,sazfilepath);
    return OK;
}



Md::HAVEFILEFLAG QPatternData::loadFile(Md::HAVEFILEFLAG flag){
    Md::HAVEFILEFLAG r = loadStatus();
    if((flag&Md::HAVECNT)&&cntfile&&!cntbuf){
        cntbuf = new unsigned char[128*tatalrow];
        cntfile->open(QIODevice::ReadOnly);
        cntfile->seek(128);
        cntfile->read((char *)(this->cntbuf),128*tatalrow);
        cntfile->close();
        r |=Md::HAVECNT;
    }

    if((flag&Md::HAVEPAT)&&patfile&&!patbuf){
        patbuf = new unsigned char[(tatalcolumn/2+tatalcolumn%2)*tatalrow];
        patfile->open(QIODevice::ReadOnly);
        patfile->seek(tatalcolumn/2+tatalcolumn%2);
        patfile->read((char *)patbuf,(tatalcolumn/2+tatalcolumn%2)*tatalrow);
        patfile->close();
        r |=Md::HAVEPAT;
    }
    if((flag&Md::HAVEWRK)&&wrkfile&&!wrkbuf){
        wrkbuf = new unsigned short[2500];
        wrkfile->open(QIODevice::ReadOnly);
        wrkfile->read((char *)wrkbuf,wrkfile->size());
        wrkfile->close();
        r |=Md::HAVEWRK;
    }
    return r;
}

void QPatternData::patSetData(unsigned short row,unsigned short column,unsigned char cha){
    unsigned short cofbuf = tatalcolumn/2+tatalcolumn%2;
    char data =patbuf[(row*cofbuf)+column/2];
    char datacha= (column%2)?(data&0x0f):((data&0xf0)>>4);
    if(datacha!=cha){
        data = (column%2)?((data&0xf0)+(cha&0x0f)):((data&0x0f)+((unsigned char)cha<<4));
        patbuf[(row*cofbuf)+column/2]= data;
        patfile->open(QIODevice::ReadOnly);
        patfile->seek((row+1)*cofbuf);
        char tempdata[cofbuf];
        patfile->read(tempdata,cofbuf);
        int dif = memcmp(tempdata,&patbuf[row*cofbuf],cofbuf);
        if(dif)
            patModifiedRow.insert(row);
        else
            patModifiedRow.remove(row);
        emit patDirty(!patModifiedRow.isEmpty());
        patfile->close();
    }
}

void QPatternData::cntSetData(unsigned short row,unsigned char index,unsigned short data){

}



void QPatternData::refreshBuf(Md::HAVEFILEFLAG flag){
    if((flag&Md::HAVEPAT)&&patbuf&&(!patModifiedRow.isEmpty())){
        patfile->open(QIODevice::ReadOnly);
        int c = tatalcolumn/2+tatalcolumn%2;
        foreach(unsigned short row,patModifiedRow){
            patfile->seek((row+1)*c);
            patfile->read((char *)&patbuf[row*c],c);
        }
        patfile->close();
        patModifiedRow.clear();
        emit patDirty(FALSE);
    }
    if((flag&Md::HAVEWRK)&&wrkbuf&&(!wrkModifiedHandle.isEmpty())){
        wrkfile->open(QIODevice::ReadOnly);
        foreach(WrkItemHd handle,wrkModifiedHandle){
            wrkfile->seek(wrkItemDsp[handle].addr*2);
            wrkfile->read((char *)&wrkbuf[wrkItemDsp[handle].addr],wrkItemDsp[handle].len*2);
        }
        wrkfile->close();
        wrkModifiedHandle.clear();
        emit wrkDirty(FALSE);
    }
    if((flag&Md::HAVECNT)&&cntbuf&&(!cntModifiedRow.isEmpty())){
        cntfile->open(QIODevice::ReadOnly);
        foreach(unsigned short row,cntModifiedRow){
            cntfile->seek((row+1)*128);
            cntfile->read((char*)&cntbuf[128*row],128);
        }
        patfile->close();
        cntModifiedRow.clear();
        emit cntDirty(FALSE);
    }
}

void QPatternData::deloadFile(Md::HAVEFILEFLAG flag){
    if((flag&Md::HAVECNT)&&cntbuf){
        delete []cntbuf;
        cntbuf = NULL;
    }
    if((flag&Md::HAVEPAT)&&patbuf){
        delete []patbuf;
        patbuf = NULL;
    }
    if((flag&Md::HAVEWRK)&&wrkbuf){
        delete []wrkbuf;
        wrkbuf = NULL;
    }
}

Md::HAVEFILEFLAG QPatternData::loadStatus(){
    Md::HAVEFILEFLAG r = Md::HAVENO;
    if(cntbuf){
        r|=Md::HAVECNT;
    }
    if(patbuf){
        r|=Md::HAVEPAT;
    }
    if(wrkbuf){
        r|=Md::HAVEWRK;
    }
    return r;
}


QPatternData::Result QPatternData::loadLoop(const QString &prmfilepath){

}

void QPatternData::Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag){
    if(saveflag&Md::HAVESAZ){
        if(!sazbuf.isEmpty()){
            sazfile->open(QIODevice::ReadWrite);
            sazfile->resize(512);
            int filelen = 0;
            QDataStream stream(sazfile.data());
            stream.setByteOrder(QDataStream::LittleEndian);
            foreach(SzkbData data,sazbuf){
                stream<<data.ZuSa<<data.FuSa<<data.Start<<data.End;
                filelen = filelen+8;
            }
            while(filelen<512){
                stream<<(long long)0;
                filelen = filelen+8;
            }

            sazfile->close();
        }
    }
    if(saveflag&Md::HAVEPAT){
        if(!patModifiedRow.isEmpty()){
            patfile->open(QIODevice::ReadWrite);
            foreach(unsigned short row,patModifiedRow){
                int c = tatalcolumn/2+tatalcolumn%2;
                patfile->seek((row+1)*c);
                patfile->write((char *)&(patbuf[row*c]),c);
                if(downloadflag&Md::HAVEPAT){
                     qSend.patUpdate(row+1,&patbuf[c*row],c); //download to mainboard
                }
            }
            patfile->flush();
            patfile->close();
            patModifiedRow.clear();
            emit patDirty(FALSE);
        }
    }
    if(saveflag&Md::HAVEWRK){
        if(!wrkModifiedHandle.isEmpty()){
            wrkfile->open(QIODevice::ReadWrite);
            QSet<unsigned char> bags;
            foreach(WrkItemHd handle,wrkModifiedHandle){
                bags.insert(wrkItemDsp[handle].bag);
                wrkfile->seek(wrkItemDsp[handle].addr*2);
                wrkfile->write((char *)&wrkbuf[wrkItemDsp[handle].addr],wrkItemDsp[handle].len*2);
            }
            wrkfile->flush();
            wrkfile->close();
            if(downloadflag&Md::HAVEWRK){
                int i = 0;
                foreach(unsigned char bag, bags){
                    i= i|(1<<(bag-1));
                }
                qSend.SendParama(*wrkfile,*paramaData.spafile,i,NULL);
            }
            wrkModifiedHandle.clear();
            emit wrkDirty(FALSE);
        }
    }
    return;
}


short QPatternData::wrk_fechData(WrkItemHd handle,int offset){
    int addr = wrkItemDsp[handle].addr;
    return wrkbuf[offset+addr];
}

void QPatternData::wrk_setData(WrkItemHd handle,int offset,short data){
    int addr = wrkItemDsp[handle].addr;
    if(wrkbuf[addr+offset]!=data){
        wrkbuf[addr+offset] = data;
        wrkModifiedHandle.insert(handle);
        emit wrkDirty(TRUE);
    }
}


QSize QPatternData::patternSize()const{
    return QSize(tatalcolumn,tatalrow);
}

bool QPatternData::isValid () const{
    //return isValid();
    return TRUE;
}

/*unsigned char QPatternData::cnt_fechData(unsigned short row,unsigned char addr){
   if(cntbuf)
     return this->cntbuf[row*128+addr];
   else{
       cntfile->open(QIODevice::ReadOnly);


     return 0;
}*/

unsigned short QPatternData::cnt_fechData(unsigned short row,unsigned char addr,unsigned short len){
    if(len>2)
        return 0xff;
    if(cntbuf!=NULL){
        if(len==1)
            return this->cntbuf[row*128+addr];
        if(len==2)
            return *(unsigned short *)&(cntbuf[(row)*128+addr]);
    }else{
        if(!cntfile->open(QIODevice::ReadOnly))
            return 0xff;
        char val[2];
        cntfile->seek((row+1)*128+addr);
        cntfile->read(val,len);
        cntfile->close();
        return *(unsigned short *)val;
    }
    return 0xff;
}

unsigned char QPatternData::shaZui(unsigned short row){
    unsigned char cnt_sz = cnt_shaZui1(row);
    unsigned char sz = cnt_sz;
    foreach(SzkbData data,sazbuf){
        if((data.Start>=row+1)&&(data.End<=row+1)){
            if(cnt_sz&(1<<data.ZuSa))
                sz  = sz|(1<<data.FuSa);
        }
    }
    return sz;
}


unsigned char QPatternData::cnt_shaZui1(unsigned short row){
   unsigned short sz1 = cnt_fechData(row,CNT_S1_SaZui,2);
   unsigned  char szh1 = (unsigned char)(sz1>>8);
   if((sz1&0x00ff)==0x80){
       return szh1;
   }
   return 0;
}

unsigned char QPatternData::cnt_shaZui2(unsigned short row){
   unsigned short sz2 = cnt_fechData(row,CNT_S2_SaZui,2);
   unsigned  char szh2 = (unsigned char)(sz2>>8);
   if((sz2&0x00ff)==0x80){
       return szh2;
   }
   return 0;
}


unsigned char QPatternData::cnt_duMu(unsigned short row,bool &doubleOrSigle){
    unsigned char dumu = cnt_fechData(row,CNT_DuMu);
    if(dumu==0){
        //如果度目是0，当非空时取上次有效的度目，如果是空取第24档
        if(cnt_fechData(row,0)==0)
            dumu = 24;
        else
            dumu = dumu_history;
        doubleOrSigle = FALSE;
        return dumu;
    }else if(dumu<25){
        dumu_history = dumu;
        doubleOrSigle = FALSE;
        return dumu;
    }else if(dumu<101){
        WARNLOG(QString("第%1行，度目=%2，按双面第一组取度目值").arg((row+1),dumu));
        doubleOrSigle = TRUE;
        return 1;
    }else if(dumu<125){
        dumu_history = dumu;
        doubleOrSigle = TRUE;
        return dumu-100;
    }else{
        WARNLOG(QString("第%1行，度目=%2，按双面第24组取度目值").arg((row+1),dumu));
        doubleOrSigle = TRUE;
        return 24;
    }
}


unsigned char QPatternData::duMuZhi(bool backorfront ,unsigned short row){
    unsigned char dumu;
    bool dors;
    dumu = cnt_duMu(row,dors)-1;
    if(backorfront)
        return row%2?wrk_fechData(WrkItemHd_DuMuZi,dumu*4+3):
                wrk_fechData(WrkItemHd_DuMuZi,dumu*4+1);
    else
        return row%2?wrk_fechData(WrkItemHd_DuMuZi,dumu*4+4):
                wrk_fechData(WrkItemHd_DuMuZi,dumu*4+2);
}



unsigned char QPatternData::cnt_yaJiao(unsigned short row){
    return this->cnt_fechData(row,CNT_YaJiaoDuan);
}

unsigned char QPatternData::cnt_tingChe(unsigned short row){
    return this->cnt_fechData(row,CNT_TingCe);
}


unsigned short QPatternData::cnt_huabanhang_q1(unsigned short row){
     return this->cnt_fechData(row,CNT_S1Q_Pat);
}

unsigned short QPatternData::cnt_huabanhang_h1(unsigned short row){
     return this->cnt_fechData(row,CNT_S1H_Pat);
}

unsigned short QPatternData::cnt_huabanhang_q2(unsigned short row){
     return this->cnt_fechData(row,CNT_S2Q_Pat);
}

unsigned short QPatternData::cnt_huabanhang_h2(unsigned short row){
    return this->cnt_fechData(row,CNT_S2H_Pat);
}

unsigned short QPatternData::wrk_qizhengdian(){
    return this->wrk_fechData(WrkItemHd_QiZenDian,0);
}

QString QPatternData::cnt_seDaiHao(unsigned short row,unsigned char inc){
    unsigned short data= cnt_fechData(row,inc,2);
    QString str;
    for(int i=0;i<16;i++){
        if(data&(1<<i))
            str.append(QString::number(i,16).toUpper());
    }
    return str;
}


QString QPatternData::cnt_yaoChuang(unsigned short row){ 
    unsigned char ycfx = cnt_fechData(row,CNT_YaoCuangDir);
    unsigned char ycz = cnt_fechData(row,CNT_YaoCuangZi);

    QString str ="";
    if(ycfx>=0xc0)
        str.append("L");
    else
        str.append("R");
    str.append(QString::number(ycz));
    if(ycfx&0x01)
        str.append("*");
    return str;
}

unsigned char QPatternData::cnt_spead(unsigned short row){
    return this->cnt_fechData(row,CNT_SuDu);
}

unsigned char QPatternData::cnt_mainLuola(unsigned short row){
    return this->cnt_fechData(row,CNT_JuanBu);
}

unsigned char QPatternData::cnt_fuzuLuola(unsigned short row){
    return this->cnt_fechData(row,CNT_FuJuanBu)-0x80;
}

unsigned char QPatternData::cnt_songSha(unsigned short row){
    return this->cnt_fechData(row,CNT_FuJuanBu)-0x80;
}

unsigned char QPatternData::cnt_shazuiTf(unsigned short row){
    return this->cnt_fechData(row,CNT_SaZuoTingFang);
}





int QPatternData::_azl(unsigned char data){
    int index=0;
    switch(data){
    case 0x00:
    case 0x0a:
    case 0x0b:
        index = 0;//kong
        break;
    case 0x01:
        index = 1;//fanzhen;
         break;
    case 0x07:
    case 0x08:
    case 0x0d:
    case 0x0e:
    case 0x0f:
        index = 2;//bianzhi;
         break;
    default:
         index = 0;
         break;
    }
    return index;
}

int QPatternData::_hzl(unsigned char data){
    int index=0;
    switch(data){
    case 0x00:
    case 0x08:
    case 0x01:
        index = 0;//kong
        break;
    case 0x0d:
    case 0x0a:
        index = 1;//diaomu
        break;
    case 0x0b:
    case 0x0e:
        index = 3;//diaomu2
        break;
    case 0x07:
        index = 2;//jiezhen
        break;
    case 0x0f:
        index = 4;//biansong2
        break;
    default:
        index = 0;
        break;
    }
    return index;
}

