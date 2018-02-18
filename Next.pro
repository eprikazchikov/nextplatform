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
    src/core/abson.cpp \
    src/core/aevent.cpp \
    src/core/ajson.cpp \
    src/core/ametamethod.cpp \
    src/core/ametaobject.cpp \
    src/core/ametaproperty.cpp \
    src/core/ametatype.cpp \
    src/core/aobject.cpp \
    src/core/aobjectsystem.cpp \
    src/core/athreadpool.cpp \
    src/core/atools.cpp \
    src/core/auri.cpp \
    src/core/avariant.cpp \
    src/math/aabb.cpp \
    src/math/matrix3.cpp \
    src/math/matrix4.cpp \
    src/math/obb.cpp \
    src/math/plane.cpp \
    src/math/quaternion.cpp \
    src/math/ray.cpp \
    src/math/vector2.cpp \
    src/math/vector3.cpp \
    src/math/vector4.cpp \
    src/math/math.cpp


HEADERS += \
    inc/core/abson.h \
    inc/core/aevent.h \
    inc/core/ajson.h \
    inc/core/ametamethod.h \
    inc/core/ametaobject.h \
    inc/core/ametaproperty.h \
    inc/core/ametatype.h \
    inc/core/aobject.h \
    inc/core/aobjectsystem.h \
    inc/core/athreadpool.h \
    inc/core/atools.h \
    inc/core/auri.h \
    inc/core/avariant.h \
    inc/math/aabb.h \
    inc/math/matrix3.h \
    inc/math/matrix4.h \
    inc/math/obb.h \
    inc/math/plane.h \
    inc/math/quaternion.h \
    inc/math/ray.h \
    inc/math/vector2.h \
    inc/math/vector3.h \
    inc/math/vector4.h \
    inc/acommon.h \
    inc/math/amath.h


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
