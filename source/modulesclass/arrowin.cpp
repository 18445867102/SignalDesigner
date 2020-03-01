#include "arrowin.h"

ArrowIn::ArrowIn(QGraphicsItem *parent)
    :ItemBase(parent)
{
    setFlags(QGraphicsItem::ItemStacksBehindParent);
    this->rect = QRect(-6,-6,12,12);
    this->isConnect = false;

    pen.setColor(Qt::black);
    pen.setWidthF(1.8);
}

QRectF ArrowIn::boundingRect()const
{
    qreal adjust = 1;
    return QRectF(rect.adjusted(-adjust/2,-adjust/2,adjust,adjust));
}

int ArrowIn::type() const
{
    return ItemBase::ConnectInType;
}

void ArrowIn::updateStatus()
{
    if(isConnect)
    {
        isConnect = false;

        connectArrow->arrowState = ArrowOut::Suspend;
        connectArrow->updateLine();
    }
}

void ArrowIn::updataStatus(bool flag)
{
    if(flag)
    {
        hide();
        isConnect = true;
    }else
    {
        show();
        isConnect = false;
    }
}

void ArrowIn::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setPen(pen);
    painter->drawLine(-5,5,0,0);
    painter->drawLine(-5,-5,0,0);
}

