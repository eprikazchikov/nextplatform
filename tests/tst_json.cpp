#include "tst_json.h"

#include "ajson.h"

#include <QtTest>

void JsonTest::Desirialize() {
    AJson json;

    QCOMPARE(json.load(JSON_STUB2), true);

    qDebug() << json.save().c_str();
}
