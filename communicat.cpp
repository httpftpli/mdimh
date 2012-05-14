#include"communicat.h"
#include"data.h"
#include<QCoreApplication>
#include<globaldata.h>
#include<QFile>
#include<QFileInfo>
#include<QtDebug>
#include<QDateTime>
#include"qmdmessagebox.h"
#include<QProgressDialog>
#include<qhmidata.h>
#include<QTime>
#include<constdata.h>


QComm::QComm(QObject *parent):QThread(parent){
}

bool QComm::isAvailable()
{
    static int i=0;
    if(i==0){
        i=1;
        if(TogSysStat(QHMIData::SysInParam)==Md::CommError)
            isavailable = FALSE;
        else
            isavailable = TRUE;
    }
    return isavailable;
}

void QComm::send(){
        unsigned short len = ntohs (*((unsigned short *)d_send));
        unsigned short tempcrc = (CRC16((unsigned char*)d_send,len-3)); //pading CRC
        *(unsigned short *)&((d_send[len-3])) = htons(tempcrc);
        *(unsigned char *)&((d_send[len-1])) = 0x0d;               //pading fin code
        udpsocket.writeDatagram((char *)d_send, len , QHostAddress(DEST_HOST_ADDR), DEST_HOST_PORT );
}


bool QComm::programsend(){
    time.start();
    while(iswaitforack){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(time.hasExpired(COMMTIMEOUT)){
            emit commTimerOut(d_send[2]);
            return FALSE;
        }
    }
    beforesend();
    for(int i =0;i< TIMEROFSEND;i++){
        send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            mutex1.lock();
            if(isacked){
                isacked = FALSE;
                iswaitforack = FALSE;
                mutex1.unlock();
                return TRUE;
            }
            mutex1.unlock();
            usleep(1);
        }
    }
    terminatesend();
    emit commTimerOut(d_send[2]);
    return FALSE;
}

 bool QComm::programsend(unsigned char &ackval){
     while(iswaitforack){
         QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
         if(time.hasExpired(COMMTIMEOUT)){
             emit commTimerOut(d_send[2]);
             return FALSE;
         }
     }
     beforesend();
     for(int i =0;i< TIMEROFSEND;i++){
         send();
         time.start();
         while(!time.hasExpired(COMMTIMEOUT)){
             QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
             mutex1.lock();
             if(isacked){
                 isacked = FALSE;
                 iswaitforack = FALSE;
                 ackval = (unsigned char)*ackbuf;
                 mutex1.unlock();
                 return TRUE;
             }
             mutex1.unlock();
             usleep(1);
         }
     }
     terminatesend();
     emit commTimerOut(d_send[2]);
     return FALSE;
 }

 bool QComm::programsend(unsigned char &ackval1,unsigned char &ackval2){
     while(iswaitforack){
         QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
         if(time.hasExpired(COMMTIMEOUT)){
             emit commTimerOut(d_send[2]);
             return FALSE;
         }
     }
     beforesend();
     for(int i =0;i< TIMEROFSEND;i++){
         send();
         time.start();
         while(!time.hasExpired(COMMTIMEOUT)){
             QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
             mutex1.lock();
             if(isacked){
                 isacked = FALSE;
                 iswaitforack = FALSE;
                 ackval1 = (unsigned char)*ackbuf;
                 ackval2 = (unsigned char)*(ackbuf+1);
                 mutex1.unlock();
                 return TRUE;
             }
             mutex1.unlock();
             usleep(1);
         }
     }
     terminatesend();
     emit commTimerOut(d_send[2]);
     return FALSE;
 }

bool QComm::programsend(unsigned char &ackval1,unsigned char &ackval2,
                  unsigned char &ackval3){
    while(iswaitforack){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(time.hasExpired(COMMTIMEOUT)){
            emit commTimerOut(d_send[2]);
            return FALSE;
        }
    }
     beforesend();
     for(int i =0;i< TIMEROFSEND;i++){
         send();
         time.start();
         while(!time.hasExpired(COMMTIMEOUT)){
             QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
             mutex1.lock();
             if(isacked){
                 isacked = FALSE;
                 iswaitforack = FALSE;
                 ackval1 = (unsigned char)*ackbuf;
                 ackval2 = (unsigned char)*(ackbuf+1);
                 ackval3 = (unsigned char)*(ackbuf+2);
                 mutex1.unlock();
                 return TRUE;
             }
             mutex1.unlock();
             usleep(1);
         }
     }
     terminatesend();
     emit commTimerOut(d_send[2]);
     return FALSE;
 }

