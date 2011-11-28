#ifndef DIALOGSETFINISHCOUNT_H
#define DIALOGSETFINISHCOUNT_H

#include "ui_dialogsetfinishcount.h"
#include"qmdstyle.h"

class DialogSetFinishCount : public QDialog, private Ui::DialogSetFinishCount
{
    Q_OBJECT

public:
    explicit DialogSetFinishCount(QWidget *parent = 0);
    int val;
private:
    QMdStyle style;

private slots:
    void on_spinBox_valueChanged(int );
};

#endif // DIALOGSETFINISHCOUNT_H
