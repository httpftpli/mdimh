#ifndef QCNTEDITFORM_H
#define QCNTEDITFORM_H

#include "ui_qcnteditform.h"
#include "pattern.h"
#include "data.h"

class QCntEditForm : public QWidget, private Ui::QCntEditForm
{
    Q_OBJECT

public:
    explicit QCntEditForm(QWidget *parent ,QPattern *data);
protected:
    virtual bool event ( QEvent * event );
private:
    QPattern *pattern;
    QPushButton *pcheckBoxArray1[8];
    QPushButton *pcheckBoxArray2[8];
    void rowToggle(int row);
    int row;

private slots:
    void on_pushButton_rowup_clicked();
    void on_pushButton_rowdown_clicked();
    void on_lineEdit_row_textChanged(QString );
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
};

#endif // QCNTEDITFORM_H
