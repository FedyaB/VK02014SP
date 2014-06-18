#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include "musicboxmodel.h"

int main(int argc, char** argv)
{
    QApplication app(argc,argv);
    MusicBoxModel musicBox;
    QListView viewBox;
    viewBox.setModel(&musicBox);
    viewBox.show();

    return app.exec();
}
