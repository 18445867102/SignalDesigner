#include "mainwindow.h"
#include "ui_mainwindow.h"

#define cout  qDebug() << __LINE__ << ":"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Module Controller v1.4"));
    setWindowIcon(QPixmap(":/icon/app.ico"));
    resize(QApplication::desktop()->width()*0.8,
           QApplication::desktop()->height()*0.8);
    setDockOptions(QMainWindow::AllowTabbedDocks|
                   QMainWindow::VerticalTabs|
                   QMainWindow::GroupedDragging);

//    QPalette palette(this->palette());
//    palette.setColor(QPalette::Background, Qt::white);
//    setPalette(palette);

//    setWindowFlag(Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_TranslucentBackground);

    //创建子窗口对像
    serialPort = new MSerialWidget();
    connect(serialPort,SIGNAL(signal_serialIsOpen(bool)),this,SLOT(slot_serialIsOpen(bool)));

    editBoard = new MBoardEditor();

    //初始化工具栏,底部状态栏
    create_mainWidget();
    create_toolbar();
    create_statusbar();
    create_dockwidget();

    connect(ui->actionExit,SIGNAL(triggered()),this,SLOT(close()));
    connect(ui->actionclear,SIGNAL(triggered()),this,SLOT(slotClear()));
    connect(ui->actionEditHardware,SIGNAL(triggered()),this,SLOT(slot_editHardware()));
    connect(ui->actionDebug,SIGNAL(triggered()),this,SLOT(slot_setSerialPort()));
}

void MainWindow::create_toolbar()
{
    action_savefile = new QAction(QIcon(":/icon/save.ico"),tr("Save"));
    action_savefile->setFont(QFont("微软雅黑",10));
    ui->mainToolBar->addAction(action_savefile);
    connect(action_savefile,SIGNAL(triggered()),this,SLOT(on_actionSaveFile_triggered()));

    action_serialPort = new QAction(QIcon("./icon/connectorRed.ico"),tr("设备连接"));
    action_serialPort->setFont(QFont("微软雅黑",10));
    ui->mainToolBar->addAction(action_serialPort);
    connect(action_serialPort,SIGNAL(triggered()),this,SLOT(slot_setSerialPort()));

    action_configure = new QAction(QIcon(":/icon/download_48px.ico"),tr("Download"));
    action_configure->setFont(QFont("微软雅黑",10));
    ui->mainToolBar->addAction(action_configure);
    connect(action_configure,SIGNAL(triggered()),this,SLOT(slot_configure()));
}

void MainWindow::create_statusbar()
{    
    statusBar = ui->statusBar;

    stb_viewPoint = new QLabel("ViewPos:");
    stb_viewPoint->setAlignment(Qt::AlignCenter);
    stb_viewPoint->setMinimumWidth(100);
    //stb_viewPoint->setFrameShape(QFrame::WinPanel); //设置标签形状
    //stb_viewPoint->setFrameShadow(QFrame::Sunken); //设置标签阴影
    statusBar->addPermanentWidget(stb_viewPoint);

    stb_scenePoint = new QLabel("ScenePos:");
    stb_scenePoint->setAlignment(Qt::AlignCenter);
    stb_scenePoint->setMinimumWidth(100);
    statusBar->addPermanentWidget(stb_scenePoint);

    stb_filePath = new QLabel("no file open.");
    stb_filePath->setAlignment(Qt::AlignCenter);
    statusBar->addWidget(stb_filePath);
}

