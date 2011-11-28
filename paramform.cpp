#include "paramform.h"
#include "data.h"
#include "globaldata.h"
#include <QFlags>

#include "formjqgzcs.h"
#include "formxtcs.h"

paramform::paramform(QWidget *parent) :
    QWidget(parent,Qt::FramelessWindowHint),patterndata(NULL),paramadata(NULL),dmzmodel(NULL),llmodel(NULL),
        zdjmodel(NULL),ssmodel(NULL),sztfmodel(NULL),ytxszxzmodel(NULL),
        ytxsztfmodel(NULL),cjpmodel(NULL),pzkmodel(NULL),
        szkbmodel(NULL),dmbcmodel(NULL),ycwzxzmodel(NULL),
        fzycwzxzmodel(NULL),formjqgzcs(NULL),formxtcs(NULL){
    //qDebug()<<"enter paramform construction fuc"<<QTime::currentTime().toString("ss-zzz");
    setupUi(this);
    //qDebug()<<"setupUi(this)"<<QTime::currentTime().toString("ss-zzz");
    ////////////////////////////
    setPattern(&patternData);
    //qDebug()<<"setPattern:"<<QTime::currentTime().toString("ss-zzz");
    setParama(&paramaData);
    //qDebug()<<"setParama:"<<QTime::currentTime().toString("ss-zzz");
    patterndata->loadFile(Md::HAVEWRK);
    label_13->setText(patterndata->wrkFileName);
    label_11->setText(paramadata->spafilename);
    //qDebug()<<"patterndata.loadfile"<<QTime::currentTime().toString("ss-zzz");
    //qDebug()<<"set model"<<QTime::currentTime().toString("ss-zzz");
    ///////////////////////////////
    connect(patterndata,SIGNAL(wrkDirty(bool)),pushButton_wrksave,SLOT(setEnabled(bool)));
    connect(patterndata,SIGNAL(wrkDirty(bool)),pushButton_wrkreset,SLOT(setEnabled(bool)));
    connect(paramadata,SIGNAL(dirty(bool)),pushButton_spasave,SLOT(setEnabled(bool)));
    connect(this,SIGNAL(indexchanged(int)),stackedWidget,SLOT(setCurrentIndex(int)));
    connect(this,SIGNAL(indexchanged(int)),SLOT(indexchange(int)));
    //connect(paramadata,SIGNAL(dirty(bool)),pushButton_spareset,SLOT(setEnabled(bool)));

}

paramform::~paramform(){
    this->paramadata->releaseBuf();;
}

void paramform::setPattern(QPatternData *pattern){
    this->patterndata = pattern;
}


void paramform::setParama(QParam *parama){
    this->paramadata = parama;
}

void paramform::on_qMdPushButton_clicked()
{
    deleteLater();
}



