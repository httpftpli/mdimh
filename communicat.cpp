

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





void QInterComm::WriteAck(bool ackornot){
    pmutex->lock();
    this->packetrcved = TRUE;
    this->isack = ackornot;
    pmutex->unlock();
}

void QInterComm::WriteData(unsigned char funcode,unsigned short ackval1,unsigned short ackval2 ){
    pmutex->lock();
    this->packetrcved = FALSE;
    this->isack == FALSE;
    this->funcode = funcode;
    this->val = ackval1;
    this->val2 = ackval2;
    pmutex->unlock();
}

void QInterComm::WriteData(unsigned char *buf,unsigned char len){
    pmutex->lock();
    this->packetrcved = FALSE;
    memcpy(this->buf,buf,len);
    pmutex->unlock();
}

void QInterComm::Clear(){
     pmutex->lock();
     packetrcved = FALSE;
     pmutex->unlock();
}

bool QInterComm::Rcv(bool &isack,unsigned char &funcode,unsigned short &val){
    pmutex->lock();
    if(FALSE == packetrcved){
        pmutex->unlock();
        return FALSE;
    }
    isack = this->isack;
    funcode = this->funcode;
    val = this->val;
    this->packetrcved = FALSE;
    pmutex->unlock();
    return TRUE;
}

bool QInterComm::Rcv(bool &isack, char *buf,unsigned char &len){

}


bool QInterComm::Rcv(bool &isack,unsigned char &funcode,unsigned short &val1,unsigned short &val2){
    pmutex->lock();
    if(FALSE == packetrcved){
        pmutex->unlock();
        return FALSE;
    }
    isack = this->isack;
    funcode = this->funcode;
    val1 = this->val;
    val2 = this->val2;
    this->packetrcved = FALSE;
    pmutex->unlock();
    return TRUE;
}

bool QInterComm::IsAcked(){
    bool isack;
    unsigned char funcode;
    unsigned short val;
    if(Rcv(isack,funcode,val))
        return isack;
    return FALSE;
}


bool QInterComm::IsAcked(unsigned char &funcode ,unsigned short &val){
    bool isack,r;
    unsigned char funcodetemp;
    unsigned short valtemp;
    r = Rcv(isack,funcodetemp,valtemp);
    if(FALSE==r)
        return FALSE;
    if(FALSE==isack)
        return FALSE;
    funcode = funcodetemp;
    val=valtemp;
    return TRUE;
}

bool QInterComm::IsAcked(unsigned short &val){
    bool isack,r;
    unsigned char funcodetemp;
    unsigned short valtemp;
    r = Rcv(isack,funcodetemp,valtemp);
    if(FALSE==r)
        return FALSE;
    if(FALSE==isack)
        return FALSE;
    val=valtemp;
    return TRUE;
}

bool QInterComm::IsAcked(unsigned short &val1,unsigned short &val2){
    bool isack,r;
    unsigned char funcodetemp;
    unsigned short valtemp1,valtemp2;
    r = Rcv(isack,funcodetemp,valtemp1,valtemp2);
    if(FALSE==r)
        return FALSE;
    if(FALSE==isack)
        return FALSE;
    val1 = valtemp1;
    val2 = valtemp2;
    return TRUE;
}

bool QInterComm::IsAcked(char *buf ,unsigned char &len){
    pmutex->lock();
    if(!packetrcved)
        pmutex->unlock();
        return FALSE;
    if(!isack)
        pmutex->unlock();
        return FALSE;
    memcpy(buf,this->buf,this->len);
    len = this->len;
    pmutex->unlock();
    return TRUE;
}



QInterComm InterComm;     //define globle share val betwin QRvc and QSend


QSend::QSend(QObject *parent):QObject(parent),TIMEROFSEND(3),COMMTIMEOUT(400){
}


void QSend::Send(){
        unsigned short len = ntohs (*((unsigned short *)d_send));
        unsigned short tempcrc = (CRC16(d_send,len-3)); //pading CRC
        *(unsigned short *)&((d_send[len-3])) = htons(tempcrc) ;
        *(unsigned char *)&((d_send[len-1])) = 0x0d;               //pading fin code
        udpsocket.writeDatagram((char *)d_send, len , QHostAddress(DEST_HOST_ADDR), DEST_HOST_PORT );
}




void QSend::TimeOutSend(){
    Send();
}


 bool QSend::ProgramSend(){
    //unsigned int count;
    emit NotifyRcver(d_send[2],1);
    InterComm.Clear();
    for(int i =0;i< TIMEROFSEND;i++){
        //count= 200000000;
        Send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            if(InterComm.IsAcked()){
                emit NotifyRcver(d_send[2],0);
                return TRUE;
            }
        }
    }
    emit NotifyRcver(d_send[2],0);
    emit commTimerOut(d_send[2]);
    return FALSE;
}

bool QSend::ProgramSend(unsigned short &ackval){
    //unsigned int count;
    emit NotifyRcver(d_send[2],1);
    InterComm.Clear();
    for(int i =0;i< TIMEROFSEND;i++){
        //count= 200000000;
        Send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            unsigned short val;
            if(InterComm.IsAcked(val)){
                ackval = val;
                emit NotifyRcver(d_send[2],0);
                return TRUE;
            }
        }
    }
    emit NotifyRcver(d_send[2],0);
    emit commTimerOut(d_send[2]);
    return FALSE;
}


