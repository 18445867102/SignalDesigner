#ifndef MTABBAR_H
#define MTABBAR_H

#include <QtWidgets>

class MTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit MTabBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
};

#endif // MTABBAR_H
