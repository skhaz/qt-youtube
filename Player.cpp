#include "Player.h"

#include <vlc/vlc.h>



Player::Player(QObject *)
: m_bounds(QPointF(), QSizeF(800, 600))
{
    // TODO resize
    connect(this, SIGNAL(frameReady(vlc_callback *)),
        SLOT(processFrame(vlc_callback *)));

    m_callback = new vlc_callback;
    m_callback->mutex = new QMutex;
    m_callback->target = this;
    m_callback->pixels = new unsigned char[int(sizeof(*(m_callback->pixels)) * m_bounds.width() * m_bounds.height() * 4)];

    m_player = libvlc_media_player_new(Instance());
    libvlc_video_set_callbacks(m_player, lock, unlock, NULL, m_callback);
    setCacheMode(NoCache);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}

Player::~Player()
{
    stop();

    if (m_player) {
        libvlc_media_player_release(m_player);
    }

    if (m_callback) {
        if (m_callback->pixels) {
            delete[] m_callback->pixels;
        }

        delete m_callback->mutex;
        delete m_callback;
    }
}

void Player::setSource(const QString& source)
{
    libvlc_media_t *media = libvlc_media_new_path(Instance(), source.toLocal8Bit().constData());
    libvlc_media_player_set_media(m_player, media);
    libvlc_video_set_format(m_player, "RV32", m_bounds.width(), m_bounds.height(), m_bounds.width() * 4);
    libvlc_media_release(media);
    m_source = source;

    emit sourceChanged();
    play();
}

QString Player::source() const
{
    return m_source;
}

QRectF Player::boundingRect() const
{
    return m_bounds;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawImage(QPoint(0, 0), m_image);
}

void Player::processFrame(vlc_callback* callback)
{
    m_image = QImage(callback->pixels, m_bounds.width(), m_bounds.height(), QImage::Format_RGB32);

    update(); // shedule redraw
}

void Player::play()
{
    libvlc_media_player_play(m_player);
}

void Player::pause()
{
    libvlc_media_player_pause(m_player);
}

void Player::stop()
{
    libvlc_media_player_stop(m_player);
    m_image = QImage();
}

void *Player::lock(void *data, void **pixels)
{
    vlc_callback *callback = static_cast<vlc_callback *>(data);
    callback->mutex->lock();
    *pixels = callback->pixels;

    return NULL;
}

void Player::unlock(void *data, void *id, void *const *pixels)
{
    Q_UNUSED(id);
    Q_UNUSED(pixels);

    vlc_callback *callback = static_cast<vlc_callback *>(data);
    callback->mutex->unlock();
    callback->target->emit frameReady(callback);
}
