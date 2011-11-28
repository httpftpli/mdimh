#include "qshazuiexchform.h"


QShazuiExchForm::QShazuiExchForm(QWidget *parent, QPatternData *data) :
    QDialog(parent),pattern(data),currentshazui(NULL){
    setupUi(this);
    ///////////////////////////////
    labelarray1[0]=label01;
    labelarray1[1]=label02;
    labelarray1[2]=label03;
    labelarray1[3]=label04;
    labelarray1[4]=label05;
    labelarray1[5]=label06;
    labelarray1[6]=label07;
    labelarray1[7]=label08;

    labelarray2[0]=label11;
    labelarray2[1]=label12;
    labelarray2[2]=label13;
    labelarray2[3]=label14;
    labelarray2[4]=label15;
    labelarray2[5]=label16;
    labelarray2[6]=label17;
    labelarray2[7]=label18;

    pushbuttonarray3[0]=pushButton01;
    pushbuttonarray3[1]=pushButton02;
    pushbuttonarray3[2]=pushButton03;
    pushbuttonarray3[3]=pushButton04;
    pushbuttonarray3[4]=pushButton05;
    pushbuttonarray3[5]=pushButton06;
    pushbuttonarray3[6]=pushButton07;
    pushbuttonarray3[7]=pushButton08;

    for(int i=0;i<8;i++){
       pushbuttonarray1[i] = NULL;
       pushbuttonarray2[i] = NULL;
       connect( pushbuttonarray3[i],SIGNAL(clicked()),SLOT(setCurrentShazuiText()));
    }
    addNewShazui();

    ///////////////////////////////
}


void QShazuiExchForm::addNewShazui(){
    unsigned char szleft = pattern->shazuiused_l;
    unsigned char szright = pattern->shazuiused_r;
    for(int i=0;i<8;i++){
        delete pushbuttonarray1[i];
        delete pushbuttonarray2[i];
    }

    for(int i=0;i<8;i++){

        if(szleft&(1<<i)){
            pushbuttonarray1[i] = new QPushButton(this);
            pushbuttonarray1[i]->setText(QString::number(i+1));
            QRect rect = QRect(labelarray1[i]->x()+labelarray1[i]->width()+10,labelarray1[i]->y(),
                                     labelarray1[i]->width(),labelarray1[i]->height());
            pushbuttonarray1[i]->setGeometry(rect.adjusted(pushbuttonarray1[i]->parentWidget()->x(),pushbuttonarray1[i]->parentWidget()->y(),0,0));
            pushbuttonarray1[i]->show();

        }
        else
            pushbuttonarray1[i] = NULL;

        if(szright&(1<<i)){
            pushbuttonarray2[i] = new QPushButton(this);
            pushbuttonarray2[i]->setText(QString::number(i+1));
            QRect rect = QRect(labelarray2[i]->x()-labelarray2[i]->width()-10,labelarray2[i]->y(),
                                     labelarray2[i]->width(),labelarray2[i]->height());
            pushbuttonarray2[i]->setGeometry(rect.adjusted(pushbuttonarray2[i]->parentWidget()->x(),pushbuttonarray2[i]->parentWidget()->y(),0,0));
            pushbuttonarray2[i]->show();

        }
        else
            pushbuttonarray2[i]=NULL;
    }
}

void QShazuiExchForm::on_pushButton_6_clicked()
{
    hide();
    deleteLater();
}


void QShazuiExchForm::setCurrentShazuiText(){
    QPushButton *currentshazui = qobject_cast<QPushButton *>(focusWidget());
    QPushButton *button = qobject_cast<QPushButton *>(QObject::sender());
    if(currentshazui&&button)
        currentshazui->setText(button->text());
    pushButton_4->setEnabled(checkchange());
}

bool QShazuiExchForm::checkchange(){
    for(int i=0;i<8;i++){
        if(pushbuttonarray1[i])
            if(pushbuttonarray1[i]->text()!=labelarray1[i]->text())
                return TRUE;
        if(pushbuttonarray2[i])
            if(pushbuttonarray2[i]->text()!=labelarray2[i]->text())
                return TRUE;
    }
    return FALSE;
}
