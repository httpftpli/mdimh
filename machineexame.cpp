#include "machineexame.h"
#include "data.h"
#include "globaldata.h"
#include "qhmidata.h"
#include "qmdstyle.h"
#include "qmdmessagebox.h"
#include "communicat.h"
#include "mdLabel/qmdlabel.h"


machineexame::machineexame(QComm *comm, QWidget *parent) :
    QWidget(parent),timereventrecursion(FALSE),
    pcomm(comm){
    setupUi(this);
    /////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    spinBox->setSuffix("%");
    spinBox_2->setSuffix("%");
    spinBox_3->setPrefix("%");
    spinBox_3->setRange(1,100);
    spinBox_5->setPrefix("%");
    spinBox_5->setRange(1,100);
    spinBox_6->setPrefix("%");
    spinBox_6->setRange(1,100);

    wlabel[0] = label_w0;
    wlabel[1] = label_w1;
    wlabel[2] = label_w2;
    wlabel[3] = label_w3;
    wlabel[4] = label_w4;
    wlabel[5] = label_w5;
    wlabel[6] = label_w6;
    wlabel[7] = label_w7;
    wlabel[8] = label_w8;
    wlabel[9] = label_w9;
    wlabel[10] = label_w10;
    wlabel[11] = label_w11;
    wlabel[12] = label_w12;
    wlabel[13] = label_w13;
    wlabel[14] = label_w14;
    wlabel[15] = label_w15;
    wlabel[16] = label_w16;
    wlabel[17] = label_w17;
    wlabel[18] = label_w18;
    wlabel[19] = label_w19;
    wlabel[20] = label_w20;
    wlabel[21] = label_w21;
    wlabel[22] = label_w22;
    wlabel[23] = label_w23;
    wlabel[24] = label_w24;
    wlabel[25] = label_w25;
    wlabel[26] = label_w26;
    //wlabel[27] = label_w27;
    //wlabel[28] = label_w28;
    //wlabel[29] = label_w29;
    //wlabel[30] = label_w30;
    //wlabel[31] = label_w31;
    // wlabel[32] = label_w32;


    QMdStyle *style1 = new QMdStyle();
    style1->setParent(spinBox);
    spinBox->setStyle(style1);

    QMdStyle *style2 = new QMdStyle();
    style2->setParent(spinBox_2);
    spinBox_2->setStyle(style2);

    QMdStyle *style3 = new QMdStyle();
    style3->setParent(spinBox_3);
    spinBox_3->setStyle(style3);

    QMdStyle *style4 = new QMdStyle();
    style4->setParent(spinBox_4);
    spinBox_4->setStyle(style4);

    QMdStyle *style5 = new QMdStyle();
    style5->setParent(spinBox_5);
    spinBox_5->setStyle(style5);

    QMdStyle *style6 = new QMdStyle();
    style6->setParent(spinBox_6);
    spinBox_6->setStyle(style6);

    QMdStyle *style7 = new QMdStyle();
    style7->setParent(spinBox_7);
    spinBox_7->setStyle(style7);

    zsdsignalmap.setMapping(pushButton_zsdred,1);
    zsdsignalmap.setMapping(pushButton_zsdyellow,2);
    zsdsignalmap.setMapping(pushButton_zsdgreen,3);
    connect(&zsdsignalmap,SIGNAL(mapped(int)),SLOT(zsdtest(int)));
    ////////////////////////
}

void machineexame::prepareToComm()
{
    timerid  = startTimer(500);
    connect(pushButton_zsdred,SIGNAL(clicked()),&zsdsignalmap,SLOT(map()));
    connect(pushButton_zsdgreen,SIGNAL(clicked()),&zsdsignalmap,SLOT(map()));
    connect(pushButton_zsdyellow,SIGNAL(clicked()),&zsdsignalmap,SLOT(map()));
    connect(pushButton_luola,SIGNAL(clicked()),SLOT(luolaTest()));
    connect(pushButton_fuzuluola,SIGNAL(clicked()),SLOT(fuzuLuolaTest()));
    connect(pushButton_jt,SIGNAL(toggled(bool)),SLOT(headTest(bool)));
}


void machineexame::timerEvent ( QTimerEvent * event ){
    //because of possible recursion call ,avoid from
    // stack overflow, add bool field "timereventrecursion"
    //to avod recursion call
    if(timereventrecursion)
        return;
    timereventrecursion = TRUE;

    if(event->timerId() == timerid){
        char buf[16];
        unsigned char len;
        if(!pcomm->readDI(buf,len)){
            timereventrecursion = FALSE;
            return;
        }
        for(int i=0;i<26;i++){
            bool temp = (buf[INPUT_MAP[i][0]]&(1<<INPUT_MAP[i][1]))==0;
            wlabel[i]->setText(temp^(wlabel[i]->LogicReverse())?"闭合":"断开");
        }
    }
    timereventrecursion = FALSE;
    return;
}




void machineexame::zsdtest(int zsd)
{
    pcomm->LedTest(zsd);
}



void machineexame::on_pushButton_4_toggled(bool checked)
{
    if(!checked){
        pushButton_4->setText("脉冲数");
        spinBox->setSuffix("个脉冲");
        spinBox->setPrefix("");
        spinBox->setRange(1,255);
        luolapulsorpercent = 1;
    }
    else{
        pushButton_4->setText("百分比");
        spinBox->setSuffix("");
        spinBox->setPrefix("%");
        spinBox->setRange(1,100);
        luolapulsorpercent = 2;
    }
}

void machineexame::on_pushButton_6_toggled(bool checked)
{
     pushButton_6->setText(checked?"反向(张开)":"正向(闭合)");
     luoladir = checked?2:1;
}

void machineexame::on_pushButton_5_toggled(bool checked)
{
    pushButton_5->setText(checked?"反向(张开)":"正向(闭合)");
    fuzuluoladir = checked?2:1;
}


void machineexame::on_pushButton_7_toggled(bool checked)
{
    if(checked)
        pushButton_7->setText("右行");
    else
        pushButton_7->setText("左行");
    mainmotorspeed = checked?2:1;

}


void machineexame::headTest(bool checked)
{
    if(checked){
        pushButton_jt->setText("启动");
        pcomm->MainMotorTest(mainmotordir,mainmotorspeed);

    }
    else{
        pushButton_jt->setText("停车");
        pcomm->MainMotorTest(0,0);
    }
}


void machineexame::on_pushButton_cancle_clicked()
{

    hide();
    hmiData.TogSysStat(QHMIData::SysRun);
    delete this;
}


void machineexame::on_spinBox_3_valueChanged(int arg1)
{
    mainmotorspeed = arg1;
}


void machineexame::on_spinBox_valueChanged(int arg1)
{
    luolaval = arg1;
}

void machineexame::on_spinBox_2_valueChanged(int arg1)
{
    fuzuluolaval = arg1;
}

void machineexame::luolaTest()
{
    pcomm->rollTest(luolapulsorpercent,luoladir,luolaval);
}

void machineexame::fuzuLuolaTest()
{
    pcomm->AssistRollTest(fuzuluoladir,fuzuluolaval);
}
