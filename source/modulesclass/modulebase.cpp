#include "modulebase.h"
#include <QSpinBox>

ModuleBase::ModuleBase(QGraphicsItem *parent, QRect rect)
        :QGraphicsWidget(parent, Qt::Widget)
{
    setFlags(QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    resize(rect.size());

    this->rect = rect;              //初始化矩形图元
    this->f_hoverEnter = false;     //鼠标是否进入边界矩形标志
    this->f_changeRec = false;      //是否改变边界矩形标志
    this->p_cursorSet = 5;

    mlinear.setStart(rect.topLeft());
    mlinear.setFinalStop(rect.bottomRight());
    mlinear.setColorAt(0.0, Qt::white);
    mlinear.setColorAt(1.0, QColor(230,230,230));

    mPen.setCapStyle(Qt::SquareCap);
    mPen.setJoinStyle(Qt::MiterJoin);
    mPen.setStyle(Qt::SolidLine);
}

QRectF ModuleBase::boundingRect()const
{
    qreal adjust = 2;
    return QRectF(rect.adjusted(-adjust,-adjust,adjust,adjust));
}

int ModuleBase::type() const
{
    return ItemBase::MoudleType;
}

void ModuleBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *moduleWidget)
{
    Q_UNUSED(moduleWidget);

    if(option->state&QStyle::State_Selected)
    {
        mPen.setColor(QColor("#00FFFF"));
        mPen.setWidthF(1);
        painter->setPen(mPen);
    } else
    {
        mPen.setColor(Qt::black);
        mPen.setWidthF(0.8);
        painter->setPen(mPen);
    }

    painter->setBrush(mlinear);
    painter->drawRoundedRect(rect, 6, 6);
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
}

void ModuleBase::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!isSelected())
    {
        scene()->clearSelection();
        setSelected(true);
    }

    QGraphicsItem::mousePressEvent(event);
}

void ModuleBase::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void ModuleBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF itemCoordinate = event->pos();

    if(f_changeRec)
    {      
        prepareGeometryChange();//如果要更改项目的边界矩形，必须先调用*
        if(itemCoordinate.x() < (rect.x()+p_cursorSet))
            itemCoordinate.setX(rect.x()+p_cursorSet);
        if(itemCoordinate.y() < (rect.y()+p_cursorSet))
            itemCoordinate.setY(rect.y()+p_cursorSet);

        rect.setBottomRight(itemCoordinate);
        resize(rect.size());
        updataArrowPosition();
    }else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ModuleBase::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    f_hoverEnter = true;
    QGraphicsItem::hoverEnterEvent(event);
}

void ModuleBase::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    //获取光标位置
//    QPoint p = event->pos().toPoint();
//    //根据光标位置设置光标形状
//    if(QRect(rect.x()+rect.width()-p_cursorSet,rect.y()+rect.height()-p_cursorSet,p_cursorSet<<1,p_cursorSet<<1).contains(p)) {
//        setCursor(Qt::SizeFDiagCursor);
//        f_changeRec = true;
//    } else {
//        setCursor(Qt::ArrowCursor);
//        f_changeRec = false;
//    }
    QGraphicsItem::hoverMoveEvent(event);
}

void ModuleBase::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    f_hoverEnter = false;
    unsetCursor();
    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant ModuleBase::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
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
        int count = listArrowOut.size();
        p = updata_p(count);
        for(int i=0; i<count; i++)
        {
            listArrowOut[i]->upDateBeginPoint(mapToScene(rect.right(), p.begin+i*p.space));
        }

        count = listArrowIn.size();
        for(int i=0; i<count; i++)
        {
            if(!listArrowIn[i]->isConnect)
                continue;
            listArrowIn[i]->connectArrow->upDateEndPoint(listArrowIn[i]->scenePos());
        }
    }

    if(change == QGraphicsItem::ItemSelectedHasChanged)
    {
        if(value.toBool())
        {
            setZValue(255);
        }else
        {
            setZValue(0);
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

void ModuleBase::setModuleName(QString str)
{
    moduleName = str;
}

void ModuleBase::initInterface(int innum, int outnum)
{
    if(innum < 1 || outnum < 1)
        return;

    p = updata_p(innum);
    for(int i =0; i<innum; i++)
    {
        ArrowIn *in = new ArrowIn();
        in->setParentItem(this);
        in->setPos(0,p.begin+i*p.space);
        listArrowIn.append(in);
    }

    p = updata_p(outnum);
    for(int i=0; i<outnum; i++)
    {
        ArrowOut *out = new ArrowOut(this);
        out->upDateBeginPoint(mapToScene(rect.right(), p.begin+i*p.space));
        listArrowOut.append(out);

        MGraphicsView *mview = static_cast<MGraphicsView*>(this->scene()->views().at(0));
        connect(mview,SIGNAL(signal_selectionAreaChange()),out,SLOT(slot_selectionAreaChange()));
        connect(this->scene(),SIGNAL(signal_sceneMove(QPointF)),out,SLOT(slot_sceneMove(QPointF)));
    }
}

void ModuleBase::updataArrowPosition()
{
    int count = listArrowIn.size();
    p = updata_p(count);
    for(int i=0; i<count; i++)
    {
        if(!listArrowIn[i]->isConnect)
        {
            listArrowIn[i]->setPos(0,p.begin+i*p.space);
            continue;
        }
        listArrowIn[i]->connectArrow->upDateEndPoint(listArrowIn[i]->scenePos());
    }

    count = listArrowOut.size();
    p = updata_p(count);
    for(int i=0; i<count; i++)
    {
        listArrowOut[i]->upDateBeginPoint(mapToScene(rect.right(), p.begin+i*p.space));
    }
}

_p ModuleBase::updata_p(int num)
{
    _p p;

    if(num == 1)
    {
        p.begin = rect.height()/2;
        return p;
    }

    int temp = rect.height()/10;
    int begin = temp /(num+1);
    int space = (temp - 2*begin)/(num - 1);

    p.begin = begin*10;
    p.space = space*10;

    return p;
}

ModuleBase::~ModuleBase()
{
    int size = listArrowIn.size();
    for(int i = 0; i<size; i++)
    {
        delete listArrowIn.takeLast();
    }

    size = listArrowOut.size();
    for(int i = 0; i<size; i++)
    {
        delete listArrowOut.takeLast();
    }
}