bool QComm::programsend(char *buf, unsigned short &len){
    while(iswaitforack){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(time.hasExpired(COMMTIMEOUT)){
            emit commTimerOut(d_send[2]);
            return FALSE;
        }
    }
    beforesend();
    for(int i =0;i< TIMEROFSEND;i++){
        send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            mutex1.lock();
            if(isacked){
                isacked = FALSE;
                iswaitforack = FALSE;
                len = ackbuflen;
                memcpy(buf,ackbuf,ackbuflen);
                mutex1.unlock();
                return TRUE;
            }
            mutex1.unlock();
            usleep(1);
        }
    }
    terminatesend();
    emit commTimerOut(d_send[2]);
    return FALSE;
}


int QComm::lineLock(bool lock){
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x46);//fun code
    *(unsigned short *)(d_send+3) = htons((unsigned short)lock);
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::goToLine(unsigned short line){
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x47);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::sazuiDownUp(bool up){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = (0x48);//fun code
    *(unsigned char *)(d_send+3) = (unsigned char)up;
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}
int QComm::sazuiReplaceSwap(bool replaceorswap,unsigned char leftsazui,unsigned char rightsazui){
    *(unsigned short *)d_send = htons(9); //len
    *(unsigned char *)(d_send+2) = (0x49);//fun code
    *(unsigned char *)(d_send+3) = (unsigned char)replaceorswap;
    *(unsigned char *)(d_send+4) = (unsigned char)leftsazui;
    *(unsigned char *)(d_send+5) = (unsigned char)rightsazui;
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::cntUpdate(int line,unsigned char *buf){
    *(unsigned short *)d_send = htons(136); //len
    *(unsigned char *)(d_send+2) = (0x4a);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    memcpy(d_send+5,buf,128);
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::patUpdate(unsigned short line,unsigned char *buf,unsigned short patlinelen){
    *(unsigned short *)d_send = htons(patlinelen+8); //len
    *(unsigned char *)(d_send+2) = (0x4b);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    memcpy(d_send+5,buf,patlinelen);
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::headStopAside(){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = (0x4d);//fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned char r;
    if(programsend(r)&&(r==0x55))
        return Md::Ok;
    return Md::CommError;
}

int QComm::pollSysVersion(QString &mainbordVersion,QString &bagVersion){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = 0x53;//fun code
    *(unsigned char *)(d_send+3) = 0x00;
    char buf[256];
    unsigned short len;
    if(programsend(buf,len)){
        int lenofmain = *(unsigned char *)buf;
        if((lenofmain>240)||(lenofmain==0)){
            mainbordVersion = "";
            return Md::Ok;
        }
        mainbordVersion = QString::fromAscii(buf+1,lenofmain);
        int lenofbag = *(unsigned char*)(buf+1+lenofmain);
        if(((lenofmain+lenofbag)>240)||(lenofbag==0)){
            bagVersion = "";
            return Md::Ok;
        }
        bagVersion = QString::fromAscii(buf+2+lenofmain,lenofbag);
        return Md::Ok;
    }
    return Md::CommError;
}

void  QComm::MainMotorTest(unsigned char direction,unsigned char speedpercent){
    if(speedpercent > 100) speedpercent = 100;
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x18);//fun code
    *(unsigned char *)(d_send+3) = (direction);
    *(unsigned char *)(d_send+4) = (speedpercent);
    unsigned char r;
    programsend(r);
}


bool QComm::DumuMotorTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr,
                   Md::POSFLAG_LEFTRIGHT motor,unsigned short dumu){
    unsigned char m = 0xff;
    if(!(sys&Md::SYSTEM1))
        m = m & 0xf0;
    if(!(sys&Md::SYSTEM2))
        m = m & 0x0f;
    if(!(fr&Md::POSFRONT))
        m = m & 0xcc;
    if(!(fr&Md::POSREAR))
        m = m & 0x33;
    if(!(motor&Md::POSLEFT))
        m = m & 0xaa;
    if(!(motor&Md::POSRIGHT))
        m = m & 0x55;
    *(unsigned short *)d_send = htons(23);       //len
    *(unsigned char *)(d_send+2) = (0x10);      //fun code
    *(unsigned char *)(d_send+3) = m;
    for(int i=0;i<8;i++){
        *(unsigned short*)(d_send+4+i*2) = htons(dumu);
    }
    send();
    return TRUE;
}

bool QComm::DumuMotorTest(unsigned char motor,unsigned short val){
    *(unsigned short *)d_send = htons(23);       //len
    *(unsigned char *)(d_send+2) = (0x10);      //fun code
    *(unsigned char *)(d_send+3) = motor;
    for(int i=0;i<8;i++){
        *(unsigned short *)(d_send+4+2*i) = htons(val);
    }
    send();
    return TRUE;
}

bool QComm::pinTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr,Md::POSFLAG_LEFTRIGHT lf,
             unsigned char pin,unsigned char val){
    unsigned int pins = pin;
    unsigned int pinss = pins+(pins<<8)+(pins<<16)+(pins<<24);
    if(!(fr&Md::POSREAR))
        pinss = pinss & 0x0000ffff;
    if(!(fr&Md::POSFRONT))
        pinss = pinss & 0xffff0000;
    if(!(lf&Md::POSLEFT))
        pinss = pinss & 0xff00ff00;
    if(!(lf&Md::POSRIGHT))
        pinss = pinss & 0x00ff00ff;
    unsigned long long buf = 0;
    if(sys&Md::SYSTEM1)
        buf = pinss;
    if(sys&Md::SYSTEM2)
        buf = pinss+((long long)pinss<<32);

    *(unsigned short *)d_send = htons(22);       //len
    *(unsigned char *)(d_send+2) = (0x11);      //fun code
    *(unsigned long long *)(d_send+3) = buf;
    *(unsigned long long *)(d_send+11) = (val==0)?0:0xffffffffffffffffuLL;
    return programsend();
}

