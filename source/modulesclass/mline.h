#ifndef MLINE_H
#define MLINE_H

#include <QtWidgets>
#include "itembase.h"

class Mline : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    explicit Mline(QObject *parent = nullptr);
    ~Mline();

    int     lineDirect();
    int     lineType();
    void    updateStatus(bool);
    void    updataMoveFlag(bool);

    enum LineDirect
    {
        Error = -1,
        Up,
        Down,
        Left,
        Right,
    };

    enum LineType
    {
        NoType = 0,
        Horizontal,
        Vertical,
    };
protected:
    void  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int   type() const override;
   // QPainterPath shape() const;
    void  hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    qreal   width;
    bool    isConnect;
    bool    moveFlag;
    QPen    mPenDisconnnect, mPenConnect, mPenSelected;
    QPen    lineItemPen;
signals:
public slots:
};

#endif // MLINE_H
