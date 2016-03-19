!android: {
    QT       += testlib
    QT       -= gui

    TARGET    = ../bin/tst_next
    CONFIG   += console
    CONFIG   += testcase
    CONFIG   -= app_bundle

    TEMPLATE  = app

    SOURCES += \
        tests/main.cpp \
        tests/tst_object.cpp \
        tests/tst_variant.cpp \
        tests/tst_objectsystem.cpp \
        tests/tst_json.cpp \
        tests/tst_math.cpp \
        tests/tst_uri.cpp

    HEADERS += \
        tests/tst_common.h \
        tests/tst_object.h \
        tests/tst_variant.h \
        tests/tst_objectsystem.h \
        tests/tst_json.h \
        tests/tst_math.h \
        tests/tst_uri.h

    Debug:LIBS   += debug/anextd.lib
    Release:LIBS += release/anext.lib

    INCLUDEPATH  += \
        inc
}

HEADERS += \
    tests/tst_bson.h

SOURCES += \
    tests/tst_bson.cpp
