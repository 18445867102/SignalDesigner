#include "mstatusbar.h"

#define cout  qDebug() << __LINE__ << ":"

MStatusBar::MStatusBar(QWidget *parent) : QStatusBar(parent)
{
   // setSizeGripEnabled(false);
    level = 1.0;
}

void MStatusBar::paintEvent(QPaintEvent *event)
{
    QFont font("华文彩云",12);

    QPainter painter(this);
    painter.setBrush(QColor("Gainsboro"));
    painter.drawRect(rect().adjusted(-1,-1,0,0));
    painter.setFont(font);
    painter.drawText(QRect(QPoint(0,0),size()), Qt::AlignCenter, QString::number(level*100)+'%');

    QStatusBar::paintEvent(event);
}

void MStatusBar::slot_updataZoomLevel(qreal level)
{
    this->level = level;
    update();
}