bool QSend::ProgramSend(char *buf, unsigned char &len){
    //unsigned int count;
    emit NotifyRcver(d_send[2],1);
    InterComm.Clear();
    for(int i =0;i< TIMEROFSEND;i++){
        //count= 200000000;
        Send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            if(InterComm.IsAcked(buf,len)){
                emit NotifyRcver(d_send[2],0);
                return TRUE;
            }
        }
    }
    emit NotifyRcver(d_send[2],0);
    emit commTimerOut(d_send[2]);
    return FALSE;
}


bool QSend::__ProgramSend(unsigned short &ackval){  //此函数是针对升级背包时下位有时出现两次回复而特殊编写，一般情况下不用此函数。
    //unsigned int count;
    emit NotifyRcver(d_send[2],1);
    InterComm.Clear();
    unsigned short val1,val2;
    for(int i =0;i< TIMEROFSEND;i++){
        //count= 200000000;
        Send();
        time.start();
        while(!time.hasExpired(COMMTIMEOUT)){
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            if(InterComm.IsAcked(val1,val2)&(val2 ==ntohs(*(unsigned short *)(d_send+3)))){
                ackval = val1;
                emit NotifyRcver(d_send[2],0);
                return TRUE;
            }
        }
    }
    emit NotifyRcver(d_send[2],0);
    emit commTimerOut(d_send[2]);
    return FALSE;
}


int QSend::firstLineLoop(bool loop){
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x46);//fun code
    *(unsigned short *)(d_send+3) = htons((unsigned short)loop);
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}

int QSend::goToLine(unsigned short line){
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x47);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}
int QSend::sazuiDownUp(bool up){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = (0x48);//fun code
    *(unsigned char *)(d_send+3) = (unsigned char)up;
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}
int QSend::sazuiReplaceSwap(bool replaceorswap,unsigned char leftsazui,unsigned char rightsazui){
    *(unsigned short *)d_send = htons(9); //len
    *(unsigned char *)(d_send+2) = (0x49);//fun code
    *(unsigned char *)(d_send+3) = (unsigned char)replaceorswap;
    *(unsigned char *)(d_send+4) = (unsigned char)leftsazui;
    *(unsigned char *)(d_send+5) = (unsigned char)rightsazui;
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}

int QSend::cntUpdate(unsigned short line,unsigned char *buf){
    *(unsigned short *)d_send = htons(136); //len
    *(unsigned char *)(d_send+2) = (0x4a);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    memcpy(d_send+5,buf,128);
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}

int QSend::patUpdate(unsigned short line,unsigned char *buf,unsigned short patlinelen){
    *(unsigned short *)d_send = htons(patlinelen+8); //len
    *(unsigned char *)(d_send+2) = (0x4b);//fun code
    *(unsigned short *)(d_send+3) = htons(line);
    memcpy(d_send+5,buf,patlinelen);
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}

int QSend::headStopAside(){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = (0x4d);//fun code
    *(unsigned char *)(d_send+3) = 0x55;
    if(ProgramSend())
        return Md::Ok;
    return Md::CommError;
}

int QSend::pollSysVersion(QString &mainbordVersion,QString &bagVersion){
    *(unsigned short *)d_send = htons(7); //len
    *(unsigned char *)(d_send+2) = (0x53);//fun code
    *(unsigned char *)(d_send+3) = 0x55;
    char buf[256];
    unsigned char len;
    if(ProgramSend(buf,len)){
        int lenofmain = ntohs(*(unsigned short *)buf);
        if(lenofmain>240)
            mainbordVersion = "";
        mainbordVersion = QString::fromAscii(buf+2,lenofmain);
        int lenofbag = ntohs(*(unsigned short*)(buf+2+lenofmain));
        if((lenofmain+lenofbag)>240)
            bagVersion = "";
        bagVersion = QString::fromAscii(buf+4+lenofmain,lenofbag);
        return Md::Ok;
    }
    return Md::CommError;
}

void  QSend::MainMotorTest(unsigned char direction,unsigned char speedpercent){
    if(speedpercent > 100) speedpercent = 100;
    *(unsigned short *)d_send = htons(8); //len
    *(unsigned char *)(d_send+2) = (0x18);//fun code
    *(unsigned char *)(d_send+3) = (direction);
    *(unsigned char *)(d_send+4) = (speedpercent);
    ProgramSend();
}



void QSend::DumuMotorTest(unsigned char motor,const unsigned char (&coord)[8]){
    *(unsigned short *)d_send = htons(23);       //len
    *(unsigned char *)(d_send+2) = (0x10);      //fun code
    *(unsigned char *)(d_send+3) = (motor);
    memcpy(d_send+4,coord,16);
    ProgramSend();
}

void QSend::pinTest( unsigned long long pin, unsigned long long  stat){
    *(unsigned short *)d_send = htons(22);       //len
    *(unsigned char *)(d_send+2) = (0x11);      //fun code
    *(unsigned long long *)(d_send+3) = pin;
    *(unsigned long long *)(d_send+11) = stat;
    ProgramSend();
}

