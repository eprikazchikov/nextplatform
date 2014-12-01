#ifndef AMATH_H_HEADER_INCLUDED
#define AMATH_H_HEADER_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <vector>

#define EPSILON 1e-6f
#define PI 3.14159265358979323846f
#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

#define MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

struct AVector2D;
struct AVector3D;
struct AVector4D;
struct AMatrix3D;
struct AMatrix4D;
struct AQuaternionernion;

/*****************************************************************************/
/*                                                                           */
/* AVector2D                                                                 */
/*                                                                           */
/*****************************************************************************/

struct AVector2D {
    /// Constructors
    inline AVector2D() : x(0), y(0)                             { }
    inline AVector2D(float v) : x(v), y(v)                      { }
    inline AVector2D(float x, float y) : x(x), y(y)             { }
    inline AVector2D(const float *v) : x(v[0]), y(v[1])         { }
    inline AVector2D(const AVector2D &v) : x(v.x), y(v.y)       { }
    /// Comparison operators
    inline int operator==(const AVector2D &v)                   { return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON); }
    inline int operator!=(const AVector2D &v)                   { return !(*this == v); }
    /// Math operators
    inline const AVector2D operator*(float f) const             { return AVector2D(x * f, y * f); }
    inline const AVector2D operator*(AVector2D &v) const {
    }

    inline const AVector2D operator/(float f) const             { return AVector2D(x / f, y / f); }
    inline const AVector2D operator+(const AVector2D &v) const  { return AVector2D(x + v.x, y + v.y); }
    inline const AVector2D operator-() const                    { return AVector2D(-x, -y); }
    inline const AVector2D operator-(const AVector2D &v) const  { return AVector2D(x - v.x, y - v.y); }
    /// Increment operators
    inline AVector2D &operator*=(float f)                       { return *this = *this * f; }
    inline AVector2D &operator/=(float f)                       { return *this = *this / f; }
    inline AVector2D &operator+=(const AVector2D &v)            { return *this = *this + v; }
    inline AVector2D &operator-=(const AVector2D &v)            { return *this = *this - v; }
    /// Adress operators
    inline operator float*()                                    { return (float*)&x; }
    inline operator const float*() const                        { return (float*)&x; }
    /// Data operators
    inline float &operator[](int i)                             { return ((float*)&x)[i]; }
    inline const float operator[](int i) const                  { return ((float*)&x)[i]; }
    /// Functions
    inline float length() const {
        return (float)sqrt(x * x + y * y);
    }

    inline float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

    inline float cross(const AVector2D &v) const {  // Cross product
        return x * v.y - y * v.x;
    }

    inline float dot(const AVector2D &v) {          // Dot product
        return x * v.x + y * v.y;
    }

    inline AVector2D direction () {
        AVector2D temp(*this);
        temp.normalize();

        return temp;
    }
	
    inline void rotate(float angle, float x, float y) {
        float cosTheta = (float)cos(angle);
        float sinTheta = (float)sin(angle);

        rotate(sinTheta, cosTheta, x, y);
    }

    inline void rotate(float sinTheta, float cosTheta, float x, float y) {
        AVector2D vNewPos;

        vNewPos.x    = (cosTheta + (1 - cosTheta) * x * x)   * this->x;
        vNewPos.x   += ((1 - cosTheta) * x * y)              * this->y;

        vNewPos.y    = ((1 - cosTheta) * x * y)              * this->x;
        vNewPos.y   += (cosTheta + (1 - cosTheta) * y * y)   * this->y;

        *this       = vNewPos;
    }

    union {
        struct {
            float x, y;
        };
        float v[2];
    };
};

/*****************************************************************************/
/*                                                                           */
/* AVector3D                                                                 */
/*                                                                           */
/*****************************************************************************/

