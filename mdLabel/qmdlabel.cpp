#include "qmdlabel.h"
#include<QSize>
#include <QDebug>



QMdLabel::QMdLabel(QWidget *parent) :
    QLabel("mdLabel", parent)
{
    SetLogicReverse(FALSE);
}



 bool QMdLabel::LogicReverse() const{
    return logicReverse;
}



void QMdLabel::SetLogicReverse(bool val){
    logicReverse =  val;
}





