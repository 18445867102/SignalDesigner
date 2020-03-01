#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("MySoft");
    QCoreApplication::setApplicationName("Module Controller");

    QApplication a(argc, argv);

    MainWindow w;
    QPropertyAnimation *animation = new QPropertyAnimation(&w,"windowOpacity");
    animation->setDuration(350);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    w.show();

    return a.exec();
}
