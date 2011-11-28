#ifndef QMLABEL_H
#define QMLABEL_H

#include <QtGui>

class QMdLabel : public QLabel
{
    Q_OBJECT
    Q_PROPERTY(bool LogicReverse READ LogicReverse WRITE SetLogicReverse)


public:
    QMdLabel(QWidget *parent = 0);
    bool  LogicReverse() const;
    void  SetLogicReverse(bool val= FALSE);


private:
    bool logicReverse;
protected:


};

#endif
