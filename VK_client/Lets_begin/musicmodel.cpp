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

void MusicModel :: onSuccess(QString const& token, QString const& user)
{
    this -> token = token;
    this -> user = user;
    updateFiles();
}
MusicModel :: ~MusicModel()
{
    auth->close();
    delete auth;
}

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
MusicModel::MusicModel(QObject *parent) :
    QAbstractListModel(parent)
{
    auth = new Auth;
    connect(auth, SIGNAL(success(QString,QString)), SLOT(onSuccess(QString, QString)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), SLOT(parseReply(QNetworkReply*)));
    auth -> authorize();
}

int MusicModel::rowCount(const QModelIndex &) const
{
    qDebug() << "size " << files.size();
    return files.size();
}

QVariant MusicModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        qDebug() << "row " << index.row();
        QString audio(files[index.row()].artist + QString(" ") + files[index.row()].title);
        return QVariant(audio);
    }
    return QVariant();
}

bool MusicModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}
