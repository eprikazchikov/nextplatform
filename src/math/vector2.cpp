#include "amath.h"

AVector2D::AVector2D() :
    x(0),
    y(0) {
}

AVector2D::AVector2D(areal v) :
    x(v),
    y(v) {
}

AVector2D::AVector2D(areal x, areal y) :
    x(x),
    y(y) {
}
AVector2D::AVector2D(const AVector2D &v) :
    x(v.x),
    y(v.y) {
}

bool AVector2D::operator==(const AVector2D &v) const {
    return (x == v.x) && (y == v.y);
}
bool AVector2D::operator!=(const AVector2D &v) const {
    return !(*this == v);
}
bool AVector2D::operator>(const AVector2D &v) const {
    return (x > v.x) && (y > v.y);
}
bool AVector2D::operator<(const AVector2D &v) const {
    return (x < v.x) && (y < v.y);
}

const AVector2D AVector2D::operator*(areal f) const {
    return AVector2D(x * f, y * f);
}
const AVector2D AVector2D::operator*(AVector2D &v) const {
    return AVector2D(x * v.x, y * v.y);
}
const AVector2D AVector2D::operator/(areal f) const {
    return AVector2D(x / f, y / f);
}
const AVector2D AVector2D::operator+(const AVector2D &v) const {
    return AVector2D(x + v.x, y + v.y);
}
const AVector2D AVector2D::operator-() const {
    return AVector2D(-x, -y);
}
const AVector2D AVector2D::operator-(const AVector2D &v) const {
    return AVector2D(x - v.x, y - v.y);
}

AVector2D &AVector2D::operator*=(areal f) {
    return *this = *this * f;
}
AVector2D &AVector2D::operator/=(areal f) {
    return *this = *this / f;
}
AVector2D &AVector2D::operator+=(const AVector2D &v) {
    return *this = *this + v;
}
AVector2D &AVector2D::operator-=(const AVector2D &v) {
    return *this = *this - v;
}

areal &AVector2D::operator[](int i) {
    return v[i];
}
const areal AVector2D::operator[](int i) const {
    return v[i];
}

areal AVector2D::length() const {
    return (areal)sqrt(sqrLength());
}
areal AVector2D::sqrLength() const {
    return x * x + y * y;
}

areal AVector2D::normalize() {
    areal len = length();
    if (len == 0.0)
        return 0.0;
    (*this) *= (1.0 / len);

    return len;
}

areal AVector2D::cross(const AVector2D &v) const {
    return x * v.y - y * v.x;
}

areal AVector2D::dot(const AVector2D &v) const {
    return x * v.x + y * v.y;
}
