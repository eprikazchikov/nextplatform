#include "math/amath.h"

/*!
    \class AVector3D
    \brief The AVector3D class represents a vector or vertex in 3D space.
    \since Next 1.0
    \inmodule Math

    Vectors are one of the main building blocks of 3D representation and
    drawing. They consist of three coordinates, traditionally called
    x, y, and z.

    The AVector3D class can also be used to represent vertices in 3D space.
    We therefore do not need to provide a separate vertex class.

    \note By design values in the AVector3D instance are stored as \c float.
    This means that on platforms where the \c areal arguments to AVector3D
    functions are represented by \c double values, it is possible to
    lose precision.

    \sa AVector2D, AVector4D, AQuaternion
*/

/*!
    Constructs a null vector, i.e. with coordinates (0, 0, 0).
*/
AVector3D::AVector3D() :
    x(0),
    y(0),
    z(0) {
}
/*!
    Constructs a vector with coordinates (\a v).
*/
AVector3D::AVector3D(areal v) :
    x(v),
    y(v),
    z(v) {
}
/*!
    Constructs a vector with coordinates (\a x, \a y, \a z).
*/
AVector3D::AVector3D(areal x, areal y, areal z) :
    x(x),
    y(y),
    z(z) {
}
/*!
    Constructs a 3D vector from the specified 2D \a v. The z
    coordinate is set to \a z.

    \sa AVector2D::AVector2D()
*/
AVector3D::AVector3D(const AVector2D &v, areal z) :
    x(v.x),
    y(v.y),
    z(z) {
}
/*!
    Constructs a 3D vector from \a v (areal[3] array).
*/
AVector3D::AVector3D(const areal *v) :
    x(v[0]),
    y(v[1]),
    z(v[2]) {
}
/*!
    Returns true if this vector is equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector3D::operator==(const AVector3D &vector) const {
    return (x == vector.x) && (y == vector.y) && (z == vector.z);
}
/*!
    Returns true if this vector is NOT equal to given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector3D::operator!=(const AVector3D &vector) const {
    return !(*this == vector);
}
/*!
    Returns true if this vector is bigger than given \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector3D::operator> (const AVector3D &vector) const {
    return (x > vector.x) && (y > vector.y) && (z > vector.z);
}
/*!
    Returns true if this vector is less than \a vector; otherwise returns false.
    This operator uses an exact floating-point comparison.
*/
bool AVector3D::operator< (const AVector3D &vector) const {
    return (x < vector.x) && (y < vector.y) && (z < vector.z);
}
/*!
    Returns a copy of this vector, multiplied by the given \a factor.

    \sa operator*=()
*/
const AVector3D AVector3D::operator*(areal factor) const {
    return AVector3D(x * factor, y * factor, z * factor);
}
/*!
    Returns a copy of this vector, multiplied by the given \a vector.

    \sa operator*=()
*/
const AVector3D AVector3D::operator*(const AVector3D &vector) const {
    return AVector3D(x * vector.x, y * vector.y, z * vector.z);
}
/*!
    Returns a copy of this vector, divided by the given \a divisor.

    \sa operator/=()
*/
const AVector3D AVector3D::operator/(areal divisor) const {
    return AVector3D(x / divisor, y / divisor, z / divisor);
}
/*!
    Returns a AVector3D object that is the sum of the this vector and \a vector; each component is added separately.

    \sa operator+=()
*/
const AVector3D AVector3D::operator+(const AVector3D &vector) const {
    return AVector3D(x + vector.x, y + vector.y, z + vector.z);
}
/*!
    Returns a AVector3D object that is formed by changing the sign of
    all three components of the this vector.

    Equivalent to \c {AVector3D(0,0,0) - vector}.
*/
const AVector3D AVector3D::operator-() const {
    return AVector3D(-x, -y, -z);
}
/*!
    Returns a AVector3D object that is formed by subtracting \a vector from this vector;
    each component is subtracted separately.

    \sa operator-=()
*/
const AVector3D AVector3D::operator-(const AVector3D &vector) const {
    return AVector3D(x - vector.x, y - vector.y, z - vector.z);
}
/*!
    Multiplies this vector's coordinates by the given \a factor, and
    returns a reference to this vector.

    \sa operator/=()
*/
AVector3D &AVector3D::operator*=(areal factor) {
    return *this = *this * factor;
}
/*!
    Divides this vector's coordinates by the given \a divisor, and
    returns a reference to this vector.

    \sa operator*=()
*/
AVector3D &AVector3D::operator/=(areal divisor) {
    return *this = *this / divisor;
}
/*!
    Adds the given \a vector to this vector and returns a reference to
    this vector.

    \sa operator-=()
*/
AVector3D &AVector3D::operator+=(const AVector3D &vector) {
    return *this = *this + vector;
}
/*!
    Subtracts the given \a vector from this vector and returns a reference to
    this vector.

    \sa operator+=()
*/
AVector3D &AVector3D::operator-=(const AVector3D &vector) {
    return *this = *this - vector;
}
/*!
    Returns the component of the vector at index position i as a modifiable reference.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 3).
*/
areal &AVector3D::operator[](int i) {
    return v[i];
}
/*!
    Returns the component of the vector at index position.
    \a i must be a valid index position in the vector (i.e., 0 <= i < 3).
*/
const areal AVector3D::operator[](int i) const {
    return v[i];
}
/*!
    Returns the length of this vector.

    \sa sqrLength()
*/
areal AVector3D::length() const {
    return (areal)sqrt(sqrLength());
}
/*!
    Returns the squared length of this vector.

    \sa length()
*/
areal AVector3D::sqrLength() const {
    return x * x + y * y + z * z;
}
/*!
    Normalizes the currect vector in place.
    Returns length of prenormalized vector.

    \sa length()
*/
areal AVector3D::normalize() {
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
AVector3D AVector3D::cross(const AVector3D &vector) const {
    return AVector3D(y * vector.z - z * vector.y,
                     z * vector.x - x * vector.z,
                     x * vector.y - y * vector.x);
}
/*!
    Returns the dot-product of this vector and given \a vector.

    \sa cross()
*/
areal AVector3D::dot(const AVector3D &vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}
