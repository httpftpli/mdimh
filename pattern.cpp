#include "pattern.h"
#include <QDataStream>
#include <QFileInfo>
#include "qhmidata.h"
#include "qparam.h"
#include <QDir>
#include <QTextCodec>
#include<QSize>
#include "config.h"
#include"communicat.h"
#include"globaldata.h"
#include"config.h"


QMap<unsigned char ,QStringList> QPattern::CntZhilingMap = QPattern::initcntzhilingmap();
QMap<unsigned char ,unsigned char> QPattern::CntZhilingIndexMap = QPattern::initcntzhilingindexmap();

QPattern::QPattern(QComm *s,QObject *parent):QObject(parent),
                            cntbuf(NULL),
                            patfile(NULL), wrkfile(NULL),
                            cntfile(NULL),patbuf(NULL),dumu_history_sys1(1),
                            dumu_history_sys2(1),
                            shazuiused_r(0),shazuiused_l(0),pcomm(s),
#if DUAL_SYSTEM
                            isdualdumuzu(FALSE),
#endif
    dumuzu_used(0){

}

QPattern::~QPattern(){
    if(wrkfile)
        wrkfile->flush();
    if(cntfile)
        cntfile->flush();
    if(patfile)
        patfile->flush();
    return;
}



QPattern::Result QPattern::setFile(const QString &pattern){
    Result r;
    reset();
    INFORMLOG(tr("扫描花型文件"));
    if(!parsepatternpath(pattern))
        return FaultPatternDir;
   probewrk();
   r = probecnt();
    if(r!=Ok){
        reset();
        return r;
    }
    probeprm();
    r=probepat();
    if(r!=Ok){
        reset();
        return r;
    }
    probesaz();
    ispatternavalible = TRUE;
    emit patternChanged(patterndirpath,patternName);
    return Ok;
}

int QPattern::sendPattern()
{
    if(!ispatternavalible)
        return Md::PatternError;
    int temp,percent=0;
    unsigned short cntpacketnum = (cntfile->size()+511)/512;
    unsigned short patpacketnum = (patfile->size()+511)/512;
    unsigned short patternpacketnum = cntpacketnum+patpacketnum;
    if(Md::Ok!=pcomm->sendCntFileInfo((unsigned int)(cntfile->size())))
        return Md::CommError;
    for(int i=0;i<cntpacketnum;i++){
        if(pcomm->sendCntFileData(i,cntbuf+i*512)!=Md::Ok)
            return Md::CommError;
        temp = (i+1)*100/patternpacketnum/5;
        if(percent != temp){
            percent = temp;
            emit patternSendPercent(percent*5);
        }
    }
    if(Md::Ok!=pcomm->sendPatFileInfo((unsigned int)(patfile->size())))
        return Md::CommError;
    for(int i=0;i<patpacketnum;i++){
        if(pcomm->sendPatFileData(i,patbuf+i*512)!=Md::Ok)
            return Md::CommError;
        temp = (i+1+cntpacketnum)*100/patternpacketnum/5;
        if(percent != temp){
            percent = temp;
            emit patternSendPercent(percent*5);
        }
    }
    return Md::Ok;
}


bool QPattern::isWrkfileSys()const
{
    return this->iswrkfilesys;
}

bool QPattern::isPatternAvailable()const
{
    return this->ispatternavalible;
}

void QPattern::pat_SetData(int row,unsigned short column,unsigned char cha){
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
                qMemCopy(patbuf+row*patbyteperrow,patlinebuf,patbyteperrow);
        }
    }
}


