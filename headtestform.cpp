#include "headtestform.h"
#include "globaldata.h"
#include "QDebug"
#include "communicat.h"
#include "QGridLayout"
#include <QTest>
#include "qhmidata.h"
#include "config.h"

headTestForm::headTestForm(QComm *com,QHMIData *data,QWidget *parent) :
    QWidget(parent),pcom(com),hmidata(data),inautotesting(FALSE),
    timereventrecursion(FALSE){
    setupUi(this);

    //////////////////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    QWidget *widget = new QWidget(this);
    widget->setFixedSize(620,600);
    QGridLayout *layout = new QGridLayout(widget);
    head0 = new FormHeadTest(pcom,Md::SYSTEM1,Md::POSFRONT);
    head1 = new FormHeadTest(pcom,Md::SYSTEM1,Md::POSREAR);
    head[0]=head0;
    head[1]=head1;
    layout->addWidget(head1,0,0);
    layout->addWidget(head0,1,0);
    yajiaoprobe0 = new QLabel;
    yajiaoprobe0->setFixedSize(10,10);
    yajiao0 = new QPushButton;
    yajiao0->setText("压脚");
    yajiao0->setFixedSize(60,40);
    yajiao0->setCheckable(TRUE);
    QWidget *widgetyj0 = new QWidget;
    QHBoxLayout *layoutyj0 = new QHBoxLayout(widgetyj0);
    layoutyj0->addStretch();
    layoutyj0->addWidget(yajiaoprobe0);
    layoutyj0->addWidget(yajiao0);
    layoutyj0->addStretch();
    layout->addWidget(widgetyj0,2,0);

    yajiaoprobe1 = new QLabel;
    yajiaoprobe1->setFixedSize(10,10);
    yajiao1 = new QPushButton;
    yajiao1->setFixedSize(60,40);
    yajiao1->setText("后压脚");
    yajiao1->setCheckable(TRUE);
    QWidget *widgetyj1 = new QWidget;
    QHBoxLayout *layoutyj1 = new QHBoxLayout(widgetyj1);
    layoutyj1->addStretch();
    layoutyj1->addWidget(yajiao1);
    layoutyj1->addWidget(yajiaoprobe1);
    layoutyj1->addStretch();
    layout->addWidget(widgetyj1,2,1);

    shazuibuttonarray[0] = pushButton_s00;
    shazuibuttonarray[1] = pushButton_s01;
    shazuibuttonarray[2] = pushButton_s02;
    shazuibuttonarray[3] = pushButton_s03;
    shazuibuttonarray[4] = pushButton_s04;
    shazuibuttonarray[5] = pushButton_s05;
    shazuibuttonarray[6] = pushButton_s06;
    shazuibuttonarray[7] = pushButton_s07;
    shazuibuttonarray[8] = pushButton_s10;
    shazuibuttonarray[9] = pushButton_s11;
    shazuibuttonarray[10] = pushButton_s12;
    shazuibuttonarray[11] = pushButton_s13;
    shazuibuttonarray[12] = pushButton_s14;
    shazuibuttonarray[13] = pushButton_s15;
    shazuibuttonarray[14] = pushButton_s16;
    shazuibuttonarray[15] = pushButton_s17;
    for(int i=0;i<8;i++){
        signalmap.setMapping(shazuibuttonarray[i],i);
    }




#if DUAL_SYSTEM
    head2 = new FormHeadTest(pcom,Md::SYSTEM2,Md::POSFRONT);
    head3 = new FormHeadTest(pcom,Md::SYSTEM2,Md::POSREAR);
    head[2]=head2;
    head[3]=head3;
    layout->addWidget(head2,1,1);
    layout->addWidget(head3,0,1);
    for(int i=8;i<16;i++){
        signalmap.setMapping(shazuibuttonarray[i],i);
    }
#else
    for(int i=8;i<16;i++){
        shazuibuttonarray[i]->hide();
    }
#endif
    //connect(&signalmap,SIGNAL(mapped(int)),SLOT(shazuitest(int)));
    //connect(head0,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    //connect(head1,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    //connect(head2,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    //connect(head3,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    //////////////////////////////////////
}

