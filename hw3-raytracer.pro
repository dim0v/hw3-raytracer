#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T15:19:07
#
#-------------------------------------------------

QT       += core\
            gui

TARGET = hw3-raytracer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Transform.cpp \
    readfile.cpp \
    camera.cpp \
    scene.cpp \
    ray.cpp \
    intersection.cpp \
    light.cpp \
    sphere.cpp \
    triangle.cpp \
    raytracer.cpp \
    visibleobject.cpp

HEADERS += \
    Transform.h \
    readfile.h \
    camera.h \
    scene.h \
    ray.h \
    intersection.h \
    light.h \
    object.h \
    sphere.h \
    triangle.h \
    raytracer.h \
    visibleobject.h

CONFIG += c++11
