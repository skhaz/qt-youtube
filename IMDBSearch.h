#ifndef IMDBSearch_h
#define IMDBSearch_h

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDeclarativeContext>

#include <QtDebug>

#define IMDB_API "http://sg.media-imdb.com/suggests"
#define THUMB_TAG "._V1_."
#define THUMB_DEFAULT_SIZE "._V1._SY317_."



class IMDBSearch : public QObject
{
    public:
        explicit IMDBSearch(QObject *parent = 0);

        void setContext(QDeclarativeContext *context);

    public slots:
        void search(const QString& query);

    protected slots:
        void finished();

        void error(QNetworkReply::NetworkError);

    private:
        Q_OBJECT

        QNetworkAccessManager _manager;
        QNetworkReply *_reply;
        QDeclarativeContext *_context;
        QList<QObject *> _objects;
};

#endif
