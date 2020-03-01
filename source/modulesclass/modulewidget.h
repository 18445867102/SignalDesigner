#ifndef MODULEWIDGET_H
#define MODULEWIDGET_H

#include <QtWidgets>

class ModuleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModuleWidget(QVariantMap obj, QWidget *parent = 0);
    ~ModuleWidget();

private:
    QJsonObject jsonData;
    QPushButton *confirm;
    QPushButton *cancel;
signals:
    void signal_setOutoutNum(int num);
public slots:
    void slot_buttonConfirm();
    void slot_buttonCancel();
};

#endif // MODULEWIDGET_H
