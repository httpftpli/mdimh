#ifndef SYSTEST_H
#define SYSTEST_H


#include <QObject>
#include<QTimer>
#include<globaldata.h>
#include<data.h>

class QSysTest : public QObject
{
    Q_OBJECT
public:
    explicit QSysTest(QObject *parent = 0);
    void start();

signals:
    void yunxinhuanxiang(unsigned short index,QVariant val);
    void jitouxiangduizhengshu(unsigned short index,QVariant val);
public slots:
    void timeout1S();
private:
    QTimer timer1s;
    int linenumber;


};

#endif // SYSTEST_H
