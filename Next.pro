#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T19:54:33
#
#-------------------------------------------------
QT       -= gui

TARGET   = anext
TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    src/aobject.cpp \
    src/avariant.cpp \
    src/aobjectsystem.cpp

HEADERS += \
    inc/aobject.h \
    inc/avariant.h \
    inc/aobjectsystem.h

INCLUDEPATH += \
    inc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
