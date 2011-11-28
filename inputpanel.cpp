#include "inputpanel.h"
#include <QMouseEvent>

inputpanel::inputpanel(QWidget *parent) :
        QWidget(parent,Qt::WindowStaysOnTopHint|Qt::Tool|Qt::FramelessWindowHint),moved(FALSE),lock(FALSE){
    setupUi(this);

    signalMapper.setMapping(panelButton_1, panelButton_1);
    signalMapper.setMapping(panelButton_2, panelButton_2);
    signalMapper.setMapping(panelButton_3, panelButton_3);
    signalMapper.setMapping(panelButton_4, panelButton_4);
    signalMapper.setMapping(panelButton_5, panelButton_5);
    signalMapper.setMapping(panelButton_6, panelButton_6);
    signalMapper.setMapping(panelButton_7, panelButton_7);
    signalMapper.setMapping(panelButton_8, panelButton_8);
    signalMapper.setMapping(panelButton_9, panelButton_9);
    signalMapper.setMapping(panelButton_star, panelButton_star);
    signalMapper.setMapping(panelButton_0, panelButton_0);
    signalMapper.setMapping(panelButton_minu,panelButton_minu);
    signalMapper.setMapping(panelButton_del,panelButton_del);
    signalMapper.setMapping(panelButton_backspace,panelButton_backspace);
    signalMapper.setMapping(panelButton_a,panelButton_a);
    signalMapper.setMapping(panelButton_b,panelButton_b);
    signalMapper.setMapping(panelButton_c,panelButton_c);
    signalMapper.setMapping(panelButton_d,panelButton_d);
    signalMapper.setMapping(panelButton_e,panelButton_e);
    signalMapper.setMapping(panelButton_f,panelButton_f);
    panelButton_a->setVisible(FALSE);
    panelButton_b->setVisible(FALSE);
    panelButton_c->setVisible(FALSE);
    panelButton_d->setVisible(FALSE);
    panelButton_e->setVisible(FALSE);
    panelButton_f->setVisible(FALSE);
    panelButton_star->setVisible(FALSE);
    panelButton_minu->setVisible(FALSE);

    connect(panelButton_1, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_2, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_3, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_4, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_5, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_6, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_7, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_8, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_9, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_star, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_0, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_del, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_backspace, SIGNAL(clicked()),
          &signalMapper, SLOT(map()));
    connect(panelButton_a,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(panelButton_b,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(panelButton_c,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(panelButton_d,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(panelButton_e,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(panelButton_f,SIGNAL(clicked()),
            &signalMapper,SLOT(map()));
    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
          this, SLOT(buttonClicked(QWidget*)));
    QObjectList childrenlist= children();
    foreach(QObject *obj,childrenlist){
        obj->installEventFilter(this);
    }
}


bool inputpanel::eventFilter( QObject * watched, QEvent * event ){
    Q_UNUSED(watched);
    if(event->type() ==QEvent::MouseButtonPress){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->button()==Qt::LeftButton){
            mousepos = e->globalPos();
            widgetpos = this->pos();
        }
    }
    if(event->type() ==QEvent::MouseMove){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->buttons()==Qt::LeftButton){
            QPoint point =e->globalPos()- mousepos;
            if(point.manhattanLength()>20){
                setGeometry(widgetpos.x()+point.x(),widgetpos.y()+point.y(),width(),height());
                moved = TRUE;
            }
        }
    }
    if(event->type() ==QEvent::MouseButtonRelease){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if((e->button()==Qt::LeftButton)&&(moved==TRUE)){
            moved = FALSE;
            return TRUE;

        }
    }
    return FALSE;
}

bool inputpanel::isLock(){
    return this->lock;
}

bool inputpanel::event ( QEvent * event ){
    if(event->type() ==QEvent::MouseButtonPress){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->button()==Qt::LeftButton){
            mousepos = e->globalPos();
            widgetpos = this->pos();
        }
    }
    if(event->type() ==QEvent::MouseMove){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if(e->buttons()==Qt::LeftButton){
            QPoint point =e->globalPos()- mousepos;
            if(point.manhattanLength()>20){
                setGeometry(widgetpos.x()+point.x(),widgetpos.y()+point.y(),width(),height());
                moved = TRUE;
            }
        }
    }
    if(event->type() ==QEvent::MouseButtonRelease){
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if((e->button()==Qt::LeftButton)&&(moved==TRUE)){
            moved = FALSE;
            return TRUE;

        }
    }
    return QWidget::event(event);
}

void inputpanel::buttonClicked(QWidget *w)
{
    if((w==panelButton_del)||(w==panelButton_backspace))
        emit keyGenerated(w->property("keycode").toInt(),QChar(),Qt::NoModifier);
    //if(w==panelButton_pad)
        //emit keyGenerated(w->property("keycode").toInt(),QChar('p'));
    else{
        QChar chr = w->property("buttonValue").toChar();
        emit keyGenerated(chr.unicode(),chr,Qt::NoModifier);
    }
}


void inputpanel::setLock(bool lock){
    this->lock = lock;
}
