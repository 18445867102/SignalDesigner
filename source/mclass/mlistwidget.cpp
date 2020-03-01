#include "mlistwidget.h"

#define cout  qDebug() << __LINE__ << ":"

MListWidget::MListWidget(QWidget *parent) : QListWidget(parent)
{
    setDragEnabled(true);
    setAcceptDrops(true);
}

//void MListWidget::dragEnterEvent(QDragEnterEvent *event){
//    //cout<<"List_dragEnter";
//    if (event->mimeData()->hasFormat("text"))
//    {
//        event->setDropAction(Qt::MoveAction);
//        event->accept();
//    } else {
//        event->ignore();
//    }
//    QListWidget::dragEnterEvent(event);
//}
//void MListWidget::dragLeaveEvent(QDragLeaveEvent *event){
//    cout<<"List_dragLeave";
//    event->accept();
//    //QListWidget::dragLeaveEvent(event);
//}
void MListWidget::dropEvent(QDropEvent *event){
    //cout<<"List_dropevent";

    if(event->source() != this)
    {
        if (event->mimeData()->hasFormat("text")) {

            event->setDropAction(Qt::MoveAction);

            QByteArray itemData = event->mimeData()->data("text");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);

            QString str;
            dataStream >>str;
            //this->addItem(str);
            emit signal_itemRowChange(str);

            event->accept();
        }
    } else {
        event->ignore();
    }
}

void MListWidget::keyPressEvent(QKeyEvent *event)
{
    if(objectName() == "AllowDelete")
    {
//        if(event->key() == Qt::Key_Delete)
//        {
//            if(!selectedItems().isEmpty()){
//                foreach(QListWidgetItem * item,selectedItems()){
//                    removeItemWidget(item);
//                }

//            }
//        }
    }
    QListWidget::keyPressEvent(event);
}

//void MListWidget::dragMoveEvent(QDragMoveEvent *event)
//{
//    //cout<<"List_dragMove";
//    QListWidget::dragMoveEvent(event);
//}

void MListWidget::mousePressEvent(QMouseEvent *event)
{
    //cout<<"List_mousePress";
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QListWidget::mousePressEvent(event);
}

void MListWidget::mouseMoveEvent(QMouseEvent *event)
{
    //cout<<"List_mouseMove";
    //cout<<"mouseMove";
    if(itemAt(event->pos()) == nullptr)
        return;
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);

    QString str = itemAt(event->pos())->text();
    dataStream <<str;

    QFontMetrics metric(this->font());
    QSize size = metric.size(Qt::TextSingleLine, str);

    QImage image(size.width() + 4, size.height() + 4, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0,0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),qRgb(164, 213, 255));
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(QRect(QPoint(0, 0), image.size()), Qt::AlignCenter, str);
    painter.end();

    mimeData->setData("text", itemData);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap::fromImage(image));
    //drag->setHotSpot(event->pos() - child->pos());
    drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    QListWidget::mouseMoveEvent(event);
}

void MListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QListWidget::mouseReleaseEvent(event);
}
