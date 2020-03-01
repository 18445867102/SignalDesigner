#include "itembase.h"

ItemBase::ItemBase(QGraphicsItem *parent) : QGraphicsObject(parent)
{
}

int ItemBase::microMove = 10;

int ItemBase::type() const
{
    return ItemBaseType;
}

QRectF ItemBase::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

ItemBase::~ItemBase()
{

}
