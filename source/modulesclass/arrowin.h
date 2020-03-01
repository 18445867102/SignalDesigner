#ifndef ARROWIN_H
#define ARROWIN_H

#include <QtWidgets>
#include "itembase.h"
#include "arrowout.h"

class ArrowOut;
class ArrowIn :  public ItemBase
{
    Q_OBJECT
public:
    ArrowIn(QGraphicsItem *parent = nullptr);
    int     type() const override;
    void    updateStatus();
    void    updataStatus(bool);

    bool isConnect;
    ArrowOut* connectArrow;
protected:
    QRectF  boundingRect()const override;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRect   rect;//边界矩形
    QPen    pen;//默认画笔

signals:
public slots:

};
#endif // ARROWIN_H
