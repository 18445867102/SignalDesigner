#ifndef MGROUPBOX_H
#define MGROUPBOX_H

#include <QtWidgets>

class MGroupBox : public QGroupBox
{
public:
    MGroupBox(QWidget *parent = Q_NULLPTR);
    MGroupBox(const QString &title, QWidget *parent = Q_NULLPTR);

protected:
};

#endif // MGROUPBOX_H
