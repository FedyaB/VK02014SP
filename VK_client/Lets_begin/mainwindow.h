#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QListView>
#include "auth.h"
#include "musicmodel.h"
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);


private:
    MusicModel* model;
    QStackedWidget* stack;

signals:
public slots:

private slots:
    void onSuccess(QString const & token, QString const & user);

};

#endif // MAINWINDOW_H
