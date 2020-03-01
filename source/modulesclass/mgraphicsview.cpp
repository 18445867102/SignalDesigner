#include "mgraphicsview.h"
#define cout  qDebug() << __LINE__ << ":"

MGraphicsView::MGraphicsView(QWidget *parent):QGraphicsView(parent)
{ 
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setRenderHints(QPainter::SmoothPixmapTransform|QPainter::Antialiasing);//设置显示方式
    setAlignment(Qt::AlignCenter);
    setFrameShape(QFrame::NoFrame);
    setAcceptDrops(true);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//隐藏滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    QPixmap bgPix(":/icon/background.jpg");
//    setBackgroundBrush(bgPix);
    setStyleSheet("QWidget{background-color:White;"
                  "border-radius:10px;"
                  "border-width: 2px;"
                  "border-style: solid;"
                  "border-color: DarkGray;"
                  "margin: 0px;};"
                  );

    scene = new MGraphicsScene;
    scene->setSceneRect(-2000,-1500,4000,3000);
    setScene(scene);
    //setMinimumSize(600,400);

    level = 5;
    zoomLevel = 1.0;
    hasPath = false;
    isSave = false;

    QPen pen(Qt::lightGray,0);
    QBrush brush(QColor(125,125,125,45), Qt::SolidPattern);
    selectedRectItem = new QGraphicsRectItem;
    selectedRectItem->setFlag(QGraphicsItem::ItemClipsToShape);
    selectedRectItem->setPen(pen);
    selectedRectItem->setBrush(brush);

    isRubberBandDrag = false;
    isScrollHandDrag = false;
}
//保存工程文件
void MGraphicsView::saveProjectFile()
{
    QJsonObject mainJson;

    QJsonArray moduleArray;
    QJsonArray inmoduleArray;
    QJsonArray outmoduleArray;
    
    QList<QGraphicsItem*> listItem = scene->items();//获取所有图元项
    
    int itemCount = listItem.size();
    for(int i =0; i<itemCount; i++)
    {       
        QJsonObject object;

        int itemType = listItem[i]->type();
        if(itemType == ItemBase::MoudleType)
        {
            QJsonArray inlistarray;
            QJsonArray outlistarray;
            ModuleBase *item = static_cast<ModuleBase*>(listItem[i]);
            int arrowInCount = item->listArrowIn.size();
            int arrowOutCount = item->listArrowOut.size();

            object.insert("pos.x",item->scenePos().x());
            object.insert("pos.y",item->scenePos().y());
            object.insert("arrowinnum",arrowInCount);
            object.insert("arrowoutnum",arrowOutCount);
            object.insert("modulename",item->moduleName);

            for(int j = 0; j<arrowInCount; j++)
            {
                QJsonObject arrowObject;
                ArrowIn *arrow = item->listArrowIn[j];
                arrowObject.insert("pos.x",arrow->pos().x());
                arrowObject.insert("pos.y",arrow->pos().y());
                inlistarray.append(QJsonValue(arrowObject));
            }

            for(int j = 0; j<arrowOutCount; j++)
            {
                QJsonObject arrowObject;
                ArrowOut *arrow = item->listArrowOut[j];
                arrowObject.insert("pos.x",arrow->scenePos().x());
                arrowObject.insert("pos.y",arrow->scenePos().y());
                arrowObject.insert("arrowstate",arrow->arrowState);
                arrowObject.insert("movepoint",arrow->f_movePoint);

                QJsonArray point;
                int pointCount = arrow->pointList.size();
                for(int k = 0; k<pointCount; k++){
                    point.append(arrow->pointList[k].x());
                    point.append(arrow->pointList[k].y());
                }
                arrowObject.insert("point",QJsonValue(point));

                outlistarray.append(QJsonValue(arrowObject));
            }

            object.insert("arrowinlist",QJsonValue(inlistarray));
            object.insert("arrowoutlist",QJsonValue(outlistarray));
           
            moduleArray.append(QJsonValue(object));
        }
        else if(itemType == ItemBase::InModuleType)
        {
            InModule *item = static_cast<InModule*>(listItem[i]);
            object.insert("pos.x",item->scenePos().x());
            object.insert("pos.y",item->scenePos().y());

            /**********************************************/
            QJsonObject arrowObject;
            ArrowOut *arrow = item->arrowOut;
            arrowObject.insert("pos.x",arrow->scenePos().x());
            arrowObject.insert("pos.y",arrow->scenePos().y());
            arrowObject.insert("arrowstate",arrow->arrowState);
            QJsonArray point;
            int pointCount = arrow->pointList.size();
            for(int k = 0; k<pointCount; k++){
                point.append(arrow->pointList[k].x());
                point.append(arrow->pointList[k].y());
            }
            arrowObject.insert("point",QJsonValue(point));
            /***********************************************/
            object.insert("arrowout",QJsonValue(arrowObject));

            inmoduleArray.append(QJsonValue(object));
        }
        else if(itemType == ItemBase::OutModuleType)
        {
            OutModule *item = static_cast<OutModule*>(listItem[i]);   
            object.insert("pos.x",item->scenePos().x());
            object.insert("pos.y",item->scenePos().y());
            
            outmoduleArray.append(QJsonValue(object));
        }      
    }
/**********************************************************************************************/
    mainJson.insert("modulebase", QJsonValue(moduleArray));
    mainJson.insert("inmodule", QJsonValue(inmoduleArray));
    mainJson.insert("outmodule", QJsonValue(outmoduleArray));
    mainJson.insert("moduledata",QJsonValue(moduleData));
    mainJson.insert("filepath",moduleDatafileinfo.filePath());
/**********************************************************************************************/

    QJsonDocument document;// 构建 Json 文档
    document.setObject(mainJson);
    
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<< "File open error";
    }
    
    //清空文件中的原有内容
    file.resize(0);
    file.write(document.toJson());
    file.close();
}

