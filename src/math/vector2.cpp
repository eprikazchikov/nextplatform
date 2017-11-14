#include "math/amath.h"

/*!
    \class AVector2D
    \brief The AVector2D class represents a vector or vertex in 2D space.
    \since Next 1.0
    \inmodule Math

    Vectors are one of the main building blocks of 2D representation and
    drawing. They consist of three coordinates, traditionally called
    x and y.

    The AVector2D class can also be used to represent vertices in 2D space.
    We therefore do not need to provide a separate vertex class.

    \note By design values in the AVector2D instance are stored as \c float.
    This means that on platforms where the \c areal arguments to AVector2D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa AVector3D, AVector4D, AQuaternion
*/

/*!
    Constructs a null vector, i.e. with coordinates (0, 0).
*/
AVector2D::AVector2D() :
    x(0),
    y(0) {
}
/*!
    Constructs a vector with coordinates (\a v).
*/
AVector2D::AVector2D(areal v) :
    x(v),
    y(v) {
}
/*!
    Constructs a vector with coordinates (\a x, \a y).
*/
AVector2D::AVector2D(areal x, areal y) :
    x(x),
    y(y) {
}
/*!
    Returns true if this vector is equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector2D::operator==(const AVector2D &vector) const {
    return (x == vector.x) && (y == vector.y);
}
/*!
    Returns true if this vector is NOT equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector2D::operator!=(const AVector2D &vector) const {
    return !(*this == vector);
}
/*!
    Returns true if this vector is bigger than given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector2D::operator>(const AVector2D &vector) const {
    return (x > vector.x) && (y > vector.y);
}
/*!
    Returns true if this vector is less than \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector2D::operator<(const AVector2D &vector) const {
    return (x < vector.x) && (y < vector.y);
}
/*!
    Returns a copy of this vector, multiplied by the given \a factor.

    \sa operator*=()
*/
const AVector2D AVector2D::operator*(areal factor) const {
    return AVector2D(x * factor, y * factor);
}
/*!
    Returns a copy of this vector, multiplied by the given \a vector.

    \sa operator*=()
*/
const AVector2D AVector2D::operator*(AVector2D &vector) const {
    return AVector2D(x * vector.x, y * vector.y);
}
/*!
    Returns a copy of this vector, divided by the given \a divisor.

    \sa operator/=()
*/
const AVector2D AVector2D::operator/(areal divisor) const {
    return AVector2D(x / divisor, y / divisor);
}
/*!
    Returns a AVector2D object that is the sum of the this vector and \a vector; each component is added separately.

    \sa operator+=()
*/
const AVector2D AVector2D::operator+(const AVector2D &vector) const {
    return AVector2D(x + vector.x, y + vector.y);
}
/*!
    Returns a AVector2D object that is formed by changing the sign of
    all three components of the this vector.

    Equivalent to \c {AVector2D(0,0) - vector}.
*/
const AVector2D AVector2D::operator-() const {
    return AVector2D(-x, -y);
}
/*!
    Returns a AVector2D object that is formed by subtracting \a vector from this vector;
    each component is subtracted separately.

    \sa operator-=()
*/
const AVector2D AVector2D::operator-(const AVector2D &vector) const {
    return AVector2D(x - vector.x, y - vector.y);
}
/*!
    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.

    \sa operator/=()
*/
AVector2D &AVector2D::operator*=(areal factor) {
    return *this = *this * factor;
}
/*!
    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.

    \sa operator*=()
*/
AVector2D &AVector2D::operator/=(areal divisor) {
    return *this = *this / divisor;
}
/*!
    Adds the given \a vector to this vector and returns a reference to
    this vector.

    \sa operator-=()
*/
AVector2D &AVector2D::operator+=(const AVector2D &vector) {
    return *this = *this + vector;
}
/*!
    Subtracts the given \a vector from this vector and returns a reference to
    this vector.

    \sa operator+=()
*/
AVector2D &AVector2D::operator-=(const AVector2D &vector) {
    return *this = *this - vector;
}
/*!
    Returns the component of the vector at index position i as a modifiable reference.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 2).
*/
areal &AVector2D::operator[](int i) {
    return v[i];
}
/*!
    Returns the component of the vector at index position.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 2).
*/
const areal AVector2D::operator[](int i) const {
    return v[i];
}
/*!
    Returns the length of this vector.

    \sa sqrLength()
*/
areal AVector2D::length() const {
    return (areal)sqrt(sqrLength());
}
/*!
    Returns the squared length of this vector.

    \sa length()
*/
areal AVector2D::sqrLength() const {
    return x * x + y * y;
}
/*!
    Normalizes the currect vector in place.
    Returns length of prenormalized vector.

    \sa length()
*/
areal AVector2D::normalize() {
    areal len = length();
    if (len == 0.0)
        return 0.0;
    (*this) *= (1.0 / len);

    return len;
}
/*!
    Returns the cross-product of this vector and given \a vector.

    \sa dot()
*/
areal AVector2D::cross(const AVector2D &vector) const {
    return x * vector.y - y * vector.x;
}
/*!
    Returns the dot-product of this vector and given \a vector.

    \sa cross()
*/
areal AVector2D::dot(const AVector2D &vector) const {
    return x * vector.x + y * vector.y;
}
