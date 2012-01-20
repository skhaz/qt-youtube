#include "MediaModel.h"



MediaModel::MediaModel(QObject *parent)
: QAbstractListModel(parent)
{
    QHash<int, QByteArray> roles;
    roles[TitleRole]       = "title";
    roles[DescriptionRole] = "description";
    roles[ImageRole]       = "image";
    roles[UrlRole]         = "url";
    setRoleNames(roles);
}

void MediaModel::addMedia(const Media& media)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_medias << media;
    endInsertRows();
}

void MediaModel::removeMedia(const Media &)
{
}

int MediaModel::rowCount(const QModelIndex &) const
{
    return m_medias.count();
}

QVariant MediaModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() > m_medias.count())
        return QVariant();

    const Media &media = m_medias[index.row()];
    switch (role)
    {
        case TitleRole:
            return media.title();
        case DescriptionRole:
            return media.description();
        case ImageRole:
            return media.image();
        case UrlRole:
            return media.url();
        default:
            return QVariant();
    }
}