bool QComm::pinTest(unsigned long long pin, unsigned char stat)
{
    *(unsigned short *)d_send = htons(22);       //len
    *(unsigned char *)(d_send+2) = (0x11);      //fun code
    *(unsigned long long *)(d_send+3) = pin;
    *(unsigned long long *)(d_send+11) = (stat==0)?0:0xffffffffffffffffuLL;
    return programsend();
}


bool QComm::shazuiTest(Md::SYSTEMFLAG sys,unsigned char shazui,unsigned char stat){
    unsigned short shazuis = ((unsigned short)shazui<<8)+shazui;
    if(!(sys&Md::SYSTEM1))
        shazuis = shazuis & 0xff00;
    if(!(sys&Md::SYSTEM2))
        shazuis = shazuis & 0x00ff;

    *(unsigned short *)d_send = htons(10);       //len
    *(unsigned char *)(d_send+2) = 0x12;      //fun code
    *(unsigned short  *)(d_send+3) = shazuis;
    *(unsigned short  *)(d_send+5) = (stat==0)?0:0xffff;
    return programsend();
}

bool QComm::shazuiTest(unsigned short shazui,unsigned char stat){
    *(unsigned short *)(d_send) = htons(10);       //len
    *(unsigned char *)(d_send+2) = 0x12;      //fun code
    *(unsigned short  *)(d_send+3) = shazui;
    *(unsigned short  *)(d_send+5) = (stat==0)?0:0xffff;
    return programsend();
}

bool QComm::sanjiaoMagneticTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr,
                         unsigned char magnet,unsigned long stat){
    unsigned long magnets = (unsigned long)magnet;
    unsigned long magnetss = (magnets<<24)+(magnets<<16)+(magnets<<8)+magnets;
    if(!(sys&Md::SYSTEM1))
        magnetss = magnetss & 0xffff0000;
    if(!(sys&Md::SYSTEM2))
        magnetss = magnetss & 0x0000ffff;
    if(!(fr&Md::POSREAR))
        magnetss = magnetss & 0x00ff00ff;
    if(!(fr&Md::POSFRONT))
        magnetss = magnetss & 0xff00ff00;
    *(unsigned short *)d_send = htons(14);       //len
    *(unsigned char *)(d_send+2) = (0x13);      //fun code
    *(unsigned long  *)(d_send+3) = magnetss;
    *(unsigned long  *)(d_send+7) = (stat==0)?0:0xffffffffL;
    return programsend();
}

bool QComm::sanjiaoMagneticTest(unsigned int sanjiao,unsigned char stat){

    *(unsigned short *)d_send = htons(14);       //len
    *(unsigned char *)(d_send+2) = (0x13);      //fun code
    *(unsigned long  *)(d_send+3) = sanjiao;
    *(unsigned long  *)(d_send+7) = (stat==0)?0:0xffffffffL;
    return programsend();
}

int QComm::yajiaoTest(int frontback, int moveflag, short val)
{
    *(unsigned short *)d_send = htons(10);       //len
    *(unsigned char *)(d_send+2) = (0x17);      //fun code

    *(unsigned char  *)(d_send+3) = frontback;
    *(unsigned char  *)(d_send+4) = moveflag;
    *(short *)(d_send+5) = htons(val);
    unsigned char r=0;
    if(!programsend(r))
        return Md::CommError;
    if(r!=0x55)
        return Md::CommError;
    return Md::Ok;
}

int QComm::sendParamaInRun(unsigned short setcount,unsigned short finishcount,unsigned char RateLimit,
                        unsigned char OneStop,unsigned char alarmLimit,unsigned char DanKouLock){
    *(unsigned short *)d_send = htons(14);       //len
    *(unsigned char *)(d_send+2) = (0x52);      //fun code
    *(unsigned short *)(d_send+3) = htons(setcount);
    *(unsigned short *)(d_send+5) = htons(finishcount);
    *(unsigned char  *)(d_send+7) = RateLimit;
    *(unsigned char  *)(d_send+8) = OneStop;
    *(unsigned char  *)(d_send+9) = alarmLimit;
    *(unsigned char  *)(d_send+10) = DanKouLock;
    unsigned char r;
    if(programsend(r)&&(r=0x55))
        return Md::Ok;
    return Md::CommError;
}


