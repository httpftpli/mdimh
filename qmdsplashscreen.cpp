#include "qmdsplashscreen.h"

QMdSplashScreen::QMdSplashScreen(const QPixmap & pixmap ,
                                 Qt::WindowFlags f):
                    QSplashScreen(pixmap,f)
{
}

void QMdSplashScreen::showPatternSendMessage(int percent){
    showMessage(tr("正在下载花型 ------------  \%%1")
                .arg(percent),Qt::AlignBottom);
}

