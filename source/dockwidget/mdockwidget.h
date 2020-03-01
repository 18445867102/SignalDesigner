#ifndef MDOCKWIDGET_H
#define MDOCKWIDGET_H

#include <QtWidgets>
#include "mdocktitlewidget.h"

class MDockTitleWidget;

class MDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit MDockWidget(const QString &title, QWidget *parent = Q_NULLPTR);
    ~MDockWidget();
    void readSetting();
    void writeSetting();
    void updateBoard();

    QListWidget *listBoard;
    QStringList filePathList;
private:
    MDockTitleWidget *titleWidget;
    QWidget *centerWidget;
    QPushButton *add,*remove;


public slots:
    void slot_addBoard();
    void slot_removeBoard();

signals:
    void signal_boardAdd(int);
    void signal_boardRemove(int);
};

#endif // MDOCKWIDGET_H
