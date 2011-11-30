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



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint){
    setupUi(this);
    setup();
}

void MainWindow::onXtGuiling(unsigned char val){
    if(0xaa==val)//系统归零完成
        qMdPushButton_5->setChecked(TRUE);
    else if(0xbb==val){//系统归零错误
        qMdPushButton_5->setChecked(FALSE);
        QMdMessageBox box;
        box.exec(tr("系统复位"),tr("归零错误"),
                 QMessageBox::Warning,
                 QMessageBox::Cancel,
                 QMessageBox::Cancel);
    }
}


void MainWindow::setup(){
    label_left->hide();
    label_right->hide();
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
    connect(&hmiData,SIGNAL(hmi_direction(Md::DIRECTION)),this,
            SLOT(runDirectionChange(Md::DIRECTION)));
    connect(&hmiData,SIGNAL(hmi_jitouxiangduizhengshu(int)),label_zwz,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(xtGuiling(unsigned char)),SLOT(onXtGuiling(unsigned char)));

    pcheckBoxArray[0] = checkBox_1;
    pcheckBoxArray[1] = checkBox_2;
    pcheckBoxArray[2] = checkBox_3;
    pcheckBoxArray[3] = checkBox_4;
    pcheckBoxArray[4] = checkBox_5;
    pcheckBoxArray[5] = checkBox_6;
    pcheckBoxArray[6] = checkBox_7;
    pcheckBoxArray[7] = checkBox_8;
    for(int i=0;i<8;i++){
        pcheckBoxArray[i]->installEventFilter(this);
    }
    azllist<<tr("空")<<tr("翻针")<<tr("编织");
    hzllist<<tr("空")<<tr("吊目")<<tr("接针")<<tr("吊目2")<<tr("编松2");
}


void  MainWindow::slot_patternChange(){
    label_totalLine->setNum(patternData.tatalrow);
    label_patternFile->setText(patternData.patternName);
}

void MainWindow::runDirectionChange(Md::DIRECTION direction){
    if(direction==Md::LEFT){
        label_left->show();
        label_right->hide();
    }else{
        label_right->show();
        label_left->hide();
    }
}

void MainWindow::Timeout1s(){
    label_currenttime->setText(QDateTime::fromTime_t(hmiData.curruntTime).toString("yyyy-MM-dd hh:mm:ss"));
    label_timebianzhi->setText(secondToString(hmiData.runTimeHistory)+QChar('/')+secondToString(hmiData.runTime));
    label_timetingche->setText(secondToString(hmiData.stopTimeHistory)+QChar('/')+secondToString(hmiData.stopTime));
    //label_timebianzhi->setNum(QDateTime::f);
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

    /////////花板行/////////////////////////////
    label_hbhh->setNum(patternData.cnt_huabanhang_h1(cntnumber));
    label_hbhq->setNum(patternData.cnt_huabanhang_q1(cntnumber));

    ///////动作/////////////////////////////////////
    label_awdzh->setText(azllist.value(patternData._azl(patternData.cntFechData(cntnumber,CNT_S1H_AZiLing))));
    label_awdzq->setText(azllist.value(patternData._azl(patternData.cntFechData(cntnumber,CNT_S1Q_AZiLing))));
    label_hwdzh->setText(azllist.value(patternData._hzl(patternData.cntFechData(cntnumber,CNT_S1H_HZiLing))));
    label_hwdzq->setText(azllist.value(patternData._hzl(patternData.cntFechData(cntnumber,CNT_S1Q_HZiLing))));
    ////////色代号//////////////////////////////
    label_awsch->setText(patternData.cnt_seDaiHao(cntnumber,CNT_S1H_AColor));
    label_hwsch->setText(patternData.cnt_seDaiHao(cntnumber,CNT_S1H_HColor));
    label_awscq->setText(patternData.cnt_seDaiHao(cntnumber,CNT_S1Q_AColor));
    label_hwscq->setText(patternData.cnt_seDaiHao(cntnumber,CNT_S1Q_HColor));
    ////////沙嘴///////////////////////////////
    unsigned char sz1 = patternData.cnt_shaZui1(cntnumber);
    for(int i=0;i<8;i++)
        pcheckBoxArray[i]->setChecked(sz1&1<<i);
    ///////////////////度目值/////////////////////////
    if(cntnumber%2){//偶数数行        
        label_dmz_hz->setNum(paramaData.duMu_BuGongZuo(QParam::BackLeft));
        label_dmz_qz->setNum(paramaData.duMu_BuGongZuo(QParam::FrontLeft));
        label_dmz_hy->setNum(patternData.duMuZhi(TRUE,cntnumber));
        label_dmz_qy->setNum(patternData.duMuZhi(FALSE,cntnumber));

    }else{//奇数行
        label_dmz_hz->setNum(patternData.duMuZhi(TRUE,cntnumber));
        label_dmz_qz->setNum(patternData.duMuZhi(FALSE,cntnumber));
        label_dmz_hy->setNum(paramaData.duMu_BuGongZuo(QParam::BackRight));
        label_dmz_qy->setNum(paramaData.duMu_BuGongZuo(QParam::FrontRight));
    }
}

bool MainWindow::eventFilter ( QObject * watched, QEvent * event ){
    if(watched->parent()==this->frame_3){
        if((event->type()==QEvent::MouseButtonPress)||(event->type()==QEvent::MouseButtonDblClick))
            return TRUE;
    }
    return FALSE;
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

}

void MainWindow::on_qMdPushButton_6_clicked(bool checked)
{
    qMdPushButton_6->setChecked(!checked);
    int r = qSend.firstLineLoop(checked);
    if(r==Md::Ok)
        qMdPushButton_6->setChecked(checked);
    else
        qMdPushButton_6->setChecked(!checked);
}

void MainWindow::on_qMdPushButton_11_clicked(bool checked)
{
    qMdPushButton_11->setChecked(!checked);
    int r = qSend.sazuiDownUp(checked);
    if(r==Md::Ok)
        qMdPushButton_11->setChecked(checked);
    else
        qMdPushButton_11->setChecked(!checked);
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
    if(hmiData.clothFinishCount==0)
        return;
    QMdMessageBox box;
    box.setText(tr("完成件数清零"));
    box.setInformativeText(tr("是否清零?"));
    box.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    box.setDefaultButton(QMessageBox::Yes);
    box.setIcon(QMessageBox::Question);
    if(box.exec()==QMessageBox::Yes){
        hmiData.setclothFinishCount(0);
        hmiData.downloadParam();
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    DialogSetFinishCount *dialog = new DialogSetFinishCount(this);
    dialog->setFixedSize(280,160);
    dialog->move((width()-dialog->width())/2,100);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    if((dialog->exec()==QDialog::Accepted)&&
                (hmiData.clothSetCount!=dialog->val)){
        hmiData.setclothSetCount(dialog->val);
        hmiData.downloadParam();
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    on_qMdPushButton_23_clicked();
}


void MainWindow::on_qMdPushButton_8_clicked(bool checked)
{

}

void MainWindow::on_qMdPushButton_5_clicked()
{

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
