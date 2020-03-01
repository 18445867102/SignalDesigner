#include "mtabbar.h"

MTabBar::MTabBar(QWidget *parent) : QTabBar(parent)
{

}
#define ICON_MARGIN 5
void MTabBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QStylePainter painter(this);

    for(int i = this->count()-1; i >= 0; --i)
    {
       if(i ==this->currentIndex())continue;
       QStyleOptionTab option;
       initStyleOption(&option, i);
       painter.drawControl(QStyle::CE_TabBarTabShape,option);


       //绘制tab icon 偏移值 ICON_MARGIN
       QRect rect = option.rect;
       rect.setX(rect.x()+ICON_MARGIN);
       rect.setWidth(rect.width()-ICON_MARGIN);
       painter.drawItemPixmap(rect, Qt::AlignLeft|Qt::AlignVCenter, option.icon.pixmap(QSize(16,16)));
       painter.drawItemText(option.rect.adjusted(-10,0,0,0), Qt::AlignCenter, palette(), 1, option.text);
    }

    //重绘当前tab标签 为了显示效果：当前标签顶部显示
    QStyleOptionTab option;
    initStyleOption(&option, this->currentIndex());
    QRect rect = option.rect;
    painter.drawControl(QStyle::CE_TabBarTabShape,option);
    rect.setX(rect.x()+ICON_MARGIN);
    rect.setWidth(rect.width()-ICON_MARGIN);
    painter.drawItemPixmap(rect, Qt::AlignLeft|Qt::AlignVCenter, option.icon.pixmap(QSize(16,16)));
    painter.drawItemText(option.rect.adjusted(-10,0,0,0), Qt::AlignCenter, palette(), 1, option.text);

}
