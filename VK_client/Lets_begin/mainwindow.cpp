#include "mainwindow.h"
#include "controls.h"

#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    Auth* auth = new Auth;
    connect(auth, SIGNAL(success(QString,QString)), SLOT(onSuccess(QString,QString)));
    stack = new QStackedWidget(this);
    stack -> insertWidget(0, auth);
    setCentralWidget(stack);
    auth -> authorize();
}

void MainWindow::onSuccess(const QString &token, const QString &user)
{
    QListView* view = new QListView;
    Controls* control = new Controls;
    model = new MusicModel(token, user, this);

    connect(control, SIGNAL (play()), model, SLOT(play()));
    connect(control, SIGNAL (pause()), model, SLOT(pause()));
    connect(control, SIGNAL (next()), model, SLOT(next()));
    connect(control, SIGNAL (previous()), model, SLOT(previous()));
    connect(model, SIGNAL (progress(float)), control, SLOT(setprogress(float)));
    QVBoxLayout* layout = new QVBoxLayout;
    layout -> addWidget(control);
    layout -> addWidget(view);
    QWidget* widget = new QWidget;
    widget -> setLayout(layout);

    view -> setModel(model);
    stack -> insertWidget(1, widget);
    stack -> setCurrentIndex(1);
}
