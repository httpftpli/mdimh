#ifndef PARAMFORM_H
#define PARAMFORM_H
#include "ui_paramform.h"
#include "data.h"
#include "globaldata.h"
#include"config.h"

class FormJqgzcs;

#if DUAL_SYSTEM
class FormXtcs2;
#else
class FormXtcs;
#endif

class paramform : public QWidget, private Ui::paramform
{
    Q_OBJECT

public:
    enum ParamFormIndex{
        Index_dmz = 0,
        Index_llzzdj,
        Index_ss,
        Index_sztf,
        Index_ytx,
        Index_cjp,
        Index_pzk,
        Index_qt,
        Index_dmbc,
        Index_ycbc,
        Index_fzycbc,
        Index_qjgzcs,
        Index_qjxtcs
    };

    explicit paramform(QComm *com,QWidget *parent = 0);
    ~paramform();
    void setPattern(QPattern *pattern);
    void setParama(QParam *parama);
    void setIndex(ParamFormIndex index);

protected:
    bool event(QEvent *e);
private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void on_lineEdit_qzd_textChanged(QString );
    void on_spinBox_valueChanged(int );
    void on_tabWidget_currentChanged(int index);
    void on_listWidget_2_currentRowChanged(int currentRow);
    void on_stackedWidget_currentChanged(int );
    void on_qMdPushButton_clicked();
    void indexchange(int index);
    void readabspinpos();
    ///////////////////////////////
signals:
    void indexchanged(int index);
private:
    void creatextgzcsform(int indexofpage);
    void createjqgzcsform(int indexofpage);
    QPattern *patterndata;
    QParam *paramadata;
    QDMZModel *dmzmodel;
    QLLModel *llmodel;
    QZDJModel *zdjmodel;
    QSSModel *ssmodel;
    QSZTFModel *sztfmodel;
    QYTXSZXZModel *ytxszxzmodel;
    QYTXSZTFModel *ytxsztfmodel;
    QCJPModel *cjpmodel;
    QPZKModel *pzkmodel;
    QSzkbModel *szkbmodel;
    QDMBCModel *dmbcmodel;
    QYCWZXZModel *ycwzxzmodel;
    QFZYCWZXZModel *fzycwzxzmodel;
    FormJqgzcs *formjqgzcs;
    QComm *pcomm;
    QLabel *label_jtzhenshu_xtcs;
    QLabel *label_jtzhenshu_jqgzcs;
#if DUAL_SYSTEM
    FormXtcs2   *formxtcs;
#else
    FormXtcs   *formxtcs;
#endif
    int showindex;
    QTimer timer;//timer for read absolute pin position

    ////////////////////////////
};

#endif // PARAMFORM_H
