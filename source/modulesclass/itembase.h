#ifndef ITEMBASE_H
#define ITEMBASE_H

#define cout  qDebug() << __LINE__ << ":"

#include <QtWidgets>

class ItemBase : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit ItemBase(QGraphicsItem *parent = Q_NULLPTR);
    ~ItemBase();

    virtual int type() const override;

    enum {
        ItemBaseType = 0,
        MoudleType,
        ConnectInType,
        ConnectOutType,
        LineType,
        InModuleType,
        OutModuleType
    };
    static int microMove;

protected: 
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:

public slots:
};

#endif // ITEMBASE_H
