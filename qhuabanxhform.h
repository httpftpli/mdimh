#ifndef QHUABANXHFORM_H
#define QHUABANXHFORM_H

#include "ui_qhuabanxhform.h"

class QHuabanXhForm : public QDialog, private Ui::QHuabanXhForm
{
    Q_OBJECT

public:
    explicit QHuabanXhForm(QWidget *parent = 0);

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // QHUABANXHFORM_H
