#include "authorization.h"

#include <QUrl>
#include <QUrlQuery>
#include <QString>
#include <QtWidgets/QWidget>


static QString const AUTHORIZATION_REDIRECT_URL("https://oauth.vk.com/blank.html");
static QString const APP_ID("4417636");
static QString const AUTHORIZATION_BASE_URL("https://oauth.vk.com/authorize");


Authorization::Authorization(QWidget *parent): QWebView(parent)
{
    connect(this,SIGNAL( urlChanged(QUrl) ), this, SLOT( handleChangedUrl(QUrl) ) ); 
}

void Authorization::authorize()
{
    QUrl authorizationUrl(AUTHORIZATION_BASE_URL);
    QUrlQuery query;
    query.addQueryItem(QString("client_id"),APP_ID);
    query.addQueryItem(QString("scope"),QString("audio"));
    query.addQueryItem(QString("redirect_uri"),AUTHORIZATION_REDIRECT_URL);
    query.addQueryItem(QString("display"),QString("mobile"));
    query.addQueryItem(QString("v"),QString("5.21"));
    query.addQueryItem(QString("response_type"),QString("token"));

    authorizationUrl.setQuery(query);
    load(authorizationUrl);
    show();
}

void Authorization::handleChangedUrl(QUrl const& newUrl)
{
    static QUrl const uAuthtorizationRedirectUrl(AUTHORIZATION_REDIRECT_URL);
    if(newUrl.matches(uAuthtorizationRedirectUrl,QUrl::RemoveFragment | QUrl::RemoveQuery))
    {
        if( newUrl.hasFragment() )
        {
            QUrlQuery query( newUrl.fragment() );

            QString accessToken = query.queryItemValue(QString("access_token"));
            QString userID = query.queryItemValue("user_id");
            emit success(accessToken,userID);
        }
        else
        {
            QUrlQuery query( newUrl.query() );
            QString error = query.queryItemValue(QString("error"));
            QString errorDescription = query.queryItemValue("eror_description");
            emit failure(error,errorDescription);
        }
        hide();
    }
}
