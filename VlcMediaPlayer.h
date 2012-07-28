#ifndef VLCMEDIAPLAYER_H
#define VLCMEDIAPLAYER_H

#include <QString>
#include <vlc/vlc.h>
#include "LibVlcInstance.h"

/// The VlcMediaPlayer class is a wrapper around libvlc_media_player_t.
/// It provides basic functionality over the player.
class VlcMediaPlayer
{
public:
    /// Creates the VlcMediaPlayer.
    VlcMediaPlayer(LibVlcInstance& instance);

    /// Destroys the VlcMediaPlayer.
    ~VlcMediaPlayer();

    void setCallbacks(libvlc_video_lock_cb lock, libvlc_video_unlock_cb unlock,
        libvlc_video_display_cb display, void* opaque);
    void setMedia(libvlc_media_t* media);
    void setFormat(const QString& chroma, unsigned int width,
        unsigned int height, unsigned int pitch);

    /// Plays the media.
    void play();

    /// Pause the media.
    void pause();

    /// Stops the media.
    void stop();

private:
    /// Reference to the libvlc instance.
    LibVlcInstance& m_vlcInstance;

    /// Media player instance.
    libvlc_media_player_t* m_mediaPlayer;
};

#endif // VLCMEDIAPLAYER_H