void MainWindow::create_dockwidget()
{
    //流布局
    flow = new QFlowLayout;

    //模块库停靠窗口
    moduleboxWidget = new QDockWidget("模块库", this);
    moduleboxWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    moduleboxWidget->setFeatures(QDockWidget::AllDockWidgetFeatures); //具有全部特性 
    MDockTitleWidget *titleWidget = new MDockTitleWidget(moduleboxWidget);
    moduleboxWidget->setTitleBarWidget(titleWidget);

    //拖拽窗口
    DragWidget * dragwidget = new DragWidget;
    dragwidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dragwidget->setLayout(flow);

    //滚动区域
    QScrollArea *scrollarea = new QScrollArea();
   // scrollarea->setFrameShape(QFrame::NoFrame);         //设置无边框
    scrollarea->setMinimumWidth(50);
    scrollarea->setWidgetResizable(true);                            /* LUCKY MOMENT! */
    scrollarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);/* LUCKY MOMENT! */
    scrollarea->setWidget(dragwidget);
    moduleboxWidget->setWidget(scrollarea);
    addDockWidget(Qt::LeftDockWidgetArea, moduleboxWidget);

    //板库窗口
    mdockWidget = new MDockWidget("板库", this);
    connect(mdockWidget->listBoard, SIGNAL(currentRowChanged(int)), this, SLOT(slot_boardChange(int)));
    connect(mdockWidget, SIGNAL(signal_boardAdd(int)), this, SLOT(slot_boardAdd(int)));
    connect(mdockWidget, SIGNAL(signal_boardRemove(int)), this, SLOT(slot_boardRemove(int)));
    mdockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    mdockWidget->updateBoard();
    addDockWidget(Qt::RightDockWidgetArea, mdockWidget);

    resizeDocks({moduleboxWidget, mdockWidget},{240,240},Qt::Horizontal);
    tabifyDockWidget(moduleboxWidget, mdockWidget);
    connect(this, SIGNAL(tabifiedDockWidgetActivated(QDockWidget*)), this, SLOT(slot_tabifiedDockWidgetActivated(QDockWidget*)));
}

void MainWindow::create_mainWidget()
{
    quickWidget = new QQuickWidget(QUrl("qrc:/Tabbar.qml"));
    quickWidget->setMinimumWidth(22);
    quickWidget->setAttribute(Qt::WA_AlwaysStackOnTop);
    quickWidget->setClearColor(Qt::transparent);

    stackWidget = new QStackedWidget();
//    connect(stackWidget, &QStackedWidget::currentChanged,
//            [=](int index){

//    });

    QHBoxLayout *l = new QHBoxLayout();
    l->setContentsMargins(2,4,4,4);
    l->addWidget(stackWidget);
    centralWidget()->setLayout(l);
}

void MainWindow::layoutAddInOutModule()
{
    static const QPoint hourHand[5] = {
        QPoint(2, 2),QPoint(25, 2), QPoint(38, 25),QPoint(25, 46),  QPoint(2,46),
    };

    QImage image(40, 50, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0,0));

    QFont font("Times New Roman");
    font.setStyleStrategy(QFont::ForceOutline);
    QPen pen(Qt::black);
    pen.setWidthF(2);
    pen.setJoinStyle(Qt::RoundJoin);
    QLinearGradient linear(image.rect().topLeft(),image.rect().bottomRight());
    linear.setColorAt(0.0, Qt::white);
    linear.setColorAt(1.0, QColor(230,230,230));

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);//qRgb(164, 213, 255)
    painter.setPen(pen);
    painter.setBrush(linear);
    painter.drawConvexPolygon(hourHand, 5);
    painter.setFont(font);
    painter.drawText(image.rect(), Qt::AlignCenter, "输入");
    painter.end();

    MLabel* label = new MLabel(QPixmap::fromImage(image));
    label->moduleType = ItemBase::InModuleType;
    flow->addWidget(label);

    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);//qRgb(164, 213, 255)
    painter.setPen(pen);
    painter.setBrush(linear);
    painter.drawConvexPolygon(hourHand, 5);
    painter.setFont(font);
    painter.drawText(image.rect(), Qt::AlignCenter, "输出");
    painter.end();

    label = new MLabel(QPixmap::fromImage(image));
    label->moduleType = ItemBase::OutModuleType;
    flow->addWidget(label);
}

//清除场景中所有的图元
void MainWindow::slotClear()
{
    MGraphicsView *view = static_cast<MGraphicsView*>(stackWidget->currentWidget());
    view->scene->clear();
}

//底部状态栏显示视图坐标
void MainWindow::slot_cursorPostion(QPoint viewPoint)
{
    MGraphicsView *view = static_cast<MGraphicsView*>(stackWidget->currentWidget());

    QPointF scenePoint = view->mapToScene(viewPoint);
    stb_viewPoint->setText("ViewPos:"+QString("%1,%2").arg(viewPoint.x()).arg(viewPoint.y()));
    stb_scenePoint->setText("ScenePos:"+QString("%1,%2")
                            .arg(scenePoint.toPoint().x()) .arg(scenePoint.toPoint().y()));
}

