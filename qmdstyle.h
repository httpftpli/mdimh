#ifndef QMDSTYLE_H
#define QMDSTYLE_H

#include <QPlastiqueStyle>

class QMdStyle : public QPlastiqueStyle
{
    Q_OBJECT

public:
    QMdStyle(){ }


    void polish(QWidget *widget);
    void unpolish(QWidget *widget);
    //int pixelMetric(PixelMetric which, const QStyleOption *option,
                   // const QWidget *widget = 0) const;

    void drawComplexControl(ComplexControl which,
                            const QStyleOptionComplex *option,
                            QPainter *painter,
                            const QWidget *widget = 0) const;
    QRect subControlRect(ComplexControl whichControl,
                         const QStyleOptionComplex *option,
                         SubControl whichSubControl,
                         const QWidget *widget = 0) const;

public slots:
    //QIcon standardIconImplementation(StandardPixmap which,
    //                                  const QStyleOption *option,
    //                                const QWidget *widget = 0) const;

private:
    void drawBronzeFrame(const QStyleOption *option,
                         QPainter *painter) const;
    void drawBronzeBevel(const QStyleOption *option,
                         QPainter *painter) const;
    void drawBronzeSpinBoxButton(SubControl which,
                                 const QStyleOptionComplex *option,
                                 QPainter *painter) const;
};

#endif // QMDSTYLE_H
