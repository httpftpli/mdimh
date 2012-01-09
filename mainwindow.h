#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include"namespace.h"
#include <QShortcut>
#include "globaldata.h"

class paramform;
class FormHead;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setup();

public slots:
    void runPatternRowChange(unsigned short val);
    void onParamChanged();
#if DUAL_SYSTEM
    void onDankouLock(bool);
#endif
    void onpatternChange();
    void Timeout1s();

private slots:

    void on_pushButton_12_clicked();
    void on_pushButton_11_clicked();
    void on_qMdPushButton_10_clicked(bool checked);
    void on_qMdPushButton_9_clicked(bool checked);
    void on_pushButton_7_clicked();
    void on_qMdPushButton_8_clicked(bool checked);
    void on_pushButton_10_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_qMdPushButton_24_clicked();
    void on_qMdPushButton_22_clicked();
    void on_qMdPushButton_17_clicked();
    void on_qMdPushButton_23_clicked();
    void on_qMdPushButton_21_clicked();
    void on_qMdPushButton_11_clicked(bool checked);
    void on_qMdPushButton_5_clicked(bool checked);
    void on_qMdPushButton_6_clicked(bool checked);
    void on_qMdPushButton_14_clicked();
    void on_qMdPushButton_15_clicked();
    void on_qMdPushButton_4_clicked();
    void on_qMdPushButton_13_clicked();
    void on_qMdPushButton_7_clicked();
    void on_qMdPushButton_2_clicked();
    void on_qMdPushButton_3_clicked();
    void on_qMdPushButton_clicked();
    void onXtGuilingError();
    void onHmidataRuning(bool);
    void onPowerDown();

    void on_qMdPushButton_12_toggled(bool checked);

    void on_pushButton_13_clicked();

    void on_qMdPushButton_18_clicked();

    void on_qMdPushButton_19_clicked();

private:
    QStringList azllist,hzllist;
    QPointer<paramform > paramForm;
    QPixmap pixmapleft,pixmapright;
    FormHead *formheadl;
#if DUAL_SYSTEM
    FormHead *formheadr;
#endif

};

#endif // MAINWINDOW_H
