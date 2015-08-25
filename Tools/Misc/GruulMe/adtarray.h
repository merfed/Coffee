#ifndef ADTARRAY_H
#define ADTARRAY_H

#include <QApplication>
#include <QtGui>

class AdtArray : public QGraphicsView
{
    Q_OBJECT

    public:

        AdtArray();

    signals:

        void mousePos(int, int);

    private:

        void mouseMoveEvent(QMouseEvent *eventMove);

};

#endif // ADTARRAY_H
