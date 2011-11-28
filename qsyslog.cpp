#include "qsyslog.h"
#include<QDebug>
#include<QDataStream>



void QSysLog::setFile(const QString &filepath){
    this->filepath = filepath;
    QFile file(this->filepath);
    file.open(QIODevice::ReadOnly);
    LogItemType item;
    QDataStream stream(&file);
    while(!stream.atEnd()){
        stream>>item.dateTime>>item.type>>item.log;
        logque<<item;
    }
    file.close();
}

void QSysLog::enqueue(unsigned char type,const QString &log){
    LogItemType item;
    item.dateTime = QDateTime::currentDateTime().toTime_t();
    item.type = type;
    item.log = log;
    if(logque.size()==300)
        logque.dequeue();
    logque<<item;
}

int QSysLog::size(){
    return logque.size();
}

void QSysLog::save(){
    QFile file(this->filepath);
    file.open(QIODevice::ReadWrite);
    QDataStream stream(&file);
    foreach(LogItemType item,logque){
         stream<<item.dateTime;
         stream<<item.type;
         stream<<item.log;
    }
    file.close();
}

void  QSysLog::warnLog(const QString log){
     enqueue(WARNING,log);
}

void  QSysLog::informLog(const QString log){
     enqueue(INFORM,log);
}

void  QSysLog::errorLog(const QString log){
    enqueue(ERROR,log);
}



QSysLogModel::QSysLogModel(QSysLog *log, QObject *parent) :
    QAbstractTableModel(parent),syslog(log)
{

}


int QSysLogModel::rowCount(const QModelIndex &parent)const{
    Q_UNUSED(parent)
    return this->syslog->size();
}

int QSysLogModel::columnCount(const QModelIndex &parent)const{
    Q_UNUSED(parent)
    return 3;
}
QVariant QSysLogModel::headerData(int section, Qt::Orientation orientation, int role)const{
    if(role!=Qt::DisplayRole)
        return QVariant();
    if(orientation == Qt::Horizontal){
        switch(section){
        case 0:
            return tr("时间");
            break;
        case 1:
            return tr("类型");
            break;
        case 2:
            return tr("描述");
            break;
        default:
            break;
        }
    }else if(orientation == Qt::Vertical){
        return section+1;
    }
     return QVariant();
 }



QVariant QSysLogModel::data(const QModelIndex &index, int role)const{
    int row = index.row();
    int size = syslog->size();
    QSysLog::LogItemType item = syslog->logque.value(size-row-1);
    int column = index.column();
    if(role == Qt::DisplayRole)
        switch(column){
        case 0:
        return QDateTime::fromTime_t(item.dateTime).toString("yyyy-MM-dd hh:mm:ss");
        break;
        case 1:
        switch(item.type){
        case ERROR:
            return tr("错误");
        case INFORM:
            return tr("通知");
        case WARNING:
            return tr("警告");
        default:
            break;
        }
        break;
        case 2:
        return item.log;
        default:
        break;
    }
    else if((role==Qt::TextColorRole)&&(column==1)){
        switch(item.type){
        case ERROR:
            return Qt::red;
        case INFORM:
            return Qt::blue;
        case WARNING:
            return Qt::yellow;
        default:
            break;
        }
    }else if((role==Qt::UserRole)){
        switch(column){
        case 0:
            return row;
            break;
        case 1:
            switch(item.type){
            case ERROR:
                return ERROR;
            case INFORM:
                return INFORM;
            case WARNING:
                return WARNING;
            default:
                break;
            }
            break;
        case 2:
            return item.log;
        default:
            break;
        }
    }
    return QVariant();
}
