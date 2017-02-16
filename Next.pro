QT       -= core gui

win32: {
    Debug:TARGET    = anextd
    Release:TARGET  = anext
}

android: {
    TARGET = anext
}

TEMPLATE = lib

CONFIG += c++11

SOURCES += \
    src/avariant.cpp \
    src/aobject.cpp \
    src/aobjectsystem.cpp \
    src/ajson.cpp \
    src/auri.cpp \
    src/atools.cpp \
    src/abson.cpp \
    src/ametaobject.cpp \
    src/ametatype.cpp \
    src/ametaproperty.cpp \
    src/ametamethod.cpp \
    src/aevent.cpp \
    src/athreadpool.cpp

HEADERS += \
    inc/aobject.h \
    inc/avariant.h \
    inc/aobjectsystem.h \
    inc/ajson.h \
    inc/auri.h \
    inc/atools.h \
    inc/abson.h \
    inc/ametaobject.h \
    inc/ametatype.h \
    inc/ametaproperty.h \
    inc/ametamethod.h \
    inc/aevent.h \
    inc/athreadpool.h

INCLUDEPATH += \
    inc
