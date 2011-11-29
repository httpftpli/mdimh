#include "qformfile.h"
#include "qmytablewidget.h"
#include "communicat.h"
#include "QFileSystemModel"
#include "globaldata.h"
#include "qmdmessagebox.h"
#include "data.h"
#include "qhmidata.h"

extern QApplication a;



QFormFile::QFormFile(QWidget *parent):QWidget(parent),
                                       udispath("./patternfile1"),
                                       dirpath("./patternfile"),
                                       udiscurrentpath("./patternfile1"),
                                       dircurrentpath("./patternfile")
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
    QModelIndex index = tableView_2->currentIndex();
    QMdSortFilterProxyModel *model =static_cast<QMdSortFilterProxyModel *>(tableView_2->model());
    QString dirname = model->upPath(index);
    QString cntfilename,patfilename,wrkfilename,sazfilename;
    QString cntfilepath,patfilepath,wrkfilepath,sazfilepath;

    QProgressDialog progressDialog(this);
    progressDialog.setAutoClose(FALSE);
    connect(&qSend,SIGNAL(commPercent(int)),&progressDialog,SLOT(setValue(int)));

    cntfilename = model->patternName(index,".cnt");
    cntfilepath = dirname+"/"+cntfilename;

    patfilename = model->patternName(index,".pat");
    patfilepath = dirname+"/"+patfilename;

    Md::HAVEFILEFLAG flag = model->haveFile(index);

    if(flag&Md::HAVEWRK){
        wrkfilename = model->patternName(index,".wrk");
        wrkfilepath = dirname+"/"+wrkfilename;
    }else{
        wrkfilepath = QString();
    }

    if(flag&Md::HAVESAZ){
        sazfilename = model->patternName(index,".saz");
        sazfilepath = dirname+"/"+sazfilename;
    }else
        sazfilepath = QString();
    INFORMLOG(tr("更换花型")+cntfilename+QString(" ")+patfilename+QString(" ")+wrkfilename+QString(" ")+sazfilename);
    QPatternData::Result r = patternData.setFile(cntfilepath,patfilepath,wrkfilepath,sazfilepath);
    QMdMessageBox box;
    if(r!=QPatternData::Ok){
        box.exec(tr("选择文件"),tr("花型文件错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }
    progressDialog.setRange(0,100);
    progressDialog.setLabelText(tr("发送CNT文件"));
    progressDialog.show();
    QFile cntfile(cntfilepath);
    int result = qSend.SendFile(cntfile,0,FALSE,this);
    if(result ==Md::CommError) {
        box.exec(tr("花型发送"),tr("发送CNT文件，通信错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }
    progressDialog.setLabelText(tr("发送PAT文件"));
    progressDialog.show();
    QFile patfile(patfilepath);
    result = qSend.SendFile(patfile,0,FALSE,this);
    if(result ==Md::CommError) {
        box.exec(tr("花型发送"),tr("发送PAT文件，通信错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }

    QFile wrkfile(wrkfilepath);
    QFile spafile(hmiData.spaFilePath);
    result = qSend.SendParama(wrkfile,spafile,0xf7,NULL);
    if(result ==Md::CommError) {
        box.exec(tr("花型发送"),tr("发送WRK文件,通信错误"),QMessageBox::Warning,
                 QMessageBox::Cancel,QMessageBox::Cancel);
        return;
    }

    if(flag&Md::HAVESAZ){ //发送沙嘴捆绑文件
        QFile sazfile(sazfilepath);
        result = qSend.SendFile(sazfile,0,FALSE,this);
        if(result ==Md::CommError) {
            box.exec(tr("花型发送"),tr("发送沙嘴捆绑,通信错误"),QMessageBox::Warning,
                     QMessageBox::Cancel,QMessageBox::Cancel);
            return;
        }
    }
    progressDialog.close();
    box.exec(tr("花型发送"),tr("下载成功"),QMessageBox::Information,
             QMessageBox::Cancel,QMessageBox::Cancel);
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


