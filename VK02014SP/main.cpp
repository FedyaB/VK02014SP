#include <QtWidgets/QApplication>
#include "authorization.h"

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    Authorization AuthorizationBox;
    AuthorizationBox.show();
    return app.exec();
}
