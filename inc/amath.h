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

#define CLAMP(x, min, max)	((x < min) ? min : (x > max) ? max : x)
#define MIX(a, b, f) (a * (1 - f) + b * f)

#define SQR(a) (a * a)

#define RANGE(min, max) (min + ((max - min) * ((float)rand() / RAND_MAX)))

#include "math/vector2.h"
#include "math/vector3.h"
#include "math/vector4.h"

#include "math/matrix3.h"
#include "math/matrix4.h"

#include "math/quaternion.h"

#include "math/curve.h"
/*
#include "math/range.h"
#include "math/range3.h"
*/
#include "math/gaussian.h"
#include "math/perlin.h"

#include "math/plane.h"

#include "math/aabb.h"
#include "math/obb.h"

#include "math/ray.h"

namespace amath {
    inline void rotate(AVector2D &v, float angle, float x, float y) {
        AVector2D vNewPos;

        float cosTheta = (float)cos(angle);

        vNewPos.x   = (cosTheta + (1 - cosTheta) * x * x)   * v.x;
        vNewPos.x  += ((1 - cosTheta) * x * y)              * v.y;

        vNewPos.y   = ((1 - cosTheta) * x * y)              * v.x;
        vNewPos.y  += (cosTheta + (1 - cosTheta) * y * y)   * v.y;

        v           = vNewPos;
    }

    inline void rotate(AVector3D &v, float sinTheta, float cosTheta, float x, float y, float z) {
        AVector3D vNewPos;

        vNewPos.x   = (cosTheta + (1 - cosTheta) * x * x)       * v.x;
        vNewPos.x  += ((1 - cosTheta) * x * y - z * sinTheta)   * v.y;
        vNewPos.x  += ((1 - cosTheta) * x * z + y * sinTheta)   * v.z;

        vNewPos.y   = ((1 - cosTheta) * x * y + z * sinTheta)   * v.x;
        vNewPos.y  += (cosTheta + (1 - cosTheta) * y * y)       * v.y;
        vNewPos.y  += ((1 - cosTheta) * y * z - x * sinTheta)   * v.z;

        vNewPos.z   = ((1 - cosTheta) * x * z - y * sinTheta)   * v.x;
        vNewPos.z  += ((1 - cosTheta) * y * z + x * sinTheta)   * v.y;
        vNewPos.z  += (cosTheta + (1 - cosTheta) * z * z)       * v.z;

        v           = vNewPos;
    }

    inline void rotate(AVector3D &v, float angle, float x, float y, float z) {
        float cosTheta = (float)cos(angle);
        float sinTheta = (float)sin(angle);

        rotate(v, sinTheta, cosTheta, x, y, z);
    }
/*
    AVector3D closest_point_on_line(const AVector3D &a, const AVector3D &b, const AVector3D &p) {
        AVector3D c		= p - a;
        AVector3D v		= b - a;
        float d			= v.normalize();
        float t			= v.dot(c);
        if ( t < 0.0f )
            return a;
        if ( t > d )
            return b;

        v *= t;
        return ( a + v );
    }

    void ortogonalize(const AVector3D &v1, const AVector3D &v2) {
        AVector3D v2ProjV1  = closest_point_on_line( v1, -v1, v2 );
        AVector3D res       = v2 - v2ProjV1;
        res.normalize();

        *this   = res;
    }
*/
}

#endif /* AMATH_H_HEADER_INCLUDED */
