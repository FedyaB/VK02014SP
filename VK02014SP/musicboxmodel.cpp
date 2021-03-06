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
#include <QMediaContent>


MusicBoxModel::MusicBoxModel(QString const& accessToken, QString const& userID, QObject *parent) :
    QAbstractListModel(parent)
{
     connect(&networkManager, SIGNAL( finished(QNetworkReply*) ), this, SLOT( parseReply(QNetworkReply*)) );

     musicPlayer.setNotifyInterval(1000);
     connect(&musicPlayer, SIGNAL( positionChanged(qint64) ), this, SLOT(checkAudioPosition(qint64)) );

     this -> accessToken = accessToken;
     this -> userID = userID;
     this -> updateAudioFiles();

     musicPlaylist.setPlaybackMode(QMediaPlaylist::Loop);
     musicPlaylist.setCurrentIndex(0);
}

bool MusicBoxModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
        QVector<AudioFile>::const_iterator begin = aFiles.begin();
        QVector<AudioFile>::const_iterator end = aFiles.end();
        for(QVector<AudioFile>::const_iterator iter = begin; iter != end; ++iter)
        {
            QMediaContent media(iter -> url);
            musicPlaylist.addMedia(media);
            //qDebug() << iter -> url;
        }
    endInsertRows();
    musicPlayer.setPlaylist(&musicPlaylist);
    return true;
}

void MusicBoxModel::playAudio()
{
   musicPlayer.play();
}

void MusicBoxModel::pauseAudio()
{
   musicPlayer.pause();
}

void MusicBoxModel::previousAudio()
{
   musicPlaylist.previous();
}

void MusicBoxModel::nextAudio()
{
   musicPlaylist.next();
}

void MusicBoxModel::checkAudioPosition(qint64 position)
{
   emit progressAudio(100.0*position/musicPlayer.duration());
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
    //qDebug() << "size " << aFiles.size();
    return aFiles.size();
}

////////////////////
QVariant MusicBoxModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        //qDebug() << "row " << index.row();
        QString audio(aFiles[index.row()].artist + QString(" ") + aFiles[index.row()].title);
        return QVariant(audio);
    }
    return QVariant();
}
/////////////////////