void MGraphicsView::openProjectFile()
{
    QString cur_path = QDir::currentPath();            //获取应用程序当前路径
    filePath = QFileDialog::getOpenFileName(this, "选择文件", cur_path, "Resource file(*.json)");

    if(filePath.isNull())
    {
        cout<<"file path null.";
        return;
    }
    hasPath = true;

    fileinfo.setFile(filePath);
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

    QJsonObject object = jsonDoc.object();  // 转化为对象
    QJsonObject moduleDataObject = object.value("moduledata").toObject();
   // int arraycount = moduleDataObject.size();

    if (object.contains("modulebase"))
    {
        QJsonArray moduleArray = object.value("modulebase").toArray();

        int modulecount = moduleArray.count();
        for(int i=0; i<modulecount; i++)
        {
            QJsonObject module = moduleArray[i].toObject();
            //模块
            ModuleBase *item = new ModuleBase();
            scene->addItem(item);
            QString moduleName = module.value("modulename").toString();
            item->setPos(module.value("pos.x").toDouble(), module.value("pos.y").toDouble());
            item->setModuleName(moduleName);

            QJsonObject moduledata = moduleDataObject.value(moduleName).toObject();

            //item->initMuduleWidget(moduledata);
            //item->initInterface(moduledata.value("inputNum").toInt(),moduledata.value("outputNum").toInt());

            //////////////////////////////////////////////
            MGroupBox *groupBox = new MGroupBox(moduleName);
            QFormLayout *formlayout = new QFormLayout;
            formlayout->setContentsMargins(2,22,2,2);
            formlayout->setVerticalSpacing(3);
            formlayout->setHorizontalSpacing(0);


            QJsonArray parameters = moduledata.value("parameters").toArray();
            int parameterCount = parameters.size();

            for(int i = 0; i<parameterCount; i++)
            {
                QJsonObject parameter = parameters.at(i).toObject();
                QString rowName= parameter.value("parametername").toString()+'('+parameter.value("parameterunit").toString()+"):";
                if(parameter.value("form").toBool())
                {//combobox
                    QComboBox *c = new QComboBox;
                    QJsonArray array = parameter.value("combobox").toArray();
                    int arrayCount = array.size();
                    for(int j=0; j<arrayCount; j++){
                        QString str = array.at(j).toString();
                        c->addItem(str);
                    }

                    formlayout->addRow(rowName, c);
                } else {//spinbox
                    QDoubleSpinBox *s = new QDoubleSpinBox;
                    s->setMaximum(parameter.value("max").toDouble());
                    s->setMinimum(parameter.value("min").toDouble());
                    s->setSingleStep(parameter.value("step").toDouble());

                    formlayout->addRow(rowName, s);
                }
            }

            groupBox->setLayout(formlayout);

            int width = groupBox->sizeHint().width();
            int height = groupBox->sizeHint().height();

            width/=10;
            if((width)%2 !=0)
                width = (width+1)*10;
            else
                width = (width+2)*10;

            height/=10;
            if((height)%2 !=0)
                height = (height+1)*10;
            else
                height = (height+2)*10;

            item->rect.setSize(QSize(width,height));
            item->resize(item->rect.size());
            item->updataArrowPosition();

            QGraphicsLinearLayout *linearlayout = new QGraphicsLinearLayout;
            linearlayout->setContentsMargins(2,5,5,2);
            linearlayout->setOrientation(Qt::Vertical);
            linearlayout->addItem(scene->addWidget(groupBox));

            item->setLayout(linearlayout);


            //输入箭头
            QJsonArray arrowinArray = module.value("arrowinlist").toArray();
            int arrowinnum = module.value("arrowinnum").toInt();
            for(int w =0; w<arrowinnum; w++)
            {
                QJsonObject arrowin = arrowinArray[w].toObject();
                ArrowIn *in = new ArrowIn();
                in->setParentItem(item);
                in->setPos(arrowin.value("pos.x").toDouble(), arrowin.value("pos.y").toDouble());

                item->listArrowIn.append(in);
            }

            //输出箭头
            QJsonArray arrowoutArray = module.value("arrowoutlist").toArray();
            int arrowoutnum = module.value("arrowoutnum").toInt();
            for(int m =0; m<arrowoutnum; m++)
            {
                QJsonObject arrowout = arrowoutArray[m].toObject();
                ArrowOut *out = new ArrowOut(item);
                out->setPos(arrowout.value("pos.x").toDouble(), arrowout.value("pos.y").toDouble());
                out->f_movePoint = arrowout.value("movepoint").toBool();
                out->arrowState = arrowout.value("arrowstate").toInt();
                if(out->arrowState != ArrowOut::Default)
                {
                    out->setParentItem(nullptr);
                }
                //out->upDateBeginPoint(out->scenePos());

                QJsonArray pointArray = arrowout.value("point").toArray();
                out->pointList.clear();
                int pointcount = pointArray.count();
                for(int k=0; k<pointcount; k+=2)
                {
                    out->pointList.append(QPointF(pointArray[k].toDouble(), pointArray[k+1].toDouble()));
                }
                out->updateLine();
                item->listArrowOut.append(out);

                connect(this,SIGNAL(signal_selectionAreaChange()),out,SLOT(slot_selectionAreaChange()));
                connect(scene,SIGNAL(signal_sceneMove(QPointF)),out,SLOT(slot_sceneMove(QPointF)));
            }
        }
    }

    if(object.contains("inmodule"))
    {
        QJsonArray moduleArray = object.value("inmodule").toArray();

        int modulecount = moduleArray.count();
        for(int i=0; i<modulecount; i++)
        {
            QJsonObject inmodule = moduleArray[i].toObject();
            InModule *in = new InModule;
            scene->addItem(in);
            in->setPos(inmodule.value("pos.x").toDouble(), inmodule.value("pos.y").toDouble());

            /***************************************************/
            QJsonObject arrowout = inmodule.value("arrowout").toObject();
            ArrowOut *out = in->arrowOut;
            out->setPos(arrowout.value("pos.x").toDouble(), arrowout.value("pos.y").toDouble());
            out->f_movePoint = arrowout.value("movepoint").toBool();
            out->arrowState = arrowout.value("arrowstate").toInt();
            if(out->arrowState != ArrowOut::Default)
            {
                out->setParentItem(nullptr);
            }

            QJsonArray pointArray = arrowout.value("point").toArray();
            out->pointList.clear();
            int pointcount = pointArray.count();
            for(int k=0; k<pointcount; k+=2)
            {
                out->pointList.append(QPointF(pointArray[k].toDouble(), pointArray[k+1].toDouble()));
            }
            out->updateLine();

            connect(this,SIGNAL(signal_selectionAreaChange()),out,SLOT(slot_selectionAreaChange()));
            connect(scene,SIGNAL(signal_sceneMove(QPointF)),out,SLOT(slot_sceneMove(QPointF)));
            /**************************************************/
        }
    }

    if(object.contains("outmodule"))
    {
        QJsonArray moduleArray = object.value("outmodule").toArray();

        int modulecount = moduleArray.count();
        for(int i=0; i<modulecount; i++)
        {
            QJsonObject outmodule = moduleArray[i].toObject();
            OutModule *out = new OutModule;
            scene->addItem(out);
            out->setPos(outmodule.value("pos.x").toDouble(), outmodule.value("pos.y").toDouble());
        }
    }

    QList<QGraphicsItem *> list = scene->items();
    for(QGraphicsItem *item :list)
    {
        if(item->type() == ItemBase::ConnectOutType)
        {
            ArrowOut *arrowOutItem = static_cast<ArrowOut*>(item);
            QList<QGraphicsItem *> collidinglist = item->collidingItems();
            for(QGraphicsItem *colliding :collidinglist)
            {
                if(colliding->type() == ItemBase::ConnectInType)
                {
                    arrowOutItem->connectArrow = static_cast<ArrowIn*>(colliding);
                    arrowOutItem->connectArrow->isConnect = true;
                    arrowOutItem->connectArrow->connectArrow = arrowOutItem;
                    arrowOutItem->connectArrow->hide();
                    break;
                }
            }
        }
    }
}