int QComm::bedMotorTest(unsigned char flag,unsigned short pos,unsigned short param){
    *(unsigned short *)d_send = htons(11);       //len
    *(unsigned char *)(d_send+2) = (0x14);      //fun code
    *(unsigned char *)(d_send+3) = (flag);
    *(unsigned short *)(d_send+4) = htons(pos);
    *(unsigned short *)(d_send+6) = htons(param);
    unsigned char r;
    if(programsend(r)&&(r=0x55))
        return Md::Ok;
    return Md::CommError;
}

void QComm::rollTest(unsigned char flag,unsigned char derection,unsigned char rollpercent){
    *(unsigned short *)d_send = htons(9);       //len
    *(unsigned char *)(d_send+2) = (0x16);      //fun code
    *(unsigned char *)(d_send+3) = (flag);
    *(unsigned char *)(d_send+4) = (derection);
    *(unsigned char *)(d_send+5) = rollpercent;
    unsigned char r;
    programsend(r);
}

void QComm::AssistRollTest(unsigned char derection,unsigned char rollpercent){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x19);      //fun code
    *(unsigned char *)(d_send+3) = (derection);
    *(unsigned char *)(d_send+4) = (rollpercent);
    unsigned char r;
    programsend(r);
}

void QComm::ClearError(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = 0x45;      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned char r;
    programsend(r);
}

void QComm::LedTest(unsigned char ledstat){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x15);      //fun code
    *(unsigned char *)(d_send+3) = (ledstat);
    unsigned char r;
    programsend(r);
}

int QComm::readDI(char *buf,unsigned char &len){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = 0x20;      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    return programsend(buf,(unsigned short &)len);
}


void QComm::LeftMuslin(unsigned char force,bool clockwise){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x1A);      //fun code
    *(unsigned char *)(d_send+3) = (unsigned char)clockwise;       //表示正反转，目前不支持，填1
    *(unsigned char *)(d_send+4) = force;
    unsigned char r;
    programsend(r);

}

void QComm::BedReposition(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x1F);      //fun code
    *(unsigned char *)(d_send+3) = 0x00;        //pading
    unsigned char r;
    programsend(r);

}
void QComm::RightMuslin(unsigned char force,bool clockwise){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x1B);      //fun code
    *(unsigned char *)(d_send+3) = (unsigned char )clockwise;
    *(unsigned char *)(d_send+4) = (force);
    unsigned char r;
    programsend(r);
}



int QComm::ReadAbsoluteNoOfPin(unsigned short &val){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x23);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned char ackval1, ackval2;
    if(programsend(ackval1,ackval2)){
        val = (ackval1<<8)+ackval2;
        return Md::Ok;
    }
    return Md::CommError;
}

int QComm::ReadEncoder(unsigned short &val){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x22);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned char ackval1,ackval2;
    if(programsend(ackval1,ackval2)){
        val = (ackval1<<8)+ackval2;
        return Md::Ok;
    }
    return Md::CommError;
}


int QComm::ReadHead(char *buf, unsigned short &len){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x24);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    if(programsend(buf,len))
        return Md::Ok;
    else
        return Md::CommError;
}

int QComm::TogSysStat(unsigned char stat){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x40);      //fun code
    *(unsigned char *)(d_send+3) = stat;
    unsigned char ackval;
    if(programsend(ackval)){
        if(ackval==0x55)
            return Md::Ok;
    }
    return Md::CommError;
}

//////////////checkCustomerId//////////////////////
int QComm::checkCustomerId(unsigned short id){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x54);      //fun code
    *(unsigned short *)(d_send+3) = id;         //和其他协议相反，小端对其
    unsigned char ackval;
    if(programsend(ackval)){
        if(ackval == 0x55)
            return Md::Ok;
        else
            return Md::CustomerIdNotPass;
    }else
        return Md::CommError;
}


void QComm::readshorts(char *scr,unsigned short *dst, int  from,int count){
    for(int i=0;i<count;i++){
        unsigned short d = *(unsigned short *)(scr+from+i*2);
        dst[i] = htons(d);
    }
}

void QComm::readshorts(char *scr,unsigned char *dst,int from,int count){
    for(int i=0;i<count;i++){
        unsigned short d = *(unsigned short *)(scr+from+i*2);
        dst[i] = (unsigned char)d;
    }
}

void QComm::readshorts(QDataStream &stream,unsigned short * buf,int count){
    for(int i=0;i<count;i++){
        unsigned short d;
        stream>>d;
        buf[i] = htons(d);
    }
}

