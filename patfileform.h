#ifndef PATFILEFORM_H
#define PATFILEFORM_H


#include "ui_patfileform.h"
#include <QFile>
#include <data.h>
#include <globaldata.h>

class Position : public QObject {
    Q_OBJECT
public:
    int row;
    int column;
    Position(int row=0,int column= 0):row(row),column(column){
    }
    void setData(int row,int column){
        if(this->row!=row){
            this->row = row;
            emit rowChanged(QString::number(row));
            emit rowChanged(row);
        }
        if(this->column!=column){
            this->column = column;
            emit columnChanged(QString::number(column));
            emit columnChanged(column);
        }
    }
signals:
    void rowChanged(QString row);
    void columnChanged(QString column);
    void rowChanged(int row);
    void columnChanged(int column);
public slots:
    void setRow(QString val){
        int i = val.toInt();
        if(i!=row){
            row = i;
            emit rowChanged(val);
            emit rowChanged(i);
        }
    }

    void setColumn(QString val){
        int i = val.toInt();
        if(i!=column){
            column = i;
            emit columnChanged(val);
            emit columnChanged(i);
        }
    }

};


class PatFileForm : public QWidget, private Ui::PatFileForm
{
    Q_OBJECT

public:
    explicit PatFileForm(QWidget *parent ,QPattern *data);
    void jumpTo(int row,int column,QAbstractItemView::ScrollHint scrollhint);

    /////////////////////////////////////////
    ~PatFileForm();
private:
    QPattern *pattern;
    /////////////////////////////////////////
    Md::HAVEFILEFLAG oldflag ;//保存patterndata
    QPatModel *patmodel;

private slots:
    void on_pushButton_pageright_clicked();
    void on_pushButton_pageleft_clicked();
    void on_pushButton_columnlast_clicked();
    void on_pushButton_columnfirt_clicked();
    void on_pushButton_pageup_clicked();
    void on_pushButton_pagedown_clicked();
    void on_pushButton_rowlast_clicked();
    void on_pushButton_rowfist_clicked();
    void on_qMdPushButton_4_clicked();
    void on_pushButton_softkey_clicked();
    void on_pushButton_rowdown_clicked();
    void on_pushButton_rowup_clicked();
    void on_pushButton_columnright_clicked();
    void on_pushButton_columnleft_clicked();
    void on_pushButton_jumpto_clicked();
    void on_qMdPushButton_3_clicked();
    void tableViewIsEditing(QModelIndex index);
    void decideButtonRl(int column); //判断左右按钮的失效
    void decideButtonUd(int row); //判断上下按钮的失效
private:
    Position pos;
    int pagerow ;
    int pagecolum;

};



#endif // PATFILEFORM_H
