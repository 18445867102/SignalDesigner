#include "mtabwidget.h"

MTabWidget::MTabWidget(QWidget *parent) : QTabWidget(parent)
{
    MTabBar *tabbar = new MTabBar;
    tabbar->setShape(QTabBar::RoundedNorth);
    setTabBar(tabbar);
    setTabsClosable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(slot_tabCloseRequested(int)));
}

void MTabWidget::slot_tabCloseRequested(int index)
{
    delete widget(index);
}
