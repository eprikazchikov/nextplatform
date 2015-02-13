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
        AVariant value      = AVector3D(1.0f, 2.0f, 3.0f);
        AVector3D result    = value.toVector();
        QCOMPARE(result.x, 1.0f);
        QCOMPARE(result.y, 2.0f);
        QCOMPARE(result.z, 3.0f);
    }
    {
        AVariant value      = AVariant::Color(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant::Color result  = value.toColor();
        QCOMPARE(result.x, 1.0f);
        QCOMPARE(result.y, 2.0f);
        QCOMPARE(result.z, 3.0f);
        QCOMPARE(result.w, 4.0f);
    }
}

void VariantTest::Value_to_Shared() {
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
        QCOMPARE(value.toVector().z, 0.0f);

        result.z    = 5.0f;
        QCOMPARE(value.toVector().z, 5.0f);

        value       = AVector3D(1.0f);
        QCOMPARE(result.y, 1.0f);
    }
    /// \todo: Add string color etc
}

void VariantTest::Shared_to_Value() {
    {
        bool result     = true;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        QCOMPARE(value2.toBool(),   true);
    }
    {
        int result      = 1;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        QCOMPARE(value2.toInt(),    1);
    }
    {
        float result    = 2.0f;
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        QCOMPARE(value2.toFloat(),  2.0f);
    }
    /// \todo: Add string color etc
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
        AVariant value  = 7.6f;
        QVariant result = 7.6f;
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
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toFloat(),   result.toFloat());
    }
    {
        AVariant value  = "9.6";
        QVariant result = "9.6";
        QCOMPARE(value.toInt(),     result.toInt());
    }
}
