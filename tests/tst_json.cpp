#include "tst_json.h"

#include "ajson.h"

#include <QtTest>

void JsonTest::Desirialize() {
    AJson parser;

    QCOMPARE(parser.parse(JSON_STUB2), true);
}
