#include <QtWidgets/QApplication>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    MainWindow clientWindow;
    clientWindow.show();

    return app.exec();
}
