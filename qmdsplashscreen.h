#ifndef QMDSPLASHSCREEN_H
#define QMDSPLASHSCREEN_H
#include<QSplashScreen>

class QMdSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    QMdSplashScreen(const QPixmap & pixmap = QPixmap(),
                    Qt::WindowFlags f = 0 );
public slots:
    void showCntMessage(int percent);  //开机显示下载进度时使用
    void showPatMessage(int percent);  //开机显示下载进度时使用
};

#endif // QMDSPLASHSCREEN_H