void QSend::muslinMagneticTest(unsigned short muslinMag,unsigned short stat){
    *(unsigned short *)d_send = htons(10);       //len
    *(unsigned char *)(d_send+2) = (0x12);      //fun code
    *(unsigned short  *)(d_send+3) = muslinMag;
    *(unsigned short  *)(d_send+5) = stat;
    ProgramSend();
}

void QSend::trigoneMagneticTest(unsigned long trigoneMag,unsigned long stat){
    *(unsigned short *)d_send = htons(14);       //len
    *(unsigned char *)(d_send+2) = (0x13);      //fun code
    *(unsigned long  *)(d_send+3) = trigoneMag;
    *(unsigned long  *)(d_send+7) = stat;
    ProgramSend();
}

int QSend::sendParamaInRun(unsigned short setcount,unsigned short finishcount,unsigned char RateLimit,
                        unsigned char OneStop,unsigned char alarmLimit,unsigned char DanKouLock){
    *(unsigned short *)d_send = htons(14);       //len
    *(unsigned char *)(d_send+2) = (0x52);      //fun code
    *(unsigned short *)(d_send+3) = htons(setcount);
    *(unsigned short *)(d_send+5) = htons(finishcount);
    *(unsigned char  *)(d_send+7) = RateLimit;
    *(unsigned char  *)(d_send+8) = OneStop;
    *(unsigned char  *)(d_send+9) = alarmLimit;
    *(unsigned char  *)(d_send+10) = DanKouLock;
    if(ProgramSend())
        return Md::Ok;
    else
        return Md::CommError;
}


void QSend::bedMotorTest(unsigned char flag,unsigned short pos,unsigned short param){

}

void QSend::StepRollTest(unsigned char flag,unsigned char derection,unsigned char rollpercent){
    *(unsigned short *)d_send = htons(9);       //len
    *(unsigned char *)(d_send+2) = (0x16);      //fun code
    *(unsigned char *)(d_send+3) = (flag);
    *(unsigned char *)(d_send+4) = (derection);
    *(unsigned char *)(d_send+5) = rollpercent;
    ProgramSend();
}

void QSend::AssistRollTest(unsigned char derection,unsigned char rollpercent){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x19);      //fun code
    *(unsigned char *)(d_send+3) = (derection);
    *(unsigned char *)(d_send+4) = (rollpercent);
    ProgramSend();
}

void QSend::ClearError(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = 0x45;      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    ProgramSend();
}

void QSend::LedTest(unsigned char ledstat){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x15);      //fun code
    *(unsigned char *)(d_send+3) = (ledstat);
    ProgramSend();
}

void QSend::readDI(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = 0x20;      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    ProgramSend();
}


void QSend::LeftMuslin(unsigned char force,bool clockwise){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x1A);      //fun code
    *(unsigned char *)(d_send+3) = (unsigned char)clockwise;       //表示正反转，目前不支持，填1
    *(unsigned char *)(d_send+4) = force;
    ProgramSend();

}

void QSend::BedReposition(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x1F);      //fun code
    *(unsigned char *)(d_send+3) = 0x00;        //pading
    ProgramSend();

}
void QSend::RightMuslin(unsigned char force,bool clockwise){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x1B);      //fun code
    *(unsigned char *)(d_send+3) = (unsigned char )clockwise;
    *(unsigned char *)(d_send+4) = (force);
    ProgramSend();
}

int QSend::ShoutDownSystem(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x42);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned short ack;
    if(!ProgramSend(ack))
        return Md::CommError;
    if(0x55==ack)
        return Md::WaitingForShoutDown;
    return  Md::CommError;
}


int QSend::ReadAbsoluteNoOfPin(unsigned short &val){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x23);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned short ackval;
    if(ProgramSend(ackval)){
        val = ackval;
        return Md::Ok;
    }
    return Md::CommError;
}

int QSend::ReadEncoder(unsigned short &val){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x22);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned short ackval;
    if(ProgramSend(ackval)){
        val = ackval;
        return Md::Ok;
    }
    return Md::CommError;
}



int QSend::ReadHead(){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x24);      //fun code
    *(unsigned char *)(d_send+3) = 0x55;
    ProgramSend();
    return Md::Ok;
}

int QSend::TogSysStat(unsigned char stat){
    *(unsigned short *)d_send = htons(7);       //len
    *(unsigned char *)(d_send+2) = (0x40);      //fun code
    *(unsigned char *)(d_send+3) = stat;
    unsigned short ackval;
    if(ProgramSend(ackval)){
        if(ackval==0x55)
            return Md::Ok;
    }
    else
        return Md::CommError;
}

//////////////checkCustomerId//////////////////////
int QSend::checkCustomerId(unsigned short id){
    *(unsigned short *)d_send = htons(8);       //len
    *(unsigned char *)(d_send+2) = (0x54);      //fun code
    *(unsigned short *)(d_send+3) = id;         //和其他协议相反，小端对其
    unsigned short ackval;
    if(ProgramSend(ackval)){
        if(ackval == 0x55)
            return Md::Ok;
        else
            return Md::CustomerIdNotPass;
    }else
        return Md::CommError;
}