//停靠窗口激活槽函数
void MainWindow::slot_tabifiedDockWidgetActivated(QDockWidget *dockWidget)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(dockWidget, "geometry");
    pAnimation->setDuration(200);
    pAnimation->setStartValue(QRect(-dockWidget->width(), dockWidget->y(), dockWidget->width(), dockWidget->height()));
    pAnimation->setEndValue(dockWidget->geometry());
    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}


//板库更改槽函数，更新模块库
void MainWindow::slot_boardChange(int index)
{
    if(index == -1)
        return;

  //  pre = stackWidget->currentIndex();

//    QPropertyAnimation *animation1= new QPropertyAnimation(stackWidget->widget(index),"geometry");
//    animation1->setDuration(300);
//    animation1->setStartValue(QRect(-stackWidget->width(), 0, stackWidget->width(), stackWidget->height()));
//    animation1->setEndValue(stackWidget->geometry());

//    QPropertyAnimation *animation= new QPropertyAnimation(stackWidget->currentWidget(),"geometry");
//    animation->setDuration(300);
//    animation->setStartValue(stackWidget->geometry());
//    animation->setEndValue(QRect(stackWidget->width(), 0, stackWidget->width(), stackWidget->height()));
//   // animation->start();

//    QParallelAnimationGroup *group = new QParallelAnimationGroup;
//    group->addAnimation(animation);
//    //group->addAnimation(animation1);
//    group->start();

    stackWidget->setCurrentIndex(index);

    QFile jsonFile(mdockWidget->filePathList.at(index));

    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        cout<<"Json file open fail.";
        return;
    }

    //清除布局中的项目,从末尾开始删除。
    int itemCount = flow->count();
    for (int i = (itemCount - 1); i >= 0; --i)
    {
        QLayoutItem *item = flow->takeAt(i);
        if (item != nullptr)
        {
            //flow->removeWidget(item->widget());
            //delete item->widget();
            item->widget()->close();
        }
    }

    QByteArray jsonArray = jsonFile.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonArray,&jsonError));
    if(jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        cout<<"json error.";
        return;
    }

    layoutAddInOutModule();//输入输出

    QJsonObject object = jsonDoc.object();  // 转化为对象

    if (object.contains("boardid"))
    {
        QJsonValue value = object.value("boardid");
        if (value.isString()) {
            //ui->lineEdit->setText(value.toString());
        }
    }
    if(object.contains("modules"))
    {
        QJsonArray modules = object.value("modules").toArray();

        int moduleCount = modules.size();
        for(int i = 0; i<moduleCount; i++)
        {
            QJsonObject module = modules.at(i).toObject();

            QString moduleName = module.value("moduleName").toString();
            int moduleNum = module.value("moduleNum").toInt();

            //改变数据结构用来还原设计界面
            modulesData.insert(moduleName,QJsonValue(module));

/*            QFontMetrics metric(this->font());
            QSize size = metric.size(Qt::TextSingleLine, module.value("moduleName").toString())*/;

            QImage image(90, 50, QImage::Format_ARGB32_Premultiplied);
            image.fill(qRgba(0, 0, 0, 0));

            QFont font("Times New Roman");
            font.setStyleStrategy(QFont::ForceOutline);
            QPen pen(Qt::black);
            pen.setWidthF(2.4);
            pen.setJoinStyle(Qt::RoundJoin);
            QLinearGradient linear(image.rect().topLeft(),image.rect().bottomRight());
            linear.setColorAt(0.0, Qt::white);
            linear.setColorAt(1.0, QColor(230,230,230));

            QPainter painter;
            painter.begin(&image);
            painter.setRenderHint(QPainter::Antialiasing);//qRgb(164, 213, 255)
            painter.setPen(pen);
            painter.setFont(font);
            if(moduleNum == 0){
                painter.setBrush(QBrush(QColor(127, 127, 127, 80), Qt::SolidPattern));
            }else{
                painter.setBrush(linear);
            }

            painter.drawRect(image.rect());
            painter.drawText(image.rect(), Qt::AlignCenter, moduleName);
            font.setPointSize(6);
            painter.setFont(font);
            painter.drawText(image.rect().adjusted(0,0,-4,-2), Qt::AlignBottom|Qt::AlignRight, QString::number(moduleNum));
            painter.end();

            MLabel* label = new MLabel(QPixmap::fromImage(image));
            label->moduleName = moduleName;
            label->moduleNum = moduleNum;
            label->moduledata = module.toVariantMap();
            label->moduleType = ItemBase::MoudleType;

            flow->addWidget(label);
        }
    }
}
//添加板槽函数，添加堆栈窗口
void MainWindow::slot_boardAdd(int index)
{
    Q_UNUSED(index)

    MGraphicsView *view = new MGraphicsView;
    connect(view,SIGNAL(signal_zoomLevel(qreal)),statusBar,SLOT(slot_updataZoomLevel(qreal)));
    connect(view,SIGNAL(signal_viewPostion(QPoint)),this,SLOT(slot_cursorPostion(QPoint)));

    stackWidget->addWidget(view);
}
//删除板
void MainWindow::slot_boardRemove(int index)
{
    stackWidget->removeWidget(stackWidget->widget(index));
}
//串口是否打开槽信号，更改连接按钮图标
void MainWindow::slot_serialIsOpen(bool open)
{
    if(open){
        action_serialPort->setIcon(QIcon("./icon/connectorGreen.ico"));
    }else{
        action_serialPort->setIcon(QIcon("./icon/connectorRed.ico"));
    }
}
//打开下载调试界面
void MainWindow::slot_setSerialPort()
{
    int width = QApplication::desktop()->width();
    int height = QApplication::desktop()->height();
    QPropertyAnimation *pAnimation = new QPropertyAnimation(serialPort, "geometry");
    pAnimation->setDuration(300);
    pAnimation->setStartValue(QRect(width/2, height/2, 0, 0));
    pAnimation->setEndValue(QRect(QPoint((width-serialPort->width())/2, (height-serialPort->height())/2),serialPort->size()));
    pAnimation->setEasingCurve(QEasingCurve::OutElastic);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    serialPort->setVisible(true);
    serialPort->raise();
    serialPort->activateWindow();
}
//板编辑器
void MainWindow::slot_editHardware()
{
    editBoard->show();
    editBoard->raise();
    editBoard->activateWindow();
}
//配置
void MainWindow::slot_configure()
{
    QByteArray array;

    MGraphicsView *view = static_cast<MGraphicsView*>(stackWidget->currentWidget());
    QList<QGraphicsItem *> mitems = view->scene->items(Qt::AscendingOrder);
    ArrowOut* arrowout;
    foreach (QGraphicsItem *item, mitems)
    {
        if(item->type() == ItemBase::InModuleType)
        {
            InModule* inmodule =  static_cast<InModule*>(item);
            arrowout = inmodule->arrowOut;
            ModuleBase *module = static_cast<ModuleBase *>(arrowout->connectArrow->parentItem());
            if(module->moduleName == "阻抗匹配"){
                array.append('0');
                arrowout = module->listArrowOut.first();
            }else {
                array.append('1');
            }

            while(true)
            {
                if(arrowout->connectArrow->parentItem()->type() == ItemBase::OutModuleType)
                {
//                    array.append('3');
//                    array.append('0');
                    break;
                }

                ModuleBase *module = static_cast<ModuleBase *>(arrowout->connectArrow->parentItem());
                QFormLayout *f = static_cast< QFormLayout *>(module->groupbox->layout());
                int index = static_cast<QComboBox*>(f->itemAt(0,QFormLayout::FieldRole)->widget())->currentIndex();

                if(module->moduleName == "放大器"){
                   switch (index) {
                    case 0:
                        array.append('0');
                        break;
                    case 1:
                        array.append('2');
                        break;
                    case 2:
                        array.append('3');
                        break;
                    case 3:
                        array.append('1');
                        break;
                    default:
                        break;
                    }
                }else if(module->moduleName == "直流偏置模块"){
                    switch (index) {
                     case 0:
                         array.append('5');
                         break;
                     case 1:
                         array.append('4');
                         break;
                     default:
                         break;
                     }
                }

                arrowout = module->listArrowOut.first();
            }
        }
    }
    serialPort->sendDate(array);
    //cout<<array;
}

