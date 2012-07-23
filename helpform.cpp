#include "helpform.h"

HelpForm::HelpForm(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),webview(new QMyWebView)
{
    setupUi(this);
    setCentralWidget(webview);
    QWebPage *page = webview->page();
    page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    page->setContentEditable(FALSE);
    webview->setContextMenuPolicy(Qt::NoContextMenu);
    connect(page,SIGNAL(linkClicked(QUrl)),SLOT(loadUrl(QUrl)));
    connect(action_back,SIGNAL(triggered()),webview,SLOT(back()));
    connect(action_forward,SIGNAL(triggered()),webview,SLOT(forward()));
    connect(action_refresh,SIGNAL(triggered()),webview,SLOT(reload()));
}

void HelpForm::loadUrl(const QUrl &url)
{
    webview->load(url);
}

void HelpForm::on_action_quit_triggered()
{
    hide();
    deleteLater();
}


QMyWebView::QMyWebView(QWidget *parent ):QWebView(parent),willscroll(FALSE)
{

}

void QMyWebView::mouseMoveEvent ( QMouseEvent * event ){
    if((event->buttons()==Qt::LeftButton)&&(page()->mainFrame())){
        int  disx = event->globalX()-oldX1;
        int  disy = event->globalY()-oldY1;
        if((qAbs(disx)>10)/*&&(horizontalScrollBar()->isVisible())*/){
            QWebFrame  *frame = page()->mainFrame();
            frame->setScrollBarValue(Qt::Horizontal,oldhscrollval-disx);
            frame->setScrollBarValue(Qt::Vertical,oldvscrollval-disy);
            event->accept();
            return;
        }
    }
    QWebView::mouseMoveEvent(event);
}

void QMyWebView::mousePressEvent ( QMouseEvent * event ){
   if((event->button()==Qt::LeftButton)&&(page()->mainFrame())){
        QWebFrame *frame = page()->mainFrame();
        oldX1 = event->globalX();
        oldY1 = event->globalY();
        oldvscrollval = frame->scrollBarValue(Qt::Vertical);
        oldhscrollval = frame->scrollBarValue(Qt::Horizontal);
    }
   QWebView::mousePressEvent(event);
}


void QMyWebView::mouseReleaseEvent ( QMouseEvent * event ){
    int  disx = event->globalX()-oldX1;
    int  disy = event->globalY()-oldY1;
    if((event->button()==Qt::LeftButton)&&(QPoint(disx,disy).manhattanLength()>10)){
        event->accept();
    }else{
        QWebView::mouseReleaseEvent(event);
    }
}
