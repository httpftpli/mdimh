#ifndef FORMMOVIE_H
#define FORMMOVIE_H

#include "ui_formmovie.h"
QT_BEGIN_NAMESPACE
class QProcess;
QT_END_NAMESPACE

class FormMovie : public QWidget, private Ui::FormMovie
{
    Q_OBJECT

public:
    explicit FormMovie(QWidget *parent = 0);
    bool event(QEvent *event);
private:
    QProcess *myProcess;
    void makeplaylist();
};

#endif // FORMMOVIE_H
