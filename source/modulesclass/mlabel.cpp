#include "mlabel.h"

MLabel::MLabel(QPixmap pixmap,QWidget *parent) : QLabel(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setPixmap(pixmap);

    moduleName = "<no name>";
    moduleNum = 1;
    moduleType = 1;
}
