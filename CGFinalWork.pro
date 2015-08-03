#-------------------------------------------------
#
# Project created by QtCreator 2015-08-02T21:28:13
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGFinalWork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    camera.cpp \
    light.cpp \
    material.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    camera.h \
    light.h \
    material.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
