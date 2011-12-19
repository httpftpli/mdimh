#ifndef MACHINEEXAME_H
#define MACHINEEXAME_H

#include "ui_machineexame.h"
#include<QTimer>

class machineexame : public QWidget, private Ui::machineexame
{
    Q_OBJECT

public:
    explicit machineexame(QWidget *parent = 0);

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
    unsigned long long timercount;
    int timerid;



};

#endif // MACHINEEXAME_H
