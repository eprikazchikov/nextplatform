#include "tst_json.h"

#include "ajson.h"

#include <QtTest>

void JsonTest::Serialize_Desirialize() {
    AVariant var1(AVariant::MAP);
    var1.appendProperty(true,   "test1");
    var1.appendProperty("",     "test2");
    var1.appendProperty(2,      "test3");
    var1.appendProperty(3.0f,   "test4");

    AVariant m(AVariant::MAP);
    m.appendProperty(true,      "test_1");
    m.appendProperty("true",    "test_2");
    m.appendProperty(1,         "test_3");

    var1.appendProperty(m,      "test5");

    var1.appendProperty(AVector2D(),  "test6");
    var1.appendProperty(AVector3D(),  "test7");
    var1.appendProperty(AVector4D(),  "test8");

    QCOMPARE(var1, AJson::load(AJson::save(var1)));
}
