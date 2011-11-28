#ifndef QMDINPUTCONTEXT_H
#define QMDINPUTCONTEXT_H

#include <QInputContext>
#include"inputpanel.h"

class QMdInputContext : public QInputContext
{
    Q_OBJECT
public:
    explicit QMdInputContext(QObject *parent = 0);
    ~QMdInputContext();
    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;
    void reset();
    inputpanel *inputPanel;
private slots:
    void sendKey(int keycode,QChar character,Qt::KeyboardModifiers modify);

private:
    void updatePosition();

private:

};


#endif // QMDINPUTCONTEXT_H
