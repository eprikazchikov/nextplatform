#include "amath.h"

AVector4D::AVector4D() :
    x(0),
    y(0),
    z(0),
    w(1) {
}
AVector4D::AVector4D(areal v) :
    x(v),
    y(v),
    z(v),
    w(v) {
}
AVector4D::AVector4D(areal x, areal y, areal z, areal w) :
    x(x),
    y(y),
    z(z),
    w(w) {
}
AVector4D::AVector4D(const AVector2D &v, areal z, areal w) :
    x(v.x),
    y(v.y),
    z(z),
    w(w) {
}
AVector4D::AVector4D(const AVector3D &v, areal w) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(w) {
}
AVector4D::AVector4D(const AVector4D &v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w) {
}

bool AVector4D::operator==(const AVector4D &v) const {
    return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w);
}
bool AVector4D::operator!=(const AVector4D &v) const {
    return !(*this == v);
}
bool AVector4D::operator>(const AVector4D &v) const {
    return (x > v.x) && (y > v.y) && (z > v.z) && (w > v.w);
}
bool AVector4D::operator<(const AVector4D &v) const {
    return (x < v.x) && (y < v.y) && (z < v.z) && (w < v.w);
}

const AVector4D AVector4D::operator*(areal f) const {
    return AVector4D(x * f, y * f, z * f, w * f);
}
const AVector4D AVector4D::operator*(const AVector4D &v) const {
    return AVector4D(x * v.x, y * v.y, z * v.z, w * v.w);
}
const AVector4D AVector4D::operator/(areal f) const {
    return AVector4D(x / f, y / f, z / f, w / f);
}
const AVector4D AVector4D::operator+(const AVector4D &v) const {
    return AVector4D(x + v.x, y + v.y, z + v.z, w + v.w);
}
const AVector4D AVector4D::operator-() const {
    return AVector4D(-x, -y, -z, -w);
}
const AVector4D AVector4D::operator-(const AVector4D &v) const {
    return AVector4D(x - v.x, y - v.y, z - v.z, z - v.w);
}

AVector4D &AVector4D::operator*=(areal f) {
    return *this = *this * f;
}
AVector4D &AVector4D::operator/=(areal f) {
    return *this = *this / f;
}
AVector4D &AVector4D::operator+=(const AVector4D &v) {
    return *this = *this + v;
}
AVector4D &AVector4D::operator-=(const AVector4D &v) {
    return *this = *this - v;
}

areal &AVector4D::operator[](int i) {
    return v[i];
}
const areal AVector4D::operator[](int i) const {
    return v[i];
}

areal AVector4D::length() const {
    return (areal)sqrt(sqrLength());
}
areal AVector4D::sqrLength() const {
    return x * x + y * y + z * z + w * w;
}

areal AVector4D::normalize() {
    areal len = length();
    if (len == 0.0f)
        return 0.0f;
    (*this) *= (1.0f / len);

    return len;
}

areal AVector4D::dot(const AVector4D &v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}
