#include "YouTubeSearch.h"

#include <QtDebug>

YouTubeSearch::YouTubeSearch(QObject *parent)
: QObject(parent)
{
    cache.setMaximumCacheSize(CACHE_SIZE);
    // TODO cache.setCacheDirectory(QDesktopServices::storageLocation(QStandardPaths::CacheLocation) + "/.qtyoutube");

    manager.setCache(&cache);
    reply = 0;
    context = 0;
}

void YouTubeSearch::setContext(QObject *context)
{
    context = context;
}

void YouTubeSearch::search(const QString& query)
{
    xml.clear();
    qDeleteAll(objects);
    objects.clear();

    if (query.isEmpty()) {
        // TODO context->setContextProperty("youtubeModel", QVariant::fromValue(objects));
        return;
    }

    if (reply) {
        if (reply->isRunning()) {
            disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
            disconnect(reply, SIGNAL(finished()), this, SLOT(finished()));

            reply->abort();
        }

        reply->deleteLater();
        reply = nullptr;
    }

    QString url = QString("%1q=%2")
        .arg(YOUTUBE_API)
        .arg(query);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

    reply = manager.get(request);
    connect(reply, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(reply, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeSearch::readyRead()
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QString currentTag;
        QString linkString;
        QString descriptionString;
        QString titleString;

        xml.addData(reply->readAll());
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                currentTag = xml.qualifiedName().toString();
            } else if (xml.isEndElement()) {
                if (xml.qualifiedName() == QLatin1String("media:title")) {
                    QUrl url(linkString);
                    QUrlQuery urlQuery(url);
                    QString videoId = urlQuery.queryItemValue("v");
                    QString videoImage = QString(YTIMG)
                        .arg(qrand() % 3 + 1)
                        .arg(videoId);

                    // FIXME for some reason, sometimes the link url comes empty
                    if (!videoId.isEmpty()) {
                        Media *media = new Media(context);
                        media->setTitle(titleString);
                        media->setDescription(descriptionString);
                        media->setId(videoId);
                        media->setImage(QUrl(videoImage));
                        media->setUrl(url);

                        objects << media;
                    }

                    // TODO context->setContextProperty("youtubeModel", QVariant::fromValue(objects));

                    currentTag.clear();
                    titleString.clear();
                    linkString.clear();
                    descriptionString.clear();
                }

            } else if (xml.isCharacters() && !xml.isWhitespace()) {
                if (currentTag == QLatin1String("media:title")) {
                    titleString += xml.text().toString();
                } else if (currentTag == QLatin1String("media:description")) {
                    descriptionString += xml.text().toString();
                } else if (currentTag == QLatin1String("link")) {
                    linkString += xml.text().toString();
                }
            }
        }

        if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
            // qWarning() << xml.lineNumber() << xml.errorString();
        }
    }
}

void YouTubeSearch::error(QNetworkReply::NetworkError error)
{

}

void YouTubeSearch::finished()
{
    // context->setContextProperty("youtubeModel", QVariant::fromValue(objects));
    xml.clear();
}
