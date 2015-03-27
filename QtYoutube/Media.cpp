#include "Media.h"

Media::Media(QObject *parent)
: QObject(parent)
{

}

void Media::setId(const QString& id)
{
    id_ = id;

    emit idChanged();
}

const QString& Media::id() const
{
    return id_;
}

void Media::setTitle(const QString& title)
{
    title_ = title;

    emit titleChanged();
}

const QString& Media::title() const
{
    return title_;
}

void Media::setDescription(const QString& description)
{
    description_ = description;

    emit descriptionChanged();
}

const QString& Media::description() const
{
    return description_;
}

void Media::setImage(const QUrl& image)
{
    image_ = image;

    emit imageChanged();
}

const QUrl& Media::image() const
{
    return image_;
}

void Media::setUrl(const QUrl& url)
{
    url_ = url;

    emit urlChanged();
}

const QUrl& Media::url() const
{
    return url_;
}
