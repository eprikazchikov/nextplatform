#include "tst_math.h"

#include "amath.h"

#include <QtTest>
#include <QDebug>

void MathTest::Rotate_to_Matrix() {
    AVector3D rot(1.0f, 2.0f, 3.0f);

    AMatrix4D m;
    m.rotate(rot);

    AVector3D result    = m.euler();
    qDebug() << result.x << result.y << result.z;

    QCOMPARE(result, rot);
}
