#include "dialogabout.h"
#include "qhmidata.h"
#include <QApplication>


DialogAbout::DialogAbout(QHMIData *data,QWidget *parent):
    QDialog(parent),hmidata(data)
{
    setupUi(this);
    label_ver_display->setText(QApplication::applicationVersion()+QString(" build: ")+QString(__DATE__)+QString("  ")+QString(__TIME__));
    label_ver_mainboard->setText(hmidata->mainboardVersion());
    label_ver_bag->setText(hmidata->bagVersion());
}

void DialogAbout::on_pushButton_clicked()
{
    deleteLater();
}
