#include "math/amath.h"

AABox::AABox() :
    pos(0.0f),
    size(1.0f) {
}
AABox::AABox(const AVector3D &p, const AVector3D &s) :
    pos(p),
    size(s) {
}

bool AABox::intersect(const AVector3D &p, areal r) const {
    AVector3D min, max;
    box(min, max);

    areal d = 0;
    areal s = 0;

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

const AABox AABox::operator*(areal f) const {
    AVector3D c     = pos * f;
    AVector3D s     = size * f;
    return AABox(c, s);
}

const AABox AABox::operator*(const AVector3D &v) const {
    AVector3D p(pos * v);
    AVector3D s(size * v);
    return AABox(p, s);
}

const AABox AABox::operator*(const AMatrix4D &m) const {
    AVector3D p = pos * AVector3D(m[0], m[5], m[10]);
    p          += AVector3D(m[12], m[13], m[14]);
    AVector3D s = size * AVector3D(m[0], m[5], m[10]);
    return AABox(p, s);
}

void AABox::box(AVector3D &min, AVector3D &max) const {
    min     = pos - size * 0.5;
    max     = min + size;
}

void AABox::setBox(const AVector3D &min, const AVector3D &max) {
    size    = max - min;
    pos     = min + size * 0.5;
}