void MGraphicsView::getConfigureFile()
{



}

void MGraphicsView::wheelEvent(QWheelEvent* event)
{
    if(event->delta() > 0)
    {
        if(level < 19){
            level ++;
            zoomLevel = p[level]/p[level-1];
        } else
            zoomLevel = 1.0;
    }else
    {
        if(level > 0)
        {
            level --;
            zoomLevel = p[level]/p[level+1];
        } else
           zoomLevel = 1.0;
    }
    scale(zoomLevel,zoomLevel);

    emit signal_viewPostion(event->pos());
    emit signal_zoomLevel(p[level]);

    //QGraphicsView::wheelEvent(event); //拦截滚轮事件
}

void MGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton  && (itemAt(event->pos()) == nullptr))
    {
        isRubberBandDrag = true;
        p1 = mapToScene(event->pos());
        selectedRectItem->setRect(QRectF(p1,p1));
        scene->addItem(selectedRectItem);
    }

    if (event->buttons() & Qt::RightButton && (itemAt(event->pos()) == nullptr))
    {
        isScrollHandDrag = true;
        previousPoint = event->pos();
        viewport()->setCursor(Qt::ClosedHandCursor);
    }

    QGraphicsView::mousePressEvent(event);
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    emit signal_viewPostion(event->pos());

    if((event->buttons() & Qt::RightButton) && isScrollHandDrag)
    {
        QPointF currentPoint = event->pos();
        QTransform transform = viewportTransform();
        QScrollBar *verticalBar = verticalScrollBar();
        QScrollBar *horizontalBar = horizontalScrollBar();
        QRect view_rect = rect();
        QRectF scene_rect = sceneRect();
        qreal dx = currentPoint.x()-previousPoint.x();
        qreal dy = currentPoint.y()-previousPoint.y();

        int scale = static_cast<int>(dy*(verticalBar->maximum() - verticalBar->minimum())/
                    ((transform.m22()*scene_rect.height()-view_rect.height()))) ;
        verticalBar->setValue(verticalBar->value() - scale);

        scale = static_cast<int>(dx*(horizontalBar->maximum() - horizontalBar->minimum())/
                ((transform.m11()*scene_rect.width()-view_rect.width())));
        horizontalBar->setValue(horizontalBar->value() - scale);

        previousPoint = event->pos();
    }

    if ((event->buttons() & Qt::LeftButton) && isRubberBandDrag)
    {
        p2 = mapToScene(event->pos());
        if(p1.x()<p2.x() && p1.y()<p2.y())
        {
            selectedRectItem->setRect(QRectF(p1,p2));
        }else if(p1.x()>p2.x() && p1.y()>p2.y())
        {
            selectedRectItem->setRect(QRectF(p2,p1));
        }else if(p1.x()<p2.x() && p1.y()>p2.y())
        {
            selectedRectItem->setRect(p1.x(),p2.y(),p2.x()-p1.x(), p1.y()-p2.y());
        }else if(p1.x()>p2.x() && p1.y()<p2.y())
        {
            selectedRectItem->setRect(p2.x(),p1.y(),p1.x()-p2.x(), p2.y()-p1.y()) ;
        }

        scene->setSelectionArea(selectedRectItem->clipPath());
        emit signal_selectionAreaChange();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && isRubberBandDrag)
    {
        isRubberBandDrag = false;
        scene->removeItem(selectedRectItem);
    }

    if(event->button() == Qt::RightButton && isScrollHandDrag)
    {
        isScrollHandDrag = false;
        viewport()->setCursor(Qt::ArrowCursor);
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void MGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
//        cout<<"Copy";
//        QRect rect = mapFromScene(selectedRectItem->rect()).boundingRect();
//        QImage image(rect.size(),QImage::Format_ARGB32_Premultiplied);
//        image.fill(qRgba(255,255,255,255));
//        QPainter painter(&image);
//        painter.setRenderHint(QPainter::Antialiasing);
//        //scene()->clearSelection();
//        render(&painter,QRect(),rect);

//        QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指
//        //clipboard->setImage(image);
//        QMimeData *data = new QMimeData;
//        data->setImageData(image);
//        clipboard->setMimeData(data);
    }

    QGraphicsView::keyPressEvent(event);
}

