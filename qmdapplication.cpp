#include "qmdapplication.h"
#include "csound.h"

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
