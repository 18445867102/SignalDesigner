#ifndef MCOMBOBOX_H
#define MCOMBOBOX_H

#include <QtWidgets>

class MCombobox :public QComboBox
{
    Q_OBJECT
public:
    explicit MCombobox(QWidget *parent = Q_NULLPTR);

private:
    void keyPressEvent(QKeyEvent*);
signals:
    void signal_ComboBoxkeyPress(int,QComboBox*);

public slots:

protected:

};

#endif // MCOMBOBOX_H
