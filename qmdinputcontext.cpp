#include "qmdinputcontext.h"
#include <QPointer>
#include <QScreen>
#include "csound.h"
#include "globaldata.h"

QMdInputContext::QMdInputContext(QObject *parent) :
    QInputContext(parent),inputPanel(new inputpanel)
{
    connect(inputPanel, SIGNAL(keyGenerated(int,QChar,Qt::KeyboardModifiers)), SLOT(sendKey(int,QChar,Qt::KeyboardModifiers)));
}


QMdInputContext::~QMdInputContext()
{
    delete inputPanel;
}

bool QMdInputContext::filterEvent(const QEvent* event)
{
    QEvent::Type t = event->type();
    if (t == QEvent::RequestSoftwareInputPanel) {
        if(!inputPanel->isLock())
            updatePosition();
        inputPanel->show();
        return true;
    }
    else if (t == QEvent::CloseSoftwareInputPanel) {
        if(!inputPanel->isLock())
            inputPanel->hide();
        return true;
    }

    return false;
}

QString QMdInputContext::identifierName()
{
    return "MyInputPanelContext";
}

void QMdInputContext::reset()
{

}

bool QMdInputContext::isComposing() const
{
    return false;
}

QString QMdInputContext::language()
{
    return "en_US";
}

void QMdInputContext::sendKey(int keycode ,QChar character,Qt::KeyboardModifiers modify)
{
    QPointer<QWidget> w = focusWidget();
    if (!w)
        return;

    QKeyEvent keyPress(QEvent::KeyPress, keycode, modify, QString(character));
    QApplication::sendEvent(w, &keyPress);

    if (!w)
        return;

    QKeyEvent keyRelease(QEvent::KeyRelease, keycode, modify, QString(character));
    QApplication::sendEvent(w, &keyRelease);
}



void QMdInputContext::updatePosition()
{
#ifdef  Q_WS_QWS
    QWidget *widget = focusWidget();
    if (!widget)
        return;
    QScreen *screen = QScreen::instance();
    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    int xdis = panelPos.x()+inputPanel->width()-screen->width();
    xdis = qMax(0,xdis);
    panelPos.setX(panelPos.x()-xdis);
    int ydis = panelPos.y()+inputPanel->height()-screen->height();
    if(ydis>0)
        panelPos.setY(panelPos.y()-inputPanel->height()-widgetRect.height()-4);
    inputPanel->move(panelPos);
#endif
}
