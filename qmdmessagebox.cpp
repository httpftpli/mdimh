#include "qmdmessagebox.h"
#include "csound.h"
#include <QTest>
#include <QDebug>
#include <QDir>

void QMdMessageBox::setVisible (bool visible){
    QMessageBox::setVisible(visible);
    if(visible){
        cSound sound;
        QString  path = QDir::currentPath();
        switch(icon()){
        case Question:
            sound.setFile(path+"/"+QString("resource/sound/question.wav"));
            break;
        case Information:
            sound.setFile(path+"/"+QString("resource/sound/information.wav"));
            break;
        case Warning:
            sound.setFile(path+"/"+QString("resource/sound/warning.wav"));
            break;
        case Critical:
            sound.setFile(path+"/"+QString("resource/sound/critical.wav"));
            break;
        default:
            break;
        }
       QTest::qWaitForWindowShown(this);
       sound.play();
    }
}

void QMdMessageBox::exec(const QString &text,const QString &informtext,
                            Icon icon,StandardButton buttons,
                            StandardButton defbutton){
    setText(text);
    setInformativeText(informtext);
    setIcon(icon);
    setStandardButtons(buttons);
    setDefaultButton(defbutton);
    QMessageBox::exec();
}

int QMdMessageBox::exec(){
    QMessageBox::exec();
}
