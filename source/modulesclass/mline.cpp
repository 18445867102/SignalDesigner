#include "mline.h"

#define cout  qDebug() << __LINE__ << ":"
#define DefaultLineWidth    12

Mline::Mline(QObject *parent): QObject(parent), QGraphicsLineItem()
{
    setFlags(QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemSendsScenePositionChanges);
    setZValue(1);
    setAcceptHoverEvents(true);

    isConnect = false;
    moveFlag = false;

    lineItemPen.setWidth(DefaultLineWidth);
    setPen(lineItemPen);

    mPenDisconnnect.setColor(Qt::red);
    mPenDisconnnect.setWidthF(0.5);
    mPenDisconnnect.setStyle(Qt::DashLine);
    mPenDisconnnect.setJoinStyle(Qt::RoundJoin);
    mPenDisconnnect.setCapStyle(Qt::RoundCap);

    mPenConnect.setColor(Qt::black);
    mPenConnect.setWidthF(0.5);
    mPenConnect.setStyle(Qt::SolidLine);
    mPenConnect.setJoinStyle(Qt::RoundJoin);
    mPenConnect.setCapStyle(Qt::RoundCap);

    mPenSelected.setColor(QColor("#00FFFF"));
    mPenSelected.setWidthF(0.8);
    mPenSelected.setStyle(Qt::SolidLine);
    mPenSelected.setJoinStyle(Qt::RoundJoin);
    mPenSelected.setCapStyle(Qt::RoundCap);
}

void Mline::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    qreal scaleFactor =  painter->matrix().m11();
    lineItemPen.setWidthF(DefaultLineWidth / scaleFactor);
    setPen(lineItemPen);

   // width = DeaultLineWidth/painter->matrix().m11();

    if(isConnect)
    {
        if(option->state & QStyle::State_Selected)
        {
            painter->setPen(mPenSelected);
        } else
        {
            painter->setPen(mPenConnect);
        }
    } else {
        painter->setPen(mPenDisconnnect);
    }

    /* 享受每个开心时刻！Sally's Dance
    cout<<mapToScene(QPoint(0,0)); */

    painter->drawLine(line());
}

int Mline::type() const
{
    return ItemBase::LineType;
}

//QPainterPath Mline::shape() const
//{
//    QPainterPath path(line().p1());
//    path.lineTo(line().p2());
//    QPainterPathStroker stroker;
//    stroker.setWidth(width);
//    path = stroker.createStroke(path);

//    return path;
//}

void Mline::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(hasFocus())
    {
        if(qAbs(line().dy()) > qAbs(line().dx()))
        {
            setCursor(Qt::SizeHorCursor);
        }else
        {
            setCursor(Qt::SizeVerCursor);
        }
    }else
    {
        setCursor(Qt::ArrowCursor);
    }

    QGraphicsLineItem::hoverMoveEvent(event);
}

QVariant Mline::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        QPointF oldPos = pos();
        QPointF newPos = value.toPointF();
        QPointF distance = newPos - oldPos;

        if(!moveFlag){
            return pos();
        }

        if(qAbs(distance.x()) >= ItemBase::microMove || qAbs(distance.y()) >= ItemBase::microMove)
        {
            return QPointF(oldPos.x() + qRound(distance.x()/ItemBase::microMove)*ItemBase::microMove,
                           oldPos.y() + qRound(distance.y()/ItemBase::microMove)*ItemBase::microMove);
        }

        return oldPos;
    }

    return QGraphicsLineItem::itemChange(change, value);
}

int Mline::lineDirect()
{
    int direct;

    if(line().isNull())
        return LineDirect::Error;

    if(line().x1() == line().x2()){
        if(line().y1() > line().y2())
            direct = LineDirect::Up;
        else
            direct = LineDirect::Down;
    }else if(line().y1() == line().y2()){
        if(line().x1() > line().x2())
            direct = LineDirect::Left;
        else
            direct = LineDirect::Right;
    }else{
        direct = LineDirect::Error;
    }

    return direct;
}

int Mline::lineType()
{
    int type = LineType::NoType;

    if(line().x1() == line().x2()){
        type = LineType::Vertical;
    }else if(line().y1() == line().y2()){
        type = LineType::Horizontal;
    }

    return type;
}

void Mline::updateStatus(bool status)
{
    isConnect = status;
    update();
}

void Mline::updataMoveFlag(bool flag)
{
    moveFlag = flag;
}

Mline::~Mline()
{

}
