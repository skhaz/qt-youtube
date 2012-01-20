
#include "YouTubeSearch.h"

#include <QtDebug>



YouTubeSearch::YouTubeSearch(QObject *parent)
: QObject(parent)
{
    cache.setMaximumCacheSize(CACHE_SIZE);
    cache.setCacheDirectory(QDesktopServices::storageLocation(QDesktopServices::CacheLocation) + "/omnimedia");
    manager.setCache(&cache);
    m_search_reply = 0;
    m_model = new MediaModel;
}

YouTubeSearch::~YouTubeSearch()
{
    delete m_model;
}

MediaModel* YouTubeSearch::model() const
{
    return m_model;
}

void YouTubeSearch::search(const QString& query)
{
    if (m_search_reply && m_search_reply->isRunning()) {
        m_search_reply->abort();
        m_search_reply->deleteLater();
    }

    QString url = QString("%1q=%2")
            .arg(YOUTUBE_API)
            .arg(query);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    m_search_reply = manager.get(request);
    connect(m_search_reply, SIGNAL(downloadProgress(qint64,qint64)), SLOT(downloadProgress(qint64,qint64)));
    connect(m_search_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
    connect(m_search_reply, SIGNAL(finished()), SLOT(searchfinished()));
}

void YouTubeSearch::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    Q_UNUSED(bytesReceived);
    Q_UNUSED(bytesTotal);
}

void YouTubeSearch::error(QNetworkReply::NetworkError code)
{
    qWarning("YouTubeSearch::error %d", code);
}

void YouTubeSearch::searchfinished()
{
    QBuffer buffer;
    buffer.setData(m_search_reply->readAll());
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

        Media media;
        media.setId(videoId);
        media.setImage(QUrl(videoImage));

        QVariant var;
        var.setValue<Media>(media);

        QNetworkRequest request(result);
        request.setAttribute(QNetworkRequest::User, var);
        request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

        QNetworkReply *reply = manager.get(request);
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(error(QNetworkReply::NetworkError)));
        connect(reply, SIGNAL(finished()), SLOT(pageFinished()));
    }
}

void YouTubeSearch::pageFinished()
{
    if (QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender())) {
        QString data = reply->readAll();
        Media media = reply->request().attribute(QNetworkRequest::User).value<Media>();

        // This block is based on file youtube.lua from VideoLAN project
        QMap<int, QString> stream_map;
        QRegExp re("\"url_encoded_fmt_stream_map\": \"([^\"]*)\"", Qt::CaseInsensitive, QRegExp::RegExp2);
        QRegExp urls("itag=(\\d+),url=(.*)");

        if (re.indexIn(data) != -1) {
            QString result = re.cap(1);
            foreach (QString line, result.split("\\u0026")) {
            if (urls.indexIn(QUrl::fromPercentEncoding(line.toAscii())) != -1) {
                    stream_map[urls.cap(1).toInt()] = urls.cap(2);
                }
            }

            media.setUrl(stream_map[18]); // XXX hardcoded
        }

        QRegExp title("<meta property=\"og:title\" content=\"([^\"]*)\">");
        if (title.indexIn(data) != -1) {
            media.setTitle(title.cap(1));
        }

        QRegExp desc("<meta property=\"og:description\" content=\"([^\"]*)\">");
        if (desc.indexIn(data) != -1) {
            media.setDescription(desc.cap(1));
        }

        m_model->addMedia(media);
    }
}
