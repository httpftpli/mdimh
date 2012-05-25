#include "formhead.h"
#include "pattern.h"
#include "qhmidata.h"
#include "qparam.h"
#include <QHBoxLayout>
#include <QStringList>

FormHead::FormHead(QPattern * data,QHMIData *hmi, QParam *p,QWidget *parent) :
        QWidget(parent),pattern(data),hmidata(hmi),param(p),cntnumber(1),
        pixmapright(":/image/resource/1w.png"),
        pixmapleft(":/image/resource/1x.png")
{
    setupUi(this);
    connect(hmidata,SIGNAL(hmi_cntNumber(unsigned short)),SLOT(onCntNumber(unsigned short)));
    connect(param,SIGNAL(changed()),SLOT(onParamChanded()));
    plablearray[0] = label_sz1;
    plablearray[1] = label_sz2;
    plablearray[2] = label_sz3;
    plablearray[3] = label_sz4;
    plablearray[4] = label_sz5;
    plablearray[5] = label_sz6;
    plablearray[6] = label_sz7;
    plablearray[7] = label_sz8;
}

void  FormHead::setKouAtribute(Md::POS_LEFTRIGHT kou){
    this->kou = kou;
}

void FormHead::onParamChanded(){
    onCntNumber(cntnumber);
}

void FormHead::onCntNumber(unsigned short cntnumber){    
    this->cntnumber = cntnumber;
    /////////花板行/////////////////////////////
    label_hbhh->setNum(pattern->cnt_huabanhang(cntnumber,kou,Md::POSREAR));
    label_hbhq->setNum(pattern->cnt_huabanhang(cntnumber,kou,Md::POSFRONT));
    ///////动作/////////////////////////////////////
     QStringList list  = pattern->cnt_ZhilingStringlist(cntnumber,kou,Md::POSREAR);
    label_awdzh->setText(list.at(0));
    label_hwdzh->setText(list.at(1));
    list = pattern->cnt_ZhilingStringlist(cntnumber,kou,Md::POSFRONT);
    label_awdzq->setText(list.at(0));
    label_hwdzq->setText(list.at(1));
    ////////色代号//////////////////////////////
    label_awsch->setText(pattern->cnt_seDaiHaoA(cntnumber,kou,Md::POSREAR));
    label_hwsch->setText(pattern->cnt_seDaiHaoH(cntnumber,kou,Md::POSREAR));
    label_awscq->setText(pattern->cnt_seDaiHaoA(cntnumber,kou,Md::POSFRONT));
    label_hwscq->setText(pattern->cnt_seDaiHaoH(cntnumber,kou,Md::POSFRONT));
    ////////沙嘴///////////////////////////////
    unsigned char sz = pattern->shaZui(cntnumber,kou);
    for(int i=0;i<8;i++)
        plablearray[i]->setText((sz&1<<i)?QString::number(i+1):"_");
    ///////////////////度目值/////////////////////////
    bool temp;
    unsigned char dumuzu = pattern->cnt_duMuZu(cntnumber,kou,temp,TRUE);
    unsigned char dumuzhibugongzuoh = param->duMuZhiBuGongZuo(cntnumber,kou,Md::POSREAR);
    unsigned char dumuzhibugongzuoq = param->duMuZhiBuGongZuo(cntnumber,kou,Md::POSFRONT);
    unsigned char dumuzhiworkh = pattern->duMuZhiWork(cntnumber,kou,Md::POSREAR,dumuzu);
    unsigned char dumuzhiworkq = pattern->duMuZhiWork(cntnumber,kou,Md::POSFRONT,dumuzu);
    if(cntnumber%2){//奇数行
        label_dmz_hz->setNum(dumuzhiworkh);
        label_dmz_qz->setNum(dumuzhiworkq);
        label_dmz_hy->setNum(dumuzhibugongzuoh);
        label_dmz_qy->setNum(dumuzhibugongzuoq);
    }else{
        label_dmz_hz->setNum(dumuzhibugongzuoh);
        label_dmz_qz->setNum(dumuzhibugongzuoq);
        label_dmz_hy->setNum(dumuzhiworkh);
        label_dmz_qy->setNum(dumuzhiworkq);
    }
    ///////////////////derection/////////////////////////
    if(cntnumber%2){//奇数行
        label_right->setPixmap(pixmapright);
        label_left->setPixmap(pixmapright);
    }else{
        label_right->setPixmap(pixmapleft);
        label_left->setPixmap(pixmapleft);
    }
}

void FormHead::paintEvent ( QPaintEvent * ){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
