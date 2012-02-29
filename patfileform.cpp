#include "patfileform.h"
#include "data.h"
#include "globaldata.h"
#include <QKeyEvent>
#include <QApplication>
#include "qmdmessagebox.h"


PatFileForm::PatFileForm(QWidget *parent, QPatternData *data) :
    QWidget(parent),pattern(data){
    setupUi(this);
    ////////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    QHeaderView *horhead = tableView->horizontalHeader();
    horhead->setResizeMode(QHeaderView::Fixed);
    QHeaderView *verhead = tableView->verticalHeader();
    verhead->setResizeMode(QHeaderView::Fixed);
    horhead->setDefaultAlignment(Qt::AlignJustify);
    patmodel = new QPatModel(&patternData,this);
    tableView->setModel(patmodel);
    QMdItemDelegate *patitemdelegate = new QMdItemDelegate(this);
    patitemdelegate->setItemSimplePaint(TRUE);
    patitemdelegate->setRegExp("[0-9a-fA-F]",-1,-1);
    tableView->setItemDelegate(patitemdelegate);
    tableView->setAlternatingRowColors(TRUE);
    QIntValidator *intv1 = new QIntValidator(this);
    QIntValidator *intv2 = new QIntValidator(this);
    intv1->setRange(1,pattern->tatalpatrow);
    intv2->setRange(1,pattern->tatalcolumn);
    lineEdit->setValidator(intv1);
    lineEdit_3->setValidator(intv2);
    oldflag = patternData.loadStatus();
    connect(patitemdelegate,SIGNAL(editing(QModelIndex)),SLOT(tableViewIsEditing(QModelIndex)));
    connect(pattern,SIGNAL(patDirty(bool)),qMdPushButton_4,SLOT(setEnabled(bool)));
    connect(&pos,SIGNAL(rowChanged(QString)),lineEdit,SLOT(setText(QString)));
    connect(&pos,SIGNAL(columnChanged(QString)),lineEdit_3,SLOT(setText(QString)));
    connect(&pos,SIGNAL(rowChanged(int)),SLOT(decideButtonUd(int)));
    connect(&pos,SIGNAL(columnChanged(int)),SLOT(decideButtonRl(int)));
    connect(lineEdit,SIGNAL(textChanged(QString)),&pos,SLOT(setRow(QString)));
    connect(lineEdit_3,SIGNAL(textChanged(QString)),&pos,SLOT(setColumn(QString)));

    pos.setData(pattern->tatalpatrow,1);
}

void PatFileForm::tableViewIsEditing(QModelIndex index){
    int row = index.row();
    int column = index.column();
    pos.setData(pattern->tatalpatrow-row,column+1);
}

void PatFileForm::on_qMdPushButton_3_clicked()
{
    hide();
    deleteLater();
}

PatFileForm::~PatFileForm(){
    if((oldflag&Md::HAVEPAT)==0)
        patternData.deloadFile(Md::HAVEPAT);
}

void PatFileForm::on_pushButton_clicked()
{
    patmodel->resetBuf();
}

void PatFileForm::decideButtonRl(int column){
    pushButton_columnleft->setDisabled(column==1);
    pushButton_columnlast->setDisabled(column==pattern->tatalcolumn);
    pushButton_columnfirt->setDisabled(column==1);
    pushButton_columnright->setDisabled(column==pattern->tatalcolumn);
}
void PatFileForm::decideButtonUd(int row){
    pushButton_rowdown->setDisabled(row==1);
    pushButton_rowup->setDisabled(row==pattern->tatalpatrow);
    pushButton_rowfist->setDisabled(row==1);
    pushButton_rowlast->setDisabled(row==pattern->tatalpatrow);
}


