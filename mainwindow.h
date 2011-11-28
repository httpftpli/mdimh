#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include"namespace.h"
#include <QShortcut>

class paramform;

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void setup();

public slots:
    void runPatternRowChange(unsigned short val);
    void slot_patternChange();
    void runDirectionChange(Md::DIRECTION direction);
    void Timeout1s();

private slots:
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
    void on_qMdPushButton_5_clicked();
    void on_qMdPushButton_5_toggled(bool checked);
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
    void on_qMdPushButton_12_clicked();

private:
    QCheckBox *pcheckBoxArray[8];
    QStringList azllist,hzllist;

    bool eventFilter(QObject *, QEvent *);
    QPointer<paramform > paramForm;

};

#endif // MAINWINDOW_H
