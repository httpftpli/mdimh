#ifndef ALARMFORM_H
#define ALARMFORM_H
#include<QWidget>
#include<QLabel>
#include<QPushButton>
#include"csound.h"

class QHMIData;
class cSound;
class AlarmForm : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmForm(QHMIData *hmidata,QWidget *parent = 0);
protected:
    virtual void paintEvent (QPaintEvent * event );
private:
    QLabel *label;
    QPushButton *pushbutton;
    QHMIData *hmidata;
    QTimer timer;
    cSound sound;
    QString alarmstr;
    int flashtimes;
private slots:
    void onAlarm();
    void onPushButtonClick();
    void onTimeOut();
};

#endif // ALARMFORM_H
