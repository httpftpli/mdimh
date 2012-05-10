#ifndef FORMHEAD_H
#define FORMHEAD_H


#include "ui_formhead.h"
#include <QPixmap>
#include "namespace.h"

class QPattern;
class QHMIData;
class QParam;

class FormHead : public QWidget, private Ui::FormHead
{
    Q_OBJECT

public:
    explicit FormHead(QPattern *,QHMIData *,QParam *,QWidget *parent = 0);
    void setKouAtribute(Md::POS_LEFTRIGHT kou);
private:
    QPattern *pattern;
    QHMIData *hmidata;
    QParam *param;
    QPixmap pixmapright;
    QPixmap pixmapleft;
    QLabel *plablearray[8];
    int cntnumber;
    Md::POS_LEFTRIGHT kou;
protected slots:
    void onCntNumber(unsigned short number);
    void onParamChanded();
protected:
    virtual void paintEvent (QPaintEvent * event );
};

#endif // FORMHEAD_H
