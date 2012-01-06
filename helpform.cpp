#include "helpform.h"

HelpForm::HelpForm(QWidget *parent) :
    QMainWindow(parent,Qt::FramelessWindowHint),webview(new QWebView)
{
    setupUi(this);
    setCentralWidget(webview);
    QWebPage *page = webview->page();
    page->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
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
