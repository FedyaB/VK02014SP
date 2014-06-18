#include <QApplication>
#include "auth.h"

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    Auth auth;
    auth.show();
    return app.exec();
}
