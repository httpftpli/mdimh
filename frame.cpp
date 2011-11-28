#include "frame.h"

Frame::Frame(QWidget *parent) :
    QFrame(parent){
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Window);
}

void Frame::on_qMdPushButton_clicked()
{
    this->hide();
}
