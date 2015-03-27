#ifndef _YouTubeDataHandler_h
#define _YouTubeDataHandler_h

#include <QObject>
#include <QUrl>
#include <QRegExp>
#include <QHash>
#include <QStringList>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class YouTubeDataHandler : public QObject
{
    Q_OBJECT

    public:
        explicit YouTubeDataHandler(QObject *parent = 0);

        virtual void asyncStart(const QUrl& url);

    protected slots:
        void finished();

        void error(QNetworkReply::NetworkError);

    signals:
        void completed(const QUrl& url);

    private:
        QUrl url_;
        QNetworkAccessManager manager_;
        QNetworkReply *reply_;
};

#endif
