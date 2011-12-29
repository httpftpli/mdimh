#include "formheadtest.h"
#include "communicat.h"
#include <QTest>

FormHeadTest::FormHeadTest(QComm *com, Md::SYSTEMFLAG s, Md::POSFLAG_FRONTREAR r,QWidget *parent):
    QWidget(parent),pcom(com),sys(s),fr(r)
{
    setupUi(this);
    leftpinarray[0] = pushButton_p10;
    leftpinarray[1] = pushButton_p11;
    leftpinarray[2] = pushButton_p12;
    leftpinarray[3] = pushButton_p13;
    leftpinarray[4] = pushButton_p14;
    leftpinarray[5] = pushButton_p15;
    leftpinarray[6] = pushButton_p16;
    leftpinarray[7] = pushButton_p17;

    rightpinarray[0] = pushButton_p20;
    rightpinarray[1] = pushButton_p21;
    rightpinarray[2] = pushButton_p22;
    rightpinarray[3] = pushButton_p23;
    rightpinarray[4] = pushButton_p24;
    rightpinarray[5] = pushButton_p25;
    rightpinarray[6] = pushButton_p26;
    rightpinarray[7] = pushButton_p27;

    sanjiaoarray[0] = pushButton_s0;
    sanjiaoarray[1] = pushButton_s1;
    sanjiaoarray[2] = pushButton_s2;
    sanjiaoarray[3] = pushButton_s3;
    sanjiaoarray[4] = pushButton_s4;
    sanjiaoarray[5] = pushButton_s5;


    for(int i=0;i<8;i++){
        lpsignalmap.setMapping(leftpinarray[i],i);
        rpsignalmap.setMapping(rightpinarray[i],i);
    }
    for(int i=0;i<6;i++){
        sanjiaosignalmap.setMapping(sanjiaoarray[i],i);
    }
    dumusignalmap.setMapping(pushButton_d0,(int)Md::POSLEFT);
    dumusignalmap.setMapping(pushButton_d1,(int)Md::POSRIGHT);
    connect(&lpsignalmap,SIGNAL(mapped(int)),SLOT(lpintest(int)));
    connect(&rpsignalmap,SIGNAL(mapped(int)),SLOT(rpintest(int)));
    connect(&sanjiaosignalmap,SIGNAL(mapped(int)),SLOT(sanjiaotest(int)));
    connect(&dumusignalmap,SIGNAL(mapped(int)),SLOT(dumutest(int)));

    QString text =QString((sys==Md::SYSTEM1)?"1":"2")+QString("系统")+
            QString((fr==Md::POSFRONT)?"前":"后")+QString("床");
    label_2->setText(text);
}

void FormHeadTest::prepareToComm()
{
    for(int i=0;i<8;i++){
        connect(leftpinarray[i],SIGNAL(clicked()),&lpsignalmap,SLOT(map()));
        connect(rightpinarray[i],SIGNAL(clicked()),&rpsignalmap,SLOT(map()));
    }
    for(int i=0;i<6;i++)
        connect(sanjiaoarray[i],SIGNAL(clicked()),&sanjiaosignalmap,SLOT(map()));
    connect(pushButton_d0,SIGNAL(clicked()),&dumusignalmap,SLOT(map()));
    connect(pushButton_d1,SIGNAL(clicked()),&dumusignalmap,SLOT(map()));
    connect(pushButton_p1auto,SIGNAL(clicked()),SLOT(lpingtestauto()));
    connect(pushButton_p2auto,SIGNAL(clicked()),SLOT(rpingtestauto()));
    connect(pushButton_sauto,SIGNAL(clicked()),SLOT(sanjiaotestauto()));
}

void FormHeadTest::lpintest(int pin)
{
    pcom->pinTest(sys,fr,Md::POSLEFT,1<<pin,leftpinarray[pin]->isChecked());
}

void FormHeadTest::rpintest(int pin)
{
    pcom->pinTest(sys,fr,Md::POSRIGHT,1<<pin,rightpinarray[pin]->isChecked());
}

void FormHeadTest::sanjiaotest(int sanjiao)
{  
    pcom->sanjiaoMagneticTest(sys,fr,1<<sanjiao,sanjiaoarray[sanjiao]->isChecked());
}

void FormHeadTest::dumutest(int dumu)
{
    QPushButton *button = (dumu==Md::POSREAR)?pushButton_d1:pushButton_d0;
    pcom->DumuMotorTest(sys,fr,(Md::POSFLAG_LFETRIGHT)dumu,button->isChecked()?500:0);
}

void FormHeadTest::lpingtestauto()
{
    emit autoTesting(TRUE);
    for(int i=0;i<8;i++){
        if(pcom->pinTest(sys,fr,Md::POSLEFT,1<<i,1)==FALSE)
            break;
        QTest::qWait(500);
    }
    for(int i=0;i<8;i++){
        if(pcom->pinTest(sys,fr,Md::POSLEFT,1<<i,0)==FALSE)
            break;
        QTest::qWait(500);
    }
    emit autoTesting(FALSE);
}

void FormHeadTest::rpingtestauto()
{
    emit autoTesting(TRUE);
    for(int i=0;i<8;i++){
        if(pcom->pinTest(sys,fr,Md::POSRIGHT,1<<i,1)==FALSE)
            break;
        QTest::qWait(500);
    }
    for(int i=0;i<8;i++){
        if(pcom->pinTest(sys,fr,Md::POSRIGHT,1<<i,0)==FALSE)
            break;
        QTest::qWait(500);
    }
    emit autoTesting(FALSE);
}

void FormHeadTest::sanjiaotestauto()
{
    emit autoTesting(TRUE);
    for(int i=0;i<6;i++){
        if(pcom->sanjiaoMagneticTest(sys,fr,1<<i,1)==FALSE)
            break;
        QTest::qWait(500);
    }
    for(int i=0;i<6;i++){
        if(pcom->sanjiaoMagneticTest(sys,fr,1<<i,0)==FALSE)
            break;
        QTest::qWait(500);
    }
    emit autoTesting(FALSE);
}

