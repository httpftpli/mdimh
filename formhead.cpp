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

void FormHead::onParamChanded(){
    onCntNumber(cntnumber);
}

void FormHead::onCntNumber(unsigned short cntnumber){
    this->cntnumber = cntnumber;
    /////////花板行/////////////////////////////
    label_hbhh->setNum(pattern->cnt_huabanhang_h1(cntnumber));
    label_hbhq->setNum(pattern->cnt_huabanhang_q1(cntnumber));
    ///////动作/////////////////////////////////////
    label_awdzh->setText(azllist.value(pattern->_azl(pattern->cntFechData(cntnumber,CNT_S1H_AZiLing))));
    label_awdzq->setText(azllist.value(pattern->_azl(pattern->cntFechData(cntnumber,CNT_S1Q_AZiLing))));
    label_hwdzh->setText(azllist.value(pattern->_hzl(pattern->cntFechData(cntnumber,CNT_S1H_HZiLing))));
    label_hwdzq->setText(azllist.value(pattern->_hzl(pattern->cntFechData(cntnumber,CNT_S1Q_HZiLing))));
    ////////色代号//////////////////////////////
    label_awsch->setText(pattern->cnt_seDaiHao(cntnumber,CNT_S1H_AColor));
    label_hwsch->setText(pattern->cnt_seDaiHao(cntnumber,CNT_S1H_HColor));
    label_awscq->setText(pattern->cnt_seDaiHao(cntnumber,CNT_S1Q_AColor));
    label_hwscq->setText(pattern->cnt_seDaiHao(cntnumber,CNT_S1Q_HColor));
    ////////沙嘴///////////////////////////////
    unsigned char sz1 = pattern->cnt_shaZui1(cntnumber);
    for(int i=0;i<8;i++)
        plablearray[i]->setText((sz1&1<<i)?QString::number(i+1):"_");
    ///////////////////度目值/////////////////////////
    if(cntnumber%2){//偶数数行
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
    }
}

void FormHead::paintEvent ( QPaintEvent * ){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
