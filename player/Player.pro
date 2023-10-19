#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T15:32:41
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Player
TEMPLATE = app


SOURCES += main.cpp\
    VideoPlayer.cpp \
    VideoWidget.cpp \
    Video.cpp \
    NetworkSetting.cpp

HEADERS  += \
    VideoPlayer.h \
    VideoWidget.h \
    Video.h \
    NetworkSetting.h

target.path = $$[QT_INSTALL_EXAMPLES]/multimediawidgets/player
INSTALLS += target

DISTFILES +=

RESOURCES += \
    src/icon.qrc
