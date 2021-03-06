#include "paramform.h"
#include "data.h"
#include "globaldata.h"
#include <QFlags>
#include"config.h"
#include"communicat.h"
#include "formjqgzcs.h"
#if DUAL_SYSTEM
#include "formxtcs2.h"
#else
#include "formxtcs.h"
#endif

paramform::paramform(QComm *com, QWidget *parent) :
    QWidget(parent,Qt::FramelessWindowHint),patterndata(NULL),paramadata(NULL),dmzmodel(NULL),llmodel(NULL),
        zdjmodel(NULL),ssmodel(NULL),sztfmodel(NULL),ytxszxzmodel(NULL),
        ytxsztfmodel(NULL),cjpmodel(NULL),pzkmodel(NULL),
        szkbmodel(NULL),dmbcmodel(NULL),ycwzxzmodel(NULL),
        fzycwzxzmodel(NULL),formjqgzcs(NULL),formxtcs(NULL),
        pcomm(com){
    //qDebug()<<"enter paramform construction fuc"<<QTime::currentTime().toString("ss-zzz");
    setupUi(this);
    //qDebug()<<"setupUi(this)"<<QTime::currentTime().toString("ss-zzz");
    ////////////////////////////
    setPattern(&patternData);
    //qDebug()<<"setPattern:"<<QTime::currentTime().toString("ss-zzz");
    setParama(&paramaData);
    label_13->setText(patterndata->wrkFileName);
    label_11->setText(paramadata->spafilename);
    //qDebug()<<"patterndata.loadfile"<<QTime::currentTime().toString("ss-zzz");
    //qDebug()<<"set model"<<QTime::currentTime().toString("ss-zzz");
    ///////////////////////////////
    connect(this,SIGNAL(indexchanged(int)),stackedWidget,SLOT(setCurrentIndex(int)));
    connect(this,SIGNAL(indexchanged(int)),SLOT(indexchange(int)));
    //connect(paramadata,SIGNAL(dirty(bool)),pushButton_spareset,SLOT(setEnabled(bool)));

    timer.setInterval(400); //timer for read absolute pin position
    timer.setSingleShot(FALSE);
    connect(&timer,SIGNAL(timeout()),SLOT(readabspinpos())); //when in xtcs form timer start,
                                                             //otherwise timer stop

    //QStatusBar *statusbar = new QStatusBar;
    //statusbar->
}

paramform::~paramform(){
    //this->paramadata->releaseBuf();
}

void paramform::setPattern(QPattern *pattern){
    this->patterndata = pattern;
}


void paramform::setParama(QParam *parama){
    this->paramadata = parama;
}

void paramform::on_qMdPushButton_clicked()
{
    paramadata->updataAll();
    patterndata->wrk_updataAll();
    deleteLater();
}



