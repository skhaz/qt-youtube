
QT += core gui declarative opengl network xml sql

# TODO hardcoded

# vlc
INCLUDEPATH += .
INCLUDEPATH += /Users/Skhaz/Workspace/vlc/build/vlc_install_dir/include
LIBS += -L/Users/Skhaz/Workspace/vlc/build/vlc_install_dir/lib -lvlc

# libMaia
include(3rdparty/libmaia/maia.pri)

# QJson
include(3rdparty/qjson/qjson.pri)

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
    YouTubeDataHandler.cpp \
    OpenSubtitlesClient.cpp \
    IMDBSearch.cpp

HEADERS  += \
    Player.h \
    Instance.h \
    DownloadManager.h \
    Download.h \
    Media.h \
    YouTubeSearch.h \
    AbstractDataHandler.h \
    YouTubeDataHandler.h \
    OpenSubtitlesClient.h \
    IMDBSearch.h

OTHER_FILES += \
    VideoControl.qml \
    SearchInput.qml \
    Delegate.qml \
    Main.qml \
    ScrollBar.qml
