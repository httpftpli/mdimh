#ifndef QMYTABLEWIDGET_H
#define QMYTABLEWIDGET_H

#include <QTableWidget>
#include<QDir>
#include<QDateTime>
#include<QTableWidgetItem>
#include<QList>
#include<globaldata.h>
#include<QMessageBox>
#include<QLinkedList>


typedef struct _MyFileinfo{
    QString dispname;
    bool isDir;
    QString cntfilename;
    QString patfilename;
    QString wrkfilename;
    QDateTime datetime;
    unsigned int parttenBoardHight;
    unsigned int parttenWidth;
    unsigned int parttenHight;
} MyFileinfo;


class QMyDirModel:public QObject{
     Q_OBJECT
public:
    QVector<MyFileinfo> fileinfovector;
    QString currentpath;
    QDir qdir;
    QMyDirModel(QObject *parent=0);
    void del(int row);
    void copy(int row,QString desdirname);
    void copy(int row,QMyDirModel &mydirmodel);
    void setdir(QString dir);
    void updatadir();
public slots:


    QVector<MyFileinfo> listEntry();
   // int copyfile(QString scrdir,int row,QString desdir); //des是目录

   // void copy(QString scrdir,int row,QString desdir);   //des是目录

signals:
    void dirupdated(QVector<MyFileinfo> fileinfoVector);

private:

    QStringList namefilter;

    QStringList index(int row);
    void del_p(QFileInfo fileinfo);//recursion algorithm
    QFileInfoList listfileinfo(QDir dir,QDir::Filters Filter = QDir::Dirs|QDir::Files);//reenterable
   // QVector<MyFileinfo>  listmyfileinfo(QDir dir,QDir::Filter= QDir::Dirs|QDir::Files)//reenterable
    void copy_p(QString filename,QString dirname);//recursion algorithm
};


class QMyTableWidget:public QTableWidget{
    Q_OBJECT
public:

    QMyTableWidget(QWidget *parent = 0);
    QVector<QTableWidgetItem> headItemVector;
    QVector<QTableWidgetItem> itemVectorArray[3];
public slots:
    void SetItems(QVector<MyFileinfo> fileinfoVector);

};

#endif // QMYTABLEWIDGET_H




