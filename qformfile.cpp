#include "qformfile.h"
#include "qmytablewidget.h"
#include "communicat.h"
#include "QFileSystemModel"
#include "globaldata.h"
#include "qmdmessagebox.h"
#include "data.h"
#include "qhmidata.h"
#include "pattern.h"
#include"qparam.h"

extern QApplication a;



QFormFile::QFormFile(QPattern *p,QWidget *parent):QWidget(parent),
                                       udispath("./patternfile1"),
                                       dirpath("./patternfile"),
                                       udiscurrentpath("./patternfile1"),
                                       dircurrentpath("./patternfile"),
                                       pattern(p)
                                       {
    setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    QFileSystemModel *fileModel_udisk = new QFileSystemModel(tableView);
    fileModel_udisk->setRootPath(hmiData.udiskDirPath);
    fileModel_udisk->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);

    QFileSystemModel *fileModel = new QFileSystemModel(tableView_2);
    fileModel->setRootPath("./patternfile");
    fileModel->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);

    QMdSortFilterProxyModel *mdSortFilterProxyModel_udisk =new QMdSortFilterProxyModel(tableView);
    mdSortFilterProxyModel_udisk->setSourceModel(fileModel_udisk);

    QMdSortFilterProxyModel *mdSortFilterProxyModel =new QMdSortFilterProxyModel(tableView_2);
    mdSortFilterProxyModel->setSourceModel(fileModel);

    tableView->setModel(mdSortFilterProxyModel_udisk);
    tableView->setRootIndex(mdSortFilterProxyModel_udisk->mapFromSource(fileModel_udisk->index(hmiData.udiskDirPath,0)));
    tableView->setColumnHidden(1,1);
    tableView->setColumnHidden(2,1);
    tableView->setSortingEnabled(TRUE);

    tableView_2->setModel(mdSortFilterProxyModel);
    tableView_2->setRootIndex(mdSortFilterProxyModel->mapFromSource(fileModel->index("./patternfile",0)));
    tableView_2->setColumnHidden(1,1);
    tableView_2->setColumnHidden(2,1);
    tableView_2->setSortingEnabled(TRUE);
}


void QFormFile::on_qMdPushButton_6_clicked()
{
    this->deleteLater();
}


void QFormFile::on_qMdPushButton_3_clicked()
{
    QModelIndex index = tableView->rootIndex();
    QMdSortFilterProxyModel *model = static_cast<QMdSortFilterProxyModel *>(tableView->model());

    tableView->setRootIndex(model->parent(index));
    QString dirpath = model->currentDirPath(model->parent(index));
    int i = dirpath.indexOf(hmiData.udiskDirPath);
    label_7->setText(tr("U盘")+dirpath.right(dirpath.size()-i-hmiData.udiskDirPath.size()));
    QString path1 = QDir(hmiData.udiskDirPath).absolutePath();
    if(dirpath==path1)
        qMdPushButton_3->setEnabled(FALSE);
}


void QFormFile::on_qMdPushButton_7_clicked()
{
    QModelIndex index = tableView_2->rootIndex();
    QMdSortFilterProxyModel *model = static_cast<QMdSortFilterProxyModel *>(tableView_2->model());

    tableView_2->setRootIndex(model->parent(index));
    QString dirpath = model->currentDirPath(model->parent(index));
    int i = dirpath.indexOf("/patternfile");
    label_8->setText(tr("内存")+dirpath.right(dirpath.size()-i-12));

    QString path1 = QDir("./patternfile").absolutePath();

    if(path1==dirpath)
        qMdPushButton_7->setEnabled(FALSE);
}


void QFormFile::on_qMdPushButton_4_clicked()
{


}
void QFormFile::on_qMdPushButton_8_clicked()
{

}

void QFormFile::on_qMdPushButton_2_clicked()
{
    QMdSortFilterProxyModel *srcmodel = static_cast<QMdSortFilterProxyModel *>(tableView->model());
    QMdSortFilterProxyModel *desmodel = static_cast<QMdSortFilterProxyModel *>(tableView_2->model());
    QModelIndex indexsrc = tableView->currentIndex();
    QModelIndex indexdes = tableView_2->rootIndex();
    srcmodel->copy(indexsrc,*desmodel,indexdes,this);
}

void QFormFile::on_qMdPushButton_clicked()
{
    QMdSortFilterProxyModel *srcmodel = static_cast<QMdSortFilterProxyModel *>(tableView_2->model());
    QMdSortFilterProxyModel *desmodel = static_cast<QMdSortFilterProxyModel *>(tableView->model());
    QModelIndex indexsrc = tableView_2->currentIndex();
    QModelIndex indexdes = tableView->rootIndex();
    srcmodel->copy(indexsrc,*desmodel,indexdes,this);
}

