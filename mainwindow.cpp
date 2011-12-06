#include "mainwindow.h"
#include "qformfile.h"
#include "paramform.h"
#include "communicat.h"
#include "machineexame.h"
#include "globaldata.h"
#include "patfileform.h"
#include "headtestform.h"
#include "updateromform.h"
#include "syslogform.h"
#include <QEvent>
#include "calibrationtestwidget.h"
#include "qhuabanxhform.h"
#include "qshazuiexchform.h"
#include "qshazuikbform.h"
#include "qcnteditform.h"
#include "formmovie.h"
#include "qmdmessagebox.h"
#include "qhmidata.h"
#include "data.h"
#include "qparam.h"
#include "qmdstyle.h"
#include "dialogsetfinishcount.h"
#include "dialogtingche.h"
#include "formhead.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),
    pixmapleft(":/image/resource/1x.png"),
    pixmapright(":/image/resource/1w.png")
{
    formhead1 = new FormHead(&patternData,&hmiData,&paramaData,this);
#if DUAL_SYSTEM
    formhead2 = new FormHead(&patternData,&hmiData,&paramaData,this);
#endif
    setupUi(this);
    setup();
}


void MainWindow::setup(){
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addStretch();
    layout->addWidget(formhead1);
    layout->addStretch();
#if DUAL_SYSTEM
    layout->addWidget(formhead2);
    layout->addStretch();
    widget->setFixedSize(780,280);
    widget->move(10,80);
#else
    widget->setFixedSize(460,280);
    widget->move(170,80);
#endif

    connect(&hmiData,SIGNAL(time1sOuted()),this,SLOT(Timeout1s()));
    connect(&hmiData,SIGNAL(clothFinishCountChanged(int)),label_FinishCount,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(clothSetCountChanged(int)),label_setCount,SLOT(setNum(int)));
    connect(&patternData,SIGNAL(patternChanged(const QString&,const QString&,const QString&,
                                               const QString&,const QString&)),
                                               SLOT(slot_patternChange()));
    connect(&hmiData,SIGNAL(hmi_loopend(int)),label_loopEnd,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopleft(int)),label_loopLeft,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopStart(int)),label_loopStart,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopend(int)),label_loopEnd,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopTatal(int)),label_loopCount,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_cntNumber(unsigned short)),this,
            SLOT(runPatternRowChange(unsigned short)));
    connect(&hmiData,SIGNAL(hmi_jitouxiangduizhengshu(int)),label_zwz,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(xtGuilingFinish(bool)),qMdPushButton_5,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(lineLock(bool)),qMdPushButton_6,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(speedLimit(bool)),qMdPushButton_8,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(stopPerOne(bool)),qMdPushButton_9,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(alarmLimit(bool)),qMdPushButton_10,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(shazuiUp(bool)),qMdPushButton_11,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(dankouLock(bool)),SLOT(onDankouLock(bool)));
    azllist<<tr("空")<<tr("翻针")<<tr("编织");
    hzllist<<tr("空")<<tr("吊目")<<tr("接针")<<tr("吊目2")<<tr("编松2");
#if DUAL_SYSTEM

#else
    frame_dankoulock->hide();
#endif

}


void  MainWindow::slot_patternChange(){
    label_totalLine->setNum(patternData.tatalrow);
    label_patternFile->setText(patternData.patternName);
}


void MainWindow::Timeout1s(){
    label_currenttime->setText(QDateTime::fromTime_t(hmiData.curruntTime).toString("yyyy-MM-dd hh:mm:ss"));
    label_timebianzhi->setText(secondToString(hmiData.runTimeHistory)+QChar('/')+secondToString(hmiData.runTime));
    label_timetingche->setText(secondToString(hmiData.stopTimeHistory)+QChar('/')+secondToString(hmiData.stopTime));
}

void MainWindow::onParamChanged(){

}

void MainWindow::onDankouLock(bool lock){
    label_dankoulock->setText(lock?tr("锁定"):tr("不锁定"));
}

