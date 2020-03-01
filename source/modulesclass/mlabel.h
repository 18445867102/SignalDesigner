#ifndef MLABEL_H
#define MLABEL_H

#include <QtWidgets>

class MLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MLabel(QPixmap pixmap = QPixmap(), QWidget *parent = nullptr);

    QVariantMap moduledata;
    QString moduleName;
    int moduleNum;
    int moduleType;
signals:

public slots:
};

#endif // MLABEL_H
