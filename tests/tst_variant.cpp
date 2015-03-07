#include "tst_variant.h"

#include "avariant.h"

#include <QTest>

void VariantTest::Set_Get_check() {
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
        std::string result  = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
    {
        AVariant value  = std::string("5");
        std::string result  = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
    {
        AVariant value      = AVector2D(1.0f, 2.0f);
        AVector2D result    = value.toVector2D();
        QCOMPARE(result.x, 1.0f);
        QCOMPARE(result.y, 2.0f);
    }
    {
        AVariant value      = AVector3D(1.0f, 2.0f, 3.0f);
        AVector3D result    = value.toVector3D();
        QCOMPARE(result.x, 1.0f);
        QCOMPARE(result.y, 2.0f);
        QCOMPARE(result.z, 3.0f);
    }
    {
        AVariant value      = AVector4D(1.0f, 2.0f, 3.0f, 4.0f);
        AVector4D result    = value.toVector4D();
        QCOMPARE(result.x, 1.0f);
        QCOMPARE(result.y, 2.0f);
        QCOMPARE(result.z, 3.0f);
        QCOMPARE(result.w, 4.0f);
    }
}

void VariantTest::Value_Copy_to_Shared() {
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
        AVector3D result(0.0f);
        AVariant value = &result;
        QCOMPARE(value.toVector3D().z, 0.0f);

        result.z    = 5.0f;
        QCOMPARE(value.toVector3D().z, 5.0f);

        value       = AVector3D(1.0f);
        QCOMPARE(result.y, 1.0f);
    }
    /// \todo: Add string
}

void VariantTest::Shared_Copy_to_Value() {
    {
        bool result     = true;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toBool(),       true);
    }
    {
        int result      = 1;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toInt(),        1);
    }
    {
        float result    = 2.0f;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toFloat(),      2.0f);
    }
    {
        string result   = "string";
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        QCOMPARE(value2.toString().c_str(),  "string");
    }
    {
        AVector2D result(1.0f, 2.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        AVector2D v     = value2.toVector2D();
        QCOMPARE(v.x, 1.0f);
        QCOMPARE(v.y, 2.0f);
    }
    {
        AVector3D result(1.0f, 2.0f, 3.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        AVector3D v     = value2.toVector3D();
        QCOMPARE(v.x,               1.0f);
        QCOMPARE(v.y,               2.0f);
        QCOMPARE(v.z,               3.0f);
    }
    {
        AVector4D result(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        AVector4D v     = value2.toVector4D();
        QCOMPARE(v.x,               1.0f);
        QCOMPARE(v.y,               2.0f);
        QCOMPARE(v.z,               3.0f);
        QCOMPARE(v.w,               4.0f);
    }
}

void VariantTest::Shared_Copy_to_Shared() {
    /// \todo: Implementation required
}

void VariantTest::Variant_to_List() {
    {
        AVector2D v     = AVector2D(1.0f, 2.0f);
        AVariant value  = &v;
        AVariant::AVariantList list = value.toList();
        float i = 1.0f;
        for(const auto &it : list) {
            QCOMPARE(it.toFloat(), i);
            i += 1.0f;
        }
    }
    {
        AVector3D v     = AVector3D(1.0f, 2.0f, 3.0f);
        AVariant value  = &v;
        AVariant::AVariantList list = value.toList();
        float i = 1.0f;
        for(const auto &it : list) {
            QCOMPARE(it.toFloat(), i);
            i += 1.0f;
        }
    }
    {
        AVector4D v     = AVector4D(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value  = &v;
        AVariant::AVariantList list = value.toList();
        float i = 1.0f;
        for(const auto &it : list) {
            QCOMPARE(it.toFloat(), i);
            i += 1.0f;
        }
    }
}

void VariantTest::Convert_Bool_to_Int_Float_String() {
    {
        AVariant value  = true;
        QVariant result = true;
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString(),  result.toString().toStdString());
    }
}

void VariantTest::Convert_Int_to_Bool_Float_String() {
    {
        AVariant value  = 5;
        QVariant result = 5;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString(),  result.toString().toStdString());
    }
}

void VariantTest::Convert_Float_to_Bool_Int_String() {
    {
        AVariant value  = 6.4f;
        QVariant result = 6.4f;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toString(),  result.toString().toStdString());
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