void MainWindow::runPatternRowChange(unsigned short cntnumber){
    bool temp;
    lcdNumber->display(cntnumber+1);
    label_dumu->setNum(patternData.cnt_duMu(cntnumber,temp));
    int speed = patternData.cnt_spead(cntnumber);
    label_speed->setNum(speed);
    label_speedzhi->setNum(patternData.wrkSpeedZhi(speed));
    int mainluola = patternData.cnt_mainLuola(cntnumber);
    label_mainLuola->setNum(mainluola);
    label_mainLuolazhi->setNum(patternData.wrkMainLuolaZhi(mainluola));
    int fuzhuluola = patternData.cnt_fuzuLuola(cntnumber);
    label_fuzhuLuola->setNum(fuzhuluola);
    label_fuzhuLuolazhi->setNum(patternData.wrkFuzhuLuolaZhi(fuzhuluola));
    label_songsha->setNum(patternData.cnt_songSha(cntnumber));
    label_shazuiTf->setNum(patternData.cnt_shazuiTf(cntnumber));
    label_yaochuang->setText(patternData.cnt_yaoChuang(cntnumber));
    label_yajiao->setNum(patternData.cnt_yaJiao(cntnumber));
    label_tingche->setText(patternData.cnt_tingChe(cntnumber)?tr("是"):tr("否"));
    label_qizhengdian->setNum(patternData.wrk_qizhengdian());
}


void MainWindow::on_qMdPushButton_12_clicked()
{
    qRcv.quit();
    qRcv.wait();
    hmiData.saveSysCfgFile();
    sysLog.save();
    close();
}

void MainWindow::on_qMdPushButton_clicked()
{   QFormFile *formfile = new QFormFile;
    formfile->show();
}



void MainWindow::on_qMdPushButton_3_clicked()
{
    if(!paramForm)
    //qDebug()<<"base time"<<QTime::currentTime().toString("ss-zzz");
        paramForm = QPointer<paramform>(new paramform);
    //qDebug()<<"new paramform finish"<<QTime::currentTime().toString("ss-zzz");
    paramForm->setIndex(paramform::Index_cjp);
    paramForm->show();    
    //qDebug()<<"paramform.show"<<QTime::currentTime().toString("ss-zzz");
}

void MainWindow::on_qMdPushButton_2_clicked()
{
    machineexame *machineexameform  = new machineexame;
    machineexameform->show();


}


void MainWindow::on_qMdPushButton_7_clicked()
{

    //qMdPushButton_7->setShowBusySpin(TRUE);
    //qMdPushButton_7->startSpin();
    PatFileForm *patFileForm = new PatFileForm(NULL,&patternData);
    patFileForm->show();
    //qMdPushButton_7->endSpin();
    //qMdPushButton_7->setCursor(QCursor(Qt::BusyCursor));
    // FormMovie *formmovie = new FormMovie;

}

void MainWindow::on_qMdPushButton_13_clicked()
{

}

void MainWindow::on_qMdPushButton_4_clicked()
{
    headTestForm *headtestForm = new headTestForm;
    headtestForm->show();
}

void MainWindow::on_qMdPushButton_15_clicked()
{
    sysLogForm *form = new sysLogForm;
    form->show();
}

void MainWindow::on_qMdPushButton_14_clicked()
{
    updateRomForm *pupdateRomForm = new updateRomForm(this);
    pupdateRomForm->exec();
}

void MainWindow::on_qMdPushButton_5_clicked(bool checked)
{
    qMdPushButton_5->setChecked(!checked);
    if(!checked)
        hmiData.setRunOrGuiling(FALSE);
}

void MainWindow::on_qMdPushButton_6_clicked(bool checked)
{
    qMdPushButton_6->setChecked(!checked);
    hmiData.setLineLock(checked,TRUE);
}

void MainWindow::on_qMdPushButton_11_clicked(bool checked)
{
    qMdPushButton_11->setChecked(!checked);
    hmiData.setShazuiUp(checked,TRUE);
}

