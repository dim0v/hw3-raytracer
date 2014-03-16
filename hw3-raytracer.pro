#-------------------------------------------------
#
# Project created by QtCreator 2012-11-21T15:19:07
#
#-------------------------------------------------

QT       += core\
            gui\
            widgets

TARGET = hw3-raytracer
CONFIG   = qt

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
    visibleobject.cpp \
    boundingbox.cpp \
    raytracingworker.cpp \
    raytracer.cpp \
    progressform.cpp

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
    visibleobject.h \
    boundingbox.h \
    raytracingworker.h \
    raytracer.h \
    progressform.h

CONFIG += c++11

FORMS += \
    progressform.ui