void headTestForm::prepareToComm()
{
    head0->prepareToComm();
    head1->prepareToComm();
    head2->prepareToComm();
    head3->prepareToComm();
    for(int i=0;i<16;i++){
        connect(shazuibuttonarray[i],SIGNAL(clicked()),&signalmap,SLOT(map()));
    }
    connect(pushButton_sauto,SIGNAL(clicked()),SLOT(shazuitestauto()));
    connect(pushButton_pauto,SIGNAL(clicked()),SLOT(pinautotest()));
    connect(pushButton_dauto,SIGNAL(clicked()),SLOT(dumutestauto()));
    connect(pushButton_sjauto,SIGNAL(clicked()),SLOT(sanjiaotestauto()));
    //connect(pushButton_autotest,SIGNAL(clicked()),SLOT(autotest()));
    connect(yajiao0,SIGNAL(toggled(bool)),SLOT(yajiaotestF(bool)));
    connect(yajiao1,SIGNAL(toggled(bool)),SLOT(yajiaotestB(bool)));
    timerid = startTimer(800);
}

void headTestForm::shazuitest(int shazui)
{
    pcom->shazuiTest(1<<shazui,shazuibuttonarray[shazui]->isChecked());
}


void headTestForm::shazuitestauto(){
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
        pcom->shazuiTest(1<<i,1);
        QTest::qWait(200);
    }
    for(int i=0;i<NUM;i++){
        pcom->shazuiTest(1<<i,0);
        QTest::qWait(200);
    }
}

void headTestForm::dumutestauto()
{

    /*if(pcom->DumuMotorTest(0xff,500)==FALSE)
        return
    QTest::qWait(1000);
    if(pcom->DumuMotorTest(0xff,0)==FALSE)
        return
    QTest::qWait(1000);*/
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
      if(pcom->DumuMotorTest(1<<i,500)==FALSE)
          break;
      QTest::qWait(300);
      if(pcom->DumuMotorTest(1<<i,0)==FALSE)
          break;
      QTest::qWait(300);
    }
}

void headTestForm::sanjiaotestauto()
{
    pcom->sanjiaoMagneticTest(0xffffffff,0);
    QTest::qWait(100);
    pcom->sanjiaoMagneticTest(0xffffffff,1);
    QTest::qWait(100);
    pcom->sanjiaoMagneticTest(0xffffffff,0);
    QTest::qWait(100);
    pcom->sanjiaoMagneticTest(0xffffffff,1);
    QTest::qWait(100);
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(1<<i,0);
       QTest::qWait(100);
    }
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x100<<i,0);
       QTest::qWait(100);
    }
#if DUAL_SYSTEM
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x10000<<i,0);
       QTest::qWait(100);
    }
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x1000000<<i,0);
       QTest::qWait(100);
    }
#endif
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(1<<i,1);
       QTest::qWait(100);
    }
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x100<<i,1);
       QTest::qWait(100);
    }

#if DUAL_SYSTEM
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x10000<<i,1);
       QTest::qWait(100);
    }
    for(int i=0;i<6;i++){
       pcom->sanjiaoMagneticTest(0x1000000<<i,1);
       QTest::qWait(100);
    }
#endif
}

void headTestForm::pinautotest()
{
    pcom->pinTest(0xffffffffffffffffuLL,0);
    QTest::qWait(100);
    pcom->pinTest(0xffffffffffffffffuLL,1);
    QTest::qWait(100);
    pcom->pinTest(0xffffffffffffffffuLL,0);
    QTest::qWait(100);
    pcom->pinTest(0xffffffffffffffffuLL,1);
    QTest::qWait(100);
    for(int i=0;i<64;i++){
       pcom->pinTest((unsigned long long)1<<i,0);
       QTest::qWait(100);
    }
    for(int i=0;i<64;i++){
       pcom->pinTest((unsigned long long)1<<i,1);
       QTest::qWait(100);
    }
}