int QComm::SendParama( char *wrkbuf,  char *spabuf,int packet){
    Q_ASSERT(wrkbuf);
    Q_ASSERT(spabuf);
    int result = 0;
    for(int i=0;i<6;i++){
        if((packet&(1<<i))!=0){
            switch(i){
            case 0:
                *(unsigned short *)d_send = htons(206);
                *(unsigned char *)(d_send+2) = 0x91;      //fun code pat file
                *(unsigned short *)(d_send+3) =htons(0x00);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x00);     //param crc  id
                readshorts(spabuf,(unsigned short *)&d_send[7],SpaAddr_JiQiGongZuo*2,34);
                readshorts(wrkbuf,(unsigned short *)&d_send[7],WrkAddr_QiZenDian*2,1); //选择wrk文件中的针起始针位置
                readshorts(wrkbuf,(unsigned short*)&d_send[55],WrkAddr_ZanKaiPianSu*2,1);//选择wrk文件中的展开片数
                readshorts(wrkbuf,(unsigned short *)&d_send[107],WrkAddr_SuDuZi*2,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[155],WrkAddr_JuanBu*2,24);
                unsigned char r;
                result = programsend(r);
                break;
            case 1:
                *(unsigned short *)d_send = htons(442);
                *(unsigned char *)(d_send+2) = 0x92;      //fun code pat file
                *(unsigned short *)(d_send+3) = ntohs(0x00);     //param crc
                *(unsigned short *)(d_send+5) = ntohs(0x00);     //param crc  id
                readshorts(wrkbuf,(unsigned short *)&d_send[7],WrkAddr_GongZuo*2,192);
                readshorts(wrkbuf,(unsigned short *)&d_send[391],WrkAddr_JuanBuFZ*2,24);
                result = result+(programsend()<<i);
                break;
            case 2:
                *(unsigned short *)d_send = htons(266);
                *(unsigned char *)(d_send+2) = 0x93;      //fun code pat file
                *(unsigned short *)(d_send+3) = ntohs(0x00);     //param crc
                *(unsigned short *)(d_send+5) = ntohs(0x00);     //param crc  id
                readshorts(wrkbuf,(unsigned short *)&d_send[7],WrkAddr_ShaZuiTF*2,128);
                result = result+(programsend()<<i);
                break;
            case 3:
                *(unsigned short *)d_send = htons(278);
                *(unsigned char *)(d_send+2) = 0x94;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0x00);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x00);     //param crc  id
                readshorts(spabuf,(unsigned short *)&d_send[7],SpaAddr_XiTong*2,29);
                readshorts(spabuf,(unsigned short *)&d_send[65],SpaAddr_DanMianDMBC*2,8);
                readshorts(spabuf,(unsigned short *)&d_send[81],SpaAddr_SiPinDMBC*2,8);
                readshorts(spabuf,(unsigned short *)&d_send[97],SpaAddr_LingWeiDMBC*2,8);
                readshorts(spabuf,(unsigned short *)&d_send[113],SpaAddr_YaoCuang*2,33);
                readshorts(spabuf,(unsigned short *)&d_send[179],SpaAddr_FanZenYC*2,16);
                readshorts(spabuf,(unsigned short *)&d_send[211],SpaAddr_FanZenZYC*2,16);
                readshorts(spabuf,(unsigned short *)&d_send[243],SpaAddr_FanZenFYC*2,16);
                result = result+(programsend()<<i);
                break;
            case 4:
                *(unsigned short *)d_send = htons(298);
                *(unsigned char *)(d_send+2) = 0x95;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0x00);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x00);     //param crc  id
                readshorts(wrkbuf,(unsigned short *)&d_send[7],WrkAddr_LeftSongSa*2,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[55],WrkAddr_RightSongSa*2,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[103],0xc00,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[151],0xc30,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[199],0xc80,24);
                readshorts(wrkbuf,(unsigned short *)&d_send[247],0xcb0,24);
                result = result+(programsend()<<i);
                break;
            case 5:
                *(unsigned short *)d_send = htons(283);
                *(unsigned char *)(d_send+2) = 0x96;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0x00);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x00);     //param crc  id
                readshorts(wrkbuf,(unsigned char *)&d_send[7],WrkAddr_YTXTingFang*2,16);
                readshorts(wrkbuf,(unsigned char *)&d_send[23],WrkAddr_YTXXiuZen*2,192);
                readshorts(wrkbuf,(unsigned char *)&d_send[215],WrkAddr_PzkSaZui*2,64);
                readshorts(wrkbuf,(unsigned char *)&d_send[279],WrkAddr_PzkClose*2,1);
                result = result+(programsend()<<i);
                break;
             default:
                break;
            }
        }
    }
    if((packet&0x3f)==result){
        return Md::Ok;
    }
    else{
        return Md::CommError;
    }
    return Md::CommError;
}

