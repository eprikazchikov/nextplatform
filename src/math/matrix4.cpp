#include "amath.h"

AMatrix4D::AMatrix4D() {
    identity();
}

AMatrix4D::AMatrix4D(const AMatrix3D &m) {
    mat[0] = m[0]; mat[4] = m[3]; mat[ 8] = m[6]; mat[12] = 0.0;
    mat[1] = m[1]; mat[5] = m[4]; mat[ 9] = m[7]; mat[13] = 0.0;
    mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = 0.0;
    mat[3] = 0.0;  mat[7] = 0.0;  mat[11] = 0.0;  mat[15] = 1.0;
}

AMatrix4D::AMatrix4D(const AMatrix4D &m) {
    mat[0] = m[0]; mat[4] = m[4]; mat[ 8] = m[ 8]; mat[12] = m[12];
    mat[1] = m[1]; mat[5] = m[5]; mat[ 9] = m[ 9]; mat[13] = m[13];
    mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
    mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
}

bool AMatrix4D::operator==(const AMatrix4D &v) const {
    for(int i = 0; i < 16; i++) {
        if(mat[i] != v.mat[i]) {
            return false;
        }
    }
    return true;
}
bool AMatrix4D::operator!=(const AMatrix4D &v) const {
    return !(*this == v);
}

const AVector3D AMatrix4D::operator*(const AVector3D &v) const {
    AVector3D ret;
    ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[ 8] * v[2] + mat[12];
    ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[ 9] * v[2] + mat[13];
    ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14];
    return ret;
}
const AVector4D AMatrix4D::operator*(const AVector4D &v) const {
    AVector4D ret;
    ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12] * v[3];
    ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13] * v[3];
    ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14] * v[3];
    ret[3] = mat[3] * v[0] + mat[7] * v[1] + mat[11] * v[2] + mat[15] * v[3];
    return ret;
}
const AMatrix4D AMatrix4D::operator*(areal f) const {
    AMatrix4D ret;
    ret[0] = mat[0] * f; ret[4] = mat[4] * f; ret[8] = mat[8] * f; ret[12] = mat[12] * f;
    ret[1] = mat[1] * f; ret[5] = mat[5] * f; ret[9] = mat[9] * f; ret[13] = mat[13] * f;
    ret[2] = mat[2] * f; ret[6] = mat[6] * f; ret[10] = mat[10] * f; ret[14] = mat[14] * f;
    ret[3] = mat[3] * f; ret[7] = mat[7] * f; ret[11] = mat[11] * f; ret[15] = mat[15] * f;
    return ret;
}
const AMatrix4D AMatrix4D::operator*(const AMatrix4D &m) const {
    AMatrix4D ret;
    ret[0] = mat[0] * m[0] + mat[4] * m[1] + mat[8] * m[2] + mat[12] * m[3];
    ret[1] = mat[1] * m[0] + mat[5] * m[1] + mat[9] * m[2] + mat[13] * m[3];
    ret[2] = mat[2] * m[0] + mat[6] * m[1] + mat[10] * m[2] + mat[14] * m[3];
    ret[3] = mat[3] * m[0] + mat[7] * m[1] + mat[11] * m[2] + mat[15] * m[3];
    ret[4] = mat[0] * m[4] + mat[4] * m[5] + mat[8] * m[6] + mat[12] * m[7];
    ret[5] = mat[1] * m[4] + mat[5] * m[5] + mat[9] * m[6] + mat[13] * m[7];
    ret[6] = mat[2] * m[4] + mat[6] * m[5] + mat[10] * m[6] + mat[14] * m[7];
    ret[7] = mat[3] * m[4] + mat[7] * m[5] + mat[11] * m[6] + mat[15] * m[7];
    ret[8] = mat[0] * m[8] + mat[4] * m[9] + mat[8] * m[10] + mat[12] * m[11];
    ret[9] = mat[1] * m[8] + mat[5] * m[9] + mat[9] * m[10] + mat[13] * m[11];
    ret[10] = mat[2] * m[8] + mat[6] * m[9] + mat[10] * m[10] + mat[14] * m[11];
    ret[11] = mat[3] * m[8] + mat[7] * m[9] + mat[11] * m[10] + mat[15] * m[11];
    ret[12] = mat[0] * m[12] + mat[4] * m[13] + mat[8] * m[14] + mat[12] * m[15];
    ret[13] = mat[1] * m[12] + mat[5] * m[13] + mat[9] * m[14] + mat[13] * m[15];
    ret[14] = mat[2] * m[12] + mat[6] * m[13] + mat[10] * m[14] + mat[14] * m[15];
    ret[15] = mat[3] * m[12] + mat[7] * m[13] + mat[11] * m[14] + mat[15] * m[15];
    return ret;
}
const AMatrix4D AMatrix4D::operator+(const AMatrix4D &m) const {
    AMatrix4D ret;
    ret[0] = mat[0] + m[0]; ret[4] = mat[4] + m[4]; ret[8] = mat[8] + m[8]; ret[12] = mat[12] + m[12];
    ret[1] = mat[1] + m[1]; ret[5] = mat[5] + m[5]; ret[9] = mat[9] + m[9]; ret[13] = mat[13] + m[13];
    ret[2] = mat[2] + m[2]; ret[6] = mat[6] + m[6]; ret[10] = mat[10] + m[10]; ret[14] = mat[14] + m[14];
    ret[3] = mat[3] + m[3]; ret[7] = mat[7] + m[7]; ret[11] = mat[11] + m[11]; ret[15] = mat[15] + m[15];
    return ret;
}
const AMatrix4D AMatrix4D::operator-(const AMatrix4D &m) const {
    AMatrix4D ret;
    ret[0] = mat[0] - m[0]; ret[4] = mat[4] - m[4]; ret[8] = mat[8] - m[8]; ret[12] = mat[12] - m[12];
    ret[1] = mat[1] - m[1]; ret[5] = mat[5] - m[5]; ret[9] = mat[9] - m[9]; ret[13] = mat[13] - m[13];
    ret[2] = mat[2] - m[2]; ret[6] = mat[6] - m[6]; ret[10] = mat[10] - m[10]; ret[14] = mat[14] - m[14];
    ret[3] = mat[3] - m[3]; ret[7] = mat[7] - m[7]; ret[11] = mat[11] - m[11]; ret[15] = mat[15] - m[15];
    return ret;
}

