#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T21:28:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGFinalWork
TEMPLATE = app


SOURCES +=\
        mainwindow.cpp \
    openglwidget.cpp \
    camera.cpp \
    light.cpp \
    material.cpp \
    ConfigLoader.cpp \
    main.cpp \
    entity.cpp \
    gamePlayer.cpp \
    scenario.cpp \
    energyball.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    camera.h \
    light.h \
    material.h \
    ConfigLoader.hpp \
    rapidxml.hpp \
    rapidxml_iterators.hpp \
    rapidxml_print.hpp \
    rapidxml_utils.hpp \
    entity.h \
    gamePlayer.h \
    scenario.h \
    energyball.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
