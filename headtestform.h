#ifndef HEADTESTFORM_H
#define HEADTESTFORM_H

#include "ui_headtestform.h"
#include<QSignalMapper>
#include"formheadtest.h"

class QComm;
class headTestForm : public QWidget, private Ui::headTestForm
{
    Q_OBJECT

public:
    explicit headTestForm(QComm *com , QWidget *parent= 0);
    void prepareToComm();
signals:
    void autoTesting(bool);

private slots:
    void shazuitest( int shazui);
    void shazuitestauto();
    void dumutestauto();
    void autotest();
    void on_pushButton_cancle_clicked();

private:
    QComm *qcom;
    FormHeadTest *head0;
    FormHeadTest *head1;
    FormHeadTest *head2;
    FormHeadTest *head3;
    QSignalMapper signalmap;
    QPushButton *shazuibuttonarray[16];
    QComm *pcom;
    bool inautotesting;

};

#endif // HEADTESTFORM_H
