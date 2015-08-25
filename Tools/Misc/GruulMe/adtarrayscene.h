#ifndef ADTARRAYSCENE_H
#define ADTARRAYSCENE_H

#include <QApplication>
#include <QtGui>

class AdtArrayScene : public QGraphicsScene
{
    Q_OBJECT

public:
    AdtArrayScene();

signals:
    void mouseIsPressed();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *eventClick);
};

#endif // ADTARRAYSCENE_H
