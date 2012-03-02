#include "pattern.h"
#include <QDataStream>
#include <QFileInfo>
#include "qhmidata.h"
#include "qparam.h"
#include <QDir>

QPatternData::QPatternData(QComm *s,QObject *parent):QObject(parent),
                            shaZuiKb(0),cntbuf(NULL),
                            patfile(NULL), wrkfile(NULL),
                            cntfile(NULL),patbuf(NULL),dumu_history_sys1(1),
                            dumu_history_sys2(1),
                            shazuiused_r(0),shazuiused_l(0),pcomm(s),
#if DUAL_SYSTEM
                            isdualdumuzu(FALSE),
#endif
    dumuzu_used(0){

}



QPatternData::~QPatternData(){
    if(cntbuf!=NULL){
        cntbuf=NULL;
    }
    if(patbuf!=NULL){
       patbuf = NULL;
    }
    return;
}



QPatternData::Result QPatternData::setFile(const QString &cntfilepath,const QString &patfilepath){
    INFORMLOG(tr("扫描花型文件"));
    cntfile.clear();
    patfile.clear();
    wrkfile.clear();
    sazbuf.clear();
    //delete [] cntbuf;
    //cntbuf = NULL;
    delete [] patbuf;
    patbuf = NULL;
    delete [] wrkbuf;
    wrkbuf = NULL;
    ispatternavalible = FALSE;
    this->shazuiused_r =0;
    this->shazuiused_l =0;
    this->tatalcolumn = 0;
    this->tatalcntrow = 0;
    this->tatalpatrow = 0;
    this->wrkFilePath = hmiData.sys_wrkFilePath;
    this->wrkfile = QSharedPointer<QFile>(new QFile(wrkFilePath));
    QFileInfo wrkfileinfo(*(this->wrkfile));
    this->wrkFileName = wrkfileinfo.fileName();
    this->iswrkfilesys = TRUE;
    this->dumuzu_used = 0;
#if DUAL_SYSTEM
    this->isdualdumuzu = FALSE;
#endif
    this->dumu_history_sys1 = 1;
    this->dumu_history_sys2 = 1;

    //检查cntfile patfile 文件是否可用//////////////
    QSharedPointer<QFile> cntfile,patfile;
    cntfile = QSharedPointer<QFile>(new QFile(cntfilepath));
    if(!cntfile->exists()){
       WARNLOG(tr("无花型文件，找不到cnt文件,使用系统WRK文件"));
       return NoCntFile;
    }
    patfile = QSharedPointer<QFile>(new QFile(patfilepath));
    if(!patfile->exists()){
       WARNLOG(tr("无花型文件，找不到pat文件,使用系统WRK文件"));
       return NoPatFile;
    }

    //检查花型格式,读取花型尺寸，沙嘴使用
    int wight,pathight,cnthight;
    patfile->open(QIODevice::ReadWrite);
    patbuf = patfile->map(0,patfile->size());
    if(patbuf==NULL){
        WARNLOG(tr("PAT文件打开错误"));
        return NoPatFile;
    }
    wight = *(unsigned short *)patbuf;
    pathight = *(unsigned short *)(patbuf+2);     //读取花型尺寸
    if((wight==0)||(pathight==0)){
        ERRORLOG(tr("花型文件格式错误"));
        return PatFileError;
    }
    if(patfile->size()<(wight/2+wight%2)*(pathight+1)){
        ERRORLOG(tr("花型文件格式错误，PAT文件长度不匹配"));
        return PatFileError;
    }

    cntfile->open(QIODevice::ReadWrite);
    unsigned int cntfilesize = cntfile->size();
    cntbuf = cntfile->map(0,cntfilesize);
    if(cntbuf==NULL){
        WARNLOG(tr("CNT文件打开错误"));
        return NoCntFile;
    }

    ispatternavalible = TRUE;

#if DUAL_SYSTEM
    //test isdualdumuzu according to d[0] and d[1] at cntfile
    if(((cntbuf[0]=='H')&&(cntbuf[1]=='Q'))
            ||((cntbuf[0]=='C')&&(cntbuf[1]=='X'))){
        cntbuf[0]='M';
        cntbuf[1]='D';
        isdualdumuzu = TRUE;
    }else if((cntbuf[0]=='M')&&(cntbuf[1]=='D')){
        isdualdumuzu = TRUE;
    }
#endif

    // scan pattern finish flag,scan shazui
    for(cnthight=1;;cnthight++){
        unsigned char sl,sr,d1;
        sl = cnt_shaZui(cnthight,Md::POSLEFT);
        sr = cnt_shaZui(cnthight,Md::POSRIGHT);
        shazuiused_l|=sl;
        shazuiused_r|=sr;
        d1 = cntbuf[cnthight*128+CNT_DuMu];
        if((d1<25)&&(d1!=0))
            dumuzu_used = dumuzu_used|(1<<(d1-1));
        else if((d1<125)&&(d1>100))
            dumuzu_used = dumuzu_used|(1<<(d1-100));
        if((1==cntbuf[cnthight*128+CNT_End])||((cnthight+1)*128>cntfile->size()))
            break;
    }
#if DUAL_SYSTEM
    if(isdualdumuzu){
        for(int i=1;i<=cnthight;i++){
            char d2 = cntbuf[cnthight*128+CNT_DuMu];
            if((d2<25)&&(d2!=0))
                dumuzu_used = dumuzu_used|(1<<(d2-1));
            else if((d2<125)&&(d2>100))
                dumuzu_used = dumuzu_used|(1<<(d2-100));
        }
    }
#endif

    cntfile->flush();
    this->cntfile = cntfile;
    this->patfile = patfile;
    this->cntFilePath = cntfilepath;
    this->patFilePath = patfilepath;
    QFileInfo cntfileinfo(*cntfile);
    QString name = cntfileinfo.fileName();
    this->patternName = name.left(name.size()-4);
    INFORMLOG(tr("花型正常，花型：")+patternName);
    patternDirPath = cntfileinfo.absolutePath();//doesn't include filename;
    this->tatalpatrow = pathight;
    this->tatalcntrow = cnthight;
    this->tatalcolumn = wight;
    this->patbyteperrow = wight/2+wight%2;

    //检查沙嘴捆绑
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);
    QDir patterndir(patternDirPath);
    QStringList filenamelist = patterndir.entryList(QStringList()<<(patternName+"*.SAZ"),QDir::Files);
    if(!filenamelist.isEmpty()){
        sazFilePath =  patternDirPath+'/'+filenamelist.at(0);
        QFile sazfile(sazFilePath);
        if(sazfile.size()!=512)
            sazfile.resize(512);
        sazfile.open(QIODevice::ReadOnly);
        stream.setDevice(&sazfile);
        SzkbData temp;
        while(!stream.atEnd()){
            stream>>temp.ZuSa>>temp.FuSa>>temp.Start>>temp.End;
            if(temp.ZuSa==0)
                break;
            shaZuiKb|=1<<(temp.FuSa-1);
            sazbuf<<temp;
        }
        sazfile.close();
    }


    QString wrkfilepath = patternDirPath+'/'+patternName+".WRK";
    wrkfile = QSharedPointer<QFile>(new QFile(wrkfilepath));
    if(wrkfile->exists()){
        this->wrkfile = wrkfile;
        this->iswrkfilesys = FALSE;
        this->wrkFileName = patternName+".WRK";
    }
    else{
        wrkfile->setFileName(patternDirPath+'/'+patternName+".wrk");
        if(wrkfile->exists()){
           this->wrkfile = wrkfile;
           this->iswrkfilesys = FALSE;
           this->wrkFileName = patternName+".wrk";
        }else{
           INFORMLOG(tr("花型WRK文件不存在，使用系统的WRK文件"));
        }
    }
    ispatternavalible = TRUE;
    emit patternChanged(patternName,this->cntFilePath,this->patFilePath,this->wrkFilePath,this->sazFilePath);
    return Ok;
}



