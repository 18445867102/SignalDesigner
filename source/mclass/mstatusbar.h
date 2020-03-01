#ifndef MSTATUSBAR_H
#define MSTATUSBAR_H

#include <QtWidgets>

class MStatusBar : public QStatusBar
{
    Q_OBJECT
public:
    explicit MStatusBar(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;

    qreal level;
signals:

public slots:
    void slot_updataZoomLevel(qreal);
};

#endif // MSTATUSBAR_H
