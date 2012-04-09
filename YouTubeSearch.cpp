
#include "YouTubeSearch.h"

#include <QtDebug>



YouTubeSearch::YouTubeSearch(QObject *parent)
: QObject(parent)
{
    cache.setMaximumCacheSize(CACHE_SIZE);
    cache.setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/omnimedia");

    manager.setCache(&cache);
    m_reply = 0;
    m_context = 0;
}

YouTubeSearch::~YouTubeSearch()
{
}

void YouTubeSearch::setContext(QDeclarativeContext *context)
{
    m_context = context;
}

void YouTubeSearch::search(const QString& query)
{
    xml.clear();
    qDeleteAll(m_objects);
    m_objects.clear();

    if (query.isEmpty()) {
        m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));
        return;
    }

    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }

    QString url = QString("%1q=%2")
            .arg(YOUTUBE_API)
            .arg(query);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    m_reply = manager.get(request);
    connect(m_reply, SIGNAL(readyRead()), SLOT(readyRead()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeSearch::readyRead()
{
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        QString currentTag;
        QString linkString;
        QString descriptionString;
        QString titleString;

        xml.addData(m_reply->readAll());
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                if (xml.name() == QLatin1String("item"))
                    linkString = xml.attributes().value("link").toString();

                currentTag = xml.qualifiedName().toString();
            } else if (xml.isEndElement()) {
                if (xml.qualifiedName() == QLatin1String("media:title")) {
                    QUrl url(linkString);
                    QString videoId = url.queryItemValue("v");
                    QString videoImage = QString(YTIMG)
                            .arg(qrand() % 3 + 1)
                            .arg(videoId);

                    Media *media = new Media(m_context);
                    media->setTitle(titleString);
                    media->setDescription(descriptionString);
                    media->setId(videoId);
                    media->setImage(QUrl(videoImage));
                    media->setUrl(url);
                    m_objects << media;

                    m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));

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
            qWarning() << xml.lineNumber() << xml.errorString();
        }
    }
}

void YouTubeSearch::error(QNetworkReply::NetworkError error)
{
}

void YouTubeSearch::finished()
{
    m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));
    xml.clear();
}
