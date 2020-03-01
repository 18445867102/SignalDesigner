#ifndef MDOCKTITLEWIDGET_H
#define MDOCKTITLEWIDGET_H

#include <QtWidgets>

class MDockTitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MDockTitleWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    QSize sizeHint() const { return minimumSizeHint(); }
    QSize minimumSizeHint() const;

    QPushButton *hold;
    QPushButton *close;
    QLinearGradient mlinear;
};


#endif // MDOCKTITLEWIDGET_H
