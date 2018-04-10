#include "tst_math.h"

#include "math/amath.h"

#include <QDebug>
#include <QQuaternion>
#include <QMatrix4x4>

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
    QMatrix4x4 mat(1.46143, 0.0,        0.0,    0.0,
                   0.0,     2.41421,    0.0,    0.0,
                   0.0,     0.0,       -1.0002, 2.24996,
                   0.0,     0.0,       -1.0,    2.44949);

    qDebug() << mat.inverted();

    Matrix4 mat4;
    mat4.mat[0] = 1.46143;  mat4.mat[4] = 0.0;      mat4.mat[ 8] = 0.0;     mat4.mat[12] = 0.0;
    mat4.mat[1] = 0.0;      mat4.mat[5] = 2.41421;  mat4.mat[ 9] = 0.0;     mat4.mat[13] = 0.0;
    mat4.mat[2] = 0.0;      mat4.mat[6] = 0.0;      mat4.mat[10] =-1.0002;  mat4.mat[14] = 2.24996;
    mat4.mat[3] = 0.0;      mat4.mat[7] = 0.0;      mat4.mat[11] =-1.0;     mat4.mat[15] = 2.44949;

    Matrix4 inv = mat4.inverse();
    qDebug() << inv.mat;
}
