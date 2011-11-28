#include "qcnteditform.h"
#include "qmdmessagebox.h"

QCntEditForm::QCntEditForm(QWidget *parent,QPatternData *data) :
    QWidget(parent),pattern(data){
    setupUi(this);
    ///////////////////////////
    setFixedSize(800,600);
    setWindowFlags(Qt::FramelessWindowHint);
    pcheckBoxArray1[0]=pushButton_01;
    pcheckBoxArray1[1]=pushButton_02;
    pcheckBoxArray1[2]=pushButton_03;
    pcheckBoxArray1[3]=pushButton_04;
    pcheckBoxArray1[4]=pushButton_05;
    pcheckBoxArray1[5]=pushButton_06;
    pcheckBoxArray1[6]=pushButton_07;
    pcheckBoxArray1[7]=pushButton_08;

    pcheckBoxArray2[0]=pushButton_11;
    pcheckBoxArray2[1]=pushButton_12;
    pcheckBoxArray2[2]=pushButton_13;
    pcheckBoxArray2[3]=pushButton_14;
    pcheckBoxArray2[4]=pushButton_15;
    pcheckBoxArray2[5]=pushButton_16;
    pcheckBoxArray2[6]=pushButton_17;
    pcheckBoxArray2[7]=pushButton_18;
    (pattern->loadStatus()&Md::HAVECNT)?iscntloaded=TRUE:iscntloaded=FALSE;
    if(!iscntloaded){
        if(!(pattern->loadFile(Md::HAVECNT)&Md::HAVECNT)){
            QMdMessageBox box;
            box.exec(tr("载入cnt文件"),tr("cnt文件载入失败"),QMessageBox::Warning,
                     QMessageBox::Cancel,QMessageBox::Cancel);
            deleteLater();
        }
    }
    QIntValidator *intval = new QIntValidator(this);
    intval->setRange(1,pattern->tatalrow);
    lineEdit_row->setValidator(intval);
    lineEdit_row->setText("1");
    label_totalrow->setNum(pattern->tatalrow);
    ////////////////////////////
}


