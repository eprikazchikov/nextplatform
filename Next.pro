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
    src/athreadpool.cpp \
    src/math/vector2.cpp \
    src/math/vector3.cpp \
    src/math/vector4.cpp \
    src/math/matrix3.cpp \
    src/math/quaternion.cpp \
    src/math/matrix4.cpp \
    src/math/ray.cpp \
    src/amath.cpp \
    src/math/aabb.cpp \
    src/math/obb.cpp

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
    inc/math/aabb.h \
    inc/math/obb.h \
    inc/math/plane.h \
    inc/math/ray.h \
    inc/amath.h \
    inc/math/matrix3.h \
    inc/math/matrix4.h \
    inc/math/quaternion.h \
    inc/math/vector2.h \
    inc/math/vector3.h \
    inc/math/vector4.h

INCLUDEPATH += \
    inc

DEFINES += NEXT_LIBRARY

defined(BUILD_WITH_EASY_PROFILER) {
    INCLUDEPATH += \
        ../easy_profiler-1.0.2/include

    LIBS += \
        -L"../../external/easy_profiler-1.0.2/bin" \
        -leasy_profiler
}
