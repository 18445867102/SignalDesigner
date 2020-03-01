#-------------------------------------------------
#
# Project created by QtCreator 2019-03-03T15:12:14
#
#-------------------------------------------------

QT   += core gui quick
QT   += serialport
QT   += quickwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The name of Application
TARGET = SignalDesigner

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#lambda
CONFIG  += C++11

SOURCES += \
    mclass/delegate.cpp \
    mclass/mcombobox.cpp \
    mclass/mlistwidget.cpp \
    mclass/moduledata.cpp \
    mclass/mtablewidget.cpp \
    mclass/qflowlayout.cpp \
    mclass/spinboxdelegate.cpp \
    mclass/mstatusbar.cpp \
    mclass/mtoolbar.cpp \
    modulesclass/arrowin.cpp \
    modulesclass/arrowout.cpp\
    modulesclass/mlabel.cpp \
    modulesclass/itembase.cpp \
    modulesclass/dragwidget.cpp \
    modulesclass/mgraphicsscene.cpp \
    modulesclass/mgraphicsview.cpp \
    modulesclass/mline.cpp \
    modulesclass/modulebase.cpp \
    modulesclass/modulewidget.cpp \
    modulesclass/inmodule.cpp \
    modulesclass/outmodule.cpp \
    serialport/mserial.cpp \
    dockwidget/mdockwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    dockwidget/mdocktitlewidget.cpp \
    tabwidget/mtabwidget.cpp \
    tabwidget/mtabbar.cpp \
    mclass/parameterdata.cpp \
    serialport/mserialwidget.cpp \
    mboardeditor.cpp \
    modulesclass/mgroupbox.cpp

HEADERS  += \
    mclass/delegate.h \
    mclass/mcombobox.h \
    mclass/mlistwidget.h \
    mclass/moduledata.h \
    mclass/mtablewidget.h \
    mclass/qflowlayout.h \
    mclass/spinboxdelegate.h \
    mclass/mstatusbar.h \
    mclass/mtoolbar.h \
    modulesclass/arrowin.h \
    modulesclass/arrowout.h \
    modulesclass/dragwidget.h \
    modulesclass/mgraphicsscene.h \
    modulesclass/mgraphicsview.h \
    modulesclass/mline.h \
    modulesclass/modulebase.h \
    modulesclass/modulewidget.h \
    modulesclass/itembase.h \
    modulesclass/mlabel.h \
    modulesclass/inmodule.h \
    modulesclass/outmodule.h \
    serialport/mserial.h \
    dockwidget/mdockwidget.h \
    mainwindow.h \
    dockwidget/mdocktitlewidget.h \
    tabwidget/mtabwidget.h \
    tabwidget/mtabbar.h \
    mclass/parameterdata.h \
    serialport/mserialwidget.h \
    mboardeditor.h \
    modulesclass/mgroupbox.h

FORMS += \
        mainwindow.ui\
    serialport/mserialwidget.ui \
    mboardeditor.ui

RC_FILE =rcfile.rc

RESOURCES += \
    resource.qrc \
    qml.qrc

DISTFILES +=

DESTDIR = ./app/

release {

}
debug {

}
