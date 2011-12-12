#include "formmovie.h"
#include <QProcess>
#include <QEventLoop>
#include <QTest>

FormMovie::FormMovie(QWidget *parent) :
        QWidget(parent,Qt::FramelessWindowHint){
    setupUi(this);
    setAttribute(Qt::WA_PaintOnScreen,TRUE);
    setAttribute(Qt::WA_TranslucentBackground,TRUE);
    const QString mplayerPath("/opt/Mplayer/mplayer/bin/mplayer");
    QStringList args;
    args << "-ac";
    args << "mad";
    args << "-slave";
    args << "-quiet";
    args << "sucai2.avi"; //bingheshiji.mp4";
    myProcess=new QProcess(this);
    myProcess->start(mplayerPath,args);
    //volatile int i=0;
    //while ((myProcess->state()!=QProcess::Running)&&(i++ < 200))
         //QTest::qWait(250);
    //myProcess->write("vo_fullscreen\n");
    //myProcess->write("mute\n");
    //myProcess->write("volume 0\n");
}

bool FormMovie::event(QEvent *event){
    if((event->type()==QEvent::KeyPress)||(event->type()==QEvent::MouseButtonPress)){
      myProcess->write("stop\n");
      myProcess->write("quit\n");
      //myProcess->kill();
      hide();
      QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
      QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
      QWidgetList widgetlist = QApplication::topLevelWidgets();
      deleteLater();
      foreach(QWidget *widget,widgetlist){
          widget->update();
      }
      return TRUE;
    }
    return QWidget::event(event);
}



