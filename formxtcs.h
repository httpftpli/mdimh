#ifndef FORMXTCS_H
#define FORMXTCS_H

#include "ui_formxtcs.h"
#include <QSignalMapper>


class QParam;
class QMdStyle;
class QComm;

class FormXtcs : public QWidget, private Ui::FormXtcs
{
    Q_OBJECT

public:
    explicit FormXtcs(QParam *paramdata, QComm *comm, QWidget *parent = 0);
    ~FormXtcs();

private:
    QParam *param;
    QSignalMapper signalmapper;
    QMdStyle *mdstyle;
    QComm *pcomm;
private slots:
    void valeChanged(QWidget *widget);

};

#endif // FORMXTCS_H
