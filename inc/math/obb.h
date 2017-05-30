#ifndef OBB_H_HEADER_INCLUDED
#define OBB_H_HEADER_INCLUDED

#include <glm/glm.hpp>

struct AOBox {
    AOBox() : pos(0.0f), size(1.0f), radius(0.5f)                                                       { }
    AOBox(const AVector3D &p, const AVector3D &s) : pos(p), size(s), radius(glm::max(s.x, glm::max(s.y, s.z)))    { }
    AOBox(const AVector3D &p, const AVector3D &s, const AMatrix3D &r) : pos(p), size(s), rot(r)         { }

    const AOBox operator*(float f) {
        AVector3D c     = pos * f;
        AVector3D s     = size * f;
        return AOBox(c, s, rot);
    }

    const AOBox operator*(const AVector3D &v) {
        AVector3D p(pos * v);
        AVector3D s(size * v);
        return AOBox(p, s, rot);
    }

    void setBox(AVector3D &min, AVector3D &max) {
        size    = max - min;
        pos     = min + size * 0.5f;
        radius  = glm::max(size.x, glm::max(size.y, size.z));
    }

    void box(AVector3D &min, AVector3D &max) {
        min     = pos - size * 0.5f;
        max     = min + size;
    }

    void box(AVector3D *b, const AMatrix4D &m) {
        const float *v  = (const float*)glm::value_ptr(m);

        AVector3D min, max;
        box(min, max);

        AVector3D t(v[12], v[13], v[14]);

        AMatrix3D r;
        float *vr = (float*)glm::value_ptr(r);
        vr[0] = v[0]; vr[3] = v[4]; vr[6] = v[8];
        vr[1] = v[1]; vr[4] = v[5]; vr[7] = v[9];
        vr[2] = v[2]; vr[5] = v[6]; vr[8] = v[10];

        b[0]  = r * AVector3D(min.x, min.y, min.z) + t;
        b[1]  = r * AVector3D(min.x, min.y, max.z) + t;
        b[2]  = r * AVector3D(max.x, min.y, max.z) + t;
        b[3]  = r * AVector3D(max.x, min.y, min.z) + t;
        b[4]  = r * AVector3D(min.x, max.y, min.z) + t;
        b[5]  = r * AVector3D(min.x, max.y, max.z) + t;
        b[6]  = r * AVector3D(max.x, max.y, max.z) + t;
        b[7]  = r * AVector3D(max.x, max.y, min.z) + t;
    }

    float radius;
    AVector3D pos;
    AVector3D size;
    AMatrix3D rot;
};

#endif // OBB_H_HEADER_INCLUDED
