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
    qDebug(">>>>> asyncStart");

    reply = manager.get(QNetworkRequest(url));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeDataHandler::finished()
{
    QString data = reply->readAll();
    QUrl url;

    // This block is based on file youtube.lua from VideoLAN project
    QHash<int, QString> stream_map;
    QRegExp re("\"url_encoded_fmt_stream_map\": \"([^\"]*)\"", Qt::CaseInsensitive, QRegExp::RegExp2);
    QRegExp urls("itag=(\\d+),url=(.*)");

    if (re.indexIn(data) != -1) {
        QString result = re.cap(1);
        foreach (QString line, result.split("\\u0026")) {
            if (urls.indexIn(QUrl::fromPercentEncoding(line.toAscii())) != -1) {
                stream_map[urls.cap(1).toInt()] = urls.cap(2);
            }
        }

        // XXX hardcoded
        QList<QString> values = stream_map.values();
        url = values.first();
    }

    QRegExp title("<meta property=\"og:title\" content=\"([^\"]*)\">");
    if (title.indexIn(data) != -1) {
        qDebug(">>>>> Title %s", qPrintable(title.cap(1)));
        // title
    }

    QRegExp desc("<meta property=\"og:description\" content=\"([^\"]*)\">");
    if (desc.indexIn(data) != -1) {
        qDebug(">>>>> Desc %s", qPrintable(desc.cap(1)));
    }

    emit completed(url);
}

void YouTubeDataHandler::error(QNetworkReply::NetworkError error)
{
    qWarning("YouTubeDataHandler::error %d", error);
}
