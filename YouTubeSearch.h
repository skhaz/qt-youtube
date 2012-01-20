#ifndef _YouTubeSearch_h
#define _YouTubeSearch_h

#include <QObject>
#include <QtNetwork>
#include <QDesktopServices>
#include <QtXmlPatterns>

#include "Media.h"
#include "MediaModel.h"

#define CACHE_SIZE 128 * 1024 * 1024
#define YOUTUBE_API "https://gdata.youtube.com/feeds/api/videos?"
#define YTIMG "http://i%1.ytimg.com/vi/%2/hqdefault.jpg"



class YouTubeSearch : public QObject
{
    public:
        explicit YouTubeSearch(QObject *parent = 0);

        ~YouTubeSearch();

        MediaModel* model() const;

    public slots:
        void search(const QString& query);

    protected slots:
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void error(QNetworkReply::NetworkError code);
        void searchfinished();
        void pageFinished();

    private:
        Q_OBJECT
        Q_DISABLE_COPY(YouTubeSearch)

        QNetworkAccessManager manager;
        QNetworkDiskCache cache;
        QNetworkReply *m_search_reply;
        MediaModel *m_model;
};

#endif
