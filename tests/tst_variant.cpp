#include "tst_variant.h"

#include "core/avariant.h"

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
        AVariant value  = 5.0;
        float result    = value.toDouble();
        QCOMPARE(result, 5.0);
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
        Vector2 vector(1.0f, 2.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector2(), vector);
    }
    {
        Vector3 vector(1.0f, 2.0f, 3.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector3(), vector);
    }
    {
        Vector4 vector(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value      = vector;
        QCOMPARE(value.toVector4(), vector);
    }
    {
        Quaternion quaternion(Vector3(1.0f, 2.0f, 3.0f), 4.0f);
        AVariant value      = quaternion;
        QCOMPARE(value.toQuaternion(), quaternion);
    }
    {
        Matrix3 matrix    = Matrix3();
        AVariant value      = matrix;
        QCOMPARE(value.toMatrix3(), matrix);
    }
    {
        Matrix4 matrix;
        AVariant value      = matrix;
        QCOMPARE(value.toMatrix4(),    matrix);
    }
}

void VariantTest::Convert_Bool_to_Int_Float_String() {
    {
        AVariant value  = true;
        QVariant result = true;
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toDouble(),  result.toFloat());
        QCOMPARE(value.toString().c_str(),  result.toString().toStdString().c_str());
    }
}

void VariantTest::Convert_Int_to_Bool_Float_String() {
    {
        AVariant value  = 5;
        QVariant result = 5;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toDouble(),  result.toFloat());
        QCOMPARE(value.toString().c_str(),  result.toString().toStdString().c_str());
    }
}

void VariantTest::Convert_Float_to_Bool_Int_String() {
    {
        AVariant value  = 6.4f;
        QVariant result = 6.4f;
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toInt(),     result.toInt());
        QCOMPARE(value.toString().c_str(),  "6.400000");
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
        QCOMPARE(value.toBool(),    result.toBool());
    }
    {
        AVariant value  = "8.4";
        QVariant result = "8.4";
        QCOMPARE(value.toBool(),    result.toBool());
        QCOMPARE(value.toInt(),     8);
        QCOMPARE(value.toDouble(),  result.toFloat());
    }
    {
        AVariant value  = "9.6";
        QCOMPARE(value.toInt(),     9);
    }
}

void VariantTest::Compare_Variants_Basic() {
    {
        AVariant value1(true);
        AVariant value2(false);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1(0);
        AVariant value2(1);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1(0.0);
        AVariant value2(1.0);

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
    {
        AVariant value1("test1");
        AVariant value2("test2");

        QCOMPARE((value1 == value1),    true);
        QCOMPARE((value1 == value2),    false);
    }
}

void VariantTest::Compare_Variants_Advanced() {
    {
        AVariantList value1;
        value1.push_back("Test");
        value1.push_back(true);
        AVariantList value2;
        value2.push_back("Test");
        value2.push_back(false);

        QCOMPARE((AVariant(value1) == AVariant(value1)),    true);
        QCOMPARE((AVariant(value1) == AVariant(value2)),    false);
    }
    {
        AVariantMap value1;
        value1["NAME"]  = "Test";
        value1["VALUE"] = true;
        AVariantMap value2;
        value2["NAME"]  = "Test";
        value2["VALUE"] = false;

        QCOMPARE((AVariant(value1) == AVariant(value1)),    true);
        QCOMPARE((AVariant(value1) == AVariant(value2)),    false);
    }
}