void QSend::readshorts(QDataStream &stream,unsigned short * buf,int count){
    for(int i=0;i<count;i++){
        unsigned short d;
        stream>>d;
        buf[i] = htons(d);
    }
}

void QSend::readshorts(QDataStream &stream,unsigned char * buf,int count){
    for(int i=0;i<count;i++){
        unsigned short d;
        stream>>d;
        buf[i] = (unsigned short)d;
    }
}


int QSend::SendParama( QFile &wrkfile, QFile &spafile,int packet,QWidget *parent){
    if(!wrkfile.open(QIODevice::ReadOnly)){
        qDebug()<<"in communicat.cpp  QSend::SendParama wrkfile open fail";
        return Md::FileOpenFail;
    }
    if(!spafile.open(QIODevice::ReadOnly)){
        qDebug()<<"in communicat.cpp  QSend::SendParama spafile open fail";
        return Md::FileOpenFail;
    }
    QDataStream wrkstream(&wrkfile),spastream(&spafile);
    wrkstream.setByteOrder(QDataStream::LittleEndian);
    spastream.setByteOrder(QDataStream::LittleEndian);
    int result = 0;
    for(int i=0;i<6;i++){
        if((packet&(1<<i))!=0){
            switch(i){
            case 0:
                *(unsigned short *)d_send = htons(206);
                *(unsigned char *)(d_send+2) = 0x91;      //fun code pat file
                *(unsigned short *)(d_send+3) =htons(0x896e);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x6119);     //param crc  id
                spafile.seek(0x4C0);
                readshorts(spastream,(unsigned short *)&d_send[7],34);
                wrkfile.seek(0xdea);
                readshorts(wrkstream,(unsigned short *)&d_send[7],1);//选择wrk文件中的针起始针位置
                wrkfile.seek(0xde0);
                readshorts(wrkstream,(unsigned short*)&d_send[31],1);//选择wrk文件中的展开片数
                wrkfile.seek(0x300);
                readshorts(wrkstream,(unsigned short *)&d_send[107],24);
                wrkfile.seek(0x600);
                readshorts(wrkstream,(unsigned short *)&d_send[155],24);
                result = ProgramSend();
                break;
            case 1:
                *(unsigned short *)d_send = htons(442);
                *(unsigned char *)(d_send+2) = 0x92;      //fun code pat file
                *(unsigned short *)(d_send+3) = ntohs(0x17f1);     //param crc
                *(unsigned short *)(d_send+5) = ntohs(0x5810);     //param crc  id
                wrkfile.seek(0);
                readshorts(wrkstream,(unsigned short *)&d_send[7],192);
                wrkfile.seek(0x6a0);
                readshorts(wrkstream,(unsigned short *)&d_send[391],24);
                result = result+(ProgramSend()<<i);
                break;
            case 2:
                *(unsigned short *)d_send = htons(266);
                *(unsigned char *)(d_send+2) = 0x93;      //fun code pat file
                *(unsigned short *)(d_send+3) = ntohs(0x84d1);     //param crc
                *(unsigned short *)(d_send+5) = ntohs(0x4b03);     //param crc  id
                wrkfile.seek(0x400);
                readshorts(wrkstream,(unsigned short *)&d_send[7],128);
                result = result+(ProgramSend()<<i);
                break;
            case 3:
                *(unsigned short *)d_send = htons(278);
                *(unsigned char *)(d_send+2) = 0x94;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0xf584);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x570f);     //param crc  id
                spafile.seek(0);
                readshorts(spastream,(unsigned short *)&d_send[7],29);
                spafile.seek(0xa0);            //dpm
                readshorts(spastream,(unsigned short *)&d_send[65],8);
                spafile.seek(0x2c0);           //spm
                readshorts(spastream,(unsigned short *)&d_send[81],8);
                spafile.seek(0x2e0);            //bgzdmlw
                readshorts(spastream,(unsigned short *)&d_send[97],8);
                spafile.seek(0x300);            //ycwzxz
                readshorts(spastream,(unsigned short *)&d_send[113],33);
                spafile.seek(0x400);            //fzycwzxz
                readshorts(spastream,(unsigned short *)&d_send[179],16);
                spafile.seek(0x440);            //+fzycwzxz
                readshorts(spastream,(unsigned short *)&d_send[211],16);
                spafile.seek(0x480);            //-fzycwzxz
                readshorts(spastream,(unsigned short *)&d_send[243],16);
                result = result+(ProgramSend()<<i);
                break;
            case 4:
                *(unsigned short *)d_send = htons(298);
                *(unsigned char *)(d_send+2) = 0x95;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0x9141);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x5109);     //param crc  id
                wrkfile.seek(0x800);             //zss
                readshorts(wrkstream,(unsigned short *)&d_send[7],24);
                wrkfile.seek(0x840);             //yss
                readshorts(wrkstream,(unsigned short *)&d_send[55],24);
                wrkfile.seek(0xc00);           //yxcjpfz
                readshorts(wrkstream,(unsigned short *)&d_send[103],24);
                wrkfile.seek(0xc30);            //zxhcjpbz
                readshorts(wrkstream,(unsigned short *)&d_send[151],24);
                wrkfile.seek(0xc80);            //yxcjpfz
                readshorts(wrkstream,(unsigned short *)&d_send[199],24);
                wrkfile.seek(0xcb0);            //zxcjpbz
                readshorts(wrkstream,(unsigned short *)&d_send[247],24);
                result = result+(ProgramSend()<<i);
                break;
            case 5:
                *(unsigned short *)d_send = htons(283);
                *(unsigned char *)(d_send+2) = 0x96;      //fun code pat file
                *(unsigned short *)(d_send+3) = htons(0x2e2b);     //param crc
                *(unsigned short *)(d_send+5) = htons(0x9951);     //param crc  id
                wrkfile.seek(0x8a0);
                readshorts(wrkstream,(unsigned char *)&d_send[7],16);
                wrkfile.seek(0xa00);
                readshorts(wrkstream,(unsigned char *)&d_send[23],192);
                wrkfile.seek(0xd00);
                readshorts(wrkstream,(unsigned char *)&d_send[215],64);
                wrkfile.seek(0xdc0);
                readshorts(wrkstream,(unsigned char *)&d_send[279],1);
                result = result+(ProgramSend()<<i);
                break;
             default:
                break;
            }

        }
    }
    if((packet&0x3f)==result){
        if(parent!=NULL)
            QMessageBox::information(parent,tr("参数下载"),tr("下载成功"));
        return Md::Ok;
    }
    else{
        if(parent!=NULL)
            QMessageBox::warning(parent,tr("参数下载"),tr("通信出错"));
        return Md::CommError;
    }
}


