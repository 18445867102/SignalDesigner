#include "mboardeditor.h"
#include "ui_mboardeditor.h"

#define cout  qDebug() << __LINE__ << ":"

MBoardEditor::MBoardEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MBoardEditor)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("板编辑器"));
    this->resize(840,550);

    tbw_propertys = ui->tbw_propertys;
    lst_parameterName = ui->propertyname;
    tbw_modules = ui->tbw_modules;

    F_ModuleCellChangeLock = false;
    F_ModuleCurrentRowLock = true;
    F_PropertyCellChangeLock = false;
    F_PropertyCurrentRowLock = true;

    interfaceInit();

    connect(ui->btn_moduleadd,SIGNAL(clicked()),this,SLOT(slot_moudleAdd()));
    connect(ui->btn_moduledec,SIGNAL(clicked()),this,SLOT(slot_moudleDec()));
    connect(ui->btn_propertyadd,SIGNAL(clicked()),this,SLOT(slot_propertyAdd()));
    connect(ui->btn_propertydec,SIGNAL(clicked()),this,SLOT(slot_propertyDec()));
    connect(tbw_modules,SIGNAL(cellChanged(int, int)),this,SLOT(slot_moduleCellChanged(int, int)));
    connect(tbw_modules,SIGNAL(itemSelectionChanged()),this,SLOT(slot_moduleSelectionChanged()));
    connect(ui->propertyname,SIGNAL(itemSelectionChanged()),this,SLOT(slot_propertySelectionChanged()));
    connect(ui->propertyname,SIGNAL(itemChanged(QListWidgetItem*)),this,SLOT(slot_propertyNameChanged(QListWidgetItem*)));
    connect(ui->tbw_propertys,SIGNAL(cellChanged(int, int)),this,SLOT(slot_propertyData(int,int)));
}

void MBoardEditor::interfaceInit()
{
    QStringList list;
    list<<"参数形式"<<"单位"<<"档位值"<<"maximum"<<"minimum"<<"signalStep";

    ui->splitter_4->setStretchFactor(0,3);
    ui->splitter_4->setStretchFactor(1,7);

    tbw_modules->setColumnCount(4);         //设置列数
    tbw_modules->setColumnWidth(0, 130);    //设置列宽
    tbw_modules->setHorizontalHeaderLabels(QStringList()<<"模块名称"<<" 输入 "<<" 输出 "<<"数量");
    tbw_modules->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置每次选择一个项目
    tbw_modules->setEditTriggers(QAbstractItemView::DoubleClicked);   //设置触发编辑模式
    tbw_modules->viewport()->setFocusPolicy(Qt::StrongFocus);
    //tbw_modules->setBackgroundRole(QPalette::NoRole);
    //tbw_modules->setForegroundRole(QPalette::NoRole);
    tbw_modules->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
    //tbw_modules->setAlternatingRowColors(true);                         //交替显示
    tbw_modules->setSelectionMode(QAbstractItemView::SingleSelection);   //设置选择模式
    //tbw_modules->setSelectionModel(0);
    QHeaderView *v_headerView = tbw_modules->verticalHeader();
    v_headerView->setHidden(true);
    v_headerView->setDefaultSectionSize(20);
    QHeaderView *h_headerView = tbw_modules->horizontalHeader();
    h_headerView->setSectionResizeMode(0,QHeaderView::Interactive);
    h_headerView->setSectionResizeMode(1,QHeaderView::Stretch);
    h_headerView->setSectionResizeMode(2,QHeaderView::Stretch);
    h_headerView->setSectionResizeMode(3,QHeaderView::Stretch);
    //h_headerView->setStretchLastSection(true);                          //列最后的空间补全
    h_headerView->setDefaultAlignment(Qt::AlignHCenter);

    SpinBoxDelegate *spinBoxDelegate = new SpinBoxDelegate(this);
    tbw_modules->setItemDelegateForColumn(1,spinBoxDelegate);
    tbw_modules->setItemDelegateForColumn(2,spinBoxDelegate);
    tbw_modules->setItemDelegateForColumn(3,spinBoxDelegate);

 //----------------------------------------------------------------------------------------------------------
    lst_parameterName->setSelectionMode(QAbstractItemView::SingleSelection);

    tbw_propertys->setColumnCount(2);
    tbw_propertys->setColumnWidth(0, 100);
    tbw_propertys->setHorizontalHeaderLabels(QStringList()<<"属性"<<"值");
    tbw_propertys->setSelectionBehavior(QAbstractItemView::SelectRows);    //设置每次选择一个项目
    tbw_propertys->setEditTriggers(QAbstractItemView::DoubleClicked);      //设置双击编辑
    tbw_propertys->viewport()->setFocusPolicy(Qt::NoFocus);
    //tbw_propertys->setStyleSheet("selection-background-color:lightblue;");  //设置选中背景色
    //tbw_propertys->setAlternatingRowColors(true);                           //交替显示
    v_headerView = tbw_propertys->verticalHeader();
    v_headerView->setHidden(true);
    v_headerView->setDefaultSectionSize(20);
    h_headerView = tbw_propertys->horizontalHeader();
    //h_headerView->hide();
    //h_headerView->setSectionResizeMode(QHeaderView::Stretch);
    h_headerView->setStretchLastSection(true);                          //列最后的空间补全
    h_headerView->setDefaultAlignment(Qt::AlignLeft);

    d = new Delegate(this);
    tbw_propertys->setItemDelegateForColumn(1,d);
    for(int i=0; i<list.size(); i++)
    {
        tbw_propertys->insertRow(i);
        QTableWidgetItem *module = new QTableWidgetItem (list[i]);
        module->setFlags(module->flags() & (~Qt::ItemIsEditable));
        tbw_propertys->setItem(i, 0, module);
    }
}

