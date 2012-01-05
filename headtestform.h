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
    explicit headTestForm(QComm *com , QHMIData *data, QWidget *parent= 0);
    void prepareToComm();
signals:
    void autoTesting(bool);

protected:
    virtual void timerEvent(QTimerEvent *);

private slots:
    void shazuitest( int shazui);
    void shazuitestauto();
    void dumutestauto();
    void sanjiaotestauto();
    void pinautotest();
    void autotest();
    void on_pushButton_cancle_clicked();

private:
    FormHeadTest *head0;
    FormHeadTest *head1;
    FormHeadTest *head2;
    FormHeadTest *head3;
    FormHeadTest *head[4];
    QSignalMapper signalmap;
    QPushButton *shazuibuttonarray[16];
    QComm *pcom;
    QHMIData *hmidata;
    bool inautotesting;
    int timerid;
    bool timereventrecursion;

    void setshazuival(unsigned short val);

};

#endif // HEADTESTFORM_H
