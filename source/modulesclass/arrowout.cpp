#include "arrowout.h"

ArrowOut::ArrowOut(QGraphicsItem *parentItem):
    ItemBase(parentItem),parentItem(parentItem)
{
    setFlags(QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsFocusable|
             QGraphicsItem::ItemSendsScenePositionChanges);
    setAcceptHoverEvents(true);
    setParentItem(parentItem);
    setZValue(2);

    this->arrowState = ArrowState::Default;
    this->f_movePoint = false;
    this->f_isGrabMouse = false;


    Pen1.setColor(Qt::black);//画笔初始化
    Pen1.setWidthF(1.8);
    Pen2.setColor(Qt::red);
    Pen2.setWidthF(1.0);
    Pen2.setStyle(Qt::DashLine);
    Pen3.setColor(Qt::black);
    Pen3.setWidthF(0.1);
    Pen3.setStyle(Qt::SolidLine);
    Pen3.setJoinStyle(Qt::MiterJoin);
    Pen3.setCapStyle(Qt::FlatCap);

    arrowPath.moveTo(QPointF(-8, -4));//箭头路径
    arrowPath.lineTo(QPointF(0,0));
    arrowPath.lineTo(QPointF(-8,4));
    arrowPath.arcTo(QRectF(-10.6, -4.5, 3.5, 9), -62, 124);

    seletedArrowPath.moveTo(QPointF(-10, -5));
    seletedArrowPath.lineTo(QPointF(0,0));
    seletedArrowPath.lineTo(QPointF(-10,5));
    seletedArrowPath.arcTo(QRectF(-12.7, -5.5,3.7, 11), -60, 120);
}

QRectF ArrowOut::boundingRect() const
{
    return QRectF(-13,-6,19,12);
}

int ArrowOut::type() const
{
    return ItemBase::ConnectOutType;
}

void ArrowOut::updateStatus()
{   
    if(arrowState == ArrowState::Connect)
    {
        connectArrow->show();
        connectArrow->isConnect = false;
    }
}

void ArrowOut::updateDefault()
{
    if(arrowState == ArrowState::Connect)
    {
        connectArrow->show();
        connectArrow->isConnect = false;
    }

    arrowState = ArrowOut::Default;
    f_movePoint = false;

    int count = linelist.size();
    for(int i =0; i<count; i++)
    {
        delete linelist.takeFirst();
    }

    QPointF point = pointList.first();
    pointList.clear();
    pointList.append(point);
    pointList.append(point + QPoint(5,0));

    //设置箭头坐标(末尾点坐标)
    setPos(pointList.last());
}

void ArrowOut::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if(arrowState == ArrowState::Default)
    {
        painter->setPen(Pen1);
        painter->drawLine(-5,5,0,0);
        painter->drawLine(-5,-5,0,0);
    }
    else if(arrowState == ArrowState::Suspend ||arrowState == ArrowState::Move)
    {
        painter->setPen(Pen2);
        if(!linelist.empty())
        {
            switch (linelist.last()->lineDirect()){
            case Mline::Up:
                painter->drawLine(-5,5,0,0);
                painter->drawLine(5,5,0,0);
                break;
            case Mline::Down:
                painter->drawLine(-5,-5,0,0);
                painter->drawLine(5,-5,0,0);
                break;
            case Mline::Left:
                painter->drawLine(5,5,0,0);
                painter->drawLine(5,-5,0,0);
                break;
            case Mline::Right:
                painter->drawLine(-5,5,0,0);
                painter->drawLine(-5,-5,0,0);
                break;
            case Mline::Error:
                break;
            }
        }else{
            painter->drawLine(-5,5,0,0);
            painter->drawLine(-5,-5,0,0);
        }
    }
    else if(arrowState == ArrowState::Connect)
    {
        if(option->state&QStyle::State_Selected)
        {
            Pen3.setColor(QColor("#00FFFF"));
            painter->setBrush(QColor("#00FFFF"));
            painter->setPen(Pen3);
            painter->drawPath(seletedArrowPath);
        }else{
            Pen3.setColor(Qt::black);
            painter->setBrush(Qt::black);
            painter->setPen(Pen3);
            painter->drawPath(arrowPath);
        }
    }
}

void ArrowOut::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::CrossCursor);
    setZValue(255);
    QGraphicsItem::hoverEnterEvent(event);
}

void ArrowOut::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setZValue(2);
    QGraphicsItem::hoverLeaveEvent(event);
}

