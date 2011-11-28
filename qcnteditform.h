#ifndef QCNTEDITFORM_H
#define QCNTEDITFORM_H

#include "ui_qcnteditform.h"
#include "pattern.h"
#include "data.h"

class QCntEditForm : public QWidget, private Ui::QCntEditForm
{
    Q_OBJECT

public:
    explicit QCntEditForm(QWidget *parent ,QPatternData *data);
    ~QCntEditForm(){
        if(!iscntloaded)
            pattern->deloadFile(Md::HAVECNT);
    }
public slots:
    void rowToggle(int row);
private:
    QPatternData *pattern;
    QPushButton *pcheckBoxArray1[8];
    QPushButton *pcheckBoxArray2[8];
    bool iscntloaded;  //保存原来是否载入了CNT 文件，如果原来没有载入了，
                            //本窗口退出后布需要卸载cnt缓冲区


private slots:
    void on_pushButton_rowup_clicked();
    void on_pushButton_rowdown_clicked();
    void on_lineEdit_row_textChanged(QString );
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
};

#endif // QCNTEDITFORM_H