//保存文件
void MainWindow::on_actionSaveFile_triggered()
{
    MGraphicsView *view = static_cast<MGraphicsView*>(stackWidget->currentWidget());
    view->moduleDatafileinfo.setFile(mdockWidget->listBoard->currentItem()->data(Qt::UserRole).toString());
    view->moduleData = modulesData;

    if(!view->hasPath)
    {
        QString cur_path = QDir::currentPath() + QString("/newfile.json"); //获取应用程序当前路径
        QString dlg_title = "Save File";                     //对话框标题"
        QString filter="Resource file(*.json)" ;           //文件过滤器
        QString filePath = QFileDialog::getSaveFileName(this, dlg_title,cur_path,filter);

        view->filePath = filePath;
        view->fileinfo.setFile(view->filePath);
    }
    view->saveProjectFile();
}
//打开工程文件槽函数
void MainWindow::on_actionOpenFile_triggered()
{
    MGraphicsView *view = static_cast<MGraphicsView*>(stackWidget->currentWidget());
    view->openProjectFile();
}
//关于窗口
void MainWindow::on_action_about_triggered()
{
    QMessageBox customMsgBox;
    customMsgBox.setWindowTitle(tr("About"));
    customMsgBox.addButton(tr("Ok"),QMessageBox:: ActionRole);
    customMsgBox.setText(tr("Version: Module Controller v1.4\n"
                            "Email: yangjinliang728@126.com\n\n"
                            "Copyright © 2019 Yjl. All Right Reserved\n"
                            ));
    customMsgBox.setIconPixmap(QPixmap(":/icon/qt.png"));
    customMsgBox.exec();
}

