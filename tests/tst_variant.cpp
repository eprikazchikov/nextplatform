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

void VariantTest::Shared_value() {
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
}

void VariantTest::Convert_value() {
    {
        AVariant value  = true;
        QVariant result = true;
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString(),  result.toString().toStdString());
    }
    {
        AVariant value  = 5;
        QVariant result = 5;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toFloat(),   result.toFloat());
        QCOMPARE(value.toString(),  result.toString().toStdString());
    }
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
