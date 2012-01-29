
#include "YouTubeSearch.h"

#include <QtDebug>



YouTubeSearch::YouTubeSearch(QObject *parent)
    : QObject(parent)
{
    cache.setMaximumCacheSize(CACHE_SIZE);
    cache.setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/omnimedia");

    manager.setCache(&cache);
    m_reply = 0;
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
    if (query.isEmpty()) {
        qDeleteAll(m_objects);
        m_objects.clear();
        xml.clear();
        m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));

        return;
    }

    if (m_reply && m_reply->isRunning()) {
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
        xml.addData(m_reply->readAll());
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.isStartElement()) {
                if (xml.name() == "item")
                    linkString = xml.attributes().value("link").toString();
                currentTag = xml.name().toString();
            } else if (xml.isEndElement()) {
                if (xml.name() == "item") {
                    QUrl url(linkString);
                    QString videoId = url.queryItemValue("v");
                    QString videoImage = QString(YTIMG)
                            .arg(qrand() % 3 + 1)
                            .arg(videoId);

                    Media *media = new Media(m_context);
                    media->setTitle(titleString);
                    media->setDescription("");
                    media->setId(videoId);
                    media->setImage(QUrl(videoImage));
                    media->setUrl(url);
                    m_objects << media;

                    m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));
                    titleString.clear();
                    linkString.clear();
                }

            } else if (xml.isCharacters() && !xml.isWhitespace()) {
                if (currentTag == "title") {
                    titleString += xml.text().toString();
                } else if (currentTag == "link") {
                    linkString += xml.text().toString();
                }
            }
        }

        if (xml.error() && xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
            qWarning() << "XML ERROR:" << xml.lineNumber() << ": " << xml.errorString();
        }
    }
}

void YouTubeSearch::error(QNetworkReply::NetworkError error)
{
}

void YouTubeSearch::finished()
{
    m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));
    m_objects.clear();
    xml.clear();
}


