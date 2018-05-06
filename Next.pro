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
    src/core/bson.cpp \
    src/core/event.cpp \
    src/core/json.cpp \
    src/core/metamethod.cpp \
    src/core/metaobject.cpp \
    src/core/metaproperty.cpp \
    src/core/metatype.cpp \
    src/core/object.cpp \
    src/core/objectsystem.cpp \
    src/core/uri.cpp \
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
    src/math/math.cpp \
    src/core/threadpool.cpp \
    src/core/variant.cpp \
    src/anim/animation.cpp \
    src/anim/variantanimation.cpp \
    src/anim/propertyanimation.cpp


HEADERS += \
    inc/core/bson.h \
    inc/core/event.h \
    inc/core/json.h \
    inc/core/metamethod.h \
    inc/core/metaobject.h \
    inc/core/metaproperty.h \
    inc/core/objectsystem.h \
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
    inc/math/amath.h \
    inc/core/variant.h \
    inc/core/metatype.h \
    inc/common.h \
    inc/core/uri.h \
    inc/core/threadpool.h \
    inc/core/object.h \
    inc/patterns/asingleton.h \
    inc/anim/animation.h \
    inc/anim/variantanimation.h \
    inc/anim/propertyanimation.h


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
