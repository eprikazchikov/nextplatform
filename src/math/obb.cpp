#include "amath.h"

AOBox::AOBox() :
        pos(0.0),
        size(1.0),
        rot(AMatrix3D()),
        radius(0.5) {
}
AOBox::AOBox(const AVector3D &p, const AVector3D &s) :
        pos(p),
        size(s),
        rot(AMatrix3D()),
        radius(MAX(s.x, MAX(s.y, s.z))) {
}
AOBox::AOBox(const AVector3D &p, const AVector3D &s, const AMatrix3D &r) :
        pos(p),
        size(s),
        rot(r),
        radius(MAX(s.x, MAX(s.y, s.z))) {
}

const AOBox AOBox::operator*(areal f) {
    AVector3D c     = pos * f;
    AVector3D s     = size * f;
    return AOBox(c, s, rot);
}

const AOBox AOBox::operator*(const AVector3D &v) {
    AVector3D p(pos * v);
    AVector3D s(size * v);
    return AOBox(p, s, rot);
}

void AOBox::box(AVector3D &min, AVector3D &max) const {
    min     = pos - size * 0.5;
    max     = min + size;
}

void AOBox::setBox(const AVector3D &min, const AVector3D &max) {
    size    = max - min;
    pos     = min + size * 0.5;
    radius  = size.length();
}
