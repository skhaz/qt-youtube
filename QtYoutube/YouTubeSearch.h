#ifndef _YouTubeSearch_h
#define _YouTubeSearch_h

#include <QObject>
#include <QtNetwork>
#include <QDesktopServices>
// #include <Q>
#include <QtXml>

#include "Media.h"

#define CACHE_SIZE 128 * 1024 * 1024
#define YOUTUBE_API "https://gdata.youtube.com/feeds/api/videos?max-results=25&alt=rss&"
#define YTIMG "http://i%1.ytimg.com/vi/%2/hqdefault.jpg"

class YouTubeSearch : public QObject
{
        Q_OBJECT

    public:
        explicit YouTubeSearch(QObject *parent = 0);

        void setContext(QObject *context);

    public slots:
        void search(const QString& query);

    protected slots:
        void readyRead();

        void error(QNetworkReply::NetworkError error);

        void finished();

    private:
        Q_DISABLE_COPY(YouTubeSearch)

        QXmlStreamReader xml;
        QNetworkAccessManager manager;
        QNetworkDiskCache cache;
        QNetworkReply *reply;
        QObject *context;
        QList<QObject *> objects;
};

#endif
