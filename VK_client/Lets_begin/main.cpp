#include <QtWidgets/QApplication>
#include "musicmodel.h"
#include <QtWidgets/QListView>

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    MusicModel music;
    QListView view;
    view.setModel(&music);
    view.show();
    return app.exec();

}
