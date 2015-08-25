#include "adtarrayscene.h"

AdtArrayScene::AdtArrayScene() : QGraphicsScene()
{

}


void AdtArrayScene::mousePressEvent(QGraphicsSceneMouseEvent *eventClick)
{
    eventClick->accept();
    emit mouseIsPressed();
}
