#ifndef FRAME_H
#define FRAME_H

#include "ui_frame.h"

class Frame : public QFrame, private Ui::Frame
{
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = 0);

private slots:
    void on_qMdPushButton_clicked();
};

#endif // FRAME_H
