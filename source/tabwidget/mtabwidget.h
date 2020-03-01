#ifndef MTABWIDGET_H
#define MTABWIDGET_H

#include <QtWidgets>

#include "mtabbar.h"

class MTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit MTabWidget(QWidget *parent = nullptr);

signals:

public slots:
    void slot_tabCloseRequested(int);
};

#endif // MTABWIDGET_H