int  QSend::SendFile(QFile &file,unsigned short fileid, bool samehint, QWidget *parent){
    if(!file.exists()){
        return Md::FileNotExist;
    }
    unsigned char infofuncode,datafuncode;
    QFileInfo fileinfo(file);
    QString fname= fileinfo.fileName();
    if(fname.endsWith(".cnt",Qt::CaseInsensitive)){
        infofuncode = 0x80;//                     CNT FILE
        datafuncode = 0x81;

    }else if (fname.endsWith(".pat",Qt::CaseInsensitive)){
        infofuncode = 0x82; //            PAT FILE
        datafuncode = 0x83;
    }else if(fname.endsWith(".saz",Qt::CaseInsensitive)){
        infofuncode = 0x00;
        datafuncode = 0x84;
    }else{
        return Md::NotPatCntSaz;
    }
    QByteArray filename = fname.left(fname.size()-4).toLatin1();
    int size = filename.size();
    if(size<8){
        filename.append("         ",8-size);
    }
    if(size>8){
        filename.resize(8);
    }
    int lenoffile = fileinfo.size();
    unsigned short packet = lenoffile/512 + (lenoffile%512?1:0);
    unsigned short  result;
    if(samehint&(infofuncode !=0x0)){
        QDateTime filetime = fileinfo.lastModified();
        unsigned short year = (unsigned short)filetime.date().year();
        unsigned char  month = (unsigned char)filetime.date().month();
        unsigned char  day =(unsigned char)filetime.date().day();
        unsigned char  hour = (unsigned char)filetime.time().hour();
        unsigned char  minute = (unsigned char)filetime.time().minute();
        unsigned char  second =(unsigned char)filetime.time().second();
        *(unsigned short *)d_send = htons(29);
        *(unsigned char *)(d_send+2) = infofuncode;      //fun code pat file
        for(int i=0;i<8;i++)
            *(unsigned char *)(d_send+3+i)=(unsigned char)filename.at(i);
        *(unsigned int *)(d_send+11) = htonl(lenoffile);   //file  lenght
        *(unsigned short *)(d_send+15) = htons(year);
        *(unsigned char *)(d_send+17) = (month);
        *(unsigned char *)(d_send+18) = (day);
        *(unsigned char *)(d_send+19) = (hour);
        *(unsigned char *)(d_send+20) = (minute);
        *(unsigned char *)(d_send+21) = (second);

        *(unsigned short *)(d_send+22) = fileid;            //file id
        *(unsigned short *)(d_send+24) = htons(packet);
        if(ProgramSend(result)){
            if(result ==0){
                if(QMessageBox::warning(parent,tr("花型发送"),fileinfo.suffix()+tr("文件已存在,是否覆盖"),
                                        QMessageBox::Yes|QMessageBox::No)== QMessageBox::No)
                    return Md::FileSame;
            }
        }else{
            return Md::CommError;
        }
    }
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);
    if(0==infofuncode){
       int i;
       for(i=0;i<64;i++){
           unsigned short sazui;
           file.seek(i*8);
           stream>>sazui;
           if(sazui==0)
               break;
       }
       file.seek(0);
       *(unsigned short *)d_send = htons(519);
       *(unsigned char *)(d_send+2) = datafuncode;
       *(unsigned char *)(d_send+3) =i+1;
       stream.readRawData((char *)&d_send[4],512);
       if(!ProgramSend(result)){
           return Md::CommError;
       }
       if(result!=0x55){
           return Md::CommError;
       }
   }else{
       for(int i=0;i<packet;i++){
           stream.readRawData((char*)&d_send[5],512);
           *(unsigned short *)d_send = htons(520);
           *(unsigned char *)(d_send+2) = datafuncode;
           *(unsigned short *)(d_send+3) =htons(i);
           if(!ProgramSend(result)){
               return Md::CommError;
           }
           if(result!=i){
               return Md::CommError;
           }
           emit commPercent((i+1)*100/packet);
       }
   }
   return Md::Ok;
}


