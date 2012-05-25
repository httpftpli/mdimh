#include "alarmform.h"
#include <QStyleOption>
#include <QVBoxLayout>
#include<QLabel>
#include"qhmidata.h"
#include<QPainter>
#include<QCoreApplication>


AlarmForm::AlarmForm(QHMIData *d, QWidget *parent):
        QWidget(parent),hmidata(d),flashtimes(0){

    connect(d,SIGNAL(alarm(unsigned char)),SLOT(onAlarm(unsigned char)));
    connect(&timer,SIGNAL(timeout()),SLOT(onTimeOut()));
    setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    setFixedSize(200,100);
    sound.setFile(QCoreApplication::applicationDirPath()+QString("/resource/sound/alarm.wav"));
    timer.setInterval(700);
    timer.setSingleShot(FALSE);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(6,0,6,0);
    label = new QLabel;
    pushbutton = new QPushButton;
    connect(pushbutton,SIGNAL(clicked()),SLOT(onPushButtonClick()));
    pushbutton->setText(tr("清除"));
    pushbutton->setFixedSize(50,40);
    layout->addWidget(label);
    layout->addWidget(pushbutton);

}

void AlarmForm::onTimeOut(){
    if(flashtimes%2){
        label->setText(QString());
    }else{
        sound.play();
        label->setText(alarmstr);
    }
    flashtimes++;
    if(flashtimes==6){
        flashtimes = 0;
        hide();
        timer.stop();
    }
}

void AlarmForm::onAlarm(unsigned char code){
    alarmstr= hmidata->fetchAlarm();
    label->setText(alarmstr);
    timer.start();
    show();
    pushbutton->setEnabled(code!=59);
    move((800-width())/2,600-height()-100);
}

void AlarmForm::onPushButtonClick(){
    hmidata->clearAlarm();
    hide();
}

void AlarmForm::paintEvent ( QPaintEvent * ){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
