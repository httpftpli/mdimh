#include "formhead.h"
#include "pattern.h"
#include "qhmidata.h"
#include "qparam.h"
#include <QHBoxLayout>

FormHead::FormHead(QPatternData * data,QHMIData *hmi, QParam *p,QWidget *parent) :
        QWidget(parent),pattern(data),hmidata(hmi),param(p),cntnumber(0),
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
    azllist<<tr("空")<<tr("翻针")<<tr("编织");
    hzllist<<tr("空")<<tr("吊目")<<tr("接针")<<tr("吊目2")<<tr("编松2");

}

void  FormHead::setKouAtribute(unsigned char val){
    Q_ASSERT((val==Md::POSLEFT)||(val==Md::POSRIGHT));
    this->kou = val;
}

void FormHead::onParamChanded(){
    onCntNumber(cntnumber);
}

void FormHead::onCntNumber(unsigned short cntnumber){    
    this->cntnumber = cntnumber;
    unsigned char  sys;
#if DUAL_SYSTEM
    if(this->kou==Md::POSLEFT)  // left kou
         sys = (cntnumber%2)?0:1;
    else                                  //right kou
         sys = (cntnumber%2)?1:0;
#else
         sys = 0;
#endif
    /////////花板行/////////////////////////////
    label_hbhh->setNum(pattern->cnt_huabanhang(cntnumber,(Md::POS_LFETRIGHT)kou,Md::POSREAR));
    label_hbhq->setNum(pattern->cnt_huabanhang(cntnumber,(Md::POS_LFETRIGHT)kou,Md::POSFRONT));
    ///////动作/////////////////////////////////////
    label_awdzh->setText(azllist.value(pattern->cnt_Azhiling(cntnumber,sys,Md::POSREAR)));
    label_awdzq->setText(azllist.value(pattern->cnt_Azhiling(cntnumber,sys,Md::POSFRONT)));
    label_hwdzh->setText(azllist.value(pattern->cnt_Hzhiling(cntnumber,sys,Md::POSREAR)));
    label_hwdzq->setText(azllist.value(pattern->cnt_Hzhiling(cntnumber,sys,Md::POSFRONT)));
    ////////色代号//////////////////////////////
    label_awsch->setText(pattern->cnt_seDaiHaoA(cntnumber,sys,Md::POSREAR));
    label_hwsch->setText(pattern->cnt_seDaiHaoH(cntnumber,sys,Md::POSREAR));
    label_awscq->setText(pattern->cnt_seDaiHaoA(cntnumber,sys,Md::POSFRONT));
    label_hwscq->setText(pattern->cnt_seDaiHaoH(cntnumber,sys,Md::POSFRONT));
    ////////沙嘴///////////////////////////////
    unsigned char sz = pattern->shaZui(sys,cntnumber);
    for(int i=0;i<8;i++)
        plablearray[i]->setText((sz&1<<i)?QString::number(i+1):"_");
    ///////////////////度目值/////////////////////////
    /*if(cntnumber%2){//偶数数行
        label_dmz_hz->setNum(param->duMu_BuGongZuo(QParam::BackLeft));
        label_dmz_qz->setNum(param->duMu_BuGongZuo(QParam::FrontLeft));
        label_dmz_hy->setNum(pattern->duMuZhi(TRUE,cntnumber));
        label_dmz_qy->setNum(pattern->duMuZhi(FALSE,cntnumber));
        label_right->setPixmap(pixmapleft);
        label_left->setPixmap(pixmapleft);
    }else{//奇数行
        label_dmz_hz->setNum(pattern->duMuZhi(TRUE,cntnumber));
        label_dmz_qz->setNum(pattern->duMuZhi(FALSE,cntnumber));
        label_dmz_hy->setNum(param->duMu_BuGongZuo(QParam::BackRight));
        label_dmz_qy->setNum(param->duMu_BuGongZuo(QParam::FrontRight));
        label_right->setPixmap(pixmapright);
        label_left->setPixmap(pixmapright);
    }*/
}

void FormHead::paintEvent ( QPaintEvent * ){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
