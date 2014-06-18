#ifndef GSUGGESTION_H
#define GSUGGESTION_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>

class QLineEdit;
class QTreeWidget;
class QTimer;
class QEvent;
class QStringList;

class GSuggestion : public QObject
{
Q_OBJECT

public:
    GSuggestion(QLineEdit *parent = 0);
    ~GSuggestion();
    bool eventFilter(QObject *focusedObject, QEvent *ev);
    void showSuggestions(const QStringList &choices);

public slots:
    void choosingDone();
    void preventSuggest();
    void autoSuggest();
    void handleNetworkData(QNetworkReply *reply);

private:
    QLineEdit *editor;
    QTreeWidget *popup;
    QTimer *timer;
    QNetworkAccessManager networkManager;
};

#endif // GSUGGESTION_H