void MGraphicsView::dropEvent(QDropEvent *event)
{
    if (!event->mimeData()->hasFormat("application/x-dnditemdata"))
        return;

    QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
    QDataStream dataStream(&itemData, QIODevice::ReadWrite);

    QVariantMap moduledata;
    QString modulename;
    QPoint offset;
    int  moduletype;

    dataStream>> offset>>modulename>>moduletype>>moduledata;
    QPointF point = mapToScene(event->pos() - offset);

    if(moduletype == ItemBase::InModuleType )
    {
        InModule *in = new InModule;
        scene->addItem(in);
        in->setPos(point);

        QPropertyAnimation *pAnimation = new QPropertyAnimation(in, "scale");
        pAnimation->setDuration(300);
        pAnimation->setStartValue(0);
        pAnimation->setEndValue(1);
        pAnimation->setEasingCurve(QEasingCurve::OutBounce);
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        connect(this,SIGNAL(signal_selectionAreaChange()),in->arrowOut,SLOT(slot_selectionAreaChange()));
        connect(scene,SIGNAL(signal_sceneMove(QPointF)),in->arrowOut,SLOT(slot_sceneMove(QPointF)));
        return;
    }

    if(moduletype == ItemBase::OutModuleType)
    {
        OutModule *out = new OutModule;
        scene->addItem(out);
        out->setPos(point);

        QPropertyAnimation *pAnimation = new QPropertyAnimation(out, "scale");
        pAnimation->setDuration(300);
        pAnimation->setStartValue(0);
        pAnimation->setEndValue(1);
        pAnimation->setEasingCurve(QEasingCurve::OutBounce);
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }

    if(moduletype == ItemBase::MoudleType)
    {
        ModuleBase *item = new ModuleBase;
        scene->addItem(item);
        item->setPos(point);
        item->setModuleName(modulename);
        //item->initMuduleWidget(moduledata);
        item->initInterface(moduledata.value("inputNum").toInt(),moduledata.value("outputNum").toInt());

        //////////////////////////////////////////////
        MGroupBox *groupBox = new MGroupBox(modulename);
        QFormLayout *formlayout = new QFormLayout;
        formlayout->setContentsMargins(2,22,2,2);
        formlayout->setVerticalSpacing(3);
        formlayout->setHorizontalSpacing(0);


        QJsonArray parameters = moduledata.value("parameters").toJsonArray();
        int parameterCount = parameters.size();

        for(int i = 0; i<parameterCount; i++)
        {
            QJsonObject parameter = parameters.at(i).toObject();
            QLabel *label = new QLabel(parameter.value("parametername").toString()+'('+
                                       parameter.value("parameterunit").toString()+"):");
            if(parameter.value("form").toBool())//combobox
            {
                QComboBox *c = new QComboBox;
                QJsonArray array = parameter.value("combobox").toArray();
                int arrayCount = array.size();
                for(int j=0; j<arrayCount; j++){
                    QString str = array.at(j).toString();
                    c->addItem(str);
                }

                formlayout->addRow(label, c);
            } else {//spinbox
                QDoubleSpinBox *s = new QDoubleSpinBox;
                s->setMaximum(parameter.value("max").toDouble());
                s->setMinimum(parameter.value("min").toDouble());
                s->setSingleStep(parameter.value("step").toDouble());

                formlayout->addRow(label, s);
            }
        }

        groupBox->setLayout(formlayout);

        //resize模块尺寸
        int width = groupBox->sizeHint().width();
        int height = groupBox->sizeHint().height();
        width/=10;
        if((width)%2 !=0)
            width = (width+1)*10;
        else
            width = (width+2)*10;
        height/=10;
        if((height)%2 !=0)
            height = (height+1)*10;
        else
            height = (height+2)*10;

        item->rect.setSize(QSize(width,height));
        item->resize(item->rect.size());
        item->updataArrowPosition();

        QGraphicsLinearLayout *linearlayout = new QGraphicsLinearLayout;
        linearlayout->setContentsMargins(2,5,5,2);
        linearlayout->setOrientation(Qt::Vertical);
        item->groupbox = groupBox;
        linearlayout->addItem(scene->addWidget(groupBox));

        item->setLayout(linearlayout);

        //出场动画
        QPropertyAnimation *pAnimation = new QPropertyAnimation(item, "scale");
        pAnimation->setDuration(500);
        pAnimation->setStartValue(0);
        pAnimation->setEndValue(1);
        pAnimation->setEasingCurve(QEasingCurve::OutElastic);
        pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MGraphicsView::dragEnterEvent(QDragEnterEvent *event)
{
    //event->setDropAction(Qt::MoveAction);
    event->accept();
    QWidget::dragEnterEvent(event);
}

void MGraphicsView::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget::dragMoveEvent(event);
}

MGraphicsView::~MGraphicsView()
{

}
