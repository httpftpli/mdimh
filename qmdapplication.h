#ifndef QMDAPPLICATION_H
#define QMDAPPLICATION_H
#include <QApplication>
#include <QPointer>
#include<QProcess>

class cSound;
class FormMovie;
class QMdApplication: public QApplication{
public:
    QMdApplication ( int & argc, char ** argv );
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
    QProcess *screenprotect;
};



#endif // QMDAPPLICATION_H
