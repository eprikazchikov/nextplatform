QT       -= core gui

win32: {
    Debug:TARGET    = anextd
    Release:TARGET  = anext
}

android: {
    TARGET = anext
}

TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    src/avariant.cpp \
    src/aobject.cpp \
    src/aobjectsystem.cpp \
    src/ajson.cpp \
    src/auri.cpp \
    src/atools.cpp \
    src/abson.cpp

HEADERS += \
    inc/aobject.h \
    inc/avariant.h \
    inc/aobjectsystem.h \
    inc/ajson.h \
    inc/auri.h \
    inc/atools.h \
    inc/abson.h

INCLUDEPATH += \
    inc
