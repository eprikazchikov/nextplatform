#include "tst_json.h"

#include "ajson.h"

#include <QtTest>

void JsonTest::Serialize_Desirialize() {
    AVariant var1(AVariant::MAP);
    /*
    var1.appendProperty(true,   "test1");
    var1.appendProperty("",     "test2");
    var1.appendProperty(2,      "test3");
    var1.appendProperty(3.0f,   "test4");

    AVariant m(AVariant::MAP);
    m.appendProperty(true,      "test_1");
    m.appendProperty("true",    "test_2");
    m.appendProperty(1,         "test_3");

    var1.appendProperty(m,      "test5");

    var1.appendProperty(AVector2D(1),  "test6");
    var1.appendProperty(AVector3D(2),  "test7");
    var1.appendProperty(AVector4D(3),  "test8");
    var1.appendProperty(AQuaternion(), "test9");
    var1.appendProperty(AMatrix3D(),   "test10");
    var1.appendProperty(AMatrix4D(),   "test11");

    ACurve curve;
    curve.append(0.0f, AVector3D(1.0f), AVector3D(2.0f), AVector3D(3.0f));
    curve.append(4.0f, AVector3D(5.0f), AVector3D(6.0f), AVector3D(7.0f));

    var1.appendProperty(curve,  "test12");
    */
    AVector2D vec2(1);
    var1.appendProperty(&vec2,  "test13");

    string data = AJson::save(var1);

    qDebug() << data.c_str();

    QCOMPARE(var1, AJson::load(data));
}
