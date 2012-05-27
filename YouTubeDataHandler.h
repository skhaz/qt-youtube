#ifndef YOUTUBEDATAHANDLER_H
#define YOUTUBEDATAHANDLER_H

#include "AbstractDataHandler.h"
#include <QtNetwork>

class YouTubeDataHandler : public AbstractDataHandler
{
    Q_OBJECT

public:
    explicit YouTubeDataHandler(QObject *parent = 0);

    virtual void asyncStart(const QUrl& url);

protected slots:
    void finished();

    void error(QNetworkReply::NetworkError);

private:
    QUrl m_url;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
};

#endif // YOUTUBEDATAHANDLER_H
