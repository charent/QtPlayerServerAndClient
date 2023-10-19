#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T18:13:28
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServiceWidget
TEMPLATE = app


SOURCES += main.cpp\
    ServerWindow.cpp \
    VideoDAO.cpp \
    Video.cpp \
    ClientServerThread.cpp \
    NetServer.cpp

HEADERS  += \
    ServerWindow.h \
    VideoDAO.h \
    Video.h \
    ClientServerThread.h \
    NetServer.h

FORMS    +=

RESOURCES += \
    btn_src.qrc