int QComm::paramaUpdata(unsigned char id,unsigned short *buf,int len,bool halfwordorbyte){
    Q_ASSERT(len<500);
    if(len>500)
        len=500;
    unsigned short packlen;
    *(unsigned char *)(d_send+2) = 0x97;
    *(unsigned char *)(d_send+3) = id;
    if(halfwordorbyte){
        for(int i=0;i<len;i++){
            short val = buf[i];
            *(unsigned short *)(d_send+4+i*2) = htons(val);
        }
        packlen =len*2+6+1;
    }
    else{
        for(int i=0;i<len;i++)
            *(unsigned char *)(d_send+4+i) = (char)(buf[i]);
        packlen =len+6+1;
    }
    *(unsigned short *)d_send = htons(packlen);
    unsigned char r;
    if(!programsend(r))
        return Md::CommError;
    if(r==0x55)
        return Md::Ok;
    return Md::ParamUpdataFail;
}

int QComm::paramaUpdata(unsigned char yiyincunzhenshu, unsigned short chijvjiaozheng,unsigned short zongzhengshu){
    *(unsigned short *)d_send = htons(0x0b);
    *(unsigned char *)(d_send+2) = 0xa0;
    *(unsigned char *)(d_send+3) = yiyincunzhenshu;
    *(unsigned char *)(d_send+5) = htons(chijvjiaozheng);
    *(unsigned char *)(d_send+7) = htons(zongzhengshu);
    unsigned char r;
    if(!programsend(r))
        return Md::CommError;
    if(r==0x55)
        return Md::Ok;
    return Md::ParamUpdataFail;
}

int  QComm::SendShazuiKb(unsigned char entrycount,unsigned char *buf){
    *(unsigned short *)d_send = htons(519);
    *(unsigned char *)(d_send+2) = 0x84;
    memset(d_send+4,0,512);
    if(entrycount>128)
        entrycount = 128;
    if(entrycount){
        *(unsigned char *)(d_send+3) =entrycount;
        for(int i =0;i<(entrycount*4);i++)
            *(unsigned short*)(d_send+4+i*2) = htons(*(unsigned short *)(buf+i*2));
    }else{
        *(unsigned char *)(d_send+3) =0;
    }
    unsigned char ackval;
    if(!programsend(ackval)){
        return Md::CommError;
    }
    if(ackval!=0x55){
        return Md::CommError;
    }
    return Md::Ok;
}

int QComm::sendCntFileInfo(unsigned int lenoffile)
{
    unsigned short packet = (lenoffile+511)/512;
    *(unsigned short *)d_send = htons(29);
    *(unsigned char *)(d_send+2) = 0x80;    
    *(unsigned int *)(d_send+11) = htonl(lenoffile);
    *(unsigned short *)(d_send+24) = htons(packet);
    if(programsend())
        return Md::Ok;
    else
        return Md::CommError;
}

int QComm::sendPatFileInfo(unsigned int lenoffile)
{
    unsigned short packet = (lenoffile+511)/512;
    *(unsigned short *)d_send = htons(29);
    *(unsigned char *)(d_send+2) = 0x82;
    *(unsigned int *)(d_send+11) = htonl(lenoffile);
    *(unsigned short *)(d_send+24) = htons(packet);
    if(programsend())
        return Md::Ok;
    else
        return Md::CommError;
}

int QComm::sendCntFileData(unsigned short pack, unsigned char *buf)
{
    qMemCopy(d_send+5,buf,512);
    *(unsigned short *)d_send = htons(520);
    *(unsigned char *)(d_send+2) = 0x81;
    *(unsigned short *)(d_send+3) =htons(pack);
    if(programsend())
        return Md::Ok;
    else
        return Md::CommError;
}

int QComm::sendPatFileData(unsigned short pack, unsigned char *buf)
{
    qMemCopy(d_send+5,buf,512);
    *(unsigned short *)d_send = htons(520);
    *(unsigned char *)(d_send+2) = 0x83;
    *(unsigned short *)(d_send+3) =htons(pack);
    if(programsend())
        return Md::Ok;
    else
        return Md::CommError;
}


int QComm::IsInBoot(){
    *(unsigned short *)d_send = htons(07);
    *(unsigned char *)(d_send+2) = 0xc1;      //fun code pat file
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned char ackval;
    if(programsend(ackval)){
        if(ackval==0x55)
            return Md::InBootState;
        else
            return Md::InUserState;
    }else{
        return Md::CommError;
    }
}