void paramform::on_stackedWidget_currentChanged(int yyy )
{
    switch(yyy){
    case 0:
        if(dmzmodel==NULL){
            dmzmodel = new QDMZModel(patterndata,this);
            tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView->setModel(dmzmodel);
#if DUAL_SYSTEM
            tableView->setGeometry(tableView->x(),tableView->y(),
                                   450,tableView->height());
#else
            tableView->setGeometry(tableView->x(),tableView->y(),
                                   250,tableView->height());
            label_sys2l->hide();
            label_sys2r->hide();
#endif

            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_DuMuZi);
            int valtop =QPattern::wrkItemValTop(QPattern::WrkItemHd_DuMuZi);
            delegate->setRange(valbottom,valtop);
            tableView->setItemDelegate(delegate);
        }
        break;
   case 1:
        if(llmodel==NULL){
            llmodel = new QLLModel(patterndata,this);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_JuanBu);
            int valtop =QPattern:: wrkItemValTop(QPattern::WrkItemHd_JuanBu);
            delegate->setRange(valbottom,+valtop);
            tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_2->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_2->setModel(llmodel);
            tableView_2->setItemDelegate(delegate);

            zdjmodel = new QZDJModel(patterndata,this);
            QMdItemDelegate *delegate2 = new QMdItemDelegate(this);
            int valbottom1 =QPattern::wrkItemValBottem(QPattern::WrkItemHd_SuDuZi);
            int valtop1 = QPattern::wrkItemValTop(QPattern::WrkItemHd_SuDuZi);
            delegate2->setRange(valbottom1,valtop1);
            tableView_3->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_3->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_3->setItemDelegate(delegate2);
            tableView_3->setModel(zdjmodel);
        }
        break;
   case 2:
        if(ssmodel ==NULL){
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_LeftSongSa);
            int valtop = QPattern::wrkItemValTop(QPattern::WrkItemHd_LeftSongSa);
            delegate->setRange(valbottom,valtop);
            ssmodel = new QSSModel(patterndata,this);
            tableView_4->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_4->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_4->setItemDelegate(delegate);
            tableView_4->setModel(ssmodel);

        }
        break;
   case 3:
        if(sztfmodel ==NULL){
            sztfmodel = new QSZTFModel(patterndata,this);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_ShaZuiTF);
            int valtop =QPattern::wrkItemValTop(QPattern::WrkItemHd_ShaZuiTF);
            delegate->setRange(valbottom,valtop);
            tableView_5->setItemDelegate(delegate);
            tableView_5->setModel(sztfmodel);
            tableView_5->setSpan(0,0,1,2);
            tableView_5->setSpan(0,2,1,2);
            tableView_5->setSpan(0,4,1,2);
            tableView_5->setSpan(0,6,1,2);
            tableView_5->setSpan(0,8,1,2);
            tableView_5->setSpan(0,10,1,2);
            tableView_5->setSpan(0,12,1,2);
            tableView_5->setSpan(0,14,1,2);
            tableView_5->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_5->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
        }
        break;
   case 4:
        if(ytxszxzmodel ==NULL){
            ytxszxzmodel = new QYTXSZXZModel(patterndata,this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_YTXXiuZen);
            int valtop =QPattern::wrkItemValTop(QPattern::WrkItemHd_YTXXiuZen);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            delegate->setRange(valbottom,valtop);
            tableView_11->setItemDelegate(delegate);
            tableView_11->setModel(ytxszxzmodel);
            tableView_11->setSpan(0,0,1,2);
            tableView_11->setSpan(0,2,1,2);
            tableView_11->setSpan(0,4,1,2);
            tableView_11->setSpan(0,6,1,2);
            tableView_11->setSpan(0,8,1,2);
            tableView_11->setSpan(0,10,1,2);
            tableView_11->setSpan(0,12,1,2);
            tableView_11->setSpan(0,14,1,2);
            tableView_11->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_11->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
        }
        if(ytxsztfmodel ==NULL){
            ytxsztfmodel = new QYTXSZTFModel(patterndata,this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_YTXTingFang);
            int valtop = QPattern::wrkItemValTop(QPattern::WrkItemHd_YTXTingFang);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            delegate->setRange(valbottom,valtop);
            tableView_12->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_12->setItemDelegate(delegate);
            tableView_12->setModel(ytxsztfmodel);
        }
        break;
   case 5:
        if(cjpmodel==NULL){
            cjpmodel = new QCJPModel(patterndata,this);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_CJP_BianZi);
            int valtop =QPattern::wrkItemValTop(QPattern::WrkItemHd_CJP_BianZi);
            delegate->setRange(valbottom,valtop);
            tableView_13->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_13->setItemDelegate(delegate);
            tableView_13->setModel(cjpmodel);
            tableView_13->setSpan(0,0,1,4);
            tableView_13->setSpan(0,4,1,4);
            tableView_13->setSpan(1,0,1,2);
            tableView_13->setSpan(1,2,1,2);
            tableView_13->setSpan(1,4,1,2);
            tableView_13->setSpan(1,6,1,2);
            tableView_13->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
            tableView_13->verticalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
            tableView_13->verticalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);

        }
        break;
   case 6:{
        if(pzkmodel==NULL){
            pzkmodel = new QPZKModel(patterndata,8,this);            
            tableView_pzk->setModel(pzkmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QPattern::wrkItemValBottem(QPattern::WrkItemHd_PzkSaZui);
            int valtop = QPattern::wrkItemValTop(QPattern::WrkItemHd_PzkSaZui);
            delegate->setRange(valbottom,valtop);
            tableView_pzk->setItemDelegate(delegate);
            tableView_pzk->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_pzk->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
            spinBox->setValue(pzkmodel->rowCount()-1);
            spinBox->setRange(valbottom,valtop);
        }
        break;
    }
   case 7:{
        unsigned short qzd = patterndata->wrk_fechData(QPattern::WrkItemHd_QiZenDian,0);
        lineEdit_qzd->setText(QString::number(qzd));
        break;
    }
   case 8:{
        if(dmbcmodel == NULL){
            dmbcmodel = new QDMBCModel(paramadata,this);
            tableView_dmbc->setModel(dmbcmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QParam::spaItemValBottom(QParam::SpaItemHd_Dmdmbc);
            int valtop = QParam::spaItemValTop(QParam::SpaItemHd_Dmdmbc);
            delegate->setRange(valbottom,valtop);
            tableView_dmbc->setItemDelegate(delegate);
            tableView_dmbc->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        }
        break;
    }
   case 9:{
        if(ycwzxzmodel==NULL){
            ycwzxzmodel= new QYCWZXZModel(paramadata,pcomm,this);
            tableView_ycwzxz->setModel(ycwzxzmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QParam::spaItemValBottom(QParam::SpaItemHd_Ycwzxz);
            int valtop = QParam::spaItemValTop(QParam::SpaItemHd_Ycwzxz);
            delegate->setRange(valbottom,valtop);
            tableView_ycwzxz->setItemDelegate(delegate);
            tableView_ycwzxz->setSpan(0,2,1,6);
        }
        break;
    }
   case 10:{
        if(NULL==fzycwzxzmodel){
            fzycwzxzmodel = new QFZYCWZXZModel(paramadata,pcomm,this);
            tableView_fzyc->setSpan(0,0,1,2);
            tableView_fzyc->setSpan(0,2,1,2);
            tableView_fzyc->setSpan(0,4,1,2);
            tableView_fzyc->setModel(fzycwzxzmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =QParam::spaItemValBottom(QParam::SpaItemHd_Fzycwzxz);
            int valtop = QParam::spaItemValTop(QParam::SpaItemHd_Fzycwzxz);
            delegate->setRange(valbottom, valtop);
            tableView_fzyc->setItemDelegate(delegate);
            tableView_fzyc->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);

        }
        break;
    }
   case 11:
        if(NULL==formjqgzcs){
            formjqgzcs = new FormJqgzcs(paramadata,pcomm,this);
            QWidget *widget = stackedWidget->currentWidget();
            QVBoxLayout *layout = new QVBoxLayout;
            widget->setLayout(layout);
            layout->addWidget(formjqgzcs);
            layout->setContentsMargins(6,6,6,6);
            label_jtzhenshu_jqgzcs = new QLabel;
            label_jtzhenshu_jqgzcs->setFixedSize(40,
                                      label_jtzhenshu_jqgzcs->height());
            QLabel *label_jtzhenshu_label = new QLabel;
            label_jtzhenshu_label->setText(tr("当前机头绝对位置: "));
            QWidget *widget_temp = new QWidget;
            widget_temp->setFixedSize(400,40);
            QHBoxLayout *hlayout = new QHBoxLayout(widget_temp);
            hlayout->addStretch();
            hlayout->addWidget(label_jtzhenshu_label);
            hlayout->addWidget(label_jtzhenshu_jqgzcs);
            layout->addWidget(widget_temp);
        }
        break;
   case 12:
        if(NULL==formxtcs){
#if DUAL_SYSTEM
            formxtcs = new FormXtcs2(paramadata,pcomm,this);
#else
            formxtcs = new FormXtcs(paramadata,pcomm,this);
#endif
            QWidget *widget =stackedWidget->currentWidget();
            QVBoxLayout *layout = new QVBoxLayout;
            widget->setLayout(layout);
            layout->addWidget(formxtcs);
            label_jtzhenshu_xtcs = new QLabel(this);
            label_jtzhenshu_xtcs->setFixedSize(40,label_jtzhenshu_xtcs->height());
            QLabel *label_temp = new QLabel(this);
            label_temp->setText(tr("当前机头绝对位置: "));
            QWidget *widget1 = new QWidget;
            QHBoxLayout *hlayout = new QHBoxLayout(widget1);
            hlayout->addStretch();
            hlayout->addWidget(label_temp);
            hlayout->addWidget(label_jtzhenshu_xtcs);
            hlayout->addStretch();
            layout->addWidget(widget1);
            layout->addStretch(50);
            QLabel *label_hint = new QLabel(this);
            label_hint->setText(tr("注：对以上参数的修改退出参数设置界面生效，不需要手动保存"));
            layout->addWidget(label_hint);
            layout->setContentsMargins(6,20,6,6);
            //formxtcs->show();
        }

   default:
        break;

    }
    if((yyy==11)||(yyy==12))
        timer.stop();
    else
        timer.stop();
}

void paramform::indexchange(int index){
    if(index<8){
        if(tabWidget->currentIndex()!=0)
            tabWidget->setCurrentIndex(0);
        if(listWidget->currentRow()!=index)
            listWidget->setCurrentRow(index);
    }else{
        if(tabWidget->currentIndex()!=1)
            tabWidget->setCurrentIndex(1);
        if(listWidget_2->currentRow()!=(index-8))
            listWidget_2->setCurrentRow(index-8);
    }
}

void paramform::readabspinpos()
{
    unsigned short val;
    QString str;
    if(pcomm->ReadAbsoluteNoOfPin(val)==Md::Ok)
        str = QString("[ %1 ]").arg(val,4);
    else
        timer.stop();   //if comm error,timer will be stop ,
                        //then this function nolong be called
                        //untill tabwidet is toggle to xtcs and jqgzcs page
    if(NULL!=formxtcs)
        label_jtzhenshu_xtcs->setText(str);
    if(NULL!=formjqgzcs)
        label_jtzhenshu_jqgzcs->setText(str);
}

void paramform::setIndex(ParamFormIndex index){
    showindex = index;
    emit indexchanged(index);
}

bool paramform::event(QEvent *e)
{
    if(e->type()==QEvent::StatusTip){
        QStatusTipEvent *tipevent = (QStatusTipEvent *)(e);
        label_tip->setText(tipevent->tip());
        e->accept();
        return TRUE;
    }
    return QWidget::event(e);
}

void paramform::on_listWidget_2_currentRowChanged(int currentRow)
{
    setIndex((ParamFormIndex)(currentRow+8));
}

void paramform::on_listWidget_currentRowChanged(int currentRow)
{
    setIndex((ParamFormIndex)currentRow);
}

void paramform::on_tabWidget_currentChanged(int index)
{
    if(index == 0){
        int i = listWidget->currentRow();
        if(-1==i)
            i=0;
        setIndex((ParamFormIndex)i);
    }
    if(index == 1){
        int i = listWidget_2->currentRow();
        if(-1==i)
            i=0;
        setIndex((ParamFormIndex)(i+8));
    }
}

void paramform::on_spinBox_valueChanged(int val)
{
    if(pzkmodel){
        pzkmodel->patterndata->wrk_setData(QPattern::WrkItemHd_ZanKaiPianSu,0,val);
        pzkmodel->modelReset();
    }
}


void paramform::creatextgzcsform(int indexofpage){

}

void paramform::createjqgzcsform(int indexofpage){

}


void paramform::on_lineEdit_qzd_textChanged(QString )
{
    const QValidator *valid = lineEdit_qzd->validator();
    QString str = lineEdit_qzd->text();
    int i=0;
    if(valid&&(valid->validate(str,i)==QValidator::Acceptable)){
        unsigned short d = lineEdit_qzd->text().toInt();
        patterndata->wrk_setData(QPattern::WrkItemHd_QiZenDian,0,d);
    }
}

