#include "headers/mainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized(); // This sets the window to maximized
    w.setWindowTitle("ViewLog");
    w.setWindowIcon(QIcon(":logo.png"));
    w.show();
    return a.exec();
}