void PatFileForm::jumpTo(int row,int column,QAbstractItemView::ScrollHint scrollhint){
    QKeyEvent event(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
    QPointer<QLineEdit > edit = qobject_cast<QMdItemDelegate *>(tableView->itemDelegate())->editor ;
    if(edit){
        QCoreApplication::sendEvent(edit.data(),&event);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
    QAbstractItemModel  *model = tableView->model();
    QModelIndex index = model->index(pattern->tatalpatrow-row,column-1,QModelIndex());
    if(index.isValid()){
        tableView->scrollTo(index,scrollhint);
        tableView->edit(index);
    }
}

void PatFileForm::on_pushButton_jumpto_clicked()
{
    jumpTo(pos.row,pos.column,QAbstractItemView::PositionAtCenter);
}


void PatFileForm::on_pushButton_columnleft_clicked()
{
    pos.setData(pos.row,pos.column-1);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_columnright_clicked()
{
    pos.setData(pos.row,pos.column+1);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_rowup_clicked()
{
    pos.setData(pos.row+1,pos.column);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_rowdown_clicked()
{
    pos.setData(pos.row-1,pos.column);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_softkey_clicked()
{
#ifdef Q_WS_QWS
        QEvent e(QEvent::RequestSoftwareInputPanel);
        QApplication::sendEvent(lineEdit,&e);
        qDebug("valid inputmethod");
#endif
}

void PatFileForm::on_qMdPushButton_4_clicked()
{
    qMdPushButton_4->setEnabled(FALSE);
    pattern->Save(Md::HAVECNT,Md::HAVECNT);
}

void PatFileForm::on_pushButton_rowfist_clicked()
{
    pos.setData(1,pos.column);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_rowlast_clicked()
{
    pos.setData(pattern->tatalpatrow,pos.column);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_columnfirt_clicked()
{
    pos.setData(pos.row,1);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}

void PatFileForm::on_pushButton_columnlast_clicked()
{
    pos.setData(pos.row,pattern->tatalcolumn);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}



void PatFileForm::on_pushButton_pagedown_clicked()
{
    QScrollBar *bar = tableView->verticalScrollBar();
    int i = bar->pageStep();
    pos.setData(qMax(pos.row-i,1),pos.column);
    qDebug()<<"base:"<<QTime::currentTime().toString("ss-zzz");
    bar->setValue(bar->value()+i);
    qDebug()<<"bar->setValue:"<<QTime::currentTime().toString("ss-zzz");
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    qDebug()<<"processEvents:"<<QTime::currentTime().toString("ss-zzz");
    //pushButton_pagedown->setDisabled((bar->value()+i)>=(pattern->tatalrow));
    //qDebug()<<"bar->value()"<<bar->value()<<"bar->value()"<<i<<"tatalrow"<<pattern->tatalrow;
   // pushButton_pageup->setEnabled(TRUE);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
    qDebug()<<"jumpto:"<<QTime::currentTime().toString("ss-zzz");

}

void PatFileForm::on_pushButton_pageup_clicked()
{
    QScrollBar *bar = tableView->verticalScrollBar();
    int i = bar->pageStep();
    pos.setData(qMin(pos.row+i,(int)pattern->tatalpatrow),pos.column);
    bar->setValue(bar->value()-i);
    QCoreApplication::processEvents(QEventLoop::AllEvents);
   // pushButton_pageup->setDisabled(bar->value()==0);
   // pushButton_pagedown->setEnabled(TRUE);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);

}





void PatFileForm::on_pushButton_pageleft_clicked()
{
    QScrollBar *bar = tableView->horizontalScrollBar();
    int i = bar->pageStep();
    pos.setData(pos.row,qMax(pos.column-i,1));
    bar->setValue(bar->value()-i);
    QCoreApplication::processEvents(QEventLoop::AllEvents);
   // pushButton_pageleft->setDisabled(bar->value()==0);
   // pushButton_pageright->setEnabled(TRUE);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}



void PatFileForm::on_pushButton_pageright_clicked()
{
    QScrollBar *bar = tableView->horizontalScrollBar();
    int i = bar->pageStep();
    pos.setData(pos.row,qMin(pos.column+i,(int)pattern->tatalcolumn));
    bar->setValue(bar->value()+i);
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    //pushButton_pageright->setDisabled(bar->value()+i>=pattern->tatalcolumn);
    //pushButton_pageleft->setEnabled(TRUE);
    jumpTo(pos.row,pos.column,QAbstractItemView::EnsureVisible);
}