void QFormFile::on_qMdPushButton_5_clicked()
{
    QProgressDialog progressDialog(this);
    progressDialog.setLabelText(tr("正在下载花型"));
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setRange(0,100);
    QModelIndex index = tableView_2->currentIndex();
    QMdSortFilterProxyModel *model =static_cast<QMdSortFilterProxyModel *>(tableView_2->model());
    QString dirname = model->upPath(index);
    QString patternname = model->patternName(index);
    QString patternpath = dirname+"/"+patternname;
    INFORMLOG(tr("更换花型:")+patternname);
    QPattern::Result r = pattern->setFile(patternpath);
    QMdMessageBox box;
    if(r!=QPattern::Ok){
        box.exec(tr("选择文件"),tr("花型文件错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }

    //////togle system/////////////////
    int commResult = qComm.TogSysStat(QHMIData::SysInParam);
    if(commResult == Md::CommError){
        box.exec(tr("通讯错误"),tr("花型下载失败"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return ;
    }
    ////download wrk spa
    commResult = paramaData.sendParama();
    if(commResult == Md::CommError){
        box.exec(tr("通讯错误"),tr("花型下载失败"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return ;
    }
    ///download szkb///////////////////////////////////
    commResult = pattern->sendShazuiKb();
    if(commResult == Md::CommError){
        box.exec(tr("通讯错误"),tr("花板捆绑文件下载失败"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return ;
    }
    ///////下载花型/////////////////////////////////////
    connect(pattern,SIGNAL(patternSendPercent(int)),&progressDialog,SLOT(setValue(int)));
    progressDialog.show();
    int result = pattern->sendPattern();
    if(result !=QPattern::Ok) {
        box.exec(tr("花型发送"),tr("发送CNT文件，通信错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }
    //等待下位机响应/////////////////////////////////
    int i=0;
    for(i=0;i<3;i++) {
        commResult = qComm.TogSysStat(QHMIData::SysInParam);
        if(commResult==Md::Ok)
            break;
    }
    if(3==i){
        box.exec(tr("花型发送"),tr("下载花型后主控未成功响应"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }
    //运行时参数设置//////////////////////////////////////////////
    /* commResult = hmiData.sendParamaInRun();
    if(commResult == Md::CommError){
        splash->showMessage(QObject::tr("通讯错误,运行时参数设置失败"),Qt::AlignBottom);
        QTest::qWait(2000);
        return;
    }*/
    box.exec(tr("花型发送"),tr("下载成功"),QMessageBox::Information,
             QMessageBox::Cancel,QMessageBox::Cancel);
    return;
}




void QFormFile::on_tableView_2_clicked(QModelIndex index)
{
    QMdSortFilterProxyModel *model = static_cast<QMdSortFilterProxyModel *>(tableView_2->model());
    QFileSystemModel *srcmodel = static_cast<QFileSystemModel *>(model->sourceModel());
    if(srcmodel->isDir(model->mapToSource(index))){
        tableView_2->setRootIndex(index);
        QString dirpath = model->currentDirPath(index);
        int i = dirpath.indexOf("/patternfile");
        label_8->setText(tr("内存")+dirpath.right(dirpath.size()-i-12));
        qMdPushButton_7->setEnabled(TRUE);
        qMdPushButton_5->setEnabled(FALSE);
        label_19->setText("");
        label_21->setText("");
        label_5->setText("");
        qMdPushButton->setEnabled(FALSE);
    }else{
        QSize size = model->patternSize(index);
        label_19->setNum(size.width());
        label_21->setNum(size.height());
        qMdPushButton_5->setEnabled(TRUE);
        label_5->setText(model->haveFileString(index));
        qMdPushButton->setEnabled(TRUE);
    }
}


void QFormFile::on_tableView_clicked(QModelIndex index)
{
    QMdSortFilterProxyModel *model = static_cast<QMdSortFilterProxyModel *>(tableView->model());
    QFileSystemModel *srcmodel = static_cast<QFileSystemModel *>(model->sourceModel());
    if(srcmodel->isDir(model->mapToSource(index))){
        QString dirpath = model->currentDirPath(index);
        int i = dirpath.indexOf(hmiData.udiskDirPath);
        label_7->setText(tr("U盘")+dirpath.right(dirpath.size()-i-hmiData.udiskDirPath.size()));
        tableView->setRootIndex(index);
        qMdPushButton_3->setEnabled(TRUE);
        label_17->setText("");
        label_16->setText("");
        label_4->setText("");
        qMdPushButton_2->setEnabled(FALSE);
    }else{
        QSize size = model->patternSize(index);
        label_17->setNum(size.width());
        label_16->setNum(size.height());
        label_4->setText(model->haveFileString(index));
        qMdPushButton_2->setEnabled(TRUE);

    }
}
