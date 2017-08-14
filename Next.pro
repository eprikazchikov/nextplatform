QT       -= core gui

win32: {
    Debug:TARGET    = ../bin/anextd
    Release:TARGET  = ../bin/anext
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
    inc/athreadpool.h \
    inc/acommon.h \
    src/aobject_p.h \
    src/aobjectsystem_p.h

INCLUDEPATH += \
    inc \
    ../glm/inc

DEFINES += NEXT_LIBRARY

defined(BUILD_WITH_EASY_PROFILER) {
    INCLUDEPATH += \
        ../easy_profiler-1.0.2/include

    LIBS += \
        -L"../../external/easy_profiler-1.0.2/bin" \
        -leasy_profiler
}
