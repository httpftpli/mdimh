#ifndef FORMHEADTEST_H
#define FORMHEADTEST_H

#include "ui_formheadtest.h"
#include "globaldata.h"
#include <QSignalMapper>

class QComm;
class FormHeadTest : public QWidget, private Ui::FormHeadTest
{
    Q_OBJECT
    
public:
    explicit FormHeadTest(QComm *com, Md::SYSTEMFLAG sys,
                          Md::POSFLAG_FRONTREAR r,QWidget *parent = 0);
    void prepareToComm();
signals:
    void autoTesting(bool);

private:
    QComm *pcom;
    QPushButton *leftpinarray[8];
    QPushButton *rightpinarray[8];
    QPushButton *sanjiaoarray[6];
    Md::SYSTEMFLAG sys;
    Md::POSFLAG_FRONTREAR fr;
    QSignalMapper lpsignalmap,rpsignalmap,sanjiaosignalmap,dumusignalmap;


private slots:
    void lpintest( int pin);
    void rpintest( int pin);
    void sanjiaotest( int sanjiao);
    void dumutest( int dumu);
    void lpingtestauto();
    void rpingtestauto();
    void sanjiaotestauto();

};

#endif // FORMHEADTEST_H
