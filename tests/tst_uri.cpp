#include "tst_uri.h"

#include "core/uri.h"

#include <QTest>

#include <QDebug>

void UriTest::Parse_URI() {
    Uri uri("scheme://host/path/to/uri?query#fragment");

    QCOMPARE(uri.scheme().c_str(),  "scheme");
    QCOMPARE(uri.host().c_str(),    "host");
    QCOMPARE(uri.path().c_str(),    "/path/to/uri");
    QCOMPARE(uri.query().c_str(),   "query");
    QCOMPARE(uri.fragment().c_str(),"fragment");
    QCOMPARE(uri.name().c_str(),    "uri");
}

void UriTest::Parse_Path() {
    {
        Uri uri("C:\\host\\path\\to\\uri.tar.gz");

        QCOMPARE(uri.path().c_str(),    "/host/path/to/uri.tar.gz");
        QCOMPARE(uri.dir().c_str(),     "/host/path/to");
        QCOMPARE(uri.name().c_str(),    "uri.tar.gz");
        QCOMPARE(uri.baseName().c_str(),"uri");
        QCOMPARE(uri.suffix().c_str(),  "tar.gz");
    }
    {
        Uri uri("/host/path/to/uri.tar.gz");

        QCOMPARE(uri.path().c_str(),    "/host/path/to/uri.tar.gz");
        QCOMPARE(uri.dir().c_str(),     "/host/path/to");
        QCOMPARE(uri.name().c_str(),    "uri.tar.gz");
        QCOMPARE(uri.baseName().c_str(),"uri");
        QCOMPARE(uri.suffix().c_str(),  "tar.gz");
    }
}
