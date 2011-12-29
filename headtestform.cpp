#include "headtestform.h"
#include "globaldata.h"
#include "QDebug"
#include"communicat.h"
#include "QGridLayout"
#include<QTest>

headTestForm::headTestForm(QComm *com,QWidget *parent) :
    QWidget(parent),qcom(com),inautotesting(FALSE){
    setupUi(this);

    //////////////////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    QWidget *widget = new QWidget(this);
    widget->setFixedSize(620,600);
    QGridLayout *layout = new QGridLayout(widget);
    head0 = new FormHeadTest(qcom,Md::SYSTEM1,Md::POSFRONT);
    head1 = new FormHeadTest(qcom,Md::SYSTEM1,Md::POSREAR);
    head2 = new FormHeadTest(qcom,Md::SYSTEM2,Md::POSFRONT);
    head3= new FormHeadTest(qcom,Md::SYSTEM2,Md::POSREAR);
    layout->addWidget(head1,0,0);
    layout->addWidget(head3,0,1);
    layout->addWidget(head0,1,0);
    layout->addWidget(head2,1,1);

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
#if DUAL_SYSTEM
#else
    for(int i=8;i<16;i++){
        shazuibuttonarray[i]->hide();
    }
#endif

    for(int i=0;i<16;i++){
        signalmap.setMapping(shazuibuttonarray[i],i);
    }

    connect(&signalmap,SIGNAL(mapped(int)),SLOT(shazuitest(int)));
    connect(head0,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    connect(head1,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    connect(head2,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
    connect(head3,SIGNAL(autoTesting(bool)),SLOT(setDisabled(bool)));
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
    connect(pushButton_autotest,SIGNAL(clicked()),SLOT(autotest()));
}

void headTestForm::shazuitest(int shazui)
{
    pcom->shazuiTest(1<<shazui,shazuibuttonarray[shazui]->isChecked());
}


void headTestForm::shazuitestauto(){
    inautotesting = TRUE;
    setDisabled(TRUE);
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
        pcom->shazuiTest(1<<i,1);
        QTest::qWait(500);
    }
    for(int i=0;i<NUM;i++){
        pcom->shazuiTest(1<<i,0);
        QTest::qWait(500);
    }
    inautotesting = FALSE;
    setDisabled(FALSE);
}

void headTestForm::dumutestauto()
{
    inautotesting = TRUE;
    setDisabled(TRUE);
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,1)==FALSE)
        return
    QTest::qWait(500);
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,0)==FALSE)
        return
    QTest::qWait(500);
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
      if(pcom->DumuMotorTest(1<<i,500)==FALSE)
          break;
      QTest::qWait(500);
      if(pcom->DumuMotorTest(1<<i,0)==FALSE)
          break;
      QTest::qWait(500);
    }
    inautotesting = FALSE;
    setDisabled(FALSE);
}

void headTestForm::autotest()
{
    inautotesting = TRUE;
    setDisabled(TRUE);
    /////////dumu test////////////////////////////////////////////
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,1)==FALSE)
        return
    QTest::qWait(500);
    if(pcom->DumuMotorTest(Md::SYSTEMALL,Md::POSFRONTREAR,Md::POSLEFTRIGHT,0)==FALSE)
        return
    QTest::qWait(500);
#ifdef DUAL_SYSTEM
    const int NUM = 16;
#else
    const int NUM = 8;
#endif
    for(int i=0;i<NUM;i++){
      if(pcom->DumuMotorTest(1<<i,500)==FALSE)
          break;
      QTest::qWait(500);
      if(pcom->DumuMotorTest(1<<i,0)==FALSE)
          break;
      QTest::qWait(500);
    }
     /////////dumu test finish///////////////////////////////////////////
    inautotesting = TRUE;
    setDisabled(FALSE);

}


void headTestForm::on_pushButton_cancle_clicked()
{
    hide();
    deleteLater();
}
