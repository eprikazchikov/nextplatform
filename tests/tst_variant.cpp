#include "tst_variant.h"

#include "avariant.h"

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
        QCOMPARE(value.toString().c_str(),  "6.4");
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
/// \todo: Add structures
void VariantTest::Compare_Variants_Advanced() {
    {
        AVariant::AVariantList value1;
        value1.push_back("Test");
        value1.push_back(true);
        AVariant::AVariantList value2;
        value2.push_back("Test");
        value2.push_back(false);

        QCOMPARE((AVariant(value1) == AVariant(value1)),    true);
        QCOMPARE((AVariant(value1) == AVariant(value2)),    false);
    }
    {
        AVariant::AVariantMap value1;
        value1["NAME"]  = "Test";
        value1["VALUE"] = true;
        AVariant::AVariantMap value2;
        value2["NAME"]  = "Test";
        value2["VALUE"] = false;

        QCOMPARE((AVariant(value1) == AVariant(value1)),    true);
        QCOMPARE((AVariant(value1) == AVariant(value2)),    false);
    }
}
