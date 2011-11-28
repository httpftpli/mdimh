#ifndef COMMUNICAT_H
#define COMMUNICAT_H

#include "algorithm.h"
#include <QThread>
#include <QTimer>
#include <QVariant>
#include <QUdpSocket>
#include <QFile>
#include <QMutex>
#include <QElapsedTimer>

#ifdef ARM6410
#define LOCAL_HOST_ADDR  ("192.168.1.40")
#define LOCAL_HOST_PORT  1028
#define DEST_HOST_ADDR   ("192.168.1.11")
#define DEST_HOST_PORT   1025
#else
#define LOCAL_HOST_ADDR  ("192.168.1.40")
#define LOCAL_HOST_PORT  1028
#define DEST_HOST_ADDR   ("192.168.1.11")
#define DEST_HOST_PORT   1025
#endif

// 短整型大小端互换
#define htons(A)        ((((unsigned short)(A) & 0xff00) >> 8) | \
(((unsigned short)(A) & 0x00ff) << 8))
#define ntohs(A) htons(A)

// 长整型大小端互换
#define ntohl(A)        ((((unsigned long)(A) & 0xff000000) >> 24) | \
(((unsigned long)(A) & 0x00ff0000) >> 8) | \
(((unsigned long)(A) & 0x0000ff00) << 8) | \
(((unsigned long)(A) & 0x000000ff) << 24))
#define  htonl(A) ntohl(A)

#define  ntohi(A)  ((((unsigned int)(A) & 0xff000000) >> 24) | \
(((unsigned int)(A) & 0x00ff0000) >> 8) | \
(((unsigned int)(A) & 0x0000ff00) << 8) | \
(((unsigned int)(A) & 0x000000ff) << 24))

#define  htoni(A) ntohi(A)

class QInterComm{
public:
    QInterComm():packetrcved(FALSE){
        pmutex = new QMutex;
    }
    ~QInterComm(){
        delete pmutex;
    }
    void WriteAck(bool ackornot);
    void WriteData(unsigned char funcode,unsigned short ackval1,unsigned short ackval2=0);
    void WriteData(unsigned char *buf,unsigned char len);
    void Clear();
    bool Rcv(bool &isack,unsigned char &funcode,unsigned short &val1);
    bool Rcv(bool &isack,unsigned char &funcode,unsigned short &val1,unsigned short &val2);
    bool Rcv(bool &isack, char *buf,unsigned char &len);
    bool IsAcked();
    bool IsAcked(unsigned char &funcode,unsigned short &val);
    bool IsAcked(unsigned short &val);
    bool IsAcked(unsigned short &val1,unsigned short &val2);
    bool IsAcked(char *buf ,unsigned char &len);
private:
   volatile bool packetrcved;
   volatile bool isack;
   volatile unsigned char funcode;
   volatile unsigned short val,val2;
   char buf[256];
   volatile unsigned char len;
    QMutex *pmutex;
};



class QSend :public QObject{
   Q_OBJECT
public:
   enum SendResult{SendFileOk = 0,FileNotExist,NotPatCntSaz,CommucationError,FileSame,
                   FileOpenFail,InBootState,InUserState,EraseError,BurnError,BurnOK,
                   BagCanNotInBoot,WaitingForShoutDown,SendOk,CustomerIdNotPass,CustomerIdPass};
   QSend(QObject *parent = 0);
   void Send();
   bool ProgramSend();
   bool ProgramSend(unsigned short &ackval);
   bool ProgramSend(char *buf,unsigned char &len);
   bool __ProgramSend(unsigned short &ackval);
   void MainMotorTest(unsigned char direction,unsigned char speedpercent);
   void DumuMotorTest(unsigned char motor,const unsigned char (&coord)[8]);
   void pinTest(unsigned long long pin, unsigned long long  stat);
   void muslinMagneticTest(unsigned short muslinMag,unsigned short stat);
   void trigoneMagneticTest(unsigned long trigoneMag,unsigned long stat);
   void bedMotorTest(unsigned char flag,unsigned short pos,unsigned short param);
   void StepRollTest(unsigned char flag,unsigned char derection,unsigned char rollpercent);
   void AssistRollTest(unsigned char derection,unsigned char rollpercent);
   void LedTest(unsigned char ledstat);
   void readDI();
   void BedReposition();
   void LeftMuslin(unsigned char force,bool clockwise = TRUE);
   void RightMuslin(unsigned char force,bool clockwise = TRUE);
   SendResult ShoutDownSystem();
   SendResult ReadAbsoluteNoOfPin(unsigned short &val);
   SendResult ReadEncoder(unsigned short &val);
   SendResult ReadHead();
   void ClearError();
   SendResult firstLineLoop(bool loop);
   SendResult goToLine(unsigned short line);
   SendResult sazuiDownUp(bool up);
   SendResult sazuiReplaceSwap(bool replaceorswap,unsigned char leftsazui,unsigned char rightsazui);
   SendResult cntUpdate(unsigned short line,unsigned char *buf);
   SendResult patUpdate(unsigned short line,unsigned char *buf,unsigned short patlinelen);
   SendResult headStopAside();
   SendResult pollSysVersion(QString &mainbordVersion,QString &bagVersion);
   SendResult TogSysStat(unsigned char stat);
   SendResult sendParamaInRun(unsigned short setcount,unsigned short finishcount,unsigned char RateLimit,
                    unsigned char OneStop,unsigned char alarmLimit,unsigned char DanKouLock);
   SendResult SendFile(QFile &file,unsigned short fileid,bool samehint,QWidget *parent);
   SendResult SendParama(  QFile &wrkfile,  QFile &spafile,int packet,QWidget *parent=NULL);
   SendResult IsInBoot();
   SendResult SendBin(QFile &binfile,QWidget *parent=NULL);
   SendResult SendBag(QString &bagfilename,QWidget *parent = NULL);
   SendResult checkCustomerId(unsigned short id);




private:
#pragma pack(1)
   unsigned char d_send[532];
#pragma pack()
  // char *p_d_send;
   QElapsedTimer time;
   QUdpSocket udpsocket;
   unsigned char TIMEROFSEND;
   long COMMTIMEOUT;
   void readshorts(QDataStream &stream,unsigned short * buf,int count);
   void readshorts(QDataStream &stream,unsigned char * buf,int count);
signals:
   void commTimerOut(unsigned char funcode);
   void NotifyRcver(unsigned char fun,bool addordel);
   void CommFail();
public slots:
   void TimeOutSend();
signals:
   void commPercent(int percent);

};


/*class QRcv :public QObject{
    Q_OBJECT
public:
    void Rcv();
protected:
    virtual void run();
public slots:
    void On_NotifyRcver(unsigned char fun,bool addordel);
    void ReadPendingDatagrams(); //derection conneted by run(),run in the run thread
    void Ack(unsigned char fun,unsigned char data);
private:
    QUdpSocket *pudpsocket;
    unsigned char funcode;

signals:
    void DataChanged(unsigned short index,QVariant val);
};*/



class QRcv :public QThread{
    Q_OBJECT
public:
    QRcv():funcode(0){ }
    void Rcv();
protected:
    virtual void run();
public slots:
    void On_NotifyRcver(unsigned char fun,bool addordel);
    void ReadPendingDatagrams(); //derection conneted by run(),run in the run thread
    void Ack(unsigned char fun,unsigned char data);
private:
    QUdpSocket *pudpsocket;
    unsigned char funcode;

signals:
    void DataChanged(unsigned short index,QVariant val);
};


#endif

