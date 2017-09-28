!android: {
    QT       += testlib
    QT       -= gui

    TARGET    = ../bin/tst_next
    CONFIG   += console
    CONFIG   += testcase
    CONFIG   -= app_bundle

    TEMPLATE  = app
win32: {
    QMAKE_CXXFLAGS  += -Zc:wchar_t -Zi -DWINVER=0x0601 -D_WIN32_WINNT=0x0601 -D_CCRT_SECURE_NO_WARNINGS
    QMAKE_LFLAGS += /INCREMENTAL:NO /FIXED:NO
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

    Debug:LIBS   += bin/anextd.lib
    Release:LIBS += bin/anext.lib

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
