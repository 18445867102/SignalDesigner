#ifndef MGraphicsView_H
#define MGraphicsView_H

#include <QtWidgets>

#include "mgraphicsscene.h"
#include "itembase.h"
#include "modulebase.h"
#include "arrowin.h"
#include "arrowout.h"
#include "inmodule.h"
#include "outmodule.h"
#include "mline.h"
#include "mgroupbox.h"

class MGraphicsScene;
class MGraphicsView :public QGraphicsView
{
    Q_OBJECT
public:
    explicit MGraphicsView(QWidget *parent = nullptr);
    ~MGraphicsView();
    void saveProjectFile();
    void openProjectFile();
    void getConfigureFile();

    bool hasPath;
    bool isSave;

    QString filePath;
    QFileInfo fileinfo;
    QFileInfo moduleDatafileinfo;

    qreal   p[20] = {0.30,0.4,0.50,0.6,0.8,1.0,1.25,1.5,1.75,2.0,2.4,2.8,3.2,3.6,4.0,4.5,5.0,6.0,7,8};
    qreal   zoomLevel;
    int     level;

    MGraphicsScene *scene;
    QJsonObject moduleData;
protected:
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);

    QPointF previousPoint;
    QPointF p1,p2;
    bool    isRubberBandDrag;
    bool    isScrollHandDrag;
    QGraphicsRectItem *selectedRectItem;

signals:
    void signal_selectionAreaChange();
    void signal_zoomLevel(qreal);
    void signal_viewPostion(QPoint);

public slots:
};

#endif // MGraphicsView_H
