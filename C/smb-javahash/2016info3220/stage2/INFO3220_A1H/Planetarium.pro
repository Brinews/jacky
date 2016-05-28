#-------------------------------------------------
#
# Project created by QtCreator 2016-03-25T22:29:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Planetarium
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
        dialog.cpp \
    body2d.cpp \
    engine.cpp \
    componentfactory.cpp \
    component2dfactory.cpp \
    definition.cpp \
    body.cpp

HEADERS  += dialog.h \
    body2d.h \
    engine.h \
    componentfactory.h \
    component2dfactory.h \
    body.h \
    definition.h

FORMS    += dialog.ui
