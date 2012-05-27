#include "VlcMediaPlayer.h"

VlcMediaPlayer::VlcMediaPlayer(LibVlcInstance& instance)
    : m_vlcInstance(instance)
{
    m_mediaPlayer = libvlc_media_player_new(m_vlcInstance);
}

VlcMediaPlayer::~VlcMediaPlayer()
{
    stop();
    libvlc_media_player_release(m_mediaPlayer);
}

void VlcMediaPlayer::setCallbacks(libvlc_video_lock_cb lock,
    libvlc_video_unlock_cb unlock, libvlc_video_display_cb display, void* opaque)
{
    libvlc_video_set_callbacks(m_mediaPlayer, lock, unlock, display, opaque);
}

void VlcMediaPlayer::setMedia(libvlc_media_t* media)
{
    libvlc_media_player_set_media(m_mediaPlayer, media);
}

void VlcMediaPlayer::setFormat(const QString& chroma, unsigned int width,
    unsigned int height, unsigned int pitch)
{
    libvlc_video_set_format(m_mediaPlayer,
        chroma.toLocal8Bit().constData(),
        width,
        height,
        pitch);
}

void VlcMediaPlayer::play()
{
    libvlc_media_player_play(m_mediaPlayer);
}

void VlcMediaPlayer::pause()
{
    libvlc_media_player_pause(m_mediaPlayer);
}

void VlcMediaPlayer::stop()
{
    libvlc_media_player_stop(m_mediaPlayer);
}