struct AVector3D {
    /// Constructors
    inline AVector3D() : x(0), y(0), z(0)                           { }
    inline AVector3D(float v) : x(v), y(v), z(v)                    { }
    inline AVector3D(float x, float y, float z) : x(x), y(y), z(z)  { }
    inline AVector3D(const float *v) : x(v[0]), y(v[1]), z(v[2])    { }
    inline AVector3D(const AVector3D &v) : x(v.x), y(v.y), z(v.z)   { }
    inline AVector3D(const AVector4D &v);
    /// Comparison operators
    inline int operator==(const AVector3D &v)                       { return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON && fabs(z - v.z) < EPSILON); }
    inline int operator!=(const AVector3D &v)                       { return !(*this == v); }
    /// Math operators
    inline const AVector3D operator*(float f) const                 { return AVector3D(x * f, y * f, z * f); }
    inline const AVector3D operator*(AVector3D &v) const {
        AVector3D res;
        res.x = y * v.z - z * v.y;
        res.y = z * v.x - x * v.z;
        res.z = x * v.y - y * v.x;
        return (res);
    }

    inline const AVector3D operator/(float f) const                 { return AVector3D(x / f, y / f, z / f); }
    inline const AVector3D operator+(const AVector3D &v) const      { return AVector3D(x + v.x, y + v.y, z + v.z); }
    inline const AVector3D operator-() const                        { return AVector3D(-x, -y, -z); }
    inline const AVector3D operator-(const AVector3D &v) const      { return AVector3D(x - v.x, y - v.y, z - v.z); }
    /// Increment operators
    inline AVector3D &operator*=(float f)                           { return *this = *this * f; }
    inline AVector3D &operator/=(float f)                           { return *this = *this / f; }
    inline AVector3D &operator+=(const AVector3D &v)                { return *this = *this + v; }
    inline AVector3D &operator-=(const AVector3D &v)                { return *this = *this - v; }
    /// Adress operators
    inline operator float*()                                        { return (float*)&x; }
    inline operator const float*() const                            { return (float*)&x; }
    /// Data operators
    inline float &operator[](int i)                                 { return ((float*)&x)[i]; }
    inline const float operator[](int i) const                      { return ((float*)&x)[i]; }
    /// Functions
    inline float length() const {
        return (float)sqrt(x * x + y * y + z * z);
    }

    inline float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

    inline AVector3D cross(const AVector3D &v) {    // Cross product
        AVector3D ret;
        ret.x   = y * v.z - z * v.y;
        ret.y   = z * v.x - x * v.z;
        ret.z   = x * v.y - y * v.x;
        return ret;
    }

    inline float dot(const AVector3D &v) {          // Dot product
        return x * v.x + y * v.y + z * v.z;
    }

    inline void rotate(float angle, float x, float y, float z) {
        float cosTheta = (float)cos(angle);
        float sinTheta = (float)sin(angle);

        rotate(sinTheta, cosTheta, x, y, z);
    }

    inline void rotate(float sinTheta, float cosTheta, float x, float y, float z) {
        AVector3D vNewPos;

        vNewPos.x  = (cosTheta + (1 - cosTheta) * x * x)		* this->x;
        vNewPos.x += ((1 - cosTheta) * x * y - z * sinTheta)	* this->y;
        vNewPos.x += ((1 - cosTheta) * x * z + y * sinTheta)	* this->z;

        vNewPos.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* this->x;
        vNewPos.y += (cosTheta + (1 - cosTheta) * y * y)		* this->y;
        vNewPos.y += ((1 - cosTheta) * y * z - x * sinTheta)	* this->z;

        vNewPos.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* this->x;
        vNewPos.z += ((1 - cosTheta) * y * z + x * sinTheta)	* this->y;
        vNewPos.z += (cosTheta + (1 - cosTheta) * z * z)		* this->z;

        this->x	= vNewPos.x;
        this->y = vNewPos.y;
        this->z	= vNewPos.z;
    }

    inline AVector3D closest_point_on_line(const AVector3D &a, const AVector3D &b, const AVector3D &p) {
        AVector3D c		= p - a;
        AVector3D v		= b - a;
        float d			= v.normalize();
        // скал€рное произведение векторов
        float t			= v.dot(c);
        // проверка на выход за границы отрезка
        if ( t < 0.0f )
            return a;
        if ( t > d )
            return b;
        // ¬ернем точку между a и b
        v *= t;
        return ( a + v );
    }

    inline void ortogonalize(const AVector3D &v1, const AVector3D &v2) {
        AVector3D v2ProjV1  = closest_point_on_line( v1, -v1, v2 );
        AVector3D res       = v2 - v2ProjV1;
        res.normalize();

        *this   = res;
    }

    union {
        struct {
            float x, y, z;
        };
        float v[3];
    };
};

/*****************************************************************************/
/*                                                                           */
/* AVector4D                                                                 */
/*                                                                           */
/*****************************************************************************/

