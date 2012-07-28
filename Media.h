#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QUrl>
#include <QMetaType>
#include <QObject>


class Media : public QObject
{
    Q_OBJECT

public:
    explicit Media(QObject *parent = 0);

    void setId(const QString& id);

    const QString& id() const;

    void setTitle(const QString& title);

    const QString& title() const;

    void setDescription(const QString& description);

    const QString& description() const;

    void setImage(const QUrl& image);

    const QUrl& image() const;

    void setUrl(const QUrl& url);

    const QUrl& url() const;

signals:
    void idChanged();

    void titleChanged();

    void descriptionChanged();

    void imageChanged();

    void urlChanged();

private:
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

    QString m_id;
    QString m_title;
    QString m_description;
    QUrl m_image;
    QUrl m_url;
};

#endif // MEDIA_H
