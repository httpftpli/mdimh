#ifndef MACHINEEXAME_H
#define MACHINEEXAME_H

#include "ui_machineexame.h"
#include<QTimer>


class QMdLabel;
class QComm;
class machineexame : public QWidget, private Ui::machineexame
{
    Q_OBJECT

public:
    explicit machineexame(QComm *comm,QWidget *parent = 0);
    void prepareTest();

protected:
    virtual void timerEvent ( QTimerEvent * event );
signals:
    void DataChange(unsigned short index,QVariant Val);

private slots:

    void on_qMdPushButton_8_clicked();

    void on_qMdPushButton_11_clicked();
    void on_qMdPushButton_10_clicked();
    void on_spinBox_5_valueChanged(int );
    void on_qMdPushButton_9_toggled(bool checked);
    void on_spinBox_6_valueChanged(int );
    void on_spinBox_2_valueChanged(int );
    void on_spinBox_valueChanged(int );
    void on_qMdPushButton_7_toggled(bool checked);
    void on_qMdPushButton_5_toggled(bool checked);
    void on_qMdPushButton_6_toggled(bool checked);
    void on_qMdPushButton_4_toggled(bool checked);
    void on_qMdPushButton_2_toggled(bool checked);
    void on_qMdPushButton_3_toggled(bool checked);
    void on_qMdPushButton_toggled(bool checked);
    void On_DataChanged(unsigned short,QVariant);


private:
    int timerid;
    bool timereventrecursion;
    QComm *pcomm;
    QMdLabel wlabel[32];



};

#endif // MACHINEEXAME_H