void MBoardEditor::slot_moudleAdd()
{
    F_ModuleCellChangeLock = false;

    //申请一个模块对象
    ModuleData *m = new ModuleData;
    module.append(m);

    int row_count = tbw_modules->rowCount();    //获取表格行数
    tbw_modules->insertRow(row_count);          //在表格末尾插入一行
    tbw_modules->selectRow(row_count);          //设置新插入行为选择状态

    //根据默认数据填充界面表格
    QTableWidgetItem *moduleName = new QTableWidgetItem (m->moduleName);
    tbw_modules->setItem(row_count, 0, moduleName);
    tbw_modules->editItem(moduleName);               //设置项目处于编辑模式
    moduleName = new QTableWidgetItem (QString::number(m->inputNum));
    tbw_modules->setItem(row_count, 1, moduleName);
    moduleName = new QTableWidgetItem (QString::number(m->outputNum));
    tbw_modules->setItem(row_count, 2, moduleName);
    moduleName = new QTableWidgetItem (QString::number(m->moduleNum));
    tbw_modules->setItem(row_count, 3, moduleName);

    //新添加模块，清除参数列表窗口
    lst_parameterName->clear();

    F_ModuleCellChangeLock = true;
}
void MBoardEditor::slot_moudleDec()
{
    F_ModuleCurrentRowLock = false;

    int row = tbw_modules->currentRow();
    //cout<<row;
    if(row != -1)
    {
        module.removeAt(row);
        tbw_modules->removeRow(row);
    }

    F_ModuleCurrentRowLock = true;

    slot_moduleSelectionChanged();
}
void MBoardEditor::slot_moduleCellChanged(int row, int column)
{
    if(F_ModuleCellChangeLock)
    {
        //cout<<"moduleCellChange"<<row<<column;

        //将更改的模块数据写入模块数据结构
        if(column == 0){
            module[row]->moduleName = tbw_modules->item(row,column)->text();
        } else if(column == 1){
            module[row]->inputNum = tbw_modules->item(row,column)->text().toInt();
        } else if(column == 2){
            module[row]->outputNum = tbw_modules->item(row,column)->text().toInt();
        } else {
            module[row]->moduleNum = tbw_modules->item(row,column)->text().toInt();
        }
    }
}

