#ifndef QSYSLOG_H
#define QSYSLOG_H

#include <QAbstractTableModel>
#include<QDateTime>
#include<QString>
#include<QQueue>

#define INFORM 0
#define WARNING 1
#define ERROR  2


class QSysLogModel;

class QSysLog{
public:
    typedef struct __LogItemType{
        unsigned int  dateTime;
        unsigned char type;
        QString log;
    }LogItemType ;


    void setFile(const QString &filepath);
    void save();
    int size();
    void warnLog(const QString log);
    void informLog(const QString log);
    void errorLog(const QString log);
    void warnAppend(const QString log);
    void informAppend(const QString log);
    void errorAppend(const QString log);
    friend class  QSysLogModel;

private:
    void enqueue(unsigned char type,const QString &log);
    QQueue<LogItemType> logque;
    QString filepath;

};


class QSysLogModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit QSysLogModel(QSysLog *log,QObject *parent = 0);
    void save()const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
private:
    QSysLog *syslog;
};




#endif // QSYSLOG_H