int QSend::IsInBoot(){
    *(unsigned short *)d_send = htons(07);
    *(unsigned char *)(d_send+2) = 0xc1;      //fun code pat file
    *(unsigned char *)(d_send+3) = 0x55;
    unsigned short ackval;
    if(ProgramSend(ackval)){
        if(ackval==0x55)
            return Md::InBootState;
        else
            return Md::InUserState;
    }else{
        return Md::CommError;
    }
}


int QSend::SendBin(QFile &binfile,QWidget *parent){
     //send ox55 0xaa entern to boot status
     *(unsigned short *)d_send = htons(07);
     *(unsigned char *)(d_send+2) = 0xc0;
     *(unsigned char *)(d_send+3) = 0x55;
     if(!ProgramSend())
          return Md::CommError;
     *(unsigned char *)(d_send+3) = 0xaa;
     if(!ProgramSend())
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
     unsigned short comack;
     QProgressDialog progressdialog(parent);
    *(unsigned short *)d_send = htons(07);
    *(unsigned char *)(d_send+2) = 0xc2;
    *(unsigned char *)(d_send+3) = 0x55;
    if(!ProgramSend(comack))
            return Md::CommError;
    if(comack == 0xaa)
        return Md::EraseError;
    ////////wait for erase process
    QElapsedTimer time;
    time.start();
    progressdialog.setRange(0,100);
    progressdialog.setLabelText(tr("擦除进度"));
    progressdialog.setCancelButton(0);
    progressdialog.setAutoClose(FALSE);
    progressdialog.show();

    bool isack;
    unsigned char funcode;
    while(!time.hasExpired(20000)){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(!InterComm.Rcv(isack,funcode,comack))
            continue;
        if(funcode == 0xc3){
            if(comack == 0xaa)
                return Md::EraseError;
            progressdialog.setValue(comack);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            if(comack == 100)
                break;
        }
    }
    if(time.elapsed()>=20000)
        return Md::EraseError;
    ProgramEncrypt encrypt;
    progressdialog.setLabelText(tr("烧写进度"));
    binfile.open(QIODevice::ReadOnly);
    for(int i=0;i<(binfile.size()/512+1);i++){
        *(unsigned short *)d_send = htons(520);
        *(unsigned char *)(d_send+2) = 0xc4;
        *(unsigned short *)(d_send+3) = htons(i+1);
        binfile.read((char *)&d_send[5],512);
        encrypt.Decrypt(&d_send[5],512);
        if(!ProgramSend(comack)){
            binfile.close();
            return Md::CommError;
        }
        if(comack==0xaa){
            binfile.close();
            return Md::BurnError;
        }
        progressdialog.setValue(512*i*100/binfile.size());

    }
    *(unsigned short *)d_send = htons(7);
    *(unsigned char *)(d_send+2) = 0xc5;
    *(unsigned char *)(d_send+3) = 0x55;
    if(!ProgramSend()){
        binfile.close();
        return Md::CommError;
    }
    progressdialog.setValue(100);
    binfile.close();
    return  Md::Ok;
}


////////////////send bag////////////////////////////////
int QSend::SendBag(QString &bagfilename,QWidget *parent){
    QFile file(bagfilename);
    if(!file.exists())
        return Md::FileNotExist;
    if(!file.open(QIODevice::ReadOnly))
        return Md::FileOpenFail;

    unsigned short comack;
    int i=0;
    for(i=0;i<3;i++){
        *(unsigned short *)d_send = htons(07);
        *(unsigned char *)(d_send+2) = 0xd0;
        *(unsigned char *)(d_send+3) = 0x55;
        if(!ProgramSend())
            return Md::CommError;
        *(unsigned short *)d_send = htons(07);
        *(unsigned char *)(d_send+2) = 0xd1;
        *(unsigned char *)(d_send+3) = 0x55;
        if(!ProgramSend(comack))
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
    if(!ProgramSend(comack))
        return Md::CommError;
    if(comack == 0xAA)
        return Md::EraseError;
    ////////wait for erase process
    QProgressDialog progressdialog(parent);
    QElapsedTimer time;
    time.start();
    progressdialog.setRange(0,100);
    progressdialog.setLabelText(tr("擦除进度"));
    progressdialog.setCancelButton(0);
    progressdialog.setAutoClose(FALSE);
    progressdialog.show();
    bool isack;
    unsigned char funcode;
    while(time.elapsed()< 10000){
        QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        if(!InterComm.Rcv(isack,funcode,comack))
            continue;
        if(funcode == 0xd3){
            if(comack == 0xaa)
                return Md::EraseError;
            progressdialog.setValue(comack);
            QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
            if(comack == 100)
                break;
        }
    }
    if(time.elapsed()>=20000)
        return Md::EraseError;
    //////////start burn//////////////////
    progressdialog.setLabelText(tr("烧写进度"));
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
        if(!__ProgramSend(comack)){
            return Md::CommError;
        }
        if(comack==0xaa){
            return Md::BurnError;
        }
        progressdialog.setValue((i+1)*100/bagcount);
    }

    ///////////////end burn//////////////
    *(unsigned short *)d_send = htons(7);
    *(unsigned char *)(d_send+2) = 0xd5;
    *(unsigned char *)(d_send+3) = 0x55;
    for(int i=0;i<3;i++){
        if(ProgramSend())
            return Md::Ok;
    }
    return Md::Ok;
}


