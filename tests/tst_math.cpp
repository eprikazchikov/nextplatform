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
}

void MathTest::Generate_Gaussian_Kernel() {
    float kernel[7];
    AGausian::computeKernel(8.0f, kernel, 7);

    qDebug() << kernel[0] << kernel[1] << kernel[2] << kernel[3] << kernel[4] << kernel[5] << kernel[6];
}
