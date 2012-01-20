
QT += core gui declarative opengl network xmlpatterns

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
    MediaModel.cpp \
    Media.cpp \
    YouTubeSearch.cpp

HEADERS  += \
    Player.h \
    Instance.h \
    DownloadManager.h \
    Download.h \
    MediaModel.h \
    Media.h \
    YouTubeSearch.h

OTHER_FILES += \
    VideoControl.qml \
    SearchInput.qml \
    Delegate.qml \
    Main.qml

