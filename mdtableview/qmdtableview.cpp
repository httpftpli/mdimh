#include"qmdtableview.h"
#include<QEvent>
#include <QMouseEvent>
#include <QScrollBar>

QMdTableView::QMdTableView(QWidget *parent):QTableView(parent),willscroll(FALSE){

}


void QMdTableView::mouseMoveEvent ( QMouseEvent * event ){
   if(event->buttons()==Qt::LeftButton){
        int  disx = event->globalX()-oldX1;
        int  disy = event->globalY()-oldY1;
        if((qAbs(disx)>10)/*&&(horizontalScrollBar()->isVisible())*/){
            QScrollBar *bar = horizontalScrollBar();
            bar->setValue(oldhscrollval-disx);
            willscroll = TRUE;
            event->accept();
            return;
        }
        if((qAbs(disy)>10)/*&&(verticalScrollBar()->isVisible())*/){
            QScrollBar *bar = verticalScrollBar();
            bar->setValue(oldvscrollval-disy);
            willscroll = TRUE;
            event->accept();
            return;
        }
    }
    QTableView::mouseMoveEvent(event);
}

void QMdTableView::mousePressEvent ( QMouseEvent * event ){
   if(event->button()==Qt::LeftButton){
        oldX1 = event->globalX();
        oldY1 = event->globalY();
        oldvscrollval = verticalScrollBar()->value();
        oldhscrollval = horizontalScrollBar()->value();
    }      
   QTableView::mousePressEvent(event);
}


void QMdTableView::mouseReleaseEvent ( QMouseEvent * event ){
    if((event->button()==Qt::LeftButton)&&(willscroll==TRUE)){
        willscroll = FALSE;
        event->accept();
    }else{
        QTableView::mouseReleaseEvent(event);
    }
}


bool QMd1TableView::event(QEvent *event){

}


QMd1TableView::QMd1TableView(QWidget *parent):QTableView(parent){

}
