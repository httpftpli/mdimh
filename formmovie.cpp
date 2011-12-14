#include "formmovie.h"
#include <QProcess>
#include <QEventLoop>
#include <QTest>
#include<QDir>
#include<QTextStream>

FormMovie::FormMovie(QWidget *parent) :
        QWidget(parent,Qt::FramelessWindowHint){
    setupUi(this);
    setAttribute(Qt::WA_PaintOnScreen,TRUE);
    setAttribute(Qt::WA_TranslucentBackground,TRUE);
    QDir dir("./media");
    QStringList filters;
    filters<<"*.avi"<<"*.mp4"<<"*.rmvb"<<"*.rm";
    QStringList namelist = dir.entryList(filters,QDir::Files);
    QFile playlistfile("./media/.playlist.txt");
    playlistfile.open(QIODevice::ReadWrite);
    QTextStream stream(&playlistfile);
    QStringList temp;
    while(1){
        QString filename = (stream.readLine(75)).trimmed();
        if(filename.isNull())
            break;
        temp<<filename;
    }
    if(temp!=namelist){
        playlistfile.resize(0);
        foreach(QString str ,namelist){
            stream<<str<<endl;
        }
        playlistfile.flush();
    }
    playlistfile.close();
    const QString mplayerPath("/opt/Mplayer/mplayer/bin/mplayer");
    QStringList args;
    args << "-ac"<<"mad"<< "-slave"<<"-quiet"
         <<"-loop"<<"0"<<"-playlist"<<"./media/.playlist.txt"
         <<"-shuffle";
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

void FormMovie::makeplaylist(){

}


