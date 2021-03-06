#-------------------------------------------------
#
# Project created by QtCreator 2017-08-11T09:34:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mymainwindow
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    about.cpp \
    system.cpp \
    git.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    about.h \
    system.h \
    git.h \
    settings.h

FORMS    += mainwindow.ui \
    about.ui \
    settings.ui

RESOURCES += \
    source/source.qrc
RC_ICONS = myicon.ico
