#include "tst_math.h"

#include "math/amath.h"

#include <QDebug>
#include <QQuaternion>
#include <QMatrix4x4>

#include <QtTest>

void MathTest::Euler_angles() {
    QVector3D angles(90.0f, 90.0f, 90.0f);
    QQuaternion q   = QQuaternion::fromEulerAngles(angles);
    QVector3D euler = q.toEulerAngles();

    qDebug() << angles << euler;

    {
        Matrix3 m;
        m.rotate(Vector3(90.0f));
        Vector3 euler   = m.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
    {
        Matrix3 m;
        m.rotate(Vector3(180.0f));
        Vector3 euler   = m.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
    {
        Matrix3 m;
        m.rotate(Vector3(360.0f));
        Vector3 euler   = m.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
    {
        Quaternion q(Vector3(90.0f));
        Vector3 euler   = q.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
    {
        Quaternion q(Vector3(180.0f));
        Vector3 euler   = q.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
    {
        Quaternion q(Vector3(360.0f));
        Vector3 euler   = q.euler();
        qDebug() << euler.x << euler.y << euler.z;
    }
}

void MathTest::Matrix_inverse() {
    Matrix4 mat;
    mat.mat[0] = 1.46143f;  mat.mat[4] = 0.0f;      mat.mat[ 8] = 0.0f;     mat.mat[12] = 0.0f;
    mat.mat[1] = 0.0f;      mat.mat[5] = 2.41421f;  mat.mat[ 9] = 0.0f;     mat.mat[13] = 0.0f;
    mat.mat[2] = 0.0f;      mat.mat[6] = 0.0f;      mat.mat[10] =-1.0002f;  mat.mat[14] = 2.24996f;
    mat.mat[3] = 0.0f;      mat.mat[7] = 0.0f;      mat.mat[11] =-1.0f;     mat.mat[15] = 2.44949f;

    QMatrix4x4 m1   = QMatrix4x4(mat.transpose().mat).inverted();
    QMatrix4x4 m2(mat.inverse().transpose().mat);

    qDebug() << m1;
    qDebug() << m2;

    QCOMPARE(m1, m2);
}
