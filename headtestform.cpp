#include "headtestform.h"
#include "globaldata.h"
#include "QDebug"
#include"communicat.h"

headTestForm::headTestForm(QWidget *parent) :
    QWidget(parent){
    setupUi(this);

    //////////////////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    for(int i =0;i<64;i++){
        //ppinArray[i]=findChild<QMdPushButton *>(QString("qMdPushButton_0%1").arg(i,2,10,'0'));
    }

    QList<QMdPushButton *> buttons = findChildren<QMdPushButton *>();
    int size = buttons.size();
    for(int i=0;i<size;i++){
        QWidget *button = buttons.at(i);
        button->installEventFilter(this);

    }

    //////////////////////////////////////
}

bool headTestForm::eventFilter(QObject *target, QEvent *event){
    QString str = target->objectName();

    if((str.right(1)=="P")&(event->type()==QEvent::MouseButtonPress)){
        if(static_cast<QMouseEvent *>(event)->button()==Qt::LeftButton){
            int i = str.right(3).left(2).toInt();
            bool temp = qobject_cast<QMdPushButton *>(target)->isChecked();
            qComm.pinTest(1uLL<<i,((unsigned long long)!temp)<<i);
        }
    }//选针器
    if((str.right(1)=="S")&(event->type()==QEvent::MouseButtonPress)){
        if(static_cast<QMouseEvent *>(event)->button()==Qt::LeftButton){
            int i = str.right(3).left(2).toInt();
            bool temp = qobject_cast<QMdPushButton *>(target)->isChecked();
            qComm.muslinMagneticTest((unsigned  short)1<<i,((unsigned short)!temp)<<i);
        }
    }//纱嘴
    if((str.right(1)=="J")&(event->type()==QEvent::MouseButtonPress)){
        if(static_cast<QMouseEvent *>(event)->button()==Qt::LeftButton){
            int i = str.right(3).left(2).toInt();
            bool temp = qobject_cast<QMdPushButton *>(target)->isChecked();
            qComm.trigoneMagneticTest(1uL<<i,((unsigned long)!temp)<<i);
        }
    }//纱嘴


    return QWidget::eventFilter(target,event);
}


void headTestForm::on_qMdPushButton_3_clicked()
{
    this->deleteLater();
}
