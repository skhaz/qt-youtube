#ifndef _MediaModel_h
#define _MediaModel_h

#include <QAbstractListModel>

#include "Media.h"



class MediaModel : public QAbstractListModel
{
    public:
        enum AnimalRoles {
            TitleRole = Qt::UserRole + 1,
            DescriptionRole,
            ImageRole,
            UrlRole
        };

        explicit MediaModel(QObject *parent = 0);

        void addMedia(const Media& media);

        void removeMedia(const Media&);

        void clear();

        int rowCount(const QModelIndex & parent = QModelIndex()) const;

        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    private:
        Q_OBJECT

        QList<Media> m_medias;
};

#endif
