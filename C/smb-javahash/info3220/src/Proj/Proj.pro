#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T21:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Base3
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    gamebody.cpp \
    gamecomposite.cpp \
    config.cpp \
    gamecomponentfactory.cpp \
    gameball.cpp

HEADERS  += dialog.h \
    gamecomponent.h \
    gamecomposite.h \
    gamebody.h \
    config.h \
    gamecomponentfactory.h \
    gameball.h

FORMS    += dialog.ui

OTHER_FILES +=
