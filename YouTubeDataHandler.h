#ifndef YouTubeDataHandler_h
#define YouTubeDataHandler_h

#include "AbstractDataHandler.h"

#include <QtNetwork>



class YouTubeDataHandler : public AbstractDataHandler
{
    public:
        explicit YouTubeDataHandler(QObject *parent = 0);

        virtual void asyncStart(const QUrl& url);

    protected slots:
        void finished();

        void error(QNetworkReply::NetworkError);

    private:
        Q_OBJECT

        QUrl m_url;

        QNetworkAccessManager manager;
        QNetworkReply *reply;
};

#endif
