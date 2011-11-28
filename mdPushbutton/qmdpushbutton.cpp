#include "qmdpushbutton.h"
#include<QSize>
#include <QDebug>



QMdPushButton::QMdPushButton(QWidget *parent) :
    QPushButton(parent),showbusyspin(FALSE),busspindelay(60),
    spin(FALSE),timerid(-1)
{
    SetTranspColorForNonAlph();
}



QColor QMdPushButton::TranspColorForNonAlph() const{
    return transpColorForNonAlph;
}



void QMdPushButton::SetTranspColorForNonAlph(const QColor &Color){
    transpColorForNonAlph = Color;
}
QColor  QMdPushButton::busSpinColor()const{
    return busyspincolor;
}
void QMdPushButton::setBusSpinColor(const QColor &color){
    busyspincolor = color;
}

bool QMdPushButton::isShowBusySpin()const{
    return showbusyspin;
}

void QMdPushButton::setShowBusySpin(bool show){
    showbusyspin = show;
}


int QMdPushButton::busSpinDelay()const{
    return busspindelay;
}
void QMdPushButton::setBusSpinDelay(int delay){
    busspindelay = delay;
}
void QMdPushButton::startSpin(){
    if(!showbusyspin)
        return;
    spin =TRUE;
    angle = 0;
    if (timerid == -1)
        timerid = startTimer(busspindelay);
}

void QMdPushButton::endSpin(){
    if(!showbusyspin)
        return;
    spin = FALSE;
    if (timerid != -1)
        killTimer(timerid);
    timerid = -1;
    update();
}

bool QMdPushButton::IsGessImage()const{
    return gessImage;
}

void QMdPushButton::SetGessImage(bool TrueFalse ){
   gessImage = TrueFalse;
}


bool QMdPushButton::AddImage(QImage &Image){
    if (!Image.isNull()){
        int w = Image.width();
        int h = Image.height();
        QImage imagetemp = Image.convertToFormat(QImage::Format_ARGB32);
        switch(imagetemp.format()){
        case QImage::Format_ARGB32:{
                for(int i =0;i<w;i++){
                    for(int j =0;j<h;j++){
                        if(transpColorForNonAlph.rgb() == imagetemp.pixel(i,j)){
                            QRgb temp = imagetemp.pixel(i,j) & ~(0xff<<24);
                            imagetemp.setPixel(i,j,temp);
                        }
                    }
                }
                break;
            }
         case QImage::Format_RGB16:
         default:
              break;

        }
        QIcon icontemp;
        icontemp = icon();
        icontemp.addPixmap(QPixmap::fromImage(imagetemp));
        setIcon(icontemp);
        //update();
        return TRUE;
    }else
        return FALSE;
}

void QMdPushButton::timerEvent (QTimerEvent * event ){
    if(event->timerId()==timerid){
        angle = (angle+30)%360;
        update();
    }
}

void QMdPushButton::paintEvent(QPaintEvent *event){
    QPushButton::paintEvent(event);
    if (!showbusyspin&&!spin)
        return;
    int width = qMin(this->width(), this->height());
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int outerRadius = (width-1)/3;
    int innerRadius = (width-1)/3/3;

    int capsuleHeight = outerRadius - innerRadius;
    int capsuleWidth  = (width > 32 ) ? capsuleHeight /4 : capsuleHeight /3;
    int capsuleRadius = capsuleWidth/2;

    for (int i=0; i<12; i++)
    {
        QColor color = busyspincolor;
        color.setAlphaF(1.0f - (i/12.0f));
        p.setPen(Qt::NoPen);
        p.setBrush(color);
        p.save();
        p.translate(rect().center());
        p.rotate(angle - i*30.0f);
        p.drawRoundedRect(-capsuleWidth*0.5, -(innerRadius+capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
        p.restore();
    }

}



