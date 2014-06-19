#include "mainwindow.h"
#include "controlbar.h"

#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), musicBox(0)
{
    Authorization* authorizationBox = new Authorization;

    connect(authorizationBox, SIGNAL(success(QString, QString) ), this, SLOT( onSuccess(QString,QString) ));

    this -> stack = new QStackedWidget(this);
    stack -> insertWidget(0, authorizationBox);
    this -> setCentralWidget(stack);

    authorizationBox -> authorize();

}

void MainWindow::onSuccess(const QString &accessToken, const QString &userID)
{
    QListView* listView = new QListView;
    ControlBar* controlBar = new ControlBar;
    musicBox = new MusicBoxModel(accessToken, userID, this);

    connect(controlBar, SIGNAL( playAudio()), musicBox, SLOT( playAudio() ) );
    connect(controlBar, SIGNAL( pauseAudio()), musicBox, SLOT( pauseAudio() ) );
    connect(controlBar, SIGNAL( previousAudio()), musicBox, SLOT( previousAudio() ) );
    connect(controlBar, SIGNAL( nextAudio()), musicBox, SLOT( nextAudio() ) );
    connect(musicBox, SIGNAL( progressAudio(float) ), controlBar, SLOT( updateProgressBar(float) ) );

    listView ->  setModel(musicBox);

    QVBoxLayout* playerBoxLayout = new QVBoxLayout;
    playerBoxLayout -> addWidget(controlBar);
    playerBoxLayout -> addWidget(listView);

    QWidget* playerBox = new QWidget;
    playerBox -> setLayout(playerBoxLayout);

    stack -> insertWidget(1, playerBox);
    stack -> setCurrentIndex(1);
}
