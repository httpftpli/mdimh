#ifndef QSHAZUIEXCHFORM_H
#define QSHAZUIEXCHFORM_H

#include "ui_qshazuiexchform.h"
#include "pattern.h"
#include <QSignalMapper>
class QShazuiExchForm : public QDialog, private Ui::QShazuiExchForm
{
    Q_OBJECT

public:
    explicit QShazuiExchForm(QWidget *parent ,QPatternData *data);

private slots:
    void on_pushButton_6_clicked();
    void addNewShazui();
    void setCurrentShazuiText();
private:
    bool checkchange();
private:
    QPatternData  *pattern;
    QLabel *labelarray1[8];
    QLabel *labelarray2[8];
    QPushButton *pushbuttonarray1[8];
    QPushButton *pushbuttonarray2[8];
    QPushButton *pushbuttonarray3[8];
    QPushButton *currentshazui;

};

#endif // QHUABANXHFORM_H
