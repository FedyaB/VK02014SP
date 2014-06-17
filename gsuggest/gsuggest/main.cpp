#include <QApplication>

#include "searchbox.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    SearchBox *searchEdit = new SearchBox;
    searchEdit->show();
    return app.exec();
}


/*#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

#include <QScopedPointer>
#include <QString>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QScopedPointer<QLabel> label(new QLabel(QString("Hello World!")));
    label -> show();
    return app.exec();
}*/
