#ifndef DIALOGTINGCHE_H
#define DIALOGTINGCHE_H

#include "ui_dialogtingche.h"
#include "pattern.h"

class QTingcheModel;
class DialogTingche : public QDialog, private Ui::DialogTingche
{
    Q_OBJECT

public:
    explicit DialogTingche(QPatternData *pattern,QWidget *parent = 0);
private:
    QPatternData *pattern;
    QTingcheModel *model;

private slots:
    void on_pushButton_4_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // DIALOGTINGCHE_H
