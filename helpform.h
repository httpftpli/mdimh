#ifndef HELPFORM_H
#define HELPFORM_H

#include "ui_helpform.h"
#include <QtWebKit>
#include <QUrl>

class HelpForm : public QMainWindow, private Ui::HelpForm
{
    Q_OBJECT
    
public:
    explicit HelpForm(QWidget *parent = 0);
    QWebView *webview;
public slots:
    void loadUrl(const QUrl &url);
private slots:
    void on_action_quit_triggered();

private:

};


class QMyWebView:public QWebView
{
public:
    explicit QMyWebView(QWidget *parent =0);
protected:
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
private:
    int oldhscrollval;
    int oldvscrollval;
    int oldY1;
    int oldX1;
    bool willscroll;
};

#endif // HELPFORM_H