struct AVector4D {
    inline AVector4D() : x(0), y(0), z(0), w(1) { }
    inline AVector4D(float v) : x(v), y(v), z(v), w(v) { }
    inline AVector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { }
    inline AVector4D(const float *v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
	inline AVector4D(const AVector3D &v) : x(v.x), y(v.y), z(v.z), w(1) { }
	inline AVector4D(const AVector3D &v,float w) : x(v.x), y(v.y), z(v.z), w(w) { }
	inline AVector4D(const AVector4D &v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
	
	inline int operator==(const AVector4D &v) { return (fabs(x - v.x) < EPSILON && fabs(y - v.y) < EPSILON && fabs(z - v.z) < EPSILON && fabs(w - v.w) < EPSILON); }
	inline int operator!=(const AVector4D &v) { return !(*this == v); }
	
	inline const AVector4D operator*(float f) const { return AVector4D(x * f,y * f,z * f,w * f); }
	inline const AVector4D operator/(float f) const { return AVector4D(x / f,y / f,z / f,w / f); }
	inline const AVector4D operator+(const AVector4D &v) const { return AVector4D(x + v.x,y + v.y,z + v.z,w + v.w); }
	inline const AVector4D operator-() const { return AVector4D(-x,-y,-z,-w); }
	inline const AVector4D operator-(const AVector4D &v) const { return AVector4D(x - v.x,y - v.y,z - v.z,z - v.w); }
	
	inline AVector4D &operator*=(float f) { return *this = *this * f; }
	inline AVector4D &operator/=(float f) { return *this = *this / f; }
	inline AVector4D &operator+=(const AVector4D &v) { return *this = *this + v; }
	inline AVector4D &operator-=(const AVector4D &v) { return *this = *this - v; }
	
	inline float operator*(const AVector4D &v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
	
	inline operator float*() { return (float*)&x; }
	inline operator const float*() const { return (float*)&x; }
	
	inline float &operator[](int i) { return ((float*)&x)[i]; }
	inline const float operator[](int i) const { return ((float*)&x)[i]; }
	
    /// Functions
    inline float length() const {
        return (float)sqrt(x * x + y * y + z * z + w * w);
    }

    inline float normalize() {
        float len = length();
        if (len == 0.0f)
            return 0.0f;
        (*this) *= (1.0f / len);

        return len;
    }

	union {
        struct {
            float x, y, z, w;
        };
        float v[4];
	};
};

inline AVector3D::AVector3D(const AVector4D &v) {
    x       = v.x / v.w;
    y       = v.y / v.w;
    z       = v.z / v.w;
}

/*****************************************************************************/
/*                                                                           */
/* ARange                                                                    */
/*                                                                           */
/*****************************************************************************/

struct ARange {
    inline ARange       () : mMin(0), mMax(1)                         { }
    inline ARange       (float min, float max) : mMin(min), mMax(max) { }

    inline float        frand() {
        return ((mMax - mMin)*((float)rand()/RAND_MAX)) + mMin;
    }

    union {
        struct {
            float   mMin;
            float   mMax;
        };
        float v[2];
    };

};

/*****************************************************************************/
/*                                                                           */
/* AVector3DRange                                                            */
/*                                                                           */
/*****************************************************************************/

struct ARange3D {
    inline ARange3D   () : mMin(0.0f), mMax(1.0f)                           { }
    inline ARange3D   (AVector3D min, AVector3D max) : mMin(min), mMax(max) { }

    inline AVector3D        frand() {
        AVector3D v;
        v.x     = ((mMax.x - mMin.x)*((float)rand()/RAND_MAX)) + mMin.x;
        v.y     = ((mMax.y - mMin.y)*((float)rand()/RAND_MAX)) + mMin.y;
        v.z     = ((mMax.z - mMin.z)*((float)rand()/RAND_MAX)) + mMin.z;
        return v;
    }

    AVector3D       mMin;
    AVector3D       mMax;
};

/*****************************************************************************/
/*                                                                           */
/* ACurve3D                                                                  */
/*                                                                           */
/*****************************************************************************/

struct ACurvePoint {
    inline ACurvePoint () {

    }

    inline ACurvePoint (float x, AVector3D &y, AVector3D &i, AVector3D &o) {
        mX  = x; mY = y; mI = i; mO = o;
    }

    float           mX;
    AVector3D       mY;
    AVector3D       mI;
    AVector3D       mO;
};

struct ACurve {
    typedef std::vector<ACurvePoint>    vector_point;

    inline ACurve () {
    }

    inline ACurve (ACurvePoint &p) {
        append (p);
    }

    inline ACurve (float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        append (x, y, in, out);
    }

    inline void append (ACurvePoint &p) {
        pList.push_back(p);
    }

    inline void append (float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        append(p);
    }

    inline void insert (size_t index, ACurvePoint &p) {
        pList.insert(pList.begin() + index, p);
    }

    inline void insert (size_t index, float x, AVector3D &y, AVector3D &in, AVector3D &out) {
        ACurvePoint p(x, y, in, out);
        insert(index, p);
    }

    inline void erase (size_t index) {
        pList.erase(pList.begin() + index);
    }

    inline size_t size () {
        return pList.size();
    }

    inline ACurvePoint &point (size_t index) {
        if(index < pList.size()) {
            return pList[index];
        }
    }

    inline void point (float pos, size_t &index, AVector3D &value) {
        for(unsigned int i = 0; i < pList.size(); i++) {
            value       = pList[i].mY;
            index       = i;
            int o       = i - 1;
            if(o >= 0) {
                if(pList[o].mX <= pos && pos <= pList[i].mX) {
                    float u         = (pos - pList[o].mX) / (pList[i].mX - pList[o].mX);

                    AVector3D p1    = pList[o].mY;
                    AVector3D p2    = pList[i].mY;

                    AVector3D s1    = p1 + AVector3D(tan(-pList[o].mO.x * DEG2RAD),
                                                     tan(-pList[o].mO.y * DEG2RAD),
                                                     tan(-pList[o].mO.z * DEG2RAD));

                    AVector3D s2    = p2 - AVector3D(tan( pList[i].mI.x * DEG2RAD),
                                                     tan( pList[i].mI.y * DEG2RAD),
                                                     tan( pList[i].mI.z * DEG2RAD));

                    AVector3D a     = p2 * pow(u, 3);
                    AVector3D b     = s2 * (3 * pow(u, 2) * (1 - u));
                    AVector3D c     = s1 * (3 * u * pow((1 - u), 2));
                    AVector3D d     = p1 * (pow((1 - u), 3));

                    value           = (a + b) + (c + d);
                    return;
                }
            }
        }
    }

    inline void sort (unsigned int &index) {
        for(unsigned int i = 0; i < pList.size(); i++) {
            ACurvePoint p1  = pList[i];
            unsigned int o          = i + 1;
            if(o < pList.size()) {
                ACurvePoint p2  = pList[o];
                if(p1.mX > p2.mX) {
                    if(index == i)
                        index       = o;
                    else if(index == o)
                        index       = i;

                    pList[i]    = p2;
                    pList[o]    = p1;
                    sort(index);
                }
            }
        }
    }

    vector_point            pList;
};

/*****************************************************************************/
/*                                                                           */
/* APerlin                                                                   */
/*                                                                           */
/*****************************************************************************/

struct APerlin {
    static inline float compile(float x, float y) {
        float intx          = int(x);       // цела€ часть х
        float fractionalx   = x - intx;     // дробь от х

        float inty          = int(y);
        float fractionaly   = y - inty;
        //получаем 4 сглаженных значени€
        float v1    = smoothed(intx,     inty);
        float v2    = smoothed(intx + 1, inty);
        float v3    = smoothed(intx,     inty + 1);
        float v4    = smoothed(intx + 1, inty + 1);
        //интерполируем значени€ 1 и 2 пары и производим интерпол€цию между ними
        float i1    = lerp(v1, v2, fractionalx);
        float i2    = lerp(v3, v4, fractionalx);

        return corp(i1, i2, fractionaly);
    }

    static inline float smoothed(float x, float y) {
        float corners = (noise2d(x - 1, y - 1) + noise2d(x + 1, y - 1) + noise2d(x - 1, y + 1) + noise2d(x + 1, y + 1) ) / 16;
        float sides   = (noise2d(x - 1, y) + noise2d(x + 1, y) + noise2d(x, y - 1) + noise2d(x, y + 1) ) /  8;
        float center  =  noise2d(x, y) / 4;

        return corners + sides + center;
    }

    static inline float noise2d(float x, float y) {
        int n   = x + y * 57;
        n       = (n << 13) ^ n;
        return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    }

    static float lerp(float a, float b, float x) {
        return a * (1 - x) + b * x;
    }

    static float corp(float a, float b, float x) {
        float f = (1.0f - (float)cos(x * PI)) * 0.5f;
        return lerp(a, b, f);
    }
};

/*****************************************************************************/
/*                                                                           */
/* AMatrix3D                                                                 */
/*                                                                           */
/*****************************************************************************/

struct AMatrix3D {
    AMatrix3D() {
        mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
        mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
        mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
    }
	AMatrix3D(const float *m) {
		mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
		mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
		mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
	}
	AMatrix3D(const AMatrix3D &m) {
		mat[0] = m[0]; mat[3] = m[3]; mat[6] = m[6];
		mat[1] = m[1]; mat[4] = m[4]; mat[7] = m[7];
		mat[2] = m[2]; mat[5] = m[5]; mat[8] = m[8];
	}

    AMatrix3D(const AMatrix4D &m);
	
	AVector3D operator*(const AVector3D &v) const {
		AVector3D ret;
		ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
		ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
		ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
		return ret;
	}
	AVector4D operator*(const AVector4D &v) const {
		AVector4D ret;
		ret[0] = mat[0] * v[0] + mat[3] * v[1] + mat[6] * v[2];
		ret[1] = mat[1] * v[0] + mat[4] * v[1] + mat[7] * v[2];
		ret[2] = mat[2] * v[0] + mat[5] * v[1] + mat[8] * v[2];
		ret[3] = v[3];
		return ret;
	}
	AMatrix3D operator*(float f) const {
		AMatrix3D ret;
		ret[0] = mat[0] * f; ret[3] = mat[3] * f; ret[6] = mat[6] * f;
		ret[1] = mat[1] * f; ret[4] = mat[4] * f; ret[7] = mat[7] * f;
		ret[2] = mat[2] * f; ret[5] = mat[5] * f; ret[8] = mat[8] * f;
		return ret;
	}
	AMatrix3D operator*(const AMatrix3D &m) const {
		AMatrix3D ret;
		ret[0] = mat[0] * m[0] + mat[3] * m[1] + mat[6] * m[2];
		ret[1] = mat[1] * m[0] + mat[4] * m[1] + mat[7] * m[2];
		ret[2] = mat[2] * m[0] + mat[5] * m[1] + mat[8] * m[2];
		ret[3] = mat[0] * m[3] + mat[3] * m[4] + mat[6] * m[5];
		ret[4] = mat[1] * m[3] + mat[4] * m[4] + mat[7] * m[5];
		ret[5] = mat[2] * m[3] + mat[5] * m[4] + mat[8] * m[5];
		ret[6] = mat[0] * m[6] + mat[3] * m[7] + mat[6] * m[8];
		ret[7] = mat[1] * m[6] + mat[4] * m[7] + mat[7] * m[8];
		ret[8] = mat[2] * m[6] + mat[5] * m[7] + mat[8] * m[8];
		return ret;
	}
	AMatrix3D operator+(const AMatrix3D &m) const {
		AMatrix3D ret;
		ret[0] = mat[0] + m[0]; ret[3] = mat[3] + m[3]; ret[6] = mat[6] + m[6];
		ret[1] = mat[1] + m[1]; ret[4] = mat[4] + m[4]; ret[7] = mat[7] + m[7];
		ret[2] = mat[2] + m[2]; ret[5] = mat[5] + m[5]; ret[8] = mat[8] + m[8];
		return ret;
	}
	AMatrix3D operator-(const AMatrix3D &m) const {
		AMatrix3D ret;
		ret[0] = mat[0] - m[0]; ret[3] = mat[3] - m[3]; ret[6] = mat[6] - m[6];
		ret[1] = mat[1] - m[1]; ret[4] = mat[4] - m[4]; ret[7] = mat[7] - m[7];
		ret[2] = mat[2] - m[2]; ret[5] = mat[5] - m[5]; ret[8] = mat[8] - m[8];
		return ret;
	}
	
	AMatrix3D &operator*=(float f) { return *this = *this * f; }
	AMatrix3D &operator*=(const AMatrix3D &m) { return *this = *this * m; }
	AMatrix3D &operator+=(const AMatrix3D &m) { return *this = *this + m; }
	AMatrix3D &operator-=(const AMatrix3D &m) { return *this = *this - m; }
	
	operator float*() { return mat; }
	operator const float*() const { return mat; }
	
	float &operator[](int i) { return mat[i]; }
	const float operator[](int i) const { return mat[i]; }
	
	AMatrix3D transpose() const {
		AMatrix3D ret;
		ret[0] = mat[0]; ret[3] = mat[1]; ret[6] = mat[2];
		ret[1] = mat[3]; ret[4] = mat[4]; ret[7] = mat[5];
		ret[2] = mat[6]; ret[5] = mat[7]; ret[8] = mat[8];
		return ret;
	}
	float det() const {
		float det;
		det = mat[0] * mat[4] * mat[8];
		det += mat[3] * mat[7] * mat[2];
		det += mat[6] * mat[1] * mat[5];
		det -= mat[6] * mat[4] * mat[2];
		det -= mat[3] * mat[1] * mat[8];
		det -= mat[0] * mat[7] * mat[5];
		return det;
	}
	AMatrix3D inverse() const {
		AMatrix3D ret;
		float idet = 1.0f / det();
		ret[0] =  (mat[4] * mat[8] - mat[7] * mat[5]) * idet;
		ret[1] = -(mat[1] * mat[8] - mat[7] * mat[2]) * idet;
		ret[2] =  (mat[1] * mat[5] - mat[4] * mat[2]) * idet;
		ret[3] = -(mat[3] * mat[8] - mat[6] * mat[5]) * idet;
		ret[4] =  (mat[0] * mat[8] - mat[6] * mat[2]) * idet;
		ret[5] = -(mat[0] * mat[5] - mat[3] * mat[2]) * idet;
		ret[6] =  (mat[3] * mat[7] - mat[6] * mat[4]) * idet;
		ret[7] = -(mat[0] * mat[7] - mat[6] * mat[1]) * idet;
		ret[8] =  (mat[0] * mat[4] - mat[3] * mat[1]) * idet;
		return ret;
	}
	
	void zero() {
		mat[0] = 0.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = 0.0; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 0.0;
	}
	void identity() {
		mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
	}
	void rotate(float angle, const AVector3D &axis) {
		float rad = angle * DEG2RAD;
		float c = (float)cos(rad);
		float s = (float)sin(rad);
		AVector3D v = axis;
		v.normalize();
		float xy = v.x * v.y;
		float yz = v.y * v.z;
		float zx = v.z * v.x;
		float xs = v.x * s;
		float ys = v.y * s;
		float zs = v.z * s;
		mat[0] = (1.0f - c) * v.x * v.x + c; mat[3] = (1.0f - c) * xy - zs; mat[6] = (1.0f - c) * zx + ys;
		mat[1] = (1.0f - c) * xy + zs; mat[4] = (1.0f - c) * v.y * v.y + c; mat[7] = mat[6] = (1.0f - c) * yz - xs;
		mat[2] = (1.0f - c) * zx - ys; mat[5] = (1.0f - c) * yz + xs; mat[8] = (1.0f - c) * v.z * v.z + c;
	}
	void rotate(float angle,float x,float y,float z) {
		rotate(angle,AVector3D(x,y,z));
	}
	void rotate_x(float angle) {
		float rad = angle * DEG2RAD;
		float c = (float)cos(rad);
		float s = (float)sin(rad);
		mat[0] = 1.0; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = c; mat[7] = -s;
		mat[2] = 0.0; mat[5] = s; mat[8] = c;
	}
	void rotate_y(float angle) {
		float rad = angle * DEG2RAD;
		float c = (float)cos(rad);
		float s = (float)sin(rad);
		mat[0] = c; mat[3] = 0.0; mat[6] = s;
		mat[1] = 0.0; mat[4] = 1.0; mat[7] = 0.0;
		mat[2] = -s; mat[5] = 0.0; mat[8] = c;
	}
	void rotate_z(float angle) {
		float rad = angle * DEG2RAD;
		float c = (float)cos(rad);
		float s = (float)sin(rad);
		mat[0] = c; mat[3] = -s; mat[6] = 0.0;
		mat[1] = s; mat[4] = c; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = 1.0;
	}
	
	void scale(const AVector3D &v) {
		mat[0] = v.x; mat[3] = 0.0; mat[6] = 0.0;
		mat[1] = 0.0; mat[4] = v.y; mat[7] = 0.0;
		mat[2] = 0.0; mat[5] = 0.0; mat[8] = v.z;
	}
    
    void scale(float x,float y,float z) {
        scale(AVector3D(x, y, z));
    }

    void orthonormalize() {
        AVector3D x(mat[0], mat[1], mat[2]);
        AVector3D y(mat[3], mat[4], mat[5]);
        AVector3D z;
        x.normalize();
        z   = x.cross(y);
        z.normalize();
        y   = z.cross(x);
        y.normalize();
        mat[0] = x.x; mat[3] = y.x; mat[6] = z.x;
        mat[1] = x.y; mat[4] = y.y; mat[7] = z.y;
        mat[2] = x.z; mat[5] = y.z; mat[8] = z.z;
    }

    float mat[9];
};

/*****************************************************************************/
/*                                                                           */
/* AMatrix4D                                                                 */
/*                                                                           */
/*****************************************************************************/

struct AMatrix4D {
	
    AMatrix4D() {
        mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
        mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
        mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    AMatrix4D(const AMatrix3D &m) {
        mat[0] = m[0]; mat[4] = m[3]; mat[8] = m[6]; mat[12] = 0.0;
        mat[1] = m[1]; mat[5] = m[4]; mat[9] = m[7]; mat[13] = 0.0;
        mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    AMatrix4D(const float *m) {
        mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
        mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
        mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
        mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
    }

    AMatrix4D(const AMatrix4D &m) {
        mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
        mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
        mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
        mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
    }
	
    AVector3D operator*(const AVector3D &v) const {
        AVector3D ret;
        ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12];
        ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13];
        ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14];
        return ret;
    }

    AVector4D operator*(const AVector4D &v) const {
        AVector4D ret;
        ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12] * v[3];
        ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13] * v[3];
        ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14] * v[3];
        ret[3] = mat[3] * v[0] + mat[7] * v[1] + mat[11] * v[2] + mat[15] * v[3];
        return ret;
    }

    AMatrix4D operator*(float f) const {
        AMatrix4D ret;
        ret[0] = mat[0] * f; ret[4] = mat[4] * f; ret[8] = mat[8] * f; ret[12] = mat[12] * f;
        ret[1] = mat[1] * f; ret[5] = mat[5] * f; ret[9] = mat[9] * f; ret[13] = mat[13] * f;
        ret[2] = mat[2] * f; ret[6] = mat[6] * f; ret[10] = mat[10] * f; ret[14] = mat[14] * f;
        ret[3] = mat[3] * f; ret[7] = mat[7] * f; ret[11] = mat[11] * f; ret[15] = mat[15] * f;
        return ret;
    }

    AMatrix4D operator*(const AMatrix4D &m) const {
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

    AMatrix4D operator+(const AMatrix4D &m) const {
        AMatrix4D ret;
        ret[0] = mat[0] + m[0]; ret[4] = mat[4] + m[4]; ret[8] = mat[8] + m[8]; ret[12] = mat[12] + m[12];
        ret[1] = mat[1] + m[1]; ret[5] = mat[5] + m[5]; ret[9] = mat[9] + m[9]; ret[13] = mat[13] + m[13];
        ret[2] = mat[2] + m[2]; ret[6] = mat[6] + m[6]; ret[10] = mat[10] + m[10]; ret[14] = mat[14] + m[14];
        ret[3] = mat[3] + m[3]; ret[7] = mat[7] + m[7]; ret[11] = mat[11] + m[11]; ret[15] = mat[15] + m[15];
        return ret;
    }

    AMatrix4D operator-(const AMatrix4D &m) const {
        AMatrix4D ret;
        ret[0] = mat[0] - m[0]; ret[4] = mat[4] - m[4]; ret[8] = mat[8] - m[8]; ret[12] = mat[12] - m[12];
        ret[1] = mat[1] - m[1]; ret[5] = mat[5] - m[5]; ret[9] = mat[9] - m[9]; ret[13] = mat[13] - m[13];
        ret[2] = mat[2] - m[2]; ret[6] = mat[6] - m[6]; ret[10] = mat[10] - m[10]; ret[14] = mat[14] - m[14];
        ret[3] = mat[3] - m[3]; ret[7] = mat[7] - m[7]; ret[11] = mat[11] - m[11]; ret[15] = mat[15] - m[15];
        return ret;
    }
	
    AMatrix4D &operator*=(float f) {
        return *this = *this * f;
    }

    AMatrix4D &operator*=(const AMatrix4D &m) {
        return *this = *this * m;
    }

    AMatrix4D &operator+=(const AMatrix4D &m) {
        return *this = *this + m;
    }

    AMatrix4D &operator-=(const AMatrix4D &m) {
        return *this = *this - m;
    }
	
    operator float*() {
        return mat;
    }

    operator const float*() const {
        return mat;
    }
	
    float &operator[](int i) {
        return mat[i];
    }

    const float operator[](int i) const {
        return mat[i];
    }
	
    AMatrix4D rotation() const {
        AMatrix4D ret;
        ret[0] = mat[0]; ret[4] = mat[4]; ret[8] = mat[8]; ret[12] = 0;
        ret[1] = mat[1]; ret[5] = mat[5]; ret[9] = mat[9]; ret[13] = 0;
        ret[2] = mat[2]; ret[6] = mat[6]; ret[10] = mat[10]; ret[14] = 0;
        ret[3] = 0; ret[7] = 0; ret[11] = 0; ret[15] = 1;
        return ret;
    }

    AMatrix4D transpose() const {
        AMatrix4D ret;
        ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[3];
        ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[7];
        ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[11];
        ret[3] = mat[12]; ret[7] = mat[13]; ret[11] = mat[14]; ret[15] = mat[15];
        return ret;
    }

    AMatrix4D transpose_rotation() const {
        AMatrix4D ret;
        ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[12];
        ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[13];
        ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[14];
        ret[3] = mat[3]; ret[7] = mat[7]; ret[14] = mat[14]; ret[15] = mat[15];
        return ret;
    }

    float det() const {
        float det;
        det = mat[0] * mat[5] * mat[10];
        det += mat[4] * mat[9] * mat[2];
        det += mat[8] * mat[1] * mat[6];
        det -= mat[8] * mat[5] * mat[2];
        det -= mat[4] * mat[1] * mat[10];
        det -= mat[0] * mat[9] * mat[6];
        return det;
    }

    AMatrix4D inverse() const {
        AMatrix4D ret;
        float idet = 1.0f / det();
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

    void zero() {
        mat[0] = 0.0; mat[4] = 0.0; mat[8 ] = 0.0; mat[12] = 0.0;
        mat[1] = 0.0; mat[5] = 0.0; mat[9 ] = 0.0; mat[13] = 0.0;
        mat[2] = 0.0; mat[6] = 0.0; mat[10] = 0.0; mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 0.0;
    }

    void identity() {
        mat[0] = 1.0; mat[4] = 0.0; mat[8 ] = 0.0; mat[12] = 0.0;
        mat[1] = 0.0; mat[5] = 1.0; mat[9 ] = 0.0; mat[13] = 0.0;
        mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    void direction(AVector3D &dir, AVector3D &up) {
        AVector3D f = dir;
        f.normalize();
        AVector3D r = f.cross(up);
        r.normalize();
        AVector3D u = r.cross(f);
        u.normalize();

        mat[0 ] = r.x; mat[4 ] = f.x; mat[8 ] = u.x; mat[12] = 0.0;
        mat[1 ] = r.y; mat[5 ] = f.y; mat[9 ] = u.y; mat[13] = 0.0;
        mat[2 ] = r.z; mat[6 ] = f.z; mat[10] = u.z; mat[14] = 0.0;
        mat[3 ] = 0.0; mat[7 ] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    void rotate(float angle, const AVector3D &axis) {
        float rad   = angle * DEG2RAD;
        float c     = (float)cos(rad);
        float s     = (float)sin(rad);
        AVector3D v = axis;
        v.normalize();
        float xy    = v.x * v.y;
        float yz    = v.y * v.z;
        float zx    = v.z * v.x;
        float xs    = v.x * s;
        float ys    = v.y * s;
        float zs    = v.z * s;
        mat[0] = (1.0f - c) * v.x * v.x + c;    mat[4] = (1.0f - c) * xy - zs;          mat[8] = (1.0f - c) * zx + ys;          mat[12] = 0.0;
        mat[1] = (1.0f - c) * xy + zs;          mat[5] = (1.0f - c) * v.y * v.y + c;    mat[9] = mat[6] = (1.0f - c) * yz - xs; mat[13] = 0.0;
        mat[2] = (1.0f - c) * zx - ys;          mat[6] = (1.0f - c) * yz + xs;          mat[10] = (1.0f - c) * v.z * v.z + c;   mat[14] = 0.0;
        mat[3] = 0.0;                           mat[7] = 0.0;                           mat[11] = 0.0;                          mat[15] = 1.0;
    }

    void rotate(float angle, float x, float y, float z) {
        rotate(angle, AVector3D(x, y, z));
    }

    void rotate_x(float angle) {
        float rad = angle * DEG2RAD;
        float c = (float)cos(rad);
        float s = (float)sin(rad);
        mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0;   mat[12] = 0.0;
        mat[1] = 0.0; mat[5] = c;   mat[9] = -s;    mat[13] = 0.0;
        mat[2] = 0.0; mat[6] = s;   mat[10] = c;    mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0;  mat[15] = 1.0;
    }

    void rotate_y(float angle) {
        float rad = angle * DEG2RAD;
        float c = (float)cos(rad);
        float s = (float)sin(rad);
        mat[0] = c;     mat[4] = 0.0; mat[8] = s;       mat[12] = 0.0;
        mat[1] = 0.0;   mat[5] = 1.0; mat[9] = 0.0;     mat[13] = 0.0;
        mat[2] = -s;    mat[6] = 0.0; mat[10] = c;      mat[14] = 0.0;
        mat[3] = 0.0;   mat[7] = 0.0; mat[11] = 0.0;    mat[15] = 1.0;
    }

    void rotate_z(float angle) {
        float rad = angle * DEG2RAD;
        float c = (float)cos(rad);
        float s = (float)sin(rad);
        mat[0] = c;     mat[4] = -s;    mat[8] = 0.0;   mat[12] = 0.0;
        mat[1] = s;     mat[5] = c;     mat[9] = 0.0;   mat[13] = 0.0;
        mat[2] = 0.0;   mat[6] = 0.0;   mat[10] = 1.0;  mat[14] = 0.0;
        mat[3] = 0.0;   mat[7] = 0.0;   mat[11] = 0.0;  mat[15] = 1.0;
    }

    void scale(const AVector3D &v) {
        mat[0] = v.x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
        mat[1] = 0.0; mat[5] = v.y; mat[9] = 0.0; mat[13] = 0.0;
        mat[2] = 0.0; mat[6] = 0.0; mat[10] = v.z; mat[14] = 0.0;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    void scale(float x,float y,float z) {
        scale(AVector3D(x,y,z));
    }

    void translate(const AVector3D &v) {
        mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = v.x;
        mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = v.y;
        mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = v.z;
        mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
    }

    void translate(float x, float y, float z) {
        translate(AVector3D(x,y,z));
    }

    void reflect(const AVector4D &plane) {
        float x = plane.x;
        float y = plane.y;
        float z = plane.z;
        float x2 = x * 2.0f;
        float y2 = y * 2.0f;
        float z2 = z * 2.0f;
        mat[0] = 1.0f - x * x2; mat[4] = -y * x2;       mat[8] = -z * x2;           mat[12] = -plane.w * x2;
        mat[1] = -x * y2;       mat[5] = 1.0f - y * y2; mat[9] = -z * y2;           mat[13] = -plane.w * y2;
        mat[2] = -x * z2;       mat[6] = -y * z2;       mat[10] = 1.0f - z * z2;    mat[14] = -plane.w * z2;
        mat[3] = 0.0;           mat[7] = 0.0;           mat[11] = 0.0;              mat[15] = 1.0;
    }

    void reflect(float x,float y,float z,float w) {
        reflect(AVector4D(x,y,z,w));
    }
	
    void perspective(float fov, float aspect, float znear, float zfar) {
        float sine, cotangent, deltaZ;
        float radians   = fov / 2 * DEG2RAD;

        deltaZ          = zfar - znear;
        sine            = sin(radians);
        if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
            return;
        }
        cotangent       = cos(radians) / sine;

        mat[0]          = cotangent / aspect;
        mat[5]          = cotangent;
        mat[10]         = -(zfar + znear) / deltaZ;
        mat[11]         = -1;
        mat[14]         = -2 * znear * zfar / deltaZ;
        mat[15]         = 0;
    }

    void look_at(AVector3D &eye, AVector3D &dir, AVector3D &up) {
        AVector3D x, y, z;
        AMatrix4D m0, m1;
        z = eye - dir;
        z.normalize();
        x = up.cross(z);
        x.normalize();
        y = z.cross(x);
        y.normalize();
        m0[0 ] = x.x; m0[4 ] = x.y; m0[8 ] = x.z; m0[12] = 0.0;
        m0[1 ] = y.x; m0[5 ] = y.y; m0[9 ] = y.z; m0[13] = 0.0;
        m0[2 ] = z.x; m0[6 ] = z.y; m0[10] = z.z; m0[14] = 0.0;
        m0[3 ] = 0.0; m0[7 ] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
        m1.translate(-eye);
        *this = m0 * m1;
    }

    void look_at(const float *eye,const float *dir,const float *up) {
        look_at(AVector3D(eye), AVector3D(dir), AVector3D(up));
    }

    void set(float  m0, float  m1, float  m2, float  m3,
             float  m4, float  m5, float  m6, float  m7,
             float  m8, float  m9, float m10, float m11,
             float m12, float m13, float m14, float m15) {

        mat[ 0] =  m0; mat[ 1] =  m1; mat[ 2] =  m2; mat[ 3] =  m3;
        mat[ 4] =  m4; mat[ 5] =  m5; mat[ 6] =  m6; mat[ 7] =  m7;
     	mat[ 8] =  m8; mat[ 9] =  m9; mat[10] = m10; mat[11] = m11;
     	mat[12] = m12; mat[13] = m13; mat[14] = m14; mat[15] = m15;
    }
	
    float mat[16];
};

inline AMatrix3D::AMatrix3D(const AMatrix4D &m) {
    mat[0] = m[0]; mat[3] = m[4]; mat[6] = m[8];
    mat[1] = m[1]; mat[4] = m[5]; mat[7] = m[9];
    mat[2] = m[2]; mat[5] = m[6]; mat[8] = m[10];
}

/*****************************************************************************/
/*                                                                           */
/* AQuaternion                                                               */
/*                                                                           */
/*****************************************************************************/

struct AQuaternion {
	
	AQuaternion() : x(0), y(0), z(0), w(1) { }
	AQuaternion(const AVector3D &dir, float angle) {
		set(dir,angle);
	}

	AQuaternion(float x, float y, float z, float angle) {
		set(x,y,z,angle);
	}

	AQuaternion(AMatrix3D &m) {
		float t		= m.mat[0] + m.mat[4] + m.mat[8];
		if(t > 0) {
			float s	= 0.5 / sqrt(t);
			w	= 0.25 / s;
			x	= ( m.mat[7] - m.mat[5] ) * s;
			y	= ( m.mat[2] - m.mat[6] ) * s;
			z	= ( m.mat[3] - m.mat[1] ) * s;
		} else {
			if ( m.mat[0] > m.mat[4] && m.mat[0] > m.mat[8] ) {
			  float s	= 2.0f * sqrt( 1.0f + m.mat[0] - m.mat[4] - m.mat[8] );
			  w = (m.mat[7] - m.mat[5] ) / s;
			  x = 0.25f * s;
			  y = (m.mat[1] + m.mat[3] ) / s;
			  z = (m.mat[2] + m.mat[6] ) / s;
			} else if (m.mat[4] > m.mat[8]) {
			  float s	= 2.0f * sqrt( 1.0f + m.mat[4] - m.mat[0] - m.mat[8] );
			  w = (m.mat[2] - m.mat[6] ) / s;
			  x = (m.mat[1] + m.mat[3] ) / s;
			  y = 0.25f * s;
			  z = (m.mat[5] + m.mat[7] ) / s;
			} else {
			  float s	= 2.0f * sqrt( 1.0f + m.mat[8] - m.mat[0] - m.mat[4] );
			  w = (m.mat[3] - m.mat[1] ) / s;
			  x = (m.mat[2] + m.mat[6] ) / s;
			  y = (m.mat[5] + m.mat[7] ) / s;
			  z = 0.25f * s;
			}
		}
	}
	
	operator float*() { return (float*)&x; }
	operator const float*() const { return (float*)&x; }
	
	float &operator[](int i) { return ((float*)&x)[i]; }
	const float operator[](int i) const { return ((float*)&x)[i]; }
	
	AQuaternion operator*(const AQuaternion &q) const {
		AQuaternion ret;
		ret.x = w * q.x + x * q.x + y * q.z - z * q.y;
		ret.y = w * q.y + y * q.w + z * q.x - x * q.z;
		ret.z = w * q.z + z * q.w + x * q.y - y * q.x;
		ret.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return ret;
	}
	
	void set(const AVector3D &dir,float angle) {
		float length = dir.length();
		if(length != 0.0) {
			length = 1.0f / length;
			float sinangle = (float)sin(angle * DEG2RAD / 2.0f);
			x = dir[0] * length * sinangle;
			y = dir[1] * length * sinangle;
			z = dir[2] * length * sinangle;
			w = (float)cos(angle * DEG2RAD / 2.0f);
		} else {
			x = y = z = 0.0;
			w = 1.0;
		}
	}
	void set(float x,float y,float z,float angle) {
		set(AVector3D(x,y,z),angle);
	}
	
	void slerp(const AQuaternion &q0,const AQuaternion &q1,float t) {
		float k0,k1,cosomega = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
		AQuaternion q;
		if(cosomega < 0.0) {
			cosomega = -cosomega;
			q.x = -q1.x;
			q.y = -q1.y;
			q.z = -q1.z;
			q.w = -q1.w;
		} else {
			q.x = q1.x;
			q.y = q1.y;
			q.z = q1.z;
			q.w = q1.w;
		}
		if(1.0 - cosomega > 1e-6) {
			float omega = (float)acos(cosomega);
			float sinomega = (float)sin(omega);
			k0 = (float)sin((1.0f - t) * omega) / sinomega;
			k1 = (float)sin(t * omega) / sinomega;
		} else {
			k0 = 1.0f - t;
			k1 = t;
		}
		x = q0.x * k0 + q.x * k1;
		y = q0.y * k0 + q.y * k1;
		z = q0.z * k0 + q.z * k1;
		w = q0.w * k0 + q.w * k1;
	}
	
	AMatrix3D to_matrix() const {
		AMatrix3D ret;
		float x2 = x + x;
		float y2 = y + y;
		float z2 = z + z;
		float xx = x * x2;
		float yy = y * y2;
		float zz = z * z2;
		float xy = x * y2;
		float yz = y * z2;
		float xz = z * x2;
		float wx = w * x2;
		float wy = w * y2;
		float wz = w * z2;

        ret[0] = 1.0f - (yy + zz);
        ret[3] = xy - wz;
        ret[6] = xz + wy;

        ret[1] = xy + wz;
        ret[4] = 1.0f - (xx + zz);
        ret[7] = yz - wx;

        ret[2] = xz - wy;
        ret[5] = yz + wx;
        ret[8] = 1.0f - (xx + yy);
		return ret;
	}
	
	union {
		struct {
            float x, y, z, w;
		};
		float q[4];
	};
};

/*****************************************************************************/
/*                                                                           */
/* APlane																	 */
/*                                                                           */
/*****************************************************************************/

struct APlane {
    APlane () {}
    APlane (AVector3D &v1, AVector3D &v2, AVector3D &v3) { set_points(v1, v2, v3);}

    float distance(const AVector3D &p) { return d + normal.dot(p);}

    void set_points(AVector3D &v1,  AVector3D &v2,  AVector3D &v3) {
        AVector3D aux1, aux2;
        aux1    = v1 - v2;
        aux2    = v3 - v2;
        normal  = aux2 * aux1;
        normal.normalize();
        point   = AVector3D(v2.x, v2.y, v2.z);
        d       = -(normal.dot(point));
    }

    AVector3D normal;
    AVector3D point;
    float d;
};

/*****************************************************************************/
/*                                                                           */
/* AABox                                                                     */
/*                                                                           */
/*****************************************************************************/

struct AABox {
    AABox ()                                                { corner = AVector3D(); set_box(corner, 1.0f, 1.0f, 1.0f);}
    AABox (AVector3D &corner, float x, float y, float z)    { set_box(corner, x, y, z);}
    AABox (AVector3D &corner, AVector3D &size)              { set_box(corner, size.x, size.y, size.z);}

    inline const AABox operator*(float f) {
        AVector3D c     = corner * f;
        AVector3D s     = size * f;
        return AABox(c, s);
    }

    inline const AABox operator*(AVector3D &v) {
        AVector3D c(corner.x * v.x, corner.y * v.y, corner.z * v.z);
        AVector3D s(size.x * v.x, size.y * v.y, size.z * v.z);
        return AABox(c, s);
    }

    void set_box(AVector3D &corner, float x, float y, float z) {
        this->corner = corner;
        if (x < 0.0) { x = -x; this->corner.x -= x;}
        if (y < 0.0) { y = -y; this->corner.y -= y;}
        if (z < 0.0) { z = -z; this->corner.z -= z;}
        size.x      = x;
        size.y      = y;
        size.z      = z;
    }

    AVector3D get_vertex_p(AVector3D &normal) {
        AVector3D res = corner;
        if (normal.x > 0) res.x += size.x;
        if (normal.y > 0) res.y += size.y;
        if (normal.z > 0) res.z += size.z;
        return(res);
    }

    AVector3D get_vertex_n(AVector3D &normal) {
        AVector3D res = corner;
        if (normal.x < 0) res.x += size.x;
        if (normal.y < 0) res.y += size.y;
        if (normal.z < 0) res.z += size.z;
        return(res);
    }

    AVector3D corner;   // position
    AVector3D size;     // size
};

#endif /* AMATH_H_HEADER_INCLUDED */