/*
void QRcv::On_NotifyRcver(unsigned char fun,bool addordel){
    if(addordel){
        funcode = fun;
    }
    else {
        if(funcode==fun)
            funcode = 0;
    }
}

void QRcv::run(){
    pudpsocket= new QUdpSocket;
    pudpsocket->bind(QHostAddress(LOCAL_HOST_ADDR),LOCAL_HOST_PORT);
    connect(pudpsocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()),Qt::DirectConnection);
    exec();
}

void QRcv::Ack(unsigned char fun,unsigned char data){
    unsigned char dat[15];
    dat[0] = 00;
    dat[1] = 07;
    dat[2] = fun;
    dat[3] = data;
    *(unsigned short *)(dat+4) = htons(CRC16(dat,4));
    dat[6] = 0xd;
    pudpsocket->writeDatagram((char *)dat, 7 , QHostAddress(DEST_HOST_ADDR), DEST_HOST_PORT );
}


void QRcv::ReadPendingDatagrams(){
   while (pudpsocket->hasPendingDatagrams()){
        QByteArray datagram;
        unsigned int size = pudpsocket->pendingDatagramSize();
        datagram.resize(size);
        QHostAddress sender;
        quint16 senderPort;
        pudpsocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);
        if((senderPort != DEST_HOST_PORT)|(sender !=(QHostAddress)(DEST_HOST_ADDR)))
            return;
        unsigned short len = ntohs(*(unsigned short *)datagram.data());
        if (len > size)
            return;
        unsigned short crc_rcv = ntohs(*(unsigned short *)(datagram.data()+len-3));
        unsigned short crc_cal = CRC16((unsigned char*)(datagram.data()),len-3);
        if(crc_rcv!=crc_cal )
            return;
        unsigned char fun =(*(unsigned char *)(datagram.data()+2));
        unsigned char *data = (unsigned char*)(datagram.data()+3);


        switch(fun){
        case 0x10:
             break;
        case 0x11:  case 0x12:  case 0x13:  case 0x14:   case 0x15:
        case 0x16:  case 0x17:  case 0x18:  case 0x19:   case 0x1a:
        case 0x1b:  case 0x1f:  case 0x42:  case 0x45:   case 0x46:
        case 0x47:  case 0x48:  case 0x4d:  case 0x52:   case 0x54:
        case 0xc2:  case 0xc3:  case 0xc4:  case 0xd1:   case 0xd2:
        case 0xd3:
            InterComm.WriteData(fun,data[0]);
            break;
        case 0x20:
            if(12==len){
                emit DataChanged(QHMIData::WBSR,(QByteArray((char *)data,4)));
                emit DataChanged(QHMIData::ZXHJSQ,htoni(*(unsigned short*)(data+4)));
                break;
            }
            else
                return;
        case 0x22:  case 0x23:
            InterComm.WriteData(fun,data[0],ntohs(*(unsigned short *)data));
            break;
        case 0x24:
            /////////////////////////////////////////////

        case 0x40:
            if((data[0]==0xaa)||(data[0]==0xbb)){
                emit DataChanged(QHMIData::GLWC,data[0]);  //系统归零完成
                Ack(0x4e,0x55);
                return;
            } else
                InterComm.WriteData(fun,data[0]);
            break;

        case 0x41:
            if(20==len){
                emit DataChanged(QHMIData::YXHXCL,(QByteArray(*data,14)));
                Ack(fun,0x55);
            }
            break;


        case 0x21:   //上发 机头相对针数
            if(8==len)
                emit DataChanged(QHMIData::JTXDZS,htons(*(unsigned short*)data));
            break;
        case 0x43:    //上发 报警信息
            if(7==len)
                emit DataChanged(QHMIData::CWBJXX,*data);
            break;
        case 0x4c:    //上发 停车状态
            if(7==len)
                emit DataChanged(QHMIData::DQZDJZT,*data);
            break;
        case 0x49:////?
        case 0x4a:
        case 0x4b:
            break;


        case 0x53:
            if(len>250)
                return;
            InterComm.WriteData(data,len-6);
            break;
        case 0x80:
        case 0x82:
            InterComm.WriteData(0x82,data[0]);
            break;
        case 0x81:
        case 0x83:
            InterComm.WriteData(fun,htons(*(unsigned short *)data));
            break;
        case 0x84:
            InterComm.WriteData(fun,*(unsigned char *)data);
            break;

        case 0xc1:
            if(funcode == 0xc0){
                InterComm.WriteData(0xc0,data[0]);
                InterComm.WriteAck(TRUE);
                return;
            }
            InterComm.WriteData(0xC1,data[0]);
            break;
        case 0xd4:
            InterComm.WriteData(fun,data[0],ntohs(*(unsigned short *)(data+1)));
            break;
        default:
            break;
        }
        if(fun == funcode)
            InterComm.WriteAck(TRUE);
        else
            InterComm.WriteAck(FALSE);
    }
}*/






