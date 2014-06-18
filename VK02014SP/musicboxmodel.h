#ifndef MUSICBOXMODEL_H
#define MUSICBOXMODEL_H

#include <QAbstractListModel>

#include "authorization.h"

#include <QUrl>
#include <QString>
#include <QVector>
#include <QVariant>


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

    explicit MusicBoxModel(QObject *parent = 0);
    ~MusicBoxModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

private:
        Authorization *authorizationBox;
        QString accessToken;
        QString userID;
        QVector<AudioFile> aFiles;
        QNetworkAccessManager networkManager;
signals:

public slots:

private slots:
           void onSuccess(QString const&, QString const&);
           void updateAudioFiles();
           void parseReply(QNetworkReply *reply);

};

#endif // MUSICBOXMODEL_H
