#ifndef QSHAZUIEXCHFORM_H
#define QSHAZUIEXCHFORM_H

#include "ui_qshazuiexchform.h"
#include "pattern.h"
#include <QSignalMapper>
class QShazuiExchForm : public QDialog, private Ui::QShazuiExchForm
{
    Q_OBJECT

public:
    explicit QShazuiExchForm(QWidget *parent ,QPattern *data);

private slots:
    void on_pushButton_6_clicked();
    void addNewShazui();
    void setCurrentShazuiText();
    void on_pushButton_4_clicked();

private:
    bool checkchange();
private:
    QPattern  *pattern;
    QLabel *labelarray1[8];
    QLabel *labelarray2[8];
    QPushButton *pushbuttonarray1[8];
    QPushButton *pushbuttonarray2[8];
    QPushButton *pushbuttonarray3[8];
    QPushButton *currentshazui;

};

#endif // QHUABANXHFORM_H
