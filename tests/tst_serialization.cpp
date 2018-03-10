#include "tst_serialization.h"

#include "core/objectsystem.h"
#include "core/bson.h"
#include "core/json.h"

#include <QtTest>

#include "tst_common.h"

void SerializationTest::initTestCase() {
    var1["bool"]    = true;
    var1["str"]     = "";
    var1["int"]     = 2;
    var1["float"]   = 3.0f;

    VariantMap m;
    m["bool"]       = true;
    m["str"]        = "true";
    m["int"]        = 1;

    var1["map"]     = m;

    VariantList a;
    a.push_back("string");
    a.push_back(1.0f);
    a.push_back(123);

    var1["array"]   = a;

    var1["vec2"]    = Vector2(1);
    var1["vec3"]    = Vector3(2);
    var1["vec4"]    = Vector4(3);
    var1["quat"]    = Quaternion();
    var1["mat3"]    = Matrix3();
    var1["mat4"]    = Matrix4();
}

void SerializationTest::Json_Serialize_Desirialize() {
    QCOMPARE(Variant(var1), Json::load(Json::save(var1)));
}

void SerializationTest::Bson_Serialize_Desirialize() {
    ByteArray bin   = {'\x00','\x01','\x02','\x03','\x04','\xFF'};
    var1["bin"]     = bin;

    uint32_t offset = 0;
    QCOMPARE(Variant(var1), Bson::load(Bson::save(var1), offset, MetaType::VARIANTMAP));
}
