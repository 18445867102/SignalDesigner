#ifndef MLISTWIDGET_H
#define MLISTWIDGET_H

#include <QtWidgets>

class MListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MListWidget(QWidget *parent = nullptr);

    //void dragEnterEvent(QDragEnterEvent *event);
    //void dragLeaveEvent(QDragLeaveEvent *event);
    //void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);


    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void signal_itemRowChange(QString);
public slots:

protected:
    QPoint dragStartPosition;
};

#endif // MLISTWIDGET_H