Md::HAVEFILEFLAG QPatternData::loadFile(Md::HAVEFILEFLAG flag){
    Md::HAVEFILEFLAG r = loadStatus();
    if((flag&Md::HAVECNT)&&cntfile&&!cntbuf){
        cntbuf = new unsigned char[128*tatalcntrow];
        cntfile->open(QIODevice::ReadOnly);
        cntfile->seek(128);
        cntfile->read((char *)(this->cntbuf),128*tatalcntrow);
        cntfile->close();
        r |=Md::HAVECNT;
    }

    if((flag&Md::HAVEPAT)&&patfile&&!patbuf){
        patbuf = new unsigned char[(tatalcolumn/2+tatalcolumn%2)*tatalpatrow];
        patfile->open(QIODevice::ReadOnly);
        patfile->seek(tatalcolumn/2+tatalcolumn%2);
        patfile->read((char *)patbuf,(tatalcolumn/2+tatalcolumn%2)*tatalpatrow);
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

bool QPatternData::isWrkfileSys()
{
    return this->iswrkfilesys;
}

bool QPatternData::isPatternAvailable()
{
    return this->ispatternavalible;
}

void QPatternData::pat_SetData(int row,unsigned short column,unsigned char cha){
    if(!patbuf)
        return;
    unsigned char patlinebuf[1024];
    qMemCopy(patlinebuf,cntbuf+row*patbyteperrow,patbyteperrow);
    char datacha= pat_FechData(row,column);
    if(datacha!=cha){
        //charactor to int
        if((cha<='9')&&(cha>='0')){
            cha = cha-'0';
        }else if((cha<='f')&&(cha>='a')){
            cha = cha-'a'+10;
        }else if((cha<='F')&&(cha>='A')){
            cha = cha-'A'+10;
        }else
            cha = 0;
        //imerge to byte;
        char data =patbuf[(row*patbyteperrow)+column/2];
        data = (column%2)?((data&0xf0)+(cha&0x0f)):((data&0x0f)+((unsigned char)cha<<4));
        //write back
        patbuf[(row*patbyteperrow)+column/2]= data;
        //download
        if(pcomm->isAvailable()){
            int r = pcomm->patUpdate(row,patbuf+row*patbyteperrow,patbyteperrow);
            if(r!=Md::Ok)
                qMemCopy(cntbuf+row*patbyteperrow,patlinebuf,patbyteperrow);
        }
    }
}


/*changed cntdatabuf,but not change cntfile and compare to data in cntfile*/
void QPatternData::cnt_SetData(int row,unsigned char index,unsigned short data,unsigned char len){
    if(!cntbuf)
        return;
    Q_ASSERT(index<128);
    if(len==1)
         cntbuf[row*128+index]=(char)data;
    else if(len==2)
        *(short*)(cntbuf+row*128+index) = data;
    char buf[128];
    cntfile->open(QIODevice::ReadOnly);
    cntfile->seek((row+1)*128);
    cntfile->read(buf,128);
    int dif = memcmp(buf,&cntbuf[row*128],128);
    if(dif)
        cntModifiedRow.insert(row);
    else
        cntModifiedRow.remove(row);
    emit cntDirty(!cntModifiedRow.isEmpty());
    cntfile->close();
}

inline  char QPatternData::pat_FechData(int row, unsigned short column)
{
    if(patbuf){
        unsigned char data = patbuf[row*patbyteperrow+column/2];
        char ch  = (column%2)?data&0x0f:data>>4;
        if((ch>=0)&&(ch<=9)){
            ch = ch+'0';
        }
        else if((ch>=0x0a)&&(ch<=0x0f)){
            ch = ch-10+'A';
        }
        return ch;
    }else
        return ' ';
}


int QPatternData::wrk_updata(WrkItemHd hd){
    const WrkItemDsp wrkdsp =wrkItemDsp[hd] ;
    switch(hd){
    case WrkItemHd_DuMuZi:
    case WrkItemHd_SuDuZi:
    case WrkItemHd_ShaZuiTF:
    case WrkItemHd_JuanBu:
    case WrkItemHd_JuanBuFZ:
    case WrkItemHd_LeftSongSa:
    case WrkItemHd_RightSongSa:
    case WrkItemHd_CJP_FanZen:
    case WrkItemHd_CJP_BianZi:
        return pcomm->paramaUpdata(wrkdsp.runsendid,wrkbuf+wrkdsp.addr,wrkdsp.len,TRUE);
        break;
    case WrkItemHd_PzkSaZui:
    case WrkItemHd_YTXTingFang:
    case WrkItemHd_YTXXiuZen:
        return pcomm->paramaUpdata(wrkdsp.runsendid,wrkbuf+wrkdsp.addr,wrkdsp.len,FALSE);
        break;
    case WrkItemHd_ZanKaiPianSu:
    case WrkItemHd_QiZenDian:{
            paramaData.loadFile();
            const SpaItemDsp spadsp  = spaItemDsp[SpaItemHd_Jqgzcs];
            const WrkItemDsp qizendiandsp =wrkItemDsp[WrkItemHd_QiZenDian] ;
            const WrkItemDsp ZanKaiPianSudsp =wrkItemDsp[WrkItemHd_ZanKaiPianSu] ;
            unsigned short buf[spadsp.len];
            for(int i=0;i<spadsp.len;i++){
                buf[i]=htons(paramaData.spabuf[spadsp.addr+i]);
            }
            buf[qizendiandsp.offsetinbag] = wrkbuf[qizendiandsp.addr];
            buf[ZanKaiPianSudsp.offsetinbag] = wrkbuf[ZanKaiPianSudsp.addr];
            return pcomm->paramaUpdata(qizendiandsp.runsendid,buf,spadsp.len,TRUE);
            break;
        }
    }
}

inline unsigned char QPatternData::kou2sys(Md::POS_LFETRIGHT kou, unsigned short cntnumber)
{
     bool temp = (kou==Md::POSLEFT)^(cntnumber%2);
     return temp?1:2;
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

int QPatternData::Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag){
    if(saveflag&Md::HAVESAZ){
        if(sazFilePath.isNull())
            sazFilePath = patternDirPath+'/'+patternName+".SAZ";
        QFile sazfile(sazFilePath);
        sazfile.open(QIODevice::ReadWrite);
        sazfile.resize(512);
        int filelen = 0;
        QDataStream stream(&sazfile);
        stream.setByteOrder(QDataStream::LittleEndian);
        foreach(SzkbData data,sazbuf){
            stream<<data.ZuSa<<data.FuSa<<data.Start<<data.End;
            filelen = filelen+8;
        }
        while(filelen<512){
            stream<<(long long)0;
            filelen = filelen+8;
        }
        sazfile.close();
        if(downloadflag&Md::HAVESAZ)
            return sendShazuiKb();
        return Md::Ok;
    }
    if(saveflag&Md::HAVEPAT){
        if(!patModifiedRow.isEmpty()){
            patfile->open(QIODevice::ReadWrite);
            int r;
            foreach(unsigned short row,patModifiedRow){
                int c = tatalcolumn/2+tatalcolumn%2;
                patfile->seek((row+1)*c);
                patfile->write((char *)&(patbuf[row*c]),c);
                if(downloadflag&Md::HAVEPAT){
                     r = qComm.patUpdate(row+1,&patbuf[c*row],c); //download to mainboard
                     if(r!=Md::Ok)
                         break;
                }
            }
            patfile->flush();
            patfile->close();
            if(r==Md::Ok){
                patModifiedRow.clear();
                emit patDirty(FALSE);
                return Md::Ok;
            }
            return r;
        }
    }
    if(saveflag&Md::HAVEWRK){
        if(!wrkModifiedHandle.isEmpty()){
            wrkfile->open(QIODevice::ReadWrite);
            QSet<unsigned char> bags;
            foreach(WrkItemHd handle,wrkModifiedHandle){
                bags.insert(wrkItemDsp[handle].runsendid);
                wrkfile->seek(wrkItemDsp[handle].addr*2);
                wrkfile->write((char *)&wrkbuf[wrkItemDsp[handle].addr],wrkItemDsp[handle].len*2);
            }
            wrkfile->flush();
            wrkfile->close();
            int r;
            if(downloadflag&Md::HAVEWRK){
                foreach(WrkItemHd hd, wrkModifiedHandle){
                    r = wrk_updata(hd);
                    if(r!=Md::Ok)
                        break;
                }
            }
            if(r==Md::Ok){
                wrkModifiedHandle.clear();
                emit wrkDirty(FALSE);
                return Md::Ok;
            }
            return r;
        }
    }
    if(saveflag&Md::HAVECNT){
        if(!cntModifiedRow.isEmpty()){
            cntfile->open(QIODevice::ReadWrite);
            int r;
            foreach(unsigned short row,cntModifiedRow){
                cntfile->seek((row+1)*128);
                cntfile->write((char *)&(cntbuf[row*128]),128);
                if(downloadflag&Md::HAVECNT){
                     r = qComm.cntUpdate(row+1,&cntbuf[128*row]); //download to mainboard
                     if(r!=Md::Ok)
                         break;
                }
            }
            cntfile->flush();
            cntfile->close();
            if(r==Md::Ok){
                cntModifiedRow.clear();
                emit cntDirty(FALSE);
                return Md::Ok;
            }
            return r;
        }
    }
    return Md::Ok;
}

int QPatternData::sendShazuiKb(){
    return pcomm->SendShazuiKb(sazFilePath);
}

short QPatternData::wrk_fechData(WrkItemHd handle,int offset){
    if(!wrkbuf){
        wrkbuf = new unsigned short[2500];
        wrkfile->open(QIODevice::ReadOnly);
        wrkfile->read((char *)wrkbuf,wrkfile->size());
        wrkfile->close();
    }
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
    return QSize(tatalcolumn,tatalcntrow);
}

bool QPatternData::isValid () const{
    //return isValid();
    return TRUE;
}

/*unsigned char QPatternData::cntFechData(unsigned short row,unsigned char addr){
   if(cntbuf)
     return this->cntbuf[row*128+addr];
   else{
       cntfile->open(QIODevice::ReadOnly);


     return 0;
}*/

unsigned short QPatternData::cnt_FechData(int row,unsigned char addr,unsigned char len){
    if(!isPatternAvailable())
        return -1;
    if(len>2)
        return -1;
    if(cntbuf==NULL)
        return -1;
    if(len==1)
        return this->cntbuf[row*128+addr];
    if(len==2)
        return *(unsigned short *)(cntbuf+row*128+addr);
}


unsigned char QPatternData::shaZui(int row,unsigned char system){
    unsigned char cnt_sz = cnt_shaZui(row,system);
    unsigned char sz = cnt_sz;
    foreach(SzkbData data,sazbuf){
        if((data.Start<=(row+1))&&(data.End>=(row+1))){
            if(cnt_sz&(1<<(data.ZuSa-1)))
                sz  = sz|(1<<(data.FuSa-1));
        }
    }
    return sz;
}

unsigned char QPatternData::shaZui(int row, Md::POS_LFETRIGHT kou)
{
    unsigned char sys = kou2sys(kou,row);
    return shaZui(row,sys);
}


unsigned char QPatternData::cnt_shaZui(int row,unsigned char system){
    Q_ASSERT((system==1)||(system==2));
    unsigned char addr = (system==1)?CNT_S1_SaZui:CNT_S2_SaZui;
    unsigned short sz = cnt_FechData(row,addr,2);
    unsigned  char szh = (unsigned char)(sz>>8);
    if((sz&0x00ff)==0x80){
        return szh;
    }
    return 0x0;
}

unsigned char QPatternData::cnt_shaZui(int row, Md::POS_LFETRIGHT kou)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_shaZui(row,sys);
}


unsigned char QPatternData::duMuZhiWork(int row,Md::POS_LFETRIGHT kou,Md::POS_FRONTREAR frontrear ,unsigned char dumuindex){
    Q_ASSERT((dumuindex<=24)&&(dumuindex>=1));
    dumuindex--;
#if DUAL_SYSTEM
    unsigned char offsetdirector = (row%2)?0:4;
    unsigned char offsetposfr = (frontrear==Md::POSREAR)?0:2;
    unsigned char offsetkou = (kou==Md::POSLEFT)?0:1;
    return wrk_fechData(WrkItemHd_DuMuZi,dumuindex*4+offsetdirector+offsetposfr+offsetkou);
#else
    xxxx
#endif
}


unsigned char QPatternData::cnt_yaJiao(int row){
    return this->cnt_FechData(row,CNT_YaJiaoDuan);
}

unsigned char QPatternData::cnt_tingChe(int row){
    return this->cnt_FechData(row,CNT_TingCe);
}

int   QPatternData::cnt_Azhiling(int row,unsigned char system,Md::POS_FRONTREAR pos){
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(pos==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_AZiLing:CNT_S2Q_AZiLing;
    else
        addr = (system==1)?CNT_S1H_AZiLing:CNT_S2H_AZiLing;
    return _azl(cnt_FechData(row,addr));
}

int QPatternData::cnt_Azhiling(int row, Md::POS_LFETRIGHT kou, Md::POS_FRONTREAR pos)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_Azhiling(row,sys,pos);
}

int   QPatternData::cnt_Hzhiling(int row,unsigned char system,Md::POS_FRONTREAR pos){
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(pos==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_HZiLing:CNT_S2Q_HZiLing;
    else
        addr = (system==1)?CNT_S1H_HZiLing:CNT_S2H_HZiLing;
    return _hzl(cnt_FechData(row,addr));
}

int QPatternData::cnt_Hzhiling(int row, Md::POS_LFETRIGHT kou, Md::POS_FRONTREAR pos)
{
    unsigned short sys = kou2sys(kou,row);
    return cnt_Hzhiling(row,sys,pos);
}

QString  QPatternData::cnt_seDaiHaoA(int row,unsigned char system,Md::POS_FRONTREAR pos){
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(pos==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_AColor:CNT_S2Q_AColor;
    else
        addr = (system==1)?CNT_S1H_AColor:CNT_S2H_AColor;
    unsigned short data= cnt_FechData(row,addr,2);
    QString str;
    if(-1==data)
        return str;
    for(int i=0;i<16;i++){
        if(data&(1<<i))
            str.append(QString::number(i,16).toUpper());
    }
    return str;
}

QString QPatternData::cnt_seDaiHaoA(int row, Md::POS_LFETRIGHT kou, Md::POS_FRONTREAR frontorrear)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_seDaiHaoA(row,sys,frontorrear);
}

QString  QPatternData::cnt_seDaiHaoH(int row,unsigned char system,Md::POS_FRONTREAR pos){
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(pos==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_HColor:CNT_S2Q_HColor;
    else
        addr = (system==1)?CNT_S1H_HColor:CNT_S2H_HColor;
    unsigned short data= cnt_FechData(row,addr,2);
    QString str;
    if(-1==data)
        return str;
    for(int i=0;i<16;i++){
        if(data&(1<<i))
            str.append(QString::number(i,16).toUpper());
    }
    return str;
}

QString QPatternData::cnt_seDaiHaoH(int row, Md::POS_LFETRIGHT kou, Md::POS_FRONTREAR frontorrear)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_seDaiHaoH(row,sys,frontorrear);
}


unsigned short QPatternData::cnt_huabanhang(int row,Md::POS_LFETRIGHT sys_kou,Md::POS_FRONTREAR pos){
    unsigned char sys = kou2sys(sys_kou,row);
    return cnt_huabanhang(row,sys,pos);
}

unsigned short QPatternData::cnt_huabanhang(int row, unsigned char sys, Md::POS_FRONTREAR pos)
{
    Q_ASSERT((sys==1)||(sys==2));
    unsigned char addr;
    addr = (sys==1)?CNT_S1Q_Pat:CNT_S2Q_Pat;
    unsigned short val = cnt_FechData(row,addr);
    return ((unsigned short)(-1)==val)?0:val;
}


unsigned short QPatternData::wrk_qizhengdian(){
    return this->wrk_fechData(WrkItemHd_QiZenDian,0);
}


QString QPatternData::cnt_yaoChuang(int row){
    unsigned char ycfx = cnt_FechData(row,CNT_YaoCuangDir);
    unsigned char ycz = cnt_FechData(row,CNT_YaoCuangZi);

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

unsigned char QPatternData::cnt_duMuZu(int row, unsigned char sys, bool &doubleOrSigle,bool usehistorydumu)
{
    Q_ASSERT((sys==1)||(sys==2));
    unsigned char zhilingfh_addr;  //front h
    unsigned char zhilingfa_addr;  //front a
    unsigned char zhilingrh_addr;  //rear h
    unsigned char zhilingra_addr;  //rear a
    unsigned char dumuaddr = CNT_DuMu;
#if DUAL_SYSTEM
    if(sys==1){
        zhilingfh_addr = CNT_S1Q_HZiLing;
        zhilingfa_addr = CNT_S1Q_AZiLing;
        zhilingrh_addr = CNT_S1H_HZiLing;
        zhilingra_addr = CNT_S1H_AZiLing;
    }else{
        zhilingfh_addr = CNT_S2Q_HZiLing;
        zhilingfa_addr = CNT_S2Q_AZiLing;
        zhilingrh_addr = CNT_S2H_HZiLing;
        zhilingra_addr = CNT_S2H_AZiLing;
        if(isdualdumuzu)
           dumuaddr = CNT_DuMu_ExS2;
    }
#else
    zhilingfh_addr = CNT_S1Q_HZiLing;
    zhilingfa_addr = CNT_S1Q_AZiLing;
    zhilingrh_addr = CNT_S1H_HZiLing;
    zhilingra_addr = CNT_S1H_AZiLing;
#endif
    //fetch data
    unsigned char dumu = cnt_FechData(row,dumuaddr);
    unsigned char zhilingfh = cnt_FechData(row,zhilingfh_addr);
    unsigned char zhilingfa = cnt_FechData(row,zhilingfa_addr);
    unsigned char zhilingrh = cnt_FechData(row,zhilingrh_addr);
    unsigned char zhilingra = cnt_FechData(row,zhilingra_addr);

    if(dumu==0){
        //if dumu=0，空跑行：24，翻针：23 其他：历史度目
        if((zhilingfh==0)&&(zhilingfa==0)&&(zhilingrh==0)&&(zhilingra==0))
            dumu = 24;
        else if((zhilingfa==0x01)||(zhilingra==0x01))
            dumu = 23;
        else if(row==0)
            dumu = 1;
        else if(usehistorydumu)
            dumu = (sys==1)?dumu_history_sys1:dumu_history_sys2;
    }else
        (sys==1)?dumu_history_sys1:dumu_history_sys2 = dumu;

    if(dumu<25){
        doubleOrSigle = FALSE;
        return dumu;
    }else if(dumu<101){
        doubleOrSigle = TRUE;
        return 1;
    }else if(dumu<125){
        doubleOrSigle = TRUE;
        return dumu-100;
    }else{
        doubleOrSigle = TRUE;
        return 24;
    }
}

unsigned char QPatternData::cnt_duMuZu(int row,Md::POS_LFETRIGHT kou, bool &doubleOrSigle,bool usehistorydumu){
    unsigned char sys = kou2sys(kou,row);
    return cnt_duMuZu(row,sys,doubleOrSigle,usehistorydumu);
}


unsigned int QPatternData::cnt_dumuUsed()
{
    return dumuzu_used|(1<<22)|(1<<23)|1;
}

unsigned char QPatternData::cnt_spead(int row){
    return this->cnt_FechData(row,CNT_SuDu);
}

unsigned char QPatternData::cnt_mainLuola(int row){
    unsigned short mainluola = cnt_FechData(row,CNT_JuanBu);
    return mainluola;
}

unsigned char QPatternData::cnt_fuzuLuola(int row){
    unsigned short fuluola = cnt_FechData(row,CNT_FuJuanBu)-0x80;
    return fuluola;
}

unsigned char QPatternData::cnt_songSha(int row){
    return cnt_fuzuLuola(row);
}

unsigned char QPatternData::cnt_shazuiTf(int row){
    return this->cnt_FechData(row,CNT_SaZuoTingFang);
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

int  QPatternData::wrkSpeedZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_SuDuZi,index);
}

int  QPatternData::wrkMainLuolaZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_JuanBu,index);
}

int  QPatternData::wrkFuzhuLuolaZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_JuanBuFZ,index);
}