//////////////class QRcv////////////////////////////////////


void QRcv::On_NotifyRcver(unsigned char fun,bool addordel){
    if(addordel){
        funcode = fun;
    }
    else {
        if(funcode==fun)
            funcode = 0;
    }
}

void QRcv::run(){
    pudpsocket= new QUdpSocket;
    pudpsocket->bind(QHostAddress(LOCAL_HOST_ADDR),LOCAL_HOST_PORT);
    connect(pudpsocket,SIGNAL(readyRead()),this,SLOT(ReadPendingDatagrams()),Qt::DirectConnection);
    exec();
}

void QRcv::Ack(unsigned char fun,unsigned char data){
    unsigned char dat[15];
    dat[0] = 00;
    dat[1] = 07;
    dat[2] = fun;
    dat[3] = data;
    *(unsigned short *)(dat+4) = htons(CRC16(dat,4));
    dat[6] = 0xd;
    pudpsocket->writeDatagram((char *)dat, 7 , QHostAddress(DEST_HOST_ADDR), DEST_HOST_PORT );
}


void QRcv::ReadPendingDatagrams(){
   while (pudpsocket->hasPendingDatagrams()){
        QByteArray datagram;
        unsigned int size = pudpsocket->pendingDatagramSize();
        datagram.resize(size);
        QHostAddress sender;
        quint16 senderPort;
        pudpsocket->readDatagram(datagram.data(), datagram.size(),
                                 &sender, &senderPort);
        if((senderPort != DEST_HOST_PORT)|(sender !=(QHostAddress)(DEST_HOST_ADDR)))
            return;
        unsigned short len = ntohs(*(unsigned short *)datagram.data());
        if (len > size)
            return;
        unsigned short crc_rcv = ntohs(*(unsigned short *)(datagram.data()+len-3));
        unsigned short crc_cal = CRC16((unsigned char*)(datagram.data()),len-3);
        if(crc_rcv!=crc_cal )
            return;
        unsigned char fun =(*(unsigned char *)(datagram.data()+2));
        unsigned char *data = (unsigned char*)(datagram.data()+3);


        switch(fun){
        case 0x10:
             break;
        case 0x11:  case 0x12:  case 0x13:  case 0x14:   case 0x15:
        case 0x16:  case 0x17:  case 0x18:  case 0x19:   case 0x1a:
        case 0x1b:  case 0x1f:  case 0x42:  case 0x45:   case 0x46:
        case 0x47:  case 0x48:  case 0x4d:  case 0x52:   case 0x54:
        case 0xc2:  case 0xc3:  case 0xc4:  case 0xd1:   case 0xd2:
        case 0xd3:
            InterComm.WriteData(fun,data[0]);
            break;
        case 0x20:
            if(12==len){
                emit DataChanged(QHMIData::WBSR,(QByteArray((char *)data,4)));
                emit DataChanged(QHMIData::ZXHJSQ,htoni(*(unsigned short*)(data+4)));
                break;
            }
            else
                return;
        case 0x22:  case 0x23:
            InterComm.WriteData(fun,data[0],ntohs(*(unsigned short *)data));
            break;
        case 0x24:
            /////////////////////////////////////////////

        case 0x40:
            if((data[0]==0xaa)||(data[0]==0xbb)){
                emit DataChanged(QHMIData::GLWC,data[0]);  //系统归零完成
                Ack(0x4e,0x55);
                return;
            } else
                InterComm.WriteData(fun,data[0]);
            break;

        case 0x41:
            if(20==len){
                emit DataChanged(QHMIData::YXHXCL,(QByteArray(*data,14)));
                Ack(fun,0x55);
            }
            break;


        case 0x21:   //上发 机头相对针数
            if(8==len)
                emit DataChanged(QHMIData::JTXDZS,htons(*(unsigned short*)data));
            break;
        case 0x43:    //上发 报警信息
            if(7==len)
                emit DataChanged(QHMIData::CWBJXX,*data);
            break;
        case 0x4c:    //上发 停车状态
            if(7==len)
                emit DataChanged(QHMIData::DQZDJZT,*data);
            break;
        case 0x49:////?
        case 0x4a:
        case 0x4b:
            break;


        case 0x53:
            if(len>250)
                return;
            InterComm.WriteData(data,len-6);
            break;
        case 0x80:
        case 0x82:
            InterComm.WriteData(0x82,data[0]);
            break;
        case 0x81:
        case 0x83:
            InterComm.WriteData(fun,htons(*(unsigned short *)data));
            break;
        case 0x84:
            InterComm.WriteData(fun,*(unsigned char *)data);
            break;

        case 0xc1:
            if(funcode == 0xc0){
                InterComm.WriteData(0xc0,data[0]);
                InterComm.WriteAck(TRUE);
                return;
            }
            InterComm.WriteData(0xC1,data[0]);
            break;
        case 0xd4:
            InterComm.WriteData(fun,data[0],ntohs(*(unsigned short *)(data+1)));
            break;
        default:
            break;
        }
        if(fun == funcode)
            InterComm.WriteAck(TRUE);
        else
            InterComm.WriteAck(FALSE);
    }
}








