#ifndef MACHINEEXAME_H
#define MACHINEEXAME_H

#include "ui_machineexame.h"
#include<QTimer>
#include<QSignalMapper>


class QMdLabel;
class QComm;
class QHMIData;
class machineexame : public QWidget, private Ui::machineexame
{
    Q_OBJECT

public:
    explicit machineexame(QComm *comm,QHMIData *data, QWidget *parent = 0);
    void prepareToComm();

protected:
    virtual void timerEvent ( QTimerEvent * event );
signals:
    void DataChange(unsigned short index,QVariant Val);

private slots:

    void on_pushButton_cancle_clicked();
    void headTest(bool checked);
    void on_pushButton_7_toggled(bool checked);
    void on_pushButton_5_toggled(bool checked);
    void on_pushButton_6_toggled(bool checked);
    void on_pushButton_4_toggled(bool checked);
    void zsdtest(int zsd);
    void on_spinBox_3_valueChanged(int arg1);
    void on_spinBox_valueChanged(int arg1);
    void on_spinBox_2_valueChanged(int arg1);
    void luolaTest();
    void fuzuLuolaTest();

private:
    int timerid;
    bool timereventrecursion;
    QComm *pcomm;
    QHMIData *hmidata;
    QMdLabel *wlabel[32];
    QSignalMapper zsdsignalmap;

    int mainmotordir;
    int mainmotorspeed;

    int fuzuluoladir;
    int fuzuluolaval;

    int luoladir;
    int luolaval;
    int luolapulsorpercent;




};

#endif // MACHINEEXAME_H