bool ArrowOut::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    Mline *line = static_cast<Mline*>(watched);

    if(event->type() == QEvent::GraphicsSceneMousePress)
    {
        scene()->clearSelection();
        for(Mline* line: linelist)
        {
            line->setSelected(true);
        }
        setSelected(true);

        return true;//拦截
    }

    if(event->type() == QEvent::GraphicsSceneMouseMove)
    {
        if(!line->hasFocus())
            return true;
        if(linelist.first() == line || linelist.last() == line)
            return true;

        /*LUCKY MOMENT! Booty Swing.*/
        QPointF point = static_cast<QGraphicsSceneMouseEvent*>(event)->scenePos();
        int index = linelist.indexOf(line);
        int direct = line->lineType();

        if(direct == Mline::Horizontal)
        {
            pointList.replace(index,QPointF(pointList[index].x(), point.y()));
            pointList.replace(index+1,QPointF(pointList[index+1].x(),point.y()));
            if(linelist[index]->lineDirect() == linelist[index-2]->lineDirect())
            {
                if(linelist[index-1]->line().length()<10)
                {
                    pointList.replace(index+1,QPointF(pointList[index+1].x(),pointList[index-1].y()));
                    pointList.removeAt(index-1);
                    pointList.removeAt(index-1);
                    line->clearFocus();
                }
            }

            if(linelist[index]->lineDirect() == linelist[index+2]->lineDirect())
            {
                if(linelist[index+1]->line().length()<10)
                {
                    pointList.replace(index,QPointF(pointList[index].x(),pointList[index+2].y()));
                    pointList.removeAt(index+1);
                    pointList.removeAt(index+1);
                    line->clearFocus();
                }
            }
        }
        else//垂线
        {
            pointList.replace(index,QPointF(point.x(), pointList[index].y()));
            pointList.replace(index+1,QPointF(point.x(),pointList[index+1].y()));

            if(linelist[1] != line)
            {
                if(linelist[index]->lineDirect() == linelist[index-2]->lineDirect())
                {
                    if(linelist[index-1]->line().length()<10)
                    {
                        pointList.replace(index+1,QPointF(pointList[index-1].x(),pointList[index+1].y()));
                        pointList.removeAt(index-1);
                        pointList.removeAt(index-1);
                        line->clearFocus();
                    }
                }
            }

            if(linelist[linelist.size()-2] != line)
            {
                if(linelist[index]->lineDirect() == linelist[index+2]->lineDirect())
                {
                    if(linelist[index+1]->line().length()<10)
                    {
                        pointList.replace(index,QPointF(pointList[index+2].x(),pointList[index].y()));
                        pointList.removeAt(index+1);
                        pointList.removeAt(index+1);
                        line->clearFocus();
                    }
                }
            }
        }

        updateLine();

        return true;//拦截
    }

    if(event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        return true;//拦截
    }

    return false;
}

void ArrowOut::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //{定拐点，定拐点，定拐点，....,移动拐点(可能有)，动点}
    pressPoint = event->scenePos();

    if(event->button() == Qt::LeftButton)
    {
        if(arrowState == ArrowState::Default)
        {
            setParentItem(nullptr);
            setPos(pointList.last());
            arrowState = ArrowState::Move;
        }
        else if(arrowState == ArrowState::Suspend)
        {
            arrowState = ArrowState::Move;
        }
        else if(arrowState == ArrowState::Move)
        {
            arrowState = ArrowState::Suspend;            
        }
        else if(arrowState == ArrowState::Connect)
        {
            for(Mline* line: linelist)
                line->setSelected(true);
            setSelected(true);
        }
    }else if(event->button() == Qt::RightButton)//鼠标点击右键添加定拐点
    {
        if(!f_movePoint)
        {
            pointList.insert(pointList.size()-1, this->pos());//没有移动拐点，将箭头坐标作为定拐点添加到列表中
        }else
        {
            f_movePoint = false;//将移动拐点变为定拐点
        }
    }

    //QGraphicsItem::mousePressEvent(event);
}

void ArrowOut::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{  
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->scenePos() - pressPoint).manhattanLength() < 8)
        return;

    if(arrowState == ArrowState::Connect)
    {
        arrowState = ArrowState::Move;
        connectArrow->updataStatus(false);
        updateLine();
    }

    //QGraphicsItem::mouseMoveEvent(event);
}

void ArrowOut::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
}

