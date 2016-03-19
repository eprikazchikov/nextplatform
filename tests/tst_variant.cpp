#include "tst_variant.h"

#include "avariant.h"

#include "atools.h"

#include <QTest>
#include <QDebug>

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <iomanip>
#include <codecvt>

void VariantTest::Set_Get_Basic_Check() {
    {
        AVariant value  = true;
        bool result     = value.toBool();
        QCOMPARE(result, true);
    }
    {
        AVariant value  = 5;
        int result      = value.toInt();
        QCOMPARE(result, 5);
    }
    {
        AVariant value  = 5.0f;
        float result    = value.toFloat();
        QCOMPARE(result, 5.0f);
    }
    {
        AVariant value  = "5";
        string result  = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
    {
        AVariant value  = string("5");
        string result  = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
}

void VariantTest::Set_Get_Advanced_Check() {
    {
        AVector2D vector(1.0f, 2.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector2D(), vector);
    }
    {
        AVector3D vector(1.0f, 2.0f, 3.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector3D(), vector);
    }
    {
        AVector4D vector(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector4D(), vector);
    }
    {
        AQuaternion quaternion(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value      = quaternion;
        QCOMPARE(value.toQuaternion(), quaternion);
    }
    {
        ACurve curve;
        curve.append(0.0f, AVector3D(1.0f), AVector3D(2.0f), AVector3D(3.0f));
        curve.append(4.0f, AVector3D(5.0f), AVector3D(6.0f), AVector3D(7.0f));
        AVariant value      = curve;
        QCOMPARE(value.toCurve(),  curve);
    }
    {
        AMatrix3D matrix;
        AVariant value      = matrix;
        QCOMPARE(value.toMatrix3D(), matrix);
    }
    {
        AMatrix4D matrix;
        AVariant value      = matrix;
        QCOMPARE(value.toMatrix4D(),    matrix);
    }
}

void VariantTest::Value_Copy_to_Shared_Basic() {
    {
        bool result = false;
        AVariant value = &result;
        QCOMPARE(value.toBool(), false);

        result  = true;
        QCOMPARE(value.toBool(), true);

        value   = false;
        QCOMPARE(result, false);
    }
    {
        int result = 0;
        AVariant value = &result;
        QCOMPARE(value.toInt(), 0);

        result  = 5;
        QCOMPARE(value.toInt(), 5);

        value   = 0;
        QCOMPARE(result, 0);
    }
    {
        float result = 0.0f;
        AVariant value = &result;
        QCOMPARE(value.toFloat(), 0.0f);

        result  = 5.0f;
        QCOMPARE(value.toFloat(), 5.0f);

        value   = 0.0f;
        QCOMPARE(result, 0.0f);
    }
    {
        string result = "0";
        AVariant value = &result;
        QCOMPARE(value.toString().c_str(), "0");

        result  = "6";
        QCOMPARE(value.toString().c_str(), "6");

        value   = "0";
        QCOMPARE(result.c_str(), "0");
    }
}

void VariantTest::Value_Copy_to_Shared_Advanced() {
    {
        AVector2D result(0.0f);
        AVariant value = &result;
        QCOMPARE(value.toVector2D(), result);

        result      = AVector2D(5.0f);
        QCOMPARE(value.toVector2D(), result);

        value       = AVector2D(1.0f);
        QCOMPARE(result, AVector2D(1.0f));
    }
    {
        AVector3D result(0.0f);
        AVariant value = &result;
        QCOMPARE(value.toVector3D(), result);

        result      = AVector3D(5.0f);
        QCOMPARE(value.toVector3D(), result);

        value       = AVector3D(1.0f);
        QCOMPARE(result, AVector3D(1.0f));
    }
    {
        AVector4D result(0.0f);
        AVariant value = &result;
        QCOMPARE(value.toVector4D(), result);

        result      = AVector4D(5.0f);
        QCOMPARE(value.toVector4D(), result);

        value       = AVector4D(1.0f);
        QCOMPARE(result, AVector4D(1.0f));
    }
    {
        AQuaternion result;
        AVariant value = &result;
        QCOMPARE(value.toQuaternion(), result);

        result      = AQuaternion(AVector3D(1.0f), 2.0f);
        QCOMPARE(value.toQuaternion(), result);

        value       = AQuaternion(AVector3D(3.0f), 4.0f);
        QCOMPARE(result, AQuaternion(AVector3D(3.0f), 4.0f));
    }
    {
        AMatrix3D result;
        AVariant value = &result;
        QCOMPARE(value.toMatrix3D(), result);

        AMatrix3D m;
        m.scale(AVector3D(2.0f));
        result      = m;
        QCOMPARE(value.toMatrix3D(), result);
        m.identity();
        m.scale(AVector3D(3.0f));
        value       = m;
        QCOMPARE(result, m);
    }
    {
        AMatrix4D result;
        AVariant value = &result;
        QCOMPARE(value.toMatrix4D(), result);

        AMatrix4D m;
        m.scale(AVector3D(2.0f));
        result      = m;
        QCOMPARE(value.toMatrix4D(), result);
        m.identity();
        m.scale(AVector3D(3.0f));
        value       = m;
        QCOMPARE(result, m);
    }
    /// \todo: Add Curve
}

void VariantTest::Shared_Copy_to_Value_Basic() {
    {
        bool result     = true;
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toBool(),       true);
    }
    {
        int result      = 1;
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toInt(),        1);
    }
    {
        float result    = 2.0f;
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toFloat(),      2.0f);
    }
    {
        string result  = "string";
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toString().c_str(),  "string");
    }
}