void QCntEditForm::rowToggle(int row){
    lineEdit_row->setText(QString::number(row+1));
    lineEdit_hbh1->setText(QString::number(pattern->cnt_fechData(row,CNT_S1H_Pat)));
    lineEdit_hbh2->setText(QString::number(pattern->cnt_fechData(row,CNT_S2H_Pat)));
    lineEdit_hbh1_q->setText(QString::number(pattern->cnt_fechData(row,CNT_S1Q_Pat)));
    lineEdit_hbh2_q->setText(QString::number(pattern->cnt_fechData(row,CNT_S2Q_Pat)));

    /**********1系统后床指令A*********************/
    int indextmp = pattern->_azl(pattern->cnt_fechData(row,CNT_S1H_AZiLing));
    comboBox_zla1->setCurrentIndex(indextmp);
    /**********1系统后床指令H*********************/
    indextmp = pattern->_hzl(pattern->cnt_fechData(row,CNT_S1H_HZiLing));
    comboBox_zlh1->setCurrentIndex(indextmp);
    /**********1系统前床指令A*********************/
    indextmp = pattern->_azl(pattern->cnt_fechData(row,CNT_S1Q_AZiLing));
    comboBox_zla1_q->setCurrentIndex(indextmp);
    /**********1系统前床指令H*********************/
    indextmp = pattern->_hzl(pattern->cnt_fechData(row,CNT_S1Q_HZiLing));
    comboBox_zlh1_q->setCurrentIndex(indextmp);

    /**********2系统后床指令A*********************/
    indextmp = pattern->_azl(pattern->cnt_fechData(row,CNT_S2H_AZiLing));
    comboBox_zla2->setCurrentIndex(indextmp);
    /**********2系统后床指令H*********************/
    indextmp = pattern->_hzl(pattern->cnt_fechData(row,CNT_S2H_HZiLing));
    comboBox_zlh2->setCurrentIndex(indextmp);
    /**********2系统前床指令A*********************/
    indextmp = pattern->_azl(pattern->cnt_fechData(row,CNT_S2Q_AZiLing));
    comboBox_zla2_q->setCurrentIndex(indextmp);
    /**********2系统前床指令H*********************/
    indextmp = pattern->_hzl(pattern->cnt_fechData(row,CNT_S2Q_HZiLing));
    comboBox_zlh2_q->setCurrentIndex(indextmp);


    /**********1系统后床色代号A*********************/
    lineEdit_sdha1->setText(pattern->cnt_seDaiHao(row,CNT_S1H_AColor));
    /**********1系统后床色代号H*********************/
    lineEdit_sdhh1->setText(pattern->cnt_seDaiHao(row,CNT_S1H_HColor));
    /**********1系统前床色代号A*********************/
    lineEdit_sdha1_q->setText(pattern->cnt_seDaiHao(row,CNT_S1Q_AColor));
    /**********1系统前床色代号H*********************/
    lineEdit_sdhh1_q->setText(pattern->cnt_seDaiHao(row,CNT_S1Q_HColor));


    /**********2系统后床色代号A*********************/
    lineEdit_sdha2->setText( pattern->cnt_seDaiHao(row,CNT_S2H_AColor));
    /**********2系统后床色代号H*********************/
    lineEdit_sdhh2->setText(pattern->cnt_seDaiHao(row,CNT_S2H_HColor));
    /**********2系统前床色代号A*********************/
    lineEdit_sdha2_q->setText(pattern->cnt_seDaiHao(row,CNT_S2Q_AColor));
    /**********2系统前床色代号H*********************/
    lineEdit_sdhh2_q->setText(pattern->cnt_seDaiHao(row,CNT_S2Q_HColor));

    /**********1系统纱嘴*********************/
    unsigned char sz1 =pattern->cnt_shaZui1(row);
    for(int i=0;i<8;i++)
        pcheckBoxArray1[i]->setChecked(sz1&1<<i);
    /**********2系统纱嘴*********************/
    unsigned char sz2 = pattern->cnt_shaZui2(row);
    for(int i=0;i<8;i++)
        pcheckBoxArray2[i]->setChecked(sz2&1<<i);
    /**********度目,类型*********************/
    bool doubleorsigle;
    unsigned char dumu = pattern->cnt_duMu(row,doubleorsigle);
    lineEdit_dm->setText(QString::number(dumu));
    lineEdit_lx->setText(doubleorsigle?tr("双面"):tr("单面"));
    /**********主罗拉*********************/
    lineEdit_zll->setText(QString::number(pattern->cnt_mainLuola(row)));
    /**********辅助罗拉*********************/
    lineEdit_fzll->setText(QString::number(pattern->cnt_fuzuLuola(row)));
    /**********速度*********************/
    lineEdit_sd->setText(QString::number(pattern->cnt_spead(row)));
    /**********送纱电机*********************/
    lineEdit_ssdj->setText(QString::number(pattern->cnt_songSha(row))); //送纱电机=副罗拉
    /**********沙嘴停放*********************/
    lineEdit_sztf->setText(QString::number(pattern->cnt_shazuiTf(row)));
    /**********压脚*********************/
    lineEdit_yj->setText(QString::number(pattern->cnt_yaJiao(row)));
    /**********停车*********************/
    lineEdit_tc->setText(pattern->cnt_tingChe(row)?tr("是"):tr("否"));
    /**********摇床*********************/
    lineEdit_yc->setText(pattern->cnt_yaoChuang(row));
    /**********循环首*********************/
    lineEdit_xhs->setText(QString::number(pattern->cnt_fechData(row,CNT_LoopStart,2)));
    /**********循环次数*********************/
    lineEdit_xhcs->setText(QString::number(pattern->cnt_fechData(row,CNT_LoopNum,2)));
}

void QCntEditForm::on_pushButton_3_clicked()
{
    hide();
    deleteLater();
}

void QCntEditForm::on_pushButton_clicked()
{
    rowToggle(lineEdit_row->text().toInt()-1);
}

void QCntEditForm::on_lineEdit_row_textChanged(QString text)
{
    pushButton_rowup->setDisabled(text.toInt()==1);
    pushButton_rowdown->setDisabled(text.toInt()==pattern->tatalrow);
}

void QCntEditForm::on_pushButton_rowdown_clicked()
{
    int row = lineEdit_row->text().toInt();//new row;
    lineEdit_row->setText(QString::number(row+1));
    rowToggle(row);
}

void QCntEditForm::on_pushButton_rowup_clicked()
{
    int row = lineEdit_row->text().toInt()-2;//new row
    lineEdit_row->setText(QString::number(row+1));
    rowToggle(row);
}
