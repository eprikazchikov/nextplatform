!android: {
    QT       += testlib

    TARGET    = ../bin/tst_next
    CONFIG   += console
    CONFIG   += testcase
    CONFIG   -= app_bundle

    TEMPLATE  = app
win32: {
    QMAKE_CXXFLAGS  += -DWINVER=0x0601 -D_WIN32_WINNT=0x0601 -D_CCRT_SECURE_NO_WARNINGS
}
    SOURCES += \
        tests/main.cpp \
        tests/tst_object.cpp \
        tests/tst_variant.cpp \
        tests/tst_objectsystem.cpp \
        tests/tst_uri.cpp \
        tests/tst_serialization.cpp \
        tests/tst_threadpool.cpp

    HEADERS += \
        tests/tst_common.h \
        tests/tst_object.h \
        tests/tst_variant.h \
        tests/tst_objectsystem.h \
        tests/tst_math.h \
        tests/tst_uri.h \
        tests/tst_serialization.h \
        tests/tst_threadpool.h

    LIBS += -Lbin

    Debug:LIBS   += -lanextd
    Release:LIBS += -lanext

    INCLUDEPATH  += \
        inc \
        #../glm/inc

    defined(BUILD_WITH_EASY_PROFILER) {
        INCLUDEPATH += \
            ../easy_profiler-1.0.2/include

        LIBS += \
            -L"../../external/easy_profiler-1.0.2/bin" \
            -leasy_profiler
    }
}

HEADERS += \
    tests/tst_animation.h \
    tests/tst_math.h

SOURCES += \
    tests/tst_animation.cpp \
    tests/tst_math.cpp
