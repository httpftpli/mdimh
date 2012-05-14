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
#include "qmdmessagebox.h"
#include "qhmidata.h"
#include "data.h"
#include "qparam.h"
#include "qmdstyle.h"
#include "dialogsetfinishcount.h"
#include "dialogtingche.h"
#include "formhead.h"
#include "QTest"
#include "helpform.h"
#include "dialogabout.h"
#include"config.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),
    pixmapleft(":/image/resource/1x.png"),
    pixmapright(":/image/resource/1w.png")
{
    formheadl = new FormHead(&patternData,&hmiData,&paramaData,this);
#if DUAL_SYSTEM
    formheadr = new FormHead(&patternData,&hmiData,&paramaData,this);
    formheadl->setKouAtribute(Md::POSLEFT);
    formheadr->setKouAtribute(Md::POSRIGHT);
#endif
    setupUi(this);
    setup();
}


void MainWindow::setup(){
    cntnumber = 1;
    QWidget *widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addStretch(20);
    layout->addWidget(formheadl);
    layout->addStretch(20);
#if DUAL_SYSTEM
    layout->addWidget(formheadr);
    layout->addStretch(20);
    widget->setFixedSize(780,280);
    widget->move(10,80);
#else
    widget->setFixedSize(460,280);
    widget->move(170,80);
#endif

    connect(&hmiData,SIGNAL(time1sOuted()),this,SLOT(Timeout1s()));
    connect(&hmiData,SIGNAL(clothFinishCountChanged(int)),label_FinishCount,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(clothSetCountChanged(int)),label_setCount,SLOT(setNum(int)));
    connect(&patternData,SIGNAL(patternChanged(QString,QString)), SLOT(onpatternChange(QString,QString)));
    connect(&hmiData,SIGNAL(hmi_loopend(int)),label_loopEnd,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopleft(int)),label_loopLeft,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopStart(int)),label_loopStart,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopend(int)),label_loopEnd,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_loopTatal(int)),label_loopCount,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(hmi_cntNumber(unsigned short)),SLOT(runPatternRowChange(unsigned short)));
    connect(&hmiData,SIGNAL(hmi_jitouxiangduizhengshu(int)),label_zwz,SLOT(setNum(int)));
    connect(&hmiData,SIGNAL(xtGuilingError()),SLOT(onXtGuilingError()));
    connect(&hmiData,SIGNAL(xtRunOrGuiling(bool)),qMdPushButton_5,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(lineLock(bool)),qMdPushButton_6,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(speedLimit(bool)),qMdPushButton_8,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(stopPerOne(bool)),qMdPushButton_9,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(alarmLimit(bool)),qMdPushButton_10,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(shazuiUp(bool)),qMdPushButton_11,SLOT(setChecked(bool)));
    connect(&hmiData,SIGNAL(runing(bool)),SLOT(onHmidataRuning(bool)));
    connect(&hmiData,SIGNAL(powerDown()),SLOT(onPowerDown()));
    connect(&paramaData,SIGNAL(changed()),SLOT(onParamChanded()));
    azllist<<tr("空")<<tr("翻针")<<tr("编织");
    hzllist<<tr("空")<<tr("吊目")<<tr("接针")<<tr("吊目2")<<tr("编松2");
#if DUAL_SYSTEM
    connect(&hmiData,SIGNAL(sigDankouLock(bool)),SLOT(onDankouLock(bool)));
    label_dankoulock->setText(hmiData.dankouLock()?tr("锁定"):tr("不锁定"));
#else
    frame_dankoulock->hide();
#endif

}

void  MainWindow::onpatternChange(const QString &patterndir, const QString &patternname){
    Q_UNUSED(patterndir);
    label_totalLine->setNum(patternData.tatalcntrow);
    label_patternFile->setText(patternname);
}

void MainWindow::onHmidataRuning(bool val){
    QWidget *tab = tabWidget->widget(4);//test widget
    QObjectList list = tab->children();
    foreach(QObject *widget,list){
        if(widget->isWidgetType()){
            ((QWidget *)widget)->setDisabled(val);
        }
    }
    qMdPushButton_11->setDisabled(val);//shazui updown
}

void MainWindow::onPowerDown()
{
    hmiData.saveSysCfgFile();
    sysLog.save();
    qComm.quit();
    qComm.wait();
    QApplication::quit();
}

void MainWindow::Timeout1s(){
    label_currenttime->setText(QDateTime::fromTime_t(hmiData.curruntTime).toString("yyyy-MM-dd hh:mm:ss"));
    label_timebianzhi->setText(secondToString(hmiData.runTimeHistory)+QChar('/')+secondToString(hmiData.runTime));
    label_timetingche->setText(secondToString(hmiData.stopTimeHistory)+QChar('/')+secondToString(hmiData.stopTime));
}