AMatrix4D &AMatrix4D::operator*=(areal f) {
    return *this = *this * f;
}
AMatrix4D &AMatrix4D::operator*=(const AMatrix4D &m) {
    return *this = *this * m;
}
AMatrix4D &AMatrix4D::operator+=(const AMatrix4D &m) {
    return *this = *this + m;
}
AMatrix4D &AMatrix4D::operator-=(const AMatrix4D &m) {
    return *this = *this - m;
}

areal &AMatrix4D::operator[](int i) {
    return mat[i];
}
const areal AMatrix4D::operator[](int i) const {
    return mat[i];
}

AMatrix3D AMatrix4D::rotation() const {
    AMatrix3D ret;
    ret[0] = mat[0]; ret[3] = mat[4]; ret[6] = mat[ 8];
    ret[1] = mat[1]; ret[4] = mat[5]; ret[7] = mat[ 9];
    ret[2] = mat[2]; ret[5] = mat[6]; ret[8] = mat[10];
    return ret;
}

AMatrix4D AMatrix4D::transpose() const {
    AMatrix4D ret;
    ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[3];
    ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[7];
    ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[11];
    ret[3] = mat[12]; ret[7] = mat[13]; ret[11] = mat[14]; ret[15] = mat[15];
    return ret;
}

areal AMatrix4D::determinant() const {
    areal det;
    det = mat[0] * mat[5] * mat[10];
    det += mat[4] * mat[9] * mat[2];
    det += mat[8] * mat[1] * mat[6];
    det -= mat[8] * mat[5] * mat[2];
    det -= mat[4] * mat[1] * mat[10];
    det -= mat[0] * mat[9] * mat[6];
    return det;
}

AMatrix4D AMatrix4D::inverse() const {
    AMatrix4D ret;
    areal idet = 1.0f / determinant();
    ret[0] =  (mat[5] * mat[10] - mat[9] * mat[6]) * idet;
    ret[1] = -(mat[1] * mat[10] - mat[9] * mat[2]) * idet;
    ret[2] =  (mat[1] * mat[6] - mat[5] * mat[2]) * idet;
    ret[3] = 0.0;
    ret[4] = -(mat[4] * mat[10] - mat[8] * mat[6]) * idet;
    ret[5] =  (mat[0] * mat[10] - mat[8] * mat[2]) * idet;
    ret[6] = -(mat[0] * mat[6] - mat[4] * mat[2]) * idet;
    ret[7] = 0.0;
    ret[8] =  (mat[4] * mat[9] - mat[8] * mat[5]) * idet;
    ret[9] = -(mat[0] * mat[9] - mat[8] * mat[1]) * idet;
    ret[10] =  (mat[0] * mat[5] - mat[4] * mat[1]) * idet;
    ret[11] = 0.0;
    ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
    ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
    ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
    ret[15] = 1.0;
    return ret;
}

