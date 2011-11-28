#include "machineexame.h"
#include "data.h"
#include "globaldata.h"
#include "qhmidata.h"




machineexame::machineexame(QWidget *parent) :
    QWidget(parent){
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
    timer.setSingleShot(FALSE);
    timer.start(250);
    connect(this,SIGNAL(DataChange(unsigned short,QVariant)),
            &hmiData,SLOT(On_DataChanged_FromHMI(unsigned short,QVariant)));
    connect(&hmiData,SIGNAL(DataChanged_ToHMI(unsigned short,QVariant)),this,SLOT(On_DataChanged(unsigned short,QVariant)));

    connect(&timer,SIGNAL(timeout()),this,SLOT(On_timerout()));
            ////////////////////////
}

void machineexame::On_timerout(){
   /* hmiData.RequireData(QHMIData::WBSR);*/
}

void machineexame::On_DataChanged(unsigned short index,QVariant Val){
    switch(index){
    case QHMIData::WBSR:{
        int row =  (reinterpret_cast<QGridLayout *>(groupBox->layout()))->rowCount();
        int colum = (reinterpret_cast<QGridLayout *>(groupBox->layout()))->columnCount();
        int cnt = 0;
        QMdLabel *lable;
        bool temp ;
        for(int i=1;i<colum;i=i+3){
            for(int j=0;j<row;j++){
                if ((reinterpret_cast<QGridLayout *> (groupBox->layout()))->itemAtPosition(j,i)!=NULL){
                   lable = reinterpret_cast<QMdLabel *>((reinterpret_cast<QGridLayout *>(groupBox->layout()))->itemAtPosition(j,i)->widget());
                   temp = ((Val.toByteArray().at(INPUT_MAP[cnt][0]))&(1<<INPUT_MAP[cnt][1]))==0;
                   lable->setText(temp^(lable->LogicReverse())?"闭合":"断开");
                   cnt++;
                   if(25==cnt)
                       break;
                }
            }

        }
    }
        break;
    case QHMIData::ZXHJSQ:
        label_60->setText(QString::number(hmiData.dataBuf[QHMIData::ZXHJSQ].Val));
        break;
    default:
        break;
    }
}


void machineexame::on_qMdPushButton_toggled(bool checked)
{
    if(checked){
        emit DataChange(QHMIData::ZSD,(unsigned char)1);

    }
}

void machineexame::on_qMdPushButton_3_toggled(bool checked)
{
    if(checked){
        emit DataChange(QHMIData::ZSD,(unsigned char)3);
    }
}

void machineexame::on_qMdPushButton_2_toggled(bool checked)
{
    if(checked){
        emit DataChange(QHMIData::ZSD,(unsigned char)2);
    }
}

void machineexame::on_qMdPushButton_4_toggled(bool checked)
{
    if(!checked){
        qMdPushButton_4->setText("脉冲数");
        spinBox->setSuffix("个脉冲");
        spinBox->setPrefix("");
        spinBox->setRange(1,255);
    }
    else{
        qMdPushButton_4->setText("百分比");
        spinBox->setSuffix("");
        spinBox->setPrefix("%");
        spinBox->setRange(1,100);

    }

}

void machineexame::on_qMdPushButton_6_toggled(bool checked)
{
    if(checked)
        qMdPushButton_6->setText("反向");
    else
        qMdPushButton_6->setText("正向");

}

void machineexame::on_qMdPushButton_5_toggled(bool checked)
{
    if(!checked)
        qMdPushButton_5->setText("反向(张开)");
    else
        qMdPushButton_5->setText("正向(闭合)");

}


void machineexame::on_qMdPushButton_7_toggled(bool checked)
{
    if(checked)
        qMdPushButton_7->setText("右行");
    else
        qMdPushButton_7->setText("左行");

}

void machineexame::on_spinBox_valueChanged(int  )
{

}

void machineexame::on_spinBox_2_valueChanged(int )
{

}

void machineexame::on_spinBox_6_valueChanged(int val)
{
    emit DataChange(QHMIData::ZSS,(unsigned char) (val));
}

void machineexame::on_spinBox_5_valueChanged(int val)
{
    emit DataChange(QHMIData::YSS,(unsigned char)(val));
}

void machineexame::on_qMdPushButton_9_toggled(bool checked)
{
    if(checked){
        qMdPushButton_9->setText("启动");
        emit DataChange(QHMIData::JTDJ,(unsigned short)((((unsigned char)qMdPushButton_7->isChecked())+1)<<8|
                                                        (unsigned char)spinBox_3->value())
                       );

    }
    else{
        qMdPushButton_9->setText("停车");
        emit DataChange(QHMIData::JTDJ,(unsigned short)0);
    }

}


void machineexame::on_qMdPushButton_10_clicked()
{
    emit DataChange(QHMIData::BJLL,(unsigned int)((unsigned char)qMdPushButton_4->isChecked()<<24|
                                                  (unsigned char)qMdPushButton_6->isChecked()<<16|
                                                  (unsigned char)spinBox_3->value()<<8
                                                 )
                    );
}

void machineexame::on_qMdPushButton_11_clicked()
{
    emit DataChange(QHMIData::FZLL,(unsigned short)((unsigned char)qMdPushButton_5->isChecked()<<8|
                                                    (unsigned char)spinBox_2->value()
                                                   )
                    );
}

void machineexame::on_qMdPushButton_8_clicked()
{

    hide();
    delete this;
}

