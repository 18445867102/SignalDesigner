#include "mdocktitlewidget.h"
#define cout  qDebug() << __LINE__ << ":"
MDockTitleWidget::MDockTitleWidget(QWidget *parent) :
    QWidget(parent)
{
    hold = new QPushButton(QIcon(":/icon/pin_24px_1128095_easyicon.net.ico"),"",this);
    hold->setFlat(true);
    hold->setStyleSheet("QPushButton"
    "{border: 0px ;"
    "border-radius: 4px;}"
    "QPushButton:hover"
    "{background-color:rgb(220, 248, 255);}");
    close = new QPushButton(QIcon(":/icon/close.ico"), "", this);
    close->setFlat(true);
    close->setStyleSheet("QPushButton"
    "{border: 0px ;"
    "border-radius: 5px;}");

    mlinear.setStart(size().width(),0);
    mlinear.setFinalStop(size().width(),size().height());
    mlinear.setColorAt(0.0, QColor("Gainsboro"));
    mlinear.setColorAt(0.5, QColor("WhiteSmoke"));
    mlinear.setColorAt(1.0, QColor("Gainsboro"));


    connect(close, &QPushButton::clicked,
            [=]()mutable
            {
                QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
                dw->close();
                //dw->setFloating(!dw->isFloating());
            });
}

void MDockTitleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    QRect rect = this->rect().adjusted(0,2,0,0);

    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);

    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar) {
        QSize s = rect.size();
        s.transpose();
        rect.setSize(s);

        painter.translate(rect.left(), rect.top() + rect.width());
        painter.rotate(-90);
        painter.translate(-rect.left(), -rect.top());
    }

    painter.setBrush(mlinear);
    painter.drawRect(this->rect().adjusted(-1,-1,0,0));
    painter.setPen(Qt::black);
    painter.drawText(QRect(5,0,size().width(),size().height()), Qt::AlignLeft|Qt::AlignVCenter, parentWidget()->windowTitle());
}

void MDockTitleWidget::resizeEvent(QResizeEvent *event)
{
    int width = event->size().width();
    close->move(width - 24,2);
    hold->move(width - 46,2);
}

QSize MDockTitleWidget::minimumSizeHint() const
{
    QDockWidget *dw = qobject_cast<QDockWidget*>(parentWidget());
    Q_ASSERT(dw != 0);
    QSize result(200, 22);
    if (dw->features() & QDockWidget::DockWidgetVerticalTitleBar)
        result.transpose();
    return result;
}
