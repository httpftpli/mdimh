#ifndef QMDAPPLICATION_H
#define QMDAPPLICATION_H
#include <QApplication>
#include <QPointer>

class cSound;
class FormMovie;
class QMdApplication: public QApplication{
public:
    QMdApplication ( int & argc, char ** argv ):QApplication(argc, argv),mousesound(0),scrprodelay(0){}
    ~QMdApplication();
    bool notify ( QObject * receiver, QEvent * event );
    void setMouseClickSound(const QString &filename);
    void setscreenprodelay(int second);
protected:
    virtual void timerEvent ( QTimerEvent * event );

private:
    cSound *mousesound;
    int timerid;
    int second;
    int scrprodelay;
    QPointer<FormMovie> formmovie;
};



#endif // QMDAPPLICATION_H
