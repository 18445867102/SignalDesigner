#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include "modulesclass/mlabel.h"
#include "modulesclass/itembase.h"
#include <QtWidgets>

class DragWidget : public QWidget
{
public:
    explicit DragWidget(QWidget *parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event)override;
    void mousePressEvent(QMouseEvent *event) override;

protected:
    QPoint dragStartPosition;
};
#endif // DRAGWIDGET_H
