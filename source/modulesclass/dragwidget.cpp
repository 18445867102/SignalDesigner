#include "dragwidget.h"

#define cout  qDebug() << __LINE__ << ":"

DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent)
{
    setAcceptDrops(true);
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->setDropAction(Qt::MoveAction);
    event->accept();
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if(event->source() == this)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void DragWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;

    MLabel *child = static_cast<MLabel*>(childAt(dragStartPosition));
    if (!child || child->moduleNum == 0)
        return;

    //图形
    QPixmap pixmap = *child->pixmap();
    //数据
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream<<QPoint(event->pos() - child->pos())<<child->moduleName<<child->moduleType<<child->moduledata;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());


    //在pixmap上画矩形，实现变暗效果
//    QPixmap tempPixmap = pixmap;
//    QPainter painter;
//    painter.begin(&tempPixmap);
//    QBrush brush(QColor(127, 127, 127,100), Qt::FDiagPattern);
//    painter.setBrush(brush);
//    painter.drawRect(pixmap.rect());
//    painter.end();
//    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
    {
        child->setPixmap(pixmap);
        child->show();
    } else {
        if(child->moduleType == ItemBase::InModuleType || child->moduleType == ItemBase::OutModuleType)
        {
            return;
        }

        //重绘QPixMap 更改模块数量
        QImage image(90, 50, QImage::Format_ARGB32_Premultiplied);
        image.fill(qRgba(0, 0, 0,0));
        QFont font("Times New Roman");
        font.setStyleStrategy(QFont::ForceOutline);
        QPen pen(Qt::black);
        pen.setWidthF(2.4);
        pen.setJoinStyle(Qt::RoundJoin);
        QLinearGradient linear(image.rect().topLeft(),image.rect().bottomRight());
        linear.setColorAt(0.0, Qt::white);
        linear.setColorAt(1.0, QColor(230,230,230));

        QPainter painter;
        painter.begin(&image);
        painter.setRenderHint(QPainter::Antialiasing);//qRgb(164, 213, 255)
        painter.setPen(pen);
        if(--child->moduleNum == 0){
            painter.setBrush(QBrush(QColor(127, 127, 127, 80), Qt::SolidPattern));
        }else{
            painter.setBrush(linear);
        }
        painter.drawRect(image.rect());
        painter.setFont(font);
        painter.drawText(image.rect(), Qt::AlignCenter, child->moduleName);
        font.setPointSize(6);
        painter.setFont(font);
        painter.drawText(image.rect().adjusted(0,0,-4,-2), Qt::AlignBottom|Qt::AlignRight, QString::number(child->moduleNum));
        painter.end();

        child->setPixmap(QPixmap::fromImage(image));
        child->show();
    }

    QWidget::mouseMoveEvent(event);
}


void DragWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QWidget::mousePressEvent(event);
}
