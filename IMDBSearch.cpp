
#include "IMDBSearch.h"

#include "Media.h"
#include "parser.h"



IMDBSearch::IMDBSearch(QObject *parent)
: QObject(parent)
, _reply(NULL)
{
}

void IMDBSearch::setContext(QDeclarativeContext *context)
{
    _context = context;
}

void IMDBSearch::search(const QString& query)
{
    if (query.isEmpty()) {
        qDeleteAll(_objects);
        _objects.clear();
        _context->setContextProperty("youtubeModel", QVariant::fromValue(_objects));

        return;
    }

    if (_reply) {
        _reply->abort();
        _reply->deleteLater();
    }

    QString url = QString("%1/%2/%3.json")
            .arg(IMDB_API)
            .arg(query.at(0))
            .arg(query)
            .replace(" ", "_");

    _reply = _manager.get(QNetworkRequest(url));
    connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(_reply, SIGNAL(finished()), SLOT(finished()));
}

void IMDBSearch::finished()
{
    int statusCode = _reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QByteArray result = _reply->readAll();
        int start = result.indexOf("(") + 1;
        int end = result.lastIndexOf(")") - start;

        bool ok;
        QJson::Parser parser;
        QVariantMap resultMap = parser.parse(result.mid(start, end), &ok).toMap();

        if (!ok) {
            qWarning() << "An error occured during parsing json";
            return;
        }

        qDeleteAll(_objects);
        _objects.clear();

        QVariantList movies = resultMap["d"].toList();
        foreach (QVariant movie, movies) {
            QVariantMap movieMap = movie.toMap();

            QString thumbnail;
            QString id = movieMap["id"].toString();
            QString name = movieMap["l"].toString();
            QString year = movieMap["y"].toString();

            QVariantList array = movieMap["i"].toList();
            if (!array.empty()) {
                thumbnail = array.first().toString();
                thumbnail.replace(THUMB_TAG, THUMB_DEFAULT_SIZE);
            }

            Media *media = new Media(_context);
            media->setTitle(name);
            media->setId(id);
            media->setImage(thumbnail);
            media->setUrl(QUrl(thumbnail));
            _objects << media;
        }

        _context->setContextProperty("youtubeModel", QVariant::fromValue(_objects));
    }
}

void IMDBSearch::error(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}
