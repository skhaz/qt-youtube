#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsObject>
#include <QMutex>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QUrl>
#include <QMap>
#include "LibVlcInstance.h"
#include "VlcMediaPlayer.h"
#include "Media.h"
#include "AbstractDataHandler.h"

struct libvlc_media_player_t;
struct vlc_callback;

class Player : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit Player(QObject* parent = 0);

    ~Player();

    void setSource(Media *source);

    Media* source() const;

    LibVlcInstance& vlcInstance();

public slots:
    void play();

    void pause();

    void stop();

    void setUrl(const QUrl& url);

signals:
    void frameReady(vlc_callback *);

    void sourceChanged();

protected:
    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected slots:
    void processFrame(vlc_callback *callback);

private:
    Q_PROPERTY(Media* source READ source WRITE setSource NOTIFY sourceChanged)

    LibVlcInstance m_vlc_instance;

    VlcMediaPlayer m_mediaPlayer;

    Media *m_source;

    libvlc_media_player_t *m_player;

    vlc_callback* m_callback;

    QRectF m_bounds;

    QImage m_image;

    QMap<QString, AbstractDataHandler *> m_handler_map;

    static void* lock(void*, void**);

    static void unlock(void*, void*, void * const*);
};

struct vlc_callback
{
    QMutex        *mutex;
    Player        *target;
    unsigned char *pixels;
};

#endif // PLAYER_H
