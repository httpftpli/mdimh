#ifndef COMMUNICAT_H
#define COMMUNICAT_H
#include"namespace.h"

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



class QComm :public QThread{
   Q_OBJECT
public:
   QComm(QObject *parent = 0);
   void MainMotorTest(unsigned char direction,unsigned char speedpercent);
   bool DumuMotorTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr,
                      Md::POSFLAG_LFETRIGHT motor,unsigned short val);
   bool DumuMotorTest(unsigned char motor, unsigned short val);
   bool pinTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr,Md::POSFLAG_LFETRIGHT lf,
                unsigned char pin,unsigned char val);
   bool pinTest(unsigned long long pin,unsigned char stat);
   bool shazuiTest(Md::SYSTEMFLAG sys,unsigned char shazui, unsigned char stat);
   bool shazuiTest(unsigned short shazui, unsigned char stat);
   bool sanjiaoMagneticTest(Md::SYSTEMFLAG sys,Md::POSFLAG_FRONTREAR fr, unsigned char magnet,
                            unsigned long stat);
   bool sanjiaoMagneticTest(unsigned int sanjiao,unsigned char stat);
   void bedMotorTest(unsigned char flag,unsigned short pos,unsigned short param);
   void rollTest(unsigned char flag,unsigned char derection,unsigned char rollpercent);
   void AssistRollTest(unsigned char derection,unsigned char rollpercent);
   void LedTest(unsigned char ledstat);
   int readDI(char *buf, unsigned char &len);
   void BedReposition();
   void LeftMuslin(unsigned char force,bool clockwise = TRUE);
   void RightMuslin(unsigned char force,bool clockwise = TRUE);
   int ReadAbsoluteNoOfPin(unsigned short &val);
   int ReadEncoder(unsigned short &val);
   int ReadHead(char *buf,unsigned short &len);
   void ClearError();
   int lineLock(bool lock);
   int goToLine(unsigned short line);
   int sazuiDownUp(bool up);
   int sazuiReplaceSwap(bool replaceorswap,unsigned char leftsazui,unsigned char rightsazui);
   int cntUpdate(unsigned short line,unsigned char *buf);
   int patUpdate(unsigned short line,unsigned char *buf,unsigned short patlinelen);
   int headStopAside();
   int pollSysVersion(QString &mainbordVersion,QString &bagVersion);
   int TogSysStat(unsigned char stat);
   int sendParamaInRun(unsigned short setcount,unsigned short finishcount,unsigned char RateLimit,
                    unsigned char OneStop,unsigned char alarmLimit,unsigned char DanKouLock);
   int SendFile(const QString &filepath,unsigned short fileid,bool samehint,QWidget *parent);
   int SendShazuiKb(const QString &sazfilepath=QString());
   int SendParama(QFile &wrkfile,  QFile &spafile,int packet,QWidget *parent=NULL);
   int paramaUpdata(unsigned char id,unsigned short *buf,int len,bool halfwordorbyte);
   int paramaUpdata(unsigned char yiyincunzhenshu,unsigned short chijvjiaozheng,unsigned short zongzhengshu);
   int IsInBoot();
   int SendBin(QFile &binfile,QWidget *parent=NULL);
   int SendBag(QString &bagfilename,QWidget *parent = NULL);
   int checkCustomerId(unsigned short id);


private:
   QMutex mutex1,mutex2;
#pragma pack(1)
   char d_send[800];
#pragma pack()
   bool iswaitforack;
   bool isacked;
   bool interruppackrcved;
   char sendfuncode;
   char requirefuncode;
   unsigned char witchsendbuf;
   QElapsedTimer time;
   QUdpSocket udpsocket;
   static const   unsigned char TIMEROFSEND = 3;
   static const   long COMMTIMEOUT = 400;
   QUdpSocket *pudpsocket;
   QByteArray *rcvrawbuf;    //raw rcvbuf used to recieve ethernet data;
   char ackbuf[256];
   unsigned short ackbuflen;
   char rcvbuf[256];         //used to save communication protocal data;
   unsigned short rcvbuflen;           //used to save len of the rcvbuf
   void readshorts(QDataStream &stream,unsigned short * buf,int count);
   void readshorts(QDataStream &stream,unsigned char * buf,int count);
   void acktoctrl(unsigned char fun,unsigned char data); //called by ReadPendingDatagrams ,
                                                   //run in secondanary thread
   bool writeackdata(char *buf,unsigned short len);
   void checkwritercvdata(char *buf,unsigned short len);
   void beforesend();
   void terminatesend();
   void send();
   bool rcv(unsigned char funcode,unsigned char &val,unsigned short timeout);
   bool programsend();

   bool programsend(unsigned char &ackval1);
   bool programsend(unsigned char &ackval1,unsigned char &ackval2);
   bool programsend(unsigned char &ackval1,unsigned char &ackval2,
                    unsigned char &ackval3);
   bool programsend(char *buf,unsigned short &len);
signals:
   void commTimerOut(unsigned char funcode);
   void CommFail();
   void commPercent(int percent);
   void DataChangedFromCtrl(unsigned short index,QVariant val);
private slots:
   void ReadPendingDatagrams(); //derection conneted by run(),run in the run thread
protected:
   virtual void run();

};

#endif