int QComm::SendBin(QFile &binfile){
     //send ox55 0xaa entern to boot status
     *(unsigned short *)d_send = htons(07);
     *(unsigned char *)(d_send+2) = 0xc0;
     *(unsigned char *)(d_send+3) = 0x55;
     if(!programsend())
          return Md::CommError;
     *(unsigned char *)(d_send+3) = 0xaa;
     if(!programsend())
          return Md::CommError;
    //poll for inboot
     int j=0;
     for(j=0;j<3;j++){
        int r;
        r = IsInBoot();
        if(r&Md::InBootState)
            break;
        else if(r&Md::InUserState)
            return Md::InUserState;
     }
     if(3==j)
         return Md::CommError;
    ///////erase////////////////////////////
     unsigned char comack;
    *(unsigned short *)d_send = htons(07);
    *(unsigned char *)(d_send+2) = 0xc2;
    *(unsigned char *)(d_send+3) = 0x55;
    if(!programsend(comack))
            return Md::CommError;
    if(comack == 0xaa)
        return Md::EraseError;
    ////////wait for erase process
    QElapsedTimer time;
    time.start();
    unsigned char rcvval;
    while(!time.hasExpired(20000)){
        if(!rcv(0xc3,rcvval,10000))
            return Md::EraseError;
        if(rcvval == 0xaa)
            return Md::EraseError;
        emit romEraserPercent(rcvval);
        if(rcvval == 100)
            break;
    }
    if(time.elapsed()>=20000)
        return Md::EraseError;
    ProgramEncrypt encrypt;
    binfile.open(QIODevice::ReadOnly);
    for(int i=0;i<(binfile.size()/512+1);i++){
        *(unsigned short *)d_send = htons(520);
        *(unsigned char *)(d_send+2) = 0xc4;
        *(unsigned short *)(d_send+3) = htons(i+1);
        binfile.read((char *)&d_send[5],512);
        encrypt.Decrypt(&d_send[5],512);
        if(!programsend(comack)){
            binfile.close();
            return Md::CommError;
        }
        if(comack==0xaa){
            binfile.close();
            return Md::BurnError;
        }
        emit romWritePercent(512*i*100/binfile.size());
    }
    *(unsigned short *)d_send = htons(7);
    *(unsigned char *)(d_send+2) = 0xc5;
    *(unsigned char *)(d_send+3) = 0x55;
    if(!programsend()){
        binfile.close();
        return Md::CommError;
    }
    emit romWritePercent(100);
    binfile.close();
    return  Md::Ok;
}


////////////////send bag////////////////////////////////
int QComm::SendBag(QString &bagfilename){
    QFile file(bagfilename);
    if(!file.exists())
        return Md::FileNotExist;
    if(!file.open(QIODevice::ReadOnly))
        return Md::FileOpenFail;

    unsigned char comack;
    int i=0;
    for(i=0;i<3;i++){
        *(unsigned short *)d_send = htons(07);
        *(unsigned char *)(d_send+2) = 0xd0;
        *(unsigned char *)(d_send+3) = 0x55;
        if(!programsend())
            return Md::CommError;
        *(unsigned short *)d_send = htons(07);
        *(unsigned char *)(d_send+2) = 0xd1;
        *(unsigned char *)(d_send+3) = 0x55;
        if(!programsend(comack))
            return Md::CommError;
        if(comack == 0x55)
            break;
    }
    if(i==3)
        return Md::BagCanNotInBoot;
    ///////erase////////////////////////////
    *(unsigned short *)d_send = htons(07);
    *(unsigned char *)(d_send+2) = 0xd2;
    *(unsigned char *)(d_send+3) = 0x55;
    if(!programsend(comack))
        return Md::CommError;
    if(comack == 0xAA)
        return Md::EraseError;
    ////////wait for erase process
    QElapsedTimer time;
    time.start();
    unsigned char rcvval;
    while(!time.hasExpired(10000)){
        if(!rcv(0xd3,rcvval,5000))
            return Md::EraseError;
        if(rcvval == 0xaa)
            return Md::EraseError;
        emit bagRomEraserPercent(rcvval);
        if(comack == 100)
            break;
    }
    if(time.elapsed()>=10000)
        return Md::EraseError;
    //////////start burn//////////////////
    int bagcount = file.size()/64+((file.size()%64)?1:0);
    for(int i=0;i<bagcount;i++){
        *(unsigned short *)d_send = htons(64+3+6);
        *(unsigned char *)(d_send+2) = 0xd4;
        *(unsigned short *)(d_send+3) = htons(i+1);
        if(i==bagcount-1)
            *(unsigned char *)(d_send+5) = 0x55;
        else
            *(unsigned char *)(d_send+5) = 0x00;
        file.read((char *)&d_send[6],64);
        //if(!__ProgramSend(comack)){
           // return Md::CommError;
       // }
        if(comack==0xaa){
            return Md::BurnError;
        }
        emit bagRomWritePercent((i+1)*100/bagcount);
    }

    ///////////////end burn//////////////
    *(unsigned short *)d_send = htons(7);
    *(unsigned char *)(d_send+2) = 0xd5;
    *(unsigned char *)(d_send+3) = 0x55;
    for(int i=0;i<3;i++){
        if(programsend())
            return Md::Ok;
    }
    return Md::Ok;
}




void QComm::run(){
    QUdpSocket udpsocket;
    pudpsocket = &udpsocket;
    QByteArray rcvbuf;
    rcvrawbuf = &rcvbuf;
    pudpsocket->bind(QHostAddress(LOCAL_HOST_ADDR),LOCAL_HOST_PORT);
    connect(pudpsocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()),Qt::DirectConnection);
    exec();
}

