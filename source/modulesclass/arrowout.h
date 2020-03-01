#ifndef ARROWOUT_H
#define ARROWOUT_H

#include <QtWidgets>

#include "modulesclass/mline.h"
#include "modulesclass/itembase.h"
#include "modulesclass/modulebase.h"
#include "modulesclass/arrowin.h"

class ModuleBase;
class ArrowIn;

class ArrowOut :  public ItemBase
{
    Q_OBJECT
public:
    ArrowOut(QGraphicsItem* parentItem = nullptr);
    ~ArrowOut();
    QRectF  boundingRect()const override;
    int     type() const override;
    void    updateStatus();
    void    updateDefault();
    void    updateLine();
    enum ArrowState
    {
        Default = 0,
        Suspend,
        Move,
        Connect,
    };

    bool f_movePoint;
    int arrowState;
    QGraphicsItem *parentItem;
    ArrowIn  *connectArrow;

    QVector<QPointF> pointList;
    QList<Mline*>  linelist;
protected:
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void    hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void    hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool    sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

    bool    f_isGrabMouse;
    QPointF pressPoint;
    QPen    Pen1, Pen2, Pen3;
    QPainterPath arrowPath;
    QPainterPath seletedArrowPath;

signals:

public slots:
    void upDateBeginPoint(QPointF);
    void upDateEndPoint(QPointF);
    void slot_sceneMove(QPointF);
    void slot_selectionAreaChange();
};

#endif // ARROWOUT_H
