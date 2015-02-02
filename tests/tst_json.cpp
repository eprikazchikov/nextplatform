#include "tst_json.h"

#include "ajson.h"

#include <QtTest>

void JsonTest::Serialize_Desirialize() {
    AJson json1;
    json1.setType(AJson::OBJECT);
    json1.appendProperty(true,  "test1");
    json1.appendProperty("1",   "test2");
    json1.appendProperty(2,     "test3");
    json1.appendProperty(3.3f,  "test4");
    json1.appendProperty(static_cast<AObject *>(NULL), "test5");

    string data = json1.save();

    AJson json2;
    QCOMPARE(json2.load(data), true);

    QCOMPARE((json1 == json2), true);
}
