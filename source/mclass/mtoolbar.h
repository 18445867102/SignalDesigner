#ifndef MTOOLBAR_H
#define MTOOLBAR_H

#include <QtWidgets>

class MToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MToolBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // MTOOLBAR_H
