
#include "calibrationtestwidget.h"
#include "calibration.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>
#ifdef  Q_WS_QWS
calibrationtestwidget::calibrationtestwidget(QWidget *parent) :
    QWidget(parent),time(0),candrawline(FALSE){
    setupUi(this);
    /////////////////
    label_3->setText("");
    timer1s.setInterval(1000);
    timer1s.setSingleShot(FALSE);
    frame->installEventFilter(this);
    setWindowFlags(Qt::FramelessWindowHint);
    connect(&timer1s,SIGNAL(timeout()),SLOT(updatetime()));

    ///////////////////
}


bool  calibrationtestwidget::eventFilter ( QObject * watched, QEvent * event ){
    if(watched!=frame)
        return false;
    if(event->type()==QEvent::MouseButtonPress){
        QMouseEvent *mevent = static_cast<QMouseEvent *>(event);
        if(mevent->button() ==Qt::LeftButton){
            oldpoint = mevent->pos();
            paintpath.moveTo(oldpoint.x(),oldpoint.y());
            candrawline = TRUE;
            return TRUE;
        }
    }
    if(candrawline&&(event->type()==QEvent::MouseMove)){
        QMouseEvent *mevent = static_cast<QMouseEvent *>(event);
        if(mevent->buttons() ==Qt::LeftButton){
            newpoint = mevent->pos();
            frame->update();
            return TRUE;
        }
    }
    if(event->type()==QEvent::MouseButtonRelease){
        QMouseEvent *mevent = static_cast<QMouseEvent *>(event);
        if(mevent->buttons() ==Qt::LeftButton){
            candrawline = FALSE;
            return TRUE;
        }
    }
    if(event->type()==QEvent::Paint){
        //QPaintEvent *pevent = static_cast<QPaintEvent *>(event);
        QPainter painter(frame);
        //painter.setClipRect(oldpoint.x(),oldpoint.y(),newpoint.x(),newpoint.y());
        paintpath.lineTo(newpoint);
        painter.drawPath(paintpath);
        return FALSE;
    }
return FALSE;
}


void calibrationtestwidget::on_pushButton_clicked()
{
    cal = new Calibration(this);
    cal->exec();
    delete cal;
    time = 0;
    timer1s.start();
}

void calibrationtestwidget::updatetime(){
    label_3->setNum(20-time++);
    if(20==time){
        timer1s.stop();
        on_pushButton_clicked();
    }
}

void calibrationtestwidget::on_pushButton_2_clicked()
{
    timer1s.stop();
    label_3->setText("");
}

void calibrationtestwidget::on_pushButton_3_clicked()
{

    deleteLater();

}
#endif

