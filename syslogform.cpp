#include "syslogform.h"
#include "globaldata.h"
#include "QSortFilterProxyModel"

sysLogForm::sysLogForm(QWidget *parent) :
    QWidget(parent){
    setupUi(this);


    ///////////////////////////
    setWindowFlags(Qt::FramelessWindowHint);
    syslogmodel = new QSysLogModel(&sysLog,this);
    QSortFilterProxyModel *proxymodel = new QSortFilterProxyModel(this);
    proxymodel->setSourceModel(syslogmodel);
    proxymodel->setSortRole(Qt::UserRole);
    tableView->setModel(proxymodel);
    tableView->setColumnWidth(0,150);
    tableView->setColumnWidth(1,65);
    tableView->setContentsMargins(0,0,0,0);
    tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setFixedSize(tableView->horizontalHeader()->width(),40);

    /////////////////////////
}

void sysLogForm::on_qMdPushButton_clicked()
{
    this->deleteLater();
}

void sysLogForm::on_qMdPushButton_2_clicked()
{
    tableView->scrollToTop();
}

void sysLogForm::on_qMdPushButton_3_clicked()
{
    tableView->scrollToBottom();
}
