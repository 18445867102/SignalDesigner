#include "mtoolbar.h"

#define cout  qDebug() << __LINE__ << ":"

MToolBar::MToolBar(QWidget *parent) : QToolBar(parent)
{

}

void MToolBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFont font("华文彩云",20);
    QPen pen(Qt::white);

    QLinearGradient mlinear;
    mlinear.setStart(QPoint(size().width()/2,0));
    mlinear.setFinalStop(size().width(),size().height());
    mlinear.setColorAt(0.0, Qt::white);
    mlinear.setColorAt(0.3, QColor(250,250,250));
    mlinear.setColorAt(1.0, QColor(120,120,120));

    painter.setBrush(mlinear);
    painter.drawRect(QRect(-1, 0, size().width()+1, size().height()));
    painter.setFont(font);
    painter.setPen(pen);
    painter.drawText(QRect(0,0,size().width()-20,size().height()), Qt::AlignRight|Qt::AlignVCenter, "Module Controller");
    /* LUCKY MOMENT! */

    QToolBar::paintEvent(event);
}