void MBoardEditor::slot_propertyAdd()
{  
    if(tbw_modules->currentRow() == -1)
        return;

    ParameterData* p = module[tbw_modules->currentRow()]->addParameter();

    QListWidgetItem *item = new QListWidgetItem(p->parameterName);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled
                   |Qt::ItemIsUserTristate);
    lst_parameterName->addItem(item);
    lst_parameterName->setCurrentRow(lst_parameterName->row(item));
    lst_parameterName->editItem(item);

    F_PropertyCellChangeLock = false;

    if(p->f_form == true)
        tbw_propertys->setItem(0, 1, new QTableWidgetItem ("档位"));
    else
        tbw_propertys->setItem(0, 1, new QTableWidgetItem ("连续可调"));

    if(p->l.isEmpty())
        tbw_propertys->setItem(2,1,new QTableWidgetItem("<无>"));
    else
    {
        d->strListAssignment(p->l);
        tbw_propertys->setItem(2,1,new QTableWidgetItem(p->l.first()));
    }

    tbw_propertys->setItem(3,1,new QTableWidgetItem(QString::number(p->maximum)));
    tbw_propertys->setItem(4,1,new QTableWidgetItem(QString::number(p->minimum)));
    tbw_propertys->setItem(5,1,new QTableWidgetItem(QString::number(p->step)));

    F_PropertyCellChangeLock = true;
}

void MBoardEditor::slot_propertyDec()
{
    if(tbw_modules->currentRow() == -1)
        return;

    //cout<<lst_parameterName->currentRow();

    F_PropertyCurrentRowLock = false;

    module[tbw_modules->currentRow()]->decParameter(lst_parameterName->currentRow());
    delete lst_parameterName->takeItem(lst_parameterName->currentRow());

    F_PropertyCurrentRowLock = true;

    slot_propertySelectionChanged();
}

/*
    tbw_propertys->setCellWidget(i,1,m_edit);
    //设置正则表达式限制输入
    QRegExp rx("^[\\+\\-]?[\\d]+(\\.[\\d]+)?$");
    m_edit->setValidator(new QRegExpValidator(rx,m_edit));
*/

void MBoardEditor::slot_propertyData(int row,int column)
{
    if(!F_PropertyCellChangeLock)
        return;

    //cout<<"propertyDataChange"<<row<<column;

    if(tbw_modules->currentRow() == -1)
        return;
    if(lst_parameterName->currentRow() == -1)
        return;

    ParameterData* p = module[tbw_modules->currentRow()]->parameterList[lst_parameterName->currentRow()];

    //将更新后的数据写入模块参数的数据结构
    QString str;
    switch (row) {
    case 0:
        str = tbw_propertys->item(row,column)->text();
        if(str == "档位")
            p->f_form = true;
        else
            p->f_form = false;
        break;
    case 1:
        p->parameterUnit = tbw_propertys->item(row,column)->text();
        break;
    case 2:
        d->GetStrList(p->l);
        //cout<<p->l;
        break;
    case 3:
        p->maximum = tbw_propertys->item(row,column)->text().toDouble();
        break;
    case 4:
        p->minimum = tbw_propertys->item(row,column)->text().toDouble();
        break;
    case 5:
        p->step = tbw_propertys->item(row,column)->text().toDouble();
        break;
    default:
        break;
    }
}

