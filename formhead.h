#ifndef FORMHEAD_H
#define FORMHEAD_H


#include "ui_formhead.h"
#include <QPixmap>

class QPatternData;
class QHMIData;
class QParam;

class FormHead : public QWidget, private Ui::FormHead
{
    Q_OBJECT

public:
    explicit FormHead(QPatternData *,QHMIData *,QParam *,QWidget *parent = 0);
    void setAtribute(unsigned char leftorright);
private:
    QPatternData *pattern;
    QHMIData *hmidata;
    QParam *param;
    QPixmap pixmapright;
    QPixmap pixmapleft;
    QLabel *plablearray[8];
    QStringList azllist;
    QStringList hzllist;
    unsigned short cntnumber;
    unsigned char leftright;
protected slots:
    void onCntNumber(unsigned short number);
    void onParamChanded();
protected:
    virtual void paintEvent (QPaintEvent * event );
};

#endif // FORMHEAD_H
