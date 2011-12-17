#include "formjqgzcs.h"
#include "qparam.h"
#include "qmdstyle.h"

FormJqgzcs::FormJqgzcs(QParam *data,QWidget *parent):
        QWidget(parent),param(data),mdstyle(new QMdStyle){
    setupUi(this);
    QWidget *widget[]={
        _1_spinBox    ,_2_spinBox    ,_3_spinBox    ,_4_spinBox ,_5_doubleSpinBox,
        _6_spinBox    ,_7_spinBox    ,_8_pushButton ,_9_spinBox ,_10_spinBox,
        _11_spinBox   ,_12_spinBox   ,_13_spinBox   ,_14_spinBox,_15_spinBox,
        _16_spinBox   ,_17_pushButton,_18_spinBox   ,_19_spinBox,_20_spinBox,
        _21_pushButton,_22_spinBox   ,_23_pushButton,_24_spinBox,_25_spinBox,
        _26_pushButton,_27_spinBox   ,_28_spinBox   ,_29_spinBox,_30_spinBox
    };

    for(int i=0;i<sizeof widget/sizeof widget[0];i++){
        int val;
        signalmapper.setMapping(widget[i],widget[i]);
        if(i<23)
            val = param->fechData(SpaItemHd_Jqgzcs,i+1);
        else
            val = param->fechData(SpaItemHd_Jqgzcs,i+2);
        switch(i){
        case 4:{
                QDoubleSpinBox *doublespinbox = qobject_cast<QDoubleSpinBox *>(widget[i]);
                doublespinbox->setValue(val/10.0);
                doublespinbox->setStyle(mdstyle);
                connect(doublespinbox,SIGNAL(valueChanged(double)),&signalmapper,SLOT(map()));
                break;
            }
        case 7:
        case 16:
        case 20:
        case 22:
        case 25:{
                QPushButton *pushbutton = qobject_cast<QPushButton *>(widget[i]);
                pushbutton->setChecked(val);
                connect(pushbutton,SIGNAL(toggled(bool)),&signalmapper,SLOT(map()));
                break;
            }
        default:{
                QSpinBox *spinbox = qobject_cast<QSpinBox *>(widget[i]);
                spinbox->setValue(val);
                spinbox->setStyle(mdstyle);
                connect(spinbox,SIGNAL(valueChanged(int)),&signalmapper,SLOT(map()));
                break;
            }
        }
    }
    connect(&signalmapper,SIGNAL(mapped(QWidget*)),SLOT(valeChanged(QWidget*)));

}


FormJqgzcs::~FormJqgzcs(){
    delete mdstyle;
}


void FormJqgzcs::valeChanged(QWidget *obj){
    QSpinBox *spinbox = qobject_cast<QSpinBox *>(obj);
    if(spinbox){
        int val = spinbox->value();
        int index = spinbox->property("index").toInt();
        param->setData(SpaItemHd_Jqgzcs,index,val);
        return;
    }
    QDoubleSpinBox *doublespinbox = qobject_cast<QDoubleSpinBox *>(obj);
    if(doublespinbox){
        int val =doublespinbox->value()*10;
        int index = doublespinbox->property("index").toInt();
        param->setData(SpaItemHd_Jqgzcs,index,val);
        return;
    }
    QPushButton *pushbutton = qobject_cast<QPushButton *>(obj);
    if(pushbutton){
        int val =pushbutton->isChecked();
        int index = pushbutton->property("index").toInt();
        param->setData(SpaItemHd_Jqgzcs,index,val);
        return;
    }
}
