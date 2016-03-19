#ifndef VECTOR3_H_HEADER_INCLUDED
#define VECTOR3_H_HEADER_INCLUDED

struct AVector3D {
    /// Constructors
    AVector3D() : x(0), y(0), z(0)                                  { }
    AVector3D(float v) : x(v), y(v), z(v)                           { }
    AVector3D(float x, float y, float z) : x(x), y(y), z(z)         { }
    AVector3D(const AVector2D &v, float z) : x(v.x), y(v.y), z(z)   { }
    AVector3D(const AVector3D &v) : x(v.x), y(v.y), z(v.z)          { }
    AVector3D(const float *v) : x(v[0]), y(v[1]), z(v[2])           { }

    /// Comparison operators
    bool operator==(const AVector3D &v) const                   { return (x == v.x) && (y == v.y) && (z == v.z); }
    bool operator!=(const AVector3D &v) const                   { return !(*this == v); }
    bool operator> (const float v) const                        { return (x > v) || (y > v) || (z > v); }
    bool operator> (const AVector3D &v) const                   { return (x > v.x) && (y > v.y) && (z > v.z); }
    bool operator< (const float v) const                        { return (x < v) || (y < v) || (z < v); }
    bool operator< (const AVector3D &v) const                   { return (x < v.x) && (y < v.y) && (z < v.z); }

    /// Math operators
    const AVector3D operator*(float f) const                    { return AVector3D(x * f, y * f, z * f); }
    const AVector3D operator*(const AVector3D &v) const         { return AVector3D(x * v.x, y * v.y, z * v.z); }
    const AVector3D operator/(float f) const                    { return AVector3D(x / f, y / f, z / f); }
    const AVector3D operator+(const AVector3D &v) const         { return AVector3D(x + v.x, y + v.y, z + v.z); }
    const AVector3D operator-() const                           { return AVector3D(-x, -y, -z); }
    const AVector3D operator-(const AVector3D &v) const         { return AVector3D(x - v.x, y - v.y, z - v.z); }

    const AVector3D operator%(const AVector3D &v) const         { return AVector3D(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);}

    /// Increment operators
    AVector3D &operator*=(float f)                              { return *this = *this * f; }
    AVector3D &operator/=(float f)                              { return *this = *this / f; }
    AVector3D &operator+=(const AVector3D &v)                   { return *this = *this + v; }
    AVector3D &operator-=(const AVector3D &v)                   { return *this = *this - v; }
    
    /// Adress operators
    operator float*()                                           { return (float*)&x; }
    operator const float*() const                               { return (float*)&x; }
    
    /// Data operators
    float &operator[](int i)                                    { return v[i]; }
    const float operator[](int i) const                         { return v[i]; }
    
    /// Functions
    float length() const {
        return (float)sqrt(x * x + y * y + z * z);
    }

    float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

    AVector3D cross(const AVector3D &v) {
        return *this % v;
    }

    float dot(const AVector3D &v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    union {
        struct {
            float x, y, z;
        };
        float v[3];
    };
};

#endif /* VECTOR3_H_HEADER_INCLUDED */
