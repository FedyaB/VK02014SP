#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QListView>
#include <QStackedWidget>

#include "authorization.h"
#include "musicboxmodel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

private:
        MusicBoxModel* musicBox;
        QStackedWidget* stack;
signals:

public slots:

private slots:
        void onSuccess(QString const& accessToken, QString const& userID);
};

#endif // MAINWINDOW_H
