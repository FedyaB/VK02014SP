#include "musicmodel.h"

#include <QtWidgets/QApplication>
#include <QtNetwork/QNetworkAccessManager>
#include <QVector>
#include <QUrl>
#include <QUrlQuery>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QMediaContent>


void MusicModel::updateFiles()
{
    QUrl musicUrl(QString("https://api.vk.com/method/audio.get"));
    QUrlQuery query;

    query.addQueryItem(QString("owner_id"), user);
    query.addQueryItem(QString("need_user"), QString("0"));
    query.addQueryItem(QString("v"), QString("5.21"));
    query.addQueryItem(QString("access_token"), token);
    musicUrl.setQuery(query);

    networkManager.get(QNetworkRequest(musicUrl));
}

AudioFile parseAudioFile(QJsonObject const &obj)
{
    QString artist(obj[QString("artist")].toString());
    QString title(obj[QString("title")].toString());
    unsigned duration = obj[(QString("duration"))].toInt();
    QUrl url(obj[QString("url")].toString());

    qDebug() << url;

    AudioFile audio = { artist, title, duration, url };
    return audio;
}

void MusicModel :: parseReply(QNetworkReply *reply)
{
    if(!reply -> error())
    {
        files.clear();
        QByteArray responce(reply -> readAll());
        QJsonDocument document(QJsonDocument::fromJson(responce, 0));
        QJsonArray items(document.object().take(QString("response")).toObject().take(QString("items")).toArray());
        QJsonArray::const_iterator first(items.begin());
        QJsonArray::const_iterator last(items.end());
        for (QJsonArray::const_iterator it = first; it != last; ++it)
            files.push_back(parseAudioFile((*it).toObject()));
        insertRows(0, files.size());
    }

}

void MusicModel::statechangde(QMediaPlayer::MediaStatus bug)
{
    qDebug() <<  bug;
}

void MusicModel::positionchanged(qint64 position)
{
    emit progress(100.0 * position / player.duration());
}
MusicModel::MusicModel(QString const & token, QString const & user, QObject *parent) :
    QAbstractListModel(parent)
{
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(parseReply(QNetworkReply*)));
    this -> token = token;
    this -> user = user;

    player.setNotifyInterval(1000);
    connect(&player, SIGNAL(positionChanged(qint64)), SLOT(positionchanged(qint64)));
    connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), SLOT(statechangde(QMediaPlayer::MediaStatus)));
    updateFiles();}

int MusicModel::rowCount(const QModelIndex &) const
{
    return files.size();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        QString audio(files[index.row()].artist + QString(" ") + files[index.row()].title);
        return QVariant(audio);
    }
    return QVariant();
}

bool MusicModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    QVector<AudioFile> :: const_iterator first = files.begin();
    QVector<AudioFile> :: const_iterator last = files.end();
    for(QVector<AudioFile> :: const_iterator i = first; i != last; ++i)
    {
        QMediaContent media (i -> url);
        playlist.addMedia(media);
    }
    endInsertRows();
    playlist.setCurrentIndex(0);
    playlist.setPlaybackMode(QMediaPlaylist::Loop);
    player.setPlaylist(& playlist);
    //player.setMedia(QMediaContent(files.last().url));
    return true;
}

void MusicModel::play()
{
    player.play();
}

void MusicModel::pause()
{
    player.pause();
}

void MusicModel::next()
{
    playlist.next();
}

void MusicModel::previous()
{
    playlist.previous();
}
