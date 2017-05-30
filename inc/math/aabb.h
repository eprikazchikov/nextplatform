#ifndef AABB_H_HEADER_INCLUDED
#define AABB_H_HEADER_INCLUDED

struct AABox {
    AABox() : pos(0.0f), size(1.0f)                                 { }
    AABox(const AVector3D &p, const AVector3D &s) : pos(p), size(s) { }

    bool intersect(const AVector3D &p, float r) {
        AVector3D min, max;
        box(min, max);

        float d = 0;
        float s = 0;

        for(int i = 0; i < 3; i++) {
            if (p[i] < min[i]) {
                s   = p[i] - min[i];
                d  += s * s;
            } else if (p[i] > max[i]) {
                s   = p[i] - max[i];
                d  += s * s;
            }
        }
        return d <= r * r;
    }

    inline const AABox operator*(float f) {
        AVector3D c     = pos * f;
        AVector3D s     = size * f;
        return AABox(c, s);
    }

    inline const AABox operator*(const AVector3D &v) {
        AVector3D p(pos * v);
        AVector3D s(size * v);
        return AABox(p, s);
    }

    inline const AABox operator*(const AMatrix4D &m) {
        const float *v  = (const float*)glm::value_ptr(m);
        AVector3D p = pos * AVector3D(v[0], v[5], v[10]);
        p          += AVector3D(v[12], v[13], v[14]);
        AVector3D s = size * AVector3D(v[0], v[5], v[10]);
        return AABox(p, s);
    }

    inline void setBox(const AVector3D &min, const AVector3D &max) {
        size    = max - min;
        pos     = min + size * 0.5f;
    }

    inline void box(AVector3D &min, AVector3D &max) const {
        min     = pos - size * 0.5f;
        max     = min + size;
    }

    inline AVector3D vertexP(AVector3D &normal) {
        AVector3D res = pos - size * 0.5f;
        if (normal.x > 0) res.x += size.x;
        if (normal.y > 0) res.y += size.y;
        if (normal.z > 0) res.z += size.z;
        return res;
    }

    inline AVector3D vertexN(AVector3D &normal) {
        AVector3D res = pos - size * 0.5f;
        if (normal.x < 0) res.x += size.x;
        if (normal.y < 0) res.y += size.y;
        if (normal.z < 0) res.z += size.z;
        return res;
    }

    AVector3D pos;
    AVector3D size;
};

#endif /* AABB_H_HEADER_INCLUDED */