void MBoardEditor::slot_moduleSelectionChanged()
{
    if(!F_ModuleCurrentRowLock)
        return;
    if(tbw_modules->currentRow() == -1)
        return;

    //cout<<"M SeleChange";

    //清除参数列表
    lst_parameterName->setCurrentRow(-1);
    lst_parameterName->clear();

    //获取当前模块的参数列表
    QList<ParameterData*> parameter = module[tbw_modules->currentRow()]->parameterList;

    if(!parameter.isEmpty())
    {
        //填充界面表格
        foreach (ParameterData *p, parameter)
        {
            QListWidgetItem *item = new QListWidgetItem(p->parameterName);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|
                           Qt::ItemIsEnabled|Qt::ItemIsUserTristate);
            lst_parameterName->addItem(item);
        }
        //设置当前行为第一行
        lst_parameterName->setCurrentRow(0);
    }
}

void MBoardEditor::slot_propertySelectionChanged()
{
    if(!F_PropertyCurrentRowLock)
        return;
    if(tbw_modules->currentRow() == -1)
        return;
    if(lst_parameterName->currentRow() == -1)
        return;
    //cout<<"P change"<<tbw_modules->currentRow()<<lst_parameterName->currentRow();

    F_PropertyCellChangeLock = false;

    //获取数据结构并向界面填充数据
    ParameterData *p = module[tbw_modules->currentRow()]->parameterList[lst_parameterName->currentRow()];

    if(p->f_form == true)
        tbw_propertys->setItem(0, 1, new QTableWidgetItem ("档位"));
    else
        tbw_propertys->setItem(0, 1, new QTableWidgetItem ("连续可调"));

    d->strListAssignment(p->l);
    if(p->l.isEmpty())
        tbw_propertys->setItem(2,1,new QTableWidgetItem(""));
    else{
        d->strListAssignment(p->l);
        tbw_propertys->setItem(2,1,new QTableWidgetItem(p->l.first()));
    }
    //tbw_modules->item()
    tbw_propertys->setItem(1,1,new QTableWidgetItem(p->parameterUnit));
    tbw_propertys->setItem(3,1,new QTableWidgetItem(QString::number(p->maximum)));
    tbw_propertys->setItem(4,1,new QTableWidgetItem(QString::number(p->minimum)));
    tbw_propertys->setItem(5,1,new QTableWidgetItem(QString::number(p->step)));

    F_PropertyCellChangeLock = true;
}

void MBoardEditor::slot_propertyNameChanged(QListWidgetItem* item)
{
    if(tbw_modules->currentRow() == -1)
        return;

    module[tbw_modules->currentRow()]\
            ->parameterList[lst_parameterName->currentRow()]\
            ->parameterName = item->text();
}

void MBoardEditor::on_btn_save_clicked()
{
    saveAsJsonFile();
}

