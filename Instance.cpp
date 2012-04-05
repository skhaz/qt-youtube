
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
        "--quiet",
    };

#ifdef _WIN32
        /* setenv() is not implemented on MinGW */
        if(!getenv("VLC_PLUGIN_PATH")) {
            putenv("VLC_PLUGIN_PATH=\"C:\\Program Files (x86)\\VideoLAN\\VLC\\plugins\"");
        }
#else
         setenv("VLC_PLUGIN_PATH", "/Users/Skhaz/Workspace/vlc/build/modules", 1);
#endif

    m_vlc_instance = libvlc_new(sizeof(argv) / sizeof(* argv), argv);
}

MonoInstance::~MonoInstance()
{
    libvlc_release(m_vlc_instance);
}

Instance::Instance()
{
}