int QPattern::cnt_SetData(int row,unsigned char index,
                                      unsigned short data,unsigned char len,bool download){
    if(cntbuf&&(index<128)){
        unsigned char cntlinebuf[128];
        qMemCopy(cntlinebuf,cntbuf+row*128,128);
        unsigned short d = (len==1)?cntlinebuf[index]:(*(unsigned short *)(cntlinebuf+index));
        if(data!=d)
            (len==1)?cntbuf[row*128+index]:(*(unsigned short*)(cntbuf+row*128+index)) = data;
        if(download&&(pcomm->isAvailable())){
            int r = pcomm->cntUpdate(row,cntbuf+row*128);
            if(r!=Md::Ok){
                qMemCopy(cntbuf+row*128,cntlinebuf,128);
                return Md::CommError;
            }
        }
        return Md::Ok;
    }
    return Md::ParamaError;
}


inline  char QPattern::pat_FechData(int row, unsigned short column)const
{
    static const char map[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    unsigned char data = patbuf[row*patbyteperrow+column/2];
    char ch  = (column%2)?data&0x0f:data>>4;
    return map[ch];
}


int QPattern::wrk_updata(WrkItemHd hd){
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

int QPattern::wrk_updataAll()
{
    QSet<WrkItemHd>::Iterator iterator = wrkModifiedHandle.begin();
    int r=Md::Ok;
    while(iterator!=wrkModifiedHandle.end()){
        r = wrk_updata(*iterator);
        if(r!=Md::Ok)
            break;
        iterator = wrkModifiedHandle.erase(iterator);
    }
    emit wrkDirty(!wrkModifiedHandle.isEmpty());
    return Md::Ok ;
}

QMap<unsigned char, QStringList> QPattern::initcntzhilingmap()
{
    QMap<unsigned char,QStringList> map;
    QTextCodec *textcode = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(textcode);
    QString str1 = tr("空");
    QString str2 = tr("翻针");
    QString str3 = tr("编织");
    QString str4 = tr("接针");
    QString str5 = tr("吊目");
    QString str6 = tr("吊目2");
    QString str7 = tr("编松2");
    map.insert(::CNT_ZHILINGVAL[0],QStringList()<<str1<<str1);
    map.insert(::CNT_ZHILINGVAL[1],QStringList()<<str2<<str1);
    map.insert(::CNT_ZHILINGVAL[2],QStringList()<<str3<<str4);
    map.insert(::CNT_ZHILINGVAL[3],QStringList()<<str3<<str1);
    map.insert(::CNT_ZHILINGVAL[4],QStringList()<<str1<<str5);
    map.insert(::CNT_ZHILINGVAL[5],QStringList()<<str1<<str6);
    map.insert(::CNT_ZHILINGVAL[6],QStringList()<<str3<<str5);
    map.insert(::CNT_ZHILINGVAL[7],QStringList()<<str3<<str6);
    map.insert(::CNT_ZHILINGVAL[8],QStringList()<<str3<<str7);
    return  map;
}

QMap<unsigned char, unsigned char> QPattern::initcntzhilingindexmap()
{
    QMap<unsigned char ,unsigned char>map;
    for(int i=0;i<sizeof(CNT_ZHILINGVAL);i++)
        map.insert(CNT_ZHILINGVAL[i],i);
    return map;
}

void QPattern::reset()
{
    cntfile.clear();
    patfile.clear();
    wrkfile.clear();
    sazbuf.clear();
    cntbuf = NULL;
    patbuf = NULL;
    wrkbuf = NULL;
    ispatternavalible = FALSE;
    shazuiused_r =0;
    shazuiused_l =0;
    tatalcolumn = 0;
    tatalcntrow = 0;
    tatalpatrow = 0;
    dumuzu_used = 0;
    patbyteperrow = 0;
#if DUAL_SYSTEM
    this->isdualdumuzu = FALSE;
#endif
    dumu_history_sys1 = 1;
    dumu_history_sys2 = 1;
    patModifiedRow.clear();
    cntModifiedRow.clear();
    wrkModifiedHandle.clear();
}

bool QPattern::parsepatternpath(const QString &patternpath)
{
    QFile file(patternpath);
    QFileInfo fileinfo(file);
    if(fileinfo.dir().exists()){
        patternName = fileinfo.fileName();
        patterndirpath = fileinfo.dir().absolutePath();
        return TRUE;
    }
    else{
        WARNLOG(tr("花型文件目录不存在"));
        return FALSE;
    }
    return FALSE;
}

 QPattern::Result  QPattern::probecnt()
{
    QDir dir(patterndirpath);
    QStringList filelist = dir.entryList(QStringList()<<patternName+".cnt",QDir::Files);
    if(filelist.isEmpty()){
        WARNLOG(tr("CNT文件不存在"));
        return NoCntFile;
    }else{
        cntFileName = filelist.at(0);
    }
    cntfile = QSharedPointer<QFile>(new QFile(patterndirpath+QString("/")+cntFileName));
    if(cntfile->open(QIODevice::ReadWrite)){
        cntbuf = cntfile->map(0,cntfile->size());
    }else{
        WARNLOG(cntFileName+tr("打开失败"));
        return CntFileError;
    }
#if DUAL_SYSTEM
    //test isdualdumuzu according to d[0] and d[1] at cntfile
    if(((cntbuf[0]=='H')&&(cntbuf[1]=='Q'))
            ||((cntbuf[0]=='C')&&(cntbuf[1]=='X'))){
        cntbuf[0]='M';
        cntbuf[1]='D';
        isdualdumuzu = TRUE;
    }else if((cntbuf[0]=='M')&&(cntbuf[1]=='D')){
        isdualdumuzu = TRUE;
    }else{
        isdualdumuzu = FALSE;
    }
#endif
    // scan pattern finish flag,scan shazui
    for(tatalcntrow=1;;tatalcntrow++){
        /*unsigned char sl=0,sr=0,d1=0;
        sl = cnt_shaZui(tatalcntrow,Md::POSLEFT);
        sr = cnt_shaZui(tatalcntrow,Md::POSRIGHT);
        shazuiused_l|=sl;
        shazuiused_r|=sr;
        d1 = cntbuf[tatalcntrow*128+CNT_DuMu];
        if((d1<25)&&(d1!=0))
            dumuzu_used = dumuzu_used|(1<<(d1-1));
        else if((d1<125)&&(d1>100))
            dumuzu_used = dumuzu_used|(1<<(d1-100));*/
        if((1==cntbuf[tatalcntrow*128+CNT_End])||((tatalcntrow+1)*128>cntfile->size()))
            break;
    }
#if DUAL_SYSTEM
    if(isdualdumuzu){
        for(int i=1;i<=tatalcntrow;i++){
            char d2 = cntbuf[i*128+CNT_DuMu];
            if((d2<25)&&(d2!=0))
                dumuzu_used = dumuzu_used|(1<<(d2-1));
            else if((d2<125)&&(d2>100))
                dumuzu_used = dumuzu_used|(1<<(d2-100));
        }
    }
#endif
    cntfile->flush();
    return Ok;
}

inline unsigned char QPattern::kou2sys(Md::POS_LEFTRIGHT kou, int cntnumber)
{
     bool temp = (kou==Md::POSLEFT)^(cntnumber%2);
     return temp?1:2;
}


/*void QPattern::refreshBuf(Md::HAVEFILEFLAG flag){
    if((flag&Md::HAVEPAT)&&patbuf&&(!patModifiedRow.isEmpty())){
        // NOTE: may add refresh patbuf
        //patModifiedRow.clear();
        //emit patDirty(FALSE);
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
        // NOTE: may add refresh cntbuf
        //cntModifiedRow.clear();
        //emit cntDirty(FALSE);
 */


QPattern::Result QPattern::loadLoop(const QString &prmfilepath){

}

int QPattern::Save(Md::HAVEFILEFLAG saveflag,Md::HAVEFILEFLAG downloadflag){
    if(saveflag&Md::HAVESAZ){
        if(sazFilePath.isNull())
            sazFilePath = patterndirpath+'/'+patternName+".SAZ";
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

int QPattern::sendShazuiKb(){
    return pcomm->SendShazuiKb(sazFilePath);
}

short QPattern::wrk_fechData(WrkItemHd handle,int offset){
    int addr = wrkItemDsp[handle].addr;
    return wrkbuf[offset+addr];
}

void QPattern::wrk_setData(WrkItemHd handle,int offset,short data){
    int addr = wrkItemDsp[handle].addr;
    if(wrkbuf[addr+offset]!=data){
        wrkbuf[addr+offset] = data;
        wrkModifiedHandle.insert(handle);
        emit wrkDirty(TRUE);
    }
}

QSize QPattern::patternSize()const{
    return QSize(tatalcolumn,tatalcntrow);
}


inline unsigned short QPattern::cnt_FechData(int row,unsigned char addr,unsigned char len) const{
    Q_ASSERT(len<3);
    if(cntbuf){
        if(len==1)
            return  __cntfetchshortdata(row,addr);
        if(len==2)
            return __cntfetchchardata(row,addr);
    }
    return -1;
}


unsigned char QPattern::shaZui(int row,unsigned char system){
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

unsigned char QPattern::shaZui(int row, Md::POS_LEFTRIGHT kou)
{
    unsigned char sys = kou2sys(kou,row);
    return shaZui(row,sys);
}


unsigned char QPattern::cnt_shaZui(int row,unsigned char system){
    Q_ASSERT((system==1)||(system==2));
    if(ispatternavalible&&cntbuf)
        return __cnt_shazui(row,system);
    else
        return 0;
}


unsigned char QPattern::cnt_shaZui(int row, Md::POS_LEFTRIGHT kou)
{   
    if(ispatternavalible&&cntbuf)
        return __cnt_shazui(row,kou);
    else
        return 0;
}

void QPattern::cnt_setShaZui(int row, Md::POS_LEFTRIGHT kou, unsigned char shazuibitmap)
{
    if((row<1)||(row>tatalcntrow))
        return;
    unsigned char sys = kou2sys(kou,row);
    unsigned short shazui = ((unsigned short)shazuibitmap<<8)+0x80;
    unsigned char indexaddr = (sys==1)?CNT_S1_SaZui:CNT_S2_SaZui;
    cnt_SetData(row,indexaddr,shazui,2,TRUE);
}


unsigned char QPattern::duMuZhiWork(int row,Md::POS_LEFTRIGHT kou,Md::POS_FRONTREAR frontrear ,unsigned char dumuindex){
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


unsigned char QPattern::cnt_yaJiao(int row) const{
    return this->cnt_FechData(row,CNT_YaJiaoDuan);
}

void QPattern::cnt_setYajiao(int row, unsigned char yajiao)
{
    if((row<1)||(row>24))
        return;
    cnt_SetData(row,CNT_YaJiaoDuan,yajiao,1,TRUE);
}

unsigned char QPattern::cnt_tingChe(int row){
    return this->cnt_FechData(row,CNT_TingCe);
}

void QPattern::cnt_setTingChe(int row, bool tingche)
{
    if((row<1)||(row>tatalcntrow))
        return;
    cnt_SetData(row,CNT_TingCe,(unsigned char)tingche,1,TRUE);
}

unsigned char QPattern::cnt_Zhiling(int row, unsigned char system, Md::POS_FRONTREAR fr)
{
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(fr==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_AZiLing:CNT_S2Q_AZiLing;
    else
        addr = (system==1)?CNT_S1H_AZiLing:CNT_S2H_AZiLing;
    return cnt_FechData(row,addr);
}

unsigned char QPattern::cnt_Zhiling(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR fr)
{
    unsigned short sys = kou2sys(kou,row);
    return cnt_Zhiling(row,sys,fr);
}

void QPattern::cnt_setZhiling(int row, unsigned char system, Md::POS_FRONTREAR fr, unsigned char val)
{
    unsigned char addra,addrh;
    if(fr==Md::POSFRONT){
        addra = (system==1)?CNT_S1Q_AZiLing:CNT_S2Q_AZiLing;
        addrh = (system==1)?CNT_S1Q_HZiLing:CNT_S2Q_HZiLing;
    }
    else{
        addra = (system==1)?CNT_S1H_AZiLing:CNT_S2H_AZiLing;
        addrh = (system==1)?CNT_S1H_HZiLing:CNT_S2H_HZiLing;
    }
    cnt_SetData(row,addra,val,1,FALSE);
    cnt_SetData(row,addrh,val,1,TRUE);
}

void QPattern::cnt_setZhiling(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR fr, unsigned char val)
{
    unsigned char sys = kou2sys(kou,row);
    cnt_setZhiling(row,sys,fr,val);
}



QString  QPattern::cnt_seDaiHaoA(int row,unsigned char system,Md::POS_FRONTREAR pos){
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

void QPattern::cnt_setSeDaiHaoA(int row, unsigned char system, Md::POS_FRONTREAR fr, unsigned short val)
{
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(fr==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_AColor:CNT_S2Q_AColor;
    else
        addr = (system==1)?CNT_S1H_AColor:CNT_S2H_AColor;
    cnt_SetData(row,addr,val,2,TRUE);
}

QString QPattern::cnt_seDaiHaoA(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR frontorrear)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_seDaiHaoA(row,sys,frontorrear);
}

void QPattern::cnt_setSeDaiHaoA(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR fr, unsigned short val)
{
    unsigned char sys= kou2sys(kou,row);
    cnt_setSeDaiHaoA(row,sys,fr,val);
}

QString  QPattern::cnt_seDaiHaoH(int row,unsigned char system,Md::POS_FRONTREAR pos){
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

void QPattern::cnt_setSeDaiHaoH(int row, unsigned char system, Md::POS_FRONTREAR fr, unsigned short val)
{
    Q_ASSERT((system==1||(system==2)));
    unsigned char addr;
    if(fr==Md::POSFRONT)
        addr = (system==1)?CNT_S1Q_HColor:CNT_S2Q_HColor;
    else
        addr = (system==1)?CNT_S1H_HColor:CNT_S2H_HColor;
    cnt_SetData(row,addr,val,2,TRUE);
}

QString QPattern::cnt_seDaiHaoH(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR frontorrear)
{
    unsigned char sys = kou2sys(kou,row);
    return cnt_seDaiHaoH(row,sys,frontorrear);
}

void QPattern::cnt_setSeDaiHaoH(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR fr, unsigned short val)
{
    unsigned char sys = kou2sys(kou,row);
    cnt_setSeDaiHaoH(row,sys,fr,val);
}


unsigned short QPattern::cnt_huabanhang(int row,Md::POS_LEFTRIGHT sys_kou,Md::POS_FRONTREAR pos){
    unsigned char sys = kou2sys(sys_kou,row);
    return cnt_huabanhang(row,sys,pos);
}

unsigned short QPattern::cnt_huabanhang(int row, unsigned char sys, Md::POS_FRONTREAR pos)
{
    Q_ASSERT((sys==1)||(sys==2));
    unsigned char addr;
    addr = (sys==1)?CNT_S1Q_Pat:CNT_S2Q_Pat;
    unsigned short val = cnt_FechData(row,addr);
    return ((unsigned short)(-1)==val)?0:val;
}

void QPattern::cnt_setHuabanhang(int row, Md::POS_LEFTRIGHT kou, Md::POS_FRONTREAR fr, int huabanrow)
{

}

bool QPattern::cnt_shaZuiUsed(unsigned char &left, unsigned char &right)
{
    left = 0;
    right = 0;
    if(!(ispatternavalible&&cntbuf))
        return FALSE;
    unsigned char sl=0,sr=0;
    for(int i=1;i<=tatalcntrow;i++){
        sl|= __cnt_shazui(i,Md::POSLEFT);
        sr|= __cnt_shazui(i,Md::POSRIGHT);
    }
    left = sl;
    right = sr;
    return TRUE;
}

QMap<int,QPattern::CntLoopType> QPattern::cnt_LoopTable() const
{
    QMap<int,QPattern::CntLoopType> loopmap;
    if(!cntbuf)
        return loopmap;
    for(int i=1;i<=tatalcntrow;i++){
       unsigned short numofloop = cnt_FechData(i,CNT_LoopNum,2);
        if(numofloop){
            QPattern::CntLoopType loop;
            loop.startline = cnt_FechData(i,CNT_LoopStart,2);
            loop.endline = i;
            loop.numofloop = numofloop;
            loopmap.insert(i,loop);
        }
    }
    return loopmap;
}

int QPattern::cnt_setLoopTable(QMap<int, CntLoopType> loop)
{
    if(loop.isEmpty())
        return Md::Ok;
    QList<int> keys = loop.keys();
    foreach(int key,keys){
        CntLoopType loopitem = loop.value(key);
        int start = loopitem.startline;
        int num = loopitem.numofloop;
        cnt_SetData(key,CNT_LoopNum,num,2,FALSE);
        if(cnt_SetData(key,CNT_LoopStart,start,2,TRUE)!=Md::Ok)
           return Md::CommError;
    }
    cntfile->flush();
    return Md::Ok;
}


unsigned short QPattern::wrk_qizhengdian(){
    return this->wrk_fechData(WrkItemHd_QiZenDian,0);
}

void QPattern::cnt_yaoChuang(int row, YCPOSITION &pos, char &val)
{
    unsigned char ycfx = cnt_FechData(row,CNT_YaoCuangDir);
    char ycz = cnt_FechData(row,CNT_YaoCuangZi);
    if(ycfx&0x40)
        ycz = -ycz;
    val = qBound((char)-8,ycz,(char)+8);
    if(ycfx&0x0f==0x00)
        pos = ZHENDUICHI;
    else if(ycfx&0x0f==0x01)
        pos = ZHENDUIZHEN;
    else if(ycfx&0x0f==0x02)
        pos = FANZHENZHE;
    else if(ycfx&0x0f==0x04)
        pos = FANZHENFU;
    return;
}

void QPattern::cnt_setYaoChang(int row, QPattern::YCPOSITION pos, char val)
{
    unsigned char ycpos=0x80,ycz;
    if(pos==ZHENDUIZHEN)
        ycpos|=0x01;
    else if(pos==FANZHENZHE)
        ycpos|=0x02;
    else if(pos==FANZHENFU)
        ycpos|=0x04;
    if(pos<0)
        ycpos!=0x40;
    cnt_SetData(row,CNT_YaoCuangDir,ycpos,1,FALSE);
    cnt_SetData(row,CNT_YaoCuangZi,qAbs(ycz),1,TRUE);
}

unsigned char QPattern::cnt_duMuZu(int row, unsigned char sys, bool &doubleOrSigle,bool usehistorydumu)
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

unsigned char QPattern::cnt_duMuZu(int row,Md::POS_LEFTRIGHT kou, bool &doubleOrSigle,bool usehistorydumu){
    unsigned char sys = kou2sys(kou,row);
    return cnt_duMuZu(row,sys,doubleOrSigle,usehistorydumu);
}

void QPattern::cnt_setDuMuZu(int row, Md::POS_LEFTRIGHT kou, bool doubleorsignle, unsigned char dumu)
{
    if((row<1)||(row>tatalcntrow))
        return;
    unsigned sys = kou2sys(kou,row);
    unsigned char indexaddr = CNT_DuMu;
    unsigned char dumu1 = doubleorsignle?(100+dumu):dumu;
#if DUAL_SYSTEM
    if((sys==2)&&(isdualdumuzu))
        indexaddr = CNT_DuMu_ExS2;
#endif
    cnt_SetData(row,indexaddr,dumu1,1,TRUE);

}


unsigned int QPattern::cnt_dumuUsed()
{
    return dumuzu_used|(1<<22)|(1<<23)|1;
}

unsigned char QPattern::cnt_spead(int row){
    return this->cnt_FechData(row,CNT_SuDu);
}

void QPattern::cnt_setSpeed(int row, unsigned char speed)
{
    if((row<1||row>tatalcntrow))
        return;
    if((speed<1)||(speed>24))
        return;
    cnt_SetData(row,CNT_SuDu,speed,1,TRUE);
}

unsigned char QPattern::cnt_mainLuola(int row){
    unsigned short mainluola = cnt_FechData(row,CNT_JuanBu);
    return mainluola;
}

void QPattern::cnt_setMainLuola(int row, unsigned char mainluola)
{
    if((row<1||row>tatalcntrow))
        return;
    if((mainluola<1)||(mainluola>24))
        return;
    cnt_SetData(row,CNT_JuanBu,mainluola,1,TRUE);
}

unsigned char QPattern::cnt_fuzuLuola(int row){
    unsigned short fuluola = cnt_FechData(row,CNT_FuJuanBu)-0x80;
    return fuluola;
}

void QPattern::cnt_setFuzuLuola(int row, unsigned char fluola)
{
    if((row<1||row>tatalcntrow))
        return;
    if((fluola<1)||(fluola>24))
        return;
    cnt_SetData(row,CNT_FuJuanBu,fluola,1,TRUE);
}

unsigned char QPattern::cnt_songSha(int row){
    return cnt_fuzuLuola(row);
}

void QPattern::cnt_setSongsha(int row, unsigned char songsha)
{
    cnt_setFuzuLuola(row,songsha);
}

unsigned char QPattern::cnt_shazuiTf(int row){
    return this->cnt_FechData(row,CNT_SaZuoTingFang);
}

void QPattern::cnt_setShazuiTf(int row, unsigned char shazuitf)
{
    if((row<1||row>tatalcntrow))
        return;
    if((shazuitf<1)||(shazuitf>8))
        return;
    cnt_SetData(row,CNT_SaZuoTingFang,shazuitf,1,TRUE);
}

int QPattern::_azlfordisplay(unsigned char data){
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

int QPattern::_hzlfordisplay(unsigned char data){
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

unsigned char QPattern::_azlfromdisplay(int data)
{
    /*int index=0;
    switch(index){
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
    return index;*/
}

unsigned char QPattern::_hzlfromdisplay(int index)
{
   /* int index=0;
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
    return index;*/
}

int  QPattern::wrkSpeedZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_SuDuZi,index);
}

int  QPattern::wrkMainLuolaZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_JuanBu,index);
}

int  QPattern::wrkFuzhuLuolaZhi(unsigned int index){
    Q_ASSERT((index<25)&&(index>0));
    if(index>24)
        index = 24;
    if(index<1)
        index = 1;
    index--;
    return wrk_fechData(WrkItemHd_JuanBuFZ,index);
}

unsigned char QPattern::saz_shaZuiUsed()
{
    unsigned char sz = 0;
    foreach(SzkbData d,sazbuf)
        sz |=1<<(d.FuSa-1);
    return sz;
}

QPattern::Result QPattern::probepat()
{
    QDir dir(patterndirpath);
    QStringList filelist = dir.entryList(QStringList()<<((patternName+".pat").trimmed()),QDir::Files);
    if(filelist.isEmpty()){
        WARNLOG(tr("PAT文件不存在"));
        return NoPatFile;
    }else{
        patFileName = filelist.at(0);
    }

    patfile = QSharedPointer<QFile>(new QFile(patterndirpath+QString("/")+patFileName));
    if(patfile->open(QIODevice::ReadWrite)){
            patbuf = patfile->map(0,patfile->size());
     }else{
        WARNLOG(tr("PAT文件打开失败"));
        return  PatFileError;
    }

    tatalcolumn = *(unsigned short *)patbuf;
    tatalpatrow = *(unsigned short *)(patbuf+2);     //读取花型尺寸
    if((tatalcolumn==0)||(tatalpatrow==0)){
        ERRORLOG(tr("花型文件格式错误"));
        return  PatFileError;
    }
    patbyteperrow = tatalcolumn/2+tatalcolumn%2;
    if(patfile->size()<(patbyteperrow*(tatalpatrow+1))){
        WARNLOG(tr("花型文件格式错误，PAT文件长度不匹配"));
        return  PatFileError;
    }
    return Ok;
}

void QPattern::probewrk()
{
    QDir dir(patterndirpath);
    QString wrkfilepath;
    QStringList filelist = dir.entryList(QStringList()<<((patternName+".wrk").trimmed()),QDir::Files);
    if(filelist.isEmpty()){
        WARNLOG(tr("WRK文件不存在,使用默认WRK文件"));
        wrkFileName = "default.wrk";
        iswrkfilesys = TRUE;
    }else{
        wrkFileName = filelist.at(0);
        wrkfilepath = patterndirpath+QString("/")+wrkFileName;
        iswrkfilesys = FALSE;
    }
    wrkfile = QSharedPointer<QFile>(new QFile(wrkfilepath));
    wrkfile->open(QIODevice::ReadWrite);
    wrkbuf = (unsigned short *)(wrkfile->map(0,wrkfile->size()));
}

void QPattern::probesaz()
{
    QDir dir(patterndirpath);
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);
    QStringList  sazfilenamelist = dir.entryList(QStringList()<<(patternName+"*.SAZ"),QDir::Files);
    if(!sazfilenamelist.isEmpty()){
        sazFileName = sazfilenamelist.at(0);
        sazFilePath =  patterndirpath+'/'+sazfilenamelist.at(0);
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
            sazbuf<<temp;
        }
        sazfile.close();
    }
}

void QPattern::probeprm()
{
    QDir dir(patterndirpath);
    QDataStream stream;
    stream.setByteOrder(QDataStream::LittleEndian);
    QStringList  list = dir.entryList(QStringList()<<(patternName+"*.PRM"),QDir::Files);
    if(!list.isEmpty()){
        CntLoopType loop;
        QMap<int ,CntLoopType> loopmap;
        QFile file(patterndirpath+'/'+list.at(0));
        file.open(QIODevice::ReadOnly);
        stream.setDevice(&file);
        unsigned short beginrow,endrow,loopcount,temp;
        do{
            stream>>beginrow>>endrow>>loopcount,temp;
            if((beginrow>tatalcntrow)||(endrow>tatalcntrow)||
                    (beginrow==0)||(endrow==0)||(loopcount==0))
                continue;
            if((!(beginrow%2))||(endrow%2))
                continue;
            loop.startline = beginrow;
            loop.endline = endrow;
            loop.numofloop = loopcount;
            loopmap.insert(endrow, loop);
        }while(!stream.atEnd());
        cnt_setLoopTable(loopmap);
        file.close();
        file.remove();
    }
}

inline unsigned short QPattern::__cntfetchshortdata(int row, unsigned char offset) const
{
    return *(unsigned short *)(cntbuf+row*128+offset);
}

inline unsigned short QPattern::__cntfetchchardata(int row, unsigned char offset) const
{
    return cntbuf[row*128+offset];
}

inline unsigned char QPattern::__cnt_shazui(int row,unsigned char system)const{
    unsigned char addr = (system==1)?CNT_S1_SaZui:CNT_S2_SaZui;
    unsigned short sz = __cntfetchshortdata(row,addr);
    unsigned  char szh = (unsigned char)(sz>>8);
    if((sz&0x00ff)==0x80)
        return szh;
    else
        return 0;
}


inline unsigned char QPattern::__cnt_shazui(int row, Md::POS_LEFTRIGHT kou)const
{
    unsigned char sys = kou2sys(kou,row);
    return __cnt_shazui(row,sys);
}
