
#include "Media.h"



Media::Media(QObject *parent)
: QObject(parent)
{
}

void Media::setId(const QString& id)
{
    m_id = id;

    emit idChanged();
}

QString Media::id() const
{
    return m_id;
}

void Media::setTitle(const QString& title)
{
    m_title = title;

    emit titleChanged();
}

QString Media::title() const
{
    return m_title;
}

void Media::setDescription(const QString& description)
{
    m_description = description;

    emit descriptionChanged();
}

QString Media::description() const
{
    return m_description;
}

void Media::setImage(const QUrl& image)
{
    m_image = image;

    emit imageChanged();
}

QUrl Media::image() const
{
    return m_image;
}

void Media::setUrl(const QUrl& url)
{
    m_url = url;

    emit urlChanged();
}

QUrl Media::url() const
{
    return m_url;
}
