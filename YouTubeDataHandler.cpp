#include "YouTubeDataHandler.h"

#include <QRegExp>
#include <QHash>
#include <QStringList>



YouTubeDataHandler::YouTubeDataHandler(QObject *parent)
: AbstractDataHandler(parent)
{
}

void YouTubeDataHandler::asyncStart(const QUrl& url)
{
    reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeDataHandler::finished()
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        // This block is based on file youtube.lua from VideoLAN project
        QHash<int, QString> stream_map;
        QRegExp re("\"url_encoded_fmt_stream_map\": \"([^\"]*)\"", Qt::CaseInsensitive, QRegExp::RegExp2);
        QRegExp urls("itag=(\\d+),url=(.*)");

        if (re.indexIn(reply->readAll()) != -1) {
            QString result = re.cap(1);
            foreach (QString line, result.split("\\u0026")) {
                if (urls.indexIn(QUrl::fromPercentEncoding(line.toAscii())) != -1) {
                    stream_map[urls.cap(1).toInt()] = urls.cap(2);
                }
            }

            // XXX hardcoded
            QList<QString> values = stream_map.values();
            emit completed(values.first());
        }
    }
}

void YouTubeDataHandler::error(QNetworkReply::NetworkError error)
{
}
