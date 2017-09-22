#include "amath.h"

AVector3D::AVector3D() :
    x(0),
    y(0),
    z(0) {
}

AVector3D::AVector3D(areal v) :
    x(v),
    y(v),
    z(v) {
}

AVector3D::AVector3D(areal x, areal y, areal z) :
    x(x),
    y(y),
    z(z) {
}

AVector3D::AVector3D(const AVector2D &v, areal z) :
    x(v.x),
    y(v.y),
    z(z) {
}

AVector3D::AVector3D(const AVector3D &v) :
    x(v.x),
    y(v.y),
    z(v.z) {
}

AVector3D::AVector3D(const areal *v) :
    x(v[0]),
    y(v[1]),
    z(v[2]) {
}

bool AVector3D::operator==(const AVector3D &v) const {
    return (x == v.x) && (y == v.y) && (z == v.z);
}
bool AVector3D::operator!=(const AVector3D &v) const {
    return !(*this == v);
}
bool AVector3D::operator> (const AVector3D &v) const {
    return (x > v.x) && (y > v.y) && (z > v.z);
}
bool AVector3D::operator< (const AVector3D &v) const {
    return (x < v.x) && (y < v.y) && (z < v.z);
}

const AVector3D AVector3D::operator*(areal f) const {
    return AVector3D(x * f, y * f, z * f);
}
const AVector3D AVector3D::operator*(const AVector3D &v) const {
    return AVector3D(x * v.x, y * v.y, z * v.z);
}
const AVector3D AVector3D::operator/(areal f) const {
    return AVector3D(x / f, y / f, z / f);
}
const AVector3D AVector3D::operator+(const AVector3D &v) const {
    return AVector3D(x + v.x, y + v.y, z + v.z);
}
const AVector3D AVector3D::operator-() const {
    return AVector3D(-x, -y, -z);
}
const AVector3D AVector3D::operator-(const AVector3D &v) const {
    return AVector3D(x - v.x, y - v.y, z - v.z);
}

AVector3D &AVector3D::operator*=(areal f) {
    return *this = *this * f;
}
AVector3D &AVector3D::operator/=(areal f) {
    return *this = *this / f;
}
AVector3D &AVector3D::operator+=(const AVector3D &v) {
    return *this = *this + v;
}
AVector3D &AVector3D::operator-=(const AVector3D &v) {
    return *this = *this - v;
}

areal &AVector3D::operator[](int i) {
    return v[i];
}
const areal AVector3D::operator[](int i) const {
    return v[i];
}

areal AVector3D::length() const {
    return (areal)sqrt(sqrLength());
}
areal AVector3D::sqrLength() const {
    return x * x + y * y + z * z;
}

areal AVector3D::normalize() {
    areal len = length();
    if (len == 0.0)
        return 0.0;
    (*this) *= (1.0 / len);

    return len;
}

AVector3D AVector3D::cross(const AVector3D &v) const {
    return AVector3D(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
}

areal AVector3D::dot(const AVector3D &v) const {
    return x * v.x + y * v.y + z * v.z;
}
