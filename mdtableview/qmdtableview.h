#ifndef __mdtableview__h__
#define __mdtableview__h__

#include <QTableView>

class QMdTableView:public  QTableView{
    Q_OBJECT
public:
    explicit QMdTableView(QWidget *parent =0);
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

#endif
