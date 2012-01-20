
#ifndef _Media_h
#define _Media_h

#include <QString>
#include <QUrl>
#include <QMetaType>


class Media;
Q_DECLARE_METATYPE(Media)

class Media
{
    public:
        Media();

        void setId(const QString& id);

        QString id() const;

        void setTitle(const QString& title);

        QString title() const;

        void setDescription(const QString& description);

        QString description() const;

        void setImage(const QUrl& image);

        QUrl image() const;

        void setUrl(const QUrl& url);

        QUrl url() const;

    private:
        QString m_id;

        QString m_title;

        QString m_description;

        QUrl m_image;

        QUrl m_url;
};

#endif
