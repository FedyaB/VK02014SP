#ifndef MUSICMODEL_H
#define MUSICMODEL_H
#include <QUrl>
#include <QString>
#include <QAbstractListModel>
#include <QMetaType>
#include "auth.h"
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

class MusicModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit MusicModel(QString const & token, QString const & user, QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt :: DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());


private:
    QString token;
    QString user;
    QVector<AudioFile> files;
    QNetworkAccessManager networkManager;
    QMediaPlaylist playlist;
    QMediaPlayer player;

signals:
    void progress(float);

public slots:
    void play();
    void pause();
    void next();
    void previous();

private slots:
    void updateFiles();
    void parseReply(QNetworkReply *reply);
    void statechangde(QMediaPlayer :: MediaStatus);
    void positionchanged(qint64);
};


#endif // MUSICMODEL_H
