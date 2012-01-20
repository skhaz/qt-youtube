
#include "Media.h"



Media::Media()
{
}

void Media::setId(const QString& id)
{
    m_id = id;
}

QString Media::id() const
{
    return m_id;
}

void Media::setTitle(const QString& title)
{
    m_title = title;
}

QString Media::title() const
{
    return m_title;
}

void Media::setDescription(const QString& description)
{
    m_description = description;
}

QString Media::description() const
{
    return m_description;
}

void Media::setImage(const QUrl& image)
{
    m_image = image;
}

QUrl Media::image() const
{
    return m_image;
}

void Media::setUrl(const QUrl& url)
{
    m_url = url;
}

QUrl Media::url() const
{
    return m_url;
}