void QComm::acktoctrl(unsigned char fun,unsigned char data){
    char dat[15];
    dat[0] = 00;
    dat[1] = 07;
    dat[2] = fun;
    dat[3] = data;
    *(unsigned short *)(dat+4) = htons(CRC16((unsigned char *)dat,4));
    dat[6] = 0xd;
    pudpsocket->writeDatagram((char *)dat, 7 , QHostAddress(DEST_HOST_ADDR), DEST_HOST_PORT );
}


void QComm::ReadPendingDatagrams(){
   while (pudpsocket->hasPendingDatagrams()){
        unsigned int size = pudpsocket->pendingDatagramSize();
        rcvrawbuf->resize(size);
        QHostAddress sender;
        quint16 senderPort;
        pudpsocket->readDatagram(rcvrawbuf->data(), rcvrawbuf->size(),
                                 &sender, &senderPort);
        if((senderPort != DEST_HOST_PORT)|(sender !=(QHostAddress)(DEST_HOST_ADDR)))
            return;
        unsigned short len = ntohs(*(unsigned short *)rcvrawbuf->data());
        if (len > size)
            return;
        unsigned short crc_rcv = ntohs(*(unsigned short *)(rcvrawbuf->data()+len-3));
        unsigned short crc_cal = CRC16((unsigned char *)(rcvrawbuf->data()),len-3);
        if(crc_rcv!=crc_cal )
            return;
        unsigned char fun =(*(unsigned char *)(rcvrawbuf->data()+2));
        char *data = (char*)(rcvrawbuf->data()+3);

        switch(fun){
        case 0x40:
            if(((unsigned char)data[0]==0xaa)||((unsigned char)data[0]==0xbb)){
                emit DataChangedFromCtrl(QHMIData::GLWC,data[0]);  //系统归零完成
                acktoctrl(0x4e,0x55);
                return;
            } else
                writeackdata(data,1);
            break;

        case 0x41:  //up
            if(20==len){
                emit DataChangedFromCtrl(QHMIData::YXHXCL,(QByteArray((char *)data,14)));
                acktoctrl(fun,0x55);
            }
            break;
        case 0x42:   //up rcv system power down
            if(0x07==len){
                emit DataChangedFromCtrl(QHMIData::XTDD,0x55);
            }
            break;
        case 0x21:   //上发 机头相对针数
            if(8==len)
                emit DataChangedFromCtrl(QHMIData::JTXDZS,htons(*(unsigned short*)data));
            break;
        case 0x43:    //上发 报警信息
            if(7==len)
                emit DataChangedFromCtrl(QHMIData::CWBJXX,*data);
            break;
        case 0x4c:    //上发 停车状态
            if(7==len)
                emit DataChangedFromCtrl(QHMIData::DQZDJZT,*data);
            break;
        case 0x4a:
        case 0x4b:
            if(0x55==*data)
                writeackdata(data,1);
            //////////////////////////////////////
            break;
        default:
            if(writeackdata(data,len-6))  //len-6 :data len
                usleep(1);
            break;
        }
        checkwritercvdata(data,len-6);
    }
}

bool QComm::writeackdata(char *buf,unsigned short len){
    mutex1.lock();
    if(iswaitforack&&(sendfuncode==d_send[2])){
        iswaitforack  = FALSE;
        isacked = TRUE;
        ackbuflen = len;
        for(int i=0;i<len;i++){
            ackbuf[i] = buf[i];
        }
        mutex1.unlock();
        return TRUE;
    }
    mutex1.unlock();
    return FALSE;
}

void QComm::checkwritercvdata(char *buf,unsigned short len){
    mutex2.lock();
    if(requirefuncode == d_send[2]){
        requirefuncode = 0x00;
        interruppackrcved = TRUE;
        rcvbuflen = len;
        for(int i=0;i<len;i++){
            rcvbuf[i] = buf[i];
        }
    }
    mutex2.unlock();
}

void QComm::beforesend(){
    mutex1.lock();
    iswaitforack = TRUE;
    sendfuncode=d_send[2];
    isacked = FALSE;
    mutex1.unlock();
}


void QComm::terminatesend(){
    mutex1.lock();
    iswaitforack = FALSE;
    isacked = FALSE;
    mutex1.unlock();
}

bool QComm::rcv(unsigned char funcode,unsigned char &val,unsigned short timeout){
    QElapsedTimer timer;
    timer.start();
    mutex2.lock();
    requirefuncode = funcode;
    interruppackrcved = FALSE;
    mutex2.unlock();
    usleep(1);
    mutex2.lock();
    while(1){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(interruppackrcved){
            interruppackrcved = FALSE;
            requirefuncode =0x00;
            val = (unsigned char)*rcvbuf;
            mutex2.unlock();
            return TRUE;
        }
        usleep(1);
        if(timer.hasExpired(timeout)){
            mutex2.lock();
            requirefuncode = 0x00;
            mutex2.unlock();
            return FALSE;
        }
    }
}




