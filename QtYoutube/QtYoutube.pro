
QT += \
    core gui widgets network xml qml quick

TARGET = QtYoutube
TEMPLATE = app
CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    YouTubeSearch.cpp \
    Media.cpp \
    YouTubeDataHandler.cpp

HEADERS += \
    MainWindow.h \
    YouTubeSearch.h \
    Media.h \
    YouTubeDataHandler.h