void VariantTest::Shared_Copy_to_Value_Advanced() {
    /// \todo: List and map copy
    {
        AVector2D result(1.0f, 2.0f);
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(),     false);
        AVector2D v     = value2.toVector2D();
        QCOMPARE(v, result);
    }
    {
        AVector3D result(1.0f, 2.0f, 3.0f);
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(), false);
        AVector3D v     = value2.toVector3D();
        QCOMPARE(v, result);
    }
    {
        AVector4D result(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value1 = &result;
        AVariant value2 = *value1;
        QCOMPARE(value2.isShared(), false);
        AVector4D v     = value2.toVector4D();
        QCOMPARE(v, result);
    }
}

void VariantTest::Shared_Copy_to_Shared_Basic() {
    {
        bool result1    = true;
        bool result2    = false;
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toBool(),       false);
    }
    {
        int result1     = 0;
        int result2     = 1;
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toInt(),        1);
    }
    {
        float result1   = 0.0f;
        float result2   = 2.0f;
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toFloat(),      2.0f);
    }
    {
        string result1 = "";
        string result2 = "string";
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toString().c_str(), "string");
    }
}

void VariantTest::Shared_Copy_to_Shared_Advanced() {
    {
        AVector2D result1(0.0f);
        AVector2D result2(1.0f);
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toVector2D(),   result2);
    }
    {
        AVector3D result1(0.0f);
        AVector3D result2(2.0f);
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toVector3D(),   result2);
    }
    {
        AVector4D result1(0.0f);
        AVector4D result2(3.0f);
        AVariant value1 = &result1;
        AVariant value2 = &result2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toVector4D(),   result2);
    }
    {
        AQuaternion quaternion1;
        AQuaternion quaternion2(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value1     = &quaternion1;
        AVariant value2     = &quaternion2;
        value1  = value2;
        QCOMPARE(value1.isShared(),     true);
        QCOMPARE(value1.toQuaternion(), quaternion2);
    }
}

void VariantTest::Convert_Bool_to_Int_Float_String() {
    {
        AVariant value  = true;
        QVariant result = true;
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString().c_str(),  result.toString().toStdString().c_str());
    }
}

void VariantTest::Convert_Int_to_Bool_Float_String() {
    {
        AVariant value  = 5;
        QVariant result = 5;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString().c_str(),  result.toString().toStdString().c_str());
    }
}

void VariantTest::Convert_Float_to_Bool_Int_String() {
    {
        AVariant value  = 6.4f;
        QVariant result = 6.4f;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toString().c_str(),  result.toString().toStdString().c_str());
    }
    {
        AVariant value  = 7.5f;
        QVariant result = 7.5f;
        QCOMPARE(value.toInt(),     result.toInt());
    }
}

void VariantTest::Convert_String_to_Bool_Int_Float() {
    {
        AVariant value  = "true";
        QVariant result = "true";
        QCOMPARE(value.toBool(),     result.toBool());
    }
    {
        AVariant value  = "8.4";
        QVariant result = "8.4";
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toInt(),     8);
        QCOMPARE(value.toFloat(),   result.toFloat());
    }
    {
        AVariant value  = "9.6";
        QCOMPARE(value.toInt(),     9);
    }
}

void VariantTest::Compare_Variants_Basic() {
    {
        AVariant value1(true);
        bool flag   = false;
        AVariant value2(&flag);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1(0);
        int flag   = 1;
        AVariant value2(&flag);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1(0.0f);
        float flag  = 1.0f;
        AVariant value2(&flag);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1("test1");
        string flag = "test2";
        AVariant value2(&flag);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
}
/// \todo: Add structures
void VariantTest::Compare_Variants_Advanced() {
    {
        AVariant value1(AVariant::LIST);
        value1.appendProperty("Test");
        value1.appendProperty(true);
        /// \todo: Shared list?
        bool flag   = false;
        AVariant value2(AVariant::LIST);
        value2.appendProperty("Test");
        value2.appendProperty(&flag);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1(AVariant::MAP);
        value1.appendProperty("Test", "NAME");
        value1.appendProperty(true,    "VALUE");
        /// \todo: Shared map?
        bool flag   = false;
        AVariant value2(AVariant::MAP);
        value2.appendProperty("Test", "NAME");
        value2.appendProperty(&flag,   "VALUE"); /// \todo: Need to keep shared

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
}