void MBoardEditor::on_btn_open_clicked()
{
    //清理界面及数据容器
    F_ModuleCellChangeLock = false;
    F_PropertyCellChangeLock = false;
    F_ModuleCurrentRowLock = false;
    F_PropertyCurrentRowLock = false;

    module.clear();

    QString cur_path = QDir::currentPath();            //获取应用程序当前路径
    QString dlg_title = "选择文件";                     //对话框标题"
    QString filter="Resource file(*.json)" ;           //文件过滤器

    QString filePath=QFileDialog::getOpenFileName(this,dlg_title,cur_path,filter);

    if(filePath.isNull())
        return;

    QFile jsonFile(filePath);

    if(!jsonFile.open(QIODevice::ReadWrite))
    {
        cout<<"open fail.";
        return;
    }

    QByteArray jsonArray = jsonFile.readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonArray,&jsonError));

    if(jsonDoc.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        cout<<"json error.";
        return;
    }

    if (jsonDoc.isObject()) {  // JSON 文档为对象
        QJsonObject object = jsonDoc.object();  // 转化为对象
        if (object.contains("boardid")) {
            QJsonValue value = object.value("boardid");
            if (value.isString())
            {
                ui->BoardNum->setText(value.toString());
            }
        }
        
        if(object.contains("modules")){
            QJsonArray modules  = object.value("modules").toArray();
            int moduleCount = modules.size();
            for(int i = 0; i<moduleCount; i++){
                QJsonObject module = modules.at(i).toObject();
                ModuleData *m = new ModuleData;
                m->moduleName = module.value("moduleName").toString();
                m->inputNum= module.value("inputNum").toInt();
                m->outputNum = module.value("outputNum").toInt();
                m->moduleNum = module.value("moduleNum").toInt();

                QJsonArray parameters = module.value("parameters").toArray();
                int parameterCount = parameters.size();
                for(int i = 0; i<parameterCount; i++){
                    QJsonObject parameter = parameters.at(i).toObject();
                    ParameterData *p = new ParameterData;
                    p->parameterName = parameter.value("parametername").toString();
                    p->parameterUnit = parameter.value("parameterunit").toString();

                    bool form = parameter.value("form").toBool();
                    p->f_form = form;
                    if (form) {
                            QJsonArray combobox = parameter.value("combobox").toArray();
                            int nSize = combobox.size();
                            for (int i = 0; i < nSize; ++i) {
                                QJsonValue value = combobox.at(i);
                                if (value.isString()) {
                                    p->l.append(value.toString());
                                }
                            }
                    } else {
                        if (parameter.contains("max")) {
                            QJsonValue value = parameter.value("max");
                            if (value.isDouble()) {
                                p->maximum = value.toDouble();
                            }
                        }
                        if (parameter.contains("min")) {
                            QJsonValue value = parameter.value("min");
                            if (value.isDouble()) {
                                p->minimum = value.toDouble();
                            }
                        }
                        if (parameter.contains("step")) {
                            QJsonValue value = parameter.value("step");
                            if (value.isDouble()) {
                                p->step = value.toDouble();
                            }
                        }
                    }

                    m->parameterList.append(p);
                }

                this->module.append(m);
            }
        }
    }


    tbw_modules->clearContents();
    tbw_modules->setRowCount(0);
    lst_parameterName->clear();


    foreach (ModuleData *m, module)
    {
        int row_count = tbw_modules->rowCount();    //获取表格行数
        tbw_modules->insertRow(row_count);          //在表格末尾插入一行
        //根据默认数据填充界面表格,模块名
        QTableWidgetItem *tableItem= new QTableWidgetItem (m->moduleName);
        tbw_modules->setItem(row_count, 0, tableItem);
        //输入引脚数
        tableItem = new QTableWidgetItem (QString::number(m->inputNum));
        tbw_modules->setItem(row_count, 1, tableItem);
        //输出引脚
        tableItem = new QTableWidgetItem (QString::number(m->outputNum));
        tbw_modules->setItem(row_count, 2, tableItem);
        //模块数量
        tableItem = new QTableWidgetItem (QString::number(m->moduleNum));
        tbw_modules->setItem(row_count, 3, tableItem);
    }

//    for(int i =0; i<5; i++){
//        tbw_propertys->item(i,1)->setText("");
//    }

    F_ModuleCellChangeLock = true;
    F_PropertyCellChangeLock = true;
    F_ModuleCurrentRowLock = true;
    F_PropertyCurrentRowLock = true;
}

