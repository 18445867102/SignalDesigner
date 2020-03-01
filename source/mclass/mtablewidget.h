#ifndef MTABLEWIDGET_H
#define MTABLEWIDGET_H

#include <QtWidgets>

class MTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MTableWidget(QWidget *parent = nullptr);

//    void dragEnterEvent(QDragEnterEvent *event);
//    void dragLeaveEvent(QDragLeaveEvent *event);
//    void dragMoveEvent(QDragMoveEvent *event);
//    void dropEvent(QDropEvent *event);


//    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);

protected:
//    QPoint dragStartPosition;
signals:


public slots:
};

#endif // MTABLEWIDGET_H
