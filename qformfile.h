#ifndef QFORMFILE_H
#define QFORMFILE_H

#include "ui_qformfile.h"
#include <QDir>
#include <QList>


/////////////////////////


/////////////////////////////


class QFormFile : public QWidget, private Ui::QFormFile
{
    Q_OBJECT

public:
    explicit QFormFile(QWidget *parent = 0);
    ////////////////////////////////////
    // QList<MyFileinfo> filelist;
    const QString udispath;
    const QString dirpath;
    QString udiscurrentpath;
    QString dircurrentpath;


    /////////////////////////////////////


    //////////////////////////////////////

    //////////////////////////////////////


private:

private slots:
    void on_tableView_clicked(QModelIndex index);
    void on_tableView_2_clicked(QModelIndex index);
    void on_qMdPushButton_5_clicked();
    void on_qMdPushButton_7_clicked();
    void on_qMdPushButton_3_clicked();
    void on_qMdPushButton_clicked();
    void on_qMdPushButton_2_clicked();
    void on_qMdPushButton_8_clicked();
    void on_qMdPushButton_4_clicked();
    void on_qMdPushButton_6_clicked();


};



#endif // QFORMFILE_H


