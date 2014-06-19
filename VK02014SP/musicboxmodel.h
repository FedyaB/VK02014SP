#ifndef MUSICBOXMODEL_H
#define MUSICBOXMODEL_H

#include <QAbstractListModel>

#include "authorization.h"

#include <QUrl>
#include <QString>
#include <QVector>
#include <QVariant>
#include <QMediaPlaylist>
#include <QMediaPlayer>


struct AudioFile
{
    QString artist;
    QString title;
    unsigned duration;
    QUrl url;
};

Q_DECLARE_METATYPE(AudioFile)

class MusicBoxModel : public QAbstractListModel
{
    Q_OBJECT
public:

    explicit MusicBoxModel(QString const& accessToken, QString const& userID, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());


private:
        QString accessToken;
        QString userID;
        QVector<AudioFile> aFiles;
        QNetworkAccessManager networkManager;
        QMediaPlaylist musicPlaylist;
        QMediaPlayer musicPlayer;

signals:
        void progressAudio(float);

public slots:
        void playAudio();
        void pauseAudio();
        void nextAudio();
        void previousAudio();

private slots:
           void updateAudioFiles();
           void parseReply(QNetworkReply *reply);
           void checkAudioPosition(qint64);

};

#endif // MUSICBOXMODEL_H
