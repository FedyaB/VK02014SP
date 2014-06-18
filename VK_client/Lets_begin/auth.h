#ifndef AUTH_H
#define AUTH_H

#include <QtWebKitWidgets/QWebView>

class QUrl;
class Auth : public QWebView
{
    Q_OBJECT
public:
    explicit Auth(QWidget *parent = 0);
private slots:

    void handleUrlChange(QUrl const & Url);
signals:
  void success(QString const & token, QString const & user);
  void failure(QString const & err, QString const & desc);
};

#endif // AUTH_H
