#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T19:54:33
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

TARGET    = tst_next
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    tests/main.cpp \
    tests/tst_object.cpp \
    tests/tst_variant.cpp \
    tests/tst_objectsystem.cpp \
    src/aobject.cpp \
    src/avariant.cpp \
    src/aobjectsystem.cpp

HEADERS += \
    tests/tst_object.h \
    tests/tst_variant.h \
    tests/tst_objectsystem.h

INCLUDEPATH += \
    inc

DEFINES += SRCDIR=\\\"$$PWD/\\\"
