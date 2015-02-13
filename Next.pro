#-------------------------------------------------
#
# Project created by QtCreator 2014-01-30T19:54:33
#
#-------------------------------------------------

QT       -= gui

!tests {
    TARGET      = anext
    TEMPLATE    = lib

    CONFIG      += staticlib
} else {
    QT          += testlib

    TARGET      = ../bin/tst_next
    TEMPLATE    = app

    CONFIG      += console
    CONFIG      -= app_bundle

    SOURCES += \
        tests/main.cpp \
        tests/tst_object.cpp \
        tests/tst_variant.cpp \
        tests/tst_objectsystem.cpp \
        tests/tst_json.cpp \
        tests/tst_uri.cpp

    HEADERS += \
        tests/tst_common.h \
        tests/tst_object.h \
        tests/tst_variant.h \
        tests/tst_objectsystem.h \
        tests/tst_json.h \
        tests/tst_uri.h
}

SOURCES += \
    src/aobject.cpp \
    src/avariant.cpp \
    src/aobjectsystem.cpp \
    src/ajson.cpp \
    src/auri.cpp

HEADERS += \
    inc/aobject.h \
    inc/avariant.h \
    inc/aobjectsystem.h \
    inc/ajson.h \
    inc/auri.h

INCLUDEPATH += \
    inc
