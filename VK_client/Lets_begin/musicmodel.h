#ifndef MUSICMODEL_H
#define MUSICMODEL_H
#include <QUrl>
#include <QString>
#include <QAbstractListModel>
#include <QMetaType>
#include "auth.h"

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
    explicit MusicModel(QObject *parent = 0);
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt :: DisplayRole) const;
    bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    ~MusicModel();
private:
    Auth *auth;
    QString token;
    QString user;
    QVector<AudioFile> files;
    QNetworkAccessManager networkManager;
signals:

public slots:

private slots:
    void onSuccess(QString const& token, QString const& user);
    void updateFiles();
    void parseReply(QNetworkReply *reply);
};

#endif // MUSICMODEL_H