void paramform::on_stackedWidget_currentChanged(int yyy )
{
    if(yyy>7)
        paramadata->loadFile();
    switch(yyy){
    case 0:
        if(dmzmodel==NULL){
            dmzmodel = new QDMZModel(patterndata,this);
            tableView->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView->setModel(dmzmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =wrkItemDsp[WrkItemHd_DuMuZi].valrangebottom;
            int valtop =wrkItemDsp[WrkItemHd_DuMuZi].valrangetop;
            delegate->setRange(valbottom,valtop);
            tableView->setItemDelegate(delegate);
        }
        break;
   case 1:
        if(llmodel==NULL){
            llmodel = new QLLModel(patterndata,this);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =wrkItemDsp[WrkItemHd_JuanBu].valrangebottom;
            int valtop = wrkItemDsp[WrkItemHd_JuanBu].valrangetop;
            delegate->setRange(valbottom,+valtop);
            tableView_2->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_2->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_2->setModel(llmodel);
            tableView_2->setItemDelegate(delegate);

            zdjmodel = new QZDJModel(patterndata,this);
            QMdItemDelegate *delegate2 = new QMdItemDelegate(this);
            int valbottom1 =wrkItemDsp[WrkItemHd_SuDuZi].valrangebottom;
            int valtop1 = wrkItemDsp[WrkItemHd_SuDuZi].valrangetop;
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
            int valbottom =wrkItemDsp[WrkItemHd_LeftSongSa].valrangebottom;
            int valtop = wrkItemDsp[WrkItemHd_LeftSongSa].valrangetop;
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
            int valbottom =wrkItemDsp[WrkItemHd_ShaZuiTF].valrangebottom;
            int valtop =wrkItemDsp[WrkItemHd_ShaZuiTF].valrangetop;
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
            int valbottom =wrkItemDsp[WrkItemHd_YTXXiuZen].valrangebottom;
            int valtop =wrkItemDsp[WrkItemHd_YTXXiuZen].valrangetop;
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
            int valbottom =wrkItemDsp[WrkItemHd_YTXTingFang].valrangebottom;
            int valtop = wrkItemDsp[WrkItemHd_YTXTingFang].valrangetop;
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
            int valbottom =wrkItemDsp[WrkItemHd_CJP_BianZi].valrangebottom;
            int valtop =wrkItemDsp[WrkItemHd_CJP_BianZi].valrangetop;
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
   case 6:
        if(pzkmodel==NULL){
            pzkmodel = new QPZKModel(patterndata,8,this);            
            tableView_pzk->setModel(pzkmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            int valbottom =wrkItemDsp[WrkItemHd_PzkSaZui].valrangebottom;
            int valtop = wrkItemDsp[WrkItemHd_PzkSaZui].valrangetop;
            delegate->setRange(valbottom,valtop);
            tableView_pzk->setItemDelegate(delegate);
            tableView_pzk->verticalHeader()->setResizeMode(QHeaderView::Fixed);
            tableView_pzk->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
            spinBox->setValue(pzkmodel->rowCount()-1);
            spinBox->setRange(valbottom,valtop);
        }
        break;
   case 7:{
        unsigned short qzd = patterndata->wrk_fechData(WrkItemHd_QiZenDian,0);
        lineEdit_qzd->setText(QString::number(qzd));
        break;
    }
   case 8:
        if(dmbcmodel == NULL){
            dmbcmodel = new QDMBCModel(paramadata,this);
            tableView_dmbc->setModel(dmbcmodel);
            tableView_dmbc->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        }
        break;
   case 9:
        if(ycwzxzmodel==NULL){
            ycwzxzmodel= new QYCWZXZModel(paramadata,this);
            tableView_ycwzxz->setModel(ycwzxzmodel);
            tableView_ycwzxz->setSpan(0,2,1,6);
        }
        break;
   case 10:
        if(NULL==fzycwzxzmodel){
            fzycwzxzmodel = new QFZYCWZXZModel(paramadata,this);
            tableView_fzyc->setSpan(0,0,1,2);
            tableView_fzyc->setSpan(0,2,1,2);
            tableView_fzyc->setSpan(0,4,1,2);
            tableView_fzyc->setModel(fzycwzxzmodel);
            QMdItemDelegate *delegate = new QMdItemDelegate(this);
            tableView_fzyc->setItemDelegate(delegate);
            delegate->setRange(ITEM_DSP[ItemHd_Fzycwzxz].valrangebottom,
                               ITEM_DSP[ItemHd_Fzycwzxz].valrangetop
                               );
            tableView_fzyc->verticalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);

        }
        break;
   case 11:
        if(NULL==formjqgzcs){
            formjqgzcs = new FormJqgzcs(paramadata,this);
            QWidget *widget = stackedWidget->currentWidget();
            QGridLayout *layout = new QGridLayout;
            widget->setLayout(layout);
            layout->addWidget(formjqgzcs);
            layout->setContentsMargins(6,6,6,6);
            formjqgzcs->show();
        }
        break;
   case 12:
        if(NULL==formxtcs){
            formxtcs = new FormXtcs(paramadata,this);
            QWidget *widget =stackedWidget->currentWidget();
            QGridLayout *layout = new QGridLayout();
            widget->setLayout(layout);
            layout->addWidget(formxtcs);
            layout->setContentsMargins(6,20,6,6);
            formxtcs->show();
        }

   default:
        break;

    }
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
void paramform::setIndex(ParamFormIndex index){
    showindex = index;
    emit indexchanged(index);
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
        pzkmodel->patterndata->wrk_setData(WrkItemHd_ZanKaiPianSu,0,val);
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
        patterndata->wrk_setData(WrkItemHd_QiZenDian,0,d);
    }
}

void paramform::on_pushButton_wrkreset_clicked()
{
    patterndata->refreshBuf(Md::HAVEWRK);
    update();
}


void paramform::on_pushButton_wrksave_clicked()
{
    patterndata->Save(Md::HAVEWRK,Md::HAVEWRK);
}

void paramform::on_pushButton_spasave_clicked()
{
    paramadata->save(TRUE);
}


