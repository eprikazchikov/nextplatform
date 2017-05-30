#ifndef AMATH_H_HEADER_INCLUDED
#define AMATH_H_HEADER_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <vector>

#define EPSILON 1e-6f
#define SQR(a) (a * a)

#define SGN(a) ((0 < a) - (a < 0));

#define RANGE(min, max) (min + ((max - min) * ((float)rand() / RAND_MAX)))

#include <glm/glm.hpp>
#include <glm/ext.hpp>

typedef glm::vec2   AVector2D;
typedef glm::vec3   AVector3D;
typedef glm::vec4   AVector4D;

typedef glm::quat   AQuaternion;

typedef glm::mat3   AMatrix3D;
typedef glm::mat4   AMatrix4D;

#include "math/curve.h"

#include "math/aabb.h"
#include "math/obb.h"

#include "math/plane.h"

#include "math/ray.h"

namespace amath {
    inline float distanceToLine(const AVector3D &a, const AVector3D &b, const AVector3D &p) {
        AVector3D v = b - a;
        AVector3D w = p - a;
        AVector3D c = glm::cross(w, v);
        return glm::length(c / glm::length(v));
    }

    inline float distanceToSegment(const AVector3D &a, const AVector3D &b, const AVector3D &p) {
        AVector3D v = b - a;
        AVector3D w = p - a;
        float c1    = glm::dot(w, v);
        if(c1 <= 0.0f) {
            return glm::length(w);
        }
        float c2    = glm::dot(v, v);
        if( c2 <= c1 ) {
            return glm::length(p - b);
        }
        AVector3D l = a + v * (c1 / c2);
        return glm::length(p - l);
    }

    inline AVector3D closestPointToLine(const AVector3D &a, const AVector3D &b, const AVector3D &p) {
        AVector3D c = p - a;
        AVector3D v = b - a;
        float d     = glm::length(v);
        v           = glm::normalize(v);
        float t     = glm::dot(v, c);
        if(t < 0.0f) {
            return a;
        }
        if(t > d) {
            return b;
        }
        v *= t;
        return (a + v);
    }
}

#endif /* AMATH_H_HEADER_INCLUDED */
