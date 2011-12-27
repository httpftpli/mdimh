#include "qmdapplication.h"
#include "csound.h"
#include <QDir>
#include <QWSEvent>
#include <QDebug>


QMdApplication::QMdApplication ( int & argc, char ** argv ):QApplication(argc, argv){
    //timerid = this->startTimer(1000);

}

/*bool QMdApplication::notify ( QObject * receiver, QEvent * event ){
    bool r = QApplication::notify(receiver,event);
        if(event->type()==QEvent::MouseButtonPress){
            mousesound->play();
            second = 0;
        }
        return  r;
    }
    */

/*void QMdApplication::setMouseClickSound(const QString &filename){
    mousesound = new cSound(filename);
}*/

/*QMdApplication::~QMdApplication(){
    delete mousesound;
}*/


/*bool QMdApplication::qwsEventFilter(QWSEvent * event){
    if(event->type == QWSEvent::Key){
        QWSKeyEvent *key = static_cast<QWSKeyEvent *>(event);
        qDebug()<<"keyunicode "<<key->simpleData.unicode
               <<"keycode "<<key->simpleData.keycode
                 <<"modifier "<<key->simpleData.modifiers;
    }
        return QApplication::qwsEventFilter(event);
}*/

/*void QMdApplication::setscreenprodelay(int s){

}*/
