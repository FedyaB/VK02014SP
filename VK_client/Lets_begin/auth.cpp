#include "auth.h"
#include <QUrl>
#include <QUrlQuery>

static const QString RedirectUrl = QString("https://oauth.vk.com/blank.html");
static const QString APP_ID = QString("4417636");


Auth::Auth(QWidget *parent) :
    QWebView(parent)

{
    connect(this, SIGNAL(urlChanged(QUrl)), SLOT(handleUrlChange(QUrl)));
}

void Auth :: authorize()
{
    QUrl loginUrl(QString("https://oauth.vk.com/authorize"));
    QUrlQuery query;

    query.addQueryItem(QString("client_id"), APP_ID);
    query.addQueryItem(QString("scope"), QString("audio"));
    query.addQueryItem(QString("redirect_uri"), RedirectUrl);
    query.addQueryItem(QString("display"), QString("popup"));
    query.addQueryItem(QString("v"), QString("5.21"));
    query.addQueryItem(QString("response_type"), QString("token"));
    loginUrl.setQuery(query);
    load(loginUrl);
    show();

}
void Auth:: handleUrlChange(QUrl const & Url)
{
    static QUrl const redir(RedirectUrl);
    if(Url.matches(redir,QUrl::RemoveFragment|QUrl::RemoveQuery))
    {
        if(Url.hasFragment())
        {
            QUrlQuery query(Url.fragment());
            QString access_token = query.queryItemValue(QString("access_token"));
            QString user_id = query.queryItemValue(QString("user_id"));
            emit success(access_token,user_id);
        }
        else
        {
            QUrlQuery query(Url.query());
            QString access_err = query.queryItemValue(QString("error"));
            QString desc_id = query.queryItemValue(QString("error_description"));
            emit failure(access_err, desc_id);
        }
        hide();
    }
}



