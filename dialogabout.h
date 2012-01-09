#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "ui_dialogabout.h"


class QHMIData;
class DialogAbout : public QDialog, private Ui::DialogAbout
{
    Q_OBJECT
    
public:
    explicit DialogAbout(QHMIData *data,QWidget *parent = 0);
private slots:
    void on_pushButton_clicked();

private:
    QHMIData *hmidata;

};

#endif // DIALOGABOUT_H
