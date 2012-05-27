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
    int statusCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QByteArray data = reply->readAll();
        QRegExp re("\"url_encoded_fmt_stream_map\": \"([^\"]*)\"", Qt::CaseInsensitive, QRegExp::RegExp2);
        QRegExp urls("itag=(\\d+),url=(.*)");
        QString storyboard("storyboard_spec=");

        if (int index = data.indexOf(storyboard)) {
            index = index + storyboard.length();
            QString temp = QUrl::fromPercentEncoding(data.mid(index, (data.indexOf("\\u0026", index)) - index));

            qDebug() << temp;
            QStringList temp2 = temp.split("|");
            QString temp3 = temp2.at(2);

            qDebug() << temp3.split("#");
            // $L
            // $N
        }

        if (re.indexIn(data) != -1) {
            QHash<int, QString> stream_map;
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

    else {
        // TODO handle error
    }

    _reply->deleteLater();
}

void YouTubeDataHandler::error(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}
