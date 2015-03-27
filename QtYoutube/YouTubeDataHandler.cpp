#include "YouTubeDataHandler.h"

YouTubeDataHandler::YouTubeDataHandler(QObject *parent)
: QObject(parent)
{
}

void YouTubeDataHandler::asyncStart(const QUrl& url)
{
    reply_ = manager_.get(QNetworkRequest(url));
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(reply_, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeDataHandler::finished()
{
    int statusCode = reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        // This block is based on file youtube.lua from VideoLAN project
        const static QRegExp re("\"url_encoded_fmt_stream_map\": \"([^\"]*)\"", Qt::CaseInsensitive, QRegExp::RegExp2);
        const static QRegExp urls("itag=(\\d+),url=(.*)");
        QHash<int, QString> stream_map;

        if (re.indexIn(reply_->readAll()) != -1) {
            QString result = re.cap(1);
            foreach (QString line, result.split("\\u0026")) {
                if (urls.indexIn(QUrl::fromPercentEncoding(line.toLocal8Bit())) != -1) {
                    stream_map[urls.cap(1).toInt()] = urls.cap(2);
                }
            }

            // XXX hardcoded
            QList<QString> values = stream_map.values();
            qDebug() << stream_map;
            emit completed(values.last());
        }
    }
}

void YouTubeDataHandler::error(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}
