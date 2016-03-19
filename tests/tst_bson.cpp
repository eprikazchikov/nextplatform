#include "tst_bson.h"

#include "abson.h"

#include <QtTest>

void BsonTest::Serialize_Desirialize() {
    AVariant var1(AVariant::MAP);

    var1.appendProperty(true,   "bool");
    var1.appendProperty("",     "str");
    var1.appendProperty(2,      "int");
    var1.appendProperty(3.0f,   "float");

    AVariant m(AVariant::MAP);
    m.appendProperty(true,      "bool");
    m.appendProperty("true",    "str");
    m.appendProperty(1,         "int");

    var1.appendProperty(m,      "map");

    var1.appendProperty(AVector2D(1),  "vec2");
    var1.appendProperty(AVector3D(2),  "vec3");
    var1.appendProperty(AVector4D(3),  "vec4");
    var1.appendProperty(AQuaternion(), "quat");
    var1.appendProperty(AMatrix3D(),   "mat3");
    var1.appendProperty(AMatrix4D(),   "mat4");

    AVariant a(AVariant::LIST);
    a.appendProperty("string");
    a.appendProperty(1.0f);
    a.appendProperty(123);

    var1.appendProperty(a, "array");

    ACurve curve;
    curve.append(0.0f, AVector3D(1.0f), AVector3D(2.0f), AVector3D(3.0f));
    curve.append(4.0f, AVector3D(5.0f), AVector3D(6.0f), AVector3D(7.0f));

    var1.appendProperty(curve,  "curve");

    AVector2D vec2(1);
    var1.appendProperty(&vec2,  "* vec2");

    AVector3D vec3(2);
    var1.appendProperty(&vec3,  "* vec3");

    AVector4D vec4(3);
    var1.appendProperty(&vec4,  "* vec3");

    AQuaternion quat;
    var1.appendProperty(&quat,  "* quat");

    AMatrix3D mat3;
    var1.appendProperty(&mat3,  "* mat3");

    AMatrix3D mat4;
    var1.appendProperty(&mat4,  "* mat4");

    var1.appendProperty(&curve, "* curve");

    QCOMPARE(var1, ABson::load(ABson::save(var1)));

}
