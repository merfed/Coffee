#include "adtarray.h"

AdtArray::AdtArray() : QGraphicsView()
{
}


void AdtArray::mouseMoveEvent(QMouseEvent *eventMove)
{
    emit mousePos(eventMove->x(), eventMove->y());
}
