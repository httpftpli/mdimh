#include "qhuabanxhform.h"
#include"globaldata.h"
#include "data.h"

QHuabanXhForm::QHuabanXhForm(QWidget *parent) :
    QDialog(parent){
    setupUi(this);    
    ///////////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    QCntLoopModel *loopmodel = new QCntLoopModel(&patternData,this);
    QMdItemDelegate *delegate = new QMdItemDelegate(this);
    delegate->setRange(1,patternData.tatalcntrow);
    delegate->setRange(1,20000,-1,2);
    tableView->setModel(loopmodel);
    tableView->setItemDelegate(delegate);
    connect(loopmodel,SIGNAL(datasValid(bool)),pushButton_4,SLOT(setEnabled(bool)));
    ///////////////////////////////
}

void QHuabanXhForm::on_pushButton_clicked()
{
    tableView->model()->insertRow(tableView->currentIndex().row()+1,QModelIndex());
}

void QHuabanXhForm::on_pushButton_2_clicked()
{
    tableView->model()->removeRow(tableView->currentIndex().row());
}

void QHuabanXhForm::on_pushButton_5_clicked()
{
    this->reject();
    this->deleteLater();
}

void QHuabanXhForm::on_pushButton_4_clicked()
{
    qobject_cast<QCntLoopModel *>(tableView->model())->save();
}
