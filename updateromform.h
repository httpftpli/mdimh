#ifndef UPDATEROMFORM_H
#define UPDATEROMFORM_H

#include "ui_updateromform.h"

class updateRomForm : public QDialog, private Ui::updateRomForm
{
    Q_OBJECT

public:
    explicit updateRomForm(QWidget *parent = 0);

private slots:
    //void on_pushButton_clicked();
    void on_qMdPushButton_clicked();
    void on_qMdPushButton_8_clicked();
    void on_qMdPushButton_17_clicked();
    void on_qMdPushButton_7_clicked();
    void on_qMdPushButton_16_clicked();
};

#endif // UPDATEROMFORM_H
