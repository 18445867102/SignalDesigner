#include "mcombobox.h"

#define cout  qDebug() << __LINE__ << ":"

MCombobox::MCombobox(QWidget *parent):QComboBox(parent)
{
    this->setStyleSheet("QComboBox {\
                        border: 0px solid gray;\
                        border-radius: 0px;\
                        padding: 1px 18px 1px 3px;\
                        min-width: 6em;}\
                    QComboBox:editable {\
                        background: white;}\
                    QComboBox:on { \
                        padding-top: 3px;\
                        padding-left: 4px;}\
                    QComboBox::drop-down {\
                        subcontrol-origin: padding;\
                        subcontrol-position: top right;\
                        width: 12px;\
                        border-left-width: 1px;\
                        border-left-color: white;\
                        border-left-style: solid; \
                        border-top-right-radius: 0px;\
                        border-bottom-right-radius: 0px;\
                    }\
                    QComboBox::down-arrow {\
                        image: url(:/icon/triangle_down_16px_1189852_easyicon.net.ico);}\
                    QComboBox::down-arrow:on {\
                        top: 1px;\
                        left: 1px;}\
                    QComboBox QAbstractItemView {\
                        border: 0px solid darkgray;\
                        selection-background-color: lightgray;\
                    }");
//    this->setBackgroundRole(QPalette::AlternateBase);
//    this->setForegroundRole(QPalette::AlternateBase);
//    this->setEditable(true);
//    this->setInsertPolicy(QComboBox::NoInsert);

}

void MCombobox::keyPressEvent(QKeyEvent *event)
{
    emit signal_ComboBoxkeyPress(event->key(),this);
    QComboBox::keyPressEvent(event);
}
