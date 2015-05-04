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

    © Copyright: 2008-2014 Evgeny Prikazchikov
*/

#ifndef VECTOR2_H_HEADER_INCLUDED
#define VECTOR2_H_HEADER_INCLUDED

struct AVector2D {
    /// Constructors
    AVector2D() : x(0), y(0)                                    { }
    AVector2D(float v) : x(v), y(v)                             { }
    AVector2D(float x, float y) : x(x), y(y)                    { }
    AVector2D(const AVector2D &v) : x(v.x), y(v.y)              { }

    /// Comparison operators
    bool operator==(const AVector2D &v) const                   { return (x == v.x) && (y == v.y); }
    bool operator!=(const AVector2D &v) const                   { return !(*this == v); }

    /// Math operators
    const AVector2D operator*(float f) const                    { return AVector2D(x * f, y * f); }
    const AVector2D operator*(AVector2D &v) const               { return AVector2D(x * v.x, y * v.y); }
    const AVector2D operator/(float f) const                    { return AVector2D(x / f, y / f); }
    const AVector2D operator+(const AVector2D &v) const         { return AVector2D(x + v.x, y + v.y); }
    const AVector2D operator-() const                           { return AVector2D(-x, -y); }
    const AVector2D operator-(const AVector2D &v) const         { return AVector2D(x - v.x, y - v.y); }

    /// Increment operators
    AVector2D &operator*=(float f)                              { return *this = *this * f; }
    AVector2D &operator/=(float f)                              { return *this = *this / f; }
    AVector2D &operator+=(const AVector2D &v)                   { return *this = *this + v; }
    AVector2D &operator-=(const AVector2D &v)                   { return *this = *this - v; }

    /// Adress operators
    operator float*()                                           { return (float*)&x; }
    operator const float*() const                               { return (float*)&x; }
    
    /// Data operators
    float &operator[](int i)                                    { return v[i]; }
    const float operator[](int i) const                         { return v[i]; }
    
    /// Functions
    float length() const {
        return (float)sqrt(x * x + y * y);
    }

    float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

    float cross(const AVector2D &v) const {
        return x * v.y - y * v.x;
    }

    float dot(const AVector2D &v) const {
        return x * v.x + y * v.y;
    }

    union {
        struct {
            float x, y;
        };
        float v[2];
    };
};

#endif /* VECTOR2_H_HEADER_INCLUDED */
