#include "dialogtingche.h"
#include "data.h"

DialogTingche::DialogTingche(QPatternData *p, QWidget *parent):
    QDialog(parent),pattern(p),model(NULL){
    setupUi(this);
    model = new QTingcheModel(pattern,this);
    spinBox->setRange(1,pattern->tatalcntrow);
    tableView->setModel(model);
    connect(pattern,SIGNAL(cntDirty(bool)),pushButton_3,SLOT(setEnabled(bool)));
    connect(pushButton_4,SIGNAL(clicked()),SLOT(accept()));
}

void DialogTingche::on_pushButton_clicked()
{
    model->insertCntRow(spinBox->value()-1);
}

void DialogTingche::on_pushButton_2_clicked()
{
    int row = tableView->currentIndex().row();
    model->removeRow(row);
}

void DialogTingche::on_pushButton_3_clicked()
{
    pushButton_3->setEnabled(FALSE);
    pattern->Save(Md::HAVECNT,Md::HAVECNT);
}

void DialogTingche::on_pushButton_4_clicked()
{
    pattern->refreshBuf(Md::HAVECNT);
}
