
QT += core gui declarative opengl network xml sql

# TODO hardcoded

# vlc
INCLUDEPATH += .
INCLUDEPATH += /Users/Skhaz/Workspace/vlc/vlc_install_dir/include
LIBS += -L/Users/Skhaz/Workspace/vlc/vlc_install_dir/lib -lvlc

# libtorrent
INCLUDEPATH += -I/usr/local/include -I/usr/local/include/libtorrent
LIBS += -L/usr/local/lib -ltorrent-rasterbar
LIBS += -lboost_system-mt
LIBS += -lboost_filesystem-mt
DEFINES += TORRENT_USE_OPENSSL
DEFINES += WITH_SHIPPED_GEOIP_H
DEFINES += BOOST_ASIO_HASH_MAP_BUCKETS=1021
DEFINES += BOOST_EXCEPTION_DISABLE
DEFINES += BOOST_FILESYSTEM_VERSION=2
DEFINES += TORRENT_LINKING_SHARED

CONFIG -= app_bundle
TARGET = OmniMedia
TEMPLATE = app

OBJECTS_DIR = temp/obj
MOC_DIR = temp/moc
RCC_DIR = temp/rcc
UI_DIR = temp/ui

SOURCES += Main.cpp \
    Player.cpp \
    Instance.cpp \
    DownloadManager.cpp \
    Download.cpp \
    Media.cpp \
    YouTubeSearch.cpp \
    AbstractDataHandler.cpp \
    YouTubeDataHandler.cpp

HEADERS  += \
    Player.h \
    Instance.h \
    DownloadManager.h \
    Download.h \
    Media.h \
    YouTubeSearch.h \
    AbstractDataHandler.h \
    YouTubeDataHandler.h

OTHER_FILES += \
    VideoControl.qml \
    SearchInput.qml \
    Delegate.qml \
    Main.qml \
    ScrollBar.qml

