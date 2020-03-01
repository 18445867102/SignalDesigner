#ifndef OUTMODULE_H
#define OUTMODULE_H

#include "itembase.h"
#include "arrowin.h"

class ArrowIn;
class OutModule : public QGraphicsWidget
{
public:
    OutModule(QGraphicsItem *parent = nullptr);
    ~OutModule();

    QRectF  rect;
    ArrowIn *arrowin;
protected:
    QRectF  boundingRect() const override;
    int     type() const override;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *moduleWidget);
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    bool f_hoverEnter;  //鼠标是否进入边界矩形标志
    bool f_changeRec;   //是否改变边界矩形标志
    int p_cursorSet;   //边界矩形四个角小矩形的1/2边长
    QPen mPen;
};

#endif // OUTMODULE_H
