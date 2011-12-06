#ifndef FORMXTCS2_H
#define FORMXTCS2_H

#include "ui_formxtcs2.h"
#include "ui_formxtcs.h"
#include <QSignalMapper>


class QParam;
class QMdStyle;
class FormXtcs2 : public QWidget, private Ui::FormXtcs2
{
    Q_OBJECT

public:
    explicit FormXtcs2(QParam *paramdata, QWidget *parent = 0);
    ~FormXtcs2();

private:
    QParam *param;
    QSignalMapper signalmapper;
    QMdStyle *mdstyle;
private slots:
    void valeChanged(QWidget *widget);
};

#endif // FORMXTCS2_H