void MainWindow::on_qMdPushButton_21_clicked()
{
#ifdef  Q_WS_QWS
    calibrationtestwidget *calwidget=new calibrationtestwidget;
    calwidget->show();
#endif
}

void MainWindow::on_qMdPushButton_23_clicked()
{
    QHuabanXhForm *huabanxhform = new QHuabanXhForm();
    huabanxhform->move((width()-huabanxhform->width())/2,30);
    huabanxhform->exec();
}

void MainWindow::on_qMdPushButton_17_clicked()
{
    QShazuiKbForm *shazuikbform = new QShazuiKbForm();
    shazuikbform->move(150,100);
    shazuikbform->exec();
}

void MainWindow::on_qMdPushButton_22_clicked()
{
    QShazuiExchForm *shazuiexchform = new QShazuiExchForm(NULL,&patternData);
    shazuiexchform->show();
}

void MainWindow::on_qMdPushButton_24_clicked()
{
    QCntEditForm *cnteditform = new QCntEditForm(NULL,&patternData);
    cnteditform->show();
}

void MainWindow::on_pushButton_clicked()
{
    if(!paramForm)
        paramForm = QPointer<paramform>(new paramform);
    paramForm->setIndex(paramform::Index_dmz);
    paramForm->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!paramForm)
        paramForm = QPointer<paramform>(new paramform);
    paramForm->setIndex(paramform::Index_llzzdj);
    paramForm->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    on_pushButton_2_clicked();
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!paramForm)
        paramForm = QPointer<paramform>(new paramform);
    paramForm->setIndex(paramform::Index_ss);
    paramForm->show();
}


void MainWindow::on_pushButton_12_clicked()
{
#if DUAL_SYSTEM
    hmiData.toggleDankouLock();
#endif
}

void MainWindow::on_pushButton_6_clicked()
{
    if(!paramForm)
        paramForm = QPointer<paramform>(new paramform);
    paramForm->setIndex(paramform::Index_sztf);
    paramForm->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    QFormFile *formfile = new QFormFile;
        formfile->show();
}

void MainWindow::on_pushButton_9_clicked()
{
    if(hmiData.clothFinishCount()==0)
        return;
    QMdMessageBox box;
    box.setText(tr("完成件数清零"));
    box.setInformativeText(tr("是否清零?"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setDefaultButton(QMessageBox::Yes);
    box.setIcon(QMessageBox::Question);
    if(box.exec()==QMessageBox::Yes){
        hmiData.setclothFinishCount(0,TRUE);
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    DialogSetFinishCount *dialog = new DialogSetFinishCount(this);
    dialog->setFixedSize(280,160);
    dialog->move((width()-dialog->width())/2,100);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    if((dialog->exec()==QDialog::Accepted)&&
                (hmiData.clothSetCount()!=dialog->val))
        hmiData.setclothSetCount(dialog->val,TRUE);
}

void MainWindow::on_pushButton_10_clicked()
{

    on_qMdPushButton_23_clicked();
}


void MainWindow::on_qMdPushButton_8_clicked(bool checked)
{
    qMdPushButton_8->setChecked(!checked);
    hmiData.setSpeedLimit(checked,TRUE);
}


void MainWindow::on_pushButton_7_clicked()
{
    DialogTingche *dialog = new DialogTingche(&patternData);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setWindowTitle(tr("停车编辑"));
    dialog->move((width()-dialog->width())/2,100);
    dialog->exec();
}

void MainWindow::on_pushButton_11_clicked()
{
    on_pushButton_2_clicked();
}

void MainWindow::on_qMdPushButton_9_clicked(bool checked)
{
    qMdPushButton_9->setChecked(!checked);
    hmiData.setStopPerOne(checked,TRUE);
}

void MainWindow::on_qMdPushButton_10_clicked(bool checked)
{
    qMdPushButton_10->setChecked(!checked);
    hmiData.setAlarmLimit(checked,TRUE);
}




