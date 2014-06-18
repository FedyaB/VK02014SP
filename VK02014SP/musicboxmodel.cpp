#include "musicboxmodel.h"

#include <QtWidgets/QApplication> //+++

#include <QtNetwork/QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QString>
#include <QUrlQuery>

MusicBoxModel::MusicBoxModel(QObject *parent) :
    QAbstractListModel(parent)
{
     authorizationBox = new Authorization;

     connect(authorizationBox, SIGNAL( success(QString,QString) ), this, SLOT( onSuccess(QString,QString) ) );
     connect(&networkManager, SIGNAL( finished(QNetworkReply*) ), this, SLOT( parseReply(QNetworkReply*)) );

     /////////////////////////////////
     authorizationBox -> authorize();
}

MusicBoxModel::~MusicBoxModel()
{
    authorizationBox -> close();//+++
    delete authorizationBox;
}

void MusicBoxModel::onSuccess(QString const& accessToken,QString const& userID)
{
    this -> accessToken = accessToken;
    this -> userID = userID;
    this -> updateAudioFiles();
}

bool MusicBoxModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

void MusicBoxModel::updateAudioFiles()
{
    static QString const API_METHOD_AUDIO_GET_BASE_URL("https://api.vk.com/method/audio.get");

    ///!!!!!!!!
    /*QUrl resultUrl(API_METHOD_BASE_URL);
    resultURL.arg("audio.get");*/

    QUrl resultUrl(API_METHOD_AUDIO_GET_BASE_URL);

    QUrlQuery query;
    query.addQueryItem(QString("owner_id"), this -> userID);
    query.addQueryItem(QString("need_user"),QString("0"));
    query.addQueryItem(QString("v"),QString("5.21"));

    query.addQueryItem(QString("access_token"), this -> accessToken);

    resultUrl.setQuery(query);

    //qDebug() << resultUrl.toString();

    networkManager.get(QNetworkRequest(resultUrl));
}

AudioFile parseAudioFile(QJsonObject const &object)
{
   QString artist( object[QString("artist")].toString() );
   QString title( object[QString("title")].toString() );
   unsigned duration( object[QString("duration")].toInt() );
   QString url( object[QString("url")].toString() );

   AudioFile audio = { artist, title, duration, url };
   return audio;
}

void MusicBoxModel::parseReply(QNetworkReply *reply)
{
   if(!reply -> error())
   {
       aFiles.clear();
       QByteArray response( reply -> readAll() );
       QJsonDocument document( QJsonDocument::fromJson(response, 0) );

       /////////////////////////////////////
       QJsonArray items(document.object().take(QString("response")).toObject().take(QString("items")).toArray());
       QJsonArray::const_iterator first(items.begin());
       QJsonArray::const_iterator last(items.end());
       for (QJsonArray::const_iterator it = first; it != last; ++it)
            aFiles.push_back( parseAudioFile((*it).toObject()) );
       /////////////////////////////

       insertRows(0, aFiles.size());
       //qApp -> quit();//+++
   }
}

int MusicBoxModel::rowCount(const QModelIndex &) const
{
    qDebug() << "size " << aFiles.size();
    return aFiles.size();
}

////////////////////
QVariant MusicBoxModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        qDebug() << "row " << index.row();
        QString audio(aFiles[index.row()].artist + QString(" ") + aFiles[index.row()].title);
        return QVariant(audio);
    }
    return QVariant();
}
/////////////////////
