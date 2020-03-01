#include "mtablewidget.h"

#define cout  qDebug() << __LINE__ << ":"

MTableWidget::MTableWidget(QWidget *parent) : QTableWidget(parent)
{
    //setDragEnabled(true);
    //setAcceptDrops(true);
    //setDragDropMode(QAbstractItemView::InternalMove);
}
/*
void MTableWidget::dragEnterEvent(QDragEnterEvent *event){
    cout<<"dragEnter";
    event->accept();
}
void MTableWidget::dragLeaveEvent(QDragLeaveEvent *event){
    cout<<"dragLeave";
}
void MTableWidget::dropEvent(QDropEvent *event){
    cout<<"dropevent";
}

void MTableWidget::dragMoveEvent(QDragMoveEvent *event){
    cout<<"dragMove";
}
void MTableWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QTableWidget::mousePressEvent(event);
}

void MTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    //cout<<"mouseMove";
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    //this->itemAt()
    QString str = item(currentRow(),0)->text();
    dataStream <<str;

    QFontMetrics metric(this->font());
    QSize size = metric.size(Qt::TextSingleLine, str);

    QImage image(size.width() + 4, size.height() + 4, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0,0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

//    QLinearGradient gradient(0, 0, 0, image.height()-1);
//    gradient.setColorAt(0.0, Qt::white);
//    gradient.setColorAt(0.2, QColor(200, 200, 255));
//    gradient.setColorAt(0.8, QColor(200, 200, 255));
//    gradient.setColorAt(1.0, QColor(127, 127, 200));

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(QRectF(0.5, 0.5, image.width()-1, image.height()-1),qRgb(164, 213, 255));
    painter.setFont(font);
    painter.setPen(Qt::black);
    painter.drawText(QRect(QPoint(0, 0), image.size()), Qt::AlignCenter, str);
    painter.end();

    mimeData->setData("text",itemData);
    drag->setMimeData(mimeData);
    drag->setPixmap(QPixmap::fromImage(image));
    //drag->setHotSpot(event->pos() - child->pos());
    drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

    QTableWidget::mouseMoveEvent(event);
}
void MTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QTableWidget::mouseReleaseEvent(event);
}
*/