void ArrowOut::slot_sceneMove(QPointF point)
{
    if(arrowState != ArrowState::Move)
        return;

    QList<QGraphicsItem *> list = collidingItems();
    for(QGraphicsItem *item :list)
    {
        if(item->type() == ItemBase::ConnectInType && item->contains(item->mapFromScene(point)))
        {
            connectArrow = static_cast<ArrowIn*>(item);
            connectArrow->updataStatus(true);
            connectArrow->connectArrow = this;

            //--------------------------------------------------/
            point = item->scenePos();
            if(linelist.last()->lineType() == Mline::Vertical)
            {
                pointList.replace(pointList.size()-2, QPointF(point.x()-10, pointList[pointList.size()-2].y()));
                pointList.insert(pointList.size()-1, QPointF(point.x()-10,point.y()));
            }
            setPos(point);
            grabMouse();
            ungrabMouse();
            scene()->clearSelection();
            //-------------------------------------------------/

            arrowState = ArrowState::Connect;
            break;
        }
    }

    if(!f_movePoint)//没有动节点
    {
        if(pointList.count()%2 == 0)//更新动点(末尾点)
        {
            pointList.replace(pointList.count()-1, QPointF(point.x(),pointList.last().y()));
        }else
        {
            pointList.replace(pointList.count()-1, QPointF(pointList.last().x(),point.y()));
        }
    }else
    {
        pointList.replace(pointList.count()-1, point);
    }

    if(!f_movePoint)//没有动节点
    {
        if(pointList.count()%2 == 0)//末尾线水平状态
        {
            if(qAbs(point.y()-pointList.at(pointList.size()-2).y())>5)
            {
                pointList.insert(pointList.size()-1, QPointF(point.x(),pointList.at(pointList.size()-2).y()));
                f_movePoint = true;
            }
        }else//垂线
        {
            if(qAbs(point.x()-pointList.at(pointList.size()-2).x())>5)
            {
                pointList.insert(pointList.size()-1,QPointF(pointList.at(pointList.size()-2).x(), point.y()));
                f_movePoint = true;
            }
        }
    }
    else//有动节点
    {
        if(pointList.count()%2 == 0)//线尾水平状态
        {
            if(qAbs(point.x()-pointList.at(pointList.size()-2).x())<5)
            {
                pointList.removeAt(pointList.count()-2);//删除移动拐点
                f_movePoint = false;
                pointList.replace(pointList.count()-1, QPointF(pointList.at(pointList.size()-2).x(),point.y()));
            }else{
                //更新移动节点
                pointList.replace(pointList.count()-2, QPointF(pointList.at(pointList.size()-2).x(),point.y()));
            }
        }else
        {
            if(qAbs(point.y()-pointList.at(pointList.size()-2).y())<5)
            {
                pointList.removeAt(pointList.count()-2);
                f_movePoint = false;
                pointList.replace(pointList.count()-1, QPointF(point.x(),pointList.at(pointList.size()-2).y()));
            }else {
                pointList.replace(pointList.count()-2, QPointF(point.x(), pointList.at(pointList.size()-2).y()));
            }
        }
    }

    if(pointList[1].x()-pointList[0].x() < 10)
        return;

    setPos(pointList.last());//设置箭头坐标(末尾点坐标)
    updateLine();
}

void ArrowOut::updateLine()
{
    if(pointList.size() < 2)
    {
        cout<<"size error.";
        return;
    }

    //调整连接线列表
    if(linelist.size() < (pointList.size() - 1))
    {
        int count = pointList.size()-linelist.size()-1;
        for(int i=0; i<count; i++)
        {
            Mline * line = new Mline;
            scene()->addItem(line);
            linelist.append(line);
            line->installSceneEventFilter(this);
        }
    }else if(linelist.size() > (pointList.size() - 1))
    {
        int count = linelist.size()- pointList.size()+1;
        for(int i=0; i<count; i++)
        {
            scene()->removeItem(linelist.takeLast());
        }
    }


    //通过拐点画线
    if(pointList.size() > 1)
    {
        int count = pointList.size()-1;
        for(int i = 0; i<count; i++)
        {
            QPointF p1 = linelist[i]->mapFromScene(pointList[i]);
            QPointF p2 = linelist[i]->mapFromScene(pointList[i+1]);
            linelist[i]->setLine(p1.x(),p1.y(),p2.x(),p2.y());
        }
    }

    //更新连线颜色
    for(Mline* line: linelist)
    {
        if(arrowState == ArrowState::Connect)
        {
            line->updateStatus(true);
        }else
        {
            line->updateStatus(false);
        }
    }

    update();
}

