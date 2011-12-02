#ifndef QMDMESSAGEBOX_H
#define QMDMESSAGEBOX_H

#include <QMessageBox>

class QMdMessageBox : public QMessageBox
{
    Q_OBJECT
public:
    QMdMessageBox(QWidget *parent=0):QMessageBox(parent){
    }
    QMdMessageBox(Icon icon, const QString &title, const QString &text,
                StandardButtons buttons = NoButton, QWidget *parent = 0,
                Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint):
            QMessageBox(icon,title,text,buttons,parent,flags){
    }

    virtual void setVisible (bool visible);
    void exec(const QString &text,const QString &informtext,
              Icon icon,StandardButton buttons,
              StandardButton defbutton);
public slots:
    int exec();

};

#endif // QMDMESSAGEBOX_H
