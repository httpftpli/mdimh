#include "dialogsetfinishcount.h"

DialogSetFinishCount::DialogSetFinishCount(QWidget *parent) :
    QDialog(parent){
    setStyleSheet(QString());
    setupUi(this);
    spinBox->setStyle(&style);
    connect(pushButton,SIGNAL(clicked()),SLOT(accept()));
    connect(pushButton_2,SIGNAL(clicked()),SLOT(reject()));
}

void DialogSetFinishCount::on_spinBox_valueChanged(int value)
{
    val = value;
}
