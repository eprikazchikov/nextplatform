/*
    This file is part of Thunder Next.

    Thunder Next is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Thunder Next is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Thunder Next.  If not, see <http://www.gnu.org/licenses/>.

    � Copyright: 2008-2014 Evgeny Prikazchikov
*/
#ifndef VECTOR4_H_HEADER_INCLUDED
#define VECTOR4_H_HEADER_INCLUDED

struct AVector4D {
    /// Constructors
    AVector4D() : x(0), y(0), z(0), w(1)                                            { }
    AVector4D(float v) : x(v), y(v), z(v), w(v)                                     { }
    AVector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)          { }
    AVector4D(const AVector2D &v, float z, float w) : x(v.x), y(v.y), z(z), w(w)    { }
    AVector4D(const AVector3D &v, float w) : x(v.x), y(v.y), z(v.z), w(w)           { }
    AVector4D(const AVector4D &v) : x(v.x), y(v.y), z(v.z), w(v.w)                  { }
	
    /// Comparison operators
    bool operator==(const AVector4D &v) const                   { return (x == v.x) && (y == v.y) && (z == v.z) && (w == v.w); }
    bool operator!=(const AVector4D &v) const                   { return !(*this == v); }
	
    /// Math operators
    const AVector4D operator*(float f) const                    { return AVector4D(x * f, y * f, z * f, w * f); }
    const AVector4D operator*(const AVector4D &v) const         { return AVector4D(x * v.x, y * v.y, z * v.z, w * v.w); }
    const AVector4D operator/(float f) const                    { return AVector4D(x / f, y / f, z / f, w / f); }
    const AVector4D operator+(const AVector4D &v) const         { return AVector4D(x + v.x, y + v.y, z + v.z, w + v.w); }
    const AVector4D operator-() const                           { return AVector4D(-x, -y, -z, -w); }
    const AVector4D operator-(const AVector4D &v) const         { return AVector4D(x - v.x, y - v.y, z - v.z, z - v.w); }
	
    /// Increment operators
    AVector4D &operator*=(float f)                              { return *this = *this * f; }
    AVector4D &operator/=(float f)                              { return *this = *this / f; }
    AVector4D &operator+=(const AVector4D &v)                   { return *this = *this + v; }
    AVector4D &operator-=(const AVector4D &v)                   { return *this = *this - v; }
	
    /// Adress operators
    operator float*()                                           { return (float*)&x; }
    operator const float*() const                               { return (float*)&x; }
	
    /// Data operators
    float &operator[](int i)                                    { return v[i]; }
    const float operator[](int i) const                         { return v[i]; }
	
    /// Functions
    float length() const {
        return (float)sqrt(x * x + y * y + z * z + w * w);
    }

    float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

    float dot(const AVector4D &v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    union {
        struct {
            float x, y, z, w;
        };
        float v[4];
    };
};

#endif /* MATH_H_HEADER_INCLUDED */