void MBoardEditor::on_btn_new_clicked()
{
    F_ModuleCellChangeLock = false;
    F_PropertyCellChangeLock = false;
    F_ModuleCurrentRowLock = false;
    F_PropertyCurrentRowLock = false;

    module.clear();
    tbw_modules->clearContents();
    tbw_modules->setRowCount(0);
    lst_parameterName->clear();

    /******************/
    //保存当前界面文件


    QWidget *newWidget = new QWidget;
    newWidget->setWindowModality(Qt::ApplicationModal);
    newWidget->setWindowTitle("新建板");
    newWidget->resize(240,70);
    newWidget->move((QApplication::desktop()->width() - newWidget->width())/2,
                    (QApplication::desktop()->height() - newWidget->height())/2);

    QLabel *l1 = new QLabel("板标识号:");
    QLabel *l2 = new QLabel("板输入通道:");
    QLabel *l3 = new QLabel("板输出通道:");
    l1->setAlignment(Qt::AlignRight);
    l2->setAlignment(Qt::AlignRight);
    l3->setAlignment(Qt::AlignRight);
    QLineEdit *n1 = new QLineEdit;
    QSpinBox *n2 = new QSpinBox;
    QSpinBox *n3 = new QSpinBox;
    n1->setMinimumWidth(180);
    n2->setMinimumWidth(180);
    n3->setMinimumWidth(180);
    QPushButton *yes = new QPushButton("确定");
    QPushButton *no = new QPushButton("取消");

    QGridLayout *g = new QGridLayout;
    g->addWidget(l1,1,1);
    g->addWidget(l2,2,1);
    g->addWidget(l3,3,1);
    g->addWidget(n1,1,2);
    g->addWidget(n2,2,2);
    g->addWidget(n3,3,2);
    QHBoxLayout *h = new QHBoxLayout;
    h->addWidget(yes);
    h->addWidget(no);
    QVBoxLayout *v = new QVBoxLayout;
    v->addLayout(g);
    v->addLayout(h);

    newWidget->setLayout(v);
    newWidget->show();

    connect(yes, &QPushButton::clicked,
            [=]()mutable
            {
                ui->BoardNum->setText(n1->text());
                ui->BoardInput->setText(n2->text());
                ui->BoardOutput->setText(n3->text());
                newWidget->close();
            });

    connect(no, &QPushButton::clicked,
            [=]()mutable
            {
                newWidget->close();
            });

    F_ModuleCellChangeLock = true;
    F_PropertyCellChangeLock = true;
    F_ModuleCurrentRowLock = true;
    F_PropertyCurrentRowLock = true;
}

void MBoardEditor::saveAsJsonFile()
{
    QJsonObject mainJson;
    mainJson.insert("boardid",ui->BoardNum->text());
    mainJson.insert("boardinput",ui->BoardInput->text());
    mainJson.insert("boardoutput",ui->BoardOutput->text());


    QJsonArray moduleArray;
    foreach (ModuleData* m, module)
    {
        QJsonObject moduledata;
        moduledata.insert("moduleName",m->moduleName);
        moduledata.insert("inputNum",m->inputNum);
        moduledata.insert("outputNum",m->outputNum);
        moduledata.insert("moduleNum",m->moduleNum);

        QJsonArray parameters;
        foreach (ParameterData* p, m->parameterList)
        {
            QJsonObject parameter;

            parameter.insert("form",p->f_form);
            parameter.insert("parametername",p->parameterName);
            parameter.insert("parameterunit",p->parameterUnit);

            if(p->f_form){
                QJsonArray array;
                foreach (QString s, p->l) {
                    array.append(s);
                }

                parameter.insert("combobox",QJsonValue(array));
            } else {
                parameter.insert("max",p->maximum);
                parameter.insert("min",p->minimum);
                parameter.insert("step",p->step);
            }

            parameters.append(QJsonValue(parameter));
        }
        moduledata.insert("parameters",QJsonValue(parameters));

        moduleArray.append(QJsonValue(moduledata));
    }
    mainJson.insert("modules",QJsonValue(moduleArray));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(mainJson);

    //QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    //QString strJson(byteArray);
    //qDebug() << strJson;

    QString cur_path = QDir::currentPath() + QString("/file.json"); //获取应用程序当前路径
    QString dlg_title = "Save File";                     //对话框标题"
    QString filter="Resource file(*.json)" ;           //文件过滤器

    QString filePath = QFileDialog::getSaveFileName(this, dlg_title,cur_path,filter);
    if(filePath.isNull())
        return;

    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "File open error";
    } else {
        qDebug() <<"File open!";
    }

    // 清空文件中的原有内容
    file.resize(0);

    file.write(document.toJson());
    file.close();
}

MBoardEditor::~MBoardEditor()
{
    delete ui;
}
