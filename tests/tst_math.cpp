#include "tst_math.h"

#include "math/amath.h"

#include <QDebug>
#include <QQuaternion>

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