QVariant ArrowOut::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{

    if(change == QGraphicsItem::ItemPositionChange && arrowState == ArrowState::Connect)
    {
        QPointF oldPos = pos();
        QPointF newPos = value.toPointF();
        QPointF distance = newPos - oldPos;

        if(parentItem->isSelected() && !connectArrow->parentItem()->isSelected())
        {
            return oldPos;
        }


        if(qAbs(distance.x()) >= microMove || qAbs(distance.y()) >= microMove)
        {
            int dx = qRound(distance.x()/microMove)*microMove;
            int dy = qRound(distance.y()/microMove)*microMove;

            if(parentItem->isSelected() && connectArrow->parentItem()->isSelected())
            {
                int count = pointList.size();
                for(int i = 0; i < count; i++)
                {
                    pointList.replace(i,pointList[i]+QPointF(dx,dy));
                }
            }

            return QPointF(oldPos.x() + dx, oldPos.y() + dy);
        }

        return oldPos;
    }

    return QGraphicsItem::itemChange(change, value);
}

void ArrowOut::slot_selectionAreaChange()
{
    bool flag = false;
    if(arrowState == ArrowState::Connect)
    {
        if(parentItem->isSelected() && connectArrow->parentItem()->isSelected())
            flag = true;
    }
    for(Mline* line : linelist)
    {
        if(line->isSelected())
        {
            for(Mline* line: linelist)
            {
                line->setSelected(true);
                line->updataMoveFlag(flag);
            }
            setSelected(true);
            break;
        }
    }
}

void ArrowOut::upDateBeginPoint(QPointF beginPoint)
{
    qreal middle;
    if(arrowState == ArrowState::Connect)
    {
        //起始模块和终止模块都处于选择状态
        if(connectArrow->parentItem()->isSelected())
            return;

        pointList.clear();

        if(beginPoint.y() == pos().y())
        {
            pointList.append(beginPoint);
            pointList.append(pos());
        }else if(pos().x() >= beginPoint.x()+20)
        {
            middle = (pos().x() + beginPoint.x())/2;
            pointList.append(beginPoint);
            pointList.append(QPointF(middle,beginPoint.y()));
            pointList.append(QPointF(middle,pos().y()));
            pointList.append(pos());
        }else
        {
            middle = (beginPoint.y() + pos().y())/2;
            pointList.append(beginPoint);
            pointList.append(QPointF(beginPoint.x()+10,beginPoint.y()));
            pointList.append(QPointF(beginPoint.x()+10,middle));
            pointList.append(QPointF(pos().x()-10,middle));
            pointList.append(QPointF(pos().x()-10,pos().y()));
            pointList.append(pos());
            /* LUCKY MOMENT! */
        }

        updateLine();
    }else
    {
        setParentItem(parentItem);
        setPos(beginPoint - parentItem->pos()+QPoint(5,0));
        f_movePoint = false;
        arrowState = ArrowOut::Default;

        int count = linelist.size();
        for(int i = 0; i<count; i++)
        {
            delete linelist.takeFirst();
        }

        pointList.clear();
        pointList.append(beginPoint);
        pointList.append(beginPoint + QPoint(5,0));
    }
}

void ArrowOut::upDateEndPoint(QPointF endPoint)
{
    qreal middle;
    if(arrowState == ArrowState::Connect)
    {
        if(parentItem->isSelected())
            return;

        QPointF beginPoint = pointList.first();
        pointList.clear();

        if(beginPoint.y() == endPoint.y())
        {
            pointList.append(beginPoint);
            pointList.append(endPoint);
        }else if(endPoint.x() >= beginPoint.x()+20)
        {
            middle = (endPoint.x() + beginPoint.x())/2;
            pointList.append(beginPoint);
            pointList.append(QPointF(middle,beginPoint.y()));
            pointList.append(QPointF(middle,endPoint.y()));
            pointList.append(endPoint);
        }else
        {
            middle = (beginPoint.y() + endPoint.y())/2;
            pointList.append(beginPoint);
            pointList.append(QPointF(beginPoint.x()+10,beginPoint.y()));
            pointList.append(QPointF(beginPoint.x()+10,middle));
            pointList.append(QPointF(endPoint.x()-10,middle));
            pointList.append(QPointF(endPoint.x()-10,endPoint.y()));
            pointList.append(endPoint);
        }

        setPos(endPoint);
        updateLine();
    }
}

ArrowOut::~ArrowOut()
{
    int count = linelist.size();
    for(int i =0; i<count; i++)
    {
        delete linelist.takeFirst();
    }
}
