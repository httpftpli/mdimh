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
    QIntValidator *intval = new QIntValidator(this);
    intval->setRange(1,pattern->tatalcntrow);
    lineEdit_row->setValidator(intval);
    lineEdit_row->setText("1");
    rowToggle(1);
    label_totalrow->setNum(pattern->tatalcntrow);
    ////////////////////////////
}


void QCntEditForm::rowToggle(int row){
    lineEdit_row->setText(QString::number(row));
    /*********花板行*****************************/
    lineEdit_hbhl->setText(QString::number(pattern->cnt_huabanhang(row,Md::POSLEFT,Md::POSREAR)));
    lineEdit_hbhr->setText(QString::number(pattern->cnt_huabanhang(row,Md::POSRIGHT,Md::POSREAR)));
    lineEdit_hbhl_q->setText(QString::number(pattern->cnt_huabanhang(row,Md::POSLEFT,Md::POSFRONT)));
    lineEdit_hbhr_q->setText(QString::number(pattern->cnt_huabanhang(row,Md::POSRIGHT,Md::POSFRONT)));

    /**********左口后床指令A*********************/
    int indextmp = pattern->cnt_Azhiling(row,Md::POSLEFT,Md::POSREAR);
    comboBox_zlal->setCurrentIndex(indextmp);
    /**********左口后床指令H*********************/
    indextmp = pattern->cnt_Hzhiling(row,Md::POSLEFT,Md::POSREAR);
    comboBox_zlhl->setCurrentIndex(indextmp);
    /**********左口前床指令A*********************/
    indextmp = pattern->cnt_Azhiling(row,Md::POSLEFT,Md::POSFRONT);
    comboBox_zlal_q->setCurrentIndex(indextmp);
    /**********左口前床指令H*********************/
    indextmp = pattern->cnt_Hzhiling(row,Md::POSLEFT,Md::POSFRONT);
    comboBox_zlhl_q->setCurrentIndex(indextmp);

    /**********右口后床指令A*********************/
    indextmp = pattern->cnt_Azhiling(row,Md::POSRIGHT,Md::POSREAR);
    comboBox_zlar->setCurrentIndex(indextmp);
    /**********右口后床指令H*********************/
    indextmp = pattern->cnt_Hzhiling(row,Md::POSRIGHT,Md::POSREAR);
    comboBox_zlhr->setCurrentIndex(indextmp);
    /**********右口前床指令A*********************/
    indextmp = pattern->cnt_Azhiling(row,Md::POSRIGHT,Md::POSFRONT);
    comboBox_zlar_q->setCurrentIndex(indextmp);
    /**********右口前床指令H*********************/
    indextmp = pattern->cnt_Hzhiling(row,Md::POSRIGHT,Md::POSFRONT);
    comboBox_zlhr_q->setCurrentIndex(indextmp);


    /**********左口后床色代号A*********************/
    lineEdit_sdhal->setText(pattern->cnt_seDaiHaoA(row,Md::POSLEFT,Md::POSREAR));
    /**********左口后床色代号H*********************/
    lineEdit_sdhhl->setText(pattern->cnt_seDaiHaoH(row,Md::POSLEFT,Md::POSREAR));
    /**********左口前床色代号A*********************/
    lineEdit_sdhal_q->setText(pattern->cnt_seDaiHaoA(row,Md::POSLEFT,Md::POSFRONT));
    /**********左口前床色代号H*********************/
    lineEdit_sdhhl_q->setText(pattern->cnt_seDaiHaoH(row,Md::POSLEFT,Md::POSFRONT));


    /**********右口后床色代号A*********************/
    lineEdit_sdhar->setText( pattern->cnt_seDaiHaoA(row,Md::POSRIGHT,Md::POSREAR));
    /**********右口后床色代号H*********************/
    lineEdit_sdhhr->setText(pattern->cnt_seDaiHaoH(row,Md::POSRIGHT,Md::POSREAR));
    /**********右口前床色代号A*********************/
    lineEdit_sdhar_q->setText(pattern->cnt_seDaiHaoA(row,Md::POSRIGHT,Md::POSFRONT));
    /**********右口前床色代号H*********************/
    lineEdit_sdhhr_q->setText(pattern->cnt_seDaiHaoH(row,Md::POSRIGHT,Md::POSFRONT));

    /**********左口纱嘴*********************/
    unsigned char sz1 =pattern->cnt_shaZui(row,Md::POSLEFT);
    for(int i=0;i<8;i++)
        pcheckBoxArray1[i]->setChecked(sz1&1<<i);
    /**********右口纱嘴*********************/
    unsigned char sz2 = pattern->cnt_shaZui(row,Md::POSRIGHT);
    for(int i=0;i<8;i++)
        pcheckBoxArray2[i]->setChecked(sz2&1<<i);
    /**********度目,类型*********************/
    //bool doubleorsigle;
    //unsigned char dumu1 = pattern->cnt_duMu(row,doubleorsigle);
    //lineEdit_dm1->setText(QString::number(dumu1));
    //lineEdit_lx->setText(doubleorsigle?tr("双面"):tr("单面"));
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
    lineEdit_xhs->setText(QString::number(pattern->cnt_FechData(row,CNT_LoopStart,2)));
    /**********循环次数*********************/
    lineEdit_xhcs->setText(QString::number(pattern->cnt_FechData(row,CNT_LoopNum,2)));
}

void QCntEditForm::on_pushButton_3_clicked()
{
    hide();
    deleteLater();
}

void QCntEditForm::on_pushButton_clicked()
{
    rowToggle(lineEdit_row->text().toInt());
}

void QCntEditForm::on_lineEdit_row_textChanged(QString text)
{
    pushButton_rowup->setDisabled(text.toInt()==1);
    pushButton_rowdown->setDisabled(text.toInt()==pattern->tatalcntrow);
}

void QCntEditForm::on_pushButton_rowdown_clicked()
{
    int row = lineEdit_row->text().toInt()+1;//new row;
    lineEdit_row->setText(QString::number(row));
    rowToggle(row);
}

void QCntEditForm::on_pushButton_rowup_clicked()
{
    int row = lineEdit_row->text().toInt()-1;//new row
    lineEdit_row->setText(QString::number(row));
    rowToggle(row);
}
