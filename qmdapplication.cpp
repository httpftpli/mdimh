#include "qmdapplication.h"
#include "csound.h"
#include "formmovie.h"

bool QMdApplication::notify ( QObject * receiver, QEvent * event ){
    bool r = QApplication::notify(receiver,event);
        if(event->type()==QEvent::MouseButtonPress){
            mousesound->play();        }
        return  r;
    }

void QMdApplication::setMouseClickSound(const QString &filename){
    mousesound = new cSound(filename);
}

QMdApplication::~QMdApplication(){
    delete mousesound;
}

void QMdApplication::timerEvent ( QTimerEvent * event ){
    if(event->timerId()==timerid)
        second++;
    if((scrprodelay!=0)&&(scrprodelay==second)){
        second = 0;
        if(!formmovie){
            formmovie = QPointer<FormMovie>(new FormMovie);
            formmovie->show();
        }
    }
}

void QMdApplication::setscreenprodelay(int s){
    this->scrprodelay = s;
    second = 0;
}
