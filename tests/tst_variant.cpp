#include "tst_variant.h"

#include "avariant.h"

#include <QTest>

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
        string result   = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
    {
        AVariant value  = string("5");
        string result   = value.toString();
        QCOMPARE(result.c_str(), "5");
    }
}

void VariantTest::Set_Get_Advanced_Check() {
    {
        AVector2D vector(1.0f, 2.0f);
        AVariant value      = vector;
        AVector2D result    = value.toVector2D();
        QCOMPARE(result, vector);
    }
    {
        AVector3D vector(1.0f, 2.0f, 3.0f);
        AVariant value      = vector;
        AVector3D result    = value.toVector3D();
        QCOMPARE(result, vector);
    }
    {
        AVector4D vector(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value      = vector;
        AVector4D result    = value.toVector4D();
        QCOMPARE(result, vector);
    }
    {
        AQuaternion quaternion(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value      = quaternion;
        AQuaternion result  = value.toQuaternion();
        QCOMPARE(result, quaternion);
    }
    {
        ACurve curve;
        curve.append(0.0f, AVector3D(1.0f), AVector3D(2.0f), AVector3D(3.0f));
        curve.append(4.0f, AVector3D(5.0f), AVector3D(6.0f), AVector3D(7.0f));
        AVariant value      = curve;
        ACurve result       = value.toCurve();
        QCOMPARE((int)result.mList.size(),  2);
        QCOMPARE(result.mList[1].mX,        4.0f);
        QCOMPARE(result.mList[1].mY.x,      5.0f);
        QCOMPARE(result.mList[1].mI.y,      6.0f);
        QCOMPARE(result.mList[1].mO.z,      7.0f);
    }
    {
        AMatrix3D matrix;
        AVariant value      = matrix;
        AMatrix3D result    = value.toMatrix3D();
        QCOMPARE(result[0], matrix[0]);
        QCOMPARE(result[1], matrix[1]);
        QCOMPARE(result[2], matrix[2]);
        QCOMPARE(result[3], matrix[3]);
        QCOMPARE(result[4], matrix[4]);
        QCOMPARE(result[5], matrix[5]);
        QCOMPARE(result[6], matrix[6]);
        QCOMPARE(result[7], matrix[7]);
        QCOMPARE(result[8], matrix[8]);
    }
    {
        AMatrix4D matrix;
        AVariant value      = matrix;
        AMatrix4D result    = value.toMatrix4D();
        QCOMPARE(result[0],  matrix[0]);
        QCOMPARE(result[1],  matrix[1]);
        QCOMPARE(result[2],  matrix[2]);
        QCOMPARE(result[3],  matrix[3]);
        QCOMPARE(result[4],  matrix[4]);
        QCOMPARE(result[5],  matrix[5]);
        QCOMPARE(result[6],  matrix[6]);
        QCOMPARE(result[7],  matrix[7]);
        QCOMPARE(result[8],  matrix[8]);
        QCOMPARE(result[9],  matrix[9]);
        QCOMPARE(result[10], matrix[10]);
        QCOMPARE(result[11], matrix[11]);
        QCOMPARE(result[12], matrix[12]);
        QCOMPARE(result[13], matrix[13]);
        QCOMPARE(result[14], matrix[14]);
        QCOMPARE(result[15], matrix[15]);
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

    /// \todo: Add string
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
    /// \todo: Add Quaterinion, Matrices, Curve
}

void VariantTest::Shared_Copy_to_Value_Basic() {
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
}

void VariantTest::Shared_Copy_to_Value_Advanced() {
    {
        AVector2D result(1.0f, 2.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(),     false);
        AVector2D v     = value2.toVector2D();
        QCOMPARE(v, result);
    }
    {
        AVector3D result(1.0f, 2.0f, 3.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        AVector3D v     = value2.toVector3D();
        QCOMPARE(v, result);
    }
    {
        AVector4D result(1.0f, 2.0f, 3.0f, 4.0f);
        AVariant value1 = &result;
        AVariant value2 = value1;
        QCOMPARE(value2.isShared(), false);
        AVector4D v     = value2.toVector4D();
        QCOMPARE(v, result);
    }
}

void VariantTest::Shared_Copy_to_Shared_Basic() {
    /// \todo: Implementation required
}

void VariantTest::Shared_Copy_to_Shared_Advanced() {
    /// \todo: Implementation required
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

void VariantTest::Compare_Variants_Check() {
    /// \todo: Implementation required
}
