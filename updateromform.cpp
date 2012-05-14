#include "updateromform.h"
#include "data.h"
#include "globaldata.h"
#include<QMessageBox>
#include<qmdmessagebox.h>
#include"communicat.h"

updateRomForm::updateRomForm(QWidget *parent) :
    QDialog(parent){
    setupUi(this);   
}

void updateRomForm::on_qMdPushButton_16_clicked()
{
    lineEdit->setText(QFileDialog::getOpenFileName(this,tr("打开文件"),QString("./rom"),tr("固件文件(*.bin *.min)")));
}

void updateRomForm::on_qMdPushButton_7_clicked()
{
    lineEdit_2->setText(QFileDialog::getOpenFileName(this,tr("打开文件"),QString("./rom"),tr("固件文件(*.bag *.bsg)")));
}

void updateRomForm::on_qMdPushButton_17_clicked()
{
    QString str = lineEdit_2->text();
        int result;
        if(!str.isEmpty()){
            result = qComm.SendBag(str);
        }
        QMdMessageBox box;
        box.setText(tr("下载固件"));
        box.setIcon(QMessageBox::Warning);
        box.setStandardButtons(QMessageBox::Cancel);
        if(result == Md::FileNotExist){
            box.setInformativeText(tr("文件不存在"));
            box.exec();
        }
        if(result == Md::FileOpenFail){
            box.setInformativeText(tr("文件打开错误"));
            box.exec();
        }
        if(result == Md::CommError){
            box.setInformativeText(tr("通信出错"));
            box.exec();
        }
        if(result == Md::EraseError){
            box.setInformativeText(tr("擦除错误"));
            box.exec();
        }
        if(result == Md::BurnError){
            box.setInformativeText(tr("烧写错误"));
            box.exec();
        }
        if(result == Md::BurnOK){
            box.setIcon(QMessageBox::Information);
            box.setStandardButtons(QMessageBox::Ok);
            box.setDefaultButton(QMessageBox::Ok);
            box.setInformativeText(tr("下载成功"));
            box.exec();
        }
}


void updateRomForm::on_qMdPushButton_8_clicked()
{
    QString str = lineEdit->text();
        int result;
        if(!str.isEmpty()){
            QFile file(str);
            result = qComm.SendBin(file);
        }
        QMdMessageBox box;
        box.setText(tr("下载固件"));
        box.setStandardButtons(QMessageBox::Cancel);
        box.setIcon(QMessageBox::Warning);
        if(result == Md::CommError){
            box.setInformativeText(tr("通信出错"));
            box.exec();
        }
        if(result == Md::EraseError){
            box.setInformativeText(tr("擦除错误"));
            box.exec();
        }
        if(result == Md::BurnError){
            box.setInformativeText(tr("烧写错误"));
            box.exec();
        }
        if(result == Md::Ok){
            box.setInformativeText(tr("下载成功"));
            box.setIcon(QMessageBox::Information);
            box.setStandardButtons(QMessageBox::Ok);
            box.exec();
        }
}

void updateRomForm::on_qMdPushButton_clicked()
{
    this->accept();
}


