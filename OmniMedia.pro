
QT += core gui declarative opengl network xmlpatterns sql

# hardcoded
INCLUDEPATH += .
INCLUDEPATH += /Users/Skhaz/Workspace/vlc/vlc_install_dir/include
LIBS += -L/Users/Skhaz/Workspace/vlc/vlc_install_dir/lib -lvlc

CONFIG -= app_bundle
TARGET = OmniMedia
TEMPLATE = app

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

