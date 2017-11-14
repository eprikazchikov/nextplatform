#include "math/amath.h"

/*!
    \class AVector4D
    \brief The AVector4D class represents a vector or vertex in 4D space.
    \since Next 1.0
    \inmodule Math

    Vectors are one of the main building blocks of 4D representation and
    drawing. They consist of three coordinates, traditionally called
    x, y, z and w.

    The AVector4D class can also be used to represent vertices in 4D space.
    We therefore do not need to provide a separate vertex class.

    \note By design values in the AVector4D instance are stored as \c float.
    This means that on platforms where the \c areal arguments to AVector4D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa AVector2D, AVector3D, AQuaternion
*/

/*!
    Constructs a null vector, i.e. with coordinates (0, 0, 0, 1).
*/
AVector4D::AVector4D() :
    x(0),
    y(0),
    z(0),
    w(1) {
}
/*!
    Constructs a vector with coordinates (\a v).
*/
AVector4D::AVector4D(areal v) :
    x(v),
    y(v),
    z(v),
    w(v) {
}
/*!
    Constructs a vector with coordinates (\a x, \a y, \a z, \a w).
*/
AVector4D::AVector4D(areal x, areal y, areal z, areal w) :
    x(x),
    y(y),
    z(z),
    w(w) {
}
/*!
    Constructs a 4D vector from the specified 2D \a v. The z and w
    coordinates is set to \a z and \a w.

    \sa AVector2D::AVector2D()
*/
AVector4D::AVector4D(const AVector2D &v, areal z, areal w) :
    x(v.x),
    y(v.y),
    z(z),
    w(w) {
}
/*!
    Constructs a 4D vector from the specified 3D \a v. The w
    coordinate is set to \a w.

    \sa AVector3D::AVector3D()
*/
AVector4D::AVector4D(const AVector3D &v, areal w) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(w) {
}
/*!
    Returns true if this vector is equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector4D::operator==(const AVector4D &vector) const {
    return (x == vector.x) && (y == vector.y) && (z == vector.z) && (w == vector.w);
}
/*!
    Returns true if this vector is NOT equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector4D::operator!=(const AVector4D &vector) const {
    return !(*this == vector);
}
/*!
    Returns true if this vector is bigger than given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector4D::operator>(const AVector4D &vector) const {
    return (x > vector.x) && (y > vector.y) && (z > vector.z) && (w > vector.w);
}
/*!
    Returns true if this vector is less than \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector4D::operator<(const AVector4D &vector) const {
    return (x < vector.x) && (y < vector.y) && (z < vector.z) && (w < vector.w);
}
/*!
    Returns a copy of this vector, multiplied by the given \a factor.

    \sa operator*=()
*/
const AVector4D AVector4D::operator*(areal factor) const {
    return AVector4D(x * factor, y * factor, z * factor, w * factor);
}
/*!
    Returns a copy of this vector, multiplied by the given \a vector.

    \sa operator*=()
*/
const AVector4D AVector4D::operator*(const AVector4D &vector) const {
    return AVector4D(x * vector.x, y * vector.y, z * vector.z, w * vector.w);
}
/*!
    Returns a copy of this vector, divided by the given \a divisor.

    \sa operator/=()
*/
const AVector4D AVector4D::operator/(areal divisor) const {
    return AVector4D(x / divisor, y / divisor, z / divisor, w / divisor);
}
/*!
    Returns a AVector4D object that is the sum of the this vector and \a vector; each component is added separately.

    \sa operator+=()
*/
const AVector4D AVector4D::operator+(const AVector4D &vector) const {
    return AVector4D(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
}
/*!
    Returns a AVector4D object that is formed by changing the sign of
    all three components of the this vector.

    Equivalent to \c {AVector4D(0,0,0,1) - vector}.
*/
const AVector4D AVector4D::operator-() const {
    return AVector4D(-x, -y, -z, -w);
}
/*!
    Returns a AVector4D object that is formed by subtracting \a vector from this vector;
    each component is subtracted separately.

    \sa operator-=()
*/
const AVector4D AVector4D::operator-(const AVector4D &vector) const {
    return AVector4D(x - vector.x, y - vector.y, z - vector.z, z - vector.w);
}
/*!
    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.

    \sa operator/=()
*/
AVector4D &AVector4D::operator*=(areal factor) {
    return *this = *this * factor;
}
/*!
    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.

    \sa operator*=()
*/
AVector4D &AVector4D::operator/=(areal divisor) {
    return *this = *this / divisor;
}
/*!
    Adds the given \a vector to this vector and returns a reference to
    this vector.

    \sa operator-=()
*/
AVector4D &AVector4D::operator+=(const AVector4D &vector) {
    return *this = *this + vector;
}
/*!
    Subtracts the given \a vector from this vector and returns a reference to
    this vector.

    \sa operator+=()
*/
AVector4D &AVector4D::operator-=(const AVector4D &vector) {
    return *this = *this - vector;
}
/*!
    Returns the component of the vector at index position i as a modifiable reference.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 4).
*/
areal &AVector4D::operator[](int i) {
    return v[i];
}
/*!
    Returns the component of the vector at index position.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 4).
*/
const areal AVector4D::operator[](int i) const {
    return v[i];
}
/*!
    Returns the length of this vector.

    \sa sqrLength()
*/
areal AVector4D::length() const {
    return (areal)sqrt(sqrLength());
}
/*!
    Returns the squared length of this vector.

    \sa length()
*/
areal AVector4D::sqrLength() const {
    return x * x + y * y + z * z + w * w;
}
/*!
    Normalizes the currect vector in place.
    Returns length of prenormalized vector.

    \sa length()
*/
areal AVector4D::normalize() {
    areal len = length();
    if (len == 0.0f)
        return 0.0f;
    (*this) *= (1.0f / len);

    return len;
}
/*!
    Returns the dot-product of this vector and given \a vector.
*/
areal AVector4D::dot(const AVector4D &vector) const {
    return x * vector.x + y * vector.y + z * vector.z + w * vector.w;
}
