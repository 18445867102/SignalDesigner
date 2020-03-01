#include "inmodule.h"

InModule::InModule(QGraphicsItem *parent)
    :QGraphicsWidget(parent, Qt::Widget)
{
    setFlags(QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);

    this->rect = QRect(0,0,30,40);
    this->f_hoverEnter = false;
    this->f_changeRec = false;
    this->p_cursorSet = 5;
    resize(rect.size());

    mPen.setWidthF(1);
    mPen.setCapStyle(Qt::SquareCap);
    mPen.setJoinStyle(Qt::MiterJoin);
    mPen.setStyle(Qt::SolidLine);

    arrowOut = new ArrowOut(this);;
    arrowOut->setPos(rect.right()+5, rect.height()/2);
}

QRectF InModule::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(rect.adjusted(-adjust,-adjust,adjust,adjust));
}

int InModule::type() const
{
    return ItemBase::InModuleType;
}

void InModule::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *moduleWidget)
{
    Q_UNUSED(moduleWidget);

    QFont font("黑体",10);

    static const QPoint hourHand[5] = {
        QPoint(0, 0),
        QPoint(20, 0),
        QPoint(30, 20),
        QPoint(20, 40),
        QPoint(0,40),
    };

    if(option->state&QStyle::State_Selected)
    {
        mPen.setColor(QColor("#00FFFF"));
        mPen.setWidthF(1);
        painter->setPen(mPen);
    } else {
        mPen.setColor(Qt::black);
        mPen.setWidthF(0.8);
        painter->setPen(mPen);
    }

    painter->setBrush(QColor("#FF6347"));
    painter->drawConvexPolygon(hourHand, 5);
    //绘制鼠标在图元上方漂浮时图元四个角的小方框
    if(f_hoverEnter)
    {
        painter->setBrush(QColor(180,180,180,180));
        painter->setPen(QPen(Qt::darkGray,0));
        painter->drawRect(rect.x()-1,rect.y()-1,2,2);
        painter->drawRect(rect.x()+rect.width()-1,rect.y()-1,2,2);
        painter->drawRect(rect.x()-1,rect.y()+rect.height()-1,2,2);
        painter->drawRect(rect.x()+rect.width()-1,rect.y()+rect.height()-1,2,2);
    }
    painter->setPen(QPen(Qt::black, 0.8));
    painter->setFont(font);
    painter->drawText(rect.adjusted(0,0,-10,0), Qt::AlignCenter, "In");
}

void InModule::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    f_hoverEnter = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void InModule::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    f_hoverEnter = false;
    QGraphicsItem::hoverLeaveEvent(event);
}

void InModule::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    //获取光标位置
    QPoint p = event->pos().toPoint();
    //根据光标位置设置光标形状
    if(QRect(rect.x()+rect.width()-p_cursorSet,rect.y()+rect.height()-p_cursorSet,p_cursorSet<<1,p_cursorSet<<1).contains(p)) {
        setCursor(Qt::SizeFDiagCursor);
        f_changeRec = true;
    } else {
        setCursor(Qt::ArrowCursor);
        f_changeRec = false;
    }
    QGraphicsItem::hoverMoveEvent(event);
}

QVariant InModule::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        QPointF oldPos = pos();
        QPointF newPos = value.toPointF();
        QPointF distance = newPos - oldPos;

        if(qAbs(distance.x()) >= ItemBase::microMove || qAbs(distance.y()) >= ItemBase::microMove)
        {
            return QPointF(oldPos.x() + qRound(distance.x()/ItemBase::microMove)*ItemBase::microMove,
                           oldPos.y() + qRound(distance.y()/ItemBase::microMove)*ItemBase::microMove);
        }

        return oldPos;
    }

    if(change == QGraphicsItem::ItemPositionHasChanged)
    {
        arrowOut->upDateBeginPoint(mapToScene(rect.right(), rect.height()/2));
    }

    return QGraphicsItem::itemChange(change, value);
}

InModule::~InModule()
{
    delete arrowOut;
}
