#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include "ui_mainwindow1.h"

class MainWindow1 : public QMainWindow, private Ui::MainWindow1
{
    Q_OBJECT

public:
    explicit MainWindow1(QWidget *parent = 0);
};

#endif // MAINWINDOW1_H
