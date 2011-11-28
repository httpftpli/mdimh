#ifndef HEADTESTFORM_H
#define HEADTESTFORM_H

#include "ui_headtestform.h"

class headTestForm : public QWidget, private Ui::headTestForm
{
    Q_OBJECT

public:
    explicit headTestForm(QWidget *parent = 0);
    QMdPushButton *ppinArray[64];
    bool eventFilter(QObject *target, QEvent *event);

private slots:
    void on_qMdPushButton_3_clicked();
};

#endif // HEADTESTFORM_H
