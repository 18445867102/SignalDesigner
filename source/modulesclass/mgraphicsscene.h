#ifndef MGRAPHICSSCENE_H
#define MGRAPHICSSCENE_H

#include <QtWidgets>
#include "modulesclass/itembase.h"
#include "modulesclass/arrowout.h"
#include "modulesclass/inmodule.h"
#include "modulesclass/outmodule.h"

class MGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MGraphicsScene();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

signals:
    void signal_sceneMove(QPointF);
};

#endif // MGRAPHICSSCENE_H
