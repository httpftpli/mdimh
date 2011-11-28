#ifndef QMDAPPLICATION_H
#define QMDAPPLICATION_H
#include <QApplication>

class cSound;
class QMdApplication: public QApplication{
public:
    QMdApplication ( int & argc, char ** argv ):QApplication(argc, argv),mousesound(0){}
    ~QMdApplication();
    bool notify ( QObject * receiver, QEvent * event );
    void setMouseClickSound(const QString &filename);
private:
    cSound *mousesound;
};



#endif // QMDAPPLICATION_H
