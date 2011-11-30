#ifndef CALIBRATIONTESTWIDGET_H
#define CALIBRATIONTESTWIDGET_H

#include "ui_calibrationtestwidget.h"
#include <QTimer>
#include <QPainterPath>
#ifdef Q_WS_QWS
class Calibration;

class calibrationtestwidget : public QWidget, private Ui::calibrationtestwidget
{
    Q_OBJECT

public:
    explicit calibrationtestwidget(QWidget *parent = 0);
    virtual bool eventFilter ( QObject * watched, QEvent * event );

private slots:
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void updatetime();
private:
    QTimer timer1s;
    int time;
    Calibration *cal;
    QPoint oldpoint,newpoint;
    bool candrawline;
    QPainterPath paintpath;

};

#endif // CALIBRATIONTESTWIDGET_H
#endif//