void AMatrix4D::zero() {
    mat[0] = 0.0; mat[4] = 0.0; mat[8 ] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 0.0; mat[9 ] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 0.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 0.0;
}

void AMatrix4D::identity() {
    mat[0] = 1.0; mat[4] = 0.0; mat[8 ] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0; mat[9 ] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

void AMatrix4D::rotate(const AVector3D &v, areal angle) {
    AMatrix3D m;
    m.rotate(v, angle);
    *this   = m;
}

void AMatrix4D::rotate(const AVector3D &angles) {
    AMatrix3D m;
    m.rotate(angles);
    *this   = m;
}

void AMatrix4D::scale(const AVector3D &v) {
    mat[0] = v.x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = v.y; mat[9] = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = v.z; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

void AMatrix4D::translate(const AVector3D &v) {
    mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = v.x;
    mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = v.y;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = v.z;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

AVector3D AMatrix4D::euler() {
    return rotation().euler();
}

void AMatrix4D::reflect(const AVector4D &plane) {
    areal x = plane.x;
    areal y = plane.y;
    areal z = plane.z;
    areal x2 = x * 2.0f;
    areal y2 = y * 2.0f;
    areal z2 = z * 2.0f;
    mat[0] = 1.0f - x * x2; mat[4] = -y * x2;       mat[8] = -z * x2;           mat[12] = -plane.w * x2;
    mat[1] = -x * y2;       mat[5] = 1.0f - y * y2; mat[9] = -z * y2;           mat[13] = -plane.w * y2;
    mat[2] = -x * z2;       mat[6] = -y * z2;       mat[10] = 1.0f - z * z2;    mat[14] = -plane.w * z2;
    mat[3] = 0.0;           mat[7] = 0.0;           mat[11] = 0.0;              mat[15] = 1.0;
}

void AMatrix4D::direction(const AVector3D &dir, AVector3D &up) {
    AVector3D z = dir;
    z.normalize();
    AVector3D x = up.cross(z);
    x.normalize();
    AVector3D y = z.cross(x);
    y.normalize();

    mat[0 ] = x.x; mat[4 ] = x.y; mat[8 ] = x.z; mat[12] = 0.0;
    mat[1 ] = y.x; mat[5 ] = y.y; mat[9 ] = y.z; mat[13] = 0.0;
    mat[2 ] = z.x; mat[6 ] = z.y; mat[10] = z.z; mat[14] = 0.0;
    mat[3 ] = 0.0; mat[7 ] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

void AMatrix4D::perspective(areal fov, areal aspect, areal znear, areal zfar) {
    float sine, cotangent, deltaZ;
    float radians   = fov / 2 * DEG2RAD;

    deltaZ          = znear - zfar;
    sine            = sin(radians);
    cotangent       = cos(radians) / sine;

    //identity();

    mat[0]          = cotangent / aspect;
    mat[5]          = cotangent;
    mat[10]         = -(zfar + znear) / (zfar - znear);
    mat[11]         = -1;
    mat[14]         = -(2.0 * zfar * znear) / (zfar - znear);
    mat[15]         = 0;
}

void AMatrix4D::ortho(areal left, areal right, areal bottom, areal top, areal znear, areal zfar) {
    //identity();

    mat[0]          =  2.0f / (right - left);
    mat[5]          =  2.0f / (top - bottom);
    mat[10]         = -2.0f / (zfar - znear);
    mat[12]         = -((right + left) / (right - left));
    mat[13]         = -((top + bottom) / (top - bottom));
    mat[14]         = -((zfar + znear) / (zfar - znear));
}

void AMatrix4D::lookAt(AVector3D &eye, AVector3D &target, AVector3D &up) {
    AMatrix4D m0, m1;

    m0.direction(eye - target, up);

    m1.translate(-eye);
    *this = m0 * m1;
}
