#include <vlc/vlc.h>
#include "Application.h"
#include "Player.h"
#include "YouTubeDataHandler.h"

Player::Player(QObject* parent)
    : m_bounds(QPointF(), QSizeF(1024, 768))
    , m_vlc_instance(Application::instance()->vlcArguments())
{
    // XXX very ungly
    YouTubeDataHandler *handler = new YouTubeDataHandler(this);
    m_handler_map["www.youtube.com"] = handler;
    connect(handler, SIGNAL(completed(QUrl)), SLOT(setUrl(QUrl)));

    // TODO resize
    connect(this, SIGNAL(frameReady(vlc_callback *)),
        SLOT(processFrame(vlc_callback *)));

    m_callback = new vlc_callback;
    m_callback->mutex = new QMutex;
    m_callback->target = this;
    m_callback->pixels = new unsigned char[int(sizeof(*(m_callback->pixels)) * m_bounds.width() * m_bounds.height() * 4)];

    m_player = libvlc_media_player_new(vlcInstance());
    libvlc_video_set_callbacks(m_player, lock, unlock, NULL, m_callback);
    setCacheMode(NoCache);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}

Player::~Player()
{
    if (m_player) {
        libvlc_media_player_stop(m_player);
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


void Player::setSource(Media *source)
{
    if (!source)
        return;

    m_source = source;

    emit sourceChanged();

    if (m_handler_map.contains(source->url().host())) {
        AbstractDataHandler *handler = m_handler_map[source->url().host()];
        handler->asyncStart(source->url());
        return;
    }

    setUrl(source->url());
}

Media *Player::source() const
{
    return m_source;
}

LibVlcInstance& Player::vlcInstance()
{
    return m_vlc_instance;
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

void Player::setUrl(const QUrl& url)
{
    libvlc_media_t *media = libvlc_media_new_path(vlcInstance(), url.toString().toLocal8Bit().constData());
    libvlc_media_player_set_media(m_player, media);
    libvlc_video_set_format(m_player, "RV32", m_bounds.width(), m_bounds.height(), m_bounds.width() * 4);
    libvlc_media_release(media);

    play();
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
