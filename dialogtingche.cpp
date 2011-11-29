#include "dialogtingche.h"
#include "data.h"

DialogTingche::DialogTingche(QPatternData *p, QWidget *parent):
    QDialog(parent),pattern(p),model(NULL){
    setupUi(this);
    model = new QTingcheModel(pattern,this);
    QMdItemDelegate  *delegate = new QMdItemDelegate(this);
    delegate->setRange(1,pattern->tatalrow);
    tableView->setModel(model);
    tableView->setItemDelegate(delegate);
    connect(model,SIGNAL(datasValid(bool)),pushButton_3,SLOT(setEnabled(bool)));
    connect(pushButton_4,SIGNAL(clicked()),SLOT(accept()));
}

void DialogTingche::on_pushButton_clicked()
{
    model->insertRow(tableView->currentIndex().row());
}

void DialogTingche::on_pushButton_2_clicked()
{
    model->removeRow(tableView->currentIndex().row());
}

void DialogTingche::on_pushButton_3_clicked()
{
    model->saveToFile();
    pushButton_3->setEnabled(FALSE);
}