void headTestForm::autotest()
{

    /////////dumu test////////////////////////////////////////////
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,500)==FALSE)
        return
    QTest::qWait(1000);
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,0)==FALSE)
        return
    QTest::qWait(1000);
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
      if(pcom->DumuMotorTest(1<<i,200)==FALSE)
          break;
      QTest::qWait(1000);
      if(pcom->DumuMotorTest(1<<i,0)==FALSE)
          break;
      QTest::qWait(1000);
    }
    /////////dumu test finish///////////////////////////////////////////
    pcom->yajiaoTest(Md::POSFRONTREAR,1,0);
    QTest::qWait(1000);
    pcom->yajiaoTest(Md::POSFRONTREAR,2,0);
    QTest::qWait(1000);
    pcom->yajiaoTest(Md::POSFRONTREAR,1,0);
    QTest::qWait(1000);
    pinautotest();
    shazuitestauto();
    sanjiaotestauto();
}

void headTestForm::yajiaotestF(bool check)
{
    pcom->yajiaoTest(Md::POSFRONT,check?2:1,0);
}

void headTestForm::yajiaotestB(bool check)
{
    pcom->yajiaoTest(Md::POSREAR,check?2:1,0);
}


void headTestForm::on_pushButton_cancle_clicked()
{
    hide();
    hmidata->TogSysStat(QHMIData::SysRun);
    deleteLater();
}

void headTestForm::setshazuival(unsigned short val)
{
    unsigned char shazuival1 = val & 0xff;
    for(int i=0;i<8;i++){
        shazuibuttonarray[i]->setChecked(shazuival1&(1<<i));
    }
#if DUAL_SYSTEM
    unsigned char shazuival2 = (val & 0xff00)>>8;
    for(int i=0;i<8;i++){
        shazuibuttonarray[i+8]->setChecked(shazuival2&(1<<i));
    }
#endif
}

void headTestForm::timerEvent(QTimerEvent *e)
{
    //because of possible recursion call ,avoid from
    // stack overflow, add bool field "timereventrecursion"
    //to avod recursion call
    if(timereventrecursion)
        return;
    timereventrecursion = TRUE;

    if(e->timerId()==timerid){
        char buf[100];
        unsigned short len;
        if(pcom->ReadHead(buf,len)!=Md::Ok){
            timereventrecursion = FALSE;
            return;
        }

       ///////////head///////////////////
#if DUAL_SYSTEM
        const unsigned char NUM=4;
#else
        const unsigned char NUM=2;
#endif
        for(int i=0;i<NUM;i++){
            unsigned char dumuproberow = ((unsigned char)buf[0]<<4)|
                    ((unsigned char)buf[0]>>4);
            unsigned char dumuprobe = (dumuproberow&(0x03<<(2*i)))>>2*i;
            unsigned char sanjiao = buf[1+i];
            unsigned char lpin = buf[7+2*i];
            unsigned char rpin = buf[8+2*i];
            unsigned short ldumuval = ntohs(*(unsigned short*)(buf+15+4*i));
            unsigned short rdumuval = ntohs(*(unsigned short*)(buf+17+4*i));
            head[i]->setHeadData(dumuprobe,sanjiao,lpin,rpin,ldumuval,rdumuval);
            unsigned char yajiao = *(unsigned char*)(buf+31);
            if(yajiao&0x10)
                yajiaoprobe0->setStyleSheet("background-color:gray");
            else
                yajiaoprobe0->setStyleSheet("background-color:red");

            if(yajiao&0x20)
                yajiaoprobe1->setStyleSheet("background-color:gray");
            else
                yajiaoprobe1->setStyleSheet("background-color:red");
        }
        /////////shazui////////////////
        unsigned short shazui = buf[5]+(buf[6]<<8);
        setshazuival(shazui);
        /////////yajiao///////////////

    }
    timereventrecursion = FALSE;
}



