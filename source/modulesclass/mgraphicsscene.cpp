#include "mgraphicsscene.h"
#define cout  qDebug() << __LINE__ << ":"

MGraphicsScene::MGraphicsScene()
{

}

void MGraphicsScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        foreach (auto *item, selectedItems())
        {
            if(item->type() == ItemBase::MoudleType)
            {
                QVector<ArrowIn*> in = static_cast<ModuleBase*>(item)->listArrowIn;
                for(ArrowIn *arrowin :in)
                {
                    arrowin->updateStatus();
                }

                QVector<ArrowOut*> out = static_cast<ModuleBase*>(item)->listArrowOut;
                for(ArrowOut *arrowout :out)
                {                 
                    arrowout->updateStatus();
                }

                delete item;
            }

            if(item->type() == ItemBase::InModuleType)
            {
                static_cast<InModule*>(item)->arrowOut->updateStatus();
                delete item;
            }

            if(item->type() == ItemBase::OutModuleType)
            {
                static_cast<OutModule*>(item)->arrowin->updateStatus();
                delete item;
            }
        }

        foreach (auto *item, selectedItems())
        {
            if(item->type() == ItemBase::ConnectOutType)
            {
                ArrowOut* arrowOut = static_cast<ArrowOut*>(item);
                arrowOut->updateDefault();
            }
        }
    }

    QGraphicsScene::keyPressEvent(event);
}

void MGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void MGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{  
    emit signal_sceneMove(event->scenePos());

    QGraphicsScene::mouseMoveEvent(event);
}

//void MGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
//{
//    QGraphicsScene::mouseReleaseEvent(event);
//}
