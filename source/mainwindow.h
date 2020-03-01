#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtQuickWidgets/QtQuickWidgets>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <ActiveQt/QAxWidget>
#include <QDesktopServices>
#include <QUrl>

//#include <QtWebKit/QtWebKit>
//#include <QtWebKit/QWebView>

#include "mclass/mstatusbar.h"
#include "serialport/mserialwidget.h"

#include "dockwidget/mdockwidget.h"
#include "dockwidget/mdocktitlewidget.h"

#include "tabwidget/mtabwidget.h"

#include "modulesclass/dragwidget.h"
#include "modulesclass/mgraphicsview.h"
#include "modulesclass/mgroupbox.h"

#include "mboardeditor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void create_toolbar();
    void create_statusbar();
    void create_dockwidget();
    void create_mainWidget();
    void layoutAddInOutModule();

public slots:
    void slotClear();           			//清除场景中所有的图元  
    void slot_setSerialPort();              //串口子窗口
    void slot_configure();
    void slot_editHardware();               //硬件编辑器子窗口
    void slot_cursorPostion(QPoint);        //更新状态栏
    void slot_tabifiedDockWidgetActivated(QDockWidget *);
    void slot_boardChange(int);
    void slot_boardAdd(int);
    void slot_boardRemove(int);
    void slot_serialIsOpen(bool);

private slots:

    void on_actionSaveFile_triggered();

    void on_actionOpenFile_triggered();

    void on_action_about_triggered();

    void on_action_description_triggered();

private:
    void closeEvent(QCloseEvent *event);

    //工具栏控件
    QAction *action_serialPort;
    QAction *action_configure;
    QAction *action_savefile;

    //停靠窗口
    MDockWidget *mdockWidget;
    QDockWidget *moduleboxWidget;
    QStackedWidget *stackWidget;
    MGraphicsView *view;
    QQuickWidget *quickWidget;
   // MTabWidget * tabwidget;
    //状态栏控件
    MStatusBar *statusBar;
    QLabel *stb_filePath;
    QLabel *stb_viewPoint;
    QLabel *stb_scenePoint;

    //子窗口
    Ui::MainWindow *ui;
    MSerialWidget *serialPort;
    MBoardEditor *editBoard;

    //流布局
    QFlowLayout *flow;
    //其它
    QListWidgetItem *previous = nullptr;
    QJsonObject modulesData;


    int pre;
};
#endif // MAINWINDOW_H