void MainWindow::onParamChanded()
{
    runPatternRowChange(this->cntnumber);
}



#if DUAL_SYSTEM
void MainWindow::onDankouLock(bool lock){
    label_dankoulock->setText(lock?tr("锁定"):tr("不锁定"));
}
#endif

void MainWindow::runPatternRowChange(unsigned short cntnumber){
    this->cntnumber=cntnumber;
    bool temp;
    lcdNumber->display(cntnumber);
    label_dumu_l->setNum(patternData.cnt_duMuZu(cntnumber,Md::POSLEFT,temp,TRUE));
    label_dumu_r->setNum(patternData.cnt_duMuZu(cntnumber,Md::POSRIGHT,temp,TRUE));
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
    //yaochuang////////////////
    QPattern::YCPOSITION pos;
    signed char yaochuangval;
    patternData.cnt_yaoChuang(cntnumber,pos,yaochuangval);
    QString yaochuangstr;
    if(yaochuangval>0)
        yaochuangstr.append("R");
    else if(yaochuangval<0)
        yaochuangstr.append("L");
    yaochuangstr.append(QString::number(qAbs(yaochuangval)));
    if(pos==QPattern::ZHENDUIZHEN)
        yaochuangstr.append("*");
    if((pos==QPattern::FANZHENFU)||(pos==QPattern::FANZHENZHE)){
        QPalette pa;
        pa.setColor(QPalette::WindowText,Qt::red);
        label_yaochuang->setPalette(pa);
    }
    label_yaochuang->setText(yaochuangstr);
    //end yaochuang ///////////
    label_yajiao->setNum(patternData.cnt_yaJiao(cntnumber));
    label_tingche->setText(patternData.cnt_tingChe(cntnumber)?tr("是"):tr("否"));
    label_qizhengdian->setNum(patternData.wrk_qizhengdian());
    qMdPushButton_6->setEnabled(cntnumber==0);
}

void MainWindow::onXtGuilingError(){
    QMdMessageBox box;
    box.exec(tr("系统归零"),tr("归零错误"),QMessageBox::Warning,QMessageBox::Cancel,QMessageBox::Cancel);
}

void MainWindow::on_qMdPushButton_clicked()
{   QFormFile *formfile = new QFormFile(&patternData);
    formfile->show();
}

void MainWindow::on_qMdPushButton_3_clicked()
{
    if(!paramForm)
    //qDebug()<<"base time"<<QTime::currentTime().toString("ss-zzz");
        paramForm = QPointer<paramform>(new paramform(&qComm));
    //qDebug()<<"new paramform finish"<<QTime::currentTime().toString("ss-zzz");
    paramForm->setIndex(paramform::Index_cjp);
    paramForm->show();    
    //qDebug()<<"paramform.show"<<QTime::currentTime().toString("ss-zzz");
}

void MainWindow::on_qMdPushButton_2_clicked()
{
    machineexame *machineexameform  = new machineexame(&qComm,&hmiData);
    machineexameform->show();
    if(hmiData.TogSysStat(QHMIData::SysTest)==Md::Ok)
        machineexameform->prepareToComm();
    else{
        QMdMessageBox box;
        box.exec("测试模式","进入测试模式失败",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::Ok);
    }

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
    headTestForm *headtestForm = new headTestForm(&qComm,&hmiData);
    headtestForm->show();
    if(hmiData.TogSysStat(QHMIData::SysTest)==Md::Ok)
        headtestForm->prepareToComm();
    else{
        QMdMessageBox box;
        box.exec("测试模式","进入测试模式失败",QMessageBox::Warning,QMessageBox::Ok,QMessageBox::Ok);
    }
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
     hmiData.xtGuiling();
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
        paramForm = QPointer<paramform>(new paramform(&qComm));
    paramForm->setIndex(paramform::Index_dmz);
    paramForm->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!paramForm)
        paramForm = QPointer<paramform>(new paramform(&qComm));
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
        paramForm = QPointer<paramform>(new paramform(&qComm));
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
        paramForm = QPointer<paramform>(new paramform(&qComm));
    paramForm->setIndex(paramform::Index_sztf);
    paramForm->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    QFormFile *formfile = new QFormFile(&patternData);
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




void MainWindow::on_qMdPushButton_12_toggled(bool checked)
{
    qComm.rollTest(0,checked,0);
}

void MainWindow::on_pushButton_13_clicked()
{
    qComm.quit();
    qComm.wait();
    hmiData.saveSysCfgFile();
    sysLog.save();
    QApplication::quit();
}

void MainWindow::on_qMdPushButton_18_clicked()
{
    HelpForm *form = new HelpForm;
    form->loadUrl(QUrl("http://www.google.com.hk"));
    form->show();
}

void MainWindow::on_qMdPushButton_19_clicked()
{
    DialogAbout *about = new DialogAbout(&hmiData);
    about->exec();
}
