#include "qmdsplashscreen.h"

QMdSplashScreen::QMdSplashScreen(const QPixmap & pixmap ,
                                 Qt::WindowFlags f):
                    QSplashScreen(pixmap,f)
{
}

void QMdSplashScreen::showCntMessage(int percent){
    showMessage(tr("正在下载CNT文件 ------------  \%%1")
                .arg(percent),Qt::AlignBottom);
}

void QMdSplashScreen::showPatMessage(int percent){
    showMessage(tr("正在下载PAT文件-------------  \%%1")
                        .arg(percent),Qt::AlignBottom);
}

