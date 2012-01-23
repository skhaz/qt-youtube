
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
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(m_reply, SIGNAL(finished()), SLOT(finished()));
}

void YouTubeSearch::error(QNetworkReply::NetworkError code)
{
    qWarning("YouTubeSearch::error %d", code);
}

void YouTubeSearch::finished()
{
    qDeleteAll(m_objects);
    m_objects.clear();

    QBuffer buffer;
    buffer.setData(m_reply->readAll());
    buffer.open(QIODevice::ReadOnly);

    // XXX hardcoded
    QString atom(
        "declare namespace atom = \"http://www.w3.org/2005/Atom\";\n"
        "declare namespace media = \"http://search.yahoo.com/mrss/\";\n"
        "doc($result)//atom:entry/media:group/media:player/@url/string()\n"
    );

    QXmlQuery query;
    query.bindVariable("result", &buffer);
    query.setQuery(atom);

    QStringList results;
    query.evaluateTo(&results);

    foreach (QUrl result, results) {
        QString videoId = result.queryItemValue("v");
        QString videoImage = QString(YTIMG)
                .arg(qrand() % 3 + 1)
                .arg(videoId);

        Media *media = new Media(m_context);
        media->setTitle("");
        media->setDescription("");
        media->setId(videoId);
        media->setImage(QUrl(videoImage));
        media->setUrl(result);
        m_objects << media;
    }

    m_context->setContextProperty("youtubeModel", QVariant::fromValue(m_objects));
}


