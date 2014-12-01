#ifndef AABB_H_HEADER_INCLUDED
#define AABB_H_HEADER_INCLUDED

struct AABox {
    AABox() : pos(0), size(1)                                       { }
    AABox(const AVector3D &p, const AVector3D &s) : pos(p), size(s) { }

    bool intersect(const AVector3D &p, float r) {
        AVector3D min, max;
        get_box(min, max);

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
        AVector3D p = pos * AVector3D(m[0], m[5], m[10]);
        p          += AVector3D(m[12], m[13], m[14]);
        AVector3D s = size * AVector3D(m[0], m[5], m[10]);
        return AABox(p, s);
    }

    inline void set_box(AVector3D &min, AVector3D &max) {
        size    = max - min;
        pos     = min + size * 0.5;
    }

    inline void get_box(AVector3D &min, AVector3D &max) const {
        min     = pos - size * 0.5;
        max     = min + size;
    }

    inline AVector3D get_vertex_p(AVector3D &normal) {
        AVector3D res = pos - size * 0.5;
        if (normal.x > 0) res.x += size.x;
        if (normal.y > 0) res.y += size.y;
        if (normal.z > 0) res.z += size.z;
        return res;
    }

    inline AVector3D get_vertex_n(AVector3D &normal) {
        AVector3D res = pos - size * 0.5;
        if (normal.x < 0) res.x += size.x;
        if (normal.y < 0) res.y += size.y;
        if (normal.z < 0) res.z += size.z;
        return res;
    }

    AVector3D pos;      // Delta position from AABB center
    AVector3D size;     // Size of AABB
};

#endif /* AABB_H_HEADER_INCLUDED */
