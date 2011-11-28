#include "systest.h"
#include "globaldata.h"
#include "qhmidata.h"


QSysTest::QSysTest(QObject *parent) : QObject(parent),linenumber(0)
{
    timer1s.setInterval(1000);
    timer1s.setSingleShot(FALSE);
    connect(&timer1s,SIGNAL(timeout()),SLOT(timeout1S()));
    connect(this,SIGNAL(yunxinhuanxiang(unsigned short,QVariant)),
            &hmiData,SLOT(On_DataChanged_FromCtrl(unsigned short,QVariant)));
    connect(this,SIGNAL(jitouxiangduizhengshu(unsigned short,QVariant)),
            &hmiData,SLOT(On_DataChanged_FromCtrl(unsigned short,QVariant)));

}

void QSysTest::timeout1S(){
    linenumber++;
    bool dir;
    dir = linenumber%2?0:1;
    if(linenumber == patternData.tatalrow)
        linenumber=1;
    char data[14];
    for(int i=0;i<14;i++)
        data[i]=0;
    *(unsigned short*)(data+2)= htons(linenumber);
    data[6]=dir;

    *(unsigned short *)(data+4)= htons(linenumber%4);//剩余循环数
    *(unsigned short *)(data+7)= htons(linenumber);//循环首
    *(unsigned short *)(data+9)= htons(linenumber%2);//循环尾
    *(unsigned short *)(data+0)= htons(linenumber%3);//设定循环数
    *(unsigned short *)(data+12)=htons(linenumber/10);//完成件数
    emit yunxinhuanxiang(QHMIData::YXHXCL,QByteArray(data,14));
    emit jitouxiangduizhengshu(QHMIData::JTXDZS,linenumber%2?100:200);

}

void QSysTest::start(){
    timer1s.start();
}
