#include "mdockwidget.h"
#define cout  qDebug() << __LINE__ << ":"

MDockWidget::MDockWidget(const QString &title, QWidget *parent) :
    QDockWidget(parent)
{
    titleWidget = new MDockTitleWidget(this);
    setTitleBarWidget(titleWidget);
    setWindowTitle(title);
    setStyleSheet("QPushButton:hover {\
                            background-color:rgb(220, 248, 255);\
                        }\
                        QPushButton:pressed {\
                            margin-top:5px;\
                            border: none;\
                        }\
                        QPushButton{\
                            border: 1px solid DarkGray;\
                            border-radius: 0px;\
                            background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 	1,stop: 0 #f6f7fa, stop: 1 #dadbde);\
                            min-width: 60px;\
                            min-height: 26px;\
                        }");

    add = new QPushButton(QIcon(":/icon/add.png"),"");
    add->setToolTip("添加库");
    remove = new QPushButton(QIcon(":/icon/sub.png"), "");
    remove->setToolTip("删除库");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(add);
    buttonLayout->addWidget(remove);
    connect(add, &QPushButton::clicked, this, &MDockWidget::slot_addBoard);
    connect(remove, &QPushButton::clicked, this, &MDockWidget::slot_removeBoard);

    listBoard = new QListWidget(this);
    listBoard->setSelectionBehavior(QAbstractItemView::SelectRows);
    listBoard->setSelectionMode(QAbstractItemView::SingleSelection);
    listBoard->setSpacing(0);
    listBoard->setStyleSheet("QListView::item{\
                             font: 9pt 宋体\
                            }");
    //listBoard->setFlow(QListView::LeftToRight);

//    QHeaderView *h_headerView = listBoard
//    h_headerView->setStretchLastSection(true);                          //列最后的空间补全
//    h_headerView->setDefaultAlignment(Qt::AlignHCenter);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(2,2,2,2);
    vbox->setSpacing(2);
    vbox->addWidget(listBoard);
    vbox->addLayout(buttonLayout);

    centerWidget = new QWidget;
    centerWidget->setLayout(vbox);
    setWidget(centerWidget);

    readSetting();      //读取设置
}

void MDockWidget::writeSetting()
{
    QSettings settings;
    settings.setValue("filePathList", filePathList);
}

void MDockWidget::readSetting()
{
    QSettings settings;
    filePathList = settings.value("filePathList").toStringList();
}

void MDockWidget::updateBoard()
{
    for(QString str: filePathList)
    {
        if(!QFile::exists(str))
        {
            QMessageBox::information(this, tr("提示"), tr("文件不存在。\t\t\t"));
            filePathList.removeOne(str);//删除路径
            continue;
        }

        QFile jsonFile(str);
        if(!jsonFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("提示"), tr("文件打开失败。\t\t\t"));
            filePathList.removeOne(str);//删除路径
            continue;
        }

        QByteArray jsonByteArray = jsonFile.readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonByteArray,&jsonError));

        if(jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError))
        {
            jsonFile.close();
            continue;
        }

        QJsonObject object = jsonDoc.object();  // 转化为对象
        if(object.contains("boardid"))//是否是板文件
        {
            QJsonValue value = object.value("boardid");
            if (value.isString())
            {
                QListWidgetItem *item = new QListWidgetItem(value.toString());
                item->setFont(QFont("宋体",12,QFont::Bold,true));
                int row = listBoard->count();
                listBoard->insertItem(row, item);
                emit signal_boardAdd(row);
            }
        } else
        {
            QMessageBox::information(this, tr("提示"), tr("文件内容不匹配。"));
        }

        jsonFile.close();
    }
}

void MDockWidget::slot_addBoard()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择文件"),
                                                    QDir::currentPath(),
                                                    tr("板文件 (*.json)"));
    filePathList.append(fileName);//向文件列表添加文件

    QFile jsonFile(fileName);
    if(!jsonFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, tr("提示"), tr("文件打开失败。\t\t\t"));
        return;
    }

    QByteArray jsonByteArray = jsonFile.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonByteArray,&jsonError));
    if(jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        jsonFile.close();
        return;
    }

    QJsonObject object = jsonDoc.object();  // 转化为对象
    if(object.contains("boardid"))//是否是板文件
    {
        QJsonValue value = object.value("boardid");
        if (value.isString())
        {
            QListWidgetItem *item = new QListWidgetItem(value.toString());
            item->setFont(QFont("宋体",12,QFont::Bold,true));
            int row = listBoard->count();
            listBoard->insertItem(row, item);
            emit signal_boardAdd(row);
        }
    } else
    {
        QMessageBox::information(this, tr("提示"), tr("文件内容不匹配。"));
    }

    jsonFile.close();
}

void MDockWidget::slot_removeBoard()
{
    int currentRow = listBoard->currentRow();

    if(currentRow == -1)
        return;

    listBoard->takeItem(currentRow);
    filePathList.removeAt(currentRow);
    emit signal_boardRemove(currentRow);
}

MDockWidget::~MDockWidget()
{
    writeSetting();
}
