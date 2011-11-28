#ifndef SYSLOGFORM_H
#define SYSLOGFORM_H

#include "ui_syslogform.h"
#include "qsyslog.h"

class sysLogForm : public QWidget, private Ui::sysLogForm
{
    Q_OBJECT

public:
    explicit sysLogForm(QWidget *parent = 0);

private:
    QSysLogModel *syslogmodel;


private slots:
    void on_qMdPushButton_3_clicked();
    void on_qMdPushButton_2_clicked();
    void on_qMdPushButton_clicked();
};

#endif // SYSLOGFORM_H
