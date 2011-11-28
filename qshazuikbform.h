#ifndef QSHAZUIKBFORM_H
#define QSHAZUIKBFORM_H

#include "ui_qshazuikbform.h"

class QShazuiKbForm : public QDialog, private Ui::QShazuiKbForm
{
    Q_OBJECT

public:
    explicit QShazuiKbForm(QWidget *parent = 0);

private slots:


private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
private:

};

#endif // QSHAZUIKBFORM_H
