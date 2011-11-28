#ifndef INPUTPANEL_H
#define INPUTPANEL_H

#include "ui_inputpanel.h"
#include <QSignalMapper>

class inputpanel : public QWidget, private Ui::inputpanel
{
    Q_OBJECT

public:
    explicit inputpanel(QWidget *parent = 0);
    bool eventFilter(QObject *, QEvent *);
    bool isLock();
protected:
    bool event ( QEvent * event );
signals:
    void keyGenerated(int keycode,QChar character,Qt::KeyboardModifiers modify);
private slots:
    //void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);
    void setLock(bool lock);

private:
    //QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;
    QPoint mousepos;
    QPoint widgetpos;
    bool moved;
    bool lock;
    //mouseMove();
};

#endif // INPUTPANEL_H
