
#ifndef _Player_h
#define _Player_h

#include <QGraphicsObject>
#include <QMutex>
#include <QImage>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>

#include "Instance.h"



struct libvlc_media_player_t;
struct vlc_callback;

class Player : public QGraphicsObject
{
    public:
        explicit Player(QObject *parent = 0);

        ~Player();

        void setSource(const QString& source);

        QString source() const;

    public slots:
        void play();

        void pause();

        void stop();

    signals:
        void frameReady(vlc_callback *);

        void sourceChanged();

    protected:
        QRectF boundingRect() const;

        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    protected slots:
        void processFrame(vlc_callback *callback);

    private:
        Q_OBJECT

        Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)

        QString m_source;

        libvlc_media_player_t *m_player;

        vlc_callback* m_callback;

        QRectF m_bounds;

        QImage m_image;

        static void *lock(void*, void**);

        static void unlock(void*, void*, void * const*);
};

struct vlc_callback
{
    QMutex        *mutex;
    Player        *target;
    unsigned char *pixels;
};

#endif
