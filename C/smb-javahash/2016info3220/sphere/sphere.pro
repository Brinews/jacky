#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T13:57:13
#
#-------------------------------------------------

QT       += core gui
QT		 += opengl

LIBS += -L../sphere -lglut32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sphere
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    planet.cpp \
    sphere.cpp \
    glmodel.cpp

HEADERS  += mainwindow.h \
    planet.h \
    sphere.h \
    glmodel.h

FORMS    += mainwindow.ui