void MainWindow::on_action_description_triggered()
{
//    QWidget *widget = new QWidget;
//    QTextBrowser *text = new QTextBrowser;
//    QFile file(":/app.mht");

//    if(!file.open(QFile::ReadOnly | QFile::Text))
//    QMessageBox::information(NULL, QStringLiteral("提示"),
//                        QStringLiteral("打不开用户协议文件"));

//    QTextStream textStream(&file);
//    text->setHtml(textStream.readAll());
//    QBoxLayout *box = new QBoxLayout(QBoxLayout::TopToBottom);
//    box->addWidget(text);
//    widget->setLayout(box);
//    widget->show();
 // cout<<QDir::currentPath()<<QDir::current();
    QDesktopServices::openUrl(QUrl::fromLocalFile(QDir::currentPath() + "/user's manual.mht"));

//     if(!officeContent_->setControl("Adobe PDF Reader"))
//         QMessageBox::critical(this, "Error", "没有安装pdf！");

//     QGridLayout *gridLayout = new QGridLayout;
//     gridLayout->addWidget(officeContent_);
//     dlg->setLayout(gridLayout);
//     officeContent_->dynamicCall(
//                 "LoadFile(const QString&)",
//                 ":/田英章 钢笔楷书实用技法字帖（1990年）.pdf");
//     dlg->exec();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
//    QString dlgTitle = "提示";
//    QString str = "文件未保存，是否保存？";
//    QMessageBox::StandardButton result = QMessageBox::question(this,dlgTitle, str,
//                                         QMessageBox::Yes|QMessageBox::No| QMessageBox::Cancel,
//                                         QMessageBox::Yes);
//    if(result == QMessageBox::Yes)
//    {
//        if(!view->hasPath)
//        {
//            QString cur_path = QDir::currentPath() + QString("/newfile.json"); //获取应用程序当前路径
//            QString dlg_title = "Save File";                     //对话框标题"
//            QString filter="Resource file(*.json)" ;           //文件过滤器
//            QString filePath = QFileDialog::getSaveFileName(this, dlg_title,cur_path,filter);

//            if(filePath.isNull())
//                event->ignore();

//            view->filePath = filePath;
//            view->fileinfo.setFile(view->filePath);
//        }
//        view->saveProjectFile();
//    }
//    else if(result == QMessageBox::No)
//    {
//        event->accept();
//    }
//    else if(result == QMessageBox::Cancel)
//    {
//        event->ignore();
//    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
