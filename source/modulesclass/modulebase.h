#ifndef ModuleBase_H
#define ModuleBase_H

#include <QtWidgets>
#include "modulewidget.h"
#include "mgraphicsview.h"
#include "itembase.h"
#include "arrowout.h"
#include "arrowin.h"
#include "mgroupbox.h"

class ArrowOut;
class ArrowIn;

struct _p{
    _p(int p_begin = 10, int p_space = 10){
        this->begin = p_begin;
        this->space = p_space;
    }

    int begin;
    int space;
};

class ModuleBase : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit ModuleBase(QGraphicsItem *parent = nullptr, QRect rect = QRect(0,0,100,50));
    ~ModuleBase();
   // void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setModuleName(QString);
    void initInterface(int,int);
    void updataArrowPosition();
    _p updata_p(int num);

    MGroupBox *groupbox;
    QString moduleName;
    QVector<ArrowOut*> listArrowOut;
    QVector<ArrowIn*>  listArrowIn;

    QRectF  rect;
protected:
    QRectF boundingRect() const override;
    int type() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *moduleWidget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    bool f_hoverEnter;  //鼠标是否进入边界矩形标志
    bool f_changeRec;   //是否改变边界矩形标志
    int p_cursorSet;   //边界矩形四个角小矩形的1/2边长
    _p p;

    QLinearGradient mlinear;
    QPen mPen;

signals:
    void signal_itemMove(QPointF);

public slots:

};

#endif // ModuleBase_H
