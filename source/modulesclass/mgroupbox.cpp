#include "mgroupbox.h"

#define cout  qDebug() << __LINE__ << ":"

MGroupBox::MGroupBox(QWidget *parent):
    QGroupBox(parent)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);

    setAttribute(Qt::WA_TranslucentBackground, true);

}

MGroupBox::MGroupBox(const QString &title, QWidget *parent):
    QGroupBox(title, parent)
{
    setStyleSheet(QLatin1String("  QGroupBox {\
                                background-color: rgba(0,0,0,0);\
                                border: 0px solid gray;\
                                border-radius: 5px;\
                                margin-top: 1ex; \
                                font-size: 14px;\
                                font-weight: bold;\
                            }\
                            QGroupBox::title {\
                                subcontrol-origin: margin;\
                                subcontrol-position: top center;\
                                border-radius: 8px;\
                                padding: 2 2px;\
                                color: black;   \
                                background-color: YellowGreen;\
                            }"));
}

