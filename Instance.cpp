
#include "Instance.h"

#include <vlc/vlc.h>
#include <stdlib.h>
#include <stdio.h>



MonoInstance Instance::m_instance;

MonoInstance::MonoInstance()
{
    static const char * const argv[] = {
        "--intf=dummy",
        "--no-video-title-show",
        "--no-stats",
        "--no-snapshot-preview",
        "--verbose=1",
    };

    setenv("VLC_PLUGIN_PATH", "/Users/Skhaz/Workspace/vlc/modules", 1); // XXX

    m_vlc_instance = libvlc_new(sizeof(argv) / sizeof(* argv), argv);
}

MonoInstance::~MonoInstance()
{
    libvlc_release(m_vlc_instance);
}

Instance::Instance()
{
}
