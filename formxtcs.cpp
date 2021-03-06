#include "formxtcs.h"
#include "qparam.h"
#include "qmdstyle.h"
#include "communicat.h"
#include <QStatusTipEvent>

FormXtcs::FormXtcs(QParam *paramdata,QComm *comm, QWidget *parent) :
    QWidget(parent),param(paramdata),pcomm(comm),mdstyle(new QMdStyle){
    setupUi(this);
    QWidget *widget[]={
        _01_spinBox  ,_02_spinBox  ,_03_spinBox   ,_04_spinBox   ,_05_spinBox,
        _06_doubleSpinBox  ,_07_doubleSpinBox     ,_08_doubleSpinBox   ,_09_spinBox   ,_10_spinBox,
        _11_spinBox  ,_12_spinBox  ,_13_spinBox   ,_14_pushButton,_15_spinBox,
        _16_spinBox  ,_17_spinBox, _18_pushButton ,_19_pushButton,_20_pushButton,
        _21_spinBox
    };
    for(int i=0;i<sizeof widget/sizeof widget[0];i++){
        int val = param->fechData(QParam::SpaItemHd_Xtcs,i);
        signalmapper.setMapping(widget[i],widget[i]);
        switch(i){
        case 13:
        case 17 ... 19:{
                QPushButton *pushbutton = qobject_cast<QPushButton *>(widget[i]);
                pushbutton->setChecked(val);
                connect(pushbutton,SIGNAL(toggled(bool)),&signalmapper,SLOT(map()));
                break;
            }
        case 5 ... 7:{
                QDoubleSpinBox *doublespinbox = qobject_cast<QDoubleSpinBox *>(widget[i]);
                doublespinbox->setValue(val/10);
                doublespinbox->setStyle(mdstyle);
                connect(doublespinbox,SIGNAL(valueChanged(double)),&signalmapper,SLOT(map()));
                break;
            }
        default:{
                QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget[i]);
                (11==i)?spinbox->setValue((unsigned short)val):spinbox->setValue(val);
                spinbox->setStyle(mdstyle);
                connect(spinbox,SIGNAL(valueChanged(int)),&signalmapper,SLOT(map()));
                break;
            }
        }
    }
    connect(&signalmapper,SIGNAL(mapped(QWidget*)),SLOT(valeChanged(QWidget*)));
}

FormXtcs::~FormXtcs(){
    delete mdstyle;
}


void FormXtcs::valeChanged(QWidget *obj){
    QSpinBox *spinbox = qobject_cast<QSpinBox *>(obj);
    if(spinbox){
        int val = spinbox->value();
        int index = spinbox->property("index").toInt();
        param->setData(QParam::SpaItemHd_Xtcs,index,val,FALSE);
        if((index == 11)||(index ==12))
            param->updataPivotal();
        if(14==index)
            pcomm->yajiaoTest(Md::POSFRONT,3,val); //shengke buchang
        if(15==index)
            pcomm->yajiaoTest(Md::POSREAR,3,val);  //shengke buchang
        return;
    }
    QDoubleSpinBox *doublespinbox = qobject_cast<QDoubleSpinBox *>(obj);
    if(doublespinbox){
        int val =qRound(doublespinbox->value()*10);
        int index = doublespinbox->property("index").toInt();
        param->setData(QParam::SpaItemHd_Xtcs,index,val,FALSE);
        if(index==5)
            param->updataPivotal();
        return;
    }
    QPushButton *pushbutton = qobject_cast<QPushButton *>(obj);
    if(pushbutton){
        int val =pushbutton->isChecked();
        int index = pushbutton->property("index").toInt();
        param->setData(QParam::SpaItemHd_Xtcs,index,val,FALSE);
        return;
    }

}
