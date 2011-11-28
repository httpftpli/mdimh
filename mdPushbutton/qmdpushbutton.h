#ifndef QMDPUSHBUTTON_H
#define QMDPUSHBUTTON_H

#include <QtGui>

class QMdPushButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor TranspColorForNonAlph READ TranspColorForNonAlph WRITE SetTranspColorForNonAlph)
    Q_PROPERTY(bool IsGessImage READ IsGessImage WRITE SetGessImage)
    Q_PROPERTY(QColor busSpinColor READ busSpinColor WRITE setBusSpinColor);
    Q_PROPERTY(bool showBusySpin READ isShowBusySpin WRITE setShowBusySpin);
    Q_PROPERTY(int busSpinDelay READ busSpinDelay WRITE setBusSpinDelay)


public:
    QMdPushButton(QWidget *parent = 0);
    QColor TranspColorForNonAlph() const;
    void SetTranspColorForNonAlph(const QColor &Color=QColor(255,163,177));
    void setBusSpinColor(const QColor &color=Qt::gray);
    QColor  busSpinColor()const;
    bool isShowBusySpin()const;
    void setShowBusySpin(bool show=FALSE);
    int busSpinDelay()const;
    void setBusSpinDelay(int delay=60);
    bool IsGessImage() const;
    void SetGessImage(bool TrueFalse );
    bool AddImage(QImage &Image);
    void startSpin();
    void endSpin();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void timerEvent (QTimerEvent * event );
private:
    QColor transpColorForNonAlph;
    QColor busyspincolor;
    bool gessImage;
    bool showbusyspin;
    int busspindelay;
    bool spin;
    int angle;
    int timerid;

};

#endif
