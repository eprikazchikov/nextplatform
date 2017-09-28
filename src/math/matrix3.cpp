#include "amath.h"

AMatrix3D::AMatrix3D() {
    identity();
}

AMatrix3D::AMatrix3D(const AMatrix3D &m) {
    mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
    mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
    mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
}

const AVector3D AMatrix3D::operator*(const AVector3D &v) const {
    AVector3D ret;
    ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
    ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
    ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
    return ret;
}
const AVector4D AMatrix3D::operator*(const AVector4D &v) const {
    return AVector4D(*this * AVector3D(v.x, v.y, v.z), v.w);
}
const AMatrix3D AMatrix3D::operator*(areal f) const {
    AMatrix3D ret;
    ret[0] = mat[0] * f; ret[3] = mat[3] * f; ret[6] = mat[6] * f;
    ret[1] = mat[1] * f; ret[4] = mat[4] * f; ret[7] = mat[7] * f;
    ret[2] = mat[2] * f; ret[5] = mat[5] * f; ret[8] = mat[8] * f;
    return ret;
}
const AMatrix3D AMatrix3D::operator*(const AMatrix3D &m) const {
    AMatrix3D ret;
    ret[0] = mat[0] * m[0] + mat[3] * m[1] + mat[6] * m[2];
    ret[1] = mat[1] * m[0] + mat[4] * m[1] + mat[7] * m[2];
    ret[2] = mat[2] * m[0] + mat[5] * m[1] + mat[8] * m[2];
    ret[3] = mat[0] * m[3] + mat[3] * m[4] + mat[6] * m[5];
    ret[4] = mat[1] * m[3] + mat[4] * m[4] + mat[7] * m[5];
    ret[5] = mat[2] * m[3] + mat[5] * m[4] + mat[8] * m[5];
    ret[6] = mat[0] * m[6] + mat[3] * m[7] + mat[6] * m[8];
    ret[7] = mat[1] * m[6] + mat[4] * m[7] + mat[7] * m[8];
    ret[8] = mat[2] * m[6] + mat[5] * m[7] + mat[8] * m[8];
    return ret;
}
const AMatrix3D AMatrix3D::operator+(const AMatrix3D &m) const {
    AMatrix3D ret;
    ret[0] = mat[0] + m[0]; ret[3] = mat[3] + m[3]; ret[6] = mat[6] + m[6];
    ret[1] = mat[1] + m[1]; ret[4] = mat[4] + m[4]; ret[7] = mat[7] + m[7];
    ret[2] = mat[2] + m[2]; ret[5] = mat[5] + m[5]; ret[8] = mat[8] + m[8];
    return ret;
}
const AMatrix3D AMatrix3D::operator-(const AMatrix3D &m) const {
    AMatrix3D ret;
    ret[0] = mat[0] - m[0]; ret[3] = mat[3] - m[3]; ret[6] = mat[6] - m[6];
    ret[1] = mat[1] - m[1]; ret[4] = mat[4] - m[4]; ret[7] = mat[7] - m[7];
    ret[2] = mat[2] - m[2]; ret[5] = mat[5] - m[5]; ret[8] = mat[8] - m[8];
    return ret;
}

AMatrix3D &AMatrix3D::operator*=(areal f) {
    return *this = *this * f;
}
AMatrix3D &AMatrix3D::operator*=(const AMatrix3D &m) {
    return *this = *this * m;
}
AMatrix3D &AMatrix3D::operator+=(const AMatrix3D &m) {
    return *this = *this + m;
}
AMatrix3D &AMatrix3D::operator-=(const AMatrix3D &m) {
    return *this = *this - m;
}

bool AMatrix3D::operator==(const AMatrix3D &v) const {
    for(int i = 0; i < 9; i++) {
        if(mat[i] != v.mat[i]) {
            return false;
        }
    }
    return true;
}
bool AMatrix3D::operator!=(const AMatrix3D &v) const {
    return !(*this == v);
}

areal &AMatrix3D::operator[](int i) {
    return mat[i];
}
const areal AMatrix3D::operator[](int i) const {
    return mat[i];
}

