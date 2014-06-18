#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QtWebKitWidgets/QWebView>

class QWidget;
class QUrl;
class QString;

class Authorization : public QWebView
{
    Q_OBJECT
public:
    Authorization(QWidget *parent = 0);
    void authorize();

signals:
        void success(QString const& token, QString const& userID);
        void failure(QString const& error, QString const& errorDescription);

private slots:
        void handleChangedUrl(QUrl const& newUrl);

};

#endif // AUTHORIZATION_H
