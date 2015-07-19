#include "tst_uri.h"

#include "auri.h"

#include <QTest>

void UriTest::Parse_URI() {
    AUri uri("scheme://host/path/to/uri?query#fragment");

    QCOMPARE(uri.scheme().c_str(),  "scheme");
    QCOMPARE(uri.host().c_str(),    "host");
    QCOMPARE(uri.path().c_str(),    "/path/to/uri");
    QCOMPARE(uri.query().c_str(),   "query");
    QCOMPARE(uri.fragment().c_str(),"fragment");
    QCOMPARE(uri.name().c_str(),    "uri");
}