AMatrix3D AMatrix3D::transpose() const {
    AMatrix3D ret;
    ret[0] = mat[0]; ret[3] = mat[1]; ret[6] = mat[2];
    ret[1] = mat[3]; ret[4] = mat[4]; ret[7] = mat[5];
    ret[2] = mat[6]; ret[5] = mat[7]; ret[8] = mat[8];
    return ret;
}
areal AMatrix3D::determinant() const {
    areal det;
    det = mat[0] * mat[4] * mat[8];
    det += mat[3] * mat[7] * mat[2];
    det += mat[6] * mat[1] * mat[5];
    det -= mat[6] * mat[4] * mat[2];
    det -= mat[3] * mat[1] * mat[8];
    det -= mat[0] * mat[7] * mat[5];
    return det;
}
AMatrix3D AMatrix3D::inverse() const {
    AMatrix3D ret;
    areal idet = 1.0f / determinant();
    ret[0] =  (mat[4] * mat[8] - mat[7] * mat[5]) * idet;
    ret[1] = -(mat[1] * mat[8] - mat[7] * mat[2]) * idet;
    ret[2] =  (mat[1] * mat[5] - mat[4] * mat[2]) * idet;
    ret[3] = -(mat[3] * mat[8] - mat[6] * mat[5]) * idet;
    ret[4] =  (mat[0] * mat[8] - mat[6] * mat[2]) * idet;
    ret[5] = -(mat[0] * mat[5] - mat[3] * mat[2]) * idet;
    ret[6] =  (mat[3] * mat[7] - mat[6] * mat[4]) * idet;
    ret[7] = -(mat[0] * mat[7] - mat[6] * mat[1]) * idet;
    ret[8] =  (mat[0] * mat[4] - mat[3] * mat[1]) * idet;
    return ret;
}

void AMatrix3D::zero() {
    mat[0] = 0.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = 0.0; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 0.0;
}
void AMatrix3D::identity() {
    mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
}

void AMatrix3D::rotate(const AVector3D &axis, areal angle) {
    areal rad = angle * DEG2RAD;
    areal c = (areal)cos(rad);
    areal s = (areal)sin(rad);
    AVector3D v = axis;
    v.normalize();
    areal xy = v.x * v.y;
    areal yz = v.y * v.z;
    areal zx = v.z * v.x;
    areal xs = v.x * s;
    areal ys = v.y * s;
    areal zs = v.z * s;
    mat[0] = (1.0f - c) * v.x * v.x + c;
    mat[3] = (1.0f - c) * xy - zs;
    mat[6] = (1.0f - c) * zx + ys;

    mat[1] = (1.0f - c) * xy + zs;
    mat[4] = (1.0f - c) * v.y * v.y + c;
    mat[7] = (1.0f - c) * yz - xs;

    mat[2] = (1.0f - c) * zx - ys;
    mat[5] = (1.0f - c) * yz + xs;
    mat[8] = (1.0f - c) * v.z * v.z + c;
}

void AMatrix3D::rotate(const AVector3D &angles) {
    AMatrix3D m;
    m.rotate(AVector3D(1.0f, 0.0f, 0.0f), angles.x);
    *this   *= m;
    m.rotate(AVector3D(0.0f, 1.0f, 0.0f), angles.y);
    *this   *= m;
    m.rotate(AVector3D(0.0f, 0.0f, 1.0f), angles.z);
    *this   *= m;
}

void AMatrix3D::scale(const AVector3D &v) {
    mat[0] = v.x; mat[3] = 0.0; mat[6] = 0.0;
    mat[1] = 0.0; mat[4] = v.y; mat[7] = 0.0;
    mat[2] = 0.0; mat[5] = 0.0; mat[8] = v.z;
}

void AMatrix3D::orthonormalize() {
    AVector3D x(mat[0], mat[1], mat[2]);
    AVector3D y(mat[3], mat[4], mat[5]);
    AVector3D z;
    x.normalize();
    z   = x.cross(y);
    z.normalize();
    y   = z.cross(x);
    y.normalize();
    mat[0] = x.x; mat[3] = y.x; mat[6] = z.x;
    mat[1] = x.y; mat[4] = y.y; mat[7] = z.y;
    mat[2] = x.z; mat[5] = y.z; mat[8] = z.z;
}

AVector3D AMatrix3D::euler() {
    return AVector3D(RAD2DEG * atan2(-mat[7], mat[8]),
                     RAD2DEG * atan2( mat[6], sqrt(mat[7] * mat[7] + mat[8] * mat[8])),
                     RAD2DEG * atan2(-mat[3], mat[0]));
}
