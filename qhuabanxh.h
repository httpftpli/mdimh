#ifndef QHUABANXH_H
#define QHUABANXH_H

#include "ui_qhuabanxh.h"

class QHuabanXh : public QDialog, private Ui::QHuabanXh
{
    Q_OBJECT

public:
    explicit QHuabanXh(QWidget *parent = 0);

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // QHUABANXH_H
