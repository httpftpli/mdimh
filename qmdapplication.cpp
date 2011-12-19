#include "qmdapplication.h"
#include "csound.h"
#include "formmovie.h"
#include <QDir>


QMdApplication::QMdApplication ( int & argc, char ** argv ):QApplication(argc, argv),mousesound(0),
                            scrprodelay(0),screenprotect(NULL){
    timerid = this->startTimer(1000);

}

bool QMdApplication::notify ( QObject * receiver, QEvent * event ){
    bool r = QApplication::notify(receiver,event);
        if(event->type()==QEvent::MouseButtonPress){
            mousesound->play();
            second = 0;
        }
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
        if(!screenprotect)
            screenprotect = new QProcess(this);
        if(screenprotect->state()==QProcess::QProcess::NotRunning){

            const QString mplayerPath("/opt/hmi/ScreenProtect");
            QStringList args;
            args << QDir::currentPath()+"/media";
            screenprotect->start(mplayerPath,args);
        }
    }
}

void QMdApplication::setscreenprodelay(int s){
    this->scrprodelay = s;
    second = 0;
}
