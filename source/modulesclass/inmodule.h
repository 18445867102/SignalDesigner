#ifndef INMODULE_H
#define INMODULE_H

#include "itembase.h"
#include "arrowout.h"

class ArrowOut;
class InModule : public QGraphicsWidget
{
public:
    InModule(QGraphicsItem *parent = nullptr);
    ~InModule();
    QRectF  rect;

    ArrowOut *arrowOut;
protected:
    QRectF  boundingRect() const override;
    int     type() const override;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *moduleWidget);
    void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QPen mPen;
    bool f_hoverEnter;  //鼠标是否进入边界矩形标志
    bool f_changeRec;   //是否改变边界矩形标志
    int p_cursorSet;   //边界矩形四个角小矩形的1/2边长

signals:
};

#endif // INMODULE_H
