#include "qshazuikbform.h"
#include "data.h"
#include "globaldata.h"

QShazuiKbForm::QShazuiKbForm(QWidget *parent) :
    QDialog(parent){
    setupUi(this);
    ////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    QSzkbModel *szkbmodel = new QSzkbModel(&patternData,this);
    QMdItemDelegate *delegate = new QMdItemDelegate(this);
    delegate->setRange(1,patternData.tatalrow,-1,2);
    delegate->setRange(1,patternData.tatalrow,-1,3);
    unsigned char shazuiraw = patternData.shazuiused_l|
                              patternData.shazuiused_r;

    QString shazuiuse('[');
    QString shazuiunuse('[');
    for(int i=0;i<8;i++)
        (shazuiraw&(1<<i))? shazuiuse.append(i+1+'0'):shazuiunuse.append(i+1+'0');
    shazuiuse.append(']');
    shazuiunuse.append(']');
    delegate->setRegExp(shazuiuse,-1,0);
    delegate->setRegExp(shazuiunuse,-1,1);
    tableView->setModel(szkbmodel);
    tableView->setItemDelegate(delegate);
    connect(szkbmodel,SIGNAL(datasValid(bool)),pushButton_4,SLOT(setEnabled(bool)));
    label->setText(label->text()+shazuiuse);
    label_2->setText((label_2->text()+shazuiunuse));

    ///////////////////////////////////
}


void QShazuiKbForm::on_pushButton_5_clicked()
{
    reject();
    this->deleteLater();
}

void QShazuiKbForm::on_pushButton_clicked()
{
    tableView->model()->insertRow(tableView->currentIndex().row()+1,QModelIndex());
}

void QShazuiKbForm::on_pushButton_2_clicked()
{
    tableView->model()->removeRow(tableView->currentIndex().row(),QModelIndex());
}

void QShazuiKbForm::on_pushButton_4_clicked()
{
    QSzkbModel *model  = qobject_cast<QSzkbModel *>(tableView->model());
    model->saveToFile();
    pushButton_4->setEnabled(FALSE);
}
