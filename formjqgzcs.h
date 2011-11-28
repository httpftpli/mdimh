#ifndef FORMJQGZCS_H
#define FORMJQGZCS_H
#include<QSignalMapper>

#include "ui_formjqgzcs.h"
#include "qparam.h"
class QMdStyle;

class FormJqgzcs : public QWidget, private Ui::FormJqgzcs
{
    Q_OBJECT

public:
    explicit FormJqgzcs(QParam *data,QWidget *parent = 0);
    ~FormJqgzcs();

    void setData(unsigned short *buf,unsigned short len);


private:
    QParam *param;
    QSignalMapper signalmapper;
    QMdStyle *mdstyle;
private slots:
    void valeChanged(QWidget *obj);

};

#endif // FORMJQGZCS_H
