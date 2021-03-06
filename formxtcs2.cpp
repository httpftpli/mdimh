#include "formxtcs2.h"tcs.h"
#include "qparam.h"
#include "qmdstyle.h"
#include "communicat.h"
#include <QStatusTipEvent>

FormXtcs2::FormXtcs2(QParam *paramdata, QComm *comm, QWidget *parent) :
        QWidget(parent),param(paramdata),pcomm(comm),mdstyle(new QMdStyle){
    setupUi(this);
    QWidget *widget[]={
        _01_spinBox  ,_02_spinBox  ,_03_spinBox   ,_04_spinBox   ,_05_spinBox_2,
        _06_spinBox  ,_07_spinBox  ,_08_doubleSpinBox_2   ,_09_doubleSpinBox   ,_10_doubleSpinBox,
        _11_spinBox_2  ,_12_spinBox  ,_13_spinBox   ,_14_pushButton   ,_15_spinBox,
        _16_spinBox  ,_17_spinBox  ,_18_pushButton   ,_19_spinBox   ,_20_spinBox,
        _21_pushButton_2,_22_pushButton,_23_pushButton,_24_spinBox
    };
    for(unsigned int i=0;i<sizeof widget/sizeof widget[0];i++){
        int val = param->fechData(QParam::SpaItemHd_Xtcs,i);
        signalmapper.setMapping(widget[i],widget[i]);
        switch(i){
        case 13:
        case 17:
        case 20 ... 22:{
                QPushButton *pushbutton = qobject_cast<QPushButton *>(widget[i]);
                pushbutton->setChecked(val);
                connect(pushbutton,SIGNAL(toggled(bool)),&signalmapper,SLOT(map()));
                break;
            }
        case 7 ... 9:{
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

FormXtcs2::~FormXtcs2(){
    delete mdstyle;
}



void FormXtcs2::valeChanged(QWidget *obj){
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
        if(index == 7)
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
